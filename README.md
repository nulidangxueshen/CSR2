# CSR2
A New Format for SIMD-accelerated SpMV

manual:

(1) Enter the `make` command to compile the Makefile. CSR2_SPMV, CSR_SPMV, and check executable files will be generated in the current directory.  
`bhd@gorgon0:/mnt/home/bhd/test1/CSR2$ ls`  
`check  check.cpp  CSR2_SPMV  CSR2_SPMV.cpp  CSR_SPMV  CSR_SPMV.cpp  Makefile  README.md`  
`bhd@gorgon0:/mnt/home/bhd/test1/CSR2$ make`  
`icc -xCORE-AVX2 -qopt-prefetch=3 -fopenmp -O3 -o CSR2_SPMV CSR2_SPMV.cpp`  
`icc -xCORE-AVX2 -qopt-prefetch=3 -fopenmp -O3 -o CSR_SPMV CSR_SPMV.cpp`  
`icc -xCORE-AVX2 -qopt-prefetch=3 -fopenmp -O3 -o check check.cpp`  
`bhd@gorgon0:/mnt/home/bhd/test1/CSR2$ ls`   
`check  check.cpp  CSR2_SPMV  CSR2_SPMV.cpp  CSR_SPMV  CSR_SPMV.cpp  Makefile  README.md`  
(2) Use CSR2_SPMV to run the CSR2 accelerated SPMV program, Input:  
**`./CSR2_SPMV <matrix name> <vector name> <iterations times>`**  
  
`bhd@gorgon0:/mnt/home/bhd/test1/CSR2$ ls`  
`check  check.cpp  CSR2_SPMV  CSR2_SPMV.cpp  CSR_SPMV  CSR_SPMV.cpp  dense.mtx  dense_vector.mtx  Makefile  README.md`  
`bhd@gorgon0:/mnt/home/bhd/test1/CSR2$ ./CSR2_SPMV dense.mtx dense_vector.mtx 10000`  
`----------------dense.mtx----------------`  
`Number of iterations are 10000 times`  
`(2048 , 2048) , nzz_num = 4194304`  
`mtx_width = 64`  
`CsrConvertCsr2 Time : 7.020000 ms`  
`CSR2_Spmv Calculate Time : 0.156340 ms`  
`CSR2_Spmv Gflops : 53.656151 Gflops`  
`bandwidth = 499.858270 GB/s`  
`------------------------END----------------------`  
  
and the "answer_CSR2.txt" result file will be generated after the operation is completed;
(3) Use CSR_SPMV to run the SPMV program under CSR, input:  
                    **`./CSR_SPMV <matrix name> <vector name>`**  
`bhd@gorgon0:/mnt/home/bhd/test1/CSR2$ ./CSR_SPMV dense.mtx dense_vector.mtx`  
  
and the "answer_serial.txt" result file will be generated after the operation is completed;
(4) Run check, Input:  
                    **`./check answer_CSR2.txt answer_serial.txt`**  
`bhd@gorgon0:/mnt/home/bhd/test1/CSR2$ ls`  
`answer_CSR2.mtx    check      CSR2_SPMV      CSR_SPMV      dense.mtx         Makefile`  
`answer_serial.mtx  check.cpp  CSR2_SPMV.cpp  CSR_SPMV.cpp  dense_vector.mtx  README.md`  
`bhd@gorgon0:/mnt/home/bhd/test1/CSR2$ ./check answer_CSR2.mtx answer_serial.mtx`  
`Answer is Right! PASS`  

