##!/bin/sh
source clean.sh
./configure --build=aarch64-unknown-linux-gnu
cd lib && make clean && make
cd ../libfree && make clean && make
cd ../sockopt && make clean && make
./checkopts
