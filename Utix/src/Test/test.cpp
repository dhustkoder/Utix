#include <iostream>
#include <stdexcept>
#include <Utix/Log.h>
#include <Utix/Vector.h>

using namespace utix;


struct NonPod
{
	NonPod() : id(s_i++) { /*Log("Constructor Of NonPod %d", id);*/ }
	NonPod(int x) : id(x) { /*Log("Int constructor called With Id: %d", id);*/ }

	NonPod(const NonPod& o) : id(o.id) 
	{ 
		Log("Copy ctor for NonPods %d", id);

//		if(id==20) 
//			throw std::runtime_error("ERR"); 
	}

	NonPod(NonPod&& o) : id(o.id) { o.id=-o.id; /*Log("Move ctor of NonPods %d", id);*/ }
	~NonPod() { /*Log("Destructor Of NonPod %d", id);*/ } 

	volatile int id;
	volatile static int s_i;
};

volatile int NonPod::s_i = 1;



int main()
{
	const auto x = 1000000;
	Log("Hello Utix test");
	Log("%zu | %zu", x, x *2);
	Vector<NonPod> v;
	v.reserve(x * 2);
	v.resize(x);
	v.emplace_back(0);










	return 0;
}
















