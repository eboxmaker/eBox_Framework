
#ifndef __SORT_H
#define __SORT_H


template<typename T>
int partion(T arr[], int low, int high)
{
    int i = low - 1;
    int j = low;

    int x = arr[high];

    for (; j < high; j++)
    {
        if (arr[j] < x)
            swap(&arr[++i], &arr[j]);

    }
    swap(&arr[i + 1], &arr[high]);

    return i + 1;
}

template<typename T>
void quick_sort(T arr[], int low, int high)
{
    if (low >= high)
        return;
    int mid = partion(arr, low, high);
    quick_sort(arr, low, mid - 1);
    quick_sort(arr, mid + 1, high);
}
#endif
