#include <iostream>
#include <set>



class Hello {


	public:
		int i;

		bool operator* () {
		  return this->i;
		}
};

struct CompareByValue {
    bool operator()(const Hello& one, const Hello& two) const {
        return *one < *two;
    }
};

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
	std::set<<Hello>, CompareByValue> yo;
	yo.insert(a);
	yo.insert(b);
	yo.insert(c);
	yo.insert(d);
	yo.insert(e);
	yo.insert(f);
	yo.insert(g);




}
