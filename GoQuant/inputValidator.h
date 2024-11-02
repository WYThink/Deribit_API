#pragma once
#ifndef __INTPUTVALIDATOR__
#define __INTPUTVALIDATOR__

#include <iostream>

inline void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#endif // !__INTPUTVALIDATOR__
