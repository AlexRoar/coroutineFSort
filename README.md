# Coroutine Files Sort

Sort numbers in files on pure C with the power of couroutines.

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
sh test.sh
```
