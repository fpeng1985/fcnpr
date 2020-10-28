#!/bin/sh

cmake -D testing=ON -D UI=args -H. -Bbuild

cmake --build build

make test -C build ARGS='-V'
