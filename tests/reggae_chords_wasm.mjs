// SPDX-License-Identifier: GPL-3.0-only
// Real button/encoder input into the same WASM module used by the browser.
import fs from 'node:fs';
import assert from 'node:assert/strict';
const bytes=fs.readFileSync('build/emu/felucca.wasm');
const signatures=new Set();
for(let mode=10;mode<=24;mode++){
 const {instance}=await WebAssembly.instantiate(bytes,{}),e=instance.exports;
 e._initialize();e.web_nor_erase();e.web_boot();
 const advance=ms=>{for(let i=0;i<ms*44.1;i+=128)e.web_render(128);};
 const tap=b=>{e.web_buttons(1<<b);advance(70);e.web_buttons(0);advance(400);};
 advance(1000);tap(1);tap(1); // SCL, CHORD
 e.web_enc(5,-100);advance(150);
 for(let i=0;i<mode;i++){e.web_enc(5,1);advance(120);}
 e.web_midi(0x90,60,100);
 let energy=0,peak=0,sig=0;
 for(let b=0;b<100;b++){
  e.web_render(128);
  const left=new Float32Array(e.memory.buffer,e.web_out_l(),128);
  for(const v of left){assert(Number.isFinite(v));energy+=v*v;peak=Math.max(peak,Math.abs(v));sig=(sig*31+Math.round(v*32768))%1000000007;}
 }
 assert(energy>0.01 && peak<1,`non-silent, unclipped chord ${mode}`);
 assert(!signatures.has(sig),`distinct audio for chord ${mode}`);signatures.add(sig);
 e.web_enc(5,-100);advance(150); // change type while held: release must still end old notes
 e.web_midi(0x80,60,0);advance(7000);
 const tail=new Float32Array(e.memory.buffer,e.web_out_l(),128);
 assert(Math.max(...tail.map(Math.abs))<0.001,`clean release for chord ${mode}`);
}
console.log('PASS: 15 new chord choices via WASM SCL/K3 input, distinct finite non-silent unclipped audio, release after changing type');
