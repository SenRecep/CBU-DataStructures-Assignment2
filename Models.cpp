#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Models.h"

Lesson::Lesson() {}
Lesson::Lesson(Lesson *lesson) //var olan bir dersin ozelliklerini kopyalayarak yeni bir ders olusturur
{
    this->Code = lesson->Code;
    this->Name = lesson->Name;
}

void Lesson::Print() //ekrana dersin ozelliklerini yazar
{
    cout << "[" << this->Code << " " << this->Name << "] ";
}

void Student::Print() //ekrana ogrencinin ozelliklerini yazar
{
    cout << this->No << " " << this->FirstName << " " << this->LastName << " ";
    Lesson *lesson = this->LessonRootNode;
    while (lesson)
    {
        lesson->Print();
        lesson = lesson->Next;
    }
    cout << endl;
}

void Lesson::PrintFile(string &str) //dosyaya yazmak icin dersin bilgilerini str degiskenine ekler
{
    str += "[" + this->Code + " " + this->Name + "] ";
}

string Student::PrintFile() //dosyaya yazmak icin ogrencinin bilgilerini str degiskenine ekler
{
    string str = "";
    str += this->No + " " + this->FirstName + " " + this->LastName + " ";
    Lesson *lesson = this->LessonRootNode;
    while (lesson)
    {
        lesson->PrintFile(str);
        lesson = lesson->Next;
    }
    str += "\n";
    return str;
}

Student::~Student() //ogrenci bellekten silinirken ders bilgileride silinir
{
    Lesson *current = this->LessonRootNode, *next = nullptr;
    while (current)
    {
        next = current->Next;
        delete current;
        current = next;
    }
}

MultipleList::~MultipleList() //liste bellekten silinirken ogrenci bilgileride silinir
{
    Student *current = this->rootNode, *next = nullptr;
    while (current)
    {
        next = current->Next;
        delete current;
        current = next;
    }
}

void MultipleList::Init() //listeye dosyalardan gelen veriler eklenir
{
    for (auto fileName : this->fileNames)
        ReadFile("./data/" + fileName);
}

void MultipleList::ReadFile(string fileName) //dosyadan gelen verileri listeye ekler
{
    string line;
    ifstream file(fileName);
    if (file.is_open())
    {
        Lesson *currentLesson = new Lesson();
        file >> currentLesson->Code >> currentLesson->Name;
        while (!file.eof())
        {
            Student *currentStudent = new Student();
            file >> currentStudent->No >> currentStudent->FirstName;
            getline(file, currentStudent->LastName);
            currentStudent->LastName = currentStudent->LastName.replace(0, 1, "");
            this->AddStudent(currentStudent, currentLesson);
        }
        file.close();
    }
    else
        cout << "Dosya okunamadı";
}

void MultipleList::List() //ogrencileri listeler
{
    Student *student = this->rootNode;
    while (student)
    {
        student->Print();
        student = student->Next;
    }
}

void MultipleList::AddLesson(Student *student, Lesson *lesson) //tekrarlanmayan şekilde ogrenciye ders ekler
{
    Lesson *search = nullptr, *found = nullptr;
    if (!student->LessonRootNode) //ilk ders yok mu
    {
        student->LessonRootNode = new Lesson(lesson); //ilk dersi olustur
        return;
    }

    search = student->LessonRootNode; //aramaya listenin basindan basla

    while (search)
        /*
         aranacak ders var ise aramaya devam et
         eklenmeye calisan dersin koduna gore kontrol et 
         var ise degiskene ata
         yok ise sonraki dersten itibaren aramaya devam et
        */
        if (search->Code == lesson->Code)
        {
            found = search;
            break;
        }
        else
            search = search->Next;

    if (!found) // bulunan ders yok ise ekle
    {
        search = student->LessonRootNode;
        while (search->Next)
            search = search->Next;
        search->Next = new Lesson(lesson);
    }
}

void MultipleList::AddStudent(Student *student, Lesson *lesson) //tekrarlanmayan şekilde ogrenci ekler
{
    Student *search = nullptr, *found = nullptr;
    if (!this->rootNode) //ilk ogrenci yok mu
    {
        this->rootNode = student;         //ilk ogrenciyi olustur
        this->AddLesson(student, lesson); //ogrencinin dersini ekle
        return;
    }
    search = rootNode; // aramayi listenin basindan baslat

    while (search)
        /*
       aranacak bir ogrenci var ise aramaya devam et
       eklenmeye calisilan ogrencinin numarasina gore kontrol et 
       var ise onu degiskene ata 
       yok ise sonraki ogrenciden itibaren aramaya devam et
       */
        if (search->No == student->No)
        {
            found = search;
            break;
        }
        else
            search = search->Next;
    if (found)
        this->AddLesson(found, lesson); //var olan ogrenciye yeni ders ekleme
    //var olan bir ogrenci degil isen yeni ogrenciyi listenin sonuna ekle
    else
    {
        search = this->rootNode;
        while (search->Next) //listenin sonuncu elemanini buldurma
            search = search->Next;
        search->Next = student;
        this->AddLesson(student, lesson); //ogrencinin dersini ekleme
    }
}

void MultipleList::Search() //ogrenci arama yapmak icin kullanilir, iki tip arama secenegi sunar
{
    SearchType type;
    string typeInput, s;
    cout << "Hangi tip arama yapmak istiyorsunuz?\n[Ogrenci numarasina gore arama: N | n]\n[Ogrenci soyadina gore arama: S | s]\nSeciminizi giriniz: ";
    cin >> typeInput;
    if (typeInput == "N" || typeInput == "n")
        type = StudentNumber;
    else if (typeInput == "S" || typeInput == "s")
        type = LastName;
    else
        type = NoSelection;

    cout << "Arama yapacaginiz kelimeyi giriniz: ";
    cin >> s;

    switch (type)
    {
    case StudentNumber:
        this->SearchbyStudentNumber(s);
        break;
    case LastName:
        this->SearchbyStudentLastName(s);
        break;
    default:
        cout << "Bu tip icin arama suanda kullanim disi" << endl;
        break;
    }
}

void MultipleList::SearchbyStudentNumber(string no) //ogrenci numarasina gore arama yapar
{
    cout << no << " Ogrenci numarasi icin arama sonuclari:" << endl;
    Student *search = nullptr, *found = nullptr;
    search = this->rootNode;
    while (search)
        if (search->No == no)
        {
            found = search;
            break;
        }
        else
            search = search->Next;
    if (found)
        found->Print();
    else
        cout << "Istenilen ozelliklerde ogrenci bulunamadi" << endl;
}

void MultipleList::SearchbyStudentLastName(string lastName) //ogrenci soyadina gore arama yapar
{
    cout << lastName << " Ogrenci soyadi icin arama sonuclari:" << endl;
    Student *search = nullptr;
    bool isFound = false;
    search = this->rootNode;
    while (search)
    {
        if (search->LastName == lastName)
        {
            search->Print();
            isFound = true;
        }
        search = search->Next;
    }

    if (!isFound)
        cout << "Istenilen ozelliklerde ogrenci bulunamadi" << endl;
}

void MultipleList::DeleteStudent() //ogrenci numarasina gore ogrenci sildirmek icin kullanilir
{
    string no;
    cout << "Silmek istediginiz ogrenci numarasini giriniz:";
    cin >> no;
    Student *search = nullptr, *temp = nullptr, *before = nullptr;
    search = this->rootNode;

    if (this->rootNode->No == no)
    {
        temp = this->rootNode->Next;
        delete this->rootNode;
        this->rootNode = temp;
        return;
    }

    before = this->rootNode;
    search = before->Next;
    while (search)
    {
        if (search->No == no)
        {
            temp = search->Next;
            delete search;
            before->Next = temp;
            break;
        }

        before = search;
        search = before->Next;
    }
}

void MultipleList::DeleteLessonByStudent(Student *student, string code) //ogrencinin dersini ders koduna gore sildirmek icin kullanilir
{
    Lesson *before = nullptr, *search = nullptr, *temp = nullptr;
    if (student->LessonRootNode->Code == code)
    {
        temp = student->LessonRootNode->Next;
        delete student->LessonRootNode;
        student->LessonRootNode = temp;
        return;
    }

    before = student->LessonRootNode;
    search = before->Next;
    while (search)
    {
        if (search->Code == code)
        {
            temp = search->Next;
            delete search;
            before->Next = temp;
            break;
        }

        before = search;
        search = before->Next;
    }
}

void MultipleList::DeleteLesson() //ders sildirmek icin kullanilir, ogenci no ve ders kodunu kullanicidan ister
{
    string no, code;
    cout << "Ogrenci numarasini giriniz:";
    cin >> no;
    cout << "Ders kodunu giriniz:";
    cin >> code;

    Student *search = nullptr, *found = nullptr;
    Lesson *searchLesson = nullptr;
    search = this->rootNode;
    while (search)
        if (search->No == no)
        {
            found = search;
            break;
        }
        else
            search = search->Next;
    if (found)
        this->DeleteLessonByStudent(found, code);
}

void MultipleList::FindingLessonIntersection() //iki ders koduna gore o dersi ortak alan ogrenclileri ogrencileri listeler
{
    string left, right;
    cout << "Birinci ders kodunu giriniz: ";
    cin >> left;
    cout << "Ikinci ders kodunu giriniz: ";
    cin >> right;

    cout << left << ", " << right << " derslerini ortak alan ogrenciler:" << endl;
    Student *student = this->rootNode;
    Lesson *lesson = nullptr;
    while (student)
    {
        int counter = 0;
        lesson = student->LessonRootNode;
        while (lesson)
        {
            if (lesson->Code == left || lesson->Code == right)
                counter++;
            lesson = lesson->Next;
        }

        if (counter == 2)
            student->Print();
        student = student->Next;
    }
}

void MultipleList::PrintFile() //ogrenci listesini output.txt dosyasina yazar
{
    ofstream file;
    file.open("./data/output.txt");
    if (file.is_open())
    {
        Student *student = this->rootNode;
        while (student)
        {
            file << student->PrintFile();
            student = student->Next;
        }
        file.close();
    }
    else
        cout << "dosya olusturulamadi" << endl;
}

void MultipleList::Menu() //sistem kapatilmadigi muddetce acik kalan bir menu calistirir
{
    while (true)
    {
        system("CLS");
        string choice = "";
        cout << "\t\t\t---ISLEM MENUSU---" << endl;
        cout << "Yapmak istediginiz islem icin asagidaki durumlardan birini giriniz." << endl;
        cout << "Arama yapmak icin: A" << endl;
        cout << "Ders silmek icin: D" << endl;
        cout << "Kesisim bulmak icin: K" << endl;
        cout << "Listeyi ekrana yazdirmak icin: L" << endl;
        cout << "Ogrenci silmek icin: O" << endl;
        cout << "Dosyaya yaz: Y" << endl;
        cout << "Cikis icin: C" << endl;
        cout << "Seciminizi giriniz: ";
        cin >> choice;
        Option option = this->MenuSelection(choice);
        switch (option)
        {
        case SearchOption:
            this->Search();
            break;
        case DeleteLessonOption:
            this->DeleteLesson();
            cout << "Ogrencinin dersi basari ile silindi" << endl;
            break;
        case FindIntersectionOption:
            this->FindingLessonIntersection();
            break;
        case DisplayListOption:
            this->List();
            break;
        case DeleteStudentOption:
            this->DeleteStudent();
            cout << "Ogrenci basari ile silindi" << endl;
            break;
        case WriteToFileOption:
            this->PrintFile();
            cout << "Ogrenci listesi ./data/output.txt dosyasina basarili bir sekilde yazildi" << endl;
            break;
        case ExitOption:
            cout << "Programdan cikis yapilmistir";
            return;
        default:
            cout << "Hatali secim" << endl;
            break;
        }
        system("PAUSE");
    }
}

Option MultipleList::MenuSelection(string option) // menu secimlerini enum hale cevirir
{
    if (option == "A" || option == "a")
        return SearchOption;
    if (option == "D" || option == "d")
        return DeleteLessonOption;
    if (option == "K" || option == "k")
        return FindIntersectionOption;
    if (option == "L" || option == "l")
        return DisplayListOption;
    if (option == "O" || option == "o")
        return DeleteStudentOption;
    if (option == "Y" || option == "y")
        return WriteToFileOption;
    if (option == "C" || option == "c")
        return ExitOption;
    return NoSelectedOption;
}
