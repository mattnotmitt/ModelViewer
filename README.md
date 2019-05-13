# ModelViewer
Continuation of [ModelLoader](https://github.com/artemisbot/ModelLoader) with VTK & Qt powered model visualisation software. This software has the capability to load STL files and the proprietary model format parsed in ModelLoader and view them.
## Setup
```sh
mkdir build
cd build
cmake ..
make -jN # N specifies number of threads to make with
```

## Running
Program can be run by using this command:
```sh
./ModelViewer
```
Unit tests can be run by using:
```sh
make test
```