#include<iostream>//プログラムの読み込み
#include<iomanip>//文字の幅を取る
#include<print>
#include<numbers>
#include<vector>

using namespace std;


int main(int argc, char** argv) {
	
	int num ;
	vector<int> array;

	cout << "Enter the number of elements: ";
	cin >> num;

	array.resize(num);

	for (int i = 0; i < num; i++) {
		cin >> array[i];
	}

	for (int i = 0; i < num; i++) {
		cout << i << "times" << array[i] << "\n";
	}

	return 0;
}

