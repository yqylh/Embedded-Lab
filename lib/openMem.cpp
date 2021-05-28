#include "include.cpp"

void openMem() {
    mem_fd = open("/dev/mem", O_RDWR);
    cpld = (unsigned short *)mmap(NULL, (size_t)0x20, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, mem_fd, (off_t)(0x8000000));
    if (cpld == MAP_FAILED) return;
}