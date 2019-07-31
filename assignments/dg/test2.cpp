#include <tuple>
#include <iostream>
#include <map>

int main() {
	std::map<char, int> s;
	s.insert({'a',1});
    s.insert({'a',1});
    s.insert({'a',2});
    std::cout << "size = " << s.size();
}
