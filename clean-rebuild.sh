#!/bin/sh

BUILD_DIR="build"
BASEDIR=$(dirname "$0");
TARGET=$BASEDIR"/"$BUILD_DIR"/"

rm -rf $TARGET;
mkdir -p $TARGET;
cd $TARGET;

cmake ..
make all

