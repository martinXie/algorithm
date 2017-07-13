#define KEY 9973
#define MAX 10000
#define MAX_CONFLICT 100
struct tree_node
{
	int id;
	int fileNum;
	int orgSize;
	int currentSize;
	int parentId;
	int childId[MAX];
	int childNum;
	tree_node() :id(0), fileNum(0), orgSize(0), currentSize(0), parentId(0), childNum(0) {};
};
tree_node db[MAX];
int dbNum;

struct Record
{
	int nodeId;
	int dataIndex;
	Record() { nodeId = -1; dataIndex = -1; }
};
struct hash_node
{
	Record record[MAX];
	int count;
	hash_node() { count = 0; }
};

hash_node hashTable[KEY];


int hash(int n)
{
	return n%KEY;
}

void removeHashIndex(int NodeId)
{
	int hashKey = hash(NodeId);
	int count = hashTable[hashKey].count;
	int position = -1;
	for (int i = 0; i < count; i++)
	{
		if (hashTable[hashKey].record[i].nodeId == NodeId)
		{
			position = i;
			break;
		}
	}

	if (position != -1)
	{
		int dataIndex = hashTable[hashKey].record[position].dataIndex;
		for (int i = 0; i < db[dataIndex].childNum; i++)
		{
			removeHashIndex(db[dataIndex].childId[i]);
		}

		hashTable[hashKey].record[position].nodeId = hashTable[hashKey].record[count - 1].nodeId;
		hashTable[hashKey].record[position].dataIndex = hashTable[hashKey].record[count - 1].dataIndex;
		hashTable[hashKey].count--;
	}
}
int getDataIndex(int NodeId)
{
	int dataIndex = -1;
	int hashKey = hash(NodeId);
	for (int i = 0; i < hashTable[hashKey].count; i++)
	{
		if (hashTable[hashKey].record[i].nodeId == NodeId)
		{
			dataIndex = hashTable[hashKey].record[i].dataIndex;
			break;
		}
	}
	return dataIndex;
}
void updateParentSize(int pid, int currentSize, int orgSize, int fileNum)
{
	int pidDataIndex = getDataIndex(pid);
	if ( pidDataIndex == -1 )
	{
		return;
	}
	db[pidDataIndex].childNum++;
	db[pidDataIndex].currentSize += currentSize;
	db[pidDataIndex].orgSize += orgSize;
	db[pidDataIndex].fileNum += fileNum;
	updateParentSize(db[pidDataIndex].parentId, currentSize, orgSize, fileNum);
}
void updateChildSize(int pid, int increase)
{
	int dataIndex = getDataIndex(pid);
	if (dataIndex != -1)
	{
		for (int i = 0; i < db[dataIndex].childNum; i++)
		{
			int pid = db[dataIndex].childId[i];
			updateChildSize(pid, increase);
		}
		db[dataIndex].currentSize += db[dataIndex].fileNum * increase;
	}

}
void RecoverChildSize(int id)
{
	int dataIndex = getDataIndex(id);
	if (dataIndex != -1)
	{
		
		for (int i = 0; i < db[dataIndex].childNum; i++)
		{
			int childId = db[dataIndex].childId[i];
			RecoverChildSize(childId);
		}
		db[dataIndex].currentSize = db[dataIndex].orgSize;
	}

}
void initial (void)
{
	dbNum = 0;
	for (int i = 0; i < KEY; i++)
	{
		hashTable[i].count = 0;
	}
	add(10000, 0, 0);
}

int add(int id, int pid, int fileSzie)
{
	// add hash index
	int hashKey = hash(id);
	int n = hashTable[hashKey].count;
	hashTable[hashKey].record[n].dataIndex = dbNum;
	hashTable[hashKey].record[n].nodeId = id;
	//save data
	db[dbNum].id = id;
	db[dbNum].parentId = pid;
	db[dbNum].orgSize = fileSzie;
	db[dbNum].currentSize = fileSzie;
	db[dbNum].childNum = 0;
	db[dbNum].fileNum = fileSzie == 0 ? 0 : 1;
	dbNum++;
	// update parent info
	int dataIndex = getDataIndex(pid);
	if (dataIndex != -1)
	{
		int count = db[dataIndex].childNum;
		db[dataIndex].childId[count] = id;
		db[dataIndex].childNum++;
		updateParentSize(pid, fileSzie, fileSzie, fileSzie == 0 ? 0 : 1);
	}
}

int move(int id, int pid)
{
	int dataIndex = getDataIndex(pid);
	if (dataIndex != -1)
	{	
		int oldParentId = db[dataIndex].parentId;
		// update old parent info
		int oldParentDataIndex = getDataIndex(oldParentId);
		if (oldParentDataIndex != -1)
		{
			updateParentSize(oldParentId, -(db[dataIndex].currentSize), -(db[dataIndex].orgSize), -(db[dataIndex].fileNum));
			int count = db[oldParentDataIndex].childNum;
			for (int i = 0; i < count; i++)
			{
				if (db[oldParentDataIndex].childId[i] == id)
				{
					db[oldParentDataIndex].childId[i] = db[oldParentDataIndex].childId[count - 1];
					db[oldParentDataIndex].childNum--;
					break;
				}
			}
			
		}
		// update self info
		db[dataIndex].parentId = pid;

		// update new parent info
		int newParentDataIndex = getDataIndex(pid);
		if (newParentDataIndex != -1)
		{
			updateParentSize(pid, db[dataIndex].currentSize, db[dataIndex].orgSize, db[dataIndex].fileNum);
			int count = db[newParentDataIndex].childNum;
			db[newParentDataIndex].childId[count] = id;
			db[newParentDataIndex].childNum++;
		}
	}
}

int remove(int id)
{

	int dataIndex = getDataIndex(id);
	if (dataIndex != -1)
	{
		if (id == 10000)
		{
			// reset data 
			initial();
		}
		else
		{
			// delete hash index; recursive
			removeHashIndex(id);
			updateParentSize(db[dataIndex].parentId, -(db[dataIndex].currentSize), -(db[dataIndex].orgSize), -(db[dataIndex].fileNum));
		}
	}
}

int infect(int id)
{
	int dataIndex = getDataIndex(id);
	if (dataIndex != -1)
	{
		int increase = db[getDataIndex(10000)].fileNum == 0 ? 0 : db[getDataIndex(10000)].currentSize / db[getDataIndex(10000)].fileNum;
		if (increase != 0)
		{
			updateChildSize(id, increase);
			updateParentSize(db[dataIndex].parentId, db[dataIndex].childNum*increase, 0, 0);
		}		
	}
}

int recover(int id)
{
	int dataIndex = getDataIndex(id);
	if (dataIndex != -1)
	{
		int size = db[dataIndex].currentSize - db[dataIndex].orgSize;
		if (size != 0)
		{
			updateParentSize(db[dataIndex].parentId, -size, 0, 0);
			RecoverChildSize(id);
		}
	}
}
