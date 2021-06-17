Steps:

```bash
$ git submodule update --remote -- freertos

$ export IDF_PATH=`pwd`/freertos/vendors/espressif/esp-idf

$ source ./freertos/vendors/espressif/esp-idf/export.sh

$ cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=freertos/tools/cmake/toolchains/xtensa-esp32.cmake -GNinja

$ cmake --build build

$ cmake --build build --target flash
```