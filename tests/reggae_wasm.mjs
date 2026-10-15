// SPDX-License-Identifier: GPL-3.0-only
// Compare the actual WebAssembly audio ISR/panel with the native test WAVs.
import fs from 'node:fs';
import assert from 'node:assert/strict';
const bytes=fs.readFileSync('build/emu/felucca.wasm');
const turns=[[13,8,1,121],[2,10,2,111],[91,119,51,115],[70,105,29,107]];
for(let p=0;p<4;p++) for(let knob=-1;knob<4;knob++) {
  const {instance}=await WebAssembly.instantiate(bytes,{}), e=instance.exports;
  e._initialize(); e.web_nor_erase(); e.web_boot();
  const advance=ms=>{for(let n=0;n<Math.floor(ms*441/10);n+=128)e.web_render(128);};
  advance(1000);
  for(let i=0;i<p;i++){e.web_enc(2,1);advance(100);}
  if(knob>=0)e.web_enc(3+knob,turns[p][knob]);
  advance(200);
  const note=p<2?60:36;
  e.web_midi(0x90,note,100);
  if(p<2){e.web_midi(0x90,note+3,100);e.web_midi(0x90,note+7,100);}
  const blocks=Math.floor(44100*4/128), pcm=Buffer.alloc(blocks*128*4);
  let at=0, energy=0;
  for(let b=0;b<blocks;b++) {
    if(b===344){e.web_midi(0x80,note,0);e.web_midi(0x80,note+3,0);e.web_midi(0x80,note+7,0);}
    e.web_render(128);
    const l=new Float32Array(e.memory.buffer,e.web_out_l(),128),r=new Float32Array(e.memory.buffer,e.web_out_r(),128);
    for(let i=0;i<128;i++) {
      assert(Number.isFinite(l[i])&&Number.isFinite(r[i]));energy+=l[i]*l[i]+r[i]*r[i];
      pcm.writeInt16LE(Math.trunc(l[i]*32768),at);pcm.writeInt16LE(Math.trunc(r[i]*32768),at+2);at+=4;
    }
  }
  assert(energy>0.1);
  const native=fs.readFileSync(`build/reggae/p${p}-k${knob}.wav`).subarray(44);
  assert(pcm.equals(native),`Native/WASM PCM mismatch: preset ${p}, knob ${knob}`);
  console.log(`PASS native = WebAssembly PCM: preset ${p}, knob ${knob}, ${blocks*128} stereo frames`);
}
