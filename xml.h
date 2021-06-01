/// simple xml library written in c
#include <stdio.h>

/// return address of first matching/non-matching character between adresses begin and end
char *xmlChr(char *begin, char *end, char c, char m)
{
	if(begin == 0 || end == 0)
		return 0;
	while(begin <= end)
	{
		if((*begin == c) == m)
			return begin;
		begin++;
	}
	return 0;
}

/// return wheter if characters between adresses begin and end are identical with the ones between adresses begin2 and end2
char xmlCmp(char *begin, char *end, char *begin2, char *end2)
{
	if(begin == 0 || end == 0 || begin2 == 0 || end2 == 0)
		return 0;
	while(begin <= end && begin2 <= end2)
	{
		if(*begin != *begin2)
			return 0;
		begin++;
		begin2++;
	}
	return (begin - end) == (begin2 - end2);
}

/// return wheter if characters between begin and end are alphanumeric (a-z + A-Z + 0-9)
char xmlAlphanum(char *begin, char *end)
{
	if(begin == 0 || end == 0)
		return 1;
	while(begin <= end)
	{
		if((*begin >= 'a' && *begin <= 'z') == 0 && (*begin >= 'A' && *begin <= 'Z') == 0 && (*begin >= '0' && *begin <= '9') == 0)
			return 0;
		begin++;
	}
	return 1;
}

/// unordered list data type used to store current path 
struct xmlPath
{
	char *nameBegin, *nameEnd;
	struct xmlPath *next, *previous;
};
typedef struct xmlPath xmlPath;

/// convert list to string
char *xmlCnv(xmlPath *pathBegin, unsigned int pathSize)
{
	char *path = malloc(pathSize), *pathIterator = path, *nameIterator;
	while(pathBegin != 0)
	{
		*pathIterator = '/';
		pathIterator++;
		nameIterator = pathBegin -> nameBegin;
		for(nameIterator = pathBegin -> nameBegin;nameIterator != pathBegin -> nameEnd;nameIterator++, pathIterator++)
			*pathIterator = *nameIterator;
		pathBegin = pathBegin -> next;
	}
	return path;
}

/// push back name to list and modify ends
void xmlPush(xmlPath **pathBegin, xmlPath **pathEnd, char *nameBegin, char *nameEnd)
{
	xmlPath *node = malloc(sizeof(xmlPath));
	node -> nameBegin = nameBegin;
	node -> nameEnd = nameEnd;
	node -> previous = *pathEnd;
	node -> next = 0;
	if(*pathBegin == 0 && *pathEnd == 0)
		*pathBegin = *pathEnd = node;
	else
	{
		(*pathEnd) -> next = node;
		*pathEnd = node;
	}
}

/// pop back name from list and modify ends
void xmlPop(xmlPath **pathBegin, xmlPath **pathEnd)
{
	xmlPath *node = *pathEnd;
	if(node -> previous)
		node -> previous -> next = node -> next;
	if(node -> next)
		node -> next -> previous = node -> previous;
	if(*pathBegin == *pathEnd)
		*pathBegin = *pathEnd = 0;
	else
		*pathEnd = (*pathEnd) -> next;
	free(node);
}

/// free list and modify ends
void xmlFree(xmlPath **pathBegin, xmlPath **pathEnd)
{
	xmlPath* node;
	while(*pathBegin != 0)
	{
		node = *pathBegin;
		*pathBegin = (*pathBegin) -> next;
		free(node);
	}
	*pathBegin = *pathEnd = 0;
}

/// xml map to store hashed elements
struct xmlMap
{
	char *path, *value;
	struct xmlMap *next;
};
typedef struct xmlMap xmlMap;

/// hash parameters
#define mapPrime 41
#define mapSize ~(1 << (sizeof(unsigned int) * 8 - 1))

/// hash path using polinomial rolling hash
unsigned int xmlHash(char *path)
{
	unsigned int hash = 0, power = 1, position;
	while(path != 0)
	{
		if(*path == '/')
			position = 1;
		else if(*path >= '0' && *path <= '9')
			position = *path - '0' + 2;
		else if(*path >= 'a' && *path <= 'z')
			position = *path - 'a' + 12;
		else if(*path >= 'A' && *path <= 'Z')
			position = *path - 'A' + 38;
		hash = (hash + position * power) % mapSize;
		power = (power * mapPrime) % mapSize;
		path++;
	}
	return hash;
}

/// to be implemented
//char *xmlGet();
//char *xmlSet();
//char *xmlOut();

/// exit messages
#define error "error in \"%.*s\": "
#define stringDataBegin "invalid string before data"
#define stringDataEnd "invalid string after data"
#define missingTagBegin "missing beginning of tag"
#define missingTagEnd "missing end of tag"
#define nameEmpty "name is empty"
#define nameInvalid "name contains invalid characters"
#define missingAttrAssign "missing attribute assignment operator"
#define multipleAttrAssign "multiple attribute assignment operators"
#define missingLvalue "missing left value"
#define attrTagClose "attributes in closing tag"
#define lvalueInvalid "left value contains invalid characters"
#define missingRvalue "missing right value"
#define missingRvalueBegin "missing beginning of right value"
#define missingRvalueEnd "missing end of right value"
#define textTagClose "text in closing tag"
#define missingTagOpen "missing opening tag"
#define missingTagClose "missing closing tag"
#define mismatchTag "mismatch between opening and closing tags"

/// return a unordered map containing hased xml elements
void xmlIn(char *dataBegin)
{
	/// variable section
	char lvalue = 0, assign = 0, close = 0, text = 0;
	char *message = 0;
	char *dataEnd = 0;
	char *tagBegin = 0, *tagEnd = 0, *tagBegin2 = 0;
	char *nameBegin = 0, *nameEnd = 0;
	char *attrBegin = 0, *attrEnd = 0, *attrSpacer = 0;
	char *lvalueBegin = 0, *lvalueEnd = 0;
	char *rvalueBegin = 0, *rvalueEnd = 0;
	char *textBegin = 0, *textEnd = 0;
	unsigned int pathSize = 0;
	xmlPath *pathBegin = 0, *pathEnd = 0;

	/// tag loop
	if(*dataBegin == 0)
		return;
	dataEnd = dataBegin;
	while(*dataEnd != 0)
		dataEnd++;
	dataEnd--;
	tagBegin2 = xmlChr(dataBegin, dataEnd, '<', 1);
	if(tagBegin2 > dataBegin && xmlChr(dataBegin, tagBegin2, ' ', 0) != 0)
	{
		/// string before data
		message = stringDataBegin;
		goto exit;
	}
	while(tagBegin2 != 0)
	{
		/// delimit tag
		tagBegin = tagBegin2;
		tagEnd = xmlChr(tagBegin + 1, dataEnd, '>', 1);
		tagBegin2 = xmlChr(tagBegin + 1, dataEnd, '<', 1);
		if(tagEnd == 0 || (tagBegin2 != 0 && tagBegin2 < tagEnd))
		{
			/// missing end of tag
			message = missingTagEnd;
			goto exit;
		}
		if(tagBegin2 != 0 && xmlChr(tagEnd + 1, tagBegin2 - 1, '>', 1) != 0)
		{
			/// missing beginning of tag
			message = missingTagBegin;
			goto exit;
		}

		/// delimit name
		nameBegin = xmlChr(tagBegin + 1, tagEnd - 1, ' ' , 0);
		if(nameBegin == 0 || (nameBegin == nameEnd && *nameBegin == '/'))
		{
			/// name is empty
			message = nameEmpty;
			goto exit;
		}
		if(*nameBegin == '/')
		{
			close = 1;
			nameBegin++;
		}
		nameEnd = xmlChr(nameBegin + 1, tagEnd - 1, ' ', 1);
		if(nameEnd == 0)
			nameEnd = tagEnd - 1;
		else
			nameEnd--;
		if(xmlAlphanum(nameBegin, nameEnd) == 0)
		{
			/// name is invalid
			message = nameInvalid;
			goto exit;
		}

		/// validate name
		if(close == 0)
			xmlPush(&pathBegin, &pathEnd, nameBegin, nameEnd);
		else
		{
			if(pathBegin == 0 || pathEnd == 0)
			{
				/// missing opening tag
				message = missingTagOpen;
				goto exit;
			}
			if(xmlCmp(nameBegin, nameEnd, pathEnd -> nameBegin, pathEnd -> nameEnd) == 0)
			{
				/// mismatch between tags
				message = mismatchTag;
				goto exit;
			}
			xmlPop(&pathBegin, &pathEnd);
		}
		pathSize += (close == 1 ? 1 : -1) * (nameEnd - nameBegin + 2);

		/// name handler
		fputs(close == 0 ? "opening name: " : "closing name: ", stdout);
		//xmlDebug(stdout, nameBegin, nameEnd);
		fputc('\n', stdout);

		/// attribute loop
		lvalue = 1;
		assign = 0;
		attrBegin = xmlChr(nameEnd + 1, tagEnd - 1, ' ', 0);
		if(close == 1 && attrBegin != 0)
		{
			/// attributes in closing tag
			message = attrTagClose;
			goto exit;
		}
		while(attrBegin != 0)
		{
			/// delimit attributes
			attrEnd = xmlChr(attrBegin + 1, tagEnd - 1, ' ', 1);
			if(attrEnd == 0)
				attrEnd = tagEnd - 1;
			else
				attrEnd--;
			attrSpacer = xmlChr(attrBegin, attrEnd, '=', 1);

			/// lvalue handler
			if(lvalue == 1)
			{
				if(attrBegin == attrSpacer)
				{
					/// missing left value
					message = missingLvalue;
					goto exit;
				}
				lvalueBegin = attrBegin;
				lvalueEnd = attrSpacer == 0 ? attrEnd : attrSpacer - 1;
				if(xmlAlphanum(lvalueBegin, lvalueEnd) == 0)
				{
					/// left value is invalid
					message = lvalueInvalid;
					goto exit;
				}
				lvalue = 0;
				assign = attrEnd == attrSpacer;

				/// rvalue clipped to lvalue
				if(attrSpacer != 0 && attrSpacer < attrEnd)
				{
					attrBegin = attrSpacer;
					goto rvalue;
				}
			}
			/// rvalue handler
			else
			{
				if(attrBegin != attrSpacer && assign == 0)
				{
					/// missing attribute assignment operator
					message = missingAttrAssign;
					goto exit;
				}
				if(attrBegin == attrSpacer && assign == 1)
				{
					/// multiple attribute assignment operators
					message = multipleAttrAssign;
					goto exit;
				}
				if(attrBegin == attrEnd && attrBegin == attrSpacer)
					assign = 1;
				else
				{
					if(attrEnd == attrSpacer)
					{
						/// missing rvalue
						message = missingRvalue;
						goto exit;
					}
					rvalue:
					rvalueBegin = attrSpacer == 0 ? attrBegin : attrSpacer + 1;
					if(*rvalueBegin != '\"' && *rvalueBegin != '\'')
					{
						/// missing beginning of rvalue
						message = missingRvalueBegin;
						goto exit;
					}
					rvalueEnd = xmlChr(rvalueBegin + 1, tagEnd - 1, *rvalueBegin, 1);
					if(rvalueEnd == 0 || (rvalueEnd + 1 < tagEnd && *(rvalueEnd + 1) != ' '))

					{
						/// missing end of rvalue
						message = missingRvalueEnd;
						goto exit;
					}
					attrEnd = rvalueEnd;
					rvalueBegin++;
					rvalueEnd--;
					lvalue = 1;
					assign = 0;
				}
			}
			attrBegin = xmlChr(attrEnd + 1, tagEnd - 1, ' ', 0);

			/// attribute handler
			if(lvalue == 1)
			{
				fputs("attribute: ", stdout);
				//xmlDebug(stdout, lvalueBegin, lvalueEnd);
				fputs(" \"", stdout);
				//xmlDebug(stdout, rvalueBegin, rvalueEnd);
				fputs("\"\n", stdout);
			}
		}
		if(lvalue == 0)
		{
			/// missing right value
			message = missingRvalue;
			goto exit;
		}

		/// text delimiter
		textBegin = tagEnd + 1;
		textEnd = tagBegin2 == 0 ? dataEnd : tagBegin2 - 1;
		text = xmlChr(textBegin, textEnd, ' ', 0) != 0;
		if(close == 1 && text == 1)
		{
			/// text in closing tag
			message = textTagClose;
			goto exit;
		}

		/// text handler
		if(text == 1)
		{
			fputs("text: \"", stdout);
			//xmlDebug(stdout, textBegin, textEnd);
			fputs("\"\n", stdout);
		}
	}
	if(tagEnd + 1 < dataEnd && xmlChr(tagEnd + 1, dataEnd, ' ', 0) != 0)
	{
		/// string after data
		message = stringDataEnd;
		goto exit;
	}
	if(pathBegin != 0 || pathEnd != 0)
	{
		/// missing closing tag
		message = missingTagClose;
		goto exit;
	}

	/// exit
	exit:
	if(message != 0)
		fputs(message, stderr);
	xmlFree(&pathBegin, &pathEnd);
	return;
}
