#include <iostream>
#include <set>
#include <string>
#include <vector>


class Hello {


	public:
		int i;

		bool operator <(const Hello &hi) const {
		    return this->i < hi.i;
		}

		friend std::ostream& operator<<(std::ostream& os, const Hello &hi);


};

std::ostream& operator<<(std::ostream& os, const Hello &hi)
{
    os << hi.i;
    return os;
}



/*
struct CompareByValue {
    bool operator()(const Hello& one, const Hello& two) const {
        return one.i < two.i;
    }
};
 */

int main() {
    std::vector<int> v{1,2,3,4,5};
    auto it = v.end();
    it--;
    std::cout << *it << "\n";


}
