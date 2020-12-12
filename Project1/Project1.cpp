#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h> 
#include <fstream>

using namespace std;

enum Specialty { КН, МЕ, ТН, ІФ, ФІ };

string strSpecialty[] = { "КН","МЕ","ТН","ІФ","ФІ" };

struct Student
{
    char prizv[64];
    Specialty specialty;
    short unsigned kurs;
    short unsigned physics;
    short unsigned math;
    union
    {
        short unsigned programming;
        short unsigned methods;
        short unsigned pedagogika;
    };
};

void Create(char* FileName);
void load_print(char* FileName);
void SortBin(char* FileName);
double favgMark(fstream& f, const int i);
int fMark(fstream& f, const int i);
char fPrizv(fstream& f, const int i);
void fChange(fstream& f, const int i, const int j);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char FileName[81];
    int menu;
    do {
        cout << "[0] - Створити перелік студентів" << endl;
        cout << "[1] - Вивести перелік студентів" << endl;
        cout << "[2] - Впорядкування" << endl;
        cout << "[3] - Вийти з програми" << endl;

        cout << "Пункт меню: "; cin >> menu;
        switch (menu)
        {
        case 0:
            cin.get(); cin.sync();
            cout << "Введіть назву файлу: "; cin.getline(FileName, sizeof(FileName));

            Create(FileName);
            break;
        case 1:
            cin.get(); cin.sync();
            cout << "Введіть назву файлу: "; cin.getline(FileName, sizeof(FileName));
            load_print(FileName);
            break;
        case 2:
            cin.get(); cin.sync();
            cout << "Введіть назву файлу: "; cin.getline(FileName, sizeof(FileName));
            SortBin(FileName);
            break;
        case 3:
            exit(1);
        default:
            cout << "Ви ввели некоректне значення" << endl;
            break;
        }
    } while (menu != 3);
}

void Create(char* FileName)
{
    ofstream f(FileName, ios::binary);
    if (!f)
    {
        cerr << "Error opening file '" << FileName << "'" << endl;
        exit(1);
    }

    Student s;
    int specialty;
    char ch;
    int i = 0;
    do {
        cout << endl;
        cout << "Студент№ " << i + 1 << ":" << endl;
        ++i;
        cin.sync();
        cout << "  прізвище: ";  cin >> s.prizv;
        cout << "  курс: "; cin >> s.kurs;
        do
        {
            cout << "  спеціальність (0 - Комп'ютерні науки, 1 - Математика та економіка, 2 - Трудове навчання, 3 - Інформатика, 4 - Фізика та інформатика): "; cin >> specialty;
        } while (specialty != 0 && specialty != 1 && specialty != 2 && specialty != 3 && specialty != 4);
        s.specialty = (Specialty)specialty;
        cout << "  оцінка з фізики: "; cin >> s.physics;
        cout << "  оцінка з математики: "; cin >> s.math;
        switch (specialty)
        {
        case 0:
            cout << "  оцінка з програмування: "; cin >> s.programming;
            break;
        case 3:
            cout << "  оцінка з чисельних методів: "; cin >> s.methods;
            break;
        case 1:
        case 2:
        case 4:
            cout << "  оцінка з педагогіки: "; cin >> s.pedagogika;
            break;
        }
        if (!f.write((char*)&s, sizeof(Student)))
        {
            cerr << "Помилка запису" << endl;
            exit(1);
        }
        cout << "Продовжити? (Y/N): "; cin >> ch;

    } while (ch == 'Y' || ch == 'y');

}
void load_print(char* FileName)
{
    ifstream f(FileName, ios::binary);
    if (!f)
    {
        cerr << "Помилка відкриття файлу" << endl;
        exit(1);
    }
    Student s;
    int i = 0;
    cout << "================================================================================================================="
        << endl;
    cout << "| №  | Прізвище     | Курс | Спеціальність | Фізика | Математика | Програмування | Чисельні методи | Педагогіка |"
        << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------" << endl;
    while (f.read((char*)&s, sizeof(Student)))
    {
        cout << "| " << setw(2) << right << i + 1 << " "
            << "| " << setw(13) << left << s.prizv
            << "| " << setw(3) << right << s.kurs << "  "
            << "| " << setw(8) << right << strSpecialty[s.specialty] << "      "
            << "| " << setw(4) << right << s.physics << "   "
            << "| " << setw(6) << right << s.math << "     ";
        switch (s.specialty)
        {
        case 0:
            cout << "| " << setw(7) << right << s.programming << "       |" << setw(18) << "|" << setw(13) << "|" << endl;
            break;
        case 3:
            cout << "| " << setw(15) << "|" << setw(10) << right << s.methods << "       |" << setw(13) << "|" << endl;
            break;
        case 1:
        case 2:
        case 4:
            cout << "| " << setw(15) << "|" << setw(18) << "|" << right << setw(7) << s.pedagogika << "     |" << endl;
            break;
        }
        ++i;
    }
    cout << "=================================================================================================================" << endl;
}
void SortBin(char* fname)
{
    fstream f(fname, ios::binary | ios::in | ios::out);
    if (!f)
    {
        cerr << "Помилка відкриття файлу" << endl;
        exit(1);
    }

    f.seekg(0 * (long)sizeof(Student), ios::end);
    int size = f.tellg() / (sizeof(Student));
    f.seekg(0, ios::beg);
    for (int i0 = 1; i0 < size; i0++)
        for (int i1 = 0; i1 < size - i0; i1++)
        {
            double avgMark = favgMark(f, i1);
            double avgMark2 = favgMark(f, i1 + 1);
            int mark = fMark(f, i1);
            int mark2 = fMark(f, i1 + 1);
            char student1 = fPrizv(f, i1);
            char student2 = fPrizv(f, i1 + 1);
            if ((avgMark > avgMark2)
                || (avgMark == avgMark2 && mark > mark2)
                || (avgMark == avgMark2 && mark == mark2 && student1 > student2)
                )
                fChange(f, i1, i1 + 1);
        }
    f.seekp(0, ios::end);
}
double favgMark(fstream& f, const int i)
{
    Student s;
    f.seekg(i * (long)sizeof(Student));
    f.read((char*)&s, sizeof(Student));
    int physics_mark = s.physics;
    int math_mark = s.math;
    int third_mark = s.programming;

    return (physics_mark + math_mark + third_mark) / 3.;
}
int fMark(fstream& f, const int i)
{
    Student s;
    f.seekg(i * (long)sizeof(Student));
    f.read((char*)&s, sizeof(Student));
    int math_mark = s.math;
    return math_mark;
}
char fPrizv(fstream& f, const int i)
{
    Student s;
    f.seekg(i * (long)sizeof(Student));
    f.read((char*)&s, sizeof(Student));
    char prizv[81];
    strcpy_s(prizv, sizeof(prizv), s.prizv);
    return *prizv;
}
void fChange(fstream& f, const int i, const int j)
{
    Student si, sj, temp;
    f.seekg(i * (long)sizeof(Student));
    f.read((char*)&si, sizeof(Student));
    temp = si;
    f.seekg(j * (long)sizeof(Student));
    f.read((char*)&sj, sizeof(Student));
    f.seekp(i * (long)sizeof(Student));
    f.write((char*)&sj, sizeof(Student));
    f.seekp(j * (long)sizeof(Student));
    f.write((char*)&temp, sizeof(Student));
}
