#include <stdlib.h>
#include <time.h>
#include "generator.h"
#include "../util/mt19937.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int generator_generate_memory(unsigned long long column,
                              unsigned long long row,
                              unsigned char **buf) {
    unsigned char *temp = calloc(row * column, 1);
    if (!temp) {
        return 1;
    }
    *buf = temp;
    struct MT19937 gen;
    mt19937_initialize(&gen, time(0));
    unsigned long long r = 0;
    unsigned long long c = 1 + mt19937_gen(&gen) % (column - 2);
    unsigned char *cur = temp + c;
    char dir = 0;
    char valid = 0;
    for (; r < row - 1;) {
        *cur = 1;
        for (valid = 0; !valid;) {
            dir = mt19937_gen(&gen) & 3;
            switch (dir) {
            case UP:
                valid = r > 1 && !cur[-column];
                break;
            case LEFT:
                valid = r > 1 && c > 1 && !cur[-1];
                break;
            case RIGHT:
                valid = r > 1 && c < column - 2 && !cur[1];
                break;
            case DOWN:
                valid = 1;
                break;
            }
        }
        switch (dir) {
        case UP:
            --r;
            cur -= column;
            break;
        case DOWN:
            ++r;
            cur += column;
            break;
        case LEFT:
            --c;
            --cur;
            break;
        case RIGHT:
            ++c;
            ++cur;
            break;
        }
    }
    *cur = 1;
    cur = temp + column + 1;
    for (r = 1; r < row - 1; ++r) {
        for (c = 1; c < column - 1; ++c, ++cur) {
            if (!*cur && mt19937_gen(&gen) & 1) {
                *cur = 1;
            }
        }
        ++cur;
        ++cur;
    }
    return 0;
}

static void buf_to_file(unsigned long long column,
                        unsigned long long row,
                        unsigned char *buf,
                        FILE *fout) {
    unsigned char *cur = buf;
    for (unsigned long long r = 0; r < row; ++r) {
        for (unsigned long long c = 0; c < column; ++c, ++cur) {
            fputc(*cur ? ' ' : '*', fout);
        }
        fputc(10, fout);
    }
}

int generator_generate_file(unsigned long long column,
                            unsigned long long row,
                            FILE *fout) {
    unsigned char *temp = 0;
    if (generator_generate_memory(column, row, &temp)) {
        return 1;
    }
    buf_to_file(column, row, temp, fout);
    free(temp);
    return 0;
}

