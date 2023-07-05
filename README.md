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

## Performance tests
Since the goal of this project was to identify areas for improvement in the sequential solution 
using parallel computing techniques, I measured the execution time of each stage in the sequential
solution, enabling the identification of potential candidates for refactoring and enhancement using multithreading. Additionally, I performed performance tests (executed by a machine using an **Intel(R) Xeon(R) Gold 5120 CPU @ 2.20GHz and 32 cores**) on the parallel solution using both native threads anThis report presents a parallel ASCII file compression algorithm utilizing the [Huffman Coding Algorithm](https://en.wikipedia.org/wiki/Huffman_coding). The algorithm exploits the concurrency of contemporary multicore processors to enhance the performance of the traditional algorithm. The primary contributions of this work are as follows:

- A **parallel** implementation of the Huffman Coding Algorithm, utilizing the **[FastFlow](https://github.com/fastflow/fastflow) C++ library**, which facilitates the development of parallel applications, modeled as structured directed graphs of processing nodes, on multicore platforms.
- A **parallel** implementation of the Huffman Coding Algorithm, employing C++ native threads, which are low-level primitives for creating and managing threads.
- A **sequential** implementation of the Huffman Coding Algorithm, serving as a baseline for comparison.
- A series of experiments, written in Python Notebooks, to measure the performance of the three implementations, using varying input sizes and numbers of threads. The results are reported in terms of **execution time**, **speedup**, and **scalability**.
- The implementations of the algorithm.
- Integration tests to verify the correctness of the parallel solutions.d the fastflow framework. Multiple thread configurations, ranging from 1 thread to 32 threads, were evaluated to measure the speedup and scalability of the implementations. By comparing the sequential and parallel execution times, we gained insights into the effectiveness of multithreading for achieving speedup and scalability.

### Single stages tests

### Speedup and scalability tests
Recalling that *speedup* is a measure of the improvement in execution time achieved through parallelization and that `speedup(n) = Tseq / Tpar(n)` and *scalability* measures the ability of the system to maintain or improve its performance as the problem size or the number of processing units is scaled up `scalability(n) = Tpar(1) / Tpar(n)`. Here are the results:

#### Speedup tests

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
