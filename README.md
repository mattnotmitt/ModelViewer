# ModelViewer
[![Build Status](https://travis-ci.com/artemisbot/ModelViewer.svg?token=akN5nYTJtkxzyTRn2x8b&branch=master)](https://travis-ci.com/artemisbot/ModelViewer)
Continuation of [ModelLoader](https://github.com/artemisbot/ModelLoader) with VTK & Qt powered model visualisation software. This software has the capability to load STL files and the proprietary model format parsed in ModelLoader and view them.
Documentation can be found [here](https://artemisbot.github.io/ModelViewer/).
## Setup
```sh
mkdir build
cd build
cmake ..
make -jN # N specifies number of threads to make with
```
Documentation can be generated by running:
```sh
doxygen Doxyfile
```
This can be found in the `build/html` folder.

## Running
Program can be run by using this command:
```sh
./ModelViewer
```
Unit tests can be run by using:
```sh
make test
```