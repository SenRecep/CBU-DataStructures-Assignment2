#include <string>
using namespace std;
class Lesson
{
public:
    string Code;
    string Name;
    Lesson *Next = nullptr;
    Lesson();
    Lesson(Lesson *lesson);
    void Print();
    void PrintFile(string  &str);
};

class Student
{
public:
    string No;
    string FirstName;
    string LastName;
    Lesson *LessonRootNode = nullptr;
    Student *Next = nullptr;
    void Print();
    string PrintFile();
    ~Student();
};

enum SearchType
{
    NoSelection,
    StudentNumber,
    LastName
};

enum Option
{
    NoSelectedOption,
    SearchOption,
    DeleteLessonOption,
    FindIntersectionOption,
    DisplayListOption,
    DeleteStudentOption,
    WriteToFileOption,
    ExitOption
};

class MultipleList
{
private:
    Student *rootNode = nullptr;
    string fileNames[3]={"eng101.txt","fiz101.txt","mat101.txt"};
public:
    void Init();
    void ReadFile(string fileName);
    void List();
    void AddStudent(Student *student, Lesson *lesson);
    void AddLesson(Student *student, Lesson *lesson);
    void Search();
    void SearchbyStudentNumber(string no);
    void SearchbyStudentLastName(string LastName);
    void DeleteStudent();
    void DeleteLesson();
    void DeleteLessonByStudent(Student* student,string code);
    void FindingLessonIntersection();
    void PrintFile();
    void Menu();
    Option MenuSelection(string option);
    ~MultipleList();
};