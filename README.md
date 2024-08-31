# How to compile the main program
1. `make clean` to remove possibly existing `mlq` `test` exec
2. `make` in CLI to create an `mlq` executable

# How to conduct tests
simply `make tests`
or 
1. `cp utility/process .` to copy `process` under `utility` to the same current working directory
2. `./mlq tests/test_file_<x>` to simulate. `<x>` could be 1 or 2
3. follow the prompts and set up integer params.

# How to run the C test file to unittest Mab.c APIs
simply `make unittest`
or you can manually do it by the following instructions
1. `gcc -o test mab.c pcb.c utility/test_mab.c` to create a`test` exec
2. `./test` and check the CLI messages.
