#!/bin/bash



DATE=`date +%Y%m%d_%H%M`
DATEAAMM=`date +%Y%m`

OPT=-Jcvf
EXT=tar.xz

PATH_SAV="../"

FILE="${PATH_SAV}/Oxyz3d${DATE}.${EXT}"
PATH_SAV_OXYZ="/media/phipo/GrossoModo/Archive/Dev/Oxyz3d/${DATEAAMM}"


EXCLUDE='
    --exclude=*.apk
    --exclude=*.ap_
    --exclude=*.dex

    --exclude=*.ncb

    --exclude=*.o
    --exclude=*.mt
    --exclude=*.a
    --exclude=*.so*
    --exclude=*.exe
    --exclude=*.dll
    --exclude=*.obj
    --exclude=*.lib*
    --exclude=*.old.*
    --exclude=*.old*
    --exclude=*.sql.*
    --exclude=configure
    --exclude=.nfs*
    --exclude=*.
    --exclude=*.pdb

    --exclude=*.class*
    --exclude=*.jar*

    --exclude=dat
    --exclude=log
    --exclude=doc/html
    --exclude=*.session*
    --exclude=dat.*
    --exclude=dat_*
    --exclude=log.*
    --exclude=log_*
    --exclude=toto*
    --exclude=titi*
    --exclude=tutu*
    --exclude=tyty*
    --exclude=tata*
    --exclude=tete*
    --exclude=tmp*
    --exclude=*.wav
    --exclude=mem.err*
    --exclude=*.core
    --exclude=core.*
    --exclude=core
    --exclude=*.log*
    --exclude=*.dat*
    --exclude=*.histo*
    --exclude=*.out*
    --exclude=*.tar
    --exclude=*.zip
    --exclude=*.gzip
    --exclude=*.z
    --exclude=*~
    --exclude=#*#
    --exclude=.#*#
    --exclude=*.ilk
    --exclude=*.csv
    --exclude=.vs*
    --exclude=*/Debug
    --exclude=*/x64
    --exclude=.git*
    --exclude=.git*/*
    --exclude=*/.git*/*/*
    --exclude=*/doc/html/*
    --exclude=*.doxy

'


rm -f ${FILE}
tar ${OPT}  ${FILE}  $EXCLUDE *

mkdir ${PATH_SAV_OXYZ}
cp -p ${FILE}  ${PATH_SAV_OXYZ}

