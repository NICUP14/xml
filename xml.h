unsigned int strLen(char* s)
{
	///Return number of bytes before the first null character
	char* i=s;
	while(*i)
		i++;
	return i-s;
}

void strCpy(char* s, char* s2, char* s3)
{
	///Copies bytes between s2 and s3 into s
	while(s2<=s3)
	{
		*s=*s2;
		s++;
		s2++;
	}
}

char* strChr(char* s, char* s2, char c, int step=1)
{
	///Returns first occurence of any char c between s and s2
	char* i;
	while(s<=s2)
	{
		if(*s==c)
			return s;
		s+=step;
	}
	return 0;
}

struct node
{
	char *name=0;
	node *next=0, *previous=0, *children=0;
}

void add(node* &begin, node* &end, char* value, char* name)
{
	///Adds new node at the end of the list
	unsigned int nn=strLen(name),nv=strLen(value);
	node* aux=malloc(sizeof(node));
	aux->name=malloc(nn);
	strCpy(aux->name,name,name+nn);
	aux->previous=end;

	if(begin==0 || end==0)
		begin=end=aux;
	else
		end=aux;
}

void remove(node* &begin, node* &end, node* aux)
{
	///Removes node aux from the list
	if(begin==aux)
		begin=begin->next;
	else
	{
		if(end==aux)
			end=end->previous;
		else
		{
			aux->previous->next=aux->next;
			aux->next->previous=aux->previous;
		}
	}

	free(aux->name);
	free(aux->value);
	free(aux);
	aux=0;
}

node* xmlParser(char* content)
{
	char *i,*i2;
	unsigned int n=strLen(content),enter,leave;
	node aux,*pathBegin=0, *pathEnd=0, *treeBegin=0;

	i=strChr(
	while(*content)
	{
}
