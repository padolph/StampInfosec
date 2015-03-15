#include <iostream>
#include <sstream>
#include <cstdint>
#include <string>
#include <map>
#include <vector>

using namespace std;

int countBits(uint64_t);
float hamming64(uint64_t, uint64_t);
uint64_t hexToUint64(string);

struct Recog
{
	Recog(string name, uint64_t code) : name(name), code(code) {}
	string name;
	uint64_t code;
};

int main(int argc, char* argv[])
{
	map<string, uint64_t> enrolled;
	enrolled["Alice"]   = hexToUint64("BE439AD598EF5147");
	enrolled["Bob"]     = hexToUint64("9C8B7A1425369584");
	enrolled["Charlie"] = hexToUint64("885522336699CCBB");

	cout << "d(Alice, Bob)    : " << hamming64(enrolled["Alice"], enrolled["Bob"])     << endl;
	cout << "d(Alice, Charlie): " << hamming64(enrolled["Alice"], enrolled["Charlie"]) << endl;
	cout << "d(Bob,   Charlie): " << hamming64(enrolled["Bob"], enrolled["Charlie"])   << endl;

	vector<Recog> recog;
	recog.push_back(Recog("U", hexToUint64("C975A2132E89CEAF")));
	recog.push_back(Recog("V", hexToUint64("DB9A8675342FEC15")));
	recog.push_back(Recog("W", hexToUint64("A6039AD5F8CFD965")));
	recog.push_back(Recog("X", hexToUint64("1DCA7A54273497CC")));
	recog.push_back(Recog("Y", hexToUint64("AF8B6C7D5E3F0F9A")));

	for (vector<Recog>::iterator i = recog.begin(); i != recog.end(); ++i)
	{
		cout << "---------\n";
		cout << "d(" << i->name << ", Alice)  : " << hamming64(i->code, enrolled["Alice"]) << endl;
		cout << "d(" << i->name << ", Bob)    : " << hamming64(i->code, enrolled["Bob"]) << endl;
		cout << "d(" << i->name << ", Charlie): " << hamming64(i->code, enrolled["Charlie"]) << endl;
	}
}

uint64_t hexToUint64(string hexStr)
{
	uint64_t x;   
	stringstream ss;
	ss << hex << hexStr;
	ss >> x;
	return x;
}

int countBits(uint64_t x)
{
	int c;
	for (c = 0; x; x >>= 1)
		c += x & 1;
	return c;
}

float hamming64(uint64_t a, uint64_t b)
{
	return countBits(a ^ b) / 64.;
}
