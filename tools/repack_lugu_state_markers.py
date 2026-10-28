"""Observe internal states of the prior recovery candidate without retiming it."""
from pathlib import Path
import argparse
import hashlib
import struct
import zipfile

from repack_lugu_no_motorbike import compress_literals, decompress, read_pbo


def write_pbo(properties, entries):
    header = properties + b"".join(n + b"\0" + struct.pack("<5I", *m) for n, m, _ in entries)
    body = header + b"\0" + bytes(20) + b"".join(d for _, _, d in entries)
    return body + b"\0" + hashlib.sha1(body).digest()


def replace_payload(properties, entries, name, source):
    old = next(e for e in entries if e[0] == name)
    encoded = compress_literals(source)
    m = old[1]
    replacement = (name, (0x43707273, len(source), m[2], m[3], len(encoded)), encoded)
    changed = [replacement if e[0] == name else e for e in entries]
    result = write_pbo(properties, changed)
    p, verified = read_pbo(result)
    assert p == properties and verified == changed
    assert decompress(replacement[2], replacement[1][1]) == source
    return result


def build(repository, output):
    with zipfile.ZipFile(repository / "downloads/LUGU-RecoveryLateEntry-TEST.zip") as z:
        prior = z.read("addons/LUGUINSTADROPFIX.pbo")
    props, entries = read_pbo(prior)
    n, m, d = next(e for e in entries if e[0].lower().endswith(b".agf"))
    original = decompress(d, m[1]).decode()
    source = original
    additions = []
    def add_after(marker, addition):
        nonlocal source
        if source.count(marker) != 1:
            raise ValueError(f"Expected unique marker: {marker}")
        source = source.replace(marker, marker + addition)
        additions.append(addition)

    add_after('    AnimSrcNodeStateMachine HKCancelRecoverySTM {\n', '     Tags {\n      "TagHKRecoveryGraphV1"\n     }\n')
    for state, tag in [('Original', 'TagHKRecoveryOriginal'), ('ProneCancel', 'TagHKRecoveryProneCancel')]:
        # Scope the marker to this specific recovery machine by its exact EditorPos.
        pos = '-80 40' if state == 'Original' else '-78 40'
        marker = f'      AnimSrcNodeState {state} {{\n       EditorPos {pos}\n'
        replacement = f'      AnimSrcNodeState {state} {{\n       Tags {{\n        "{tag}"\n       }}\n       EditorPos {pos}\n'
        if source.count(marker) != 1:
            raise ValueError(f"Expected one {state} state")
        addition = f'       Tags {{\n        "{tag}"\n       }}\n'
        source = source.replace(marker, replacement)
        additions.append(addition)
    add_after('      AnimSrcNodeState BackRise {\n       Tags {\n', '        "TagHKRecoveryBackRise"\n')
    for state, tag in [('Cancel', 'TagHKCancelParent'), ('CancelPoB', 'TagHKCancelPoBParent')]:
        add_after(f'      AnimSrcNodeState {state} {{\n       Tags {{\n', f'        "{tag}"\n')
    stripped = source
    for addition in additions:
        assert stripped.count(addition) == 1
        stripped = stripped.replace(addition, '', 1)
    assert stripped == original
    luigi = replace_payload(props, entries, n, source.encode())

    trace_props, trace_entries = read_pbo((repository / "HK_Trace.pbo").read_bytes())
    tn, tm, td = next(e for e in trace_entries if e[0].lower().endswith(b".c"))
    trace = (decompress(td, tm[1]) if tm[0] == 0x43707273 else td).decode()
    marker = '            m_HKTraceNames.Insert("TagThrowingOut");'
    new_tags = ['TagHKRecoveryGraphV1', 'TagHKRecoveryOriginal', 'TagHKRecoveryProneCancel', 'TagHKRecoveryBackRise', 'TagHKCancelParent', 'TagHKCancelPoBParent']
    assert trace.count(marker) == 1
    trace = trace.replace(marker, marker + ''.join(f'\n            m_HKTraceNames.Insert("{tag}");' for tag in new_tags))
    bind = '                m_HKTraceTags.Insert(tag);'
    assert trace.count(bind) == 1
    trace = trace.replace(bind, bind + '\n                Print("[HKTRACE] bound_tag=" + m_HKTraceNames[bindIndex] + " id=" + tag.ToString());')
    trace = trace.replace('>= 1200', '>= 2400').replace('== 1200', '== 2400')
    trace = trace.replace('version=2 sample_interval_s=0.05 max_samples=1200', 'version=3_state_markers sample_interval_s=0.01 max_samples=2400')
    trace = trace.replace('m_HKTraceElapsed < 0.05', 'm_HKTraceElapsed < 0.01')
    trace_pbo = replace_payload(trace_props, trace_entries, tn, trace.encode())
    output.mkdir(parents=True, exist_ok=True)
    archive = output / "LUGU-RecoveryStateMarkers-DIAGNOSTIC.zip"
    with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr("addons/LUGUINSTADROPFIX.pbo", luigi)
        z.writestr("addons/HK_Trace.pbo", trace_pbo)
        z.writestr("source/movement.agf", source)
        z.writestr("source/HKTrace.c", trace)
        z.write(repository / "HK_DropRecoveryTrace/STATE_MARKERS.md", "README.md")
    with zipfile.ZipFile(archive) as z:
        assert z.testzip() is None
        assert z.read("addons/LUGUINSTADROPFIX.pbo") == luigi
        assert z.read("addons/HK_Trace.pbo") == trace_pbo
    print("PASS: graph differs from prior candidate only by six diagnostic Tags; archives verified.")
    print(archive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("/workspace/artifacts"))
    args = parser.parse_args()
    build(Path(__file__).resolve().parents[1], args.output)
