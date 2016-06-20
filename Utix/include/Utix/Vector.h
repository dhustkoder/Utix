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




#ifndef _UTIX_PODVECTOR_H_
#define _UTIX_PODVECTOR_H_

#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include "Traits.h"
#include "Alloc.h"
#include "Assert.h"
#include "Log.h"



namespace utix {


template<class TYPE>
class PodVector
{
	static_assert(std::is_pod<TYPE>::value, "PodVector only support pod types");
public:
	PodVector(const PodVector&) = delete;
	PodVector& operator=(const PodVector&) = delete;

	PodVector() = default;
	PodVector(PodVector&& other) noexcept;
	~PodVector();
	PodVector& operator=(PodVector&& other) noexcept;

	bool empty() const;
	size_t size() const;
	size_t capacity() const;

	const TYPE* cbegin() const;
	const TYPE* cend() const;
	const TYPE* begin() const;
	const TYPE* end() const;
	const TYPE* data() const;
	const TYPE& operator[](size_t offset) const;


	bool initialize(const size_t PodVectorSize = 1);
	bool initialize(const TYPE* data, const size_t size);
	bool initialize(const PodVector& other);
	void initialize(PodVector&& other) noexcept;
	bool initialize(const TYPE* begin, const TYPE* end); 
	bool initialize(std::initializer_list<TYPE> list);
	template<size_t sz>
	bool initialize(TYPE(&data)[sz]);
	bool push_back(const TYPE& type);
	template<class ...Args>
	bool emplace_back(Args&& ...args);


	void clear();
	bool resize(const size_t size);
	bool reserve(const size_t size);

	TYPE* begin();
	TYPE* end();
	TYPE* data();
	TYPE& operator[](size_t offset);

	bool copy(const PodVector& other);
	void swap(PodVector& other) noexcept;
	void free() noexcept;


private:
	TYPE* _data = nullptr;
	size_t _size = 0;
};






template<class TYPE>
PodVector<TYPE>::PodVector(PodVector&& other) noexcept
	: _data(other._data),
	_size(other._size)
{
	other._data = nullptr;
	other._size = 0;
}


template<class TYPE>
PodVector<TYPE>::~PodVector()
{
	this->free();
}


template<class TYPE>
PodVector<TYPE>& PodVector<TYPE>::operator=(PodVector&& other) noexcept
{
	this->swap(other);
}




template<class TYPE>
inline bool PodVector<TYPE>::empty() const
{
	return _size == 0;
}


template<class TYPE>
inline size_t PodVector<TYPE>::size() const
{
	return _size;
}


template<class TYPE>
inline size_t PodVector<TYPE>::capacity() const
{
	return arr_size(_data);
}



template<class TYPE>
inline const TYPE* PodVector<TYPE>::cbegin() const
{
	return _data;
}


template<class TYPE>
inline const TYPE* PodVector<TYPE>::cend() const
{
	return _data + this->size();
}




template<class TYPE>
inline const TYPE* PodVector<TYPE>::begin() const
{
	return _data;
}

template<class TYPE>
inline const TYPE* PodVector<TYPE>::end() const
{
	return _data + this->size();
}

template<class TYPE>
inline const TYPE* PodVector<TYPE>::data() const
{
	return _data;
}


template<class TYPE>
inline const TYPE& PodVector<TYPE>::operator[](size_t offset) const
{
	ASSERT_MSG( _data != nullptr && offset < arr_size(_data), 
		"offset overflow or not initialized PodVector");

	return _data[offset];
}







template<class TYPE>
inline bool PodVector<TYPE>::initialize(const size_t PodVectorSize)
{
	if (this->resize(PodVectorSize > 1 ? PodVectorSize : 1))
		return true;

	return false;
}





template<class TYPE>
bool PodVector<TYPE>::initialize(const TYPE* data, const size_t size)
{
	if(this->resize(size ? size : 1))
	{
		if(size) 
			std::memcpy(this->_data, data, sizeof(TYPE) * size);
		
		return true;
	}

	return false;
}




template<class TYPE>
inline bool PodVector<TYPE>::initialize(const PodVector& other)
{
	return this->initialize(other._data, other.capacity());
}



template<class TYPE>
inline void PodVector<TYPE>::initialize(PodVector&& other) noexcept
{
	this->swap(other);
}


template<class TYPE>
inline bool PodVector<TYPE>::initialize(const TYPE* begin, const TYPE* end)
{
	return this->initialize(begin, static_cast<size_t>(end - begin));
}



template<class TYPE>
inline bool PodVector<TYPE>::initialize(std::initializer_list<TYPE> list)
{
	return this->initialize(list.begin(), list.size());
}



template<class TYPE>
template<size_t sz>
inline bool PodVector<TYPE>::initialize(TYPE(&data)[sz])
{
	return this->initialize(data, sz);
}





template<class TYPE>
inline TYPE* PodVector<TYPE>::begin()
{
	return _data;
}




template<class TYPE>
inline TYPE* PodVector<TYPE>::end()
{
	return _data + this->size();
}



template<class TYPE>
inline TYPE* PodVector<TYPE>::data()
{
	return _data;
}



template<class TYPE>
inline TYPE& PodVector<TYPE>::operator[](size_t offset)
{
	ASSERT_MSG( _data != nullptr && offset < arr_size(_data), 
		"offset overflow or not initialized PodVector");

	return _data[offset];
}





template<class TYPE>
bool PodVector<TYPE>::push_back(const TYPE& type)
{
	if( this->capacity() > _size )
	{	
		_data[_size++] = type;
		return true;
	}

	else if(this->reserve(_size + 5)) 
	{
		_data[_size++] = type;
		return true;
	}

	return false;
}





template<class TYPE>
template<class ...Args>
bool PodVector<TYPE>::emplace_back(Args&& ...args)
{
	if( this->capacity() > _size )
	{
		_data[_size++] = { forward<Args>(args)... };
		return true;
	}

	else if(this->reserve(_size + 5)) 
	{	
		_data[_size++] = { forward<Args>(args)...};
		return true;
	}

	return false;
}






template<class TYPE>
void PodVector<TYPE>::clear()
{
	_size = 0;
}






template<class TYPE>
bool PodVector<TYPE>::reserve(const size_t size)
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

	LogError("Failed to reserve memory for PodVector");
	return false;	
}






template<class TYPE>
bool PodVector<TYPE>::resize(const size_t size)
{
	if(this->reserve(size))
	{
		_size = size;
		return true;
	}

	return false;
}







template<class TYPE>
bool PodVector<TYPE>::copy(const PodVector& other)
{
	if(this != &other)
	{
		PodVector<TYPE> tmp;
		
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
void PodVector<TYPE>::swap(PodVector& other) noexcept
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
inline void PodVector<TYPE>::free() noexcept
{
	if(_data)
	{
		free_arr(_data);
		_data = nullptr;
		_size = 0;
	}
}



















}


#endif
