#define NULL 0
#define TRIE_NODE 62
#define MAX 100
#define STR_LEN 20
#define FEILD_NUMBER 5
enum FEILD
{
	NAME,
	BIRTHDAY,
	NUMBER,
	EMAIL,
	MEMO
};

struct Result
{
	int number;
	char str[STR_LEN];
};

struct Record
{
	char str[FEILD_NUMBER][STR_LEN]
};

void strcpy(char* dest, char* src)
{
	int i = 0;
	while (*(src+i) != '\0')
	{
		*(dest + i) = *(src + i);
		i++;
	}
	*(dest + i) = '\0';
}

int strcmp(char* a, char* b)
{
	int i = 0;
	while (*(a + i) != '\0' && *(b + i) != '\0')
	{
		if (*(a + i) == *(b + i))
		{
			i++;
		}
		else
		{
			break;
		}
	}
	if (*(a + i) == '\0' && *(b + i) == '\0')
	{
		return 0;
	}
	return 1;
}

class Vector //只添加，不删除
{
public:
	Vector() :count(0){};
	void add(char* name, char* birthday, char* number, char* email, char* memo)
	{
		strcpy(data[count].str[BIRTHDAY], birthday);
		strcpy(data[count].str[NAME], name);
		strcpy(data[count].str[NUMBER], number);
		strcpy(data[count].str[EMAIL], email);
		strcpy(data[count].str[MEMO], memo);
		count++;
	};
	char* get(int id, FEILD field)
	{
		return data[id].str[field];
	};
	void update(int index, FEILD field, char* newStr)
	{
		strcpy(get(index, field), newStr);
	};
public:
	Record data[MAX];
	int count;
};

class List
{
public:
	int node[MAX];   //保存记录id
	int count;
public:
	List() :count(0) {};
	List(List* p) 
	{ 
		count = p->count; 
		for (int i = 0; i < count; i++)
		{
			node[i] = p->node[i];
		}
	}
	void Add(int data) // 不存在，则添加；存在，则忽略
	{
		int index = Get(data);
		if (index == -1)
		{
			node[count] = data;
			count++; //可能越界
		}
	};
	void Delete(int data)
	{
		int index = Get(data);
		if (index != -1)
		{
			for (int i = index; i < count - 1; i++)
			{
				node[i] = node[i + 1];
			}
			count--;
		}
	};
	int Get(int data)
	{
		int retIndex = -1;
		for (int i = 0; i < count; i++)
		{
			if (data == node[i])
			{
				retIndex = i;
			}
		}
		return retIndex;
	};
};

struct TrieNode
{
	bool isStr;
	TrieNode* next[TRIE_NODE];
	List* list;  //单词对应的记录集合
	TrieNode() :isStr(false),list(NULL) { for (int i = 0; i < MAX; i++) next[i] = NULL; };
};

class Trie
{
public:
	TrieNode* root;
public:
	Trie() { root = new TrieNode(); };
	~Trie() { Free(root); };
	void Free(TrieNode* p)
	{
		for (int i = 0; i < MAX; i++)
		{
			if (p->next[i] != NULL)
			{
				Free(p->next[i]);
			}
		}
		delete p;

	};
	void Insert(const char* str, int record_id) // str,id都已存在；str存在id不同；str不存在，id不存在；
	{
		TrieNode* p = root;
		while (*str != '\0')
		{
			int index = m_GetIndex(*str);
			if (p->next[index] == NULL)
			{
				TrieNode* tmp = new TrieNode();
				p->next[index] = tmp;
			}
			p = p->next[index];
			str++;
		}
		p->isStr = true;
		if (p->list == NULL)
		{
			p->list = new List;
		}
		p->list->Add(record_id);
	};
	TrieNode* Search(char* str)
	{
		TrieNode* p = root;
		while (p != NULL && *str != '\0')
		{
			int index = m_GetIndex(*str);
			p = p->next[index];
			str++;
		}
		if (p != NULL && p->isStr == true)
		{
			return p;
		}
		return NULL;
	};
	void Change(char* str, char* newStr)
	{
		TrieNode*p = Search(str);
		if (p != NULL)  //先增新，后删旧
		{
			int count = p->list->count;
			List list(p->list);
			for (int i = 0; i < count; i++)
			{
				Insert(newStr, list.node[i]);
				Delete(str, list.node[i]);
			}
		}
	};
	void Delete(char* str, int id)
	{
		TrieNode* p = Search(str);
		if (p != NULL)
		{
			p->list->Delete(id);
			if (p->list->count == 0)
			{
				p->isStr = false;
			}
		}
	};
private:
	int m_GetIndex(char c)
	{
		if (c >= 'a' && c <= 'z')
		{
			return c - 'a';
		}
		else if (c >= 'A'&& c <= 'Z')
		{
			return c - 'A' + 26;
		}
		else if (c >= '0'&& c <= '9')
		{
			return c - '0' + 52;
		}
		else
		{
			return 0;
		}
	};
};

class DataBase
{
public:
	void Add(char* name, char* birthday, char* number, char* email, char* memo)
	{
		trie[NAME].Insert(name, db.count);
		trie[BIRTHDAY].Insert(birthday, db.count);
		trie[NUMBER].Insert(number, db.count);
		trie[EMAIL].Insert(email, db.count);
		trie[MEMO].Insert(memo, db.count);
		db.add(name, birthday, number, email, memo);
	};
	Result Search(FEILD field, char* str, FEILD returnField)
	{
		Result result = { 0 };
		TrieNode*p = trie[field].Search(str);
		if (p != NULL)
		{
			result.number = p->list->count;
			if (result.number == 1)
			{
				int index = p->list->node[0];
				strcpy(result.str, db.get(index, returnField));
			}
		}
		return result;
	};
	int Change(FEILD field, char* str, FEILD changeField, char* changStr)
	{
		int count = 0;
		TrieNode*p = trie[field].Search(str);
		if (p != NULL)
		{
			count = p->list->count;
			List list(p->list);
			for (int i = 0; i < count; i++)
			{
				int index = list.node[i];
				char* oldStr = db.get(index, changeField);
				if (strcmp(changStr, oldStr) != 0) //记录的对应字段已经是changestr不处理,先删旧后增新
				{
					trie[changeField].Delete(oldStr, index);
					trie[changeField].Insert(changStr, index);
					db.update(index, changeField, changStr);
				}
			}
		}
		return count;
	}
	int Delete(FEILD field, char* str)
	{
		int count = 0;
		TrieNode*p = trie[field].Search(str);
		if (p != NULL)
		{
			count = p->list->count;
			List list(p->list);
			for (int i = 0; i < count; i++)
			{
				int index = list.node[i];
				trie[field].Delete(str, index);
			}
		}
		return count;
	};
public:
	Trie trie[FEILD_NUMBER];
	Vector db;
};

DataBase* contact = NULL;

void initial(void )
{
	if (contact != NULL)
	{
		delete contact;
	}
	contact = new DataBase;
}
int Delete(FEILD field, char* str)
{
	return contact->Delete(field, str);
}
int Change(FEILD field, char* str, FEILD changeField, char* changStr)
{
	return contact->Change(field, str, changeField, changStr);
}
Result Search(FEILD field, char* str, FEILD returnField)
{
	return contact->Search(field, str, returnField);
}
void Add(char* name, char* birthday)
{
	contact->Add(name, birthday);
}


