#include <stdlib.h>
#include <time.h>
#include "../core/generator.h"
#include "../core/solver.h"

int main(int argc, const char **argv) {
    if (argc < 5) {
        puts("row col -/mazefn iter/pathfn solfn");
        return 1;
    }
    unsigned long long row = atoll(argv[1]);
    if (row == (unsigned long long)-1) {
        puts("row invalid");
        return 1;
    }
    unsigned long long column = atoll(argv[2]);
    if (column == (unsigned long long)-1) {
        puts("col invlid");
        return 1;
    }
    if (*argv[3] != '-') {
        FILE *fmaze = fopen(argv[3], "w");
        if (!fmaze) {
            printf("Cannot open %s\n", argv[4]);
            return 2;
        }
        int ret = generator_generate_file(column, row, fmaze);
        fclose(fmaze);
        if (ret) {
            puts("Allocation error");
            return 3;
        }
        FILE *fmaze_in = fopen(argv[3], "r");
        FILE *fpath = fopen(argv[4], "w");
        if (!fpath) {
            printf("Cannot open %s\n", argv[4]);
            fclose(fmaze_in);
            return 2;
        }
        FILE *fgraph = fopen(argv[5], "w");
        if (!fgraph) {
            printf("Cannot open %s\n", argv[4]);
            fclose(fmaze_in);
            fclose(fpath);
            return 2;
        }
        ret = solver_solve_file(column, row, fmaze_in, fpath, fgraph);
        fclose(fmaze_in);
        fclose(fpath);
        fclose(fgraph);
        if (ret) {
            puts(ret == 1 ? "Allocation error" : "No solution");
            return 3;
        }
    } else {
        unsigned long long iter = atoll(argv[4]);
        if (iter == (unsigned long long)-1) {
            return 1;
        }
        time_t start = time(0);
        for (unsigned long long i = 0; i < iter; ++i) {
            unsigned char *buf = 0;
            generator_generate_memory(column, row, &buf);
            solver_solve_memory(column, row, buf, 0, 0);
            free(buf);
        }
        time_t end = time(0);
        printf("time elapsed: %ld\n", end - start);
    }
}
