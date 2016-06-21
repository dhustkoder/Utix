#include <iostream>
#include <stdexcept>
#include <vector>
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

		if(id==20) 
			throw std::runtime_error("ERR"); 
	}

	//NonPod(NonPod&& o) : id(o.id) { o.id=-o.id; Log("Move ctor of NonPod"); }
	~NonPod() { Log("Destructor Of NonPod %d", id); } 

	int id;
	static int s_i;
};

struct NonCtor
{
	NonCtor() = delete;
	NonCtor(int) {}
	~NonCtor() {}

};


int NonPod::s_i = 1;



int main()
{

	Log("Hello Utix test");

/*
	int *x;
	x = new int;
	delete x;
	x = new int[1];
	delete[] x;
	x = new(std::nothrow) int[1];
	delete[] x;
	x = (int*) ::operator new(sizeof(int)*1, std::nothrow);
	::operator delete(x);
*/


	//PodVector<int> podV;

//	NonPod x[2];

//	NonPod o1;
//	NonPod o2;


	Vector<NonPod> nonPodV;
	std::initializer_list<NonPod> x = {10,20,30,40,50,60,70,80,90};
	try {
		nonPodV.initialize(std::move(x));
	


	} catch(std::runtime_error& err)
	{
		std::cout <<"EXCEPTION: " << err.what() << '\n';
	}

	for(auto& x : nonPodV)
		std::cout << x.id << '\n';



	/*
	try 
	{

		nonPodV.emplace_back(10);
		nonPodV.emplace_back(20);
		nonPodV.emplace_back(30);
		nonPodV.emplace_back(40);
		nonPodV.emplace_back(50);
		nonPodV.emplace_back(60);
		nonPodV.emplace_back(70);
		nonPodV.emplace_back(80);

	}
	catch(std::runtime_error& err)
	{
		std::cout << "EXCEPTION: " << err.what() << '\n';
	}

	return 0;
	*/
	return 0;
}
















