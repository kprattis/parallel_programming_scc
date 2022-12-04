##### _Finding the Strongly Connected Components of the graph in a shared memory multiprocessor_

Created in Linux Ubuntu 22.04.
Processor: 

	AMD Ryzen 5 5600H with Radeon Graphics 

	Base speed:	3.30 GHz
	Sockets:	1
	Cores:	6
	Logical processors:	12


* To run Opencilk download compiler from: [https://www.opencilk.org/](https://www.opencilk.org/) and change the path of CC to its path at the corresponding makefile.

* Download Repository and cd to a folder. Then run "make" at command line to create the binary file at "bin" subfolder of the current.

* E.g. in folder omp_scc : **bin/scc_omp (.mtx path) (OPTIONAL: nthreads)**

* To run all the graphs use ./run_graphs.sh bash script
