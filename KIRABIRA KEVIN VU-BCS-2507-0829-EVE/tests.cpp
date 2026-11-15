#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include "DynamicArray.h"
#include "SortSearch.h"

using namespace std;

int passed = 0;
int failed = 0;

void check(bool condition, const string& testName) {
    if (condition) {
        cout << "[PASS] " << testName << "\n";
        passed++;
    } else {
        cout << "[FAIL] " << testName << "\n";
        failed++;
    }
}

StudentResult makeStudent(int id, string name,
                          string reg, double mark) {
    StudentResult s;
    s.id = id;
    s.firstName = name;
    s.lastName = "Student";
    s.regNo = reg;
    s.result = mark;
    s.grade = gradeFor(mark);
    return s;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    cout << "=====================================\n";
    cout << " STUDENT RESULTS REGISTER TESTS\n";
    cout << "=====================================\n\n";

    // Test 1: Empty array
    DynamicArray<StudentResult> empty;
    check(empty.empty(), "Empty array starts with zero records");
    check(!empty.removeFirst(), "Cannot remove first from empty array");
    check(!empty.removeLast(), "Cannot remove last from empty array");

    // Test 2: One record
    DynamicArray<StudentResult> one;
    one.push_back(makeStudent(1, "John", "REG001", 75));
    check(one.size() == 1, "One record can be inserted");
    check(one.at(0).result == 75, "One record can be accessed");

    // Test 3: Invalid index
    bool invalidIndexCaught = false;
    try {
        one.at(10);
    } catch (const out_of_range&) {
        invalidIndexCaught = true;
    }
    check(invalidIndexCaught, "Invalid index is safely rejected");

    // Test 4: Invalid update
    check(!one.updateAt(10,
          makeStudent(2, "Mary", "REG002", 80)),
          "Invalid update is rejected");

    // Test 5: Duplicate marks and insertion sort stability
    DynamicArray<StudentResult> duplicates;
    duplicates.push_back(makeStudent(1, "First", "REG001", 80));
    duplicates.push_back(makeStudent(2, "Second", "REG002", 80));
    duplicates.push_back(makeStudent(3, "Third", "REG003", 90));

    insertionSort(duplicates, ByResultDesc());

    check(duplicates.at(0).firstName == "Third",
          "Insertion sort orders highest mark first");

    check(duplicates.at(1).firstName == "First" &&
          duplicates.at(2).firstName == "Second",
          "Insertion sort preserves order of equal marks");

    // Test 6: Remove first and last
    DynamicArray<StudentResult> removal;
    removal.push_back(makeStudent(1, "A", "R1", 50));
    removal.push_back(makeStudent(2, "B", "R2", 60));
    removal.push_back(makeStudent(3, "C", "R3", 70));

    bool firstRemoved = removal.removeFirst();
    bool lastRemoved = removal.removeLast();

    check(firstRemoved && removal.size() == 1 &&
          removal.at(0).firstName == "B",
          "Remove first and last records");

    // Test 7: Remove invalid position
    check(!removal.removeAt(10),
          "Invalid removal position is rejected");

    // Test 8: Already sorted input
    DynamicArray<StudentResult> sorted;
    sorted.push_back(makeStudent(1, "A", "R1", 90));
    sorted.push_back(makeStudent(2, "B", "R2", 80));
    sorted.push_back(makeStudent(3, "C", "R3", 70));

    insertionSort(sorted, ByResultDesc());

    check(sorted.at(0).result == 90 &&
          sorted.at(2).result == 70,
          "Already sorted records remain correctly sorted");

    // Test 9: Reversed input using quicksort
    DynamicArray<StudentResult> reversed;
    reversed.push_back(makeStudent(1, "A", "R1", 30));
    reversed.push_back(makeStudent(2, "B", "R2", 60));
    reversed.push_back(makeStudent(3, "C", "R3", 90));

    quickSort(reversed, ByResultDesc());

    check(reversed.at(0).result == 90 &&
          reversed.at(1).result == 60 &&
          reversed.at(2).result == 30,
          "Quicksort orders reversed records");

    // Test 10: Binary search
    DynamicArray<StudentResult> searchData;
    searchData.push_back(makeStudent(1, "A", "REG001", 60));
    searchData.push_back(makeStudent(2, "B", "REG002", 70));
    searchData.push_back(makeStudent(3, "C", "REG003", 80));

    quickSort(searchData, ByRegNoAsc());

    check(binarySearchByRegNo(searchData, "REG002") == 1,
          "Binary search finds an existing registration number");

    check(binarySearchByRegNo(searchData, "REG999") == -1,
          "Binary search rejects an absent registration number");

    // Test 11: Deep-copy constructor
    DynamicArray<StudentResult> original;
    original.push_back(makeStudent(1, "Original", "REG100", 80));

    DynamicArray<StudentResult> copied(original);
    copied.at(0).result = 20;

    check(original.at(0).result == 80 &&
          copied.at(0).result == 20,
          "Copy constructor creates an independent copy");

    // Test 12: Copy assignment
    DynamicArray<StudentResult> assigned;
    assigned = original;
    assigned.at(0).result = 10;

    check(original.at(0).result == 80 &&
          assigned.at(0).result == 10,
          "Assignment operator creates an independent copy");

    // Test 13: Capacity doubling
    DynamicArray<StudentResult> growing;

    int initialCapacity = growing.capacity();

 for (int i = 0; i < initialCapacity + 1; i++) {
    stringstream ss;
    ss << i;

    growing.push_back(
        makeStudent(i, "Test", "GROW" + ss.str(), 50)
    );
}

check(growing.capacity() >= initialCapacity * 2,
      "Array capacity doubles when full");

    // Test 14: File loading
    DynamicArray<StudentResult> loaded;
    bool loadOK = loaded.loadFromFile("results.txt");

    check(loadOK && loaded.size() >= 10,
          "At least 10 records load from the file");

    // Test 15: File saving
    bool saveOK = loaded.saveToFile("test_saved.txt");

    check(saveOK, "Records save to a text file");

    cout << "\n=====================================\n";
    cout << "TEST SUMMARY\n";
    cout << "=====================================\n";
    cout << "Passed: " << passed << "\n";
    cout << "Failed: " << failed << "\n";
    cout << "Total:  " << passed + failed << "\n";

    if (failed == 0) {
        cout << "\nALL TESTS PASSED.\n";
    } else {
        cout << "\nSOME TESTS FAILED. Investigate the failures.\n";
    }

    return failed == 0 ? 0 : 1;
}

