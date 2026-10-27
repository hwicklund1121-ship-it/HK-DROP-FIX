"""Append a crouch-height-only script without altering original zoom entries."""
from pathlib import Path
import argparse
import hashlib
import struct
import zipfile

from repack_lugu_no_motorbike import compress_literals, decompress, read_pbo


def build(repository, output):
    original = (repository / "StormZz_Zoom.pbo").read_bytes()
    movement = (repository / "lwp_movement.pbo").read_bytes()
    properties, entries = read_pbo(original)
    source_path = repository / "HK_ZoomBobOnly/HKCrouchBobOnly.c"
    source = source_path.read_bytes()
    name = b"Scripts\\4_World\\HKCrouchBobOnly.c"
    if any(entry[0].lower() == name.lower() for entry in entries):
        raise ValueError("Use the original zoom PBO; patch entry already exists")
    if not any(entry[0] == b"Scripts\\4_World\\OldZoom.c" for entry in entries):
        raise ValueError("Original camera handler script not found")
    encoded = compress_literals(source)
    added = (name, (0x43707273, len(source), 0, 0, len(encoded)), encoded)
    all_entries = entries + [added]
    headers = properties + b"".join(n + b"\0" + struct.pack("<5I", *m) for n, m, _ in all_entries)
    body = headers + b"\0" + bytes(20) + b"".join(data for _, _, data in all_entries)
    result = body + b"\0" + hashlib.sha1(body).digest()
    actual_properties, actual = read_pbo(result)
    assert actual_properties == properties
    assert actual[:-1] == entries
    assert actual[-1][0] == name
    assert decompress(actual[-1][2], actual[-1][1][1]) == source
    read_pbo(movement)
    output.mkdir(parents=True, exist_ok=True)
    archive = output / "StormZz_Zoom-CrouchBobOnly-TEST.zip"
    with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as zipped:
        zipped.writestr("addons/StormZz_Zoom.pbo", result)
        zipped.writestr("addons/lwp_movement.pbo", movement)
        zipped.write(source_path, "source/Scripts/4_World/HKCrouchBobOnly.c")
        zipped.write(repository / "HK_ZoomBobOnly/README.md", "README.md")
    with zipfile.ZipFile(archive) as zipped:
        assert zipped.testzip() is None
        assert zipped.read("addons/StormZz_Zoom.pbo") == result
        assert zipped.read("addons/lwp_movement.pbo") == movement
    print(f"PASS: all {len(entries)} original zoom entries/metadata preserved; one bob-only script added.")
    print(f"Zoom PBO SHA256: {hashlib.sha256(result).hexdigest()}")
    print(archive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("/workspace/artifacts"))
    arguments = parser.parse_args()
    build(Path(__file__).resolve().parents[1], arguments.output)
