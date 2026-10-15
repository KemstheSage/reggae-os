#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-only
# Native Linux or macOS with Pillow built with libraqm: PYTHON=/path/to/python ./tools/reggae/generate.sh
set -eu
cd "$(dirname "$0")/../.."
"${PYTHON:-python3}" -c 'from PIL import features; assert features.check("raqm"), "Pillow needs libraqm; see BUILDING.md"; import sys; sys.path.insert(0,"tools"); import build; build.generate()'
