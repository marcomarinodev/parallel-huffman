# Parallel Huffman Coding

## Motivation
Huffman code is an optimal prefix code that is commonly used for lossless data compression. We
require to implement a parallel application reading an ASCII file, computing the Huffman code and using the code to write a “compressed” version of the original file. Measures relative to parallelization should be taken both including and excluding read (write) times of the input (output) file in (from) memory.
(see [](https://en.wikipedia.org/wiki/Huffman_coding))

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