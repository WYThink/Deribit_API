#pragma once
#ifndef __INTPUTVALIDATOR__
#define __INTPUTVALIDATOR__

#include <iostream>
#include <algorithm>

inline void ignoreLine()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#endif // !__INTPUTVALIDATOR__
