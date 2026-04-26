#include<iostream>//プログラムの読み込み
#include<iomanip>//文字の幅を取る
#include<print>
#include<numbers>
using namespace std;
int main(int argc, char** argv) {
	
	int num = 10;
	do {
		cout << "message\n";
	} while (num < 5);

	while (num < 5) {
		cout << "message2";
	}

	return 0;
}