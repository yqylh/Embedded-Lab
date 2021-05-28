#ifndef NFC_MODULE
#define NFC_MODULE

#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>
#include <mtd/mtd-user.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/reboot.h>

//#include <cutils/sockets.h>
#include <termios.h>
#include <linux/kd.h>
//#include <linux/keychord.h>

//#include <sys/system_properties.h>
// #define LOG_TAG "bonovo_keypad"
//#include <cutils/log.h>

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>
#include <string>

class NFC{
public:
    NFC();
    ~NFC();
    int initNFC();
    int set_port_option(int fd,int nSpeed, int nBits, char nEvent, int nStop);
    void WakeUpNFC();
    std::string LabelNFC();
    int  handler(int );
	// std::string getData();

private:
	// std::string data;
    unsigned char buf[100];
    int fd;
    int writebyte,readbyte;
    int i;
    const unsigned char WakeUp[24];
    const unsigned char Label[11];
	// static const unsigned char NFC::WakeUp[] = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};
	// static const unsigned char NFC::Label[] = {0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD4, 0x4A, 0x01, 0x00, 0xE1, 0x00};
};

NFC::NFC():
	WakeUp {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00},
	Label {0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD4, 0x4A, 0x01, 0x00, 0xE1, 0x00}
{
    memset(buf,0,sizeof(buf));
    fd = -1;
    i=0;
    writebyte = readbyte = 0;
}

NFC::~NFC(){
    if(fd > 0)
        close(fd);
}

int NFC::initNFC(){
    fd = open("/dev/ttyS2",O_RDWR|O_NOCTTY|O_NONBLOCK);
    if(fd<0)
        return 0;
    set_port_option(fd,115200,8,'N',1);// 设置波特率
    return 1;
}

int NFC::set_port_option(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;

	tcflush(fd, TCIOFLUSH);

	if  ( tcgetattr( fd,&oldtio)  !=  0) 
	{ 
		printf("SetupSerial 1\n");
		return -1;
	}
	
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag  |=  CLOCAL | CREAD; 
	newtio.c_cflag &= ~CSIZE; 

	switch( nBits )
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	switch( nEvent )
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E': 
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':  
		newtio.c_cflag &= ~PARENB;
		break;
	}
	
	switch( nSpeed )
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}
	if( nStop == 1 )
	{
		newtio.c_cflag &=  ~CSTOPB;
	}
	else if ( nStop == 2 )
	{
		newtio.c_cflag |=  CSTOPB;
	}

	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 0;

	tcflush(fd,TCIOFLUSH);


	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		printf("com set error\n");
		return -1;
	}
	printf("set done!\n");
	
	return 0;
}

void NFC::WakeUpNFC()
{
	int a;
	writebyte =write(fd, WakeUp, sizeof(WakeUp));
	sleep(1); // 有可能导致bug
	readbyte=read(fd,buf,30);
	printf("readbyte == %d\n",readbyte);
	if(readbyte)
	{
		for(i=0;i<readbyte;i++)
		{
			printf("%x-",buf[i]);
		}
	}
	readbyte=0;
	memset(buf,0,30);
	printf("\n");
}

std::string NFC::LabelNFC(){
    writebyte =write(fd, Label, sizeof(Label));
	sleep(1);
	readbyte=read(fd,buf,30);
	printf("readbyte == %d\n",readbyte);
	char temp[100];
	if(readbyte)
	{
		for(i=0;i<readbyte;i++)
			printf("%x-",buf[i]);
		printf("\n");
		for(i=0;i<readbyte;i++)
		{
			if(buf[i]==0x8&&buf[i+1]==0x04)
			{
					printf("UID: ");
					printf("%x-",buf[i+2]);
					printf("%x-",buf[i+3]);
					printf("%x-",buf[i+4]);
					printf("%x",buf[i+5]);

					sprintf(temp,"UID: ");
					sprintf(temp,"%x-",buf[i+2]);
					sprintf(temp,"%x-",buf[i+3]);
					sprintf(temp,"%x-",buf[i+4]);
					sprintf(temp,"%x",buf[i+5]);
			}
		}
		readbyte=0;
		memset(buf,0,30);
	}
	printf("\n");
	return std::string(temp);
}

int NFC::handler(int code){
    if(code == 1)
        WakeUpNFC();
    else if (code == 2)
        LabelNFC();
	return 1;
}


// std::string NFC::getData(){
// 	char temp[100];
// 	sprintf(temp,"UID: ");
// 	sprintf(temp,"%x-",buf[i+2]);
// 	sprintf(temp,"%x-",buf[i+3]);
// 	sprintf(temp,"%x-",buf[i+4]);
// 	sprintf(temp,"%x",buf[i+5]);
// 	return std::string(temp);
// }

// const NFC::WakeUp = {0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD4, 0x14, 0x01, 0x17, 0x00};
// const NFC::Label = {0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD4, 0x4A, 0x01, 0x00, 0xE1, 0x00};


#endif