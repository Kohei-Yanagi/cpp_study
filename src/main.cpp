#include<iostream>//プログラムの読み込み
#include<iomanip>//文字の幅を取る
#include<print>
#include<numbers>
using namespace std;
int main(int argc, char** argv) {
	
	int num = 1;
	int count = 0;

	while (num < 1000) {
		num *= 2;
		count++;
		cout << count << "times:" << num << "\n";
	}
	cout << count << "times are over 1000";
	return 0;
}