
#ifndef SORT_SEARCH_H
#define SORT_SEARCH_H

#include "DynamicArray.h"
#include <cstdlib>
#include <ctime>
#include <string>

// Sort by examination result, highest first
struct ByResultDesc {
    bool operator()(const StudentResult& a,
                    const StudentResult& b) const {
        return a.result > b.result;
    }
};

// Sort by registration number, ascending
struct ByRegNoAsc {
    bool operator()(const StudentResult& a,
                    const StudentResult& b) const {
        return a.regNo < b.regNo;
    }
};

// Compare a student record with a registration-number search key
struct RegNoKeyCompare {
    bool operator()(const StudentResult& a,
                    const string& key) const {
        return a.regNo < key;
    }
};

// Stable insertion sort
template <typename T, typename Compare>
void insertionSort(DynamicArray<T>& arr, Compare comp) {
    for (int i = 1; i < arr.size(); i++) {
        T key = arr.at(i);
        int j = i - 1;

        while (j >= 0 && comp(key, arr.at(j))) {
            arr.at(j + 1) = arr.at(j);
            j--;
        }

        arr.at(j + 1) = key;
    }
}

// Randomized-pivot quicksort partition
template <typename T, typename Compare>
int randomizedPartition(DynamicArray<T>& arr,
                        int low, int high, Compare comp) {
    int pivotIndex = low + rand() % (high - low + 1);

    T temp = arr.at(pivotIndex);
    arr.at(pivotIndex) = arr.at(high);
    arr.at(high) = temp;

    T pivot = arr.at(high);
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (comp(arr.at(j), pivot)) {
            i++;

            T swapValue = arr.at(i);
            arr.at(i) = arr.at(j);
            arr.at(j) = swapValue;
        }
    }

    T swapValue = arr.at(i + 1);
    arr.at(i + 1) = arr.at(high);
    arr.at(high) = swapValue;

    return i + 1;
}

// Randomized-pivot quicksort
template <typename T, typename Compare>
void quickSortHelper(DynamicArray<T>& arr,
                     int low, int high, Compare comp) {
    while (low < high) {
        int pivot = randomizedPartition(arr, low, high, comp);

        // Recurse on the smaller partition to limit stack depth
        if (pivot - low < high - pivot) {
            quickSortHelper(arr, low, pivot - 1, comp);
            low = pivot + 1;
        } else {
            quickSortHelper(arr, pivot + 1, high, comp);
            high = pivot - 1;
        }
    }
}

template <typename T, typename Compare>
void quickSort(DynamicArray<T>& arr, Compare comp) {
    if (arr.size() > 1) {
        quickSortHelper(arr, 0, arr.size() - 1, comp);
    }
}

// Iterative binary search.
// Iterative binary search for registration numbers
inline int binarySearchByRegNo(
    const DynamicArray<StudentResult>& arr,
    const string& searchReg) {

    int low = 0;
    int high = arr.size() - 1;

    while (low <= high) {
        int middle = low + (high - low) / 2;

        if (arr.at(middle).regNo == searchReg) {
            return middle;
        }

        if (arr.at(middle).regNo < searchReg) {
            low = middle + 1;
        } else {
            high = middle - 1;
        }
    }

    return -1;
}

#endif


