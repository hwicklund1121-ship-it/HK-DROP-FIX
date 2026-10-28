"""Compare the resolved get-up clip with the improved back-cancel clip."""
from pathlib import Path
import argparse
import difflib
import zipfile

from repack_lugu_no_motorbike import decompress, read_pbo
from repack_lugu_state_markers import replace_payload


def build(repository, output):
    with zipfile.ZipFile(repository / "downloads/LUGU-SharedBackCancel-TEST.zip") as z:
        previous = z.read("addons/LUGUINSTADROPFIX.pbo")
        trace = z.read("addons/HK_Trace.pbo")
    props, entries = read_pbo(previous)
    name, meta, data = next(e for e in entries if e[0].lower().endswith(b".agf"))
    original = decompress(data, meta[1]).decode()
    start = original.index('    AnimSrcNodeSource HKBackCancelSource {')
    end = original.index('    AnimSrcNodeSource ThrowCancelAnimPne {', start)
    block = original[start:end]
    old = '     Source "Throw.PoB.Cancel"'
    new = '     Source "Throw.PoB.ToErect"'
    assert block.count(old) == 1
    patched = original[:start] + block.replace(old, new) + original[end:]
    assert len(original.splitlines()) == len(patched.splitlines())
    assert sum(a != b for a, b in zip(original.splitlines(), patched.splitlines())) == 1
    assert patched[:start] == original[:start]
    result = replace_payload(props, entries, name, patched.encode())
    tprops, tentries = read_pbo(trace)
    tn, tm, td = next(e for e in tentries if e[0].lower().endswith(b".c"))
    toriginal = decompress(td, tm[1]).decode()
    assert toriginal.count('version=4_back_cancel') == 1
    tsource = toriginal.replace('version=4_back_cancel', 'version=5_back_rise_clip')
    trace_result = replace_payload(tprops, tentries, tn, tsource.encode())
    output.mkdir(parents=True, exist_ok=True)
    archive = output / "LUGU-BackRiseClip-TEST.zip"
    diff = ''.join(difflib.unified_diff(original.splitlines(True), patched.splitlines(True), fromfile='improved/movement.agf', tofile='back-rise-clip/movement.agf'))
    with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr("addons/LUGUINSTADROPFIX.pbo", result)
        z.writestr("addons/HK_Trace.pbo", trace_result)
        z.writestr("source/movement.agf", patched)
        z.writestr("source/movement.patch", diff)
        z.writestr("source/HKTrace.c", tsource)
        z.write(repository / "HK_DropRecoveryTrace/BACK_RISE_CLIP_TEST.md", "README.md")
    with zipfile.ZipFile(archive) as z:
        assert z.testzip() is None
        assert z.read("addons/LUGUINSTADROPFIX.pbo") == result
        assert z.read("addons/HK_Trace.pbo") == trace_result
    print('PASS: exactly one animation-source line changed from improved build; archive integrity verified.')
    print(archive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path('/workspace/artifacts'))
    args = parser.parse_args()
    build(Path(__file__).resolve().parents[1], args.output)
