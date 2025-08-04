set -xe

clang -DPLATFORM_WASM --target=wasm32 -c -o aldrin-light.o wasm.c
wasm-ld --no-entry --allow-undefined --export-all -o aldrin-light.wasm aldrin-light.o
rm aldrin-light.o