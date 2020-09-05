#include <stdlib.h>
#include <string.h>
#include "solver.h"

#define VISITED 0x80
#define PATH 0x40
#define EMPTY 0x1
#define PREV 0x6
#define PREV_UP 0x0
#define PREV_DOWN 0x2
#define PREV_LEFT 0x4
#define PREV_RIGHT 0x6
#define NEXT 0x30
#define NEXT_UP 0x0
#define NEXT_DOWN 0x10
#define NEXT_LEFT 0x20
#define NEXT_RIGHT 0x30

int solver_solve_memory(unsigned long long column,
                        unsigned long long row,
                        unsigned char *buf,
                        FILE *fout_path,
                        FILE *fout_graph) {
    unsigned long long r = 0;
    unsigned long long c = 0;
    unsigned char *cur = buf;
    for (; c < column && !*cur; ++c, ++cur);
    unsigned char **queue = malloc(sizeof(unsigned char*) * column * row);
    if (!queue) {
        return 1;
    }
    unsigned char **queue_end = queue;
    unsigned long long queue_len = 0;
    unsigned long long offset = 0;
    unsigned char data = 0;
    *cur |= VISITED;
    *(queue_end++) = cur;
    ++queue_len;
    for (; queue_len;) {
        cur = *queue;
        memmove(queue, queue + 1, --queue_len * sizeof(unsigned char*));
        --queue_end;
        offset = cur - buf;
        r = offset / column;
        c = offset % column;
        if (r == row - 1) {
            break;
        }
        if (r && (cur[-column] & EMPTY) && !(cur[-column] & VISITED)) {
            cur[-column] |= VISITED;
            cur[-column] |= PREV_DOWN;
            *(queue_end++) = cur - column;
            ++queue_len;
        }
        if (r < row - 1 && (cur[column] & EMPTY) && !(cur[column] & VISITED)) {
            cur[column] |= VISITED;
            cur[column] |= PREV_UP;
            *(queue_end++) = cur + column;
            ++queue_len;
        }
        if (c && (cur[-1] & EMPTY) && !(cur[-1] & VISITED)) {
            cur[-1] |= VISITED;
            cur[-1] |= PREV_RIGHT;
            *(queue_end++) = cur - 1;
            ++queue_len;
        }
        if (c && (cur[1] & EMPTY) && !(cur[1] & VISITED)) {
            cur[1] |= VISITED;
            cur[1] |= PREV_LEFT;
            *(queue_end++) = cur + 1;
            ++queue_len;
        }
    }
    free(queue);
    if (r != row - 1) {
        return 2;
    }
    for (; r;) {
        *cur |= PATH;
        data = *cur;
        switch (data & PREV) {
        case PREV_UP:
            --r;
            cur -= column;
            *cur |= NEXT_DOWN;
            break;
        case PREV_DOWN:
            ++r;
            cur += column;
            *cur |= NEXT_UP;
            break;
        case PREV_LEFT:
            --c;
            --cur;
            *cur |= NEXT_RIGHT;
            break;
        case PREV_RIGHT:
            ++c;
            ++cur;
            *cur |= NEXT_LEFT;
            break;
        }
    }
    *cur |= PATH;
    if (fout_path) {
        fputs("PATH:\n", fout_path);
        for (; r < row - 1;) {
            fprintf(fout_path, "row: %llu col: %llu\n", r, c);
            data = *cur;
            switch (data & NEXT) {
            case NEXT_UP:
                --r;
                cur -= column;
                break;
            case NEXT_DOWN:
                ++r;
                cur += column;
                break;
            case NEXT_LEFT:
                --c;
                --cur;
                break;
            case NEXT_RIGHT:
                ++c;
                ++cur;
                break;
            }
        }
    }
    if (fout_graph) {
        cur = buf;
        for (r = 0; r < row; ++r) {
            for (c = 0; c < column; ++c, ++cur) {
                data = *cur;
                fputc((data & PATH) ? 'O' : ((data & EMPTY) ? ' ' : '*'),
                      fout_graph);
            }
            fputc(10, fout_graph);
        }
    }
    return 0;
}

static void file_to_buf(unsigned long long column,
                        unsigned long long row,
                        FILE *fin,
                        unsigned char *buf) {
    unsigned char *cur = buf;
    for (unsigned long long r = 0; r < row; ++r) {
        for (unsigned long long c = 0; c < column; ++c, ++cur) {
            *cur = fgetc(fin) == ' ';
        }
        fgetc(fin);
    }
}

int solver_solve_file(unsigned long long column,
                      unsigned long long row,
                      FILE *fin,
                      FILE *fout_path, 
                      FILE *fout_graph) {
    unsigned char *buf = malloc(column * row);
    if (!buf) {
        return 1;
    }
    file_to_buf(column, row, fin, buf);
    int ret = solver_solve_memory(column, row, buf, fout_path, fout_graph);
    free(buf);
    return ret;
}
