#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <Utix/Log.h>
#include <Utix/Vector.h>

using namespace utix;


struct NonPod
{
	NonPod() : id(s_i++) { Log("Constructor Of NonPod %d", id); }
	NonPod(int x) : id(x) { Log("Int constructor called With Id: %d", id); }

	NonPod(const NonPod& o) : id(o.id) { Log("Copy ctor"); }

	NonPod(NonPod&& o) noexcept : id(o.id), buffer(o.buffer) { 
		o.buffer = nullptr;
		o.id=-o.id; Log("Move ctor of NonPods %d", id);
	}
	~NonPod() { 
		Log("Destructor Of NonPod %d", id);
		std::free(buffer);
	} 

	volatile int id;
	volatile static int s_i;
	int * buffer = { (int*) std::malloc(1000) } ;
};

volatile int NonPod::s_i = 1;



int main()
{
	Log("Hello Utix test");

 	Vector<NonPod> v,v2;


	v.initialize({10,20,30});

	v2.initialize(std::move(v));
	v.initialize(std::move(v2));
	v2.initialize(std::move(v));
	v.initialize(std::move(v2));


	Log("printing V:");

	for(auto &x : v )
		Log("%d", x.id);



	Log("printing V2:");

	for(auto &x : v2 )
		Log("%d", x.id);




	return 0;
}
















