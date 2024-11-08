#include <iostream>

using namespace std;

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

int partition(int array[], int left, int right)
{
    if (left == right)
    {
        return left;
    }

    int pivot = right;

    int i = left - 1;

    for (int j = left; j < right; j++)
    {
        if (array[j] < array[pivot])
        {
            i++;
            swap(array[i], array[j]);
        }
    }

    swap(array[pivot], array[i + 1]);

    return i + 1;
}

void qs(int array[], int left, int right)
{
    if (left < right)
    {
        int pi = partition(array, left, right);

        qs(array, left, pi - 1);
        qs(array, pi + 1, right);
    }
}

void quickSort(int array[], int length)
{
    qs(array, 0, length - 1);
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

    quickSort(array, length);

    for (int i = 0; i < length; i++)
    {
        cout << array[i] << " ";
    }
    cout << endl;
}