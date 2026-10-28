"""Test a shared on-back cancel clip without modifying existing exit guards."""
from pathlib import Path
import argparse
import difflib
import re
import zipfile

from repack_lugu_no_motorbike import decompress, read_pbo
from repack_lugu_state_markers import replace_payload


def build(repository, output):
    props, entries = read_pbo((repository / "LUGUINSTADROPFIX.pbo").read_bytes())
    name, meta, data = next(e for e in entries if e[0].lower().endswith(b".agf"))
    original = decompress(data, meta[1]).decode()
    marker = '     Child1 "ThrowCancelAnimPne"\n'
    assert original.count(marker) == 1
    addition = '''    AnimSrcNodeBlendT HKProneCancelFacing {
     EditorPos -72.8 39.5
     BlendTime "0.0"
     BlendFn S
     TriggerOn ""
     TriggerOff ""
     Condition "ThrowingOnBack || AimX < -90 || AimX > 90"
     Child0 "ThrowCancelAnimPne"
     Child1 "HKBackCancelSource"
     OptimizeMin 1
     OptimizeMax 1
     SelectMainPath 1
    }
    AnimSrcNodeSource HKBackCancelSource {
     Tags {
      "TagProneBack"
      "TagHKBackCancelSelected"
     }
     EditorPos -70.5 40.9
     Source "Throw.PoB.Cancel"
     Looptype "No Loop"
    }
'''
    insert_before = '    AnimSrcNodeSource ThrowCancelAnimPne {\n'
    assert original.count(insert_before) == 1
    patched = original.replace(marker, '     Child1 "HKProneCancelFacing"\n').replace(insert_before, addition + insert_before)
    assert patched.replace(addition, '', 1).replace('     Child1 "HKProneCancelFacing"\n', marker, 1) == original
    assert 'TagHKCancelParent' not in patched
    result = replace_payload(props, entries, name, patched.encode())

    trace_props, trace_entries = read_pbo((repository / "HK_Trace.pbo").read_bytes())
    tn, tm, td = next(e for e in trace_entries if e[0].lower().endswith(b".c"))
    trace = (decompress(td, tm[1]) if tm[0] == 0x43707273 else td).decode()
    tag_marker = '            m_HKTraceNames.Insert("TagThrowingOut");'
    assert trace.count(tag_marker) == 1
    trace = trace.replace(tag_marker, tag_marker + '\n            m_HKTraceNames.Insert("TagHKBackCancelSelected");')
    bind = '                m_HKTraceTags.Insert(tag);'
    trace = trace.replace(bind, bind + '\n                Print("[HKTRACE] bound_tag=" + m_HKTraceNames[bindIndex] + " id=" + tag.ToString());')
    trace = trace.replace('>= 1200', '>= 2400').replace('== 1200', '== 2400')
    trace = trace.replace('version=2 sample_interval_s=0.05 max_samples=1200', 'version=4_back_cancel sample_interval_s=0.01 max_samples=2400')
    trace = trace.replace('m_HKTraceElapsed < 0.05', 'm_HKTraceElapsed < 0.01')
    trace_result = replace_payload(trace_props, trace_entries, tn, trace.encode())
    output.mkdir(parents=True, exist_ok=True)
    archive = output / "LUGU-SharedBackCancel-TEST.zip"
    diff = ''.join(difflib.unified_diff(original.splitlines(True), patched.splitlines(True), fromfile='original/movement.agf', tofile='back-cancel/movement.agf'))
    with zipfile.ZipFile(archive, "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr("addons/LUGUINSTADROPFIX.pbo", result)
        z.writestr("addons/HK_Trace.pbo", trace_result)
        z.writestr("source/movement.agf", patched)
        z.writestr("source/movement.patch", diff)
        z.writestr("source/HKTrace.c", trace)
        z.write(repository / "HK_DropRecoveryTrace/BACK_CANCEL_CLIP_TEST.md", "README.md")
    with zipfile.ZipFile(archive) as z:
        assert z.testzip() is None
        assert z.read("addons/LUGUINSTADROPFIX.pbo") == result
        assert z.read("addons/HK_Trace.pbo") == trace_result
    print('PASS: one child selection plus two nodes added; original graph recoverable exactly; no prior exit guards.')
    print(archive)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path('/workspace/artifacts'))
    args = parser.parse_args()
    build(Path(__file__).resolve().parents[1], args.output)
