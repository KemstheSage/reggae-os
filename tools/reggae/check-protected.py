#!/usr/bin/env python3
"""Keep the musician-approved organ/piano/bass PCM unchanged (40 cases)."""
import hashlib,json
from pathlib import Path
expected=json.loads(Path('assets/reggae-reference/protected-bank03.json').read_text())
for name,sha in expected.items():
    assert hashlib.sha256((Path('build/reggae')/name).read_bytes()).hexdigest()==sha,name
print(f'PASS: {len(expected)} protected organ/piano/bass renders unchanged')
