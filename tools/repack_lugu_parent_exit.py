"""Test retaining the nested prone cancel until its recovery can evaluate."""
from pathlib import Path
import argparse
import difflib
import re
import zipfile

from repack_lugu_no_motorbike import decompress, read_pbo
from repack_lugu_state_markers import replace_payload


def build(repository, output):
    with zipfile.ZipFile(repository / "downloads/LUGU-RecoveryStateMarkers-DIAGNOSTIC.zip") as z:
        prior = z.read("addons/LUGUINSTADROPFIX.pbo")
        trace = z.read("addons/HK_Trace.pbo")
    props, entries = read_pbo(prior)
    name, metadata, payload = next(e for e in entries if e[0].lower().endswith(b".agf"))
    original = decompress(payload, metadata[1]).decode()
    gate = r' && !((AimX < -90 || AimX > 90) && IsTag(\"TagHKCancelParent\") && IsTag(\"TagHKRecoveryProneCancel\"))'
    pattern = r'(AnimSrcNodeTransition "\{[^}]+\}" \{\n       FromState "Throwing"\n       ToState "(IdleLow|MovementLow|MovementRas|IdleRas|PneOnBack)"\n.*?       Condition ")(.*?)("\n       BlendFn)'
    touched = []
    def patch(match):
        touched.append(match[2])
        return match[1] + '(' + match[3] + ')' + gate + match[4]
    patched, count = re.subn(pattern, patch, original, flags=re.S)
    assert count == 5 and set(touched) == {'IdleLow', 'MovementLow', 'MovementRas', 'IdleRas', 'PneOnBack'}
    assert len(original.splitlines()) == len(patched.splitlines())
    differences = [(a, b) for a, b in zip(original.splitlines(), patched.splitlines()) if a != b]
    assert len(differences) == 5
    assert all('Condition ' in a and 'Condition ' in b for a, b in differences)
    result = replace_payload(props, entries, name, patched.encode())
    output.mkdir(parents=True, exist_ok=True)
    archive = output / "LUGU-RecoveryParentExit-TEST.zip"
    patch_text = ''.join(difflib.unified_diff(original.splitlines(True), patched.splitlines(True), fromfile='state-markers/movement.agf', tofile='parent-exit/movement.agf'))
    with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr("addons/LUGUINSTADROPFIX.pbo", result)
        z.writestr("addons/HK_Trace.pbo", trace)
        z.writestr("source/movement.agf", patched)
        z.writestr("source/movement.patch", patch_text)
        z.write(repository / "HK_DropRecoveryTrace/PARENT_EXIT_TEST.md", "README.md")
    with zipfile.ZipFile(archive) as z:
        assert z.testzip() is None
        assert z.read("addons/LUGUINSTADROPFIX.pbo") == result
        assert z.read("addons/HK_Trace.pbo") == trace
    print("PASS: five outer exit guards added; all other graph lines and unrelated payloads preserved.")
    print(archive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("/workspace/artifacts"))
    args = parser.parse_args()
    build(Path(__file__).resolve().parents[1], args.output)
