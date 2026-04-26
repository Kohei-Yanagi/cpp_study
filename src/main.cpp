#include<iostream>//プログラムの読み込み
#include<print>
#include<numbers>
using namespace std;
int main(int argc, char** argv) {
	int num;
	cout << "number:";
	cin >> num; 
	
	switch(num) {
		case 1:
			cout << "john";
			break;
		case 2:
			cout << "michael";
			break;
		default:
			cout << "unknown";
			break;
	}

	return 0;
}