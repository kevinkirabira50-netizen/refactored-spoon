# STUDENT RESULTS REGISTER

### 1. Project Description

The Student Results Register is a C++ application designed to manage student academic records. It allows users to add, display, update, delete, sort, search and save student results.

The application uses a Dynamic Array as its primary linear data structure. Its capacity automatically doubles when the array becomes full.

### 2. Project Files

- `main.cpp` � Main application and interactive menu.
- `DynamicArray.h` � Student record structure and Dynamic Array implementation.
- `SortSearch.h` � Sorting and searching algorithms.
- `tests.cpp` � Automated functional tests.
- `benchmark.cpp` � Performance benchmarking program.
- `results.txt` � Initial student records.
- `results_updated.txt` � Saved and updated student records.
- `benchmark_results.csv` � Benchmark output, when generated.

### 3. Main Features

1. Display all student records.
2. Add a new student.
3. Update an existing student.
4. Remove a student.
5. Sort students by examination results using Insertion Sort.
6. Sort students by registration number using Randomized Quick Sort.
7. Search for a student using Iterative Binary Search.
8. Save updated records to a text file.
9. Exit the application.

### 4. Data Structure � A1

The project implements a Dynamic Array using a C++ class template.

Features include:

- Automatic capacity doubling.
- Insertion and removal of records.
- Updating and accessing records.
- Dynamic memory management.
- Deep-copy support.
- File loading and saving.
- Validation of invalid operations.

### 5. Sorting and Searching � A2

The application implements:

- Insertion Sort: sorts students by examination results in descending order.
- Randomized-Pivot Quick Sort: sorts students by registration number.
- Iterative Binary Search: searches for a registration number in sorted records.

The sorting algorithms use comparators to determine record ordering.

### 6. Testing � A3

The project includes automated tests for:

- Empty arrays.
- Single records.
- Duplicate keys.
- Removing first and last records.
- Sorting records.
- Searching for existing and missing records.
- Dynamic array capacity growth.
- File loading and saving.

Memory-safety testing should be conducted using AddressSanitizer or Valgrind.

### 7. Performance Analysis � A4

The benchmark program uses the C++ `<chrono>` library to measure sorting performance.

It is designed to test increasing record counts:

- 1,000
- 2,000
- 4,000
- 8,000
- 16,000
- 32,000
- 64,000

Each benchmark size should be tested three times and averaged. Results can be exported to CSV for analysis and graph preparation.

### 8. Compilation and Execution

Using GCC:

Compile the main application:

`g++ -std=c++11 -Wall -Wextra main.cpp -o main`

Run:

`./main`

Compile and run automated tests:

`g++ -std=c++11 -Wall -Wextra tests.cpp -o tests`

`./tests`

Compile and run the benchmark:

`g++ -std=c++11 -O2 benchmark.cpp -o benchmark`

`./benchmark`

On Windows, run the generated executable using `main.exe`, `tests.exe` or `benchmark.exe`.

### 9. Important Instructions

- Keep all header files and `results.txt` in the project directory.
- Run the application from the project directory so it can find `results.txt`.
- Use menu option 8 to save changes.
- Updated records are saved in `results_updated.txt`.
- Run tests and benchmarks separately from the main application.
- Keep screenshots and benchmark results as project evidence.

### 10. Conclusion

The Student Results Register demonstrates the practical application of dynamic arrays, memory management, sorting algorithms, binary searching, file handling, automated testing and performance analysis in C++.

**Project:** Student Results Register
**Primary Data Structure:** Dynamic Array
**Programming Language:** C++

