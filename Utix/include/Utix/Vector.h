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




#ifndef UTIX_VECTOR_H_
#define UTIX_VECTOR_H_

#include <initializer_list>
#include "Traits.h"
#include "Alloc.h"
#include "Assert.h"
#include "Log.h"
#include "ScopeExit.h"


namespace utix {


template<class TYPE>
class Vector
{
public:
	Vector(const Vector&) = delete;
	Vector& operator=(const Vector&) = delete;

	Vector() = default;
	Vector(Vector&& other) noexcept;
	~Vector();
	Vector& operator=(Vector&& other) noexcept;

	bool empty() const;
	size_t size() const;
	size_t capacity() const;

	const TYPE* cbegin() const;
	const TYPE* cend() const;
	const TYPE* begin() const;
	const TYPE* end() const;
	const TYPE* data() const;
	const TYPE& operator[](const size_t offset) const;


	bool initialize(const size_t VectorSize = 0);
	
	template<class T>
	bool initialize(const T* array, const size_t size);

	bool initialize(const Vector& other);
	void initialize(Vector&& other);
	bool initialize(std::initializer_list<TYPE> list);

	template<class T1, class T2>
	bool initialize(const T1* begin, const T2* end);
	template<size_t sz>
	bool initialize(const TYPE(&array)[sz]);
	template<size_t sz>
	bool initialize(TYPE(&&array)[sz]);


	bool push_back(const TYPE& type);
	bool push_back(TYPE&& type);
	template<class ...Args>
	bool emplace_back(Args&& ...args);
	void clear();
	bool resize(const size_t size);
	bool reserve(const size_t size);

	TYPE* begin();
	TYPE* end();
	TYPE* data();
	TYPE& operator[](const size_t offset);

	void swap(Vector& other) noexcept;
	void free() noexcept;

private:
	bool reserve_init(size_t requested_size);
	bool check_capacity();

	// pod functions
	template<class U = TYPE, class ...Args>
	enable_if_t<std::is_pod<U>::value == true,
	bool> _insert_back(Args&& ...args);

	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == true> 
	_clear();

	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == true, 
	bool> _reserve(size_t requested_size = 0);

	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == true, 
	bool> _resize(const size_t requested_size);


	template<class T, class U = TYPE>
	enable_if_t<std::is_pod<U>::value == true>
	_fill(const T* src, const size_t size, TYPE* dest);


	template<class T, class U = TYPE>
	enable_if_t<std::is_pod<U>::value == true> 
	_fill_move(T* src, const size_t size, TYPE* dest);
	

	// Non-Pod functions

	template<class U = TYPE, class ...Args>
	enable_if_t<std::is_pod<U>::value == false,
	bool>_insert_back(Args&& ...args);

	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == false> 
	_clear();

	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == false, 
	bool> _reserve(size_t requested_size = 0);

	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == false, 
	bool> _resize(const size_t requested_size);

	template<class T, class U = TYPE>
	enable_if_t<std::is_pod<U>::value == false> 
	_fill(const T* src, const size_t size, TYPE* dest);


	template<class T, class U = TYPE>
	enable_if_t<std::is_pod<U>::value == false> 
	_fill_move(T* src, const size_t size, TYPE* dest);

	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == false,
	void> _call_destructors(TYPE* const begin, TYPE* end) noexcept;




private:
	TYPE* _data = nullptr;
	size_t _size = 0;
};






template<class TYPE>
inline Vector<TYPE>::Vector(Vector&& other) noexcept
	: _data(other._data),
	_size(other._size)
{
	other._data = nullptr;
	other._size = 0;
}


template<class TYPE>
inline Vector<TYPE>::~Vector()
{
	this->free();
}


template<class TYPE>
inline Vector<TYPE>& Vector<TYPE>::operator=(Vector&& other) noexcept
{
	this->swap(other);
	return *this;
}




template<class TYPE>
inline bool Vector<TYPE>::empty() const
{
	return _size == 0;
}


template<class TYPE>
inline size_t Vector<TYPE>::size() const
{
	return _size;
}


template<class TYPE>
inline size_t Vector<TYPE>::capacity() const
{
	return arr_size(_data);
}



template<class TYPE>
inline const TYPE* Vector<TYPE>::cbegin() const
{
	return _data;
}


template<class TYPE>
inline const TYPE* Vector<TYPE>::cend() const
{
	return _data + this->size();
}




template<class TYPE>
inline const TYPE* Vector<TYPE>::begin() const
{
	return _data;
}



template<class TYPE>
inline const TYPE* Vector<TYPE>::end() const
{
	return _data + this->size();
}



template<class TYPE>
inline const TYPE* Vector<TYPE>::data() const
{
	return _data;
}



template<class TYPE>
inline const TYPE& Vector<TYPE>::operator[](const size_t offset) const
{
	ASSERT_MSG( _data != nullptr && offset < arr_size(_data), 
		"offset overflow or not initialized Vector");

	return _data[offset];
}




template<class TYPE>
inline bool Vector<TYPE>::initialize(const size_t vectorSize)
{
	this->free();
	return reserve_init(vectorSize);
}





template<class TYPE>
template<class T>
bool Vector<TYPE>::initialize(const T* array, const size_t arraySize)
{
	if(this->initialize(arraySize))
	{
		this->_fill(array, arraySize, this->_data);
		_size = arraySize;
		return true;
	}

	return false;
}




template<class TYPE>
bool Vector<TYPE>::initialize(const Vector& other)
{
	const auto otherCap = other.capacity();

	if(this->initialize(otherCap))
	{	
		this->_fill(other._data, other._size, this->_data);
		_size = other._size;
		return true;
	}

	return false;
}



template<class TYPE>
inline void Vector<TYPE>::initialize(Vector&& other)
{
	this->swap(other);
}



template<class TYPE>
template<class T1, class T2>
inline bool Vector<TYPE>::initialize(const T1* begin, const T2* end)
{
	return this->initialize(begin, static_cast<size_t>(end - begin));
}



template<class TYPE>
inline bool Vector<TYPE>::initialize(std::initializer_list<TYPE> list)
{
	return this->initialize(list.begin(), list.size());
}



template<class TYPE>
template<size_t sz>
inline bool Vector<TYPE>::initialize(const TYPE(&array)[sz])
{
	return this->initialize(array, sz);
}


template<class TYPE>
template<size_t sz>
inline bool Vector<TYPE>::initialize(TYPE(&&array)[sz])
{
	if( this->initialize(sz) )
	{
		this->_fill_move(array, sz, _data);
		this->_size = sz;
		return true;
	}

	return false;
}





template<class TYPE>
inline TYPE* Vector<TYPE>::begin()
{
	return _data;
}




template<class TYPE>
inline TYPE* Vector<TYPE>::end()
{
	return _data + this->size();
}



template<class TYPE>
inline TYPE* Vector<TYPE>::data()
{
	return _data;
}



template<class TYPE>
inline TYPE& Vector<TYPE>::operator[]( const size_t offset )
{
	ASSERT_MSG( _data != nullptr && offset < arr_size(_data), 
		"offset overflow or not initialized Vector");

	return _data[offset];
}





template<class TYPE>
inline bool Vector<TYPE>::push_back(const TYPE& type)
{
	return this->_insert_back(type);
}



template<class TYPE>
inline bool Vector<TYPE>::push_back(TYPE&& type)
{
	return this->_insert_back(std::move(type));
}




template<class TYPE>
template<class ...Args>
inline bool Vector<TYPE>::emplace_back(Args&& ...args)
{
	return this->_insert_back(std::forward<Args>(args)...);
}




template<class TYPE>
inline void Vector<TYPE>::clear()
{
	this->_clear();
}




template<class TYPE>
inline bool Vector<TYPE>::reserve(const size_t size)
{
	return _reserve(size);
}



template<class TYPE>
inline bool Vector<TYPE>::resize(const size_t requested_size)
{
	return _resize(requested_size);
}






template<class TYPE>
inline void Vector<TYPE>::swap(Vector& other) noexcept
{
	if( this != &other )
	{
		const auto dataAux = this->_data;
		const auto sizeAux = this->_size;
		this->_data = other._data;
		this->_size = other._size;
		other._data = dataAux;
		other._size = sizeAux;
	}
}






template<class TYPE>
inline bool Vector<TYPE>::reserve_init(size_t requested_size)
{
	if( _data == nullptr )
	{
		if( requested_size == 0 )
			requested_size = 10;

		const size_t bytes_to_allocate = sizeof(TYPE) * requested_size;

		if( (bytes_to_allocate / sizeof(TYPE)) < requested_size) {
			LogError("Can't allocate %zu elements of TYPE with size %zu", requested_size, sizeof(TYPE));
			return false;
		}


		_data = (TYPE*) alloc_arr(bytes_to_allocate);
		return _data != nullptr;
	}

	return true;
}

template<class TYPE>
inline bool Vector<TYPE>::check_capacity()
{
	if( this->capacity() > _size )
		return true;

	return this->_reserve( _size * 2 );
}



template<class TYPE>
inline void Vector<TYPE>::free() noexcept
{
	if(_data)
	{
		this->clear();
		free_arr(_data);
		_data = nullptr;
	}
}




// Pod functions

template<class TYPE>
template<class U, class ...Args>
inline enable_if_t<std::is_pod<U>::value == true,
bool> Vector<TYPE>::_insert_back(Args&& ...args)
{
	if( check_capacity() ) {
		_data[_size++] = TYPE(std::forward<Args>(args)...);
		return true;
	}

	return false;
}



template<class TYPE>
template<class U>
inline enable_if_t<std::is_pod<U>::value == true> 
Vector<TYPE>::_clear()
{
	_size = 0;
}


template<class TYPE>
template<class U>
enable_if_t<std::is_pod<U>::value == true, 
bool> Vector<TYPE>::_reserve(size_t requested_size)
{
	ASSERT_MSG(_data != nullptr, "_reserve called before reserve_init");

	const size_t bytes_to_allocate = sizeof(TYPE) * requested_size;

	if( (bytes_to_allocate / sizeof(TYPE)) < requested_size) {
		LogError("Can't allocate %zu elements of TYPE with size %zu", requested_size, sizeof(TYPE));
		return false;
	}

	TYPE* const buff = (TYPE*) realloc_arr(_data, bytes_to_allocate);

	if(buff)
	{
		_data = buff;
		return true;
	}
	
	LogError("Failed to reserve memory for Vector");
	return false;
}


template<class TYPE>
template<class U>
inline enable_if_t<std::is_pod<U>::value == true, 
bool> Vector<TYPE>::_resize(const size_t requested_size)
{
	if(_size < requested_size)
	{
		if( requested_size > this->capacity() )
			if(!this->reserve(requested_size))
				return false;
	}
	
	_size = requested_size;

	return true;
}





template<class TYPE>
template<class T, class U>
inline enable_if_t<std::is_pod<U>::value == true> 
Vector<TYPE>::_fill(const T* src, const size_t size, TYPE* dest)
{
	const auto end = dest + size;
	while(dest != end) 
	{
		*dest = *src;
		++dest;
		++src;
	}
}



template<class TYPE>
template<class T, class U>
inline enable_if_t<std::is_pod<U>::value == true> 
Vector<TYPE>::_fill_move(T* src, const size_t size, TYPE* dest)
{
	// pod types don't care about moving
	this->_fill(src, size, dest);
}







// Non-Pod functions


template<class TYPE>
template<class U, class ...Args>
inline enable_if_t<std::is_pod<U>::value == false,
bool> Vector<TYPE>::_insert_back(Args&& ...args)
{
	if( check_capacity() ) {
		new(_data + _size) TYPE(std::forward<Args>(args)...);
		++_size;
		return true;
	}

	return false;
}



template<class TYPE>
template<class U>
inline enable_if_t<std::is_pod<U>::value == false> 
Vector<TYPE>::_clear()
{
	_call_destructors(this->begin(), this->end());
	_size = 0;
}



template<class TYPE>
template<class U>
enable_if_t<std::is_pod<U>::value == false, 
bool> Vector<TYPE>::_reserve(size_t requested_size)
{
	ASSERT_MSG(_data != nullptr, "_reserve called before reserve_init");

	const size_t bytes_to_allocate = sizeof(TYPE) * requested_size;

	if( (bytes_to_allocate / sizeof(TYPE)) < requested_size)
	{
		LogError("Can't allocate %zu elements of TYPE with size %zu", requested_size, sizeof(TYPE));
		return false;
	}

	TYPE* const buff = (TYPE*) alloc_arr( bytes_to_allocate );

	if(!buff) 
	{
		LogError("Failed to reserve memory for Vector");
		return false;
	}

	// need copy _data to new memory block pointed by buff
	UTIX_TRY_ (
		this->_fill_move(_data, _size, buff);
	)
	UTIX_CATCH_(...,
		// if exception is thrown, keeps the old _data
		// and free buff
		free_arr(buff);
		throw;
	)

	// if success then erase the old _data
	_call_destructors(this->begin(), this->end());
	free_arr(_data);

	_data = buff;
	return true;	
}




template<class TYPE>
template<class U>
enable_if_t<std::is_pod<U>::value == false, 
bool> Vector<TYPE>::_resize(const size_t requested_size)
{
	if( _size < requested_size )
	{
		if( requested_size > this->capacity() )
			if(!this->reserve(requested_size))
				return false;

		// fill the new space
		TYPE* itr = _data + _size;
		const TYPE* end = _data + requested_size;

		UTIX_TRY_(
			for( ; itr != end; ++itr)
				new(itr) TYPE();
		)

		UTIX_CATCH_(...,
			// if exceptions is thrown
			// preserve the ones who construct successful
			_size = reinterpret_cast<size_t>(itr - _data);
			throw;
		)
	}
	else if ( _size > requested_size ) 
	{
		this->_call_destructors(this->begin() + requested_size, this->end());
	}


	_size = requested_size;
	return true;
}




template<class TYPE>
template<class T, class U>
inline enable_if_t<std::is_pod<U>::value == false> 
Vector<TYPE>::_fill(const T* src, const size_t size, TYPE* dest)
{
	auto destItr = dest;
	const auto destEnd = dest + size;

	UTIX_TRY_(
		while(destItr != destEnd) 
		{
			new(destItr) TYPE((*src));
			++destItr;
			++src;
		}
	)
	UTIX_CATCH_(...,
		// exception was thrown, destroy the data
		// that has been copied.
		_call_destructors(dest, destItr);
		throw;
	)
}











template<class TYPE>
template<class T, class U>
inline enable_if_t<std::is_pod<U>::value == false> 
Vector<TYPE>::_fill_move(T* src, const size_t size, TYPE* dest)
{
	auto destItr = dest;
	const auto destEnd = dest + size;
	UTIX_TRY_(
		while(destItr != destEnd) 
		{
			new(destItr) TYPE(std::move_if_noexcept(*src));
			++destItr;
			++src;
		}
	)
	UTIX_CATCH_(...,
		// exception was thrown, destroy the data
		// that has been copied.
		_call_destructors(dest, destItr);
		throw;
	)
}










template<class TYPE>
template<class U>
inline enable_if_t<std::is_pod<U>::value == false,
void> Vector<TYPE>::_call_destructors(TYPE* const begin, TYPE* end) noexcept
{
	while(end != begin) 
	{
		--end;
		end->~TYPE();
	}
}







}


#endif // UTIX_VECTOR_H_
