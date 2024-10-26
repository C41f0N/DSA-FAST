#include <iostream>

using namespace std;

void merge(int array[], int left, int mid, int right)
{
    int sub1[mid - left + 1], sub2[right - mid];

    // Copy values
    for (int i = 0; i < mid - left + 1; i++)
    {
        sub1[i] = array[left + i];
    }

    for (int i = 0; i < right - mid; i++)
    {
        sub2[i] = array[mid + i + 1];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < mid - left + 1 && j < right - mid)
    {
        if (sub1[i] <= sub2[j])
        {
            array[k] = sub1[i];
            i++;
        }
        else if (sub1[i] > sub2[j])
        {
            array[k] = sub2[j];
            j++;
        }

        k++;
    }

    while (j < right - mid)
    {
        array[k] = sub2[j];
        j++;
        k++;
    }

    while (i < mid - left + 1)
    {
        array[k] = sub1[i];
        i++;
        k++;
    }
}

void mergeSort(int array[], int left, int right)
{
    if (left >= right)
    {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSort(array, left, mid);
    mergeSort(array, mid + 1, right);
    merge(array, left, mid, right);
}

int main()
{
    int array[8] = {
        23,
        34,
        83,
        54,
        63,
        14,
        23,
        35,
    };

    mergeSort(array, 0, 8);

    for (int i = 0; i < 8; i++)
    {
        cout << array[i] << ", ";
    }
}