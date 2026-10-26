#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
"""Build optional CC0 percussion for the existing USR sample-slot format."""
import sys, wave, struct, json
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
import sampleio as s
ROOT=Path(__file__).resolve().parents[2]
out=ROOT/'build/reggae/percussion';out.mkdir(parents=True,exist_ok=True)
names=['rim','conga','claves','shaker','tambourine']; notes=[53,55,57,59,60]
zones=[];report=[];tour=[]
for name,note in zip(names,notes):
    sr,x=s.read_any_wav(ROOT/f'assets/samples-cc0/REGGAE_PERC/{name}.wav')
    x=s.resample(x,sr,s.SLOT_RATE)
    pcm=s.to_int16(x)
    # Each white key has one piece; intervening black keys are silent.
    zones.append((pcm,note,note,note))
    report.append(dict(name=name,note=note,seconds=len(pcm)/s.SLOT_RATE,peak=max(map(abs,pcm))))
    tour+=pcm+[0]*s.SLOT_RATE
hdr,data=s.user_slot('ROOTPERC',zones)
(out/'ROOTPERC.hdr').write_bytes(hdr);(out/'ROOTPERC.bin').write_bytes(data)
(out/'ROOTPERC.slot').write_bytes(hdr.ljust(512,b'\0')+data)
with wave.open(str(out/'percussion-tour.wav'),'wb') as w:
    w.setparams((1,2,s.SLOT_RATE,0,'NONE','not compressed'))
    w.writeframes(struct.pack('<'+'h'*len(tour),*tour))
(out/'report.json').write_text(json.dumps(dict(bytes=len(data),sounds=report),indent=2)+'\n')
print((out/'report.json').read_text())
