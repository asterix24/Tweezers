#! /bin/bash

cd tools/exiv2
./configure --enable-shared=no --enable-static=yes --prefix=$HOME/src/tweezers_cpp/tools/exiv2/../../lib/

make
make install

cd -

