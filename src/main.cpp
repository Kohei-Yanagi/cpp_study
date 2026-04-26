#include<iostream>//プログラムの読み込み
#include<iomanip>//文字の幅を取る
#include<print>
#include<numbers>
using namespace std;


int main(int argc, char** argv) {
	
	int array[] = { 1,2,3,4,5 };

	for (int i = 0; i < 5; i++) {
		cout << array[i] << endl;
	}

	return 0;
}

