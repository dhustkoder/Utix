/*

Utix - utility library from XChip
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

#include <Utix/DLoader.h>
#include <Utix/Assert.h>
#include <Utix/Log.h>

namespace utix {


DLoader::DLoader(DLoader&& rhs) noexcept
	: _handle(rhs._handle)
{
	rhs._handle = nullptr;
}

DLoader& DLoader::operator=(DLoader&& rhs) noexcept
{
	this->Swap(rhs);
	return *this;
}

DLoader::~DLoader()
{
	this->Free();
}




bool DLoader::Load(const std::string& dlPath)
{

#if defined(__linux__)
	constexpr const char* const postfix = ".so";
#elif defined(__APPLE__)
	constexpr const char* const postfix = ".dylib";
#elif defined(_WIN32)
	constexpr const char* const postfix = ".dll";
#endif


#if defined(__linux__) || defined(__APPLE__)

	auto newHandle = dlopen(dlPath.c_str(), RTLD_LAZY);
	
	if (!newHandle)
	{
		const std::string dlPathFix = dlPath + postfix;
		newHandle = dlopen(dlPathFix.c_str(), RTLD_LAZY);

		if (!newHandle)
		{
			const char* error = dlerror();
			LogError("Could not load shared library: %s or %s\nError: %s", 
                                  dlPath.c_str(), dlPathFix.c_str(), error);

			return false;
		}
	}


#elif defined(_WIN32)

	auto newHandle = LoadLibrary(dlPath.c_str());

	if(!newHandle)
	{
		const std::string dlPathFix = dlPath + postfix;
		newHandle = LoadLibrary(dlPath.c_str());
		if (!newHandle)
		{
			const int errorCode = GetLastError();
			LogError("Could not load %s, or %s\nErrorCode %d", 
                      dlPath.c_str(), dlPathFix.c_str(), errorCode);
			return false;
		}
	}


#endif

	this->Free();
	_handle = newHandle;
	return true;
}





void DLoader::Free() noexcept
{
	if(_handle)
	{

#if defined(__linux__) || defined(__APPLE__)
		dlclose(_handle);
#elif defined(_WIN32)
		FreeLibrary(_handle);	
#endif
	

		_handle = nullptr;
	}
}




void* DLoader::GetSymbol(const std::string& symbolName)
{
	using namespace literals; 
	ASSERT_MSG(_handle != nullptr, "Attempt to Get symbol from null shared library");


#if defined(__linux__) || defined(__APPLE__)

	dlerror(); // clean
	void* symbolAddr = reinterpret_cast<void*>( dlsym(_handle, symbolName.c_str() ) );
	const char* error = dlerror();
	if(error)
	{
		LogError("Failed to get symbol addr: %s", error);
		return nullptr;
	}
	
#elif defined(_WIN32)
	SetLastError(0); // clean
	void* symbolAddr = GetProcAddress(_handle, symbolName.c_str());
	const auto errorCode = GetLastError();
	if (!symbolAddr && errorCode)
	{
		LogError("Failed to get symbol addr: Error Code: %d", errorCode);
		return nullptr;
	}

#endif	


	return symbolAddr;
}





void DLoader::Swap(DLoader& other) noexcept
{
	if(&other != this)
	{
		auto* const aux = this->_handle;
		this->_handle = other._handle;
		other._handle = aux;
	}
}












}



