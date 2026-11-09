// SPDX-License-Identifier: GPL-3.0-only
// Exercise the actual AudioWorklet upload path, including non-destructive refusal.
import assert from 'node:assert/strict';
import fs from 'node:fs';
let Processor;
globalThis.registerProcessor=(_,p)=>{Processor=p;};
globalThis.AudioWorkletProcessor=class {constructor(){this.messages=[];this.port={postMessage:m=>this.messages.push(m)};}};
await import('../web/emu/worklet.js');
const p=new Processor();
await p.onMessage({type:'load',wasm:fs.readFileSync('build/emu/felucca.wasm')});
assert(p.ex);
const e=p.ex,flash=new Uint8Array(e.memory.buffer,e.web_nor(),e.web_nor_size());
const data=fs.readFileSync('build/reggae/drums/RDMDRM2.slot');
const snapshot=flash.slice();
for(let k=0;k<3;k++)flash[0xA0000+k*0x14000]=17;
const full=flash.slice();await p.onMessage({type:'drums',data});
assert.match(p.messages.at(-1).message,/No empty/);assert.deepEqual(flash,full);
flash.set(snapshot);const bad=Uint8Array.from(data);bad[600]^=1;
await p.onMessage({type:'drums',data:bad});assert.match(p.messages.at(-1).message,/Invalid/);assert.deepEqual(flash,snapshot);
await p.onMessage({type:'drums',data});assert.match(p.messages.at(-1).message,/Acoustic pack loaded/);
assert.deepEqual(flash.slice(0xA0000,0xA0000+data.length),new Uint8Array(data));
assert.deepEqual(flash.slice(0,0xA0000),snapshot.slice(0,0xA0000));
assert.deepEqual(flash.slice(0xA0000+0x14000),snapshot.slice(0xA0000+0x14000));
const installed=flash.slice();await p.onMessage({type:'drums',data});assert.deepEqual(flash,installed);
console.log('PASS drum pack CRC refusal, occupied slots preserved, empty-slot upload, other NOR unchanged and idempotent reload');
