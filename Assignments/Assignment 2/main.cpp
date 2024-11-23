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

    BST()
    {
        root = NULL;
    }

    BSTNode *insertData(int id, string name, int age, BSTNode *node)
    {

        if (node == NULL)
        {
            return new BSTNode(id, name, age);
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

    void inorderTraversal(BSTNode *node)
    {
        if (node)
        {
            inorderTraversal(node->left);
            node->data->print();
            inorderTraversal(node->right);
        }
    }

    void inorderTraversal()
    {
        inorderTraversal(root);
    }
};

class AVLNode
{
public:
    TableData *data;
    int height;
    AVLNode *left, *right;

    AVLNode(int id, string name, int age)
        : height(1),
          left(NULL),
          right(NULL)
    {
        data = new TableData(id, name, age);
    }
};

int getHeight(AVLNode *node)
{
    if (!node)
    {
        return 0;
    }
    else
    {
        return node->height;
    }
}

int getBalance(AVLNode *node)
{
    if (!node)
    {
        return 0;
    }
    else
    {
        return getHeight(node->left) - getHeight(node->right);
    }
}

class AVLTree
{
public:
    AVLNode *root;

    AVLTree() : root(NULL) {}

    AVLNode *leftRotate(AVLNode *node)
    {
        AVLNode *x = node->right;
        AVLNode *y = x->left;

        x->left = node;
        node->right = y;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));

        return x;
    }

    AVLNode *rightRotate(AVLNode *node)
    {
        AVLNode *x = node->left;
        AVLNode *y = x->right;

        x->right = node;
        node->left = y;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));

        return x;
    }

    AVLNode *insertData(int id, string name, int age, AVLNode *node)
    {
        if (!node)
        {
            return new AVLNode(id, name, age);
        }

        if (id > node->data->id)
        {
            node->right = insertData(id, name, age, node->right);
        }
        else if (id < node->data->id)
        {
            node->left = insertData(id, name, age, node->left);
        }
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getHeight(node->left) - getHeight(node->right);

        // Rotations
        if (balance > 1 && id < node->left->data->id)
        {
            return rightRotate(node);
        }

        if (balance > 1 && id > node->left->data->id)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && id < node->right->data->id)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        if (balance < -1 && id > node->right->data->id)
        {
            return leftRotate(node);
        }

        return node;
    }

    AVLNode *getLeftMost(AVLNode *node)
    {
        if (!node)
        {
            return NULL;
        }
        else if (!node->left)
        {
            return node;
        }
        else
        {
            return getLeftMost(node->left);
        }
    }

    void insertData(int id, string name, int age)
    {
        root = insertData(id, name, age, root);
    }

    AVLNode *deleteData(int id, AVLNode *node)
    {
        if (!node)
        {
            return NULL;
        }

        else if (id < node->data->id)
        {
            node->left = deleteData(id, node->left);
        }
        else if (id > node->data->id)
        {
            node->right = deleteData(id, node->right);
        }
        else
        {
            if (!node->right && !node->left)
            {
                delete node;
                return NULL;
            }
            else if (!node->right)
            {
                AVLNode *temp = node->left;
                delete node;
                node = temp;
            }
            else if (!node->left)
            {
                AVLNode *temp = node->right;
                delete node;
                node = temp;
            }
            else
            {
                // Get min successor
                AVLNode *minSuccessor = getLeftMost(node->right);

                node->data = minSuccessor->data;

                node->right = deleteData(node->data->id, node->right);
            }
        }

        int balance = getHeight(node->left) - getHeight(node->right);

        // Rotations
        if (balance > 1 && id > node->left->data->id)
        {
            return rightRotate(node);
        }

        if (balance > 1 && id < node->left->data->id)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && id > node->right->data->id)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        if (balance < -1 && id < node->right->data->id)
        {
            return leftRotate(node);
        }

        return node;
    }

    void deleteData(int data)
    {
        root = deleteData(data, root);
    }

    AVLNode *searchData(int id, AVLNode *root)
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

    AVLNode *searchData(int id)
    {
        return searchData(id, root);
    }

    void inorderTraversal(AVLNode *node)
    {
        if (node)
        {
            inorderTraversal(node->left);
            node->data->print();
            inorderTraversal(node->right);
        }
    }

    void inorderTraversal()
    {
        inorderTraversal(root);
    }
};

int main()
{
    AVLTree t;

    t.insertData(1, "", 1);
    t.insertData(3, "", 1);
    t.insertData(2, "", 1);
    t.insertData(5, "", 1);
    t.insertData(8, "", 1);
    t.insertData(81, "", 1);
    t.insertData(4, "", 1);
    t.insertData(9, "", 1);

    AVLNode *searchResult = t.searchData(9);

    if (searchResult)
    {
        searchResult->data->print();
    }

    cout << endl;
    t.inorderTraversal();
}