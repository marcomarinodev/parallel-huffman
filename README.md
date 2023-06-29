# Parallel Huffman Coding

### Motivation
Huffman code is an optimal prefix code that is commonly used for lossless data compression. We
require to implement a parallel application reading an ASCII file, computing the Huffman code and using the code to write a “compressed” version of the original file. Measures relative to parallelization should be taken both including and excluding read (write) times of the input (output) file in (from) memory.
(see [](https://en.wikipedia.org/wiki/Huffman_coding))

## Report
This report presents a parallel ASCII file compression algorithm utilizing the [Huffman Coding Algorithm](https://en.wikipedia.org/wiki/Huffman_coding). The algorithm exploits the concurrency of contemporary multicore processors to enhance the performance of the traditional algorithm. The primary contributions of this work are as follows:

- A **parallel** implementation of the Huffman Coding Algorithm, utilizing the **[FastFlow](https://github.com/fastflow/fastflow) C++ library**, which facilitates the development of parallel applications, modeled as structured directed graphs of processing nodes, on multicore platforms.
- A **parallel** implementation of the Huffman Coding Algorithm, employing C++ native threads, which are low-level primitives for creating and managing threads.
- A **sequential** implementation of the Huffman Coding Algorithm, serving as a baseline for comparison.
- A series of experiments, written in Python Notebooks, to measure the performance of the three implementations, using varying input sizes and numbers of threads. The results are reported in terms of **execution time**, **speedup**, and **scalability**.
- The implementations of the algorithm.
- Integration tests to verify the correctness of the parallel solutions.


### TODO
- [X] Sequential implementation
- [X] Native threads implementation
- [X] FastFlow implementation
- [X] Unit/Integration tests
- [X] Local machine tests 
  - [X] Python Notebook to keep track of tests and plots
- [X] Doxygen
- [ ] Remote Server performance tests
- [ ] Write Documentation