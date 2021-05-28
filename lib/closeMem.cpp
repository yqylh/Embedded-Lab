#include "include.cpp"

void closeMem() {
    munmap(cpld, 0x20);
    close(mem_fd);
}