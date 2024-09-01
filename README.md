# 个人课程项目简介
- 手撸了一个小型的多级反馈队列（3级）`mlq.c` 来保证进程的公平性以及cpu的使用效率，模拟随机到来的进程饿cpu时间分配调度。第一级用了固定时间间隙的FCFS队列，第二级用了RR队列，第三级用了可以被更高优先级任务打断的FCFS队列。详情请参考`stage1.pdf`. 
- 另外，写了一个简易的伙伴内存分配器 `mab.c` 来进行相应的进程内存管理。见`stage2.pdf`.

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
