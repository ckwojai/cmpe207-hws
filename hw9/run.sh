##!/bin/sh
source clean.sh
./configure
cd lib && make clean && make
cd ../libfree && make clean && make
cd ../ping && make clean && make
./ping
