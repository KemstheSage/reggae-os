// SPDX-License-Identifier: GPL-3.0-only
import fs from 'node:fs';
import assert from 'node:assert/strict';
const {instance}=await WebAssembly.instantiate(fs.readFileSync('build/emu/felucca.wasm'),{});
const e=instance.exports;e._initialize();e.web_nor_erase();e.web_boot();
const advance=ms=>{for(let i=0;i<ms*44.1;i+=128)e.web_render(128);};
const tap=k=>{e.web_buttons(1<<k);advance(70);e.web_buttons(0);advance(200);};
const snap=()=>Buffer.from(new Uint8Array(e.memory.buffer,e.web_screen(),240*240*2));
advance(1000);tap(9);const roll=snap();tap(9);const detail=snap();assert(!roll.equals(detail));
e.web_enc(3,-1);advance(200);assert(!detail.equals(snap()));
tap(9);e.web_enc(3,-100);advance(200);e.web_enc(6,-1);advance(200);
const automation=snap();e.web_enc(6,1);advance(200);assert(!automation.equals(snap()));
tap(9);e.web_buttons(1<<9);advance(700);const tools=snap();assert(!tools.equals(roll));
e.web_enc(3,1);advance(200);assert(!tools.equals(snap()));
e.web_buttons(0);advance(200);assert(!tools.equals(snap()));
console.log('PASS: WASM detail, automation and held-SEQ tools render and respond to physical knobs');
