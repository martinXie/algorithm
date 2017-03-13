
struct node{
    int key;
    void* data;
    node* parent;
    node* left;
    node* right;
    node(int nkey, void* pdata):key(nkey),data(pdata),parent(nullptr_t), left(nullptr_t),right(nullptr_t){};
};

class BinaryTree
{
public:
    BinaryTree():{};
    ~BinaryTree(){remove(root);};
    bool add(int key, void*data)
    {
        if(get(key) == nullptr_t)
        {
            if(root)
        }
    }
    bool remove(node* pnode)
    {

    };
    node* get(int key)
    {
        if(root == nullptr_t)
        {
            return nullptr_t; 
        }
        else
        {
            return find(root, key);
        }
    };
    node* find(node*pnode, int key)
    {
        if(pnode != nullptr_t && pnode->key==key)
        {
            return pnode;
        }
        else if(pnode->left != nullptr_t)
        {
            find()
        }
    };
private:
    node* root=nullptr_t;
    int number=0;
}

int main()
{
    cout << "hello" << endl;
    return 1;
}