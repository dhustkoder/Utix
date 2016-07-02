#ifndef UTIX_NOTNULL_H_
#define UTIX_NOTNULL_H_
#include "Assert.h"
#include "BaseTraits.h"


namespace utix {


template<class T>
class NotNull 
{
  static_assert(is_pointer<T>::value, "T must be a pointer type. Ex: NotNull<int*> not NotNull<int>");

public:
	NotNull(const NotNull&) = delete;
	NotNull(NotNull&&) = delete;
	NotNull& operator=(const NotNull&) = delete;
	NotNull& operator=(NotNull&&) = delete;
	NotNull(T p);
	decltype(*T()) operator*();
	T operator->();
	T Get();

private:
	T _p;
};


template<class T>
inline NotNull<T>::NotNull(T p) 
	: _p(p)
{
	ASSERT_MSG(_p != nullptr, "NotNull must not hold null pointers");
}

template<class T>
inline decltype(*T()) NotNull<T>::operator*() { return *_p; }
template<class T>
inline T NotNull<T>::operator->() { return _p; }
template<class T>
inline T NotNull<T>::Get() { return _p; }










}




#endif // UTIX_NOTNULL_H_
