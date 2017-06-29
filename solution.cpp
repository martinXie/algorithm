
#define MAX 100
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

struct Record
{
	char name[LEN];
	char birthday[LEN];
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

class Vector //ֻ���ӣ���ɾ��
{
public:
	Vector() :count(0), currentRecordId(0) {};
	void push_back(char* name, char* birthday)
	{
		currentRecordId = count;
		strcpy(data[count].birthday, birthday);
		strcpy(data[count].name, name);
		count++;
	};
	char* get(int id, FEILD field)
	{
		char* str = nullptr;
		switch (field)
		{
		case NAME:
			str = data[id].name;
			break;
		case BIRTHDAY:
			str = data[id].birthday;
			break;
		default:
			break;
		}
		return str;
	};
	void update(int index, FEILD field, char* newStr)
	{
		strcpy(get(index, field), newStr);
	};
public:
	Record data[MAX];
	int currentRecordId;
	int count;
};

class List
{
public:
	int node[MAX];   //�����¼id
	int count;
public:
	List() :count(0) {};
	void Add(int data) // �����ڣ������ӣ����ڣ������
	{
		int index = Get(data);
		if (index == -1)
		{
			node[count] = data;
			count++; //����Խ��
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
	TrieNode* next[MAX];
	List list;  //���ʶ�Ӧ�ļ�¼����
	TrieNode() :isStr(false) { for (int i = 0; i < MAX; i++) next[i] = nullptr; };
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
			if (p->next[i] != nullptr)
			{
				Free(p->next[i]);
			}
		}
		delete p;

	};
	void Insert(const char* str, int record_id) // str,id���Ѵ��ڣ�str����id��ͬ��str�����ڣ�id�����ڣ�
	{
		TrieNode* p = root;
		while (p != nullptr && *str != '\0')
		{
			if (p->next[*str - 'a'] == nullptr)
			{
				TrieNode* tmp = new TrieNode();
				p->next[*str - 'a'] = tmp;
			}
			p = p->next[*str - 'a'];
			str++;
		}
		p->list.Add(record_id);
		p->isStr = true;
	};
	TrieNode* Search(char* str)
	{
		TrieNode* p = root;
		while (p != nullptr && *str != '\0')
		{
			p = p->next[*str - 'a'];
			str++;
		}
		if (p != nullptr && p->isStr == true)
		{
			return p;
		}
		return nullptr;
	};
	void Change(char* str, char* newStr)
	{
		TrieNode*p = Search(str);
		if (p != nullptr)  //�����£���ɾ��
		{
			int count = p->list.count;
			for (int i = 0; i < count; i++)
			{
				Insert(newStr, p->list.node[i]);
			}
			DeleteAll(str);
		}
	};
	void Delete(char* str, int id)
	{
		TrieNode* p = Search(str);
		if (p != nullptr)
		{
			p->list.Delete(id);
			if (p->list.count == 0)
			{
				p->isStr = false;
			}
		}
	};
	void DeleteAll(char* str)
	{
		TrieNode* p = Search(str);
		if (p != nullptr)
		{
			p->isStr = false;
			p->list.count = 0;
		}
	};

};

class DataBase
{
public:
	void Add(char* name, char* birthday)
	{
		db.push_back(name, birthday);
		trie[NAME].Insert(name, db.currentRecordId);
		trie[BIRTHDAY].Insert(birthday, db.currentRecordId);
	};
	Result Search(FEILD field, char* str, FEILD returnField)
	{
		Result result = { 0 };
		TrieNode*p = trie[field].Search(str);
		if (p != nullptr)
		{
			result.number = p->list.count;
			if (result.number == 1)
			{
				int index = p->list.node[0];
				strcpy(result.str, db.get(index, returnField));
			}
		}
		return result;
	};
	int Change(FEILD field, char* str, FEILD changeField, char* changStr) // field == changeField; field != changeField;
	{
		int count = 0;
		if (field == changeField)
		{
			TrieNode*p = trie[field].Search(str);
			if (p != nullptr)  //�����£���ɾ��
			{
				count = p->list.count;
				for (int i = 0; i < count; i++)
				{
					trie[changeField].Insert(changStr, p->list.node[i]);
					db.update(p->list.node[i], changeField, changStr);
				}
				trie[field].DeleteAll(str);
			}

		}
		else
		{
			TrieNode*p = trie[field].Search(str);
			if (p != nullptr)
			{
				for (int i = 0; i < p->list.count; i++)
				{
					int index = p->list.node[i];
					if (strcmp(changStr, db.get(index, changeField)) != 0) //��¼�Ķ�Ӧ�ֶ��Ѿ���changestr������,��ɾ�ɺ�����
					{
						trie[changeField].Delete(db.get(index, changeField), index);
						trie[changeField].Insert(changStr, index);
						db.update(index, changeField, changStr);
						count++;
					}
				}
			}

		}
		return count;
	}
	int Delete(FEILD field, char* str)
	{
		TrieNode*p = trie[field].Search(str);
		int count = p->list.count;
		if (p != nullptr)
		{
			trie[field].DeleteAll(str);
		}
		return count;
	};
public:
	Trie trie[MAX];
	Vector db;
};

DataBase* contact = nullptr;

void initial(void )
{
	if (contact != nullptr)
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

