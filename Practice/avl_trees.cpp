#include <iostream>

using namespace std;

class Node
{
public:
    int key, height;
    Node *left, *right;

    Node(int key)
        : key(key),
          height(1),
          left(NULL),
          right(NULL) {}
};

int getHeight(Node *node)
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

int getBalance(Node *node)
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
    Node *root;

    AVLTree() : root(NULL) {}

    Node *leftRotate(Node *node)
    {
        Node *x = node->right;
        Node *y = x->left;

        x->left = node;
        node->right = y;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));

        return x;
    }

    Node *rightRotate(Node *node)
    {
        Node *x = node->left;
        Node *y = x->right;

        x->right = node;
        node->left = y;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        x->height = 1 + max(getHeight(x->left), getHeight(x->right));

        return x;
    }

    Node *insertKey(int key, Node *node)
    {
        if (!node)
        {
            return new Node(key);
        }

        if (key > node->key)
        {
            node->right = insertKey(key, node->right);
        }
        else if (key < node->key)
        {
            node->left = insertKey(key, node->left);
        }
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getHeight(node->left) - getHeight(node->right);

        // Rotations
        if (balance > 1 && key < node->left->key)
        {
            return rightRotate(node);
        }

        if (balance > 1 && key > node->left->key)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        if (balance < -1 && key < node->right->key)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        if (balance < -1 && key > node->right->key)
        {
            return leftRotate(node);
        }

        return node;
    }

    Node *getLeftMost(Node *node)
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

    void insertKey(int key)
    {
        root = insertKey(key, root);
    }

    Node *deleteKey(int key, Node *node)
    {
        if (!node)
        {
            return NULL;
        }

        else if (key < node->key)
        {
            node->left = deleteKey(key, node->left);
        }
        else if (key > node->key)
        {
            node->right = deleteKey(key, node->right);
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
                Node *temp = node->left;
                delete node;
                return temp;
            }
            else if (!node->left)
            {
                Node *temp = node->right;
                delete node;
                return temp;
            }
            else
            {
                // Get min successor
                Node *minSuccessor = getLeftMost(node->right);

                node->key = minSuccessor->key;

                node->right = deleteKey(node->key, node->right);
            }
        }
        return node;
    }

    void deleteKey(int key)
    {
        root = deleteKey(key, root);
    }

    void inorderTraversal(Node *node)
    {
        if (node)
        {
            inorderTraversal(node->left);
            cout << node->key << endl;
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

    t.insertKey(1);
    t.insertKey(2);
    t.insertKey(3);
    t.insertKey(4);
    t.insertKey(5);
    t.insertKey(6);
    t.insertKey(7);
    t.insertKey(8);

    t.inorderTraversal();

    cout << "balance: ";
    cout << getBalance(t.root) << endl;
}