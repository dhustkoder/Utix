#ifndef UTIX_ALLOC_T_H_
#define UTIX_ALLOC_T_H_
#include <stdlib.h>
namespace utix {

template<class T>
inline T* malloc_t(const size_t size = 1) {
	return static_cast<T*>(malloc(sizeof(T) * size));
}



template<class T>
inline T* realloc_t(T* from, const size_t size) {
	return static_cast<T*>(realloc(from, sizeof(T) * size));
}



















}
#endif
