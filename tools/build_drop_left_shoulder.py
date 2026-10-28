"""Package a shoulder-only addon with unchanged working spin/trace PBOs."""
from pathlib import Path
import argparse
import hashlib
import struct
import zipfile

from repack_lugu_no_motorbike import compress_literals, decompress, read_pbo


def build(repository, output):
    root = repository / "HK_DropLeftShoulder"
    files = [(b"config.cpp", root / "config.cpp"), (b"scripts\\4_World\\HKDropLeftShoulder.c", root / "scripts/4_World/HKDropLeftShoulder.c")]
    header = b"\0" + struct.pack("<5I", 0x56657273, 0, 0, 0, 0) + b"prefix\0HK_DropLeftShoulder\0\0"
    payload = bytearray()
    for name, path in files:
        data = path.read_bytes()
        encoded = compress_literals(data)
        header += name + b"\0" + struct.pack("<5I", 0x43707273, len(data), 0, 0, len(encoded))
        payload.extend(encoded)
    body = header + b"\0" + bytes(20) + payload
    camera_pbo = body + b"\0" + hashlib.sha1(body).digest()
    _, entries = read_pbo(camera_pbo)
    assert len(entries) == len(files)
    for (name, metadata, stored), (expected, path) in zip(entries, files):
        assert name == expected and decompress(stored, metadata[1]) == path.read_bytes()
    with zipfile.ZipFile(repository / "downloads/LUGU-BackRiseClip-TEST.zip") as z:
        luigi = z.read("addons/LUGUINSTADROPFIX.pbo")
        trace = z.read("addons/HK_Trace.pbo")
    output.mkdir(parents=True, exist_ok=True)
    archive = output / "LUGU-BackRise-LeftShoulder-TEST.zip"
    with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr("addons/LUGUINSTADROPFIX.pbo", luigi)
        z.writestr("addons/HK_Trace.pbo", trace)
        z.writestr("addons/HK_DropLeftShoulder.pbo", camera_pbo)
        z.write(root / "README.md", "README.md")
        for _, path in files:
            z.write(path, "source/HK_DropLeftShoulder/" + str(path.relative_to(root)))
    with zipfile.ZipFile(archive) as z:
        assert z.testzip() is None
        assert z.read("addons/LUGUINSTADROPFIX.pbo") == luigi
        assert z.read("addons/HK_Trace.pbo") == trace
        assert z.read("addons/HK_DropLeftShoulder.pbo") == camera_pbo
    print('PASS: working spin/trace PBOs byte-identical; optional shoulder addon payloads/checksums verified.')
    print(archive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path('/workspace/artifacts'))
    args = parser.parse_args()
    build(Path(__file__).resolve().parents[1], args.output)
