##!/bin/sh
rm libnup.a Make.defines Makefile config.h config.log
./configure --build=aarch64-unknown-linux-gnu
cd lib && make clean && make
cd ../libfree && make clean && make
cd ../sockopt && make clean && make
./sockopt
