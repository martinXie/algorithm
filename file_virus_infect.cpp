#define KEY 9973
#define MAX 10000
class List;
struct list_node;
struct tree_node
{
	int id;
	bool isDir;
	int fileNum;
	int orgSize;
	int currentSize;
	tree_node* parent;
	List* childList;
	tree_node(int nId, int fileSize, tree_node* p) :id(nId), isDir(!fileSize), orgSize(fileSize), currentSize(fileSize), parent(p), childList(0)
	{ fileNum = (fileSize == 0) ? 0 : 1; };
	tree_node() {};
};
struct list_node
{
	tree_node* treeNode;
	list_node* next;
	list_node(tree_node*p) :treeNode(p), next(0) {};
};

class List
{
public:
	List(void)
	{
		head = new list_node(0);
		tail = head;
		count = 0;
	};
	~List(void) 
	{ 
		list_node* current = head;
		while (current != 0)
		{
			list_node* p = current->next;
			delete current;
			current = p;	
		}
	};
	void add(tree_node* data)
	{
		list_node* node = new list_node(data);
		tail->next = node;
		tail = node;
		count++;
	};
	void remove(tree_node* data)
	{
		list_node* prev = head;
		list_node* current = prev->next;
		while (current != 0 && current->treeNode != data)
		{
			prev = current;
			current = current->next;
		}
		if (current != 0)
		{
			if (tail == current)
			{
				delete current;
				prev->next = 0;
				tail = prev;
			}
			else
			{
				prev->next = current->next;
				delete current;
				current = 0;
			}
			count--;
		}
	};
	tree_node* get(int id)
	{
		list_node* p = head->next;
		while (p != 0 && p->treeNode != 0 && p->treeNode->id != id)
		{
			p = p->next;
		}
		if (p != 0 && p->treeNode != 0)
		{
			return p->treeNode;
		}
		else
		{
			return 0;
		}
	}
	tree_node* at(int n)
	{
		list_node* p = head->next;
		while (p != 0 && n != 0)
		{
			p = p->next;
			n--;
		}
		if (p != 0)
		{
			return p->treeNode;
		}
		return 0;
	};
	inline int size(void) { return count; };
public:
	int count;
	list_node* head; //哨兵
	list_node* tail;
};
class HashTable
{
public:
	HashTable()
	{
		for (int i = 0; i < MAX; i++)
		{
			db[i] = 0;
		};
	}
	~HashTable()
	{ 
		for (int i = 0; i < MAX; i++)
		{
			if (db[i] != 0 )
			{
				delete db[i];
			}
		}
	};
	void SaveData(tree_node*p)
	{
		int index = hash(p->id);
		if (db[index] == 0)
		{
			db[index] = new List;
		}
		db[index]->add(p);

	}
	tree_node* GetData(int id)
	{
		int index = hash(id);
		if (db[index] == 0)
		{
			return 0;
		}
		return db[index]->get(id);

	};
	void RemoveData(int id)
	{
		int index = hash(id);
		if (db[index] == 0)
		{
			return;
		}
		db[index]->remove(db[index]->get(id));
		if (db[index]->size() == 0)
		{
			delete db[index];
			db[index] = 0;
		}
	}
	int hash(int id)
	{
		return id % KEY;
	};
public:
	List* db[MAX];
};
class Tree
{
public:
	Tree()
	{ 
		table = new HashTable;  
		Add(10000, 0, 0); 
	};
	~Tree()
	{
		tree_node* root = table->GetData(10000);
		Free(root);
		delete root;
		delete table;
	};
	int Add(int id, int pid, int fileSize)
	{
		tree_node* parent = table->GetData(pid);
		tree_node* p = new tree_node(id, fileSize, parent);
		int size = 0;
		if (parent != 0)
		{
			if (parent->childList == 0)
			{
				parent->childList = new List;
			}
			parent->childList->add(p);
			if (fileSize != 0)
			{
				UpdateParentInfo(parent, fileSize, fileSize, 1);
			}
			size = parent->currentSize;
		}
		table->SaveData(p);
		return size;
	};
	int Move(int id, int pid)
	{
		tree_node* p = table->GetData(id);
		if (p->parent != 0 && p->parent->childList != 0)
		{
			p->parent->childList->remove(p);
			UpdateParentInfo(p->parent, -(p->orgSize), -(p->currentSize), -(p->fileNum));
		}
		tree_node* newParent = table->GetData(pid);
		if (newParent != 0)
		{
			if (newParent->childList == 0)
			{
				newParent->childList = new List;
			}
			newParent->childList->add(p);
			p->parent = newParent;
			UpdateParentInfo(p->parent, p->orgSize, p->currentSize, p->fileNum);
		}
		return newParent->currentSize;
	};
	int Remove(int id)
	{
		tree_node* p = table->GetData(id);
		int size = p->currentSize;
		if (p != 0 && p->parent != 0)
		{
			p->parent->childList->remove(p);
			UpdateParentInfo(p->parent, -(p->orgSize), -(p->currentSize), -(p->fileNum));
		}
		ClearTableData(p); //删除数据索引
		Free(p);
		return size;
	}
	int Infect(int id)
	{
		tree_node* p = table->GetData(id);
		tree_node* root = table->GetData(10000);
		int increase = 0;
		if (root->fileNum != 0)
		{
			increase = root->currentSize / root->fileNum;
		}
		if (p != 0)
		{
			m_Infect(p, increase);
		}
		UpdateParentInfo(p->parent, 0, p->fileNum*increase, 0);
		return p->currentSize;
	}
	int Recover(int id)
	{
		tree_node* p = table->GetData(id);
		int decrease = 0;
		int size = 0;
		if (p != 0)
		{
			decrease = p->currentSize - p->orgSize;
			m_Recover(p);
			size = p->orgSize;
		}
		UpdateParentInfo(p->parent, 0,-decrease, 0);
		return size;
	}
	void m_Recover(tree_node* p)
	{
		if (p != 0)
		{
			p->currentSize = p->orgSize;
			if (p->childList != 0)
			{
				for (int i = 0; i < p->childList->size(); i++)
				{
					m_Recover(p->childList->at(i));
				}
			}
		}
	}
	void m_Infect(tree_node* p, int increaseSize)
	{
		if (p != 0)
		{
			p->currentSize += (increaseSize * p->fileNum);
		}

		if (p != 0 && p->isDir == true && p->childList != 0)
		{
			for (int i = 0; i < p->childList->size(); i++)
			{
				tree_node* tmp = p->childList->at(i);
				if (tmp != 0)
				{
					m_Infect(tmp, increaseSize);
				}
			}
		}
	}
	void ClearTableData(tree_node* p)
	{
		if (p->id != 10000)
		{
			table->RemoveData(p->id);
		}
		if (p->childList != 0)
		{
			for (int i = 0; i < p->childList->size(); i++)
			{
				ClearTableData(p->childList->at(i));
			}
		}
	}
	void UpdateParentInfo(tree_node* p, int OrgFileSize, int currentFileSize, int fileNum)
	{
		while (p != 0 && p->isDir)
		{
			p->currentSize += currentFileSize;
			p->orgSize += OrgFileSize;
			p->fileNum += fileNum;
			p = p->parent;
		}
	}
	void Free(tree_node* p)
	{
		if (p != 0 && p->isDir == true && p->childList != 0)
		{
			int count = p->childList->size();
			for (int i = 0; i < count; i++)
			{
				tree_node* tmp = p->childList->at(i);
				if (tmp != 0)
				{
					Free(tmp);
				}
			}
		}

		if (p != 0 && p->id != 10000)
		{
			delete p;
		}

	};

public:
	HashTable* table;
};

Tree* tree = 0;

void init(void)
{
	if (tree != 0)
	{
		delete tree;
	}
	tree = new Tree;
}

int add(int id, int pid, int fileSzie)
{
	return tree->Add(id, pid, fileSzie);
}

int move(int id, int pid)
{
	return tree->Move(id, pid);
}

int remove(int id)
{
	return tree->Remove(id);
}

int infect(int id)
{
	return tree->Infect(id);
}

int recover(int id)
{
	return tree->Recover(id);
}
