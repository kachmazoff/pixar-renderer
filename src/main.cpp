#include <iostream>
#include "./bmp_writer.h"

using namespace std;

int main() {
    cout << "Good bye world :(\n";
    vector<vector<Color> > test(1024, vector<Color>(1024, Color(255, 0, 255)));
	write_bmp("test.bmp", 72, test);

    return 0;
}
