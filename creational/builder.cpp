#include <bits/stdc++.h>
using namespace std;

class StudentBuilder;

class Student{
    private:
        string name;
        int rollNumber;
        vector<string> subjects;
        string fatherName;
        string motherName;
    public:
        Student(StudentBuilder* builder);
        void display();
};

class StudentBuilder {
    public:
    string name;
    int rollNumber;
    vector<string> subjects;
    string fatherName;
    string motherName;
    StudentBuilder& setName(const string& name) {
            this->name = name;
            return *this;
        }

        StudentBuilder& setRollNumber(int rollNumber) {
            this->rollNumber = rollNumber;
            return *this;
        }

        virtual StudentBuilder& setSubjects() = 0;
        
        StudentBuilder& setFatherName(const string& fatherName) {
            this->fatherName = fatherName;
            return *this;
        }
        
        StudentBuilder& setMotherName(const string& motherName) {
            this->motherName = motherName;
            return *this;
        }
        
        Student build() {
           return Student(this);
        }
    };

    Student::Student(StudentBuilder* builder) {
        name = builder->name;
        rollNumber = builder->rollNumber;
        subjects = builder->subjects;
        fatherName = builder->fatherName;
        motherName = builder->motherName;
    }

    void Student::display() {
        cout << "Name: " << name << endl;
        cout << "Roll Number: " << rollNumber << endl;
        cout << "Subjects: ";
        for (const auto& subject : subjects) {
            cout << subject << " ";
        }
        cout << endl;
        cout << "Father's Name: " << fatherName << endl;
        cout << "Mother's Name: " << motherName << endl;
    }
    
    class EngineeringStudentBuilder : public StudentBuilder {
        public:
            StudentBuilder& setSubjects() override {
                vector<string> subjects = {"Mathematics", "Physics", "Chemistry"};
                this->subjects = subjects;
                return *this;
            }
    };

    class MBAStudentBuilder : public StudentBuilder {
        public:
            StudentBuilder& setSubjects() override {
                vector<string> subjects = {"Business Administration", "Marketing", "Finance"};
                this->subjects = subjects;
                return *this;
            }
    };

    class Director{
        public:
            StudentBuilder* builder;
            Director(StudentBuilder* builder) : builder(builder) {}
            Student construct() {
                return builder->setSubjects().build();
            }
    };

    int main(){
        
        EngineeringStudentBuilder engBuilder;
        Director engDirector(&engBuilder);
        Student engStudent = engDirector.construct();
        engStudent.display();

        cout << endl;

        MBAStudentBuilder mbaBuilder;
        Director mbaDirector(&mbaBuilder);
        Student mbaStudent = mbaDirector.construct();
        mbaStudent.display();

        return 0;
    }
