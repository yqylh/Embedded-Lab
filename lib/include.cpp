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
#include <signal.h>
#include "jsonxx/json.hpp"
#define HTTPSERVER_IMPL
#include "http.h"
// DEBUG
#ifdef EBUG
#include "./lib/test.cpp"
#endif

int mem_fd;
unsigned short *cpld;
struct http_server_s *server;

int request_target_is(struct http_request_s* request, char const * target) {
    http_string_t url = http_request_target(request);
    int len = strlen(target);
    return (len == url.len) && ( memcmp(url.buf, target, url.len) == 0 );
}
void set_response_ok(http_response_s* response){
    http_response_header(response,"Content-Type", "text/plain");
    http_response_body(response,"OK",2);
}
void set_response_fail(http_response_s* response){
    http_response_header(response,"Content-Type", "text/plain");
    http_response_body(response,"fail",4);
}

void openMem() {
    mem_fd = open("/dev/mem", O_RDWR);
    cpld = (unsigned short *)mmap(NULL, (size_t)0x20, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, mem_fd, (off_t)(0x8000000));
    if (cpld == MAP_FAILED) return;
}
void closeMem() {
    munmap(cpld, 0x20);
    close(mem_fd);
}

// 改变Ctrl + C
void handle_sigint(int signum){
    free(server);
    closeMem();
    exit(0);
}

std::string get_body_string (http_string_s& body){
    char* temp = new char[body.len+1];
    bcopy(body.buf, temp, body.len);
    temp[body.len] = 0;
    std::string str(temp);
    return str;
}

void init() {
    signal(SIGINT,handle_sigint);
    openMem();
}

#endif