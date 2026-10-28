"""Build the GRZ Movement baseline and verify every packaged source payload."""
import hashlib
from pathlib import Path
import struct
import zipfile
from repack_lugu_no_motorbike import compress_literals, decompress, read_pbo


def main():
    repo = Path(__file__).resolve().parents[1]
    source = repo / 'GRZ_Movement'
    files = sorted(p for p in source.rglob('*') if p.is_file() and p.suffix in ('.cpp', '.c'))
    header = b'\0' + struct.pack('<5I', 0x56657273, 0, 0, 0, 0) + b'prefix\0GRZ_Movement\0\0'
    payload = bytearray()
    for path in files:
        raw = path.read_bytes()
        packed = compress_literals(raw)
        name = str(path.relative_to(source)).replace('/', '\\').encode()
        header += name + b'\0' + struct.pack('<5I', 0x43707273, len(raw), 0, 0, len(packed))
        payload.extend(packed)
    body = header + b'\0' + bytes(20) + payload
    pbo = body + b'\0' + hashlib.sha1(body).digest()
    _, entries = read_pbo(pbo)
    assert len(entries) == len(files)
    for (name, fields, packed), path in zip(entries, files):
        assert name.decode().replace('\\', '/') == path.relative_to(source).as_posix()
        assert decompress(packed, fields[1]) == path.read_bytes()
    target = repo / 'downloads/GRZ-Movement-v0.16-TEST.zip'
    with zipfile.ZipFile(target, 'w', zipfile.ZIP_DEFLATED) as archive:
        archive.writestr('@GRZ_Movement/addons/GRZ_Movement.pbo', pbo)
        archive.write(source / 'README.md', 'README.md')
        archive.write(source / 'PROVENANCE.md', 'PROVENANCE.md')
        for path in files:
            archive.write(path, 'source/GRZ_Movement/' + path.relative_to(source).as_posix())
    with zipfile.ZipFile(target) as archive:
        assert archive.testzip() is None
        assert archive.read('@GRZ_Movement/addons/GRZ_Movement.pbo') == pbo
    print('PASS: PBO checksum, all source payloads and ZIP verified. Runtime testing still required.')
    print(target)


if __name__ == '__main__':
    main()
