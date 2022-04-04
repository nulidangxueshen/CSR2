# CSR2
[A New Format for SIMD-accelerated SpMV](https://ieeexplore.ieee.org/document/9139720)

Our Paper :

H. Bian, J. Huang, R. Dong, L. Liu and X. Wang, "CSR2: A New Format for SIMD-accelerated SpMV," 2020 20th IEEE/ACM International Symposium on Cluster, Cloud and Internet Computing (CCGRID), Melbourne, Australia, 2020, pp. 350-359, doi: 10.1109/CCGrid49817.2020.00-58.

## manual:

(1) Enter the `make` command to compile the Makefile. CSR2_SPMV, CSR_SPMV, and check executable files will be generated in the current directory.  
  
`$ ls`  
`check  check.cpp  CSR2_SPMV  CSR2_SPMV.cpp  CSR_SPMV  CSR_SPMV.cpp  Makefile  README.md`  
`$ make`  
`icc -xCORE-AVX2 -qopt-prefetch=3 -fopenmp -O3 -o CSR2_SPMV CSR2_SPMV.cpp`  
`icc -xCORE-AVX2 -qopt-prefetch=3 -fopenmp -O3 -o CSR_SPMV CSR_SPMV.cpp`  
`icc -xCORE-AVX2 -qopt-prefetch=3 -fopenmp -O3 -o check check.cpp`  
`$ ls`   
`check  check.cpp  CSR2_SPMV  CSR2_SPMV.cpp  CSR_SPMV  CSR_SPMV.cpp  Makefile  README.md`  
  
(2) Run "CSR2_SPMV" , Input:  
**`./CSR2_SPMV <matrix name> <vector name> <iterations times>`**  
  
`$ ls`  
`check  check.cpp  CSR2_SPMV  CSR2_SPMV.cpp  CSR_SPMV  CSR_SPMV.cpp  dense.mtx  dense_vector.mtx  Makefile  README.md`  
`$ ./CSR2_SPMV dense.mtx dense_vector.mtx 10000`  
**`----------------dense.mtx----------------`  
`Number of iterations are 10000 times`  
`(2048 , 2048) , nzz_num = 4194304`  
`mtx_width = 64`  
`CsrConvertCsr2 Time : 7.020000 ms`  
`CSR2_Spmv Calculate Time : 0.156340 ms`  
`CSR2_Spmv Gflops : 53.656151 Gflops`  
`bandwidth = 499.858270 GB/s`  
`------------------------END----------------------`**  
  
and the "answer_CSR2.mtx" result file will be generated after the operation is completed;  
  
(3) Run "CSR_SPMV" , input:  
                    **`./CSR_SPMV <matrix name> <vector name>`**  
  
`$ ./CSR_SPMV dense.mtx dense_vector.mtx`  
  
and the "answer_serial.mtx" result file will be generated after the operation is completed;  

(4) Run "check" , Input:  
                    **`./check answer_CSR2.mtx answer_serial.mtx`**  
`$ ls`  
`answer_CSR2.mtx    check      CSR2_SPMV      CSR_SPMV      dense.mtx         Makefile`  
`answer_serial.mtx  check.cpp  CSR2_SPMV.cpp  CSR_SPMV.cpp  dense_vector.mtx  README.md`  
`$ ./check answer_CSR2.mtx answer_serial.mtx`  
**`Answer is Right! PASS`**  

## Note

(1) The experimental data comes from formerly the University of Florida Sparse Matrix Collection. For the convenience of the experiment, our program does not process the code in the comment part of the matrix file, so we delete the comment part in the downloaded matrix.  
  
(2) We found in the experiment that the storage format of the sparse matrix in the data is not the row main order but the column main order, so we reversed the read order when reading the data, like this:  
  
```fscanf(fp_mtx,"%d %d %lf",&col,&row,&val)```  
  
In the process of comparing CSR5 experiments, we also used Liu's CSR5 program to read data files in the same way to ensure the consistency of data reading.

(3)This program supports `g++` and `icc` compilation. The Makefile file in this program is initially set to be compiled by the `icc` compiler. You can also comment the `icc` compiled code and use the `g++` compiler in the file to compile. For comparison experiments, please choose The same compilation environment.  
  
(4)This experiment was tested on two CPU platforms: `Dual Socket Intel Xeon CPU E5-2670 v3` and `Intel Core i7-7700HQ CPU`.  
  
## Cite This:  
```
@INPROCEEDINGS{9139720, 
  author={Bian, Haodong and Huang, Jianqiang and Dong, Runting and Liu, Lingbin and Wang, Xiaoying},  
  booktitle={2020 20th IEEE/ACM International Symposium on Cluster, Cloud and Internet Computing (CCGRID)},   
  title={CSR2: A New Format for SIMD-accelerated SpMV},   
  year={2020},  
  volume={},  
  number={},  
  pages={350-359},  
  doi={10.1109/CCGrid49817.2020.00-58}
}
```
