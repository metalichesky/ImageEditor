//
// Created by dmitriy on 14.03.2020.
//

#ifndef IMAGEANALYSER_QUICKSORTER_H
#define IMAGEANALYSER_QUICKSORTER_H


#include "Comparator.h"
#include "Sorter.h"

template <class T>
class QuickSorter : public Sorter<T>{
public:
    explicit QuickSorter() : Sorter<T>(){};

    void sort(T *arr, int size) {
        quickSort(arr, 0, size-1);
    }

private:

    /* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
    int partition (T *arr, int low, int high)
    {
        T pivot = arr[high];    // pivot
        int i = (low - 1);  // Index of smaller element

        for (int j = low; j < high; j++)
        {
            // If current element is smaller than or
            // equal to pivot
            if (arr[j] < pivot || arr[j] == pivot)
            {
                i++;    // increment index of smaller element
                swap(&arr[i], &arr[j]);
            }
        }
        swap(&arr[i + 1], &arr[high]);
        return (i + 1);
    }

    void quickSort(T *arr, int low, int high)
    {
        if (low < high)
        {
            /* pi is partitioning index, arr[p] is now
               at right place */
            int pi = partition(arr, low, high);

            // Separately sort elements before
            // partition and after partition
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    // A utility function to swap two elements
    void swap(T* a, T* b)
    {
        T t = *a;
        *a = *b;
        *b = t;
    }
};


#endif //IMAGEANALYSER_QUICKSORTER_H
