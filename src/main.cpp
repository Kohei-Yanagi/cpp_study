#include<iostream>//プログラムの読み込み
#include<iomanip>//文字の幅を取る
#include<print>
#include<numbers>
using namespace std;

void hello();

int main(int argc, char** argv) {
	hello();

	return 0;
}

void hello() {
	cout << "Hello, World!";
}