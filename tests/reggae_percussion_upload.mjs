// SPDX-License-Identifier: GPL-3.0-only
import fs from 'node:fs';
import assert from 'node:assert/strict';
let Processor;globalThis.AudioWorkletProcessor=class{constructor(){this.messages=[];this.port={postMessage:m=>this.messages.push(m)};}};
globalThis.registerProcessor=(name,cls)=>Processor=cls;
const {storeSectors}=await import('../web/emu/worklet.js');
const p=new Processor();await p.onMessage({type:'load',wasm:fs.readFileSync('build/emu/felucca.wasm'),sectors:{}});
assert(p.ex);const pack=fs.readFileSync('build/reggae/percussion/ROOTPERC.slot');
const flash=()=>new Uint8Array(p.ex.memory.buffer,p.ex.web_nor(),p.ex.web_nor_size());
// Occupied USR1 must remain intact; load into USR2.
flash().fill(0x55,0xA0000,0xB4000);const before=flash().slice(0xA0000,0xB4000);
await p.onMessage({type:'percussion',data:pack});assert.match(p.messages.at(-1).message,/Loaded USR2/);assert.deepEqual(flash().slice(0xA0000,0xB4000),before);
await p.onMessage({type:'percussion',data:pack});assert.match(p.messages.at(-1).message,/Loaded USR2/);assert(flash().subarray(0xC8000,0xDC000).every(v=>v===255));
const all=flash().slice();const corrupt=Buffer.from(pack);corrupt[600]^=1;
await p.onMessage({type:'percussion',data:corrupt});assert.match(p.messages.at(-1).message,/Invalid/);assert.deepEqual(flash(),all);
const q=new Processor();await q.onMessage({type:'load',wasm:fs.readFileSync('build/emu/felucca.wasm'),sectors:storeSectors(flash())});assert.equal(q.ex.web_percussion_select(1),1);
console.log('PASS sample upload: occupied slot preserved, identical pack reused, CRC corruption rejected, flash reload works');
