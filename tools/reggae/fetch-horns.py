#!/usr/bin/env python3
"""Restore the exact CC0 horn WAVs from the checked-in provenance manifest."""
import hashlib
import json
from pathlib import Path
import urllib.request

folder = Path(__file__).resolve().parents[2] / 'assets/samples-cc0/BRASS'
manifest = json.loads((folder / 'PROVENANCE.json').read_text())
for record in manifest['files']:
    path = folder / record['file']
    data = path.read_bytes() if path.exists() else urllib.request.urlopen(record['url'], timeout=60).read()
    if len(data) != record['bytes'] or hashlib.sha256(data).hexdigest() != record['sha256']:
        raise SystemExit(f'Unexpected sample bytes: {path.name}; refusing to replace the source')
    if not path.exists():
        path.write_bytes(data)
    print(f'Verified {path.name}')
