#include <iostream>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

class Ticket
{
public:
    int id;
    string customerName;
    int priority;
    string requestDescription;
    chrono::_V2::system_clock::time_point creationTime;
    bool isOpen;
    chrono::_V2::system_clock::time_point closeTime{};

    Ticket(
        int id,
        string customerName,
        int priority,
        string requestDescription)
        : id(id),
          customerName(customerName),
          priority(priority),
          requestDescription(requestDescription),
          creationTime(chrono::system_clock::now()),
          isOpen(true) {}

    time_t getTime()
    {
        return system_clock::to_time_t(creationTime);
    }

    void printDetails()
    {
        cout << "Ticket ID: " << id << endl;
        cout << "Customer Name: " << customerName << endl;
        cout << "Request Decription: " << requestDescription << endl;
        cout << "Creation Time: ";
        printCreationTime();
        cout << "Is Open: " << isOpen << endl;
        cout << endl;
    }

    void printCreationTime()
    {
        time_t retTime = system_clock::to_time_t(creationTime);
        cout << ctime(&retTime);
    }

    void close()
    {
        isOpen = false;
        closeTime = chrono::system_clock::now();
    }
};

class TicketNode
{
public:
    Ticket ticket;
    TicketNode *next;
    TicketNode *prev;

    TicketNode(Ticket &ticket) : ticket(ticket), next(NULL), prev(NULL) {}
};

class TicketLL
{
public:
    TicketNode *head;
    TicketNode *tail;

    TicketLL() : head(NULL), tail(NULL) {}

    // Traverse
    void traverse()
    {
        TicketNode *temp = head;

        while (temp != NULL)
        {
            temp->ticket.printDetails();
            temp = temp->next;
        }
    }

    // Add at start
    void addTicket(Ticket &ticket)
    {
        TicketNode *newNode = new TicketNode(ticket);

        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
            return;
        }

        newNode->next = head;
        head->prev = newNode;
        head = newNode;

        return;
    }

    void removeTicket(int id)
    {
        TicketNode *temp = head;

        while (temp != NULL)
        {
            if (temp->ticket.id == id)
            {
                cout << "[+] Deleting ticket " << id << endl;

                // Remove ticket
                TicketNode *prevNode = temp->prev;
                TicketNode *nextNode = temp->next;

                // If only node
                if (prevNode == NULL && nextNode == NULL)
                {
                    head = NULL;
                    tail = NULL;
                }
                else

                    // If head
                    if (prevNode == NULL)
                    {
                        nextNode->prev = NULL;
                        head = nextNode;
                    }
                    else
                        // If tail
                        if (nextNode == NULL)
                        {
                            prevNode->next = NULL;
                            tail = prevNode;
                        }
                        else
                            // Neither head nor tail
                            if (nextNode != NULL && prevNode != NULL)
                            {
                                prevNode->next = nextNode;
                                nextNode->prev = prevNode;
                            }

                delete temp;
                break;
            }

            temp = temp->next;
        }
    }

    void bubbleSort(string sortBy)
    {
        if (head == tail || head == NULL)
        {
            return;
        }

        // Calculating length
        int length = 0;
        TicketNode *current = head;

        while (current != NULL)
        {
            length++;
            current = current->next;
        }

        for (int i = 0; i < length; i++)
        {
            current = head->next;
            while (current != NULL)
            {
                // Swap if prev is greater than next
                bool condition = false;
                if (sortBy == "priority")
                {
                    condition = current->prev->ticket.priority > current->ticket.priority;
                }
                else if (sortBy == "creationTime")
                {
                    condition = current->prev->ticket.creationTime > current->ticket.creationTime;
                }
                else if (sortBy == "customerName")
                {
                    condition = current->prev->ticket.customerName > current->ticket.customerName;
                }

                if (condition)
                {
                    Ticket temp(current->prev->ticket);
                    current->prev->ticket = current->ticket;
                    current->ticket = temp;
                }

                current = current->next;
            }
        }
    }
};

int main()
{
    Ticket ticket3(1, "S2arim Ahmed", 1, "A request");
    Ticket ticket2(4, "S3arim Ahmed", 1, "A request");
    Ticket ticket4(8, "S4arim Ahmed", 1, "A request");
    Ticket ticket1(2, "S1arim Ahmed", 1, "A request");

    TicketLL ll;

    ticket4.close();

    ll.addTicket(ticket2);
    ll.addTicket(ticket1);
    ll.addTicket(ticket4);
    ll.addTicket(ticket3);

    ll.bubbleSort("customerName");

    // ll.removeTicket(1);
    // ll.removeTicket(2);
    // ll.removeTicket(4);
    // ll.removeTicket(8);

    ll.traverse();
}