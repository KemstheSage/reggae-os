// SPDX-License-Identifier: GPL-3.0-only
import fs from 'node:fs';
import assert from 'node:assert/strict';
const order=[0,1,11,2,3,10,5,6,18,4,20,21,7,19,8,15,17,16,22,23,9,13,14,12,24,25,26,27];
const fingerprints=[];
for(const id of [12,24,25,26,27]) {
 const {instance:{exports:e}}=await WebAssembly.instantiate(fs.readFileSync('build/emu/felucca.wasm'),{});
 e._initialize();e.web_nor_erase();e.web_boot();
 const advance=ms=>{for(let n=0;n<ms*44.1;n+=128)e.web_render(128);};advance(3500);
 for(let i=0;i<order.indexOf(id);i++){e.web_enc(2,1);advance(50);}
 let hash=0,peak=0;
 for(const note of [36,38,39,42,46,45,37,56]) {
  let energy=0;e.web_midi(0x90,note,96);
  for(let b=0;b<345;b++){
   e.web_render(128);
   for(const v of new Float32Array(e.memory.buffer,e.web_out_l(),128)){
    assert(Number.isFinite(v));energy+=v*v;peak=Math.max(peak,Math.abs(v));hash=(Math.imul(hash,31)+Math.round(v*32768))|0;
   }
  }
  e.web_midi(0x80,note,0);assert(energy>.001,`silent lane ${id}/${note}`);
 }
 // Simultaneous kit hits and rapid closed-hat retrigger/choke.
 for(let b=0;b<345;b++){
  if(b%20===0)for(const n of [36,38,42,46])e.web_midi(0x90,n,127);
  e.web_render(128);
  for(const v of new Float32Array(e.memory.buffer,e.web_out_l(),128)){assert(Number.isFinite(v)&&Math.abs(v)<.99);}
 }
 advance(10000);assert(new Float32Array(e.memory.buffer,e.web_out_l(),128).every(v=>Math.abs(v)<.001));
 assert(peak<.99);fingerprints.push(hash);console.log('PASS WASM drum kit',id,{hash,peak});
}
assert.equal(new Set(fingerprints).size,5,'kits must render different audio');
