#!/bin/bash

DEST=${PWD}/out/package
VERSION=$(<./out/version.txt)
DATE=$(date +"%m%d%Y%H%M")
TARGET=ixwss_sdk_${VERSION}_${DATE}.tar.gz

function __prepare
{
    rm -rf $DEST;
    mkdir -p $DEST;
    printf "\tcreating the directory for package...\n";
}

function __copy_components()
{
    if [ -d $DEST ]; then
        cp -af ./out/ixwss_sdk/* $DEST;
        printf "\tcopying IXWSS software components...\n"
    fi
}

function __copy_docs()
{
    if [ -d $DEST ]; then
	mkdir -p $DEST/docs;
        cp -af ./docs/*User_Guide* $DEST/docs;
        cp -af ./out/api $DEST;
        printf "\tcopying documents...\n"
    fi
}

function __pack()
{
    if [ -d $DEST ]; then
        tar cvf ./out/${TARGET} ./out/package
        printf "\tpackaging the SDK...\n"
    fi

    echo "$TARGET has created."
}

__prepare;
__copy_components;
__copy_docs;
__pack
