#include "include.cpp"

void led(/*void *arg*/int status, int num) {
    // int status = (int) (arg);
    // int num = (int) (arg + 1);
    std::string instr = "ioctl -d /dev/ledtest " + std::to_string(status) + " " + std::to_string(num);
    system(instr.c_str());
    return;
}