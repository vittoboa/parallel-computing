# parallel-computing

## Mandelbrot
Parallel program to compute the Mandelbrot set using C++ multithreading. It features both static and dynamic work allocation.

The program generates an image of the Mandelbrot set where each element of the set is assigned a color based on the thread responsible for it.

Static work allocation     |  Dynamic work allocation
:-------------------------:|:-------------------------:
![Static](https://user-images.githubusercontent.com/38300176/216814683-6084d74f-736b-4556-ab1a-3c93093a9082.png)  |  ![Dynamic](https://user-images.githubusercontent.com/38300176/216814621-c1a04a6d-2c46-44a2-92d5-ea09f648e2c8.png)
 
## 2D Heat Equation
Parallel program implemented with MPI to solve the 2D heat equation using Jacobi iterative method.

The program generates a matrix with the final temperature for each point, in the range 0-100.

![heat2d](https://user-images.githubusercontent.com/38300176/216820000-3cea1050-6c8e-47ac-b241-d210b1f7f9c3.png)

## Technologies
- C++
- MPI - Message Passing Interface

## Usage
Once g++ (C++ compiler) and mpic++ (MPI C++ wrapper compiler) have been installed, the application can be run from the command line:
```
# Clone this repository
$ git clone https://github.com/vittoboa/parallel-computing.git

# Go into the repository
$ cd parallel-computing

# Compile the programs
$ make

# Run Mandelbrot
$ ./mbrot --num-threads <num_threads> --work-allocation <static/dynamic>

# (Example) Run Mandelbrot on 32 threads and with dynamic work allocation
$ ./mbrot --num-threads 32 --work-allocation dynamic

# Run the heat equation solver
$ mpirun -n <num_processes> -ppn <num_processes_node> ./heat --cols <num_cols> --rows <num_rows> --epsilon <epsilon> --max-iterations <num_iterations>

# (Example) Run the heat equation solver with 96 processes, 16 processes per node,
#           on a matrix with 5000 columns and rows, an epsilon of 0.001 and at most 1000 iterations
$ mpirun -n 96 -ppn 16 ./heat --cols 5000 --rows 5000 --epsilon 0.001 --max-iterations 1000
```
