#include "ebox_core.h"
#include "sort.h"

int partion(int arr[], int low, int high)
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
void quick_sort(int arr[], int low, int high)
{
    if (low >= high)
        return;
    int mid = partion(arr, low, high);
    quick_sort(arr, low, mid - 1);
    quick_sort(arr, mid + 1, high);
}
int partion(float arr[], int low, int high)
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
void quick_sort(float arr[], int low, int high)
{
    if (low >= high)
        return;
    int mid = partion(arr, low, high);
    quick_sort(arr, low, mid - 1);
    quick_sort(arr, mid + 1, high);
}

