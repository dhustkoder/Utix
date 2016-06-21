#include <cstdio>
#include <stdexcept>
#include <Utix/Log.h>
#include <Utix/Vector.h>

using namespace utix;


struct NonPod
{
	NonPod() : id(s_i++) { Log("Constructor Of NonPod %d", id); }
	NonPod(int x) : id(x) { Log("Int constructor called With Id: %d", id); }

	NonPod(const NonPod& o) : id(o.id) 
	{ 
		Log("Copy ctor for NonPods %d", id);

//		if(id==20) 
//			throw std::runtime_error("ERR"); 
	}

	NonPod(NonPod&& o) noexcept : id(o.id) { o.id=-o.id; Log("Move ctor of NonPods %d", id); }
	~NonPod() { Log("Destructor Of NonPod %d", id); } 

	volatile int id;
	volatile static int s_i;
};

volatile int NonPod::s_i = 1;



int main()
{
	Log("Hello Utix test");
	Vector<NonPod> v;

//	v.initialize ( { 10, 20 ,30 ,40 , 50 } );
	NonPod x[] = { 10 , 20 ,30 ,40 , 50 };
	v.initialize(std::move(x));


	for(auto &x : v )
		Log("%d", x.id);






	return 0;
}
















