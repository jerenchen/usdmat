# [USDMAT](https://github.com/jerenchen/usdmat)

A simple C++ header for reading [USD](https://openusd.org) mesh/point-based geoms as [Eigen](https://eigen.tuxfamily.org) matrices, intended for [libIGL](https://libigl.github.io), or as ML tensors.

## Dependencies

* [Pixar OpenUSD](https://openusd.org)
* [Eigen](https://eigen.tuxfamily.org)

## Demo
NOTE: To build the demo, ensure USD has *Alembic* & *HDF5* support.
```shell
git clone https://github.com/jerenchen/usdmat.git
cd usdmat/demo
mkdir build
cd build
# Replace `path-to-usd` & `path-to-eigen3` accordingly
cmake -DCMAKE_PREFIX_PATH="path-to-usd;path-to-eigen3" ..
make -j8
```
Once complete, run
```
./build/usdmat_demo
```
It will print out the dimensions of the matrices from the octopus example:
```
A: 31 x 282054
V: 94018 x 3, at A.row(0)
F: 188032 x 3
```

## License
This source code is licensed under the BSD 3-Clause "New" or "Revised" License