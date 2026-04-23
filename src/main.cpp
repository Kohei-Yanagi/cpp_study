#include<iostream>
#include<print>
#include<numbers>
int main(int argc, char** argv) {
	std::println("Hello World");
	std::println("{:03d} {:+.5f} ", 3, std::numbers::pi);
}