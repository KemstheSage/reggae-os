# REGGAE-OS UI 0.3.2 — native artwork quality

The user's request was for higher-quality instrument graphics. This iteration
improves the actual firmware rendering while keeping the hardware resolution.

- Smoother circles and diagonal lines throughout instrument/effect artwork.
- Fuller organ console and rotary cabinet, clearer registration rails and shaded keys.
- Recessed speaker baffles, layered cones and cabinet construction details.
- Refined piano, brass and siren housings; bounded envelope-driven cone movement.
- No DSP, preset, physical input mapping or audio-buffer changes.

The renderer uses integer coverage in the existing RGB565 strip. It does not render
at desktop resolution and then substitute an image into the emulator. No additional
framebuffer, heap allocation, or third-party image assets. Animation stays at 10 Hz;
physical display/audio timing remains unverified.

[Before/after, each screen 240 × 240](../images/ui-0.3.2-comparison.png)

[All thirteen screens](../images/ui-0.3.2-bank.png)

Validation: native graphics tests run under AddressSanitizer, checking motion,
reduced-animation mode, parameter focus and circle coverage/clipping. Screens
are inspected at the native size. Final hardware/emulator build and audio parity
results are recorded in TESTING.md.
