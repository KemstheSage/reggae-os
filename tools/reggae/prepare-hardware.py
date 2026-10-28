#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
"""Bundle an already-built FM-1 candidate; never opens MIDI or writes a device."""
from pathlib import Path
import hashlib,json,shutil,subprocess,sys
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools'))
import fm1_install
out=ROOT/'build/hardware-test';out.mkdir(parents=True,exist_ok=True)
package=ROOT/'build/felucca.fwsc'
identity,image=fm1_install.load_package(package,False)
if identity!='FM-1_900': raise SystemExit('Unexpected candidate identity: '+identity)
name='reggae-os-ui-0.7.1-hwtest1.fwsc'
shutil.copy2(package,out/name)
shutil.copy2(ROOT/'HARDWARE_TEST.md',out/'START-HERE.md')
sha=hashlib.sha256((out/name).read_bytes()).hexdigest()
manifest=dict(candidate='REGGAE-OS UI 0.7.1 hardware test 1',hardware_status='UNTESTED',
 source_commit=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),
 engine_commit=subprocess.check_output(['git','-C','firmware/engines','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),
 package=name,identity=identity,sha256=sha,package_bytes=package.stat().st_size,
 app_bytes=(ROOT/'build/felucca.bin').stat().st_size,
 note='Existing development USB identity; verify REGGAE UI version on screen. No device write performed.')
(out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
(out/'SHA256SUMS').write_text(f'{sha}  {name}\n')
(out/'verify.py').write_text('''# Read-only package validation; no MIDI access.
from pathlib import Path
import hashlib,json
p=Path(__file__).resolve().parent
m=json.loads((p/'manifest.json').read_text())
b=(p/m['package']).read_bytes()
if hashlib.sha256(b).hexdigest()!=m['sha256'] or len(b)!=m['package_bytes']:
 raise SystemExit('STOP: candidate checksum/size mismatch')
print('PASS:',m['candidate'],m['identity'],len(b),'bytes')
print('SHA-256:',m['sha256'])
print('Physical hardware: UNTESTED')
''')
# This local launcher deliberately offers only the read-only identity operation.
(out/'CHECK DEVICE.command').write_text('''#!/bin/zsh
set -u
kit_dir="${0:A:h}"
project_dir="${kit_dir:h:h}"
python_bin="$project_dir/.venv312/bin/python"
"$python_bin" "$kit_dir/verify.py" && "$python_bin" "$project_dir/tools/fm1_install.py" --info
check_status=$?
echo "Read-only check finished (exit $check_status). No firmware was installed."
read -r '?Press Return to close. '
exit "$check_status"
''')
(out/'CHECK DEVICE.command').chmod(0o755)
recovery=out/'recovery/FM-1.fwsc'
if recovery.exists():
 raw=recovery.read_bytes();rsha=hashlib.sha256(raw).hexdigest()
 result=dict(identity=fm1_install.product_of(raw),bytes=len(raw),sha256=rsha,
  known_v15=rsha==fm1_install.STOCK_V15_SHA256,physical_recovery_tested=False)
 (out/'recovery-check.json').write_text(json.dumps(result,indent=2)+'\n')
for name,src in [('build.log','reggae-hw-build.log'),('installer-tests.log','reggae-hw-install-test.log'),('ota-tests.log','reggae-hw-ota-test.log'),('regression.log','reggae-hw-regression.log'),('wasm-chords.log','reggae-hw-wasm-chords.log'),('wasm-emulator.log','reggae-hw-wasm.log')]:
 p=Path('/private/tmp')/src
 if p.exists(): shutil.copy2(p,out/name)
print(json.dumps(manifest,indent=2))

# Complete corresponding source including the engine submodule, no vendor downloads.
subprocess.run(['tar','--exclude=.git','--exclude=__pycache__','-czf',str(out/'reggae-os-source.tar.gz'),
 'firmware','tools','tests','assets','web','docs','.github','.gitmodules','build.sh','LICENSE','LICENSES',
 *[p.name for p in ROOT.glob('*.md')]],cwd=ROOT,check=True)
shutil.copy2(ROOT/'LICENSE',out/'LICENSE')
shutil.copy2(ROOT/'LICENSING.md',out/'LICENSING.md')
print('Complete corresponding source and licence documents bundled; vendor downloads remain local.')
