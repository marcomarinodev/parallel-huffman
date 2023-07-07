# Parallel Huffman Coding

## Report
This report presents a parallel ASCII file compression algorithm utilizing the [Huffman Coding Algorithm](https://en.wikipedia.org/wiki/Huffman_coding). The algorithm exploits the concurrency of contemporary multicore processors to enhance the performance of the traditional algorithm. The primary contributions of this work are as follows:

- A **parallel** implementation of the Huffman Coding Algorithm, employing C++ native threads, which are low-level primitives for creating and managing threads.
- A **parallel** implementation of the Huffman Coding Algorithm, utilizing the **[FastFlow](https://github.com/fastflow/fastflow) C++ library**, which facilitates the development of parallel applications, modeled as structured directed graphs of processing nodes, on multicore platforms.
- A **sequential** implementation of the Huffman Coding Algorithm, serving as a baseline for comparison.
- A series of experiments, written in Python Notebooks, to measure the performance of the three implementations, using varying input sizes and numbers of threads. The results are reported in terms of **execution time**, **speedup**, and **scalability**.
- The implementations of the algorithm.
- Integration tests to verify the correctness of the parallel solutions.

### Algorithm phases
After conducting an analysis, the following steps were identified as necessary to compress files using the Huffman Coding algorithm:

1. **File read**: The file to be compressed is read.
2. **Counting characters**: The frequency of each character in the file is counted.
3. **Build the Huffman tree**: A Huffman tree is built based on the character frequencies.
4. **Build the encoding table**: An encoding table is created, which maps each character to its respective Huffman code taken from the Huffman tree.
5. **Encoding phase**: The ASCII file is encoded using the encoding table.
6. **Compression phas**e: The compressed string is grouped into `bitset<8>` to actually compress the file.
7. **Write the compressed file**.

### Performance tests
Since the goal of this project was to identify areas for improvement in the sequential solution 
using parallel computing techniques, I measured the execution time of each stage in the sequential
solution, enabling the identification of potential candidates for refactoring and enhancement using multithreading. Additionally, I performed performance tests (executed by a machine using an **Intel(R) Xeon(R) Gold 5120 CPU @ 2.20GHz and 32 cores**) on both the parallel implementations (native threads, and **[FastFlow](https://github.com/fastflow/fastflow)** C++ library). 

#### Stages performance
I decided to first benchmark how much each stages takes in the entire process of compression and here's what emerged by measuring the time taken from each stage (excluding the read/write and the actual compression) sequentially with three different file sizes which are `s=10MB`, `m=50MB`, and `l=100MB`:

| stages performance (%) | counting | huffman tree | encoding |
| ---------------------- | -------- | ------------ | -------- |
| s                      | 57.04    | 0.80         | 42.16    |
| m                      | 57.53    | 0.92         | 41.55    |
| l                      | 57.00    | 0.98         | 42.02    |

The reason why the Huffman tree phase is much quicker is because the tree has an upper bound equals to 256 which is the dimension of the ASCII table. Since the binary tree is filled with all the characters and symbolic nodes, to traverse the tree one pays only `O(1)`. A map was used to store for each character its Huffman encoding, in that way each time one wants to get in the encoding phase the code referred to that character it will be done in `O(1)`. The other stages are “expensive” because one has to count the characters in `O(n)` and encode the characters still in `O(n)`.


### Speedup and scalability tests
Recalling that *speedup* is a measure of the improvement in execution time achieved through parallelization and that `speedup(n) = Tseq / Tpar(n)` and *scalability* measures the ability of the system to maintain or improve its performance as the problem size or the number of processing units is scaled up `scalability(n) = Tpar(1) / Tpar(n)`. In the following table there is the speedup table, considering that the sequential time on average `Tseq = `

#### Speedup tests
| 10MB Speedup       | 2 threads | 4 threads | 8 threads | 16 threads | 32 threads | 64 threads |
| ------------------ | --------- | --------- | --------- | ---------- | ---------- | ---------- |
| **nt_results (s)** | 1.61      | 3.01      | 4.08      | 5.54       | 9.35       | 8.52       |
| **ff_results (s)** | 1.15      | 1.50      | 1.93      | 1.66       | 1.70       | 1.83       |

| 50MB Speedup       | 2 threads | 4 threads | 8 threads | 16 threads | 32 threads | 64 threads |
| ------------------ | --------- | --------- | --------- | ---------- | ---------- | ---------- |
| **nt_results (m)** | 1.37      | 2.35      | 3.70      | 5.01       | 5.93       | 6.39       |
| **ff_results (m)** | 1.15      | 1.58      | 1.68      | 1.65       | 1.92       | 1.88       |

| 100MB Speedup      | 2 threads | 4 threads | 8 threads | 16 threads | 32 threads | 64 threads |
| ------------------ | --------- | --------- | --------- | ---------- | ---------- | ---------- |
| **nt_results (l)** | 1.29      | 2.20      | 3.18      | 5.09       | 6.67       | 7.25       |
| **ff_results (l)** | 1.13      | 1.53      | 1.85      | 1.80       | 1.88       | 1.92       |


**Speedup test with 10MB file and `Tseq = `** 
| par. degree    |   2   |   4   |   8   |  16   |   32 |
| :------------- | :---: | :---: | :---: | :---: | ---: |
| native threads |       |       |       |       |      |
| fastflow       |       |       |       |       |      |

![](imgs/su_10.png)

**Speedup test with 50MB file and `Tseq = `**
| par. degree    |   2   |   4   |   8   |  16   |   32 |
| :------------- | :---: | :---: | :---: | :---: | ---: |
| native threads |       |       |       |       |      |
| fastflow       |       |       |       |       |      |

![](imgs/su_50.png)

**Speedup test with 100MB file and `Tseq = `**
| par. degree    |   2   |   4   |   8   |  16   |   32 |
| :------------- | :---: | :---: | :---: | :---: | ---: |
| native threads |       |       |       |       |      |
| fastflow       |       |       |       |       |      |

![](imgs/su_100.png)

#### Scalability tests

**Scalability test with 10MB file** 
| par. degree    |   2   |   4   |   8   |  16   |   32 |
| :------------- | :---: | :---: | :---: | :---: | ---: |
| native threads |       |       |       |       |      |
| fastflow       |       |       |       |       |      |

**Scalability test with 50MB file** 
| par. degree    |   2   |   4   |   8   |  16   |   32 |
| :------------- | :---: | :---: | :---: | :---: | ---: |
| native threads |       |       |       |       |      |
| fastflow       |       |       |       |       |      |

**Scalability test with 100MB file** 
| par. degree    |   2   |   4   |   8   |  16   |   32 |
| :------------- | :---: | :---: | :---: | :---: | ---: |
| native threads |       |       |       |       |      |
| fastflow       |       |       |       |       |      |


**Native threads execution times**
| par. degree |    2    |    4    |    8    |   16    |      32 |
| :---------- | :-----: | :-----: | :-----: | :-----: | ------: |
| small       | 591602  | 430787  | 279434  | 214148  |  198527 |
| medium      | 3704533 | 2404020 | 1466251 | 974799  |  829128 |
| large       | 6343420 | 4366054 | 3495891 | 2521686 | 2158098 |

**FastFlow execution times**
| par. degree |    2    |    4    |    8    |   16    |      32 |
| :---------- | :-----: | :-----: | :-----: | :-----: | ------: |
| s           | 778778  | 595902  | 564325  | 506217  |  470083 |
| m           | 3928956 | 3043096 | 2782430 | 2489869 | 2347214 |
| l           | 7799152 | 6077468 | 5518873 | 4985412 | 4640515 |

### How to run it

##### Performance tests

To create a random 10MB ascii file, just run:
`base64 /dev/urandom | head -c 10000000 > 10M`

in the inputs file"
```bash
total 153M
-rw-rw-r-- 1 m.marino38 m.marino38 96M Jul  5 19:09 100MB
-rw-rw-r-- 1 m.marino38 m.marino38 10M Jul  5 19:04 10M
-rw-rw-r-- 1 m.marino38 m.marino38 48M Jul  5 19:09 50M
```

To execute the native threads solution using 2 as parallelisation's degree on a 10MB file:
`./output n 1 2 ./inputs/10M`
