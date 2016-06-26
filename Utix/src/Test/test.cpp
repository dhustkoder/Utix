#include <string>
#include <vector>
#include <algorithm>

#include <benchmark/benchmark.h>
#include <Utix/Vector.h>



std::vector<std::string> std_vec;
utix::Vector<std::string> utix_vec;


inline void PrepareVectors(benchmark::State& state) {
	state.PauseTiming();	
	benchmark::DoNotOptimize(std_vec);
	benchmark::DoNotOptimize(utix_vec);
	std_vec = std::vector<std::string>();
	std_vec.reserve(state.range_x());
	utix_vec.initialize(state.range_x());
	state.ResumeTiming();
}


template<class C>
inline void GenerateStrings(C& vec, const size_t size) {
	for(size_t i = 0; i < size; ++i)
		vec.emplace_back("");
}



void UTIX(benchmark::State& state) {
	while(state.KeepRunning()) {
		PrepareVectors(state);
		GenerateStrings(utix_vec, state.range_x());
	}
}


void STD(benchmark::State& state) {
	while(state.KeepRunning()) {
		PrepareVectors(state);
		GenerateStrings(std_vec, state.range_x());
	}
}


BENCHMARK(UTIX)->RangeMultiplier(2)->Range(8, 8<<10);
BENCHMARK(STD)->RangeMultiplier(2)->Range(8, 8<<10);
BENCHMARK_MAIN()
