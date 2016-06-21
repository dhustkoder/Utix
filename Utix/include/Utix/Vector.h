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




#ifndef _UTIX_VECTOR_H_
#define _UTIX_VECTOR_H_

#include <cstdlib>
#include <cstring>
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
	const TYPE& operator[](size_t offset) const;


	bool initialize(const size_t VectorSize = 1);
	bool initialize(const TYPE* data, const size_t size);
	bool initialize(const Vector& other);
	void initialize(Vector&& other) noexcept;
	bool initialize(const TYPE* begin, const TYPE* end); 
	bool initialize(std::initializer_list<TYPE> list);
	template<size_t sz>
	bool initialize(TYPE(&data)[sz]);
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
	TYPE& operator[](size_t offset);

	bool copy(const Vector& other);
	void swap(Vector& other) noexcept;
	void free() noexcept;



private:
	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == true, 
	bool> _reserve(const size_t size);

	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == false, 
	bool> _reserve(const size_t size);


	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == true> 
	_fill(TYPE* dest, const TYPE* src, const size_t size);


	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == false> 
	_fill(TYPE* dest, const TYPE* src, const size_t size);


	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == true> 
	_fill_move(TYPE* dest, TYPE* src, const size_t size);


	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == false> 
	_fill_move(TYPE* dest, TYPE* src, const size_t size);


	template<class U = TYPE, class T>
	enable_if_t<std::is_pod<U>::value == true,
	bool> _push_back(T&& t);

	template<class U = TYPE, class T>
	enable_if_t<std::is_pod<U>::value == false,
	bool> _push_back(T&& t);
	
	template<class U = TYPE, class ...Args>
	enable_if_t<std::is_pod<U>::value == true,
	bool> _emplace_back(Args&& ...args);

	template<class U = TYPE, class ...Args>
	enable_if_t<std::is_pod<U>::value == false,
	bool> _emplace_back(Args&& ...args);

	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == true,
	void> _free() noexcept;

	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == false,
	void> _free() noexcept;

	template<class U = TYPE>
	enable_if_t<std::is_pod<U>::value == false,
	void> _call_destructors(TYPE* begin, TYPE* end) noexcept;




private:
	TYPE* _data = nullptr;
	size_t _size = 0;
};






template<class TYPE>
Vector<TYPE>::Vector(Vector&& other) noexcept
	: _data(other._data),
	_size(other._size)
{
	other._data = nullptr;
	other._size = 0;
}


template<class TYPE>
Vector<TYPE>::~Vector()
{
	this->free();
}


template<class TYPE>
Vector<TYPE>& Vector<TYPE>::operator=(Vector&& other) noexcept
{
	this->swap(other);
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
inline const TYPE& Vector<TYPE>::operator[](size_t offset) const
{
	ASSERT_MSG( _data != nullptr && offset < arr_size(_data), 
		"offset overflow or not initialized Vector");

	return _data[offset];
}




template<class TYPE>
inline bool Vector<TYPE>::initialize(const size_t vectorSize)
{
	if (this->reserve(vectorSize > 0 ? vectorSize : 10))
		return true;

	return false;
}





template<class TYPE>
bool Vector<TYPE>::initialize(const TYPE* array, const size_t arraySize)
{
	if(arraySize)
	{
		if(!this->reserve(arraySize))
			return false;

		this->_fill(_data, array, arraySize);
		_size = arraySize;
	}

	return true;
}




template<class TYPE>
inline bool Vector<TYPE>::initialize(const Vector& other)
{
	const auto otherSize = other.size();
	
	if(otherSize)
	{
		if(!this->reserve(other.capacity()))
			return false;
		
		this->_fill(this->_data, other._data, otherSize);
		_size = otherSize;
	}

	return true;
}



template<class TYPE>
inline void Vector<TYPE>::initialize(Vector&& other) noexcept
{
	this->swap(other);
}


template<class TYPE>
inline bool Vector<TYPE>::initialize(const TYPE* begin, const TYPE* end)
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
inline bool Vector<TYPE>::initialize(TYPE(&data)[sz])
{
	return this->initialize(data, sz);
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
inline TYPE& Vector<TYPE>::operator[](size_t offset)
{
	ASSERT_MSG( _data != nullptr && offset < arr_size(_data), 
		"offset overflow or not initialized Vector");

	return _data[offset];
}





template<class TYPE>
bool Vector<TYPE>::push_back(const TYPE& type)
{
	return _push_back(type);
}

template<class TYPE>
bool Vector<TYPE>::push_back(TYPE&& type)
{
	return _push_back(move(type));
}


template<class TYPE>
template<class ...Args>
bool Vector<TYPE>::emplace_back(Args&& ...args)
{
	return _emplace_back(forward<Args>(args)...);
}



template<class TYPE>
void Vector<TYPE>::clear()
{
	_size = 0;
}



template<class TYPE>
inline bool Vector<TYPE>::reserve(const size_t size)
{
	return _reserve(size);
}

template<class TYPE>
bool Vector<TYPE>::resize(const size_t size)
{
	if(this->reserve(size))
	{
		_size = size;
		return true;
	}

	return false;
}







template<class TYPE>
bool Vector<TYPE>::copy(const Vector& other)
{
	if(this != &other)
	{
		Vector<TYPE> tmp;
		
		if( tmp.initialize(other._data, other.capacity()) )
		{
			this->swap(tmp);
			return true;
		}
		
		return false;
	}

	return true;
}








template<class TYPE>
void Vector<TYPE>::swap(Vector& other) noexcept
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
inline void Vector<TYPE>::free() noexcept
{
	_free();
}





template<class TYPE>
template<class U>
inline enable_if_t<std::is_pod<U>::value == true, 
bool> Vector<TYPE>::_reserve(const size_t size)
{
	const auto _capacity = this->capacity();
	
	TYPE* buff = nullptr;

	if(_capacity == 0)
		buff = (TYPE*) alloc_arr(sizeof(TYPE) * size);
	else if(_capacity < size)
		buff = (TYPE*) realloc_arr(_data, sizeof(TYPE) * size);


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
inline enable_if_t<std::is_pod<U>::value == false, 
bool> Vector<TYPE>::_reserve(const size_t sizeToReserve)
{
	const auto _capacity = this->capacity();
	
	TYPE* buff = nullptr;

	if(_capacity < sizeToReserve)
		buff = (TYPE*) alloc_arr(sizeof(TYPE) * sizeToReserve);


	if(buff)
	{
		if(_data) 
		{
			size_t i = 0;

			try {
				
				const auto elementsToTransfer = this->size();
				for(; i < elementsToTransfer; ++i)
					new(buff + i) TYPE(std::move_if_noexcept(_data[i]));
			}
			catch(...) {
				// if exception is thrown, keeps the old _data
				_call_destructors(buff, buff+i);
				free_arr(buff);
				throw;
			}

			// if success then erase the _data
			_call_destructors(_data, _data + _size);
			free_arr(_data);
		}

		_data = buff;
		return true;
	}

	LogError("Failed to reserve memory for Vector");
	return false;
}






template<class TYPE>
template<class U>
inline enable_if_t<std::is_pod<U>::value == true> 
Vector<TYPE>::_fill(TYPE* dest, const TYPE* src, const size_t size)
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
template<class U>
inline enable_if_t<std::is_pod<U>::value == false> 
Vector<TYPE>::_fill(TYPE* dest, const TYPE* src, const size_t size)
{
	const auto end = dest + size;
	while(dest != end) 
	{
		new(dest) TYPE((*src));
		++dest;
		++src;
	}
}



template<class TYPE>
template<class U>
inline enable_if_t<std::is_pod<U>::value == true> 
Vector<TYPE>::_fill_move(TYPE* dest, TYPE* src, const size_t size)
{
	// pod types don't care about moving
	this->_fill(dest, src, size);
}


template<class TYPE>
template<class U>
inline enable_if_t<std::is_pod<U>::value == false> 
Vector<TYPE>::_fill_move(TYPE* dest, TYPE* src, const size_t size)
{
	const auto end = dest + size;
	while(dest != end) 
	{
		new(dest) TYPE(move(*src));
		++dest;
		++src;
	}
}





template<class TYPE>
template<class U, class T>
inline enable_if_t<std::is_pod<U>::value == true,
bool> Vector<TYPE>::_push_back(T&& t)
{
	if( _size >= this->capacity() )
		if(!this->reserve(_size))
			return false;

	_data[_size++] = t;
	return false;
}





template<class TYPE>
template<class U, class T>
inline enable_if_t<std::is_pod<U>::value == false,
bool> Vector<TYPE>::_push_back(T&& t)
{
	if( _size >= this->capacity() )
		if(!this->reserve(_size))
			return false;

	new(_data + _size) TYPE(forward<U>(t));
	++_size;

	return false;

}



template<class TYPE>
template<class U, class ...Args>
inline enable_if_t<std::is_pod<U>::value == true,
bool> Vector<TYPE>::_emplace_back(Args&& ...args)
{
	if( _size >= this->capacity() )
		if(!this->reserve(_size*2))
			return false;

	_data[_size++] = { forward<Args>(args)...};
	return true;
}




template<class TYPE>
template<class U, class ...Args>
inline enable_if_t<std::is_pod<U>::value == false,
bool> Vector<TYPE>::_emplace_back(Args&& ...args)
{
	if( _size >= this->capacity() )
		if(!this->reserve(_size*2))
			return false;

	new (_data + _size) TYPE(forward<Args>(args)...);
	++_size;
	return false;
}








template<class TYPE>
template<class U>
inline enable_if_t<std::is_pod<U>::value == true,
void> Vector<TYPE>::_free() noexcept
{
	if(_data)
	{
		free_arr(_data);
		_data = nullptr;
		_size = 0;
	}
}

template<class TYPE>
template<class U>
inline enable_if_t<std::is_pod<U>::value == false,
void> Vector<TYPE>::_free() noexcept
{
	if(_data)
	{
		_call_destructors(_data, _data + this->size());
		free_arr(_data);
		_data = nullptr;
		_size = 0;
	}
}




template<class TYPE>
template<class U>
inline enable_if_t<std::is_pod<U>::value == false,
void> Vector<TYPE>::_call_destructors(TYPE* begin, TYPE* end) noexcept
{
	while(end != begin) 
	{
		--end;
		end->~TYPE();
	}
}







}


#endif
