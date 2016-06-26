#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <Utix/Vector.h>
#include <Utix/CliOpts.h>



using namespace utix;




int main(int argc, char** argv)
{
	CliOpts opts(argc, argv);

	std::string x;
	if( ( x = opts.GetOpt("-AMANDA")) != "")
		std::cout << "OPT FOUND\n" << x << '\n';




}