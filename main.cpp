#include "./lib/include.cpp"
#include "./lib/badapple.cpp"
#include "./lib/openMem.cpp"
#include "./lib/closeMem.cpp"
#ifdef EBUG
#include "./lib/test.cpp"
#endif
int main() {
    #ifdef EBUG
        test();
        return 0;
    #endif
    openMem();
    badapple();
    closeMem();
    return 0;
}