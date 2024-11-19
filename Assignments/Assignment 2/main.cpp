#include <iostream>

using namespace std;

class TableData
{
public:
    int id;
    string name;
    int age;

    TableData(int id, string name, int age) : id(id), name(name), age(age) {}

    void print()
    {
        cout << "id: " << id << " name: " << name << " age: " << age << endl;
    }
};

class BSTNode
{
public:
    TableData *data;
    BSTNode *left, *right;

    BSTNode(int id, string name, int age)
    {
        data = new TableData(id, name, age);
        left = NULL;
        right = NULL;
    }
};

class BST
{
public:
    BSTNode *root;

    BSTNode *insertData(int id, string name, int age, BSTNode *root)
    {
        BSTNode *node = new BSTNode(id, name, age);

        if (root == NULL)
        {
            root = node;
        }
        else
        {
            if (id > root->data->id)
            {
                root->right = insertData(id, name, age, root->right);
            }
            else
            {
                root->left = insertData(id, name, age, root->left);
            }
        }

        return root;
    }

    void insertData(int id, string name, int age)
    {
        root = insertData(id, name, age, root);
    }

    BSTNode *getLeftMost(BSTNode *node)
    {
        if (!node)
        {
            return NULL;
        }
        if (!node->left)
        {
            return node;
        }
        else
        {
            return getLeftMost(node->left);
        }
    }

    BSTNode *deleteData(int id, BSTNode *root)
    {
        if (root == NULL)
        {
            return root;
        }
        else
        {
            if (id == root->data->id)
            {
                // If leaf
                if (!root->left && !root->right)
                {
                    delete root;
                    return NULL;
                }
                // If only right child
                else if (!root->left && root->right)
                {
                    BSTNode *rootRight = root->right;
                    delete root;
                    return rootRight;
                }
                // If only left child
                else if (!root->right && root->left)
                {
                    BSTNode *rootLeft = root->left;
                    delete root;
                    return rootLeft;
                }
                // If has two children
                else
                {
                    // Get min successor
                    BSTNode *temp = getLeftMost(root->right);

                    // Replace data with min successor
                    root->data = temp->data;

                    // Delete the min successor
                    root->right = deleteData(id, root->right);
                }
            }
            else if (id > root->data->id)
            {
                root->right = deleteData(id, root->right);
            }
            else
            {
                root->left = deleteData(id, root->left);
            }
            return root;
        }
    }

    void deleteData(int id)
    {
        root = deleteData(id, root);
    }

    BSTNode *searchData(int id, BSTNode *root)
    {
        if (root != NULL)
        {
            if (root->data->id == id)
            {
                return root;
            }
            else if (id > root->data->id)
            {
                return searchData(id, root->right);
            }
            else
            {
                return searchData(id, root->left);
            }
        }
        else
        {
            return NULL;
        }
    }

    BSTNode *searchData(int id)
    {
        return searchData(id, root);
    }

    void inOrderTraversal(BSTNode *node)
    {
        if (node)
        {
            inOrderTraversal(node->left);
            node->data->print();
            inOrderTraversal(node->right);
        }
    }

    void inOrderTraversal()
    {
        inOrderTraversal(root);
    }
};

int main()
{
    BST t;

    t.insertData(1, "", 1);
    t.insertData(3, "", 1);
    t.insertData(2, "", 1);
    t.insertData(5, "", 1);
    t.insertData(8, "", 1);
    t.insertData(81, "", 1);
    t.insertData(4, "", 1);
    t.insertData(9, "", 1);

    BSTNode *searchResult = t.searchData(2);

    if (searchResult)
    {
        searchResult->data->print();
    }

    cout << endl;
    t.inOrderTraversal();
}