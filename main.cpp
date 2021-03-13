#include <iostream>
#include "xml.h"

struct xmlNode
{
	///Container of the xml tree
	char *name=0, *value=0;
	xmlNode *next, *previous, *begin, *end, *parent;
};

void push(xmlNode* &begin, xmlNode* &end, xmlNode* node)
{
	node->previous=end;
	
	if(begin==0)
		begin=end=node;
	else
	{
		end->next=node;
		end=node;
	}
}

void pop(xmlNode* &begin, xmlNode* &end)
{
	if(begin==0 || end==0)
		return;

	xmlNode* node=end;
	if(begin==end)
		begin=end=0;
	else
	{
		end=end->previous;
		end->next=0;
	}
	delete node;
}

int main()
{
	char s[200];

	///Xml tag checker via stdin
	/*while(std::cin.getline(s,200))
	{
		std::cout << s << '\n';
		xmlParser(s);
		std::cout << '\n';
	}*/


	
	return 0;
}
