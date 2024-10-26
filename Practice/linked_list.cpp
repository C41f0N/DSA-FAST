#include <iostream>

using namespace std;

class Node
{
public:
    int data;
    Node *next;

    Node(int data)
    {
        this->data = data;
        this->next = nullptr;
    }
};

class LinkedList
{
    Node *head;

public:
    LinkedList()
    {
        this->head = nullptr;
    }

    void traverse()
    {
        if (head == nullptr)
        {

            cout << "[-] List is empty, cant traverse." << endl;
        }
        else
        {
            Node *currentNode = head;
            while (currentNode != nullptr)
            {
                cout << currentNode->data << " -> ";
                currentNode = currentNode->next;
            }
            cout << "NULL" << endl;
        }
    }

    void insertAtEnd(int value)
    {
        if (head == nullptr)
        {
            head = new Node(value);
        }
        else
        {
            Node *currentNode = head;

            while (currentNode->next != nullptr)
            {
                currentNode = currentNode->next;
            }

            currentNode->next = new Node(value);
        }
    }

    void insertAtStart(int value)
    {
        Node *newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }

    void insertInBetween(int value, int index)
    {
        if (index == 0)
        {
            insertAtStart(value);
        }
        else
        {
            Node *newNode = new Node(value);

            int currentIndex = 0;
            Node *currentNode = head;
            while (currentNode->next != nullptr && currentIndex < index - 1)
            {
                currentNode = currentNode->next;
                currentIndex++;
            }

            newNode->next = currentNode->next;
            currentNode->next = newNode;
        }
    }

    void deletionFromStart()
    {
        if (head == nullptr)
        {
            cout << "[-] List is empty" << endl;
        }
        else
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }

    void deletionFromEnd()
    {
        if (head == nullptr)
        {
            cout << "[-] List is empty, cant remove from tail." << endl;
        }
        else if (head->next == nullptr)
        {
            delete head;
            head = nullptr;
        }
        else
        {
            Node *currentPointer = head;

            while (currentPointer->next->next != nullptr)
            {
                currentPointer = currentPointer->next;
            }

            Node *temp = currentPointer->next;
            currentPointer->next = nullptr;
            delete temp;
        }
    }

    void deletionFromMiddle(int index)
    {
        if (head == nullptr)
        {
            cout << "[-] List is empty, cant remove from middle." << endl;
        }
        else
        {
            if (index == 0)
            {
                deletionFromStart();
            }
            else
            {
                Node *currentNode = head;
                int currentIndex = 0;

                while (currentNode->next != nullptr && currentIndex < index - 1)
                {
                    currentNode = currentNode->next;
                    currentIndex++;
                }

                if (currentNode->next == nullptr)
                {
                    cout << "[-] Position out of bounds, cant delete at " << index << "." << endl;
                }
                else
                {
                    Node *temp = currentNode->next;
                    currentNode->next = currentNode->next->next;
                    delete temp;
                }
            }
        }
    }
};

int main()
{
    LinkedList list;

    list.traverse();

    list.insertAtEnd(0);
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);
    list.insertAtEnd(4);
    list.insertAtEnd(5);

    list.traverse();
    list.deletionFromMiddle(10);
    list.traverse();
}
