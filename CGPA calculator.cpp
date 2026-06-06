#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

const int MAX = 10;

// --- Course Class -----------------------------------------------
class Course {
    string name;
    double grade, credits;
public:
    void input(int num) {
        cout << "\n  Course " << num << ":\n";
        cout << "  Name         : "; cin.ignore(); getline(cin, name);
        cout << "  Grade (0-4.0): "; cin >> grade;
        cout << "  Credit Hours : "; cin >> credits;
    }
    double getPoints()  const { return grade * credits; }
    double getCredits() const { return credits; }
    void display() const {
        cout << left << setw(20) << name << setw(8) << grade
             << setw(8) << credits << setw(10) << getPoints() << "\n";
    }
};

// --- Semester Class ----------------------------------------------
class Semester {
    Course courses[MAX];
    int count;
public:
    void input(int semNum) {
        cout << "\nSemester " << semNum << " - Number of Courses: "; cin >> count;
        for (int i = 0; i < count; i++)
            courses[i].input(i + 1);
    }
    double totalCredits() const {
        double t = 0; for (int i = 0; i < count; i++) t += courses[i].getCredits(); return t;
    }
    double totalPoints() const {
        double t = 0; for (int i = 0; i < count; i++) t += courses[i].getPoints(); return t;
    }
    double GPA() const { return totalPoints() / totalCredits(); }
    void display(int sem) const {
        cout << "\n--- Semester " << sem << " Results ---\n";
        cout << left << setw(20) << "Course" << setw(8) << "Grade" << setw(8) << "Credits" << "Points\n";
        cout << string(46, '-') << "\n";
        for (int i = 0; i < count; i++) courses[i].display();
        cout << string(46, '-') << "\n";
        cout << "  GPA: " << fixed << setprecision(2) << GPA() << "\n";
    }
};

// --- Student Class -----------------------------------------------
class Student {
    string name;
    Semester semesters[MAX];
    int count;
public:
    void input() {
        cout << "Student Name       : "; getline(cin, name);
        cout << "Number of Semesters: "; cin >> count;
        for (int i = 0; i < count; i++)
            semesters[i].input(i + 1);
    }
    void displayReport() const {
        double totCredits = 0, totPoints = 0;
        for (int i = 0; i < count; i++) {
            semesters[i].display(i + 1);
            totCredits += semesters[i].totalCredits();
            totPoints  += semesters[i].totalPoints();
        }
        cout << "\n========== CGPA Report: " << name << " ==========\n";
        cout << "  Total Credits : " << totCredits << "\n";
        cout << "  Total Points  : " << totPoints  << "\n";
        cout << "  CGPA          : " << fixed << setprecision(2) << totPoints / totCredits << " / 4.00\n";
        cout << string(46, '=') << "\n";
    }
};

// --- Main --------------------------------------------------------
int main() {
    cout << "===== CGPA Calculator =====\n";
    Student s;
    s.input();
    s.displayReport();
}
