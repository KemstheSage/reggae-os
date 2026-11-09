#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
"""Build a self-contained manual update site. Never opens MIDI or writes a device."""
import hashlib,json,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'web'))
import make_site
version=sys.argv[1] if len(sys.argv)>1 else '0.8.1'
out=ROOT/'build'/('update-'+version)
pkg=ROOT/'build/felucca.fwsc'
make_site.main(pkg,'riddim-os-'+version,out)
installer=out/'webapp/installer/index.html'
s=installer.read_text().replace('<title>Felucca</title>','<title>Riddim OS manual installer</title>').replace('<h1>Felucca</h1>',f'<h1>Riddim OS {version}</h1>')
s=s.replace('say(`Felucca ${meta.version}`);','say(`Riddim OS ${meta.version}`);')
s=s.replace('<p><button id="go"', '''<p><a href="../../START-HERE.html">Read the update and recovery guide</a> · <a href="../editor/" target="_blank" rel="noopener">Open backup editor</a></p>
  <p><label><input type="checkbox" id="riddim-backup"> I have downloaded a complete backup of my FM-1 to this Mac.</label></p>
  <p><button id="go"''')
s=s.replace('if (writing) return;\n  lockInstaller(true);','if (writing) return;\n  if (!$("riddim-backup").checked) { alert("Save a complete backup in the editor before installing. If backup is unavailable, stop and keep your current firmware."); return; }\n  lockInstaller(true);',1)
s=s.replace('Felucca 1.0 is a field-testing release; please report problems on GitHub.',f'Riddim OS {version} is a hardware-test candidate; it has not been tested on a physical FM-1.')
# Keep original copyright attribution and transport/protocol identities intact.
s=s.replace('href="https://github.com/hugelton/Felucca"','href="https://github.com/KemstheSage/reggae-os"').replace('>github.com/hugelton/Felucca<','>github.com/KemstheSage/reggae-os<')
installer.write_text(s)
sha=hashlib.sha256(pkg.read_bytes()).hexdigest()
fwname='felucca-riddim-os-'+version+'.fwsc'
manifest=dict(version=version,hardware_status='COMPILES; HARDWARE UNTESTED',sha256=sha,package_bytes=pkg.stat().st_size,source_commit=subprocess.check_output(['git','rev-parse','HEAD'],cwd=ROOT,text=True).strip(),working_tree_changes=bool(subprocess.check_output(['git','status','--porcelain'],cwd=ROOT,text=True).strip()),package=fwname)
(out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
(out/'firmware/SHA256SUMS').write_text(f'{sha}  {fwname}\n')
shutil.copy(ROOT/'MANUAL_UPDATE.md',out/'START-HERE.md')
import html
text=(ROOT/'MANUAL_UPDATE.md').read_text()
(out/'START-HERE.html').write_text('<!doctype html><meta charset="utf-8"><meta name="viewport" content="width=device-width"><title>Riddim OS update guide</title><style>body{max-width:760px;margin:30px auto;padding:16px;background:#131c18;color:#f5e8c4;font:17px/1.6 system-ui}a{color:#e9bb48}pre{white-space:pre-wrap;font:inherit}</style><h1>Riddim OS '+version+'</h1><p><a href="webapp/editor/">1. Backup editor</a> · <a href="webapp/installer/">2. Manual installer</a> · <a href="firmware/'+fwname+'">Firmware file</a> · <a href="source.tar.gz">Corresponding source</a></p><pre>'+html.escape(text)+'</pre>')
(out/'index.html').write_text('<!doctype html><meta charset="utf-8"><meta http-equiv="refresh" content="0;url=START-HERE.html"><a href="START-HERE.html">Riddim OS manual update guide</a>')
shutil.copy(ROOT/'build/emu/reggae-os-source.tar.gz',out/'source.tar.gz')
print(json.dumps(manifest,indent=2))
