#include <iostream>
#include <set>
#include <string>


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
	Hello a;
	a.i = 2;
	Hello b;
	b.i = 3;
    Hello c;
    c.i = -1;
    Hello d;
    d.i = 10;
    Hello e;
    e.i = 7;
    Hello f;
    f.i = 1;
    Hello g;
    g.i = 20;
	//std::cout << hi.i << by.i << "\n";
	std::set<Hello> yo;
	yo.insert(a);
	yo.insert(b);
	yo.insert(c);
	yo.insert(d);
	yo.insert(e);
	yo.insert(f);
	yo.insert(g);

    for (const auto& hi : yo) {
        std::cout << hi.i << "\n";
    }





}
