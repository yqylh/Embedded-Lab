#ifndef includeList
#define includeList
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#define HTTPSERVER_IMPL

#include "http.h"
int mem_fd;
unsigned short *cpld;
#endif