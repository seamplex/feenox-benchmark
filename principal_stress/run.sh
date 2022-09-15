#!/bin/bash

make clean; make OPT="-O0"
./principal_stress | tee results1.txt

make clean; make OPT="-O2"
./principal_stress | tee results2.txt

make clean; make OPT="-O3"
./principal_stress | tee results3.txt

make clean; make OPT="-Ofast"
./principal_stress | tee results4.txt

make clean; make OPT="-O3 -flto"
./principal_stress | tee results5.txt

make clean; make OPT="-Ofast -flto"
./principal_stress | tee results6.txt

