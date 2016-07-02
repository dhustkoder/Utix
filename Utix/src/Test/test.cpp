#include <iostream>
#include <Utix/NotNull.h>
using namespace utix;

class Bug {
public:
	Bug() {}
	~Bug() {}
	void CallBug() { std::cout << "You're the shit!\n"; }
};




int main(int argc, char** argv)
{
	NotNull<Bug*> bug(new(std::nothrow) Bug());

	(*bug).CallBug();
}
