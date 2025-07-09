#!/bin/bash
set -ueo pipefail

mkdir -p vendor.bak/
mv vendor/{flac,flac-config} vendor.bak/ || true
mv vendor/CMakeLists.txt vendor/CMakeLists_original.txt || true
cp vendor/CMakeLists_arch.txt vendor/CMakeLists.txt

repo_dir="$(readlink -e .)"
mkdir -p build
cd build
mkdir -p lib32
ln -sf /usr/lib32/libX11.so lib32/libx11.so
ln -sf /usr/lib32/libFLAC.so lib32/libflac32.so
mkdir -p lib64
ln -sf /usr/lib/libX11.so lib64/libx11.so
ln -sf /usr/lib/libFLAC.so lib64/libflac64.so
ln -sf /usr/lib/libaudiofile.so lib64/libaudiofile64.so
cmake \
	-D CMAKE_BUILD_TYPE=Release \
	-DVERBOSE=TRUE \
	-DX11_X11_LIB=/usr/lib/libX11.so \
	-DX11=/usr/include/X11 \
	-DX11_X11_INCLUDE_PATH=/usr/include/X11 \
	-DX11_Xlib_INCLUDE_PATH=/usr/include/X11 \
	-DWITH_GUI=1 \
	-DWITH_FLAC=1 \
	-DCMAKE_LIBRARY_PATH=/usr/lib \
	-DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
	"-DCMAKE_C_FLAGS_RELEASE=-O3 -DNDEBUG -fmessage-length=0 -pipe -Werror -Waddress -Wchar-subscripts -Wcomment -Wformat -Wmissing-field-initializers -Wno-trigraphs -Wnonnull -Wparentheses -Wreturn-type -Wsequence-point -Wshadow -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wswitch-default -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wmaybe-uninitialized -Wenum-compare -Wimplicit-int -Wimplicit-function-declaration -Wmain -Wmissing-braces -Wpointer-sign -std=c99 -Wno-error=uninitialized -Wno-unused-const-variable -Wno-error=unused-const-variable= -Wno-error=sign-compare" \
	"-DCMAKE_CXX_FLAGS_RELEASE=-O3 -DNDEBUG -fmessage-length=0 -pipe -Waddress -Wchar-subscripts -Wcomment -Wformat -Wmissing-field-initializers -Wno-trigraphs -Wnonnull -Wparentheses -Wreturn-type -Wsequence-point -Wshadow -Wsign-compare -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wswitch-default -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wmaybe-uninitialized -Wc++11-compat -Wreorder     -I /usr/src/vst2sdk/pluginterfaces/vst2.x/ -Wno-error=deprecated-declarations -Wno-error=maybe-uninitialized  -Wno-error=alloc-size-larger-than=" \
	"-DCMAKE_EXE_LINKER_FLAGS=-Wl,-O1 -Wl,--sort-common -Wl,--as-needed -Wl,-z,relro -Wl,-z,now          -Wl,-z,pack-relative-relocs -flto=auto -L$repo_dir/build/lib64 -L$repo_dir/build/lib32" \
	..
make -j

cd "$repo_dir"
cp vendor/CMakeLists_original.txt vendor/CMakeLists.txt
mv vendor.bak/{flac,flac-config} vendor/
