# Parallel Huffman

## Report
This report presents a parallel ASCII file compression algorithm utilizing the [Huffman Coding Algorithm](https://en.wikipedia.org/wiki/Huffman_coding). The algorithm exploits the concurrency of contemporary multicore processors to enhance the performance of the traditional algorithm. The primary contributions of this work are as follows:

- A **parallel** implementation of the Huffman Coding Algorithm, employing C++ native threads, which are low-level primitives for creating and managing threads.
- A **parallel** implementation of the Huffman Coding Algorithm, utilizing the **[FastFlow](https://github.com/fastflow/fastflow) C++ library**, which facilitates the development of parallel applications, modeled as structured directed graphs of processing nodes, on multicore platforms.
- A **sequential** implementation of the Huffman Coding Algorithm, serving as a baseline for comparison.
- A series of experiments, written in Python Notebooks, to measure the performance of the three implementations, using varying input sizes and numbers of threads. The results are reported in terms of **execution time**, **speedup**, and **scalability**.
- The implementations of the algorithm.
- Integration tests to verify the correctness of the parallel solutions.

## Algorithm phases
After conducting an analysis, the following steps were identified as necessary to compress files using the Huffman Coding algorithm:

1. **File read**: The file to be compressed is read.
2. **Counting characters**: The frequency of each character in the file is counted.
3. **Build the Huffman tree**: A Huffman tree is built based on the character frequencies.
4. **Build the encoding table**: An encoding table is created, which maps each character to its respective Huffman code taken from the Huffman tree.
5. **Encoding phase**: The ASCII file is encoded using the encoding table.
6. **Compression phas**e: The compressed string is grouped into `bitset<8>` to actually compress the file.
7. **Write the compressed file**.

For more info check the docs folder.
