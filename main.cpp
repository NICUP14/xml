#include <iostream>
#include "xml.h"

int main()
{
	char s[200];
	std::cin.getline(s,200);
	xmlParser(s);
	return 0;
}
