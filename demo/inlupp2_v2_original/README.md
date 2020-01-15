# Inlupp 2


This documentation shows how to use and build the warehouse application.
We reccomend viewing test_warehouse.c for a more detailed view on how the program works.


## Build and Running

This instructions assume you are using a Linux machine or equivalent.


- To build and run the program, use **make run**  
      Launch the main program which has a user interface that you can navigate around the program with and test the programs functionality.

- To build and run the CUnit tests, use **make test**  
      Run the tests using CUnit.
      
- To build and run a memory check, use **make mem**  
      Runs valgrind on the the test file with the flags --leak-check=yes -v
      
      

## Prerequisites
These are our recommended prerequisites. The instructions below may or may not work if you fail to fulfill them.

Linux machine or equivalent (preferably Ubuntu)  
_valgrind_ (for checking memory leaks)  
_gcc_ (for compiling and linking)  
_make_ (for using our makefile)  
_CUnit_  (for unit testing)


