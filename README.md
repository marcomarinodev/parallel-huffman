# Parallel Huffman Coding

## Motivation
Huffman code is an optimal prefix code that is commonly used for lossless data compression. We
require to implement a parallel application reading an ASCII file, computing the Huffman code and using the code to write a “compressed” version of the original file. Measures relative to parallelization should be taken both including and excluding read (write) times of the input (output) file in (from) memory.
(see [](https://en.wikipedia.org/wiki/Huffman_coding))

### TODO
- [X] write `seq_write_file_string` to write human readable compressed file
- [X] write `seq_compress`
- [X] use `seq_write_file_string` and `seq_compress` to generate a true result in order to test `par_compress`
- [X] create different compositions and make some measurements
- [X] implement the `fastflow` version
  - [X] make tests
- [ ] monitor performances to an external server