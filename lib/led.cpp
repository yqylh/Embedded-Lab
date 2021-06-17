#include "include.cpp"

void led(/*void *arg*/int status, int num) {
    // std::string instr = "ioctl -d /dev/ledtest " + std::to_string(status) + " " + std::to_string(num);
    // std::cout << instr << std::endl;
    // std::cout << system(instr.c_str());
    int fd;
    fd = open("/dev/ledtest", O_RDWR | O_SYNC); /* 打开设备文件 */
    // if (fd == -1) return -1; /* TODO: throw an exception */
    ioctl(fd, status, num); /* 系统调用，设置 LED 的状态 */
    close(fd); /* 关闭设备文件 */
    return;
}
