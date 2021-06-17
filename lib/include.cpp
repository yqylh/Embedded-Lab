#ifndef includeList
#define includeList

#include <stdlib.h>
#include <string>
#include <cstdlib>
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
#include "nfc.hpp"
// DEBUG
#ifdef EBUG
#include "./lib/test.cpp"
#endif

struct http_server_s *server;
NFC* nfc;
int matrixRet = 0;
int tubeRet = 0;

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
    http_response_body(response,"FAILED",6);
}

// 改变Ctrl + C
void handle_sigint(int signum){
    matrixRet = 1;
    tubeRet = 1;
    sleep(2);
    puts("我跑卤辣");
    free(server);
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
}

#endif