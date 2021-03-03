#include <iostream>

unsigned long strLen(char* s)
{
	///Returns the number of characters before null
	char* i=s;
	while(*i)
		i++;
	return i-s;
}

char* strChr(char* s, char* s2, char c, bool m)
{
	///Searches address containing c between memory ranges s-s2
	if(s==0 || s2==0)
		return 0;
	int j=s<s2?1:-1;
	while(j>0?s<=s2:s>=s2)
	{
		if(*s && (*s==c)==m)
			return s;
		s+=j;
	}
	return 0;
}

void strChr2(char* s, char* s2, bool f[256])
{
	for(int aux=0;aux<256;aux++)
		f[aux]=0;
	if(s==0 || s2==0)
		return;
	while(s<=s2)
	{
		f[(int)*s]=1;
		s++;
	}
}

void strPrint(char* s, char* s2)
{
	while(*s && s<=s2)
	{
		if(*s!=' ')
			std::cout << *s;
		else
			std::cout << "X";
		s++;
	}
	std::cout << '\n';
}

struct Node
{
	char *name, *value;
	Node *next=0, *previous=0, *parent=0, *children=0;
};

Node* xmlParser(char* content)
{
	char *i, *i2, *i3, *i4, *i5, *i6, *i7, *i8, *i9;
	unsigned int n=strLen(content),f2[n]={0};
	bool f[256];
	Node aux, *tree=0, *pathBegin=0, *pathEnd=0;

	i3=content;
	while(i3)
	{
		i=i3;
		i2=strChr(i, content+n, '>', true);
		i3=strChr(i2, content+n, '<', true);
		if(i3 && i3<i2)
		{
			std::cout << "\"Less than\" character detected inside tag";
			return 0;
		}

		i++;
		i2--;

		///Element name extraction
		i4=strChr(i, i2, ' ', true);
		if(i4==0)
			i4=i2;
		else
			i4--;
		std::cout << "Name: ";
		strPrint(i,i4);

		std::cout << "Debug: " << i << ' ' << i4 << '\n';
		if(*i==' ' || *i4=='<')
		{
			std::cout << "Name is empty\n";
			return 0;
		}
		
		strChr2(i, i4, f);
		if(f[34]==1 || f[39]==1 || f['='])
		{
			std::cout << "Name contains invalid characters\n";
			return 0;
		}

		///Attribute extraction
		if(i4!=i2)
		{	
			i5=strChr(i4+1, i2, ' ' , false);
			if(i5 && *i5=='=')
			{
				std::cout << "Left extremity attribute lvalue missing\n";
				return 0;
			}
			i5=strChr(i2, i4+1, ' ', false);
			if(i5 && *i5=='=')
			{
				std::cout << "Right extremity attribute rvalue missing\n";
				return 0;
			}
			
			i5=strChr(i4+1, i2, '=', true);
			while(i5)
			{
				///Attribute lvalue
				i6=strChr(i5-1, i4+1, ' ', false);
				i7=strChr(i6, i4+1, ' ', true)+1;
				strChr2(i7, i6, f);
				if(f['_']==1 || f[34]==1 || f[39]==1)
				{
					std::cout << "Attribute lvalue contains invalid characters\n";
					return 0;
				}
				std::cout << "Attribute lvalue: ";
				strPrint(i7, i6);

				///Attribute rvalue
				i8=strChr(i5+1, i2, ' ', false);
				if(*i8!=34 && *i8!=39)
				{
					std::cout << "Attribute rvalue not quoted\n";
					return 0;
				}
				i9=strChr(i8+1, i2, *i8, true);
				if(i9==0)
				{
					std::cout << "Attribute lvalue closing quotes missing\n";
					return 0;
				}
				std::cout << "Attribute rvalue: ";
				strPrint(i8, i9);

				i5=strChr(i5+1, i2, '=', true);
			}
		}

	}

}

