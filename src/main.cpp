#include<iostream>//プログラムの読み込み
#include<iomanip>//文字の幅を取る
#include<print>
#include<numbers>
using namespace std;

void calculate(int x, int y) {
	int total = x + y;
	cout << "You have " << total << " fruits in total." << endl;
}
int main(int argc, char** argv) {
	
	int apple, orange;

	cout << "How many apples do you have? ";
	cin >> apple;
	cout << "How many oranges do you have? ";
	cin >> orange;
	calculate(apple, orange);

	return 0;
}

