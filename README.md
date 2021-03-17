# Coroutine Files Sort

Sort numbers in files and merge them into one on pure C with the power of couroutines.

Mail.ru Tarantool Highload & SysProg first task

## Build
```bash
mkdir build
cd build
cmake ..
make
```
## Use
```bash
coSort <latency> [file1, file2, ...]
```

- latency - every coroutine will switch if execution time is longer than latency/(#routines)
- file1, ... - files to be sorted. Wil create new coroutine for each file.

## Testing
```bash
sh test.sh <latency> <numfiles>
```
```bash
sh test.sh 880 10
```

Output example:

```bash
$ sh test.sh 880 10
880 data/1.txt data/2.txt data/3.txt data/4.txt data/5.txt data/6.txt data/7.txt data/8.txt data/9.txt data/10.txt
Desired latency: 0.000880
Switched (id 0) 360 times, total coroutine time: 0.031792
Switched (id 1) 1459 times, total coroutine time: 0.129873
Switched (id 2) 3227 times, total coroutine time: 0.285185
Switched (id 3) 5625 times, total coroutine time: 0.498926
Switched (id 4) 8687 times, total coroutine time: 0.768576
Switched (id 5) 12411 times, total coroutine time: 1.094848
Switched (id 6) 16791 times, total coroutine time: 1.480242
Switched (id 7) 21834 times, total coroutine time: 1.923903
Switched (id 8) 27555 times, total coroutine time: 2.428438
Switched (id 9) 33931 times, total coroutine time: 2.991488
Total time: 11.633880, 131880 switches
Real latency: 0.000880
All is ok
```
