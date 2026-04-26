#include<iostream>//プログラムの読み込み
#include<iomanip>//文字の幅を取る
#include<print>
#include<numbers>
#include<vector>

using namespace std;

void average(int x[]) {

	int sum = 0;

	for (int i = 0; i < 3; i++) {
		sum += x[i];
	}

	cout << "average is " << sum / 3 << "\n";
}

int main(int argc, char** argv) {
	
	int array[3] = { 78, 94, 64 };
	average(array);

	return 0;
}

