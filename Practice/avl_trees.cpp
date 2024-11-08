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

    Node *getMinSuccessor(Node *node)
    {
        // if (node)

        return temp;
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
            deleteKey(key, node->left);
        }
        else if (key > node->key)
        {
            deleteKey(key, node->right);
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
                Node *minSuccessor = getMinSuccessor(node);

                node->key = minSuccessor->key;

                delete minSuccessor;
                return node;
            }
        }
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

    cout << t.root->left->height - t.root->right->height << endl;
}