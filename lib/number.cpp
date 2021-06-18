#ifndef __number__
#define __number__
#include "include.cpp"

void *number(void *arg) {
    // 80 40 20 10 08 04 02 01
    int num = *(int*)arg;
    if (num < 0) num = -num;
	int mem_fd = open("/dev/mem", O_RDWR);
	unsigned char* cpld = (unsigned char*)mmap(NULL,(size_t)0x10,PROT_READ | PROT_WRITE | PROT_EXEC,MAP_SHARED,mem_fd,(off_t)(0x8000000));
    unsigned char tube[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x7f,0xff};
    unsigned char addr[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    while (1)
    {
        if (tubeRet == 1) {
            puts("程序结束");
            break;
        }
        int temp = num;
        for (int i = 0; i < 8; i++) {
            *(cpld+(0xe6<<1)) = addr[i];   //数码管地址
            *(cpld+(0xe4<<1)) = tube[temp % 10];   //数码管千位
            temp /= 10;
            usleep(1000);
        }
    }
	munmap(cpld,0x10);
	close(mem_fd);
    return arg;
}

#endif