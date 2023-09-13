# Building

## Prerequisites

* clang trunk
* mold linker (recommended) or lld linker (if using mold, you will need to build clang with the gold plugin)
* libstdc++ (gcc's standard library, currently tested against 13.1.1)
* Boost 1.67.0+
* CMake 3.27+
* ninja 1.10+

## Building (Linux)

* `git clone https://github.com/davidstone/technical-machine.git`
* `cd technical-machine`
* `git submodule update --init`
* `cmake -G"Ninja" -B build -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=mold -DCMAKE_BUILD_TYPE=Release`
* `cmake --build build` (this step takes approximately 20 minutes on my AMD Ryzen 9 3950X)
