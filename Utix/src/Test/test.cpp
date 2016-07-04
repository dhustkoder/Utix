#include <cstdio>
#include <iostream>
#include <memory>
#include <Utix/NotNull.h>


using namespace utix;

class Bug {
public:
	Bug() { std::cout << "A bug is born!\n"; }
	~Bug() { std::cout << "A bug has died!\n";}
	void CallBug() { std::cout << "You're the shit!\n"; }
};



#define LOG(x, y) std::printf("%s: %zX\n",x, reinterpret_cast<size_t>(y));
int main()
{
	NotNull<Bug*> b1( new Bug() );
	LOG("b1", b1.Get());
	auto b2 = b1;
	LOG("b2", b2.Get());
	LOG("b1", b1.Get());
	b1 = b2;
	LOG("b2", b2.Get());
	LOG("b1", b1.Get());
	delete b1;
	
}
