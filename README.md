# MPI-Based Parallel Programs
This repository contains two C programs that demonstrate parallel computing concepts using MPI 

## 1. Matrix.c - Parallel Matrix Addition
### Description
This program uses MPI to perform parallel matrix addition. Two 3x3 matrices are initialized and added together. The computation is distributed across available processors.
### Features
- Initializes two 3x3 matrices.
- Flattens the matrices and distributes chunks to multiple processors.
- Each processor computes its portion of the matrix addition.
- Results are gathered and printed by the root process.
### How to Run
Compile and run with MPI:
```bash
mpicc Matrix.c -o matrix
mpirun -np 4 ./matrix
```
---

## 2. Caesar_Cipher_Encryption.c - Parallel Caesar Cipher
### Description
This program performs Caesar Cipher encryption or decryption using MPI. The workload is distributed among multiple processors, with the master process handling input/output and the worker processes performing the transformation.
### Features
- Supports both encryption and decryption.
- Input can be taken from the console or a file.
- Distributes the string to be encrypted/decrypted among worker processes.
- Uses Caesar Cipher logic with wrap-around for `x`, `y`, `z` and their uppercase equivalents.
### How to Run
Compile and run with MPI:
```bash
mpicc Caesar_Cipher_Encryption.c -o cipher
mpirun -np 4 ./cipher
```
You will be prompted to:
1. Choose the input mode (console or file).
2. Choose whether to encrypt or decrypt.
3. Provide the input string or file name.
---


## Requirements
- MPI Compiler (e.g., `mpicc`)
- MPI Runtime (e.g., `OpenMPI` or `MPICH`)
