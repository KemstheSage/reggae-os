#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
"""CC0 one-shots, standard 4-bit IMA ADPCM user slot; deterministic source assets included."""
import sys,math
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]))
import sampleio
root=Path(__file__).resolve().parents[2]
items=[('kick','10_kick_BDrumNew_hit_v5.wav',.35,36),('snare','11_snare_Snare2_HitSN_v7.wav',.3,38),('clap','13_clap_Clap_rr1.wav',.25,39),('closed','14_hihat_HitC_v3.wav',.15,42),('open','15_ohat_HitO.wav',.8,46),('tom','16_tomlo_TomL_HitS_v4.wav',.5,45),('rim','12_rim_sidestick.wav',.12,37),('bell','20_cowbell_Cowbell1_Normal_v3.wav',.5,56)]
zones=[]
for name,file,dur,note in items:
 sr,x=sampleio.read_any_wav(root/'assets/samples-cc0/ROOTS_DRUM'/file)
 x=sampleio.resample(x,sr,22050)
 peak=max(map(abs,x));start=next(i for i,v in enumerate(x) if abs(v)>peak*.01);x=x[max(0,start-8):][:round(dur*22050)]
 # Remove DC with a 30 Hz high-pass, normalize peak, then smooth both cut edges.
 a=math.exp(-2*math.pi*30/22050);prev=y=0.;out=[]
 for v in x:y=a*(y+v-prev);prev=v;out.append(y)
 gain=.72/max(map(abs,out));fade=min(round(.025*22050),len(out)//3)
 out=[v*gain*min(1,i/8,(len(out)-1-i)/fade) for i,v in enumerate(out)]
 pcm=[round(v*32767) for v in out]
 zones.append((pcm,note,note,note))
 print(name,len(pcm),'samples')
hdr,data=sampleio.user_slot('RDMDRM2',zones)
dest=root/'build/reggae/drums';dest.mkdir(parents=True,exist_ok=True)
(dest/'RDMDRM2.slot').write_bytes(hdr.ljust(512,b'\0')+data)
print('Pack:',len(hdr.ljust(512,b'\0')+data),'bytes')
