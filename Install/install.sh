#!/bin/bash


InstallAppImage.sh ../../Install/AppImage/
cd ../../Install/AppImage
export set ARCH=x86_64;~/Bin/appimagetool Oxyz3d
