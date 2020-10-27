Steps:

```bash
$ git submodule update --remote -- freertos

$ cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=freertos/tools/cmake/toolchains/xtensa-esp32.cmake -GNinja

$ cmake --build build

$ cmake --build build --target flash
```