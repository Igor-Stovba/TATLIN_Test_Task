## Project

Structure:

1) `config.json` file contains different configuration options
    * `read_delay_ms` - the time (in milliseconds) it takes to read element from tape.
    * `write_delay_ms` - the time (in milliseconds) it takes to write element to specific place in tape
    * `step_move_ms` - the time (in milliseconds) it takes to еake a step to the left or right.
    * `rewind_to_beginning_ms` - the time (in milliseconds) it takes to rewind the tape to the very beginning.
    * `max_ram_bytes` - the amount of bytes which will be available for program as "RAM". This number is only applied to processing of tapes themselves. Total amount of bytes = `max_ram_bytes` + constant.
    * `path_tmp_dir` - the directory where temporary tapes will be stored.

2) `tests` directory contains different tests.

3) `include` and `src` directories which contain headers and source files, respectively.

## Build

```bash 
mkdir build
cd build
cmake ..
```

In order to build the binary itself:

```bash 
make bin-tape
```

In order to build tests:

```bash 
make tape_tests
```

## Run 

Everything is done through the ***cmake*** system

```bash
./bin-tape path-to-input-file path-to-output-file
```
or 
```bash
./bin-tape path-to-input-file path-to-output-file path-to-config-json
```

If ypu want to run tests, you should go to `build/tests` and

```bash
./tape_tests
```