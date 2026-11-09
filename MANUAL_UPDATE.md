# Riddim OS 0.8.1 — manual FM-1 update

This is a hardware-test candidate. It builds for the FM-1, but this release has not been physically tested. Installation is a separate action you perform; opening these pages does not flash anything.

Use Chrome or Edge on your Mac, a reliable USB data cable and a powered FM-1. Safari/iPhone can run the emulator but cannot use this Web MIDI installer. Close the emulator, DAW and other MIDI applications while updating.

## 1. Preserve your beats first

Save your current work using the PROJECT page on the FM-1. Open the Backup editor link, connect to your FM-1 and allow MIDI/SysEx access. Find Complete backup → Save backup. Keep the downloaded JSON file somewhere safe, and check that the download completed. This archive includes the current song, saved project slots, user sounds, user samples and settings.

If Complete backup is unavailable or fails, stop before installation. Do not reset the device or overwrite project slots. We need to resolve backup compatibility first. An emulator's browser storage is not a backup of your physical FM-1.

Close the editor tab after downloading so only the installer uses MIDI.

## 2. Install manually

Open the Manual installer link. Confirm the page says Riddim OS 0.8.1. Tick the backup acknowledgement only after saving your backup. Click Install, allow MIDI/SysEx access and follow the progress display. Keep USB and power connected throughout writing and reboot. Do not use a factory reset.

The installer supplies the firmware file automatically. The separate .fwsc download and SHA256SUMS file are available for archiving. The firmware still uses the compatible FM-1_900 USB identity and Felucca loader marker internally; those are not the visible OS name.

After reboot, look for RIDDIM OS and open a saved project. Verify each section, its instruments, note velocities/lengths and effects before continuing work. Browse to the new kits to audition them. Existing saved kits keep their saved parameter values: load the new Roots Kit explicitly to hear its revised factory defaults.

## 3. What happens to saved beats?

This update does not change the project format, flash storage addresses, engine IDs or existing factory preset IDs. Instrument grouping changes browser presentation only. New kits are appended. Saved projects retain their sound settings, including the old kit settings; they are not automatically replaced by the new factory voicing. Keep the backup even if everything appears correct.

Do not restore a backup merely because installation succeeded. Restore only if data is missing or you deliberately want the backed-up state: Complete backup → Restore backup replaces current device data. Keep the firmware package that was used to create a backup when possible.

## 4. If installation is interrupted

Keep the saved backup. Reopen this same installer, reconnect the device and retry Install; it supports resuming an OTA loader. If the device no longer exposes a usable MIDI/OTA connection, stop repeated attempts: low-level recovery may require a JieLi Transporter. The installer links the upstream recovery information. Recovery has not been tested here.

The optional stock-restore section accepts only the exact supported official V15 package. It does not accept arbitrary vendor firmware or V16. Stock firmware cannot read Riddim OS projects. Do not use stock restore as a routine update step, and do not bypass package validation.

## License and source

Riddim OS is based on Felucca, GPL-3.0-only, with additional component licences. The update site includes corresponding source, LICENSE, LICENSING.md and LICENSES beside the firmware. No official vendor firmware is redistributed in this package.
