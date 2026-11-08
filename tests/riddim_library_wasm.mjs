// SPDX-License-Identifier: GPL-3.0-only
import fs from 'node:fs';
import assert from 'node:assert/strict';
const bytes=fs.readFileSync('build/emu/felucca.wasm');
for(const preset of [7,15,16,17,18,19,20,21,22,23]){
 const {instance}=await WebAssembly.instantiate(bytes,{}),e=instance.exports;
 e._initialize();e.web_nor_erase();e.web_boot();
 const advance=ms=>{for(let n=0;n<ms*44.1;n+=128)e.web_render(128);};advance(3500);
 for(let k=0;k<preset;k++){e.web_enc(2,1);advance(100);}
 e.web_midi(0x90,60,96);let energy=0,late=0,peak=0;
 for(let block=0;block<1380;block++){
  e.web_render(128);for(const x of new Float32Array(e.memory.buffer,e.web_out_l(),128)){
   assert(Number.isFinite(x));energy+=x*x;peak=Math.max(peak,Math.abs(x));if(block>1100)late+=x*x;
  }
 }
 assert(energy>.01&&peak<.99);
 if([7,20,21].includes(preset))assert(late<.001);
 if(preset===19)assert(late>.01);
 e.web_midi(0x80,60,0);advance(10000);
 assert(new Float32Array(e.memory.buffer,e.web_out_l(),128).every(x=>Math.abs(x)<.001));
 console.log('PASS WASM instrument',preset,{energy,peak,late});
}
