
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <utility>

using namespace std;

// Student record used throughout the project
struct StudentResult {
    int id;
    string firstName;
    string lastName;
    string regNo;
    double result;
    string grade;
};

// Determine grade from examination mark
inline string gradeFor(double mark) {
    if (mark >= 80) return "A";
    if (mark >= 70) return "B";
    if (mark >= 60) return "C";
    if (mark >= 50) return "D";
    return "F";
}

template <typename T>
class DynamicArray {
private:
    T* data;
    int currentSize;
    int currentCapacity;

    void resize(int newCapacity) {
        T* newData = new T[newCapacity];

        for (int i = 0; i < currentSize; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        currentCapacity = newCapacity;
    }

public:
    // Constructor
    DynamicArray() {
        currentSize = 0;
        currentCapacity = 2;
        data = new T[currentCapacity];
    }

    // Deep-copy constructor
    DynamicArray(const DynamicArray<T>& other) {
        currentSize = other.currentSize;
        currentCapacity = other.currentCapacity;
        data = new T[currentCapacity];

        for (int i = 0; i < currentSize; i++) {
            data[i] = other.data[i];
        }
    }

    // Copy assignment operator
    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this != &other) {
            T* newData = new T[other.currentCapacity];

            for (int i = 0; i < other.currentSize; i++) {
                newData[i] = other.data[i];
            }

            delete[] data;
            data = newData;
            currentSize = other.currentSize;
            currentCapacity = other.currentCapacity;
        }

        return *this;
    }

    // Destructor
    ~DynamicArray() {
        delete[] data;
    }

    int size() const {
        return currentSize;
    }

    int capacity() const {
        return currentCapacity;
    }

    bool empty() const {
        return currentSize == 0;
    }

    // Add a record; double capacity when full
    bool push_back(const T& value) {
        if (currentSize == currentCapacity) {
            resize(currentCapacity * 2);
        }

        data[currentSize] = value;
        currentSize++;
        return true;
    }

    // Safe access with bounds checking
    T& at(int index) {
        if (index < 0 || index >= currentSize) {
            throw out_of_range("Invalid array index");
        }

        return data[index];
    }

    const T& at(int index) const {
        if (index < 0 || index >= currentSize) {
            throw out_of_range("Invalid array index");
        }

        return data[index];
    }

    // Update a record at a given position
    bool updateAt(int index, const T& value) {
        if (index < 0 || index >= currentSize) {
            return false;
        }

        data[index] = value;
        return true;
    }

    // Remove a record at a given position
    bool removeAt(int index) {
        if (index < 0 || index >= currentSize) {
            return false;
        }

        for (int i = index; i < currentSize - 1; i++) {
            data[i] = data[i + 1];
        }

        currentSize--;
        return true;
    }

    bool removeFirst() {
        if (empty()) return false;
        return removeAt(0);
    }

    bool removeLast() {
        if (empty()) return false;
        currentSize--;
        return true;
    }

    void clear() {
        currentSize = 0;
    }

    // Display all records
    void display() const {
        if (empty()) {
            cout << "No student records available.\n";
            return;
        }

        cout << "\nID\tRegistration\tName\t\tResult\tGrade\n";
        cout << "-------------------------------------------------------------\n";

        for (int i = 0; i < currentSize; i++) {
            cout << data[i].id << "\t"
                 << data[i].regNo << "\t"
                 << data[i].firstName << " "
                 << data[i].lastName << "\t"
                 << data[i].result << "\t"
                 << data[i].grade << "\n";
        }
    }

    // Load pipe-separated records from a text file.
    // Existing records are replaced only if the whole file is valid.
    bool loadFromFile(const string& filename) {
        ifstream file(filename.c_str());

        if (!file.is_open()) {
            return false;
        }

        DynamicArray<T> temporary;
        string line;

        try {
            while (getline(file, line)) {
                if (line.empty()) continue;

                stringstream ss(line);
                string idText, first, last, reg, resultText, grade;
                string extra;

                if (!getline(ss, idText, '|') ||
                    !getline(ss, first, '|') ||
                    !getline(ss, last, '|') ||
                    !getline(ss, reg, '|') ||
                    !getline(ss, resultText, '|') ||
                    !getline(ss, grade, '|')) {
                    return false;
                }

                if (getline(ss, extra, '|')) {
                    return false;
                }

                T student;
                stringstream idStream(idText);
                stringstream resultStream(resultText);

                if (!(idStream >> student.id) ||
                    !(resultStream >> student.result)) {
                    return false;
                }

                student.firstName = first;
                student.lastName = last;
                student.regNo = reg;
                student.grade = grade;

                if (student.firstName.empty() ||
                    student.lastName.empty() ||
                    student.regNo.empty() ||
                    student.result < 0 ||
                    student.result > 100) {
                    return false;
                }

                temporary.push_back(student);
            }
        } catch (...) {
            return false;
        }

        if (file.bad()) return false;

        *this = temporary;
        return true;
    }

    // Save records to a pipe-separated text file
    bool saveToFile(const string& filename) const {
        ofstream file(filename.c_str());

        if (!file.is_open()) return false;

        for (int i = 0; i < currentSize; i++) {
            file << data[i].id << "|"
                 << data[i].firstName << "|"
                 << data[i].lastName << "|"
                 << data[i].regNo << "|"
                 << data[i].result << "|"
                 << data[i].grade << "\n";
        }

        return file.good();
    }
};

#endif

