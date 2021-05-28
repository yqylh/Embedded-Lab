#include "include.cpp"

void test() {
    // std::ifstream in("./badapple2.txt");
	// std::string str;
	// while (std::getline(in, str, 'x')) {
	// 	while (str.front() != '0' && str.front() != '1' ) str.erase(str.begin());
    //     int look = 0;
    //     int line = 0;
	// 	for (auto & i : str) {
	// 		if (i == '1') {
	// 			look += 1;
    //             look <<= 1;
	// 		} else if (i == '0') {
    //             look <<= 1;
	// 		} else {
    //             std::cout << look << std::endl;
    //             look = 0;
    //             line += 1;
	// 		}
	// 	}
	// 	usleep(126000);
	// }
    // in.close();
	std::ifstream in("badapple2.txt");
	std::string str;
	while (std::getline(in, str, 'x')) {
		
        int arr[15][15];
        int x = 0;
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
				if (str[x] != '0' && str[x] != '1') {
					x++;
					j--;
					continue;
				}
                if (str[x] == '1') {
                    arr[i][j] = 1;
                } else {
                    arr[i][j] = 0;
                }
                x++;
            }
        }
        for (int i = 0; i < 15; i++) {
            int look = 0;
            for (int j = 0; j < 15; j++) {
                if (arr[j][i] == 1) {
                    look += 1;
                    look <<= 1;
                } else {
                    look <<= 1;
                }
            }
        }
		usleep(126000);
	}
    in.close();
}