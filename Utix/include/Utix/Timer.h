/*

UTIX - utility library from XChip
Copyright (C) 2016  Rafael Moura

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/

#ifndef UTIX_TIMER_H_
#define UTIX_TIMER_H_
#include <chrono>

#if defined(__linux__) || defined(__APPLE__)

#include <ctime>

#elif _WIN32 

#include <Windows.h>

#endif


#include "BaseTraits.h"
 
namespace utix {

using Milli = std::chrono::milliseconds;
using Micro = std::chrono::microseconds;
using Nano = std::chrono::nanoseconds;
using Duration = std::chrono::duration<long long, std::nano>;

namespace literals {
	constexpr Micro operator""_sec(unsigned long long x) { return std::chrono::seconds(x); }
	constexpr Micro operator""_milli(unsigned long long x) { return std::chrono::milliseconds(x); }
	constexpr Micro operator""_micro(unsigned long long x) { return std::chrono::microseconds(x); }
	constexpr Micro operator""_hz(unsigned long long x) { return 1_sec / x; }
	constexpr Nano operator""_nano(unsigned long long x) { return std::chrono::nanoseconds(x); }
}



class Timer
{
public:
	Timer() noexcept = default;
	Timer(const Micro& target) noexcept;

	const Micro& GetTargetTime() const;
	int GetTargetHz() const;
	Duration GetRemain() const;
	bool Finished() const;
	void SetTargetTime(const Micro& target);
	void Start();

	template<class T>
	enable_if_t<is_numeric<T>::value> SetTargetHz(const T);

private:
	std::chrono::steady_clock::time_point m_startPoint = std::chrono::steady_clock::now();
	Micro m_target;
};










inline Timer::Timer(const Micro& target) noexcept : 
	m_target(target) 
{

}


inline const Micro& Timer::GetTargetTime() const { return m_target; }

inline int Timer::GetTargetHz() const { return static_cast<int>(literals::operator""_sec(1) / m_target); }


inline Duration Timer::GetRemain() const
{
	//using namespace std::chrono;
	using namespace std::chrono;
	const auto passedTime = duration_cast<Duration>(steady_clock::now() - m_startPoint);
	return passedTime < m_target ? (m_target - passedTime) : Duration(0);
}



inline bool Timer::Finished() const
{
	return ((std::chrono::steady_clock::now() - m_startPoint) > m_target);
}



inline void Timer::Start() { m_startPoint = std::chrono::steady_clock::now(); }


inline void Timer::SetTargetTime(const Micro& target) { m_target = target; }


template<class T>
inline enable_if_t<is_numeric<T>::value> Timer::SetTargetHz(const T val) { this->SetTargetTime(literals::operator""_hz(val)); }








}


#endif // UTIX_TIMER_H_
