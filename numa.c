// (c) Daniel Stelter-Gliese, InnoGames GmbH 2016
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <numa.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Commands:\n");
        fprintf(stderr, "  bind <NODES>        Binds CPU to given nodeset\n");
        fprintf(stderr, "  membind <NODES>     Binds memory allocation to given nodeset\n");
        fprintf(stderr, "  interleave <NODES>  Interleaves memory allocation on given nodeset\n");
        fprintf(stderr, "  prefer <NODE>       Prefers given node for memory allocation\n");
        fprintf(stderr, "  alloc <BS> <TOTAL>  Allocates <TOTAL> MiB memory in <BS> MiB chunks.\n");
        return 1;
    }

    int max_node = numa_max_node();
    printf("NUMA nodes: %d\n", max_node+1);

    int i = 1;

    while (i < argc) {
        const char *cmd = argv[i];
        if (strcmp(cmd, "bind") == 0) {
            if (argc < i+1) {
                fprintf(stderr, "Usage: bind <NODE>\n");
                return 1;
            }

            struct bitmask* bm = numa_parse_nodestring(argv[i+1]);
            if (!bm) {
                printf("Illegal bitmask!\n");
                return 1;
            }
    
            numa_bind(bm);
            printf("CPU bound to nodes %s\n", argv[i+1]);
            i += 2;
        } else if (strcmp(cmd, "membind") == 0) {
            if (argc < i+1) {
                fprintf(stderr, "Usage: membind <NODE>\n");
                return 1;
            }

            struct bitmask* bm = numa_parse_nodestring(argv[i+1]);
            if (!bm) {
                printf("Illegal bitmask!\n");
                return 1;
            }
    
            numa_set_bind_policy(1);
            numa_set_membind(bm);
            numa_set_bind_policy(0);
            printf("Memory bound to nodes %s\n", argv[i+1]);
            i += 2;
        } else if (strcmp(cmd, "interleave") == 0) {
            if (argc < i+1) {
                fprintf(stderr, "Usage: interleave <NODE>\n");
                return 1;
            }

            struct bitmask* bm = numa_parse_nodestring(argv[i+1]);
            if (!bm) {
                printf("Illegal bitmask!\n");
                return 1;
            }
            numa_set_interleave_mask(bm);
            printf("Memory allocation interleaved on nodes %s\n", argv[i+1]);
            i += 2;
        } else if (strcmp(cmd, "prefer") == 0) {
            if (argc < i+1) {
                fprintf(stderr, "Usage: prefer <NODE>\n");
                return 1;
            }

            int node = atoi(argv[i+1]);
            if (node < 0 || node > max_node) {
                printf("Illegal node!\n");
                return 1;
            }
            numa_set_bind_policy(0);
            numa_set_preferred(node);
            printf("Memory allocation prefers node %d\n", node);
            i += 2;

        } else if (strcmp(cmd, "alloc") == 0) {
            if (argc < i+3) {
                fprintf(stderr, "Usage: alloc <MiB chunksize> <Total MiB>\n");
                return 1;
            }
    
            size_t blocksize = atoi(argv[i+1]);
            size_t total = atoi(argv[i+2]);
            if (blocksize < 1 || total < 1) {
                fprintf(stderr, "Illegal blocksize.\n");
                return 1;
            }
            printf("Allocating %d MiB in chunks of %d MiB.\n", total, blocksize);
            blocksize *= 1024*1024;
            total *= 1024*1024;
    
            for (size_t allocated=0; allocated < total; allocated += blocksize) {
                char *p = malloc(blocksize);
                memset(p, 0xFE, blocksize);
    
                printf(".");
                fflush(stdout);
                usleep(20*1000);
            }
            printf("\n");
            i += 3;
        } else {
            fprintf(stderr, "Unknown command.\n");
            return 1;
        }
    }

    printf("Done, inspect me. PID: %d\n", getpid());
    sleep(-1);
}
