


#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <benchmark/benchmark.h>
#include <Utix/Log.h>
#include <Utix/Vector.h>



#define MAX_LOOP 5000 * 100



using namespace utix;


struct NonPod
{
	NonPod() : id(s_i++) {}//{ Log("Constructor Of NonPod %d", id); }
	NonPod(int x) : id(x) {}//{ Log("Int constructor called With Id: %d", id); }

	NonPod(const NonPod& o) : id(o.id) {}//{ Log("Copy ctor"); }

	NonPod(NonPod&& o) noexcept : id(o.id), buffer(o.buffer) { 
		o.buffer = nullptr;
		o.id=-o.id;
		// Log("Move ctor of NonPods %d", id);
	}

	~NonPod() { 
		//Log("Destructor Of NonPod %d", id);
		std::free(buffer);
	} 


	NonPod& operator=(NonPod&& o) {
		this->id = o.id;
		this->buffer = o.buffer;
		o.id = -o.id;
		o.buffer = nullptr;
		return *this;
	}

	NonPod& operator=(const NonPod& o) {
		id = o.id;
		return *this;
	}



	volatile int id;
	volatile static int s_i;
	int * buffer = nullptr;
};

volatile int NonPod::s_i = 1;


static void utix_vector(benchmark::State& state) {

	while(state.KeepRunning()) 
	{
		Vector<NonPod> v;
		v.initialize(MAX_LOOP);
		v.reserve(MAX_LOOP);
	
		for(size_t i = 0; i < MAX_LOOP; ++i)
			v.push_back(NonPod(i));
	}

}



static void std_vector(benchmark::State& state) 
{

	while(state.KeepRunning()) 
	{
		std::vector<NonPod> v;
		v.reserve(MAX_LOOP);

		for(size_t i = 0; i < MAX_LOOP; ++i)
			v.push_back(NonPod(i));
	}

}

/*
static void UVector_POD(benchmark::State &state) {

	while(state.KeepRunning()) {
		Vector<int> v;
		v.initialize(MAX_LOOP);
		v.resize(MAX_LOOP);
		for(volatile size_t i = 0; i < MAX_LOOP; ++i)
			v[i] = i;
	}

}



static void StdVector_POD(benchmark::State &state) {

	while(state.KeepRunning()) {
		std::vector<int> v;
		v.reserve(MAX_LOOP);
		for(size_t i = 0; i < MAX_LOOP; ++i)
			v.emplace_back(i);
	}

}
*/

BENCHMARK(utix_vector);
BENCHMARK(std_vector);
BENCHMARK(utix_vector);
BENCHMARK(std_vector);


BENCHMARK_MAIN()





