#!/bin/bash

PROG_NAME=Oxyz3d
PROG_EXE=${PROG_NAME}.exe
PROG_PATH=../Debug/${PROG_EXE}
ICONS=../Run/Icons
CONFIG=../Run/${PROG_NAME}.ini


DEST=${1}/${PROG_NAME}
DEST_BIN=${DEST}/
DEST_IMG=${DEST}/Icons
DEST_CFG=${DEST}/
DEST_LIB=${DEST}/Lib

# a enlever
# rm -rf  ${DEST_BIN} ${DEST_IMG} ${DEST_CFG}  ${DEST_LIB}

mkdir -p ${DEST_BIN} ${DEST_IMG} ${DEST_CFG}  ${DEST_LIB} 


install_prog_lib.sh $PROG_PATH $DEST_LIB


cp -f ${PROG_PATH}  ${DEST_BIN}
cp -rp ${ICONS}/*   ${DEST_IMG}

cp -rp ${CONFIG}    ${DEST}


cp AppRun ${PROG_NAME}.desktop ${PROG_NAME}.png $DEST
