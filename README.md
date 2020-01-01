# CSR2
A New Format for SIMD-accelerated SpMV

manual:

(1) Enter the "make" command to compile the Makefile. CSR2_SPMV, CSR_SPMV, and check executable files will be generated in the current directory.

(2) Use CSR2_SPMV to run the CSR2 accelerated SPMV program, Input: 
                    ./CSR2_SPMV <matrix name> <vector name> <iterations times>
and the "answer_avx2.txt" result file will be generated after the operation is completed;
(3) Use CSR_SPMV to run the SPMV program under CSR, input: 
                    ./CSR_SPMV <matrix name> <vector name>
and the "answer_serial.txt" result file will be generated after the operation is completed;
(4) Run check, Input: 
                    ./check answer_avx2.txt answer_serial.txt
