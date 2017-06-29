#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LEN 20
enum FEILD
{
	NAME,
	BIRTHDAY
};
struct Result
{
	int number;
	char str[LEN];
};

extern void initial(void);
extern int Delete(FEILD field, char* str);
extern Result Search(FEILD field, char* str, FEILD returnField);
extern int Change(FEILD field, char* str, FEILD changeField, char* changStr);
extern void Add(char* name, char* birthday);


int main(void)
{
	initial();
	Add("aaa", "19900227");
	Result result = Search(NAME, "aaa", BIRTHDAY);
	if (result.number != 1 || strcmp(result.str, "19900227") != 0)
	{
		printf("search error %d", __LINE__);
		return -1;
	}
	Add("aaa", "19900228");
	result = Search(NAME, "aaa", BIRTHDAY);
	if(result.number != 2)
	{
		printf("search error %d", __LINE__);
		return -1;
	}
	Add("bbb", "19900229");
	Add("bbb", "19900230");
	Add("ccc", "19900231");

	int ret = Change(NAME, "bbb", NAME, "ddd");
	if (ret != 2)
	{
		printf("search error %d", __LINE__);
		return -1;
	}
	result = Search(NAME, "bbb", BIRTHDAY);
	if (result.number != 0)
	{
		printf("search error %d", __LINE__);
		return -1;
	}
	
	result = Search(NAME, "ddd", BIRTHDAY);
	if (result.number != 2)
	{
		printf("search error %d", __LINE__);
		return -1;
	}

	ret = Change(NAME, "ddd", BIRTHDAY, "19900227");
	if (ret != 2)
	{
		printf("search error %d", __LINE__);
		return -1;
	}
	
	result = Search(BIRTHDAY, "19900227", NAME);
	if (result.number != 3)
	{
		printf("search error %d", __LINE__);
		return -1;
	}
	ret = Delete(BIRTHDAY, "19900227");
	if(ret != 3)
	{
		printf("search error %d", __LINE__);
		return -1;
	}
	result = Search(BIRTHDAY, "19900227", NAME);
	if (result.number != 0)
	{
		printf("search error %d", __LINE__);
		return -1;
	}

	result = Search(NAME, "aaa", BIRTHDAY);
	if (result.number != 1 || strcmp(result.str, "19900228") != 0 )
	{
		printf("search error %d", __LINE__);
		return -1;
	}
	return 0;
}
