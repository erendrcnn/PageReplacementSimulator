# PageReplacementSimulator

## Overview
This repository contains a C program that simulates various page replacement algorithms. It helps in understanding how different algorithms perform in terms of page faults in a system with a specified number of page accesses.

## Key Features
- Simulates page replacement algorithms: FIFO, LRU, LFU, Random, and Weighted Random.
- Analyzes page fault rates for different algorithms.
- Easily configurable for different numbers of page accesses and memory sizes.

## Prerequisites
- GCC compiler

## Compilation
```
gcc -o pr pr.c
```

## Usage
Run the program with:
```
./pr [memory size]
```
Replace `[memory size]` with the desired size of the memory for the simulation.

I've added a section to the README.md for example inputs and outputs:

### Example Inputs and Outputs

This section will guide you through example inputs and outputs for the PageReplacementSimulator, demonstrating how the program operates with different page replacement algorithms and memory sizes.

#### Example 1
**Input:**
```
./pr 3
```
**Output:**
```
Sayfa Erisim Listesi: 0 1 2 1 0 3 2 1 4 3 2 5 4 3 6 5 4 7 6 5 
FIFO: 8
LRU: 18
LFU: 17
Random: 13.119000
Weighted random: 13.128000
Sayfa Erisim Listesi: 0 0 1 1 2 0 1 3 0 1 3 0 4 3 0 4 3 2 4 3 
FIFO: 12
LRU: 6
LFU: 11
Random: 9.313000
Weighted random: 8.740000
Sayfa Erisim Listesi: 0 0 1 1 2 3 4 0 1 5 2 3 0 1 4 6 2 0 1 3 5 4 0 1 
FIFO: 22
LRU: 22
LFU: 14
Random: 19.091000
Weighted random: 17.727000
Sayfa Erisim Listesi: 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 
FIFO: 20
LRU: 20
LFU: 20
Random: 12.000000
Weighted random: 12.866000
Sayfa Erisim Listesi: 1 2 3 4 1 5 6 7 1 5 8 9 1 5 8 0 1 5 10 11
FIFO: 20
LRU: 20
LFU: 20
Random: 16.871000
Weighted random: 14.679000
```

#### Example 2
**Input:**
```
./pr 4
```
**Output:**
```
Sayfa Erisim Listesi: 0 1 2 1 0 3 2 1 4 3 2 5 4 3 6 5 4 7 6 5 
FIFO: 8
LRU: 8
LFU: 15
Random: 10.957000
Weighted random: 10.765000
Sayfa Erisim Listesi: 0 0 1 1 2 0 1 3 0 1 3 0 4 3 0 4 3 2 4 3
FIFO: 6
LRU: 6
LFU: 7
Random: 6.635000
Weighted random: 6.622000
Sayfa Erisim Listesi: 0 0 1 1 2 3 4 0 1 5 2 3 0 1 4 6 2 0 1 3 5 4 0 1
FIFO: 22
LRU: 22
LFU: 14
Random: 15.865000
Weighted random: 14.598000
Sayfa Erisim Listesi: 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4
FIFO: 4
LRU: 4
LFU: 4
Random: 4.000000
Weighted random: 4.000000
Sayfa Erisim Listesi: 1 2 3 4 1 5 6 7 1 5 8 9 1 5 8 0 1 5 10 11
FIFO: 15
LRU: 12
LFU: 12
Random: 15.414000
Weighted random: 13.417000
```

These examples will help users understand how to run the program and what to expect in terms of output, providing a practical understanding of the page replacement algorithms' behavior.

## Contributing
Contributions to enhance the simulation or add more algorithms are welcome. Please adhere to standard coding practices.

## License
This project is licensed under the MIT License - see the LICENSE file for details.
