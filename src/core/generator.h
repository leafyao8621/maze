#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <stdio.h>

int generator_generate_memory(unsigned long long column,
                              unsigned long long row,
                              unsigned char **buf);
int generator_generate_file(unsigned long long column,
                            unsigned long long row,
                            FILE *fout);

#endif
