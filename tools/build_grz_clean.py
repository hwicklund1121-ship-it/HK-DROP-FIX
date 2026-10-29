"""Build a clean GRZ package; reject obsolete scripts instead of merging them."""
from pathlib import Path
import hashlib
import json
import struct
import subprocess
import zipfile

from repack_lugu_no_motorbike import compress_literals, decompress, read_pbo


EXPECTED = {
    "config.cpp",
    "gui/layouts/grz_camera.layout",
    "scripts/3_Game/GRZCameraPreferences.c",
    "scripts/4_World/GRZEyeZoom.c",
    "scripts/4_World/GRZMovement.c",
    "scripts/4_World/GRZNoObjectWeaponLift.c",
    "scripts/4_World/GRZRunningLean.c",
    "scripts/5_Mission/GRZCameraMenu.c",
}


def main():
    repo = Path(__file__).resolve().parents[1]
    root = repo / "GRZ_Movement"
    actual = {
        p.relative_to(root).as_posix()
        for p in root.rglob("*")
        if p.is_file() and p.suffix.lower() in {".cpp", ".bin", ".c", ".layout"}
    }
    if actual != EXPECTED:
        raise RuntimeError(f"Unexpected/missing addon files: {actual ^ EXPECTED}")
    files = {name: (root / name).read_bytes() for name in sorted(EXPECTED)}
    for name, raw in files.items():
        if name.endswith(".c"):
            for flags in ([], ["-DSERVER"]):
                result = subprocess.run(
                    ["cpp", "-P", "-x", "c", *flags, str(root / name)],
                    check=True, capture_output=True,
                )
                if flags and name.endswith(("GRZEyeZoom.c", "GRZCameraPreferences.c", "GRZCameraMenu.c")):
                    assert not result.stdout.strip()
    assert sum(b"override int GetEyeZoomLevel" in data for data in files.values()) == 1
    header = b"\0" + struct.pack("<5I", 0x56657273, 0, 0, 0, 0) + b"prefix\0GRZ_Movement\0\0"
    payload = bytearray()
    for name, raw in files.items():
        packed = compress_literals(raw)
        header += name.replace("/", "\\").encode() + b"\0"
        header += struct.pack("<5I", 0x43707273, len(raw), 0, 0, len(packed))
        payload.extend(packed)
    body = header + b"\0" + bytes(20) + payload
    pbo = body + b"\0" + hashlib.sha1(body).digest()
    _, entries = read_pbo(pbo)
    assert len(entries) == len(files)
    for name, metadata, stored in entries:
        assert decompress(stored, metadata[1]) == files[name.decode().replace("\\", "/")]
    digest = hashlib.sha256(pbo).hexdigest()
    notes = """GRZ CLEAN MOVEMENT + FOV

Use the supplied prebuilt PBO. This package excludes the six obsolete camera
scripts found in the uploaded mixed PBO and contains exactly one eye-zoom policy.
Movement remains 0.06 / 100 / 0.14 / 17 / 6.5; original sprint lean remains.
FOV live preview/settings and focused eye zoom remain. Native weapon obstruction
remains. No camera-distance/HK recovery, legacy camera selection, sprint framing,
bob corrections, zoom activation carry-over or old camera trace is packaged.

STOP server and client. Back up the existing GRZ PBO outside ALL loaded addon
folders. Replace the ENTIRE GRZ_Movement.pbo on both sides with the supplied file.
Do not merge source folders or pack this over an old extracted addon. If editing
source, extract into an EMPTY folder and use the strict included builder.
Ensure only one copy of GRZ scripts is loaded, including repacked shared addons.
Sign through your normal workflow if required. Restart both.

Verify installed files on BOTH machines from PowerShell:
  Get-FileHash "C:\\path\\to\\GRZ_Movement.pbo" -Algorithm SHA256
Both hashes must match SHA256.txt. Verify after deployment, not only in staging.

Keep movement trace v3 loaded for the short comparison and send both new trace
logs. Repeat the same Shift+W / Q+A / E+D sequence within two minutes after joining.
This fixes the demonstrated mixed package; it is NOT proof the underlying client
position corrections are resolved. Do not change server correction thresholds.
The uploaded PBO was not identified as client/server, so their equality was not
established. If this still jumps, test identical native-turning behavior next.

Validation: strict file inventory, one zoom override, PBO/source checksums,
client/SERVER preprocessing and archive integrity. No engine/runtime test here.
"""
    manifest = {
        "pbo_sha256": digest,
        "files": {name: hashlib.sha256(raw).hexdigest() for name, raw in files.items()},
    }
    target = repo / "downloads/GRZ-Clean-Movement-FOV-Test.zip"
    with zipfile.ZipFile(target, "w", zipfile.ZIP_DEFLATED) as archive:
        archive.writestr("@GRZ_Movement/addons/GRZ_Movement.pbo", pbo)
        archive.writestr("SHA256.txt", digest + "  GRZ_Movement.pbo\n")
        archive.writestr("BUILD-MANIFEST.json", json.dumps(manifest, indent=2) + "\n")
        archive.writestr("INSTALL.txt", notes)
        for name, raw in files.items():
            archive.writestr("source/GRZ_Movement/" + name, raw)
        for name in ("build_grz_clean.py", "repack_lugu_no_motorbike.py"):
            archive.write(repo / "tools" / name, "source/tools/" + name)
    with zipfile.ZipFile(target) as archive:
        assert archive.testzip() is None
        assert archive.read("@GRZ_Movement/addons/GRZ_Movement.pbo") == pbo
    print(f"PASS: strict inventory, payloads, preprocessing and ZIP. SHA256 {digest}")
    print(target)


if __name__ == "__main__":
    main()
