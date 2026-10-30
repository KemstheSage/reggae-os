// SPDX-License-Identifier: GPL-3.0-only
// Real panel saves to A/B, browser flash persistence, fresh-instance audible recall.
import fs from 'node:fs';
import assert from 'node:assert/strict';
import {storeSectors,restoreSectors} from '../web/emu/worklet.js';
const bytes=fs.readFileSync('build/emu/felucca.wasm');
async function device(saved){
 const {instance}=await WebAssembly.instantiate(bytes,{}),e=instance.exports;
 e._initialize();e.web_nor_erase();
 const nor=()=>new Uint8Array(e.memory.buffer,e.web_nor(),e.web_nor_size());
 if(saved)restoreSectors(nor(),saved);e.web_boot();
 const advance=ms=>{for(let n=0;n<ms*44.1;n+=128)e.web_render(128);};
 const tap=k=>{e.web_buttons(1<<k);advance(70);e.web_buttons(0);advance(400);};
 const turn=(k,n)=>{e.web_enc(k,n);advance(200);};
 advance(1000);return {e,nor,advance,tap,turn};
}
const a=await device();
function writeNote(key){
 a.tap(9);a.e.web_buttons(1<<11);a.advance(70);
 a.e.web_keys(1<<key);a.advance(100);a.e.web_keys(0);a.e.web_buttons(0);a.advance(500);
}
function save(delta){a.tap(7);if(delta)a.turn(3,delta);a.turn(6,1);a.tap(13);a.tap(13);}
writeNote(0);save(0);
const first=a.nor().slice(0x97000,0x99000);
assert(first.some(v=>v!==255));
writeNote(7);save(1);
assert.deepEqual(a.nor().slice(0x97000,0x99000),first,'saving B changed A flash');
const saved=storeSectors(a.nor());
async function audition(slot){
 const d=await device(saved);d.tap(7);if(slot)d.turn(3,slot);d.turn(5,1);d.tap(13);d.tap(10);
 let energy=0,hash=0;
 for(let n=0;n<800;n++){
  d.e.web_render(128);
  for(const v of new Float32Array(d.e.memory.buffer,d.e.web_out_l(),128)){
   assert(Number.isFinite(v)&&Math.abs(v)<1);energy+=v*v;hash=(Math.imul(hash,31)+Math.round(v*32768))|0;
  }
 }
 assert(energy>0.01,'saved beat silent');return {energy,hash};
}
const A=await audition(0),B=await audition(1),again=await audition(0);
assert.notEqual(A.hash,B.hash,'A/B recalled the same audio');assert.deepEqual(A,again);
console.log('PASS: WASM panel saves independent A/B flash; fresh browser instances recall distinct, repeatable audible beats', {A,B});
