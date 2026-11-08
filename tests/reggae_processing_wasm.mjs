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
a.tap(9);a.e.web_buttons(1<<11);a.advance(70);a.e.web_keys(1);a.advance(100);a.e.web_keys(0);a.e.web_buttons(0);a.advance(500);
function save(delta){a.tap(7);if(delta)a.turn(3,delta);a.turn(6,1);a.tap(13);a.tap(13);}
save(0);const original=a.nor().slice(0x97000,0x99000);
a.tap(6);for(let i=0;i<2;i++)a.tap(0); // punch-in then EQ
// MASTER: 880-ish Hz bell +9 dB, then 4:1 compression at -24 dBFS.
a.turn(3,16);a.turn(4,16);a.turn(5,9);a.tap(0);a.turn(4,24);a.turn(5,3);
save(1);assert.deepEqual(a.nor().slice(0x97000,0x99000),original);
a.tap(6);a.tap(9);a.turn(5,1);a.turn(6,8);a.turn(5,1);a.turn(6,4);save(1);
const saved=storeSectors(a.nor());
function wav(path,values){const b=Buffer.alloc(44+values.length*2);b.write('RIFF');b.writeUInt32LE(b.length-8,4);b.write('WAVEfmt ',8);b.writeUInt32LE(16,16);b.writeUInt16LE(1,20);b.writeUInt16LE(1,22);b.writeUInt32LE(44100,24);b.writeUInt32LE(88200,28);b.writeUInt16LE(2,32);b.writeUInt16LE(16,34);b.write('data',36);b.writeUInt32LE(values.length*2,40);values.forEach((v,i)=>b.writeInt16LE(Math.round(v*32767),44+i*2));fs.writeFileSync(path,b);}
async function audition(slot,file){
 const d=await device(saved);d.tap(7);if(slot)d.turn(3,slot);d.turn(5,1);d.tap(13);d.tap(10);
 let energy=0,peak=0,hash=0,samples=[];
 for(let n=0;n<800;n++){d.e.web_render(128);for(const v of new Float32Array(d.e.memory.buffer,d.e.web_out_l(),128)){assert(Number.isFinite(v)&&Math.abs(v)<1);energy+=v*v;peak=Math.max(peak,Math.abs(v));hash=(Math.imul(hash,31)+Math.round(v*32768))|0;samples.push(v);}}
 assert(energy>.01);if(file)wav(file,samples);return {energy,peak,hash};
}
const dry=await audition(0,'build/reggae/processing-dry.wav'),processed=await audition(1,'build/reggae/processing-wet.wav'),roll=await audition(2,'build/reggae/processing-roll.wav');
assert.notEqual(dry.hash,processed.hash);assert.notEqual(processed.hash,roll.hash);assert.deepEqual(processed,await audition(1));
console.log('PASS shared WASM: UI edits EQ/compression + note length/repeats, audible output changes, original slot unchanged, fresh-instance recall repeatable', {dry,processed,roll});

// Real audio workload with EQ and compression enabled on all five targets.
const stress=await device();stress.e.web_test_heavy();
for(let target=0;target<5;target++){
 stress.tap(6);for(let i=0;i<2;i++)stress.tap(0);
 stress.turn(3,-20);stress.turn(3,target*4);
 for(let band=0;band<4;band++){if(band)stress.turn(3,1);stress.turn(4,8+band*6);stress.turn(5,3);}
 stress.tap(0);stress.turn(4,18);stress.turn(5,3);
}
stress.tap(10);stress.advance(1000);const started=performance.now();
let stressPeak=0;for(let k=0;k<345;k++){stress.e.web_render(128);for(const v of new Float32Array(stress.e.memory.buffer,stress.e.web_out_l(),128)){assert(Number.isFinite(v)&&Math.abs(v)<1);stressPeak=Math.max(stressPeak,Math.abs(v));}}
const ms=performance.now()-started;assert(stressPeak>.001);assert(ms<500);
console.log('PASS all-five processor heavy-song host benchmark:',{ms,stressPeak});
