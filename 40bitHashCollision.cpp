// compile instructions
//1. download openssl library => sudo apt-get install libssl-dev
//2. compile file by linking the functions => g++ 40bitHashCollision.cpp -o collision -lssl -lcrypto


#include <iostream>
#include <iomanip>
#include <openssl/sha.h>
#include <cstring>
#include <unordered_map>
#include <random>
#include <climits>
#include <tuple>

using namespace std;

class SHA40bitsCollision {
	public:
		static tuple<string, string, int> hashCollision() {
			unordered_map<string, string> table;

			random_device rd;
			mt19937 gen(rd());

			int n = 1;
			while(true) {
				long random_num = gen();
				// cout << dec << "num: " << random_num << '\n';
				string m = intToHex(random_num);
				
				// cout << m << '\n';
				const unsigned char* str = reinterpret_cast<const unsigned char*>(m.c_str());
				
				unsigned char hash[20];
				SHA1(str, m.length(), hash);

				string strInt(reinterpret_cast<char*>(hash), 5);
				// stringstream ss;
				// for (int i = 0; i < 5; ++i) {
				// 	ss << hex << setw(2) << setfill('0') << (int)(unsigned char)hash[i];
				// }
				// string strInt = ss.str();


				if(table.find(strInt) != table.end()) {	
					if(table[strInt] == m) continue;			
					return make_tuple(table[strInt], m, n);
					// cout << "(" << table[strInt] << ", " << m << ", " << dec << n << ")" << '\n';
				} else {
					table.insert({strInt, m});
				}
				
				n++;
			}

			return make_tuple("", "", int(0));
		}

	private:
		static string intToHex(long num) {
			if (num == 0) return "0";

			string hexDigits = "0123456789ABCDEF";
			string result = "";

			while(num > 0) {
				result = hexDigits[num % 16] + result;
				num /= 16;
			}

			return result;
		}
};

int main() {
	auto [m1, m2, n1] = SHA40bitsCollision::hashCollision();
	auto [m3, m4, n2] = SHA40bitsCollision::hashCollision();
	while(m3 == m1 || m2 == m4) {
		tie(m3, m4, n2) = SHA40bitsCollision::hashCollision();
	}

	cout << "tuple 1:\n";
	cout << "(" << m1 << ", " << m2 << ", " << dec << n1 << ")" << '\n';
	cout << "\n";
	cout << "tuple 2:\n";
	cout << "(" << m3 << ", " << m4 << ", " << dec << n2 << ")" << '\n';

	return 0;
}

