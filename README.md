# bluez-dbus-cpp

Depending on the beautiful `sdbus-cpp` library, this library features class implementations for the most important LE-Advertisement and Gatt functions .

## Dependencies

- cmake
- build-essentials
- sdbus-cpp library https://github.com/Kistler-Group/sdbus-cpp

Please note that in order to comply with the LPGL license of sdbus-cpp, it is important to link against it dynamically.

Kistler have provided bitbake recipes for integrating `sdbus-cpp` into a yocto build. https://github.com/Kistler-Group/sdbus-cpp/blob/master/docs/using-sdbus-c++.md#yocto

## Building (with example)

```
mkdir build
cd build
cmake .. -DBUILD_EXAMPLE=1
make -j 4
```

To build the socket example:

```
cmake .. -DBUILD_EXAMPLE_ELL=1
```

## Running example

From the `build` directory

```
./bin/example
```

## Building for install
```
mkdir build
cd build
cmake ..
make -j 4
```

## Installing

```
sudo make install
```

## Using it in your project

In your CMakeLists.txt

```
find_package(bluez-dbus-cpp REQUIRED)
target_link_libraries( your-target-name PRIVATE bluez-dbus-cpp )
```
