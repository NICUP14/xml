unsigned int length(char *s)
{
	///Returns number of characters before null
	char* i=s;
	while(*i)
		i++;
	return s-i;
}

char* find(char* s, char* s2, char c)
{
	///Returns first occurence of character c between adress ranges s and s2
	if(s==0 || s2==0)
		return 0;
	while(*s!=0 && s<=s2)
	{
		if(*s==c)
			return s;
		s++;
	}
	return 0;
}

void word(char* s, char* s2, char* &s3, char* &s4)
{
	///Extracts first word between adress ranges s, s2 via iterators
	if(s==0 || s2==0)
		return;
	while(*s!=0 && s<=s2 && *s==' ')
		s++;
	if(*s==0 || s>s2)
	{
		s3=s4=0;
		return;
	}
	else
		s3=s;
	while(*s!=0 && s<=s2 && *s!=' ')
		s++;
	s4=(s>s2)?s2:s-1;
}

void mark(char* s, char* s2, bool *f)
{
	///Marks present characters between adress ranges s, s2
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

void print(char* s, char* s2)
{
	///Prints all characters between adress ranges s, s2 also making spaces visible (debug)
	if(s==0 || s2==0)
		return;
	while(*s && s<=s2)
	{
		if(*s==' ')
			std::cout << "[space]";
		else
			std::cout << *s;
		s++;
	}
	std::cout << '\n';
}

void xmlParser(char* data)
{
	bool lvalue, equal, f[256];
	unsigned int n=length(data);
	char *i, *i2, *i3, *i4, *i5, *i6, *i7, *i8, *i9, *i10;
	
	i3=data;
	if(*i3!=0 && *i3!='<')
	{
		std::cout << "Invalid characters before xml data\n";
		return;
	}

	while(i3)
	{
		///Tag and text splitting
		i=i3;
		i2=find(i+1, data+n, '>');
		i3=find(i2+1, data+n, '<');

		///Empty name exception
		if(i+1==i2 || *(i+1)==' ')
		{
			std::cout << "Empty name\n";
			return;
		}

		///Tag name extraction and validation
		word(i+1, i2-1, i4, i5);
		mark(i4, i5, f);
		if(f[(int)'<']==1 || f[(int)'=']==1 || f[(int)'_']==1 || f[(int)'\"']==1 || f[(int)'\"']==1)
		{
			std::cout << "Invalid characters in name\n";
			return;
		}

		std::cout << "Name: ";
		print(i4, i5);

		///Tag attribute parser
		if(i4!=0)
		{
			lvalue=0;
			equal=0;

			word(i5+1, i2-1, i4, i5);
			while(i4!=0)
			{
				i9=i10=0;
				i6=find(i4, i5, '=');
				
				///Assignment handler
				if(equal!=lvalue)
				{
					std::cout << "Missing equal in attribute assignment\n";
					return;
				}
				if(i6!=0)
				{
					if(equal==1)
					{
						std::cout << "Extra equal in attribute assignment\n";
						return;
					}
					equal=1;
				}

				if(i8>i7)
				{
					std::cout << "Missing lvalue\n";
					return;
				}

				if(i6==i4 && i6==i5)
					lvalue=1;
				else
				{
					if(lvalue==0)
					{
						///lvalue
						i7=i4;
						i8=(i6==0)?i5:i6-1;
						mark(i7, i8, f);
						


						if(f[(int)'<']==1 || f[(int)'=']==1 || f[(int)'\"']==1 || f[(int)'\'']==1)
						{
							std::cout << "invalid characters in lvalue\n";
							return;
						}
						///Rvalue clipped to lvalue
						if(i6!=0 && i5!=i6)
							goto rvalue;
						lvalue=1;
					}
					else if(lvalue==1)
					{
						///Rvalue
						rvalue:

						i9=(i6==0)?i4:i6+1;
						if(*i9!='\"' && *i9!='\'')
						{
							std::cout << "Missing rvalue opening quotes\n";
							return;
						}
						i10=find(i9+1, i2-1, *i9);
						if(i10==0)
						{
							std::cout << "Missing rvalue closing quotes\n";
							return;
						}

						mark(i9, i10, f);
						if(f[(int)'<']==1 || (i10<i2-1 && *(i10+1)!=' '))
						{
							std::cout << "Invalid characters in rvalue\n";
							return;
						}

						i9++;
						i10--;
						lvalue=0;
						equal=0;
					}
					if(i9!=0)
					{
						std::cout << "Lvalue: ";
						print(i7, i8);
						std::cout << "Rvalue: ";
						print(i9, i10);
					}
				}
				word(i5+1, i2-1, i4, i5);
			}
			if(lvalue==1)
			{
				std::cout << "Missing rvalue\n";
				return;
			}
		}
	}
}
