"""Replace old zoom camera-selection script, retaining the original config."""
from pathlib import Path
import argparse
import hashlib
import struct
import zipfile

from repack_lugu_no_motorbike import compress_literals, decompress, read_pbo


def build(repository, output):
    original = (repository / "StormZz_Zoom.pbo").read_bytes()
    movement = (repository / "lwp_movement.pbo").read_bytes()
    props, entries = read_pbo(original)
    source = (repository / "HK_ZoomStable/OldZoom.c").read_bytes()
    target = b"Scripts\\4_World\\OldZoom.c"
    matches = [entry for entry in entries if entry[0] == target]
    if len(matches) != 1:
        raise ValueError("Expected exactly one original OldZoom.c entry")
    config = next(entry for entry in entries if entry[0] == b"config.bin")
    decoded_config = decompress(config[2], config[1][1]) if config[1][0] == 0x43707273 else config[2]
    if b"StormZz_Zoom" not in decoded_config:
        raise ValueError("Unexpected zoom config")
    if b"override int CameraHandler" in source:
        raise ValueError("The replacement must retain inherited camera selection")
    encoded = compress_literals(source)
    metadata = matches[0][1]
    replacement = (target, (0x43707273, len(source), metadata[2], metadata[3], len(encoded)), encoded)
    changed = [replacement if entry[0] == target else entry for entry in entries]
    header = props + b"".join(name + b"\0" + struct.pack("<5I", *fields) for name, fields, _ in changed)
    body = header + b"\0" + bytes(20) + b"".join(data for _, _, data in changed)
    result = body + b"\0" + hashlib.sha1(body).digest()
    actual_props, actual = read_pbo(result)
    assert props == actual_props and len(actual) == len(entries)
    for before, after in zip(entries, actual):
        assert after == (replacement if before[0] == target else before)
    assert decompress(replacement[2], replacement[1][1]) == source
    read_pbo(movement)
    output.mkdir(parents=True, exist_ok=True)
    archive = output / "StormZz_Zoom-StableCamera-TEST.zip"
    with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as zipped:
        zipped.writestr("addons/StormZz_Zoom.pbo", result)
        zipped.writestr("addons/lwp_movement.pbo", movement)
        zipped.writestr("source/Scripts/4_World/OldZoom.c", source)
        zipped.write(repository / "HK_ZoomStable/README.md", "README.md")
    with zipfile.ZipFile(archive) as zipped:
        assert zipped.testzip() is None
        assert zipped.read("addons/StormZz_Zoom.pbo") == result
        assert zipped.read("addons/lwp_movement.pbo") == movement
    print("PASS: one zoom script replaced; config/prefix unchanged; original movement PBO verified.")
    print(f"Zoom PBO SHA256: {hashlib.sha256(result).hexdigest()}")
    print(archive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("/workspace/artifacts"))
    arguments = parser.parse_args()
    build(Path(__file__).resolve().parents[1], arguments.output)
