"""Build the motorcycle-selector isolation test from the original Luigi PBO.

Only the AGR payload changes; one local vehicle AGF entry is added. Other
archive entries and prefix properties are preserved byte for byte.
"""
from pathlib import Path
import argparse
import hashlib
import re
import struct
import zipfile


def read_cstring(data, position):
    end = data.index(b"\0", position)
    return data[position:end], end + 1


def read_pbo(data):
    position = 0
    headers = []
    properties = b""
    while True:
        start = position
        name, position = read_cstring(data, position)
        fields = struct.unpack_from("<5I", data, position)
        position += 20
        if not name and fields[0] == 0x56657273:
            while True:
                key, position = read_cstring(data, position)
                if not key:
                    break
                _, position = read_cstring(data, position)
            if properties:
                raise ValueError("Multiple property headers")
            properties = data[start:position]
            continue
        if not name:
            break
        headers.append((name, fields))
    entries = []
    for name, fields in headers:
        end = position + fields[4]
        entries.append((name, fields, data[position:end]))
        position = end
    if data[position:] != b"\0" + hashlib.sha1(data[:position]).digest():
        raise ValueError("PBO checksum mismatch or unexpected trailing data")
    return properties, entries


def decompress(data, size):
    ring = bytearray(b" " * 4096)
    cursor = 4078
    position = 0
    flags = 0
    output = bytearray()
    while len(output) < size:
        flags >>= 1
        if not flags & 256:
            flags = data[position] | 0xFF00
            position += 1
        if flags & 1:
            values = [data[position]]
            position += 1
            distance = None
        else:
            lo, hi = data[position:position + 2]
            position += 2
            distance = lo | ((hi & 240) << 4)
            values = range((hi & 15) + 3)
        for literal in values:
            value = literal if distance is None else ring[(cursor - distance) & 4095]
            output.append(value)
            ring[cursor] = value
            cursor = (cursor + 1) & 4095
    if len(output) != size or len(data) - position != 4:
        raise ValueError("Invalid compressed payload size")
    if struct.unpack("<I", data[position:])[0] != sum(output) & 0xFFFFFFFF:
        raise ValueError("Compressed payload checksum mismatch")
    return bytes(output)


def build(repository, destination):
    original = (repository / "LUGUINSTADROPFIX.pbo").read_bytes()
    properties, entries = read_pbo(original)
    graph_entries = [entry for entry in entries if entry[0].lower().endswith(b".agr")]
    if len(graph_entries) != 1:
        raise ValueError("Expected exactly one Luigi AGR")
    graph_name, fields, stored = graph_entries[0]
    if fields[0] != 0x43707273:
        raise ValueError("Expected the original compressed Luigi AGR")
    graph = decompress(stored, fields[1])
    # Preserve the existing GUID, as Luigi already does for its local locomotion
    # sheet, and change only this resource's path in GraphFilesResourceNames.
    old = b"DZ/anims/workspaces/player/player_main/Vehicles.agf"
    prefix = b"LUGUINSTADROPFIX\\IO2JF\\GJDH4KD\\FGHJDGH\\I3FJ4\\GJKL2H"
    vehicle_name = graph_name.rsplit(b"\\", 1)[0] + b"\\HK_NoMotorbike_Vehicles.agf"
    vehicle_resource = prefix + b"\\" + vehicle_name
    if graph.count(old) != 1:
        raise ValueError("Expected exactly one original vehicle graph reference")
    patched_graph = graph.replace(old, vehicle_resource)
    vehicle_graph = (repository / "experiments/no_motorbike_graph/vehicles.agf").read_bytes()
    if len(re.findall(rb'^\s*(?:StartCondition|Condition) "false"$', vehicle_graph, re.M)) < 7:
        raise ValueError("Missing experimental disabled selectors")
    if any(entry[0].lower() == vehicle_name.lower() for entry in entries):
        raise ValueError("Experimental entry already exists")
    replacement = (graph_name, (0, len(patched_graph), 0, 0, len(patched_graph)), patched_graph)
    patched = [replacement if entry[0] == graph_name else entry for entry in entries]
    patched.append((vehicle_name, (0, len(vehicle_graph), 0, 0, len(vehicle_graph)), vehicle_graph))
    headers = properties
    payload = bytearray()
    for name, metadata, content in patched:
        headers += name + b"\0" + struct.pack("<5I", *metadata)
        payload.extend(content)
    body = headers + b"\0" + bytes(20) + payload
    result = body + b"\0" + hashlib.sha1(body).digest()
    actual_properties, actual = read_pbo(result)
    assert actual_properties == properties
    assert len(actual) == len(entries) + 1
    for before, after in zip(entries, actual):
        assert after == (replacement if before[0] == graph_name else before)
    assert actual[-1][0] == vehicle_name and actual[-1][2] == vehicle_graph
    assert old not in patched_graph and vehicle_resource in patched_graph
    destination.mkdir(parents=True, exist_ok=True)
    archive = destination / "HK_LUGU_NoMotorbike-TEST.zip"
    readme = (repository / "experiments/no_motorbike_graph/PBO_TEST.md").read_bytes()
    with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as zipped:
        zipped.writestr("@LUGU_NoMotorbike_TEST/addons/LUGUINSTADROPFIX.pbo", result)
        zipped.writestr("README.md", readme)
        zipped.writestr("source/vehicles.agf", vehicle_graph)
        zipped.writestr("source/luigi_graph.agr", patched_graph)
    with zipfile.ZipFile(archive) as zipped:
        assert zipped.testzip() is None
        assert zipped.read("@LUGU_NoMotorbike_TEST/addons/LUGUINSTADROPFIX.pbo") == result
    print(f"Verified {len(entries) - 1} unchanged archive entries; one AGR replaced and one AGF added.")
    print(f"Original SHA256: {hashlib.sha256(original).hexdigest()}")
    print(f"Test PBO SHA256: {hashlib.sha256(result).hexdigest()}")
    print(archive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("/workspace/artifacts"))
    args = parser.parse_args()
    build(Path(__file__).resolve().parents[1], args.output)
