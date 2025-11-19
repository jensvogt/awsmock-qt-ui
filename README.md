
## Installation on Linux

### Dependencies
For Windows as well as for Linux the following dependencies are needed:

- FTPClient
- Curl library

### Building on Linux

Installation on Linux needs the qt-cmake executable in the path variable. The Linux version uses the GCC toolchain and
cmake to create the executables and the libraries. All executables and libraries are installed in the default directory

```
qt-cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local/awsmock-qt-ui .
cmake --build . --config Release -j32
cmake --install .
```

This will copy all necessary file to /usr/local/awsmock-qt-ui.


## Installation on Windows

### Dependencies
For Windows as well as for Linux the following dependencies are needed:

 - FTPClient
 - Curl library

### Building on Windows

Installation on Windows needs the qt-cmake executable in the %PATH% variable. The Windows uses the MSVC toolchain and
cmake to create the executables and the libraries. All executables and libraries are installed in the default directory
```C:\Program Files (x86)\awsmock-qt-ui```.

To build the executable, use:

```
qt-cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=C:/Program Files (x86)/awsmock-qt-ui .
cmake --build . --config Release -j8
cmake --install .
```

In order to package for Windows use (currently the Release is not working):
```
windeployqt6 Debug
```