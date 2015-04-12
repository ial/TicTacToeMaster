#!/bin/sh

config=$1
if [ -z $1 ]; then
  config=gmake
fi

premake4 $config && cd build && make && cd ..
