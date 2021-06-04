#include "include.cpp"
void *badapple( void *arg) {
    int mem_fd = open("/dev/mem", O_RDWR);
    unsigned short *cpld = (unsigned short *)mmap(NULL, (size_t)0x20, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, mem_fd, (off_t)(0x8000000));
    if (cpld == MAP_FAILED) return arg;

    std::ifstream in("badapple2.txt");
	std::string str;
	while (std::getline(in, str, 'x')) {
		while (str.front() != '0' && str.front() != '1' ) str.erase(str.begin());
        int look = 0;
        int line = 0;
		for (auto & i : str) {
			if (i == '1') {
				look += 1;
                look <<= 1;
			} else if (i == '0') {
                look <<= 1;
			} else {
                look >>= 1;
                *(cpld + ((0xc0 + line) << 1)) = look;
                look = 0;
                line += 1;
			}
		}
		usleep(126000);

	}
    in.close();
    munmap(cpld, 0x20);
    close(mem_fd);
    return arg;
}

/*
    // for (int i = 0; i <= 15; i++) {
    //     *(cpld + ((0xc0 + i) << 1)) = 0xffff;
    // }


        int look = 0;
        int line = 0;
		for (auto & i : str) {
			if (i == '1') {
				look += 1;
                look <<= 1;
			} else if (i == '0') {
                look <<= 1;
			} else {
                // std::cout << look << std::endl;
				*(cpld + ((0xc0 + line) << 1)) = look;
                look = 0;
                line += 1;
			}
		}

*/

		// while (str.front() != '0' && str.front() != '1' ) str.erase(str.begin());
        // int arr[15][15];
        // int x = 0;
        // for (int i = 0; i < 15; i++) {
        //     for (int j = 0; j < 15; j++) {
        //         if (str[x] != '0' && str[x] != '1') {
		// 			x++;
		// 			j--;
        //             continue;
		// 		}
        //         if (str[x] == '1') {
        //             arr[i][j] = 1;
        //         } else {
        //             arr[i][j] = 0;
        //         }
        //         x++;
        //     }
        // }
        // for (int i = 0; i < 15; i++) {
        //     int look = 0;
        //     for (int j = 0; j < 15; j++) {
        //         if (arr[j][i] == 1) {
        //             look += 1;
        //             look <<= 1;
        //         } else {
        //             look <<= 1;
        //         }
        //     }
        //     *(cpld + ((0xc0 + i) << 1)) = look;
        // }
		// usleep(126000);