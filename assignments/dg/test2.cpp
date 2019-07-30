#include <tuple>
#include <iostream>

int main() {
	std::tuple<std::string, std::string> tup = std::make_tuple("one", "two");
	std::string &num1 = std::get<0>(tup);	
	std::cout << "num = " << *num1 <<  "\n";
}
