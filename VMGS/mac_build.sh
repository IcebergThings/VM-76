#!/bin/bash
cp ../VMDE/libVMDE.dylib .
make -f build/Makefile.mac $1
