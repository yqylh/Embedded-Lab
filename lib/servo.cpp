#ifndef __servo__
#define __servo__
#include "include.cpp"

/***************************************************************
0:	停止
1:	停止
2：	逆时针
3：	顺时针
****************************************************************/

void servo(int status) {
    unsigned char data;
	int mem_fd;
	unsigned char *cpld;
	mem_fd = open("/dev/mem", O_RDWR);
	cpld = (unsigned char*)mmap(NULL,(size_t)0x04,PROT_READ | PROT_WRITE | PROT_EXEC,MAP_SHARED,mem_fd,(off_t)(0x8000000));
	if(cpld == MAP_FAILED) return;
    *(cpld+(0xe2<<1)) = status;   //步进电机地址0xe2<<1
	munmap(cpld,0x04);
	close(mem_fd);
}

#endif