#!/bin/bash

set -eu

NEWLINE=$'\n'

script_path="$(realpath $0)"
root="$(dirname $script_path)"
pushd "$(dirname "$0")" > /dev/null

flags="$(pkg-config --cflags --libs gtkmm-4.0) $(pkg-config --cflags --libs fontconfig)"

flag_yml_list=""
for f in ${flags}; do
  flag_yml_list+="    - ${f}"$'\n'
done

cat > .clangd <<- EOF
CompileFlags:
  Add:
    - -I$root/lib/nanopack/include
    - -I$root/src
    - -I$root/include
$flag_yml_list
EOF

popd > /dev/null
