#ifndef IO_BLOCKING_H_
#define IO_BLOCKING_H_

#include <unistd.h>
#include <stdlib.h>

const size_t k_max_msg = 4096;

int32_t read_full(int fd, char *buf, size_t n);
int32_t write_all(int fd, const char *buf, size_t n);

#endif
