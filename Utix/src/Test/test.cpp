#include <cstdio>
#include <iostream>
#include <memory>
#include <Utix/BaseTraits.h>

int main()
{
	utix::remove_pointer_t<int* const volatile restrict> x = 0;
	std::cout << utix::is_pointer<decltype(x)>::value << '\n';	
}
