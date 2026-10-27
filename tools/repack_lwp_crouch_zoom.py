"""Add the crouch eye-zoom script to the original movement PBO."""
from pathlib import Path
import argparse
import hashlib
import struct
import zipfile

from repack_lugu_no_motorbike import compress_literals, decompress, read_pbo


def build(repository, output):
    original = (repository / "lwp_movement.pbo").read_bytes()
    properties, entries = read_pbo(original)
    source_path = repository / "HK_CrouchZoom/scripts/4_World/HKCrouchZoom.c"
    source = source_path.read_bytes()
    name = b"scripts\\4_World\\HKCrouchZoom.c"
    if any(entry[0].lower() == name.lower() for entry in entries):
        raise ValueError("Zoom script already exists; use the original movement PBO")
    # Confirm the existing config loads this script directory. Its config.bin
    # remains untouched; the separate addon config is deliberately not added.
    config = next(entry for entry in entries if entry[0] == b"config.bin")
    config_data = decompress(config[2], config[1][1]) if config[1][0] == 0x43707273 else config[2]
    if b"lwp_movement/scripts/4_World" not in config_data:
        raise ValueError("Existing world script directory is not registered")
    encoded = compress_literals(source)
    new_entry = (name, (0x43707273, len(source), 0, 0, len(encoded)), encoded)
    all_entries = entries + [new_entry]
    headers = properties + b"".join(n + b"\0" + struct.pack("<5I", *m) for n, m, _ in all_entries)
    body = headers + b"\0" + bytes(20) + b"".join(data for _, _, data in all_entries)
    result = body + b"\0" + hashlib.sha1(body).digest()
    actual_properties, actual = read_pbo(result)
    assert actual_properties == properties
    assert actual[:-1] == entries
    assert actual[-1][0] == name
    assert decompress(actual[-1][2], actual[-1][1][1]) == source
    output.mkdir(parents=True, exist_ok=True)
    archive = output / "lwp_movement-CrouchZoom-TEST-v2.zip"
    with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as zipped:
        zipped.writestr("addons/lwp_movement.pbo", result)
        zipped.write(repository / "HK_CrouchZoom/LWP_TEST.md", "README.md")
        zipped.write(source_path, "source/scripts/4_World/HKCrouchZoom.c")
    with zipfile.ZipFile(archive) as zipped:
        assert zipped.testzip() is None
        assert zipped.read("addons/lwp_movement.pbo") == result
    print(f"PASS: {len(entries)} existing entries unchanged; zoom script added and decoded back to source.")
    print(f"PBO SHA256: {hashlib.sha256(result).hexdigest()}")
    print(archive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("/workspace/artifacts"))
    arguments = parser.parse_args()
    build(Path(__file__).resolve().parents[1], arguments.output)
