#!/bin/bash

set -eu

pushd "$(dirname $0)"

if [ ! -v out ]; then
	out="/usr/local"
fi

mkdir -p "$out/lib"
mkdir -p "$out/include/gtkpoly"

install -t $out/lib ./build/libgtkpoly.a
install -t $out/include/gtkpoly ./include/gtkpoly/*

popd

