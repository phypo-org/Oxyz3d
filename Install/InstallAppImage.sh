#!/bin/bash

PROG_NAME=Oxyz3d
PROG_EXE=${PROG_NAME}.exe
PROG_PATH=../Debug/${PROG_EXE}
ICONS=../Run/share/icons
CONFIG=../Run/${PROG_NAME}.ini


DEST=${1}/${PROG_NAME}
DEST_USR=${1}/${PROG_NAME}/usr

DEST_BIN=${DEST}/usr/bin
DEST_LIB=${DEST}/usr/lib
DEST_SHR=${DEST}/usr/share
DEST_IMG=${DEST}/usr/share/icons
DEST_CFG=${DEST}/usr/share
DEST_BAS=${DEST}/usr/bases

# a enlever
# rm -rf  ${DEST_BIN} ${DEST_IMG} ${DEST_CFG}  ${DEST_LIB}

mkdir -p ${1} ${DEST}
mkdir -p ${DEST_USR} ${DEST_BIN} ${DEST_LIB} ${DEST_SHR} ${DEST_IMG} ${DEST_CFG}  ${DEST_BAS}


install_prog_lib.sh ${PROG_PATH} ${DEST_LIB}


cp -f ${PROG_PATH}  ${DEST_BIN}
cp -rp ${ICONS}/*   ${DEST_IMG}

cp -rp ${CONFIG}    ${DEST_SHR}


cp AppRun ${PROG_NAME}.desktop ${PROG_NAME}.png $DEST
