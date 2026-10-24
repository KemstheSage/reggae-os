// SPDX-License-Identifier: GPL-3.0-only
// Exercise real WASM panel navigation and capture its native RGB565 screen.
import fs from 'node:fs';
import assert from 'node:assert/strict';
const {instance}=await WebAssembly.instantiate(fs.readFileSync('build/emu/felucca.wasm'),{});
const e=instance.exports;e._initialize();e.web_nor_erase();e.web_boot();
const advance=ms=>{for(let i=0;i<ms*44.1;i+=128)e.web_render(128);};
const tap=k=>{e.web_buttons(1<<k);advance(60);e.web_buttons(0);advance(150);};
advance(1000);
// Framebuffer export name is checked against the existing emulator interface.
const snap=()=>Buffer.from(new Uint8Array(e.memory.buffer,e.web_screen(),240*240*2));
tap(9);const pattern=snap();
tap(5);const empty=snap();assert(!pattern.equals(empty));
e.web_enc(5,1);advance(200);const section=snap();assert(!section.equals(empty));
e.web_enc(6,1);advance(200);assert(!section.equals(snap()));
console.log('PASS: WASM SEQ/GLO screens, add section and repeat control change the native framebuffer');
