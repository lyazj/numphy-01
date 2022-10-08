# Homework 1 of Numerical Physics, Fall 2022, PKU

Above all, [here](solution.pdf) comes the solution.

To build and test this project, you need a UNIX-like system with GNU Make toolchain.

To generate all executables, type:

    unix> make all

To run a program named 'program-name', type:

    unix> make <program-name>.run

NOTE: the standard output and standard error streams are redirected to \<program-name\>.out and \<program-name\>.log, respectively.

To run all programs in the current directory, type:

    unix> make run

To clean compiled object files as well as their output files, type:

    unix> make clean

To show the configuration of the current Make task, type:

    unix> make show
