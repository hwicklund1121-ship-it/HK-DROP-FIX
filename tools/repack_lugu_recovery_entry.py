"""Test a late standing entry into Luigi's existing cancel recovery graph."""
from pathlib import Path
import argparse
import difflib
import hashlib
import re
import struct
import zipfile

from repack_lugu_no_motorbike import compress_literals, decompress, read_pbo


def build(repository, output):
    properties, entries = read_pbo((repository / "LUGUINSTADROPFIX.pbo").read_bytes())
    matches = [e for e in entries if e[0].lower().endswith(b".agf")]
    if len(matches) != 1:
        raise ValueError("Expected the original single movement AGF")
    name, metadata, stored = matches[0]
    original = decompress(stored, metadata[1]).decode()
    start = original.index("    AnimSrcNodeStateMachine HKCancelRecoverySTM {")
    end = original.index("    AnimSrcNodeStateMachine ThrowSTM {", start)
    section = original[start:end]
    original_state = re.search(r'(AnimSrcNodeState Original \{.*?StartCondition )"Stance != 2"', section, re.S)
    back_state = re.search(r'(AnimSrcNodeState BackRise \{.*?StartCondition )""', section, re.S)
    if not original_state or not back_state:
        raise ValueError("Recovery entry conditions differ from expected original")
    normal_entry = r'Stance == 1 || (Stance == 0 && !IsTag(\"TagThrowingPneTransition\"))'
    recovery_entry = r'Stance == 0 && IsTag(\"TagThrowingPneTransition\")'
    changed, n1 = re.subn(r'(AnimSrcNodeState Original \{.*?StartCondition )"Stance != 2"', lambda m: m[1] + '"' + normal_entry + '"', section, count=1, flags=re.S)
    changed, n2 = re.subn(r'(AnimSrcNodeState BackRise \{.*?StartCondition )""', lambda m: m[1] + '"' + recovery_entry + '"', changed, count=1, flags=re.S)
    assert n1 == n2 == 1
    patched = original[:start] + changed + original[end:]
    line_changes = [(a, b) for a, b in zip(original.splitlines(), patched.splitlines()) if a != b]
    assert len(original.splitlines()) == len(patched.splitlines())
    assert len(line_changes) == 2
    assert all('StartCondition' in a and 'StartCondition' in b for a, b in line_changes)
    encoded = compress_literals(patched.encode())
    replacement = (name, (metadata[0], len(patched.encode()), metadata[2], metadata[3], len(encoded)), encoded)
    changed_entries = [replacement if e[0] == name else e for e in entries]
    header = properties + b"".join(n + b"\0" + struct.pack("<5I", *m) for n, m, _ in changed_entries)
    body = header + b"\0" + bytes(20) + b"".join(d for _, _, d in changed_entries)
    result = body + b"\0" + hashlib.sha1(body).digest()
    actual_properties, actual = read_pbo(result)
    assert actual_properties == properties and actual == changed_entries
    assert decompress(replacement[2], replacement[1][1]).decode() == patched
    output.mkdir(parents=True, exist_ok=True)
    archive = output / "LUGU-RecoveryLateEntry-TEST.zip"
    trace = (repository / "HK_Trace.pbo").read_bytes()
    patch = ''.join(difflib.unified_diff(original.splitlines(True), patched.splitlines(True), fromfile='original/movement.agf', tofile='late-entry/movement.agf'))
    with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr("addons/LUGUINSTADROPFIX.pbo", result)
        z.writestr("addons/HK_Trace.pbo", trace)
        z.writestr("source/movement.agf", patched)
        z.writestr("source/movement.patch", patch)
        z.write(repository / "HK_DropRecoveryTrace/LATE_ENTRY_TEST.md", "README.md")
    with zipfile.ZipFile(archive) as z:
        assert z.testzip() is None
        assert z.read("addons/LUGUINSTADROPFIX.pbo") == result
        assert z.read("addons/HK_Trace.pbo") == trace
    print(f"PASS: two entry-condition lines changed; {len(entries)-1} other entries and metadata preserved.")
    print(archive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("/workspace/artifacts"))
    args = parser.parse_args()
    build(Path(__file__).resolve().parents[1], args.output)
