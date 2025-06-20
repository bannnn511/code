#!/bin/sh
set -e
set -x
patchelf --set-rpath $PWD/pizfix/yolo/lib:$PWD/pizfix/lib64:$PWD/pizfix/lib pizfix/lib/libc.so
patchelf --set-rpath $PWD/pizfix/yolo/lib:$PWD/pizfix/lib64:$PWD/pizfix/lib pizfix/lib/libpizlo.so
patchelf --set-rpath $PWD/pizfix/yolo/lib:$PWD/pizfix/lib64:$PWD/pizfix/lib pizfix/lib/libc++.so.1.0
patchelf --set-rpath $PWD/pizfix/yolo/lib:$PWD/pizfix/lib64:$PWD/pizfix/lib pizfix/lib/libc++abi.so.1.0
cd pizfix
mkdir os-include
cd os-include
ln -s /usr/include/linux .
ln -s /usr/include/x86_64-linux-gnu/asm .
ln -s /usr/include/asm-generic .
cd ../..
set +x
echo
echo "You are all set. Try compiling something with:"
echo
echo "    build/bin/clang -o whatever whatever.c -O2 -g"
echo
echo "or:"
echo
echo "    build/bin/clang++ -o whatever whatever.cpp -O2 -g"
echo
echo "Take a look at pizfix/stdfil-include/stdfil.h for Fil-C-specific APIs. You can"
echo "optionally #include <stdfil.h> if you find those APIs useful."
echo
echo "New releases are at: https://github.com/pizlonator/llvm-project-deluge/releases"
echo "The Fil-C Manifesto: https://github.com/pizlonator/llvm-project-deluge/blob/deluge/Manifesto.md"
echo
echo "Have fun and thank you for trying filc-0.668.7-linux-x86_64."
