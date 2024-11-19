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
    t.insertData(2, "", 2);
    t.insertData(3, "", 3);
    t.insertData(4, "", 4);
    t.insertData(5, "", 5);
    t.insertData(6, "", 6);
    t.insertData(7, "", 7);
    t.insertData(8, "", 8);

    // t.deleteData(1);
    // t.deleteData(2);
    // t.deleteData(3);
    // t.deleteData(4);

    t.inorderTraversal();

    cout << "balance: ";
    cout << getBalance(t.root) << endl;
}