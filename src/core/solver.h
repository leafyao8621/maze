#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <stdio.h>

int solver_solve_memory(unsigned long long column,
                        unsigned long long row,
                        unsigned char *buf,
                        FILE *fout_path,
                        FILE *fout_graph);
int solver_solve_file(unsigned long long column,
                      unsigned long long row,
                      FILE *fin,
                      FILE *fout_path, 
                      FILE *fout_graph);

#endif
