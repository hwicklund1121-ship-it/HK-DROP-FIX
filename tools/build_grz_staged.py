"""Build separately installable GRZ stability stages; never bundle legacy mods."""
from pathlib import Path
import hashlib
import json
import struct
import zipfile
from repack_lugu_no_motorbike import compress_literals, decompress, read_pbo


def make_pbo(module):
    files = sorted(p for p in module.rglob('*') if p.is_file())
    header = b'\0' + struct.pack('<5I', 0x56657273, 0, 0, 0, 0) + b'prefix\0' + module.name.encode() + b'\0\0'
    payload = bytearray()
    for path in files:
        data = path.read_bytes()
        encoded = compress_literals(data)
        name = path.relative_to(module).as_posix().replace('/', '\\').encode()
        header += name + b'\0' + struct.pack('<5I', 0x43707273, len(data), 0, 0, len(encoded))
        payload.extend(encoded)
    body = header + b'\0' + bytes(20) + payload
    result = body + b'\0' + hashlib.sha1(body).digest()
    _, entries = read_pbo(result)
    assert len(entries) == len(files)
    for (name, metadata, stored), path in zip(entries, files):
        assert name.decode().replace('\\', '/') == path.relative_to(module).as_posix()
        assert decompress(stored, metadata[1]) == path.read_bytes()
    return result


def main():
    repo = Path(__file__).resolve().parents[1]
    root = repo / 'stability/GRZ_Staged'
    stages = sorted(p for p in root.iterdir() if p.is_dir() and p.name[:2].isdigit())
    output = repo / 'downloads/GRZ-Staged-Stability-TEST.zip'
    output.parent.mkdir(parents=True, exist_ok=True)
    manifest = {'purpose': 'Controlled staged isolation; not a proven crash fix', 'pbo_files': []}
    with zipfile.ZipFile(output, 'w', zipfile.ZIP_DEFLATED) as archive:
        for stage in stages:
            module = next(stage.iterdir())
            data = make_pbo(module)
            relative = stage.name + '/@' + module.name + '/addons/' + module.name + '.pbo'
            archive.writestr(relative, data)
            manifest['pbo_files'].append({'path': relative, 'sha256': hashlib.sha256(data).hexdigest()})
        for path in sorted(root.rglob('*')):
            if not path.is_file():
                continue
            relative = path.relative_to(root)
            if relative.parts[0] == 'docs' or path.name == 'README.md':
                archive.write(path, relative.as_posix())
            archive.write(path, 'source/stability/GRZ_Staged/' + relative.as_posix())
        for name in ('build_grz_staged.py', 'repack_lugu_no_motorbike.py'):
            archive.write(repo / 'tools' / name, 'source/tools/' + name)
        archive.writestr('BUILD-MANIFEST.json', json.dumps(manifest, indent=2) + '\n')
    with zipfile.ZipFile(output) as archive:
        assert archive.testzip() is None
        for entry in manifest['pbo_files']:
            assert hashlib.sha256(archive.read(entry['path'])).hexdigest() == entry['sha256']
    print('PASS: six independent stage PBOs verified; source, evidence notes and SHA256 deployment manifest included.')
    print(output)


if __name__ == '__main__':
    main()
