#!/bin/bash


# general dependencies
if ! command -v python3 &>/dev/null; then
    echo "Installing python3"
    sudo apt install python3
fi
if ! command -v git &>/dev/null; then
    echo "Installing git"
    sudo apt install git
fi

# install SDL2
if ! dpkg -l libsdl2-dev 2>/dev/null | grep . -q; then
    sudo apt install libsdl2-dev
fi
if ! dpkg -l libsdl2-image-dev 2>/dev/null | grep . -q; then
    sudo apt install libsdl2-image-dev
fi
if ! dpkg -l libsdl2-ttf-dev 2>/dev/null | grep . -q; then
    sudo apt install libsdl2-ttf-dev
fi

# get emscripten
if ! [[ -d emsdk ]]; then
    echo "Getting emscripten"
    git clone https://github.com/emscripten-core/emsdk.git
fi

# configure emscripten
echo "Updating emscripten"
cd emsdk
git pull
export EMSDK_QUIET=1
./emsdk install latest >/dev/null
./emsdk activate latest >/dev/null
echo "Configuring emscripten"
source ./emsdk_env.sh >/dev/null
cd ..
echo "----------------"

# build libc_controlpads.a
here=$(pwd)
cd ..
if ! [[ -d c_controlpads ]]; then
    echo "ERROR: c_controlpads is required in a sister directory to potion_masters" >&2
    read -p "Clone the repo? (y/n): " response
    if [[ "$response" != "y" ]]; then
	echo "Script finished but c_controlpads not built"
	exit
    fi
    git clone git@github.com:/RecBox-Games/c_controlpads.git
fi

cd c_controlpads
rustup default nightly
cargo build --release
rustup default stable
cd "$here"
cp ../c_controlpads/target/release/libc_controlpads.a console/
echo "libc_controlpads.a copied"
