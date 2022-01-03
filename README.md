# Distributed-Computing-using-MPI

The latest version of MPICH is available at https://www.mpich.org/downloads/.

tar -xzf mpich3-3.2.tar.gz && cd mpich3-3.2

Once doing this, you should be able to configure your installation by performing `./configure`. 
If you need to install MPICH to a local directory (for example, if you don’t have root access to your machine), 
type `./configure --prefix=/installation/directory/path`. It is possible to avoid building the MPI Fortran library 
by using `./configure --disable-fortran` if you do not have Fortran compilers. For more information about possible 
configuration parameters, type `./configure --help`.

If you get error during configuration, just use the following command:

./configure --with-device=ch3

When configuration is done, it should say “Configuration completed.” Once this is through, it is time to build and 
install MPICH2 with `make; sudo make install`.

If your build was successful, you should be able to type `mpiexec --version`.

Then you can compile your c code using `mpicc -o test test.c -lm` and run it using `mpirun -np 8 ./test`.
