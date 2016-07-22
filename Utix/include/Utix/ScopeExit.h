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

#ifndef UTIX_SCOPE_EXIT_H_
#define UTIX_SCOPE_EXIT_H_
#include "Exceptions.h"
#include "BaseTraits.h"

namespace utix {



// calls F no metter what
template<class F>
struct ScopeExit
{
#if UTIX_HAS_EXCEPTIONS_

	constexpr ScopeExit(F&& fun) noexcept : m_fun(forward<F>(fun)) {
		static_assert(noexcept(fun()) == true, "ScopeExit functor must be noexcept!");
	}
	~ScopeExit() noexcept { m_fun(); }
	ScopeExit(ScopeExit&& rhs) noexcept = default;

#else

	constexpr ScopeExit(F&& fun) : m_fun(forward<F>(fun)) {}
	~ScopeExit() { m_fun(); }
	ScopeExit(ScopeExit&& rhs) = default;

#endif

	ScopeExit(const ScopeExit&) = delete;
	ScopeExit& operator=(const ScopeExit&) = delete;
private:
	F m_fun;
};

// calls F if not canceled
template<class F>
struct ScopeExitIf
{
#if UTIX_HAS_EXCEPTIONS_

	constexpr ScopeExitIf(F&& fun) noexcept : m_fun(forward<F>(fun)) {
		static_assert(noexcept(fun()) == true, "ScopeExitIf functor must be noexcept!");
	}
	~ScopeExitIf() noexcept { if (!m_canceled) m_fun(); }
	ScopeExitIf(ScopeExitIf&& rhs) noexcept = default;

#else 

	constexpr ScopeExitIf(F&& fun) : m_fun(forward<F>(fun)) {}
	~ScopeExitIf() { if (!m_canceled) m_fun(); }
	ScopeExitIf(ScopeExitIf&& rhs) = default;

#endif

	ScopeExitIf(const ScopeExitIf&) = delete;
	ScopeExitIf& operator=(const ScopeExitIf&) = delete;
	void Cancel() { m_canceled = true; }
private:
	F m_fun;
	bool m_canceled = false;
};


template<class Callable>
constexpr ScopeExit<Callable> MakeScopeExit(Callable&& c) {
	return ScopeExit<Callable>(forward<Callable>(c));
}

template<class Callable>
constexpr ScopeExitIf<Callable> MakeScopeExitIf(Callable&& c) {
	return ScopeExitIf<Callable>(forward<Callable>(c));
}




}









#endif // UTIX_SCOPE_EXIT_H_
