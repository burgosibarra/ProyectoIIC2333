#include "mergeSort.h"


/* Código inspirado en https://www.geeksforgeeks.org/merge-sort/ */
void mergeArray(int* arr, int* sized, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
  
    int* L = calloc(n1, sizeof(int));
    int* R = calloc(n2, sizeof(int));
    int* L2 = calloc(n1, sizeof(int));
    int* R2 = calloc(n2, sizeof(int));
  
    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[l + i];
        L2[i] = sized[l + i];
    }

    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[m + 1 + j];
        R2[j] = sized[m + 1 + j];
    }
  
    int i = 0;
    int j = 0;
    int k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            sized[k] = L2[i];
            i++;
        }
        else {
            arr[k] = R[j];
            sized[k] = R2[j];
            j++;
        }
        k++;
    }
  
    while (i < n1) {
        arr[k] = L[i];
        sized[k] = L2[i];
        i++;
        k++;
    }
  
    while (j < n2) {
        arr[k] = R[j];
        sized[k] = R2[j];
        j++;
        k++;
    }

    free(L);
    free(R);
    free(L2);
    free(R2);

}

/* Código inspirado en https://www.geeksforgeeks.org/merge-sort/ */
void mergeSort(int* arr, int* sized, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;
  
        mergeSort(arr, sized, l, m);
        mergeSort(arr, sized, m + 1, r);
  
        mergeArray(arr, sized, l, m, r);
    }
}
