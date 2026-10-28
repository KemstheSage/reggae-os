// SPDX-License-Identifier: GPL-3.0-only
import fs from 'node:fs';
import assert from 'node:assert/strict';
const bytes=fs.readFileSync('build/emu/felucca.wasm');
async function phrase(auto) {
 const {instance}=await WebAssembly.instantiate(bytes,{}),e=instance.exports;
 e._initialize();e.web_nor_erase();e.web_boot();
 const advance=ms=>{for(let i=0;i<ms*44.1;i+=128)e.web_render(128);};
 const tap=b=>{e.web_buttons(1<<b);advance(70);e.web_buttons(0);advance(400);};
 advance(1000);tap(1);tap(1);
 e.web_enc(5,-100);advance(150);
 for(let i=0;i<3;i++){e.web_enc(5,1);advance(120);} // MAJ
 e.web_enc(6,-100);advance(150);
 if(auto)for(let i=0;i<5;i++){e.web_enc(6,1);advance(120);} // AUTO, appended ID 5
 const hashes=[];
 for(const note of [60,65,67,60]) {
  e.web_midi(0x90,note,100);let hash=0,energy=0,peak=0;
  for(let b=0;b<100;b++){
   e.web_render(128);const l=new Float32Array(e.memory.buffer,e.web_out_l(),128);
   for(const v of l){assert(Number.isFinite(v));energy+=v*v;peak=Math.max(peak,Math.abs(v));hash=(hash*31+Math.round(v*32768))%1000000007;}
  }
  assert(energy>0.01 && peak<1);hashes.push(hash);
  if(auto && note===65) {
   const fb=new Uint16Array(e.memory.buffer,e.web_screen(),240*240),rgb=Buffer.alloc(240*240*3);
   for(let i=0;i<fb.length;i++){const p=(fb[i]>>8)|((fb[i]&255)<<8);rgb[i*3]=((p>>11)&31)*255/31;rgb[i*3+1]=((p>>5)&63)*255/63;rgb[i*3+2]=(p&31)*255/31;}
   fs.writeFileSync('build/reggae/auto-voicing-wasm.ppm',Buffer.concat([Buffer.from('P6\n240 240\n255\n'),rgb]));
  }
  e.web_midi(0x80,note,0);advance(2000);
 }
 advance(5000);assert(Math.max(...new Float32Array(e.memory.buffer,e.web_out_l(),128).map(Math.abs))<0.001);
 return hashes;
}
const close=await phrase(false),auto=await phrase(true);
assert.equal(close[0],auto[0]);assert.notEqual(close[1],auto[1]);
assert.deepEqual(auto,await phrase(true));
console.log('PASS: WASM SCL/K4 AUTO, unchanged first chord, changed later inversions, deterministic audible unclipped progression, clean release');
