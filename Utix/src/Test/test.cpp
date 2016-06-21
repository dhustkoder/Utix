#include <iostream>
#include <stdexcept>
#include <vector>
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

		if(id==20) 
			throw std::runtime_error("ERR"); 
	}

	NonPod(NonPod&& o) : id(o.id) { o.id=-o.id; /*Log("Move ctor of NonPods %d", id);*/ }
	~NonPod() { /*Log("Destructor Of NonPod %d", id);*/ } 

	volatile int id;
	volatile static int s_i;
};

volatile int NonPod::s_i = 1;



int main()
{

	Log("Hello Utix test");

//	std::vector<NonPod> v;
	Vector<NonPod> v;

	v.reserve(1000000);
	for(int i = 0; i < 1000000; ++i)
		v.emplace_back(i);


	return 0;
}
















