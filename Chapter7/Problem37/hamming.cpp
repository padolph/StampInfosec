#include <iostream>
#include <sstream>
#include <cstdint>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	// input hex values
	uint64_t x, y;   
	stringstream ss1, ss2;
	ss1 << hex << string(argv[1]);
	ss1 >> x;
	ss2 << hex << string(argv[2]);
	ss2 >> y;
	cout << hex << x << " " << y << dec << endl;

	// xor and count bits
	uint64_t z = x ^ y;
	float c;
	for (c = 0; z; z >>= 1)
		c += z & 1;

	cout << "Hamming distance: " << c / 64 << endl;
}
