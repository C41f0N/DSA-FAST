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

    Ticket() {}

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

    void swapTicket(Ticket &ticket1, Ticket &ticket2)
    {
        Ticket temp(ticket1);
        ticket1 = ticket2;
        ticket2 = temp;
    }

    Ticket *getTicketById(int id)
    {
        TicketNode *current = head;

        while (current != NULL)
        {
            if (current->ticket.id == id)
            {
                return &current->ticket;
            }

            current = current->next;
        }

        return NULL;
    }

    Ticket *getTicketByCustomerName(string customerName)
    {
        TicketNode *current = head;

        while (current != NULL)
        {
            if (current->ticket.customerName == customerName)
            {
                return &current->ticket;
            }

            current = current->next;
        }

        return NULL;
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

    void insertionSort(string sortBy)
    {

        TicketNode *current = head;

        while (current != NULL)
        {

            Ticket key(current->ticket);

            TicketNode *j = current;
            bool condition = false;

            if (sortBy == "priority" && j->prev != NULL)
            {
                condition = j->prev->ticket.priority > key.priority;
            }
            else if (sortBy == "creationTime" && j->prev != NULL)
            {
                condition = j->prev->ticket.creationTime > key.creationTime;
            }
            else if (sortBy == "customerName" && j->prev != NULL)
            {
                condition = j->prev->ticket.customerName > key.customerName;
            }

            while (j->prev != NULL && condition)
            {

                j->ticket = j->prev->ticket;
                j = j->prev;

                condition = false;

                if (sortBy == "priority" && j->prev != NULL)
                {
                    condition = j->prev->ticket.priority > key.priority;
                }
                else if (sortBy == "creationTime" && j->prev != NULL)
                {
                    condition = j->prev->ticket.creationTime > key.creationTime;
                }
                else if (sortBy == "customerName" && j->prev != NULL)
                {
                    condition = j->prev->ticket.customerName > key.customerName;
                }
            }

            j->ticket = key;

            current = current->next;
        }
    }

    void selectionSort(string sortBy)
    {

        TicketNode *i = head;

        while (i != NULL)
        {
            TicketNode *current = i;
            TicketNode *minimum = i;

            while (current != NULL)
            {
                bool condition = false;

                if (sortBy == "priority")
                {
                    condition = current->ticket.priority > minimum->ticket.priority;
                }
                else if (sortBy == "creationTime")
                {
                    condition = current->ticket.creationTime > minimum->ticket.creationTime;
                }
                else if (sortBy == "customerName")
                {
                    condition = current->ticket.customerName > minimum->ticket.customerName;
                }

                if (condition)
                {
                    minimum = current;
                }

                Ticket temp(minimum->ticket);
                minimum->ticket = current->ticket;
                current->ticket = temp;

                current = current->next;
            }

            i = i->next;
        }
    }

    int getLength()
    {
        int length = 0;
        TicketNode *current = head;

        while (current != NULL)
        {
            length++;
            current = current->next;
        }

        return length;
    }

    Ticket *toArray()
    {
        // Getting length
        int length = getLength();

        // Initializing array
        if (length > 0)
        {
            Ticket *ticketsArray = new Ticket[length];

            TicketNode *current = head;
            int i = 0;

            while (current != NULL)
            {
                ticketsArray[i] = current->ticket;
                current = current->next;

                i++;
            }
            return ticketsArray;
        }

        return NULL;
    }

    void deleteAll()
    {
        TicketNode *current = tail;

        while (current != NULL)
        {
            TicketNode *temp = current;
            current = current->prev;
            delete temp;
        }

        head = tail = NULL;
    }

    void fromArray(Ticket *array, int length)
    {
        deleteAll();

        for (int i = length - 1; i >= 0; i--)
        {
            addTicket(array[i]);
        }
    }

    int partition(Ticket *array, int left, int right, string sortBy)
    {

        Ticket pivot = array[right];
        int i = left - 1;

        for (int j = left; j < right; j++)
        {

            bool condition = false;

            if (sortBy == "priority")
            {
                condition = array[j].priority < pivot.priority;
            }
            else if (sortBy == "creationTime")
            {
                condition = array[j].creationTime < pivot.creationTime;
            }
            else if (sortBy == "customerName")
            {
                condition = array[j].customerName < pivot.customerName;
            }

            if (condition)
            {
                i++;
                swap(array[i], array[j]);
            }
        }

        swap(array[i + 1], array[right]);

        return i + 1;
    }

    void qs(Ticket *array, int left, int right, string sortBy)
    {
        if (left < right)
        {
            int pi = partition(array, left, right, sortBy);
            qs(array, left, pi - 1, sortBy);
            qs(array, pi + 1, right, sortBy);
        }
    }

    void quickSort(string sortBy)
    {
        // Convert LL to Array
        Ticket *tickets = toArray();
        int length = getLength();

        qs(tickets, 0, length - 1, sortBy);

        fromArray(tickets, length);
        delete[] tickets;
    }

    void merge(Ticket *tickets, int left, int mid, int right, string sortBy)
    {
        if (left == right)
        {
            return;
        }

        int n1 = mid - left + 1;
        int n2 = right - mid;

        Ticket *leftArray = new Ticket[n1];
        Ticket *rightArray = new Ticket[n2];

        for (int i = 0; i < n1; i++)
        {
            leftArray[i] = tickets[left + i];
        }

        for (int i = 0; i < n2; i++)
        {
            rightArray[i] = tickets[mid + 1 + i];
        }

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2)
        {

            bool condition = false;

            if (sortBy == "priority")
            {
                condition = leftArray[i].priority <= rightArray[j].priority;
            }
            else if (sortBy == "creationTime")
            {
                condition = leftArray[i].creationTime <= rightArray[j].creationTime;
            }
            else if (sortBy == "customerName")
            {
                condition = leftArray[i].customerName <= rightArray[j].customerName;
            }

            if (condition)
            {
                tickets[k] = leftArray[i];
                i++;
                k++;
            }
            else

            {
                tickets[k] = rightArray[j];
                j++;
                k++;
            }
        }

        while (i < n1)
        {
            tickets[k] = leftArray[i];
            i++;
            k++;
        }

        while (j < n2)
        {
            tickets[k] = rightArray[j];
            j++;
            k++;
        }

        delete[] leftArray;
        delete[] rightArray;
    }

    void ms(Ticket *tickets, int left, int right, string sortBy)
    {
        if (left < right)
        {
            int mid = left + (right - left) / 2;

            ms(tickets, left, mid, sortBy);
            ms(tickets, mid + 1, right, sortBy);
            merge(tickets, left, mid, right, sortBy);
        }
    }

    void mergeSort(string sortBy)
    {

        Ticket *tickets = toArray();
        int length = getLength();

        ms(tickets, 0, length - 1, sortBy);

        fromArray(tickets, length);
        delete[] tickets;
    }
};

int main()
{
    Ticket ticket1(2, "S1arim Ahmed", 1, "A request");
    Ticket ticket2(1, "S2arim Ahmed", 1, "A request");
    Ticket ticket3(2, "S1arim Ahmed", 1, "A request");
    Ticket ticket4(1, "S2arim Ahmed", 1, "A request");
    Ticket ticket5(4, "S3arim Ahmed", 1, "A request");
    Ticket ticket6(2, "S1arim Ahmed", 1, "A request");
    Ticket ticket7(4, "S3arim Ahmed", 1, "A request");
    Ticket ticket8(1, "S2arim Ahmed", 1, "A request");
    Ticket ticket9(8, "S4arim Ahmed", 1, "A request");
    Ticket ticket10(1, "S2arim Ahmed", 1, "A request");

    TicketLL ll;

    ticket4.close();

    ll.addTicket(ticket1);
    ll.addTicket(ticket2);
    ll.addTicket(ticket3);
    ll.addTicket(ticket4);
    ll.addTicket(ticket5);
    ll.addTicket(ticket6);
    ll.addTicket(ticket7);
    ll.addTicket(ticket8);
    ll.addTicket(ticket9);
    ll.addTicket(ticket10);

    ll.quickSort("creationTime");

    // ll.removeTicket(1);
    // ll.removeTicket(2);
    // ll.removeTicket(4);
    // ll.removeTicket(8);

    ll.traverse();
}