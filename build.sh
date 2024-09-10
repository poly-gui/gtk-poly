#!/bin/bash

# error when there are unset variables instead of silently ignoring
set -o nounset
set -o errexit

pushd "$(dirname $0)"

for arg in "$@"; do declare $arg='1'; done
if [ ! -v release ]; then debug=1; fi

if [ ! -f ./lib/nanopack/build.sh ] && [ ! -v nix ]; then
	echo "getting submodules..."
	git submodule update --init --recursive
fi

gtkmm_flags="$(pkg-config --cflags --libs gtkmm-4.0)"
compiler="${CC:-g++}"
ar="${AR:-ar}"
src_files=(
    src/rpc/nanopack_message_factory.np.cxx
    src/rpc/native_layer_service.np.cxx
    src/rpc/portable_layer_service.np.cxx
    src/rpc/widget/button.np.cxx
    src/rpc/widget/center.np.cxx
    src/rpc/widget/column.np.cxx
    src/rpc/widget/font_style.np.cxx
    src/rpc/widget/list_view.np.cxx
	src/rpc/widget/list_view_operation.np.cxx
    src/rpc/widget/list_view_batch_operations.np.cxx
    src/rpc/widget/list_view_delete_operation.np.cxx
    src/rpc/widget/list_view_insert_operation.np.cxx
    src/rpc/widget/list_view_item.np.cxx
    src/rpc/widget/list_view_item_config.np.cxx
    src/rpc/widget/make_list_view_operation.np.cxx
    src/rpc/widget/make_widget.np.cxx
    src/rpc/widget/row.np.cxx
    src/rpc/widget/slider.np.cxx
    src/rpc/widget/slider_value_changed_event.np.cxx
    src/rpc/widget/text.np.cxx
    src/rpc/widget/text_field.np.cxx
    src/rpc/widget/text_field_changed_event.np.cxx
    src/rpc/widget/widget.np.cxx
    src/rpc/event/click_event.np.cxx
    src/application.cxx
    src/window/window_manager.cxx
    src/widget/widget_factory.cxx
    src/widget/text.cxx
    src/widget/column.cxx
    src/widget/center.cxx
    src/widget/button.cxx
    src/widget/widget_registry.cxx
    src/widget/widget_updater.cxx
    src/widget/text_field.cxx
    src/widget/row.cxx
    src/widget/spacer.cxx
    src/widget/list_view.cxx
)

# build dependencies
if [ ! -v nix ]; then
	echo "building dependencies"
	./lib/nanopack/build.sh
else
	echo "building in nix, skipping dependency builds."
fi

if [ -v debug ]; then
	echo "building in debug mode."
fi
if [ -v release ]; then
	echo "building in release mode."
fi
echo "using ${compiler}."

common_opts="-I../include -Wall -Wno-unused-variable --std=c++20 ${gtkmm_flags}"
if [ ! -v nix ]; then common_opts="-I../lib/nanopack/include ${common_opts}"; fi
debug_opts="--debug --optimize -DDEBUG ${common_opts}"
release_opts="--optimize -DDEBUG=0 ${common_opts}"

if [ -v debug ]; then compile="$compiler ${debug_opts}"; fi
if [ -v release ]; then compile="$compiler ${release_opts}"; fi

mkdir -p build
cd build

all_src=""
for p in "${src_files[@]}"; do
	src="../${p}"
	out="$(basename ${src}).o"
	$compile -c $src -o $out
done

$ar -rcs libgtkpoly.a *.o

rm *.o

popd
