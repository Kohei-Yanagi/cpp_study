#include<iostream>//プログラムの読み込み
#include<print>
#include<numbers>
using namespace std;
int main(int argc, char** argv) {
	int num;
	cout << "number:";
	cin >> num; 
	
	if (num == 10)
		cout << "true";
	else if (num < 10)
		cout << "small";
	else if (num > 10)
		cout << "big";
	

	return 0;
}