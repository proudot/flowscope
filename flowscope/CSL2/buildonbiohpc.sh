module load cmake/3.10.2
module load gcc/8.3.0
module load tiff/gcc/4.0.7
export CC=/cm/shared/apps/gcc/8.3.0/bin/gcc
export CXX=/cm/shared/apps/gcc/8.3.0/bin/g++
mkdir build
cd build 

cmake ..
make
