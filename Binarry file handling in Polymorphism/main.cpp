#include <iostream>
#include <fstream>
#include <cstring>  
using namespace std;

const int MAX{ 100 };
const int max_n{ 30 };
const int max_cnic{ 13 };
class Person {
protected:
    char name[max_n];
    char cnic[max_cnic];

public:
    Person(const char* n = "", const char* c = "");
    virtual void setter();
    virtual void getter();
    virtual void isoutstanding() = 0;
    virtual ~Person();
};

Person::Person(const char* n, const char* c) {
    strncpy_s(name,sizeof(name),  n, max_n - 1);
    name[max_n - 1] = '\0';  // Ensure null-terminated
    strncpy_s(cnic,sizeof(cnic),  c, max_cnic - 1);
    cnic[max_cnic - 1] = '\0';  // Ensure null-terminated
}

void Person::setter() {
    cout << "Enter name: ";
    cin.getline(name, max_n);
    cout << "Enter cnic: ";
    cin.getline(cnic, max_cnic);
}

void Person::getter() {
    cout << "Name: " << name << "  cnic: " << cnic << endl;
}

Person::~Person() {
}

class Student : virtual public Person {
    char degree_program[max_n];
    float cgpa;

public:
    Student(const char* n = "", const char* c = "", const char* d = "", float g = 0.0);
    void setter();
    void getter();
    void isoutstanding() override;
};

Student::Student(const char* n, const char* c, const char* d, float g)
    : Person(n, c), cgpa(g) {
    strncpy_s(degree_program,sizeof(degree_program),  d, max_n - 1);
    degree_program[max_n - 1] = '\0';  // Ensure null-terminated
}

void Student::setter() {
    Person::setter();
    cout << "Enter degree Program: ";
    cin.getline(degree_program, max_n);
    cout << "Enter cgpa: ";
    cin >> cgpa;
}

void Student::getter() {
    cout << "*Student*" << endl;
    Person::getter();
    cout << "Degree Program: " << degree_program << " cgpa: " << cgpa << endl;
}

void Student::isoutstanding() {
    if (cgpa > 3.0) {
        cout << "*Outstanding*" << endl;
    }
}

class Teacher : virtual public Person {
    char department[max_n];
    int no_of_publication;

public:
    Teacher(const char* n = "", const char* c = "", const char* d = "", int no = 0);
    void setter();
    void getter();
    void isoutstanding() override;
};

Teacher::Teacher(const char* n, const char* c, const char* d, int no)
    : Person(n, c), no_of_publication(no) {
    strncpy_s(department,sizeof(department),  d, max_n - 1);
    department[max_n - 1] = '\0';  // Ensure null-terminated
}

void Teacher::setter() {
    Person::setter();
    cout << "Enter Department: ";
    cin.getline(department, max_n);
    cout << "NO of Publication: ";
    cin >> no_of_publication;
}

void Teacher::getter() {
    cout << "*Teacher*" << endl;
    Person::getter();
    cout << "Department: " << department << "  no of Publication: " << no_of_publication << endl;
}

void Teacher::isoutstanding() {
    if (no_of_publication > 10) {
        cout << "*Outstanding*" << endl;
    }
}
void create_Teacher(Person* (&arr)[MAX], int& n) {
    arr[n] = new Teacher;
    arr[n++]->setter();
}
void create_Student(Person* (&arr)[MAX], int& n) {
    arr[n] = new Student;
    arr[n++]->setter();
}
void display(Person* (&arr)[MAX], int n) {
    for (int i = 0; i < n; i++) {
        arr[i]->getter();
        arr[i]->isoutstanding();
    }
}
void write2file(Person* (&arr)[MAX], int n) {
    ofstream outfile;
    outfile.open("abc.DAT", ios::out | ios::binary);
    if (!outfile) {
        cout << "Error opening file" << endl;
        exit(-1);
    }
    char ch{};
    outfile.write(reinterpret_cast<char*>(&n), sizeof(n));
    for (int i = 0; i < n; i++) {
        ch = (typeid(*arr[i]) == typeid(Student)) ? 'S' : 'T';
        outfile.write(reinterpret_cast<char*>(&ch), sizeof(ch));
        if (ch == 'T') {
            outfile.write(reinterpret_cast<char*>(dynamic_cast<Teacher*>(arr[i])), sizeof(Teacher));
        }
        else if (ch == 'S') {
            outfile.write(reinterpret_cast<char*>(dynamic_cast<Student*>(arr[i])), sizeof(Student));
        }
    }
    outfile.close();
}
void read2file(Person* (&arr)[MAX], int& n , int& nt, int &ns) {
    ifstream infile;
    infile.open("abc.DAT", ios::in | ios::binary);
    if (!infile) {
        cout << "Error opening File" << endl;
        exit(-1);
    }
    char ch{};
    infile.read(reinterpret_cast<char*>(&n), sizeof(int));
    for (int i = 0; i < n; i++) {
        infile.read(reinterpret_cast<char*>(&ch), sizeof(char));
        if (ch == 'T') {
            if (arr[i] == nullptr)
                arr[i] = new Teacher;
            infile.read(reinterpret_cast<char*>(dynamic_cast<Teacher*>(arr[i])), sizeof(Teacher));
            nt++;
        }
        else if (ch == 'S') {
            if (arr[i] == nullptr)
                arr[i] = new Student;
            infile.read(reinterpret_cast<char*>(dynamic_cast<Student*>(arr[i])), sizeof(Student));
            ns++;
        }
    }
}
int main() {
    int n{}, nt{}, ns{};
    char choice{};
    Person* arr[MAX]{};
    while (true) {
        cout << "Menu\n"
            << "1: Create Teacher\n"
            << "2: Create Student\n"
            << "3: Display All\n"
            << "4: Display number\n"
            << "5: Write Data to Binary file\n"
            << "6: Read Data from Binary file\n"
            << "7: Exit" << endl;
        cout << "Which option you want to choose: ";
        cin >> choice;
        cin.ignore(20, '\n');
        switch (choice) {
        case '1': // Create a Teacher
            create_Teacher(arr, n);
            nt++;
            break;
        case '2': // Create a Student
            create_Student(arr, n);
            ns++;
            break;
        case '3':  // Display all
            display(arr, n);
            break;
        case '4':
            cout << "Number of teachers: " << nt << endl;
            cout << "Number of Students: " << ns << endl;
            break;
        case '5':
            write2file(arr, n);
            break;
        case '6': {
            read2file(arr, n , nt, ns);
            break;
        }
        case '7':
            for (int i = 0; i < n; i++) {
                delete arr[i];
            }
            return 0;
        default:
            cout << "You entered the wrong choice" << endl;
            break;
        }
    }
    return 0;
}