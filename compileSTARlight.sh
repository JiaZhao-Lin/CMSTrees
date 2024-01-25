#!/bin/sh

if [ "$1" == "313" ] || [ "$1" == "300" ]; then
    echo "============================================"
    echo "- Compiling STARlight v"$1" ..."
    echo "- STARlight v3.13 (313) and v3.00 (300) are the two available versions in 'https://starlight.hepforge.org/downloads'."
    echo "- v3.13 was used for the official CMS STARlight MC production for Coherent/Incoherent Jpsi(Psi2S) analysis."
    echo ""

    slVer=$1
    wget 'https://starlight.hepforge.org/downloads?f=starlight_r'${slVer}'.tar'
    mv 'downloads?f=starlight_r'${slVer}'.tar' starlight_r${slVer}.tar
    mkdir -p starlightTrunk_v${slVer}/build
    mv starlight_r${slVer}.tar starlightTrunk_v${slVer}/
    cd starlightTrunk_v${slVer}
    tar xvf starlight_r${slVer}.tar
    rm -rf starlight_r${slVer}.tar
    cd build
    cmake ..
    make

else
    echo "============================================"
    echo "- Compiling Latest STARlight version from GitHub ..."
    echo "- Check 'https://github.com/STARlightsim/STARlight' for the latest version."
    echo ""

    if [ -d "STARlight" ]; then
        rm -rf STARlight
    fi
    git clone https://github.com/STARlightsim/STARlight.git
    cd STARlight
    mkdir build
    cd build
    cmake ..
    make
fi

echo "- Done."
echo "============================================"
