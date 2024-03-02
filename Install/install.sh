#!/bin/bash


rm-rf AppImage
mkdir -p AppImage

InstallAppImage.sh AppImage/
cd AppImage
export set ARCH=x86_64; appimagetool Oxyz3d
