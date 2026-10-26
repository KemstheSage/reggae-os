// SPDX-License-Identifier: GPL-3.0-only
import fs from 'node:fs';
import assert from 'node:assert/strict';
const {instance}=await WebAssembly.instantiate(fs.readFileSync('build/emu/felucca.wasm'),{}),e=instance.exports;
e._initialize();e.web_nor_erase();e.web_boot();
const advance=ms=>{for(let n=0;n<ms*44.1;n+=128)e.web_render(128);};advance(1000);
const pack=fs.readFileSync('build/reggae/percussion/ROOTPERC.slot');
new Uint8Array(e.memory.buffer,e.web_nor()+0xA0000,pack.length).set(pack);
assert.equal(e.web_percussion_select(0),1);advance(300);
let pcm=[];
for(const note of [53,55,57,59,60,54]) {
 let energy=0,tail=0,peak=0;e.web_keys(1<<(note-53));
 for(let b=0;b<1034;b++) {
  if(b===700)e.web_keys(0);
  e.web_render(128);const l=new Float32Array(e.memory.buffer,e.web_out_l(),128),r=new Float32Array(e.memory.buffer,e.web_out_r(),128);
  for(let i=0;i<128;i++){assert(Number.isFinite(l[i])&&Number.isFinite(r[i]));energy+=l[i]**2+r[i]**2;peak=Math.max(peak,Math.abs(l[i]),Math.abs(r[i]));if(b>1000)tail+=l[i]**2+r[i]**2;pcm.push(Math.trunc(l[i]*32768),Math.trunc(r[i]*32768));}
 }
 assert(peak<.99);assert(tail<.00001);assert(note===54?energy<.00001:energy>.01);
 console.log(`PASS sample ${note}: energy ${energy.toFixed(3)}, peak ${peak.toFixed(3)}, silent tail`);
}
const wav=Buffer.alloc(44+pcm.length*2);wav.write('RIFF');wav.writeUInt32LE(wav.length-8,4);wav.write('WAVEfmt ',8);wav.writeUInt32LE(16,16);wav.writeUInt16LE(1,20);wav.writeUInt16LE(2,22);wav.writeUInt32LE(44100,24);wav.writeUInt32LE(176400,28);wav.writeUInt16LE(4,32);wav.writeUInt16LE(16,34);wav.write('data',36);wav.writeUInt32LE(pcm.length*2,40);pcm.forEach((v,i)=>wav.writeInt16LE(v,44+i*2));fs.writeFileSync('build/reggae/percussion/firmware-tour.wav',wav);
assert.equal(e.web_percussion_select(3),0);
// The real panel's new effects must change sample output and release safely.
e.web_buttons(1);advance(650);e.web_keys((1<<24)|(1<<26));advance(200);
e.web_midi(0x90,60,100);advance(200);e.web_keys(0);e.web_buttons(0);e.web_midi(0x80,60,0);advance(5000);
const l=new Float32Array(e.memory.buffer,e.web_out_l(),128);assert(l.every(v=>Number.isFinite(v)&&Math.abs(v)<.001));
console.log('PASS actual WASM sample-slot playback, unsupported note silence, FX release and bounded tails');
