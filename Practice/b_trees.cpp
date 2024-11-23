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

class BTreeNode
{
public:
    int minChildren;
    TableData **keys;
    BTreeNode **children;
    int n;
    bool isLeaf;

    int findKey(int id)
    {
        int index = 0;
        while (index < n && keys[index]->id < id)
            ++index;
        return index;
    }

    void deleteData(int id)
    {
        int index = findKey(id);
        if (index < n && keys[index]->id == id)
        {
            if (isLeaf)
                leafRemove(index);
            else
                nonLeafRemove(index);
        }
        else
        {
            if (isLeaf)
            {
                cout << "The key " << id << " does not exist in the tree\n";
                return;
            }
            bool flag = ((index == n) ? true : false);
            if (children[index]->n < minChildren)
                fill(index);
            if (flag && index > n)
                children[index - 1]->deleteData(id);
            else
                children[index]->deleteData(id);
        }
        return;
    }

    void leafRemove(int index)
    {
        for (int i = index + 1; i < n; ++i)
            keys[i - 1] = keys[i];
        n--;
        return;
    }

    void nonLeafRemove(int index)
    {
        int id = keys[index]->id;
        if (children[index]->n >= minChildren)
        {
            TableData *predecessor = getPredecessor(index);
            keys[index] = predecessor;
            children[index]->deleteData(predecessor->id);
        }
        else if (children[index + 1]->n >= minChildren)
        {
            TableData *successor = getSuccessor(index);
            keys[index] = successor;
            children[index + 1]->deleteData(successor->id);
        }
        else
        {
            merge(index);
            children[index]->deleteData(id);
        }
        return;
    }

    TableData *getPredecessor(int index)
    {
        BTreeNode *cur = children[index];
        while (!cur->isLeaf)
            cur = cur->children[cur->n];
        return cur->keys[cur->n - 1];
    }

    TableData *getSuccessor(int index)
    {
        BTreeNode *cur = children[index + 1];
        while (!cur->isLeaf)
            cur = cur->children[0];
        return cur->keys[0];
    }

    void fill(int index)
    {
        if (index != 0 && children[index - 1]->n >= minChildren)
            borrowFromPrevious(index);
        else if (index != n && children[index + 1]->n >= minChildren)
            borrowFromNext(index);
        else
        {
            if (index != n)
                merge(index);
            else
                merge(index - 1);
        }
        return;
    }

    void borrowFromPrevious(int index)
    {
        BTreeNode *child = children[index];
        BTreeNode *sibling = children[index - 1];
        for (int i = child->n - 1; i >= 0; --i)
            child->keys[i + 1] = child->keys[i];
        if (!child->isLeaf)
        {
            for (int i = child->n; i >= 0; --i)
                child->children[i + 1] = child->children[i];
        }
        child->keys[0] = keys[index - 1];
        if (!child->isLeaf)
            child->children[0] = sibling->children[sibling->n];
        keys[index - 1] = sibling->keys[sibling->n - 1];
        child->n += 1;
        sibling->n -= 1;
        return;
    }

    void borrowFromNext(int index)
    {
        BTreeNode *child = children[index];
        BTreeNode *sibling = children[index + 1];
        child->keys[(child->n)] = keys[index];
        if (!(child->isLeaf))
            child->children[(child->n) + 1] = sibling->children[0];
        keys[index] = sibling->keys[0];
        for (int i = 1; i < sibling->n; ++i)
            sibling->keys[i - 1] = sibling->keys[i];
        if (!sibling->isLeaf)
        {
            for (int i = 1; i <= sibling->n; ++i)
                sibling->children[i - 1] = sibling->children[i];
        }
        child->n += 1;
        sibling->n -= 1;
        return;
    }

    void merge(int index)
    {
        BTreeNode *child = children[index];
        BTreeNode *sibling = children[index + 1];
        child->keys[minChildren - 1] = keys[index];
        for (int i = 0; i < sibling->n; ++i)
            child->keys[i + minChildren] = sibling->keys[i];
        if (!child->isLeaf)
        {
            for (int i = 0; i <= sibling->n; ++i)
                child->children[i + minChildren] = sibling->children[i];
        }
        for (int i = index + 1; i < n; ++i)
            keys[i - 1] = keys[i];
        for (int i = index + 2; i <= n; ++i)
            children[i - 1] = children[i];
        child->n += sibling->n + 1;
        n--;
        delete (sibling);
        return;
    }

    void insertNonFull(int id, string name, int age)
    {
        int i = n - 1;
        if (isLeaf == true)
        {
            while (i >= 0 && keys[i]->id > id)
            {
                keys[i + 1] = keys[i];
                i--;
            }
            keys[i + 1] = new TableData(id, name, age);
            n = n + 1;
        }
        else
        {
            while (i >= 0 && keys[i]->id > id)
                i--;
            if (children[i + 1]->n == 2 * minChildren - 1)
            {
                splitChild(i + 1, children[i + 1]);
                if (keys[i + 1]->id < id)
                    i++;
            }
            children[i + 1]->insertNonFull(id, name, age);
        }
    }

    void splitChild(int i, BTreeNode *y)
    {
        BTreeNode *z = new BTreeNode(y->minChildren, y->isLeaf);
        z->n = minChildren - 1;
        for (int j = 0; j < minChildren - 1; j++)
            z->keys[j] = y->keys[j + minChildren];
        if (y->isLeaf == false)
        {
            for (int j = 0; j < minChildren; j++)
                z->children[j] = y->children[j + minChildren];
        }
        y->n = minChildren - 1;
        for (int j = n; j >= i + 1; j--)
            children[j + 1] = children[j];
        children[i + 1] = z;
        for (int j = n - 1; j >= i; j--)
            keys[j + 1] = keys[j];
        keys[i] = y->keys[minChildren - 1];
        n = n + 1;
    }

    BTreeNode(int t1, bool leaf1)
    {
        minChildren = t1;
        isLeaf = leaf1;
        n = 0;
        keys = new TableData *[2 * minChildren - 1];
        children = new BTreeNode *[2 * minChildren];
    }

    void traverse()
    {
        int i;
        for (i = 0; i < n; i++)
        {
            if (isLeaf == false)
                children[i]->traverse();
            keys[i]->print();
        }
        if (isLeaf == false)
            children[i]->traverse();
    }

    BTreeNode *searchData(int id)
    {
        int i = 0;

        while (i < n && id > keys[i]->id)
            i++;

        if (keys[i] && keys[i]->id == id)
        {
            return this;
        }

        if (isLeaf == true)
        {
            return NULL;
        }
        return children[i]->searchData(id);
    }
};

class BTree
{
public:
    BTreeNode *root;
    int minChildren;

    BTree(int _t)
    {
        root = NULL;
        minChildren = _t;
    }

    void traverse()
    {
        if (root != NULL)
            root->traverse();
    }

    BTreeNode *searchData(int id)
    {
        if (!root)
        {
            return root;
        }
        else
        {
            return root->searchData(id);
        }
    }

    void insertData(int id, string name, int age)
    {
        if (root == NULL)
        {
            root = new BTreeNode(minChildren, true);
            root->keys[0] = new TableData(id, name, age);
            root->n = 1;
        }
        else
        {
            if (root->n == 2 * minChildren - 1)
            {
                BTreeNode *s = new BTreeNode(minChildren, false);
                s->children[0] = root;
                s->splitChild(0, root);
                int i = 0;
                if (s->keys[0]->id < id)
                    i++;
                s->children[i]->insertNonFull(id, name, age);
                root = s;
            }
            else
                root->insertNonFull(id, name, age);
        }
    }
    void deleteData(int id)
    {
        if (!root)
        {
            cout << "The tree is empty\n";
            return;
        }
        root->deleteData(id);
        if (root->n == 0)
        {
            BTreeNode *tmp = root;
            if (root->isLeaf)
                root = NULL;
            else
                root = root->children[0];
            delete tmp;
        }
        return;
    }
};

int main()
{
    BTree minChildren(5);
    minChildren.insertData(1, "", 1);
    minChildren.insertData(3, "", 1);
    minChildren.insertData(7, "", 1);
    minChildren.insertData(12, "", 1);
    minChildren.insertData(11, "", 1);
    minChildren.insertData(13, "", 1);

    cout << "Traversing B-Tree:" << endl;
    minChildren.traverse();
    int id = 10;
    (minChildren.searchData(id) != NULL) ? cout << endl
                                                << id << " is found"
                                         : cout << endl
                                                << id << " is not Found";

    id = 12;
    (minChildren.searchData(id) != NULL) ? cout << endl
                                                << id << " is found"
                                         : cout << endl
                                                << id << " is not Found\n";

    minChildren.deleteData(10);
    minChildren.insertData(10, "", 1);
    minChildren.deleteData(7);
    minChildren.insertData(7, "", 1);
    minChildren.deleteData(5);
    minChildren.insertData(5, "", 1);
    minChildren.deleteData(3);
    minChildren.insertData(3, "", 1);
    cout << "\nThe B-tree after deleteData of 20: ";
    minChildren.traverse();
}