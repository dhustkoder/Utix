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






#include <Utix/Ints.h>
#include <Utix/Log.h>
#include <Utix/CliOpts.h>


namespace utix {



CliOpts::CliOpts(int argc, char** argv)
	: ArgVec(argv, argv + argc)
{

}


CliOpts::CliOpts(std::vector<std::string>&& cliArgs)
	: ArgVec(std::move(cliArgs))
{

}


std::string CliOpts::GetOpt(const std::string& match) const
{

	const auto itr = GetOptItr(match);
	if (itr != this->end())
	{
		if (_isSub)
			return itr->substr(match.size(), itr->size());
		else
			return *itr;
	}

	return std::string();

}







CliOpts::ArgVec::const_iterator CliOpts::GetOptItr(const std::string& match) const
{
	const auto matchSize = match.size();
	const auto end = this->cend();

	for (auto itr = this->begin(); itr != end; ++itr)
	{
		const auto argSize = itr->size();
		if (argSize == matchSize)
		{
			if (*itr == match)
			{
				_isSub = false;
				if((itr+1) != end)
					return ++itr;
				else
					return itr;
			}

		}
		else if (argSize > matchSize)
		{
			const auto substr = itr->substr(0, matchSize);
			if (substr == match)
			{
				_isSub = true;
				return itr;
			}
		}
	}

	return end;
}






bool CliOpts::RemoveOpt(const std::string& match)
{
	auto itr = GetOptItr(match);
	const auto end = this->cend();

	if (itr != end)
	{
		if (_isSub || *itr == match)
		{
			this->erase(itr);
		}
		else
		{
			itr = this->erase(itr);
			this->erase(itr);
		}

		return true;
	}

	return false;
}

























}
