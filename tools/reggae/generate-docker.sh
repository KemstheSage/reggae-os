#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-only
# Debian's packaged Pillow includes libraqm. Plain pip wheels here lacked it.
set -eu
cd "$(dirname "$0")/../.."
docker run --rm -v "$PWD:/work" -w /work debian:bookworm-slim sh -c \
  'apt-get update -qq && apt-get install -y -qq --no-install-recommends python3-pil python3-fonttools libraqm0 && sh tools/reggae/generate.sh'
