"""Insert a held-camera guard in the original FOV-revert handler."""
from pathlib import Path
import argparse
import difflib
import hashlib
import struct
import zipfile

from repack_lugu_no_motorbike import compress_literals, decompress, read_pbo


def build(repository, output):
    properties, entries = read_pbo((repository / "StormZz_Zoom.pbo").read_bytes())
    target = b"Scripts\\4_World\\OldZoom.c"
    entry = next(e for e in entries if e[0] == target)
    original = decompress(entry[2], entry[1][1])
    field_marker = b"modded class DayZPlayerImplement\r\n{\r\n"
    fields = b"\tprivate bool m_HKZoomHeld;\r\n\tprivate bool m_HKZoomHoldRaised;\r\n\tprivate bool m_HKZoomHoldReported;\r\n"
    marker = b"\t\t\t// normal movement cameras\t\t\t\r\n"
    guard = '''            // Preserve this mod's original raised-camera framing during a zoom hold.
            HumanInputController hkInput = GetInputController();
            bool hkZoom = hkInput && (hkInput.IsZoom() || hkInput.IsZoomToggle());
            if (!hkZoom)
            {
                m_HKZoomHeld = false;
                m_HKZoomHoldRaised = false;
                m_HKZoomHoldReported = false;
            }
            else if (!m_HKZoomHeld)
            {
                m_HKZoomHeld = true;
                m_HKZoomHoldRaised = hkInput.IsWeaponRaised() || m_MovementState.IsRaised();
                Print("[HKZOOMHOLD] captured_raised=" + m_HKZoomHoldRaised.ToString());
            }
            if (hkZoom && m_HKZoomHoldRaised && GetCurrentCommandID() == DayZPlayerConstants.COMMANDID_MOVE)
            {
                int hkStance = m_MovementState.m_iStanceIdx;
                if (hkStance == DayZPlayerConstants.STANCEIDX_CROUCH || hkStance == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
                {
                    if (!m_HKZoomHoldReported && hkStance == DayZPlayerConstants.STANCEIDX_CROUCH)
                    {
                        Print("[HKZOOMHOLD] preserving_crouch_camera=1");
                        m_HKZoomHoldReported = true;
                    }
                    return DayZPlayerCameras.DAYZCAMERA_3RD_CRO_RAISED;
                }
                if (hkStance == DayZPlayerConstants.STANCEIDX_ERECT || hkStance == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
                {
                    if (m_IsWeapon)
                        return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED;
                    return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED_MELEE;
                }
            }

'''.replace('\n', '\r\n').encode()
    if original.count(field_marker) != 1 or original.count(marker) != 1:
        raise ValueError("Original insertion markers changed")
    patched = original.replace(field_marker, field_marker + fields).replace(marker, marker + guard)
    assert patched.replace(fields, b"", 1).replace(guard, b"", 1) == original
    encoded = compress_literals(patched)
    meta = entry[1]
    replacement = (target, (meta[0], len(patched), meta[2], meta[3], len(encoded)), encoded)
    changed = [replacement if e[0] == target else e for e in entries]
    bob_path = repository / "HK_ZoomBobOnly/HKCrouchBobOnly.c"
    bob = bob_path.read_bytes()
    bob_encoded = compress_literals(bob)
    changed.append((b"Scripts\\4_World\\HKCrouchBobOnly.c", (0x43707273, len(bob), 0, 0, len(bob_encoded)), bob_encoded))
    header = properties + b"".join(n + b"\0" + struct.pack("<5I", *m) for n, m, _ in changed)
    body = header + b"\0" + bytes(20) + b"".join(d for _, _, d in changed)
    result = body + b"\0" + hashlib.sha1(body).digest()
    actual_props, actual = read_pbo(result)
    assert actual_props == properties and actual == changed
    assert decompress(replacement[2], replacement[1][1]) == patched
    assert decompress(actual[-1][2], actual[-1][1][1]) == bob
    movement = (repository / "lwp_movement.pbo").read_bytes()
    output.mkdir(parents=True, exist_ok=True)
    archive = output / "StormZz_Zoom-HoldCrouch-TEST.zip"
    patch = ''.join(difflib.unified_diff(original.decode().splitlines(True), patched.decode().splitlines(True), fromfile='original/OldZoom.c', tofile='patched/OldZoom.c'))
    with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr("addons/StormZz_Zoom.pbo", result)
        z.writestr("addons/lwp_movement.pbo", movement)
        z.writestr("source/OldZoom.c", patched)
        z.writestr("source/OldZoom.patch", patch)
        z.write(bob_path, "source/HKCrouchBobOnly.c")
        z.write(repository / "HK_ZoomHoldTransition/README.md", "README.md")
    with zipfile.ZipFile(archive) as z:
        assert z.testzip() is None
        assert z.read("addons/StormZz_Zoom.pbo") == result
        assert z.read("addons/lwp_movement.pbo") == movement
    print("PASS: original camera script recovered exactly after removing insertions; PBO and ZIP verified.")
    print(archive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("/workspace/artifacts"))
    args = parser.parse_args()
    build(Path(__file__).resolve().parents[1], args.output)
