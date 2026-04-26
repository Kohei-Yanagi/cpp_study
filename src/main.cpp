#include<iostream>//プログラムの読み込み
#include<iomanip>//文字の幅を取る
#include<print>
#include<numbers>
using namespace std;

int calculate(int a) {
	int total = a * 200;
	return total;
	
}
int main(int argc, char** argv) {
	
	int apple;

	cout << "How many apples do you have? ";
	cin >> apple;
	int judge = calculate(apple);

	if (judge > 2000) {
		cout << judge << "yen " << "Over your budget.";
	}
	else {
				cout << judge << "yen" << "Within your budget.";
	}


	return 0;
}

