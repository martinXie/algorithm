#define KEY 9973;
#define MAX 10000

struct tree_node
{
	int id;
	bool isDir;
	int fileNum;
	int orgSize; // only work for file
	int currentSize;
	tree_node* parent;
	List* childList;
	tree_node(int nId, int fileSize) :id(nId),isDir(!fileSize), fileNum(0), orgSize(fileSize), currentSize(fileSize), parent(0), childList(0) {};
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
	~List(void) { delete head; head = tail = 0; };
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
			if (current == tail)
			{
				delete tail;
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
		if (n < count)
		{
			list_node* p = head + (n+1);
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
	void SaveData(tree_node*p)
	{
		int index = hash(p->id);
		List* list = db[index];
		if (list == 0)
		{
			list = new List;
		}
		list->add(p);
	}
	tree_node* GetData(int id)
	{
		int index = hash(id);
		List* p = db[index];
		if (p == 0)
		{
			return 0;
		}
		return p->get(id);

	};
	void RemoveData(int id)
	{
		int index = hash(id);
		List* p = db[index];
		if (p == 0)
		{
			return;
		}
		p->remove(p->get(id));
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
	Tree() {};
	~Tree() {};
	int Add(int id, int pid, int fileSize)
	{
		tree_node* p = new tree_node(id, fileSize);
		tree_node* parent = table->GetData(pid);
		if (parent != 0)
		{
			if (parent->childList == 0)
			{
				parent->childList == new List;
			}
			parent->childList->add(p);
			if (fileSize != 0)
			{
				UpdateParentSize(parent, fileSize, 1);
			}
		}
		table->SaveData(p);
		return parent->currentSize;
	};
	int Move(int id, int pid)
	{
		tree_node* p = table->GetData(id);
		int size = p->currentSize;
		if (p->parent != 0 && p->parent->childList != 0)
		{
			p->parent->childList->remove(p);
			UpdateParentSize(p->parent, -size, -(p->fileNum));
		}
		tree_node* newParent = table->GetData(pid);
		if (newParent != 0)
		{
			if (newParent->childList == 0)
			{
				newParent->childList = new List;
			}
			newParent->childList->add(p);
			UpdateParentSize(newParent, size, p->fileNum);
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
			UpdateParentSize(p->parent, -size, -(p->fileNum));
		}
		ClearTableData(p); //删除数据索引
		Free(p);// 删除数据
		return size;
	}
	int Infect(int id)
	{

	}
	void Infect(tree_node* p)
	{
		while (p != 0)
		{

		}
	}
	void ClearTableData(tree_node* p)
	{
		while (p != 0)
		{
			table->RemoveData(p->id);
			if (p->childList != 0)
			{
				for (int i = 0; i < p->childList->size(); i++)
				{
					ClearTableData(p->childList->at(i));
				}
			}
		}
	}
	void UpdateParentSize(tree_node* p, int fileSize, int fileNum)
	{
		while (p != 0 && p->isDir )
		{
			p->currentSize += fileSize;
			p->fileNum += fileNum;
			p = p->parent;
		}
	}
	void Free(tree_node* p)
	{
		while (p != 0 && p->childList != 0)
		{
			int count = p->childList->size();
			for (int i = 0; i < count; i++)
			{
				if (p->childList->at(i) != 0)
				{
					Free(p->childList->at(i));
				}
			}
		}

		if (p != 0)
		{
			delete p;
		}
		
	};
	
public:
	tree_node* root;
	HashTable* table;
};
Tree* tree = 0;

void inital (void)
{
	if (tree != 0)
	{
		delete tree;
	}
	tree = new Tree;
	tree->Add(10000, 0, 0);
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

}

int recover(int id)
{

}
