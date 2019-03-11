#!/bin/sh

cmake -D testing=ON UI=args -H. -Bbuild

cmake --build build

make test -C build ARGS='-V'
