#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <string>

#include "DynamicArray.h"
#include "SortSearch.h"

using namespace std;
using namespace chrono;

// Create random student records
vector<StudentResult> makeData(int n) {
    vector<StudentResult> records;
    records.reserve(n);

    for (int i = 0; i < n; i++) {
        StudentResult s;

        s.id = i + 1;
        s.firstName = "Student";
        s.lastName = "Test";
        s.regNo = "REG" + to_string(100000 + i);

        s.result = rand() % 101;
        s.grade = gradeFor(s.result);

        records.push_back(s);
    }

    return records;
}

// Copy records into the dynamic array
DynamicArray<StudentResult> makeDynamicArray(
    const vector<StudentResult>& records) {

    DynamicArray<StudentResult> arr;

    for (size_t i = 0; i < records.size(); i++) {
        arr.push_back(records[i]);
    }

    return arr;
}

// Measure one algorithm over three trials
double measureSort(int n, bool useQuickSort) {
    double totalMilliseconds = 0.0;

    for (int trial = 0; trial < 3; trial++) {
        vector<StudentResult> source = makeData(n);

        // Build the array before starting the timer
        DynamicArray<StudentResult> arr =
            makeDynamicArray(source);

        steady_clock::time_point start = steady_clock::now();

        if (useQuickSort) {
            quickSort(arr, ByResultDesc());
        } else {
            insertionSort(arr, ByResultDesc());
        }

        steady_clock::time_point finish = steady_clock::now();

        double elapsed = duration<double, milli>(
            finish - start
        ).count();

        totalMilliseconds += elapsed;

        // Verify that the result is sorted
        bool correct = true;

        for (int i = 1; i < arr.size(); i++) {
            if (arr.at(i - 1).result < arr.at(i).result) {
                correct = false;
                break;
            }
        }

        if (!correct) {
            cout << "Sorting verification failed.\n";
            return -1.0;
        }
    }

    return totalMilliseconds / 3.0;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    int sizes[] = {
        1000, 2000, 4000, 8000,
        16000, 32000, 64000
    };

    ofstream csv("benchmark_results.csv");

    if (!csv.is_open()) {
        cout << "Could not create benchmark_results.csv\n";
        return 1;
    }

    csv << "Records,Insertion_ms,QuickSort_ms,"
        << "Insertion_ratio,QuickSort_ratio\n";

    cout << fixed << setprecision(3);
    cout << "\nSTUDENT RESULTS REGISTER BENCHMARK\n";
    cout << "Average of three runs; time in milliseconds\n\n";

    cout << left
         << setw(12) << "Records"
         << setw(20) << "Insertion Sort"
         << setw(20) << "Quick Sort" << "\n";

    cout << "--------------------------------------------------\n";

    double previousInsertion = 0.0;
    double previousQuick = 0.0;

    for (int i = 0; i < 7; i++) {
        int n = sizes[i];

        cout << "\nTesting " << n << " records...\n";

        double insertionTime = measureSort(n, false);
        double quickTime = measureSort(n, true);

        if (insertionTime < 0 || quickTime < 0) {
            cout << "Benchmark stopped due to a sorting error.\n";
            csv.close();
            return 1;
        }

        double insertionRatio = 0.0;
        double quickRatio = 0.0;

        if (i > 0) {
            insertionRatio = insertionTime / previousInsertion;
            quickRatio = quickTime / previousQuick;
        }

        cout << left
             << setw(12) << n
             << setw(20) << insertionTime
             << setw(20) << quickTime << "\n";

        csv << n << ","
            << insertionTime << ","
            << quickTime << ","
            << insertionRatio << ","
            << quickRatio << "\n";

        previousInsertion = insertionTime;
        previousQuick = quickTime;
    }

    csv.close();

    cout << "\nBenchmark completed.\n";
    cout << "Results saved in benchmark_results.csv\n";

    return 0;
}

