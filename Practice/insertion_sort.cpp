#include <iostream>

using namespace std;

void insertionSort(int array[], int length)
{
    for (int i = 1; i < length; i++)
    {
        int key = array[i];
        int j = i - 1;

        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = key;
    }
}

int main()
{
    int array[] = {
        10,
        6,
        9,
        8,
        3,
        5,
        14,
        7,
        2,
        1,
    };

    int length = sizeof(array) / sizeof(array[0]);

    insertionSort(array, length);

    for (int i = 0; i < length; i++)
    {
        cout << array[i] << " ";
    }
    cout << endl;
}