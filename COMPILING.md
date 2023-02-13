# Compiling 

Some of the programs are coded in C++ for maximum efficiency over large data files on batch processing server backends.

To compile them, first install CMake for your platform (https://cmake.org/)

install PCL 
```
sudo apt install libpcl-dev
```

TODO: Provide detailed platform specific instructions to install CMake

Then, from the root directory of the repository, run the following commands:

```
mkdir build
cd build
cmake ..
```

This should give you a set of binary executables.
