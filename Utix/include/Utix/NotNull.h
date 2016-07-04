#ifndef UTIX_NOTNULL_H_
#define UTIX_NOTNULL_H_
#include "Ints.h"
#include "Assert.h"
#include "BaseTraits.h"


namespace utix {


template<class T>
class NotNull 
{
  static_assert(is_pointer<T>::value, "T must be a pointer type. Ex: NotNull<int*> not NotNull<int>");

public:
	NotNull(NotNull&&) = delete;
	NotNull(nullptr_t) = delete;
	NotNull(int) = delete;
	NotNull& operator=(NotNull&&) = delete;
	NotNull& operator=(nullptr_t) = delete;
	NotNull& operator=(int) = delete;

	NotNull(T p);
	NotNull(const NotNull&) = default;
	NotNull& operator=(const NotNull&) = default;
	operator T() const;
	decltype(*T()) operator*() const;
	T operator->() const;
	T Get() const;

private:
	T m_p;
};


template<class T>
inline NotNull<T>::NotNull(T not_null_ptr) 
	: m_p(not_null_ptr)
{
	ASSERT_MSG(not_null_ptr != nullptr, "NotNull must not hold null pointers");
}



template<class T>
inline NotNull<T>::operator T() const { return Get(); }
template<class T>
inline decltype(*T()) NotNull<T>::operator*() const { return *Get(); }
template<class T>
inline T NotNull<T>::operator->() const { return Get(); }
template<class T>
inline T NotNull<T>::Get() const { return m_p; }










}




#endif // UTIX_NOTNULL_H_
