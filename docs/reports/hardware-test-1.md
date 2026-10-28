# Hardware test candidate 1

Prepared for the user's first FM-1, to connect to the development Mac.
The device is believed to be on stock firmware; its version is not yet known.
No connected device has been probed, flashed or physically tested in this preparation.

Frozen candidate: UI 0.7.1 / bank 0.5, existing FM-1_900 development identity.
No new synthesis, voicing, upstream hardware-layer or bootloader code added.

- Fresh target build passes linker bounds, RAM-code and register-access checks.
- Application: 542,596 / 581,564 bytes; static RAM: 61,000 / 73,728;
  pool: 328,896 / 368,640; package: 610,090 bytes.
- Installer simulated-device regression passes; known-stock V15 restore case is
  skipped because that exact stock file is not available locally.
- Actual candidate through native OTA simulator passes with AddressSanitizer and
  UndefinedBehaviorSanitizer, including corruption/interruption rejection.
- Full REGGAE native suite passes: 75 preset/control renders, protected sounds,
  graphics, recording, arrangement, step gestures, mixer audio, screen coverage,
  settings/save navigation and expanded chord tests.
- WASM emulator/heavy-song and all fifteen new chord-input cases pass.
- Package SHA-256:
  `72186809e1e0a3afd4f3b7dba953cf9a4ceb3a27e2b45adb5b52edcf0f5ac907`.

`tools/reggae/prepare-hardware.py` bundles the candidate, checksum verifier,
read-only Mac launcher, complete corresponding source, licences and test logs.
The launcher invokes only `fm1_install.py --info`, never the write operation.
Project-local Python dependencies: mido 1.3.3 and python-rtmidi 1.5.8.

Official vendor firmware and Mac updater were downloaded from M-VAVE's current
site into ignored `build/hardware-test/recovery/`, without running/installing them.
The firmware is internally FM-1_016 (V16), not the known V15 hash accepted by
our installer. No verification rule was weakened. A recovery route must be
confirmed after identifying the device, before any flash. Vendor files are not
committed or included in the source archive.

See [HARDWARE_TEST.md](../../HARDWARE_TEST.md) for the musician-facing procedure.
