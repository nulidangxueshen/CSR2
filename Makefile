all:CSR2_SPMV CSR_SPMV check

.PHONY : CSR2_SPMV CSR_SPMV check
#compilers(g++ or icc)

CC=icc
#CC = g++

#g++
#CSR2_SPMV:
#	$(CC) -march=native -fopenmp -O -o CSR2_SPMV CSR2_SPMV.cpp
#CSR_SPMV:
#	$(CC) -march=native -fopenmp -O -o CSR_SPMV CSR_SPMV.cpp
#check:
#	$(CC) -march=native -fopenmp -O -o check check.cpp

#icc
CSR2_SPMV:
	$(CC) -xCORE-AVX2 -fopenmp -ftz -IPF_fltacc -IPF_fma -O2 -o CSR2_SPMV CSR2_SPMV.cpp
CSR_SPMV:
	$(CC) -xCORE-AVX2 -ftz -IPF_fltacc -IPF_fma -qopt-prefetch=3 -qopenmp -O -o CSR_SPMV CSR_SPMV.cpp
check:
	$(CC) -xCORE-AVX2 -qopt-prefetch=3 -qopenmp -O -o check check.cpp

.PHONY:clean
	rm CSR2_SPMV CSR_SPMV check
