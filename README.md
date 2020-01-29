# bluez-dbus-cpp

Depending on the beautiful `sdbus-cpp` library, this library features class implementations for the most important LE-Advertisement and Gatt functions .

## Dependencies

- cmake
- build-essentials
- sdbus-cpp library https://github.com/Kistler-Group/sdbus-cpp

Please note that in order to comply with the LPGL license of sdbus-cpp, it is important to link against it dynamically.

Kistler have provided bitbake recipes for integrating `sdbus-cpp` into a yocto build. https://github.com/Kistler-Group/sdbus-cpp/blob/master/docs/using-sdbus-c++.md#yocto

## Building

```
mkdir build
cd build
cmake .. -DBUILD_EXAMPLE=1
make
```

## Running

From the `build` directory

```
./bin/example
```
