all:CSR2_SPMV CSR_SPMV check

.PHONY : CSR2_SPMV CSR_SPMV check
#compilers(g++ or icc)

CC=icc
#CC = g++

#g++
#CSR2_SPMV:
#       $(CC) -march=native -fopenmp -O3 -o CSR2_SPMV CSR2_SPMV.cpp
#CSR_SPMV:
#       $(CC) -march=native -fopenmp -O3 -o CSR_SPMV CSR_SPMV.cpp
#check:
#       $(CC) -march=native -fopenmp -O3 -o check check.cpp

#icc
CSR2_SPMV:
        $(CC) -xCORE-AVX2 -qopt-prefetch=3 -fopenmp -O3 -o CSR2_SPMV CSR2_SPMV.cpp
CSR_SPMV:
        $(CC) -xCORE-AVX2 -qopt-prefetch=3 -fopenmp -O3 -o CSR_SPMV CSR_SPMV.cpp
check:
        $(CC) -xCORE-AVX2 -qopt-prefetch=3 -fopenmp -O3 -o check check.cpp

.PHONY:clean
        rm CSR2_SPMV CSR_SPMV check
