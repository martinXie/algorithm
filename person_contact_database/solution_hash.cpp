#define LEN 20
#define FIELD_NUMBER 5
#define MAX_RECORD 50000
#define MAX_SAME 100
#define NULL 0
enum FIELD
{
	NAME,
	NUMBER,
	BIRTHDAY,
	EMAIL,
	MEMO
};
struct RESULT
{
	int number;
	char str[LEN];
};
struct Record
{
	char str[FIELD_NUMBER][LEN];
};

void Strcpy(char* dest, char* src)
{
	int i = 0;
	while (*(src + i) != '\0')
	{
		*(dest + i) = *(src + i);
		i++;
	}
	*(dest + i) = '\0';
}
class List
{
public:
	List() :count(0){};
	List(List*p)
	{ 
		count = p->count; 
		for (int i = 0; i < count; i++)
		{
			data[i] = p->data[i];
		}

	};
	void Add(int id)
	{
		int index = Get(id);
		if (index == -1)
		{
			data[count] = id;
			count++;
		}
	};
	void Delete(int id)
	{
		int index = Get(id);
		if (index != -1)
		{
			data[index] = data[count - 1];
			count--;
		}
	};
	int Get(int id)
	{
		for (int i = 0; i < count; i++)
		{
			if (data[i] == id)
			{
				return i;
			}
		}
		return -1;
	};
public:
	int data[MAX_SAME];
	int count;
};


class HashTable
{
public:
	HashTable()
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < MAX_RECORD; j++)
			{
				list[i][j] = NULL;
			}
		}
	};
	~HashTable()
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < MAX_RECORD; j++)
			{
				if (list[i][j] != NULL)
				{
					delete list[i][j];
					list[i][j] = NULL;
				}
			}
		}
	};
	void Add(FIELD field, char* str, int record_id)
	{
		unsigned int index = hash(str);
		if (list[field][index] == NULL)
		{
			list[field][index] = new List;
		}
		list[field][index]->Add(record_id);
	};
	List* Search(FIELD field, char* str)
	{
		unsigned int index = hash(str);
		return list[field][index];
	}
	void Delete(FIELD field, char* str, int record_id)
	{
		unsigned int index = hash(str);
		if (list[field][index] != NULL)
		{
			list[field][index]->Delete(record_id);
			if (list[field][index]->count == 0)
			{
				delete list[field][index];
				list[field][index] = NULL;
			}
		}
	}
private:
	unsigned int hash(char* str)
	{
		unsigned int c = 0;
		while (*str != '\0')
		{
			c = c * 131 + *str;
			str++;
		}
		int hashKey = c & 0x7FFFFFFF;
		return hashKey % MAX_RECORD;
	};
public:
	List* list[FIELD_NUMBER][MAX_RECORD];
};

Record db[MAX_RECORD];
int record_id = 0;
HashTable* hashTable = NULL;
void InitDB(void)
{
	if (hashTable != NULL)
	{
		delete hashTable;
	}
	hashTable = new HashTable;
	record_id = 0;
}

int Delete(FIELD field, char* str)
{
	int count = 0;
	List* recordList = hashTable->Search(field, str);
	if (recordList != NULL)
	{
		List list(recordList);
		count = list.count;
		for (int i = 0; i < list.count; i++)
		{
			int index = list.data[i];
			hashTable->Delete(NAME, db[index].str[NAME], index);
			hashTable->Delete(NUMBER, db[index].str[NUMBER], index);
			hashTable->Delete(BIRTHDAY, db[index].str[BIRTHDAY], index);
			hashTable->Delete(EMAIL, db[index].str[EMAIL], index);
			hashTable->Delete(MEMO, db[index].str[MEMO], index);
		}
	}
	return count;
}
int Change(FIELD field, char* str, FIELD changeField, char* changStr)
{
	int count = 0;
	List* recordList = hashTable->Search(field, str);
	if (recordList != NULL)
	{
		List list(recordList);
		count = list.count;
		for (int i = 0; i < list.count; i++)
		{
			int index = list.data[i];
			char*oldStr = db[index].str[changeField];
			hashTable->Delete(changeField, oldStr, index);
			hashTable->Add(changeField, changStr, index);
			Strcpy(db[index].str[changeField], changStr);
		}

	}
	return count;
}
RESULT Search(FIELD field, char* str, FIELD returnField)
{
	RESULT result = { 0 };
	List* recordList = hashTable->Search(field, str);
	if (recordList != NULL)
	{
		result.number = recordList->count;
		if (recordList->count == 1)
		{
			int index = recordList->data[0];
			Strcpy(result.str, db[index].str[returnField]);
		}
	}
	return result;
}
void Add(char* name, char* number, char* birthday, char* email, char* memo)
{
	hashTable->Add(NAME, name, record_id);
	hashTable->Add(NUMBER, number, record_id);
	hashTable->Add(BIRTHDAY, birthday, record_id);
	hashTable->Add(EMAIL, email, record_id);
	hashTable->Add(MEMO, memo, record_id);

	Strcpy(db[record_id].str[NAME], name);
	Strcpy(db[record_id].str[NUMBER], number);
	Strcpy(db[record_id].str[BIRTHDAY], birthday);
	Strcpy(db[record_id].str[EMAIL], email);
	Strcpy(db[record_id].str[MEMO], memo);

	record_id++;
}
