#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>

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
        string customerName,
        int priority,
        string requestDescription)
        : customerName(customerName),
          priority(priority),
          requestDescription(requestDescription),
          creationTime(chrono::system_clock::now()),
          isOpen(true)
    {
        id = rand() % 1000000;
    }

    time_t getTime()
    {
        return system_clock::to_time_t(creationTime);
    }

    void print()
    {
        cout << "====================" << endl;
        cout << "    TICKET INFO" << endl;
        cout << "====================" << endl;
        cout << "Ticket ID: " << id << endl;
        cout << "Priority: " << priority << endl;
        cout << "Customer Name: " << customerName << endl;
        cout << "Request Decription: " << requestDescription << endl;
        cout << "Creation Time: ";
        printCreationTime();

        if (!isOpen)
        {
            cout << "Closing Time: ";
            printClosingTime();
        }

        cout << "Is Open: " << isOpen << endl;

        cout << endl;
    }

    void printCreationTime()
    {
        time_t retTime = system_clock::to_time_t(creationTime);
        cout << ctime(&retTime);
    }

    void printClosingTime()
    {
        time_t retTime = system_clock::to_time_t(closeTime);
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
    Ticket *ticket;
    TicketNode *next;
    TicketNode *prev;

    TicketNode(Ticket *ticket) : ticket(ticket), next(NULL), prev(NULL) {}
};

class TicketLL
{
public:
    TicketNode *head;
    TicketNode *tail;
    int length;

    TicketLL() : head(NULL), tail(NULL), length(0) {}

    // Print all tickets
    void print()
    {
        TicketNode *temp = head;

        while (temp != NULL)
        {
            temp->ticket->print();
            temp = temp->next;
        }
    }

    // Print all open tickets
    void displayOpenTickets()
    {
        TicketNode *temp = head;

        while (temp != NULL)
        {
            if (temp->ticket->isOpen)
            {
                temp->ticket->print();
            }
            temp = temp->next;
        }
    }

    // Print all closed tickets
    void displayClosedTickets()
    {
        TicketNode *temp = head;

        while (temp != NULL)
        {
            if (!temp->ticket->isOpen)
            {
                temp->ticket->print();
            }
            temp = temp->next;
        }
    }

    // Add a ticket to the list
    void addTicket(Ticket *ticket)
    {
        TicketNode *newNode = new TicketNode(ticket);
        length++;

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

    // Remove a ticket by id
    void removeTicket(int id)
    {
        TicketNode *temp = head;

        while (temp != NULL)
        {
            if (temp->ticket->id == id)
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

    // Returns a reference to a ticket, given it's id
    Ticket *getTicketById(int id)
    {
        TicketNode *current = head;

        while (current != NULL)
        {
            if (current->ticket->id == id)
            {
                return current->ticket;
            }

            current = current->next;
        }

        return NULL;
    }

    // Returns a reference to a ticket, given it's customer's name
    Ticket *getTicketByCustomerName(string customerName)
    {
        TicketNode *current = head;

        while (current != NULL)
        {
            if (current->ticket->customerName == customerName)
            {
                return current->ticket;
            }

            current = current->next;
        }

        return NULL;
    }

    // Returns a dynamic array containing elements of the linked list
    Ticket **toArray()
    {

        // Initializing array
        if (length > 0)
        {
            Ticket **ticketsArray = new Ticket *[length];

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

    // Remove all nodes from the linked list
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

    // Repopulate the linked list, given an array
    void fromArray(Ticket **array, int length)
    {
        deleteAll();

        for (int i = length - 1; i >= 0; i--)
        {
            addTicket(array[i]);
        }
    }

    // Sorting algorithms
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
                    condition = current->prev->ticket->priority > current->ticket->priority;
                }
                else if (sortBy == "creationTime")
                {
                    condition = current->prev->ticket->creationTime > current->ticket->creationTime;
                }
                else if (sortBy == "customerName")
                {
                    condition = current->prev->ticket->customerName > current->ticket->customerName;
                }

                if (condition)
                {
                    Ticket *temp = current->prev->ticket;
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

            Ticket *key = current->ticket;

            TicketNode *j = current;
            bool condition = false;

            if (sortBy == "priority" && j->prev != NULL)
            {
                condition = j->prev->ticket->priority > key->priority;
            }
            else if (sortBy == "creationTime" && j->prev != NULL)
            {
                condition = j->prev->ticket->creationTime > key->creationTime;
            }
            else if (sortBy == "customerName" && j->prev != NULL)
            {
                condition = j->prev->ticket->customerName > key->customerName;
            }

            while (j->prev != NULL && condition)
            {

                j->ticket = j->prev->ticket;
                j = j->prev;

                condition = false;

                if (sortBy == "priority" && j->prev != NULL)
                {
                    condition = j->prev->ticket->priority > key->priority;
                }
                else if (sortBy == "creationTime" && j->prev != NULL)
                {
                    condition = j->prev->ticket->creationTime > key->creationTime;
                }
                else if (sortBy == "customerName" && j->prev != NULL)
                {
                    condition = j->prev->ticket->customerName > key->customerName;
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
                    condition = current->ticket->priority > minimum->ticket->priority;
                }
                else if (sortBy == "creationTime")
                {
                    condition = current->ticket->creationTime > minimum->ticket->creationTime;
                }
                else if (sortBy == "customerName")
                {
                    condition = current->ticket->customerName > minimum->ticket->customerName;
                }

                if (condition)
                {
                    minimum = current;
                }

                Ticket *temp = minimum->ticket;
                minimum->ticket = current->ticket;
                current->ticket = temp;

                current = current->next;
            }

            i = i->next;
        }
    }

    int partition(Ticket **array, int left, int right, string sortBy)
    {

        Ticket pivot = *array[right];
        int i = left - 1;

        for (int j = left; j < right; j++)
        {

            bool condition = false;

            if (sortBy == "priority")
            {
                condition = array[j]->priority < pivot.priority;
            }
            else if (sortBy == "creationTime")
            {
                condition = array[j]->creationTime < pivot.creationTime;
            }
            else if (sortBy == "customerName")
            {
                condition = array[j]->customerName < pivot.customerName;
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

    void qs(Ticket **array, int left, int right, string sortBy)
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
        Ticket **tickets = toArray();

        qs(tickets, 0, length - 1, sortBy);

        fromArray(tickets, length);
        delete[] tickets;
    }

    void merge(Ticket **tickets, int left, int mid, int right, string sortBy)
    {
        if (left == right)
        {
            return;
        }

        int n1 = mid - left + 1;
        int n2 = right - mid;

        Ticket **leftArray = new Ticket *[n1];
        Ticket **rightArray = new Ticket *[n2];

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
                condition = leftArray[i]->priority <= rightArray[j]->priority;
            }
            else if (sortBy == "creationTime")
            {
                condition = leftArray[i]->creationTime <= rightArray[j]->creationTime;
            }
            else if (sortBy == "customerName")
            {
                condition = leftArray[i]->customerName <= rightArray[j]->customerName;
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

    void ms(Ticket **tickets, int left, int right, string sortBy)
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

        Ticket **tickets = toArray();

        ms(tickets, 0, length - 1, sortBy);

        fromArray(tickets, length);
        delete[] tickets;
    }
};

class Agent
{
public:
    int id;
    string name;
    string type;
    int numTicketsAssigned;
    Ticket *assignedTickets[5];
    bool available;

    Agent() {}

    Agent(
        string name,
        string type)
        : name(name),
          type(type),
          numTicketsAssigned(0),
          available(true)
    {
        id = rand() % 1000000;
    }

    // Print the agent
    void print()
    {
        cout << "========================" << endl;
        cout << "   AGENT DESCRIPTION" << endl;
        cout << "========================" << endl;
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Type: " << type << endl;
        cout << "Total Tickets Assigned: " << numTicketsAssigned
             << endl;
        for (int i = 0; i < numTicketsAssigned; i++)
        {
            cout << "--> Ticket " << i + 1 << ":" << endl;
            assignedTickets[i]->print();
        }
    }

    // Assign a given ticket to a specific agent
    void assignTickets(Ticket *ticket)
    {
        if (available)
        {
            assignedTickets[numTicketsAssigned] = ticket;
            numTicketsAssigned++;

            if (numTicketsAssigned >= 5)
            {
                available = false;
            }
        }
    }

    // Resolves a ticket assigned to any agent
    Ticket *resolveTicket()
    {
        if (numTicketsAssigned > 0)
        {
            Ticket *resolvedTicket = assignedTickets[0];

            numTicketsAssigned--;

            // Push all tickets one step back
            for (int i = 1; i < 5; i++)
            {
                assignedTickets[i - 1] = assignedTickets[i];
            }

            assignedTickets[4] = NULL;

            available = true;
            return resolvedTicket;
        }

        return NULL;
    }
};

class AgentArray
{
public:
    Agent **data;
    int capacity;
    int length;

    AgentArray()
        : capacity(1),
          length(0)
    {
        data = new Agent *[capacity];
    }

    // Print all agents
    void print()
    {
        for (int i = 0; i < length; i++)
        {
            data[i]->print();
            cout << endl;
        }
    }

    // Add a new agent
    void addAgent(Agent *agent)
    {

        length++;

        if (length > capacity)
        {
            // Update array capacity
            capacity *= 2;

            Agent **newData = new Agent *[capacity];

            // Copy old data
            for (int i = 0; i < length - 1; i++)
            {
                newData[i] = data[i];
            }

            delete[] data;
            data = newData;
        }

        data[length - 1] = agent;
    }

    // Assigning ticket to the agent that has the lowest workload
    bool assignTickets(Ticket *ticket)
    {

        int mostFree = -1;
        for (int i = 0; i < length; i++)
        {
            if (data[i]->available && data[i]->type == ticket->requestDescription)
            {
                if (mostFree == -1)
                {
                    mostFree = i;
                }
                else if (data[i]->numTicketsAssigned < data[mostFree]->numTicketsAssigned)
                {
                    mostFree = i;
                }
            }
        }

        if (mostFree != -1)
        {
            data[mostFree]->assignTickets(ticket);
            return true;
        }

        return false;
    }

    // resolve a single ticket
    Ticket *resolveTicket()
    {
        for (int i = 0; i < length; i++)
        {
            if (data[i]->numTicketsAssigned > 0)
            {
                return data[i]->resolveTicket();
            }
        }

        return NULL;
    }

    // Sorting algorithms
    void bubbleSort()
    {
        for (int i = 0; i < length; i++)
        {
            for (int j = 0; j < length - 1; j++)
            {
                if (data[j]->numTicketsAssigned > data[j + 1]->numTicketsAssigned)
                {
                    Agent *temp = data[j];
                    data[j] = data[j + 1];
                    data[j + 1] = temp;
                }
            }
        }
    }

    void selectionSort()
    {
        for (int i = 0; i < length - 1; i++)
        {
            int smallest = i;

            for (int j = i + 1; j < length; j++)
            {
                if (data[j]->numTicketsAssigned < data[smallest]->numTicketsAssigned)
                {
                    smallest = j;
                }
            }

            if (i != smallest)
            {
                Agent *temp = data[smallest];
                data[smallest] = data[i];
                data[i] = temp;
            }
        }
    }

    void insertionSort()
    {
        for (int i = 1; i < length; i++)
        {
            Agent *key = data[i];
            int j = i;

            while (j > 0 && data[j - 1]->numTicketsAssigned > key->numTicketsAssigned)
            {
                data[j] = data[j - 1];
                j--;
            }

            data[j] = key;
        }
    }

    void merge(int left, int mid, int right)
    {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        Agent **leftArray = new Agent *[n1];
        Agent **rightArray = new Agent *[n2];

        for (int i = 0; i < n1; i++)
        {
            leftArray[i] = data[left + i];
        }

        for (int i = 0; i < n2; i++)
        {
            rightArray[i] = data[mid + i + 1];
        }

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2)
        {
            if (leftArray[i]->numTicketsAssigned <= rightArray[j]->numTicketsAssigned)
            {
                data[k] = leftArray[i];
                i++;
                k++;
            }
            else
            {
                data[k] = rightArray[j];
                j++;
                k++;
            }
        }

        while (i < n1)
        {
            data[k] = leftArray[i];
            i++;
            k++;
        }

        while (j < n2)
        {
            data[k] = rightArray[j];
            j++;
            k++;
        }

        delete[] leftArray;
        delete[] rightArray;
    }

    void ms(int left, int right)
    {
        if (left < right)
        {

            int mid = left + (right - left) / 2;

            ms(left, mid);
            ms(mid + 1, right);
            merge(left, mid, right);
        }
    }

    void mergeSort()
    {
        ms(0, length - 1);
    }

    int partition(int left, int right)
    {
        int pivot = right;

        if (right <= left)
        {
            return left;
        }

        int i = left - 1;

        for (int j = left; j < right; j++)
        {
            if (data[j]->numTicketsAssigned > data[pivot]->numTicketsAssigned)
            {
                i++;

                Agent *temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }

        Agent *temp = data[pivot];
        data[pivot] = data[i + 1];
        data[i + 1] = temp;

        return i + 1;
    }

    void qs(int left, int right)
    {
        if (left < right)
        {
            int pi = partition(left, right);
            qs(left, pi - 1);
            qs(pi + 1, right);
        }
    }

    void quickSort()
    {
        qs(0, length - 1);
    }
};

class TicketResolutionNode
{
public:
    Ticket *ticket;
    TicketResolutionNode *next;
    chrono::_V2::system_clock::time_point closeTime;

    TicketResolutionNode(
        Ticket *ticket)
        : ticket(ticket),
          next(NULL),
          closeTime(ticket->closeTime) {}
};

class ResolutionLogsStack
{
    TicketResolutionNode *top;

public:
    ResolutionLogsStack() : top(NULL) {}

    // Print the complete logs
    void print()
    {
        cout << "===============================" << endl;
        cout << "      RESOLVED TICKETS" << endl;
        cout << "===============================" << endl;

        TicketResolutionNode *current = top;

        while (current != NULL)
        {
            current->ticket->print();
            current = current->next;
        }
    }

    // Add a new Log
    void addTicket(Ticket *ticket)
    {
        if (ticket->isOpen)
        {
            cout << "[-] Cannot add to resolution stack, ticket is open." << endl;
            return;
        }

        TicketResolutionNode *newNode = new TicketResolutionNode(ticket);

        if (top == NULL)
        {
            top = newNode;
            return;
        }

        newNode->next = top;
        top = newNode;
    }

    // Get the most recent log
    Ticket *peek()
    {
        if (top == NULL)
        {
            return NULL;
        }

        return top->ticket;
    }
};

class PendingTicketsQueue
{
    TicketNode *front, *back;
    int length;

public:
    PendingTicketsQueue() : front(NULL), back(NULL), length(0) {}

    // whether the queue is empty or not
    bool isEmpty()
    {
        return front == NULL && back == NULL;
    }

    // enqueue a ticket based on priority
    void enqueueTicket(Ticket *ticket)
    {
        if (!ticket->isOpen)
        {
            cout << "Cannot enqueue, this queue is for open tickets only." << endl;
            return;
        }

        TicketNode *newNode = new TicketNode(ticket);
        length++;

        if (isEmpty())
        {
            front = back = newNode;
            return;
        }

        // Enqueing According to order of queue
        TicketNode *current = back;

        while (current != NULL && current->ticket->priority < ticket->priority)
        {
            current = current->next;
        }

        if (current == NULL)
        {
            front->next = newNode;
            newNode->prev = front;
            front = newNode;

            return;
        }

        if (current == back)
        {
            newNode->next = back;
            back->prev = newNode;

            back = newNode;

            return;
        }

        TicketNode *currentPrev = current->prev;

        currentPrev->next = newNode;
        newNode->prev = currentPrev;

        current->prev = newNode;
        newNode->next = current;

        return;
    }

    // dequeue a ticket
    Ticket *dequeueTicket()
    {
        if (isEmpty())
        {
            return NULL;
        }

        Ticket *removedTicket = front->ticket;

        // If last element
        if (front == back)
        {
            front = back = NULL;
        }
        else
        {
            front = front->prev;
            front->next = NULL;
        }

        return removedTicket;
    }

    // print the whole queue
    void displayQueue()
    {
        cout << "================================" << endl;
        cout << "     PENDING TICKETS QUEUE" << endl;
        cout << "================================" << endl;
        TicketNode *current = front;

        while (current != NULL)
        {
            current->ticket->print();
            current = current->prev;
        }
    }

    // returns the ticket at front if exits
    Ticket *peekFront()
    {
        if (front)
        {
            return front->ticket;
        }
        else
        {
            return NULL;
        }
    }

    // returns the ticket at back if exits
    Ticket *peekBack()
    {
        if (back)
        {
            return back->ticket;
        }
        else
        {
            return NULL;
        }
    }
};

class Configuration
{
public:
    string sortHigh;
    string sortLow;
    int threshold;

    Configuration()
    {
        // Load data from file
        loadFromFile();
    }

    // Sets threshold and updates it in files
    void setThreshold(int value)
    {
        threshold = value;
        writeToFile();
    }

    // Sets sorting algorithm for larger datasets
    void setSortHigh(string value)
    {
        sortHigh = value;
        writeToFile();
    }

    // Sets sorting algorithm for smaller datasets
    void setSortLow(string value)
    {
        sortLow = value;
        writeToFile();
    }

    // Display the set configuration
    void display()
    {
        cout << "=========================================" << endl;
        cout << "    DISPLAYING SORTING CONFIGURATION" << endl;
        cout << "=========================================" << endl;

        cout << "Above Threshold Sort: " << sortHigh << endl;
        cout << "Below Threshold Sort: " << sortLow << endl;
        cout << "Threshold: " << threshold << endl;
    }

    // Set default configuration
    void setDefaultData()
    {
        sortHigh = "quick";
        sortLow = "insertion";
        threshold = 10;
    }

    // Write config data to file
    void writeToFile()
    {
        ofstream configFile("oneStop.conf");

        configFile << sortHigh << endl;
        configFile << sortLow << endl;
        configFile << threshold << endl;
    }

    // Load config data from file
    void loadFromFile()
    {
        ifstream configFile("oneStop.conf");

        if (!configFile)
        {
            setDefaultData();
            writeToFile();
            return;
        }

        string rawThreshold;
        getline(configFile, sortHigh);
        getline(configFile, sortLow);
        getline(configFile, rawThreshold);

        threshold = stoi(rawThreshold);
    }
};

class OneStopTicketManagement
{
private:
public:
    TicketLL allTickets;
    AgentArray agents;
    ResolutionLogsStack resolvedTickets;
    Configuration configuration;

    PendingTicketsQueue pendingTicketsIT;
    PendingTicketsQueue pendingTicketsAdmin;
    PendingTicketsQueue pendingTicketsAccounts;
    PendingTicketsQueue pendingTicketsAcademics;

    // Function to start the management system's user interaction
    void run()
    {
        bool running = true;
        int option;

        while (running)
        {
            // Main Menu
            cout << "========================================" << endl;
            cout << "   ONE STOP TICKET MANAGEMENT SYSTEM" << endl;
            cout << "========================================" << endl;
            cout << endl;
            cout << "[0] Quit" << endl;
            cout << "[1] Admin Menu" << endl;
            cout << "[2] User Menu" << endl;
            cout << endl;
            cout << "Your option: ";

            cin >> option;

            string agentName, agentType, inputStr, customerNameInput, ticketDetailsInput;
            int inputInt;

            switch (option)
            {
            case 0:
                // Exit
                running = false;
                break;

            case 1:;
                // Admin Menu
                cout << "==============================" << endl;
                cout << "        ADMIN MENU" << endl;
                cout << "==============================" << endl;
                cout << "[0] Quit" << endl;
                cout << "[1] Add Agent" << endl;
                cout << "[2] Assign Tickets" << endl;
                cout << "[3] Resolve a Ticket" << endl;
                cout << "[4] Print Tickets (Open/Closed)" << endl;
                cout << "[5] Print Config" << endl;
                cout << "[6] Change Config" << endl;
                cout << "[7] Search Ticket with Report" << endl;
                cout << "[8] Print Queue" << endl;
                cout << "[9] Print Resolved Tickets" << endl;
                cout << "[10] Show Agents" << endl;
                cout << "[11] Sort Agents with Report" << endl;
                cout << "[12] Sort Tickets with Report" << endl;
                cout << endl;
                cout << "Your option: ";

                cin >> option;

                switch (option)
                {
                case 0:
                    // exit
                    running = false;
                    break;

                case 1:
                    // Add Agent
                    cout << "-> Enter the agent's name: ";
                    cin >> agentName;

                    cout << "-> Enter the agent's type (IT, Admin, Accounts, Academics) :";
                    cin >> agentType;

                    addNewAgent(agentName, agentType);

                    cout << "[+] Agent added." << endl;
                    char c;
                    cout << "Press any key to continue...";
                    getchar();
                    getchar();
                    cout << endl;
                    break;

                case 2:
                    // Assign Tickets
                    assignTickets();

                    cout << "[+] Assigned tickets to available agents." << endl;
                    cout << "Press any key to continue...";
                    getchar();
                    getchar();
                    cout << endl;
                    break;

                case 3:
                    // Resolve Ticket
                    cout << "[+] Attempting to resolve a ticket assigned to agents." << endl;
                    resolveTicket();

                    cout << "Press any key to continue...";
                    getchar();
                    getchar();
                    cout << endl;
                    break;

                case 4:
                    cout << "[0] Quit" << endl;
                    cout << "[1] Show Open Tickets" << endl;
                    cout << "[2] Show Closed Tickets" << endl;
                    cout << "[3] Show All Tickets" << endl;

                    cout << endl;
                    cout << "Your option: ";
                    cin >> option;

                    switch (option)
                    {
                    case 0:
                        running = false;
                        break;

                    case 1:
                        allTickets.displayOpenTickets();
                        break;

                    case 2:
                        allTickets.displayClosedTickets();
                        break;

                    case 3:
                        allTickets.print();
                        break;

                    default:
                        break;
                    }
                    cout << "Press any key to continue...";
                    getchar();
                    getchar();
                    cout << endl;
                    break;
                    break;

                case 5:
                    configuration.display();
                    break;

                case 6:
                    cout << "[0] Quit" << endl;
                    cout << "[1] Set Threshold" << endl;
                    cout << "[2] Set High Sorting Algorithm" << endl;
                    cout << "[3] Set Low Sorting Algorithm" << endl;
                    cout << endl;
                    cout << "Your option: ";
                    cin >> option;

                    switch (option)
                    {
                    case 0:
                        running = false;
                        break;

                    case 1:
                        cout << "-> Enter new threshold: ";
                        cin >> inputInt;

                        configuration.setThreshold(inputInt);
                        break;

                    case 2:
                        cout << " - bubble" << endl;
                        cout << " - insertion" << endl;
                        cout << " - selection" << endl;
                        cout << " - quick" << endl;
                        cout << " - merge" << endl;
                        cout << " -> Enter one of the above: ";
                        cin >> inputStr;

                        configuration.setSortHigh(inputStr);
                        break;

                    case 3:
                        cout << " - bubble" << endl;
                        cout << " - insertion" << endl;
                        cout << " - selection" << endl;
                        cout << " - quick" << endl;
                        cout << " - merge" << endl;
                        cout << " -> Enter one of the above: ";
                        cin >> inputStr;

                        configuration.setSortLow(inputStr);
                        break;

                    default:
                        break;
                    }

                    break;

                case 7:
                    cout << "What do you want to search by?" << endl;
                    cout << "[0] ID" << endl;
                    cout << "[1] Customer Name" << endl;
                    cout << endl;
                    cout << "Your option: " << endl;

                    cin >> option;

                    if (!option)
                    {
                        cout << "Enter ID: ";
                        cin >> inputInt;

                        searchTicketByID(inputInt);
                    }
                    else
                    {
                        cout << "Enter Customer Name: ";
                        cin >> inputStr;

                        searchTicketByName(inputStr);
                    }

                    break;

                case 8:
                    cout << "[0] IT" << endl;
                    cout << "[1] Admin" << endl;
                    cout << "[2] Accounts" << endl;
                    cout << "[3] Academics" << endl;
                    cout << endl;
                    cout << "Choose a type:" << endl;

                    cin >> option;

                    switch (option)
                    {
                    case 0:
                        pendingTicketsIT.displayQueue();
                        break;

                    case 1:
                        pendingTicketsAdmin.displayQueue();
                        break;

                    case 2:
                        pendingTicketsAccounts.displayQueue();
                        break;

                    case 3:
                        pendingTicketsAcademics.displayQueue();
                        break;

                    default:
                        break;
                    }

                    break;

                case 9:
                    resolvedTickets.print();
                    break;

                case 10:
                    agents.print();
                    break;

                case 11:
                    sortAgents();
                    cout << "[+] Agents Sorted." << endl;
                    break;

                case 12:
                    cout << "What do you want to sort them by?" << endl;
                    cout << "[0] Priority" << endl;
                    cout << "[1] Creation Time" << endl;
                    cout << "[2] Customer Name" << endl;
                    cout << endl;
                    cout << "Your option: ";
                    cin >> option;

                    switch (option)
                    {
                    case 0:
                        sortTickets("priority");
                        break;
                    case 1:
                        sortTickets("creationTime");
                        break;
                    case 2:
                        sortTickets("customerName");
                        break;

                    default:
                        break;
                    }
                    cout << "[+] Tickets Sorted." << endl;

                    break;

                default:
                    break;
                }

                break;
            case 2:
                // User Menu
                cout << "[0] Quit" << endl;
                cout << "[1] Add Ticket" << endl;
                cout << "[2] Search Ticket" << endl;
                cout << "[3] See Pending Tickets" << endl;
                cout << endl;
                cout << "Your option: ";

                cin >> option;

                switch (option)
                {
                case 0:
                    running = false;
                    break;
                case 1:

                    cout << "-> Enter Customer Name: ";
                    cin >> customerNameInput;

                    cout << "-> Enter Ticket Priority: ";
                    cin >> inputInt;

                    cout << "-> Enter ticket type/description (IT, Admin, Accounts, Academics): ";
                    cin >> ticketDetailsInput;

                    inputInt = addNewTicket(customerNameInput, inputInt, ticketDetailsInput);
                    cout << "[+] Ticket Successfully added." << endl;
                    cout << "[+] Your Ticket ID: " << inputInt << endl;
                    char c;
                    cout << "Press any key to continue...";
                    getchar();
                    getchar();
                    cout << endl;

                    break;
                case 2:
                    cout << "What do you want to search by?" << endl;
                    cout << "[0] ID" << endl;
                    cout << "[1] Customer Name" << endl;
                    cout << endl;
                    cout << "Your option: " << endl;

                    cin >> option;

                    if (!option)
                    {
                        cout << "Enter ID: ";
                        cin >> inputInt;

                        searchTicketByID(inputInt);
                    }
                    else
                    {
                        cout << "Enter Customer Name: ";
                        cin >> inputStr;

                        searchTicketByName(inputStr);
                    }

                    break;

                case 3:
                    cout << "[0] IT" << endl;
                    cout << "[1] Admin" << endl;
                    cout << "[2] Accounts" << endl;
                    cout << "[3] Academics" << endl;
                    cout << endl;
                    cout << "Choose a type:";

                    cin >> option;

                    switch (option)
                    {
                    case 0:
                        pendingTicketsIT.displayQueue();
                        break;

                    case 1:
                        pendingTicketsAdmin.displayQueue();
                        break;

                    case 2:
                        pendingTicketsAccounts.displayQueue();
                        break;

                    case 3:
                        pendingTicketsAcademics.displayQueue();
                        break;

                    default:
                        break;
                    }

                    break;
                default:
                    break;
                }

                break;

            default:
                break;
            }
        }
    }

    // Function that assigns the most urgent ticket from each type to available agents
    void assignTickets()
    {
        // Assigning from IT
        Ticket *pendingTicket = pendingTicketsIT.peekFront();

        if (pendingTicket != NULL)

        {

            bool successfullyAssigned = agents.assignTickets(pendingTicket);

            if (successfullyAssigned)
            {
                pendingTicketsIT.dequeueTicket();
            }
            else
            {

                cout << "[-] No free IT agents, can't assign ticket." << endl;
            }
        }

        // Assigning from Admin
        pendingTicket = pendingTicketsAdmin.peekFront();

        if (pendingTicket != NULL)

        {

            bool successfullyAssigned = agents.assignTickets(pendingTicket);

            if (successfullyAssigned)
            {
                pendingTicketsAdmin.dequeueTicket();
            }
            else
            {
                cout << "[-] No free Admin agents, can't assign ticket." << endl;
            }
        }

        // Assigning from Accounts
        pendingTicket = pendingTicketsAccounts.peekFront();

        if (pendingTicket != NULL)

        {

            bool successfullyAssigned = agents.assignTickets(pendingTicket);

            if (successfullyAssigned)
            {
                pendingTicketsAccounts.dequeueTicket();
            }
            else
            {
                cout << "[-] No free Accounts agents, can't assign ticket." << endl;
            }
        }

        // Assigning from Academics
        pendingTicket = pendingTicketsAcademics.peekFront();

        if (pendingTicket != NULL)

        {

            bool successfullyAssigned = agents.assignTickets(pendingTicket);

            if (successfullyAssigned)
            {
                pendingTicketsAcademics.dequeueTicket();
            }
            else
            {
                cout << "[-] No free Academics agents, can't assign ticket." << endl;
            }
        }
    }

    // Function that resolves tickets every time its called
    void resolveTicket()
    {
        Ticket *resolvedTicket = agents.resolveTicket();

        if (resolvedTicket == NULL)
        {
            cout << "[+] No tickets to resolve." << endl;
            return;
        }

        resolvedTicket->close();
        resolvedTickets.addTicket(resolvedTicket);
    }

    // Function that adds a new ticket to the system
    int addNewTicket(string customerName, int priority, string requestDescription)
    {
        Ticket *newTicket = new Ticket(customerName, priority, requestDescription);

        allTickets.addTicket(newTicket);

        if (requestDescription == "IT")
        {
            pendingTicketsIT.enqueueTicket(newTicket);
        }
        else if (requestDescription == "Admin")
        {
            pendingTicketsAdmin.enqueueTicket(newTicket);
        }
        else if (requestDescription == "Accounts")
        {
            pendingTicketsAccounts.enqueueTicket(newTicket);
        }
        else if (requestDescription == "Academics")
        {
            pendingTicketsAcademics.enqueueTicket(newTicket);
        }

        return newTicket->id;
    }

    // Function to add a new agent to the system
    void addNewAgent(string name, string type)
    {
        Agent *newAgent = new Agent(name, type);
        agents.addAgent(newAgent);
    }

    // Search a ticket by id
    void searchTicketByID(int id)
    {
        Ticket *ticket = allTickets.getTicketById(id);

        if (ticket)
        {
            cout << "[+] Here is your ticket." << endl;

            ticket->print();
        }
    }

    // Search a ticket by customer name
    void searchTicketByName(string customerName)
    {
        Ticket *ticket = allTickets.getTicketByCustomerName(customerName);

        if (ticket)
        {
            cout << "[+] Here is your ticket." << endl;

            ticket->print();
        }
    }

    // Display all tickets that ever entered the system
    void displayAllTickets()
    {
        cout << "======================================" << endl;
        cout << "      Displaying All Tickets" << endl;
        cout << "======================================" << endl;

        allTickets.print();
    }

    // Display all open tickets that ever entered the system
    void displayAllOpenTickets()
    {
        cout << "======================================" << endl;
        cout << "      Displaying All Open Tickets" << endl;
        cout << "======================================" << endl;

        allTickets.displayOpenTickets();
    }

    // Display all closed tickets that ever entered the system
    void displayAllClosedTickets()
    {
        cout << "======================================" << endl;
        cout << "    Displaying All Closed Tickets" << endl;
        cout << "======================================" << endl;

        allTickets.displayClosedTickets();
    }

    // Sort agents based on the configuration set
    void sortAgents()
    {
        string sortAlgorithm;

        if (agents.length <= configuration.threshold)
        {
            sortAlgorithm = configuration.sortLow;
        }
        else
        {
            sortAlgorithm = configuration.sortHigh;
        }

        chrono::_V2::system_clock::time_point sortStart = chrono::_V2::system_clock::now();

        if (sortAlgorithm == "bubble")
        {
            agents.bubbleSort();
        }
        else if (sortAlgorithm == "insertion")
        {
            agents.insertionSort();
        }
        else if (sortAlgorithm == "selection")
        {
            agents.selectionSort();
        }
        else if (sortAlgorithm == "quick")
        {
            agents.quickSort();
        }
        else if (sortAlgorithm == "merge")
        {
            agents.mergeSort();
        }

        chrono::_V2::system_clock::time_point sortEnd = chrono::_V2::system_clock::now();

        cout << "--> Agents Sorting Report" << endl;
        cout << "Algorithm: " << sortAlgorithm << endl;
        cout << "n: " << agents.length << endl;
        cout << "Time Taken (ms): " << difftime(system_clock::to_time_t(sortEnd), system_clock::to_time_t(sortStart)) << endl;
    }

    // Sort tickets based on the configuration set
    void sortTickets(string sortBy)
    {
        string sortAlgorithm;

        if (allTickets.length <= configuration.threshold)
        {
            sortAlgorithm = configuration.sortLow;
        }
        else
        {
            sortAlgorithm = configuration.sortHigh;
        }

        chrono::_V2::system_clock::time_point sortStart = chrono::_V2::system_clock::now();

        if (sortAlgorithm == "bubble")
        {
            allTickets.bubbleSort(sortBy);
        }
        else if (sortAlgorithm == "insertion")
        {
            allTickets.insertionSort(sortBy);
        }
        else if (sortAlgorithm == "selection")
        {
            allTickets.selectionSort(sortBy);
        }
        else if (sortAlgorithm == "quick")
        {
            allTickets.quickSort(sortBy);
        }
        else if (sortAlgorithm == "merge")
        {
            allTickets.mergeSort(sortBy);
        }

        chrono::_V2::system_clock::time_point sortEnd = chrono::_V2::system_clock::now();

        cout << "--> Ticket Sorting Report" << endl;
        cout << "Algorithm: " << sortAlgorithm << endl;
        cout << "n: " << allTickets.length << endl;
        cout << "Time Taken (ms): " << system_clock::to_time_t(sortEnd) - system_clock::to_time_t(sortStart) << endl;
    }
};

int main()
{
    srand(time(NULL));

    OneStopTicketManagement tm;

    // Adding agents as per assignment requirement
    tm.addNewAgent("Sarim", "IT");
    tm.addNewAgent("Moosa", "Admin");
    tm.addNewAgent("Shaheer", "Accounts");
    tm.addNewAgent("Rafay", "Academics");
    tm.addNewAgent("Abdullah", "Academics");
    tm.addNewAgent("Usaid", "Academics");

    // Initiating the user interaction.
    tm.run();

    exit(0);
}