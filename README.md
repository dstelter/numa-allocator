# numa-allocator
Small utility to experiment with NUMA memory binding policies

# Installation

```
make
```

# Requirements

`libnuma` development headers are required. On Debian-based systems, install `libnuma-dev`.

# Usage

```
Commands:
  bind <NODES>        Binds CPU to given nodeset
  membind <NODES>     Binds memory allocation to given nodeset
  interleave <NODES>  Interleaves memory allocation on given nodeset
  prefer <NODE>       Prefers given node for memory allocation
  alloc <BS> <TOTAL>  Allocates <TOTAL> MiB memory in <BS> MiB chunks.
```

Multiple commands will be executed in the order the are specified.

# Example
```
~# ./numa bind 0 membind 1 alloc 1 50 membind 0 alloc 1 100
NUMA nodes: 2
CPU bound to nodes 0
Memory bound to nodes 1
Allocating 1 MiB in chunks of 50 MiB.
..................................................
Memory bound to nodes 0
Allocating 1 MiB in chunks of 100 MiB.
....................................................................................................
Done, inspect me. PID: 54261
```

```
~# numastat -p 54261

Per-node process memory usage (in MBs) for PID 54261 (numa)
                           Node 0          Node 1           Total
                  --------------- --------------- ---------------
Huge                         0.00            0.00            0.00
Heap                         0.01            0.00            0.01
Stack                        0.01            0.00            0.01
Private                    100.52           51.58          152.10
----------------  --------------- --------------- ---------------
Total                      100.53           51.58          152.11
```

# Credits
This utility was created at [InnoGames](https://github.com/innogames/).
