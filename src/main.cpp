#include<iostream>//プログラムの読み込み
#include<iomanip>//文字の幅を取る
#include<print>
#include<numbers>
using namespace std;

void nibai(int a) {
	cout << a * 2;
}

int main(int argc, char** argv) {
	
	int num = 10;
	
	nibai(num);

	return 0;
}

