#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <sstream>

#include "DynamicArray.h"
#include "SortSearch.h"

using namespace std;


// Read an integer safely
bool readInteger(const string& prompt, int& value) {
    string line;
    cout << prompt;

    if (!getline(cin, line)) {
        return false;
    }

    stringstream ss(line);

    if (!(ss >> value)) {
        cout << "Invalid input. Please enter a whole number.\n";
        return false;
    }

    ss >> ws;

    if (!ss.eof()) {
        cout << "Invalid input. Please enter a whole number.\n";
        return false;
    }

    return true;
}


// Read a decimal number safely
bool readDouble(const string& prompt, double& value) {
    string line;
    cout << prompt;

    if (!getline(cin, line)) {
        return false;
    }

    stringstream ss(line);

    if (!(ss >> value)) {
        cout << "Invalid input. Please enter a number.\n";
        return false;
    }

    ss >> ws;

    if (!ss.eof()) {
        cout << "Invalid input. Please enter a valid number.\n";
        return false;
    }

    return true;
}


// Read text safely
bool readText(const string& prompt, string& value) {
    cout << prompt;

    if (!getline(cin, value)) {
        return false;
    }

    if (value.empty()) {
        cout << "This field cannot be empty.\n";
        return false;
    }

    return true;
}


// Display one student
void displayStudent(const StudentResult& student) {
    cout << "\nStudent ID: " << student.id;
    cout << "\nFirst name: " << student.firstName;
    cout << "\nLast name: " << student.lastName;
    cout << "\nRegistration number: " << student.regNo;
    cout << "\nResult: " << student.result;
    cout << "\nGrade: " << student.grade;
    cout << "\n";
}


// Find a student using registration number
int findStudent(
    DynamicArray<StudentResult>& students,
    const string& regNo
) {
    for (int i = 0; i < students.size(); i++) {
        if (students.at(i).regNo == regNo) {
            return i;
        }
    }

    return -1;
}


// Display all students
void displayAllStudents(DynamicArray<StudentResult>& students) {
    if (students.empty()) {
        cout << "\nNo student records available.\n";
        return;
    }

    cout << "\n========== STUDENT RECORDS ==========\n";
    students.display();
}


// Add a student
void addStudent(DynamicArray<StudentResult>& students) {
    StudentResult student;

    cout << "\n========== ADD STUDENT ==========\n";

    if (!readInteger("Student ID: ", student.id)) {
        return;
    }

    if (student.id <= 0) {
        cout << "Student ID must be greater than zero.\n";
        return;
    }

    if (!readText("First name: ", student.firstName)) {
        return;
    }

    if (!readText("Last name: ", student.lastName)) {
        return;
    }

    if (!readText("Registration number: ", student.regNo)) {
        return;
    }

    // Check for duplicate student ID or registration number
    for (int i = 0; i < students.size(); i++) {
        if (students.at(i).id == student.id ||
            students.at(i).regNo == student.regNo) {
            cout << "A student with that ID or registration number already exists.\n";
            return;
        }
    }

    // IMPORTANT: Read result as a double
    if (!readDouble("Examination result (0-100): ", student.result)) {
        return;
    }

    if (student.result < 0 || student.result > 100) {
        cout << "Result must be between 0 and 100.\n";
        return;
    }

    student.grade = gradeFor(student.result);

    students.push_back(student);

    cout << "\nStudent added successfully.\n";
}


// Update a student
void updateStudent(DynamicArray<StudentResult>& students) {
    string regNo;

    cout << "\n========== UPDATE STUDENT ==========\n";

    if (!readText("Enter registration number: ", regNo)) {
        return;
    }

    int position = findStudent(students, regNo);

    if (position == -1) {
        cout << "Student not found.\n";
        return;
    }

    StudentResult student = students.at(position);

    cout << "\nCurrent student details:";
    displayStudent(student);

    cout << "\nEnter the new details.\n";

    if (!readText("New first name: ", student.firstName)) {
        return;
    }

    if (!readText("New last name: ", student.lastName)) {
        return;
    }

    if (!readDouble("New examination result (0-100): ", student.result)) {
        return;
    }

    if (student.result < 0 || student.result > 100) {
        cout << "Result must be between 0 and 100.\n";
        return;
    }

    student.grade = gradeFor(student.result);

    students.updateAt(position, student);

    cout << "\nStudent record updated successfully.\n";
}


// Remove a student
void removeStudent(DynamicArray<StudentResult>& students) {
    string regNo;

    cout << "\n========== REMOVE STUDENT ==========\n";

    if (!readText("Enter registration number: ", regNo)) {
        return;
    }

    int position = findStudent(students, regNo);

    if (position == -1) {
        cout << "Student not found.\n";
        return;
    }

    students.removeAt(position);

    cout << "\nStudent record removed successfully.\n";
}


// Sort by examination results using insertion sort
void sortByResults(DynamicArray<StudentResult>& students) {
    if (students.empty()) {
        cout << "\nNo records available to sort.\n";
        return;
    }

    insertionSort(students, ByResultDesc());

    cout << "\nRecords sorted by examination results (highest first).\n";
    students.display();
}


// Sort by registration number using randomized quick sort
void sortByRegistration(DynamicArray<StudentResult>& students) {
    if (students.empty()) {
        cout << "\nNo records available to sort.\n";
        return;
    }

    quickSort(students, ByRegNoAsc());

    cout << "\nRecords sorted by registration number.\n";
    students.display();
}


// Search using binary search
void searchStudent(DynamicArray<StudentResult>& students) {
    string regNo;

    cout << "\n========== SEARCH STUDENT ==========\n";

    if (!readText("Enter registration number: ", regNo)) {
        return;
    }

    if (students.empty()) {
        cout << "No student records available.\n";
        return;
    }

    // Make a copy so the original records are not reordered
    DynamicArray<StudentResult> sortedStudents = students;

    // Binary search requires records sorted by registration number
    quickSort(sortedStudents, ByRegNoAsc());

    int position = binarySearchByRegNo(sortedStudents, regNo);

    if (position != -1) {
        cout << "\nSTUDENT FOUND\n";
        displayStudent(sortedStudents.at(position));
    } else {
        cout << "\nStudent not found.\n";
    }
}


// Save records
void saveRecords(DynamicArray<StudentResult>& students) {
    if (students.saveToFile("results_updated.txt")) {
        cout << "\nRecords successfully saved to results_updated.txt\n";
    } else {
        cout << "\nError saving student records.\n";
    }
}


// Main application
int main() {
    srand(static_cast<unsigned int>(time(0)));

    DynamicArray<StudentResult> students;

    cout << "========================================\n";
    cout << "       STUDENT RESULTS REGISTER\n";
    cout << "========================================\n";

    // Load student records
    if (!students.loadFromFile("results.txt")) {
        cout << "\nCould not load results.txt\n";
        cout << "Check that the file exists in the program folder.\n";
        return 1;
    }

    cout << "\nRecords successfully loaded.\n";
    cout << "Number of students: " << students.size() << "\n";
    cout << "Array capacity: " << students.capacity() << "\n";

    int choice;

    do {
        cout << "\n\n========== MAIN MENU ==========\n";
        cout << "1. Display all student records\n";
        cout << "2. Add a student\n";
        cout << "3. Update a student\n";
        cout << "4. Remove a student\n";
        cout << "5. Sort by examination results (Insertion Sort)\n";
        cout << "6. Sort by registration number (Randomized Quick Sort)\n";
        cout << "7. Search student (Binary Search)\n";
        cout << "8. Save updated records\n";
        cout << "9. Exit\n";
        cout << "================================\n";

        if (!readInteger("Enter your choice: ", choice)) {
            if (!cin) {
                break;
            }
            continue;
        }

        switch (choice) {
            case 1:
                displayAllStudents(students);
                break;

            case 2:
                addStudent(students);
                break;

            case 3:
                updateStudent(students);
                break;

            case 4:
                removeStudent(students);
                break;

            case 5:
                sortByResults(students);
                break;

            case 6:
                sortByRegistration(students);
                break;

            case 7:
                searchStudent(students);
                break;

            case 8:
                saveRecords(students);
                break;

            case 9:
                cout << "\nExiting Student Results Register.\n";
                cout << "Remember to save your changes using option 8.\n";
                break;

            default:
                cout << "\nInvalid menu choice. Select 1 to 9.\n";
        }

    } while (choice != 9);

    return 0;
}

