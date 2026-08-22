#!/bin/sh

set -e

REPO="https://github.com/kiammota/klib.git"
DEST="/tmp/klib-install"

echo "Cloning klib..."
git clone --depth=1 "$REPO" "$DEST"

cd "$DEST"

mkdir -p build
cd build

echo "Building..."
cmake ..
make

echo "Installing..."
sudo make install

cd /
rm -rf "$DEST"

echo "Done."
