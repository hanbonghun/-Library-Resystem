#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include<vector>
using namespace std;
struct Book {
    string bookName;    //å�̸�
    string Author;		//����
    string Publisher;   //���ǻ� 
    int Reserves;	    //���� �� �� 
};
struct Student {
    string Id;			//�л� ID
    string Pw;			//�л� PW
    string Name;		//�л� �̸�
    int Count;			//���� �� �ִ� �� ��
    vector<string> Appliedbooks;	//���� å��(å�� �߰��ϰų� ������ �� �迭�δ� �Ѱ谡 �־ vector�� ����Ͽ����ϴ�.)
};

//���� �б�
void readStuInfo(vector<Student>& student);    //�л� ������ �о���� �Լ� 
void readBookInfo(vector<Book>& book);        //å ������ �о���� �Լ� 

//���� ����
void writeStu(vector<Student>& student);       //����� ��� ���� �ҷ��� ���Ͽ� ���� ����
void writeBookInfo(vector<Book>& book);

//�α���
bool adminLogin();
int  login(vector<Student>& student);
int studentLogin(vector<Student>& student);

//�Է�
string getInputId();
string getInputPassword();

// ������
void addStudent(vector<Student>& student);        //�л� �߰�
void addBook(vector<Book>& book);                //å �߰�
void deleteBook(vector<Book>& book, vector<Student>& student);      //å ����
int runAdmin(vector<Student>& student, vector<Book>& book);

//�����

int runStudent(vector<Student>& student, vector<Book>& book, int user);
void applyBook(vector<Student>& student, vector<Book>& book, const int& user);                //å �뿩
void disapplyBook(vector<Student>& student, vector<Book>& book, const int& user);   //å �ݳ�
void changePassword(vector<Student>& student, const int& user);     //��й�ȣ ����
void printStudent(vector<Student>& student, const int& user);       //�л����� ��� 
void searchBook(vector<Book>& book, const int& user, vector<Student>& student);       //å �˻� 

//����
void deleteElement(vector<string>& v, int index);       //vector ��� ���� 
void printBookInfo(vector<Student>& student, vector<Book>& book, const int& user, int book_num);    //å ���� ��� (��� å, �뿩�� å)
int findBookByName(vector<Book>& book, string bookname);  //å �̸����� ã��(�ߺ� or ���� ���� �Ǵ�)
int findStudentById(vector<Student>& student, string id);  //id�� �л� �˻� (�ߺ��Ǵ�)


int main() {
    int user = -1; //user index
    int status = -1;
    vector<Student> student;               //�л� �� �ִ� 100�� ���� ����
    vector<Book> book;                 //å �� �ִ� 2000�� ����
    readStuInfo(student);
    readBookInfo(book);
    while (status == -1) {
        user = login(student);

        if (user == -999) { // �α��� ���� ��
            break;
        }
        else if (user == -1) { // ���� 
            break;
        }
        else if (user == -100) { // ������ �α��� ��
            status = runAdmin(student, book);

        }
        else { // �л� �α��� �� 
            status = runStudent(student, book, user);
        }
    }
    writeStu(student);
    writeBookInfo(book);
}

void readStuInfo(vector<Student>& student) {
    ifstream fin("stu.txt");
    string line;
    while (getline(fin, line)) {
        Student temp;
        int index1 = line.find('\t');                                   //�޸��忡�� \t ������ ���еǹǷ� 
        temp.Id = line.substr(0, index1);

        int index2 = line.find('\t', index1 + 1);
        temp.Pw = line.substr(index1 + 1, index2 - index1 - 1);

        int index3 = line.find('\t', index2 + 1);
        temp.Name = line.substr(index2 + 1, index3 - index2 - 1);

        int index4 = line.find('\t', index3 + 1);
        temp.Count = (line[index3 + 1]) - 48;

        if (temp.Count == 1) {
            int index5 = line.find('\t', index4 + 1);
            temp.Appliedbooks.push_back(line.substr(index4 + 1, index5 - index4 - 1));
        }
        else if (temp.Count == 0) {
            int index5 = line.find('\t', index4 + 1);
            temp.Appliedbooks.push_back(line.substr(index4 + 1, index5 - index4 - 1));
            temp.Appliedbooks.push_back(line.substr(index5 + 1, line.size() - index5 - 2));
        }
        student.push_back(temp);
    }
}

void readBookInfo(vector<Book>& book) {
    ifstream fin("BookInfo.txt");
    string line;
    while (getline(fin, line)) {
        Book temp;
        int index1 = line.find('\t');
        temp.bookName = line.substr(0, index1);

        int index2 = line.find('\t', index1 + 1);
        temp.Author = line.substr(index1 + 1, index2 - index1 - 1);

        int index3 = line.find('\t', index2 + 1);
        temp.Publisher = line.substr(index2 + 1, index3 - index2 - 1);

        temp.Reserves = line[index3 + 1] - 48;
        book.push_back(temp);
    }
}

void writeStu(vector<Student>& student) {
    ofstream fout;
    fout.open("stu.txt");
    for (unsigned int i = 0; i < student.size(); i++) {
        string id = student[i].Id;
        string pw = student[i].Pw;
        string name = student[i].Name;
        int Credits_available = student[i].Count;
        fout << student[i].Id << '\t' << student[i].Pw << '\t' << student[i].Name << '\t' << student[i].Count << '\t';
        if (student[i].Appliedbooks.size() > 0) {
            for (unsigned int j = 0; j < student[i].Appliedbooks.size(); j++) {
                fout << student[i].Appliedbooks[j] << '\t';
            }
            fout << endl;
        }
        else  fout << endl;
    }
    fout.close();
}

void writeBookInfo(vector<Book>& book) {
    ofstream fout;
    fout.open("BookInfo.txt");
    for (unsigned int i = 0; i < book.size(); i++) {
        fout << book[i].bookName << '\t' << book[i].Author << '\t' << book[i].Publisher << '\t' << book[i].Reserves << endl;
    }
    fout.close();
}

bool adminLogin() {
    int failcount = 1;
    while (failcount < 4) {
        string id = getInputId();
        string pw = getInputPassword();
        if (id == "admin" && pw == "admin") {
            failcount = 0;
            return true;
        }
        else {
            cout << "�α��� " << failcount << "ȸ ���� (3ȸ ���н� ����)" << endl;
            failcount++;
        }
    }
    cout << "3ȸ �����Ͽ� �����մϴ�." << endl;
    return false;
}


string getInputId() {
    string id;
    cout << "���̵�: ";
    cin >> id;
    return id;
}

string getInputPassword() {
    string password;
    cout << "��й�ȣ: ";
    cin >> password;
    return password;
}
void deleteElement(vector<string>& v, int index) {
    v.erase(v.begin() + index);
}

int studentLogin(vector<Student>& student) {
    int failcount = 1; //�α��� ���� Ƚ�� ���� 
    while (failcount < 4) {
        string id = getInputId();
        string pw = getInputPassword();
        for (unsigned int i = 0; i < student.size(); i++) {
            if (id == student[i].Id && pw == student[i].Pw) {
                failcount = 0;
                return i;
            }
        }
        cout << "�α��� " << failcount << "ȸ ���� (3ȸ ���н� ����)" << endl;
        failcount++;
    }
    cout << "3ȸ �����Ͽ� �����մϴ�." << endl;
    return -1;
}

int login(vector<Student>& student) {
    while (1) {
        int login_type;
        cout << " ---------<<�α���>>--------" << endl;
        cout << "|                           |" << endl;
        cout << "|     1. �л� �α���        |" << endl;
        cout << "|     2. ������ �α���      |" << endl;
        cout << "|     0. ����               |" << endl;
        cout << "|                           |" << endl;
        cout << " ---------------------------" << endl;
        cout << "�α��� ������ �������ּ��� : ";
        cin >> login_type;
        if (login_type == 1) {
            int result_studentlogin = studentLogin(student);
            if (result_studentlogin == -1)
                return -999;
            else return result_studentlogin;
        }
        else if (login_type == 2) {
            bool result_adminlogin = adminLogin();
            if (result_adminlogin == false)
                return -999;
            else return -100;
        }
        else if (login_type == 0)
            return -1;
        else {
            cout << "���ڸ� �ٽ� �Է����ּ���" << endl;
            continue;
        }
    }
}

int findBookByName(vector<Book>& book, string bookname) {
    for (unsigned int i = 0; i < book.size(); i++) {
        if (book[i].bookName == bookname)
            return i;
    }
    return -1;
}

int findStudentById(vector<Student>& student, string id) {
    for (unsigned int i = 0; i < student.size(); i++) {
        if (student[i].Id == id)
            return i;
    }
    return -1;
}
void printStudent(vector<Student>& student, const int& user) {
    cout << "---------------------------------------------------------------------------------" << endl;
    cout << "\t\t�й�: " << student[user].Id << "    " << "�̸�: " << student[user].Name << "  ���� ���� Ƚ��: " << student[user].Count << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
}

void printBookInfo(vector<Student>& student, vector<Book>& book, const int& user) {
    if (user == -100)cout << "**********************************���� ���� ���*********************************" << endl;
    else cout << "**********************************���� ���� ���*********************************" << endl;
    cout << "=================================================================================" << endl;
    cout << setw(22) << "����" << setw(18) << "|"
        << setw(12) << "�۰�" << setw(8) << "|"
        << setw(11) << "���ǻ�" << setw(4) << "|"
        << setw(4) << " �Ǽ�" << "|" << endl;
    cout << "=================================================================================" << endl;
    if (user == -100) {
        for (unsigned int i = 0; i < book.size(); i++)
            cout << setw(40 - (40 - book[i].bookName.size()) / 2) << book[i].bookName << setw((40 - book[i].bookName.size()) / 2) << "|"
            << setw(20 - (20 - book[i].Author.size()) / 2) << book[i].Author << setw((20 - book[i].Author.size()) / 2) << "|"
            << setw(15 - (15 - book[i].Publisher.size()) / 2) << book[i].Publisher << setw((15 - book[i].Publisher.size()) / 2) << "|"
            << setw(4) << book[i].Reserves << setw(2) << "|" << endl
            << "---------------------------------------------------------------------------------" << endl;
    }
    else {
        for (int i = 0; i < 2 - student[user].Count; i++) {
            int index = findBookByName(book, student[user].Appliedbooks[i]);
            if (index >= 0)
                cout << setw(40 - (40 - student[user].Appliedbooks[i].size()) / 2) << student[user].Appliedbooks[i] << setw((40 - student[user].Appliedbooks[i].size()) / 2) << "|"
                << setw(20 - (20 - book[index].Author.size()) / 2) << book[index].Author << setw((20 - book[index].Author.size()) / 2) << "|"
                << setw(15 - (15 - book[index].Publisher.size()) / 2) << book[index].Publisher << setw((15 - book[index].Publisher.size()) / 2) << "|"
                << setw(4) << book[index].Reserves << setw(2) << "|" << endl;
            cout << "---------------------------------------------------------------------------------" << endl;
        }
    }
}

int runStudent(vector<Student>& student, vector<Book>& book, int user) {
    while (1) {
        char runstudent_type;
        cout << "---------------------<<�޴�>>----------------------" << endl;
        cout << "|1. �����˻�                                      |" << endl;
        cout << "|2. ��������                                      |" << endl;
        cout << "|3. ������                                      |" << endl;
        cout << "|4. �����ݳ�                                      |" << endl;
        cout << "|5. ��й�ȣ ����                                 |" << endl;
        cout << "|6. �α׾ƿ�                                      |" << endl;
        cout << "|0. ����                                          |" << endl;
        cout << "---------------------------------------------------" << endl << "�޴��� �������ּ��� :";

        cin >> runstudent_type;
        cin.clear();
        cin.ignore();
        switch (runstudent_type) {
        case('1'):
            searchBook(book, -100, student);
            break;
        case ('2'):
            applyBook(student, book, user);
            break;
        case('3'):
            printStudent(student, user);
            printBookInfo(student, book, user);
            cout << "����Ϸ��� �ƹ�Ű�� �����ʽÿ�...." << endl;
            cin.ignore();
            getchar();
            break;
        case('4'):
            disapplyBook(student, book, user);
            break;
        case('5'):
            changePassword(student, user);
            break;
        case('6'):
            return -1;
            break;
        case('0'):
            return 1;
            break;
        default:
            cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
            break;
        }
        continue;
    }
}

void applyBook(vector<Student>& student, vector<Book>& book, const int& user) {
    printStudent(student, user);
    printBookInfo(student, book, -100);
    while (1) {
        bool apply_possible = true;                      //���� ���� ���� ���� ����
        int cnt = 0;                                       //���� �Ұ� ������ ���� ���� �� ���ǰ˻� ������ ���� �Ѱ��� ���θ� ����ϰԲ� �ϴ� ����
        string applybook_name;
        cout << "���⸦ �����Ͽ� ��û�� å�� �̸��� �Է����ּ���.(0: �ڷΰ���)>>";
        getline(cin, applybook_name);
        if (applybook_name == "0") break;
        int index = -1;
        index = findBookByName(book, applybook_name);

        if (index == -1) {
            cout << "�߸��Է��ϼ̽��ϴ�." << endl;                                              //å �̸��� �ƴ� �ٸ� ���� �Է��� ���
            cout << "����Ϸ��� �ƹ� Ű�� �����ʽÿ�....";
            cin.ignore();
            getchar();
            continue;
        }
        if (book[index].Reserves == 0) {
            cout << "�����ϰ� �ִ� å�� �����ϴ�." << endl;                                          //1. ���� å ��� ����
            apply_possible = false;
            continue;
        }
        if (cnt == 0) {
            if (student[user].Count == 0) {
                cout << "�ִ� 2�Ǳ��� ���� �����մϴ�." << endl;                                   //2. �����ܿ� Ƚ�� ��п���
                apply_possible = false;
                continue;
            }
        }
        if (cnt == 0) {
            for (int i = 0; i < 2 - student[user].Count; i++) {
                if (applybook_name == student[user].Appliedbooks[i]) {                                  //3. �ߺ�����
                    cout << "�̹� �ش� å�� �����߽��ϴ�." << endl;
                    apply_possible = false;
                    cnt++;
                    break;
                }
            }

        }
        if (apply_possible == true) {
            cout << '[' << applybook_name << ']' << "(��)�� ���������� �����Ͽ����ϴ�." << endl;
            book[index].Reserves--;                                             //���� å 1�� ����
            student[user].Count--;       //�л��� ���� ���� Ƚ�� ����
            student[user].Appliedbooks.push_back(book[index].bookName);    //�л��� ������ vector�� ��û�� å �̸� �߰�            
            break;
        }
    }
}
void disapplyBook(vector<Student>& student, vector<Book>& book, const int& user)
{
    printStudent(student, user);
    printBookInfo(student, book, user);

    while (1) {
        string disapply_book;
        bool disapply_possible = false;         //å �ݳ� ���� ���� ���� ����
        cout << "�ݳ��� å ����(0: �ڷΰ���)>>" << endl;
        getline(cin, disapply_book);
        if (disapply_book == "0") break;
        int user_index = 0;
        int book_index = 0;
        for (unsigned int i = 0; i < student[user].Appliedbooks.size(); i++) {
            if (disapply_book == student[user].Appliedbooks[i]) {
                user_index = i;
                disapply_possible = true;
                book_index = findBookByName(book, student[user].Appliedbooks[i]);
            }
        }
        if (!disapply_possible) {
            cout << "å �̸��� �ùٸ��� �ʽ��ϴ�." << endl;
            cout << "����Ϸ��� �ƹ�Ű�� �����ʽÿ�...." << endl;
            cin.ignore();
            getchar();
            continue;
        }
        cout << book[book_index].bookName << "(��)�� �ݳ��Ͽ����ϴ�." << endl;
        book[book_index].Reserves++;
        student[user].Count++;                              //�����ܿ� Ƚ�� ����
        deleteElement(student[user].Appliedbooks, user_index);                                         //�뿩�� å ��Ͽ��� �ش� å ���� 
        printStudent(student, user);
        printBookInfo(student, book, user);
        cout << "����Ϸ��� �ƹ� Ű�� �����ʽÿ�...." << endl;
        cin.ignore();
        getchar();
    }
}
void changePassword(vector<Student>& student, const int& user) {
    while (1) {
        string password;
        cout << "---------------------------------------------------" << endl << ">>";
        cout << "���� Ȯ���� ���� ��й�ȣ�� �� �� �� �Է����ּ���." << endl;
        cout << "��й�ȣ: ";
        cin >> password;
        if (password == student[user].Pw) {
            cout << "---------------------------------------------------" << endl << ">>";
            cout << "���� ������ ��й�ȣ�� �Է��ϼ���." << endl;
            cout << "��й�ȣ: ";
            cin >> password;
            cout << "����Ǿ����ϴ�." << endl;
            student[user].Pw = password;
            cout << "---------------------------------------------------" << endl << ">>";
        }
        else cout << "��й�ȣ�� ��ġ���� �ʽ��ϴ�." << endl;
        cout << "����Ϸ��� �ƹ� Ű�� �����ʽÿ�....(0: �ڷΰ���)" << endl;
        cin.ignore();
        if (getchar() == '0') break;;
    }
}

void searchBook(vector<Book>& book, const int& user, vector<Student>& student) {
    while (1) {
        string keyword;
        printBookInfo(student, book, user);
        cout << "�˻��� Ű���带 �Է��� �ּ���(����, �۾���, ���ǻ� �˻� ����)    �ڷΰ���:0 " << endl << ">>";
        getline(cin, keyword);
        if (keyword == "0")break;
        cout << endl;
        bool exist = false;
        cout << "*************************************�˻� ���***********************************" << endl;
        cout << "=================================================================================" << endl;
        cout << setw(22) << "����" << setw(18) << "|"
            << setw(12) << "�۰�" << setw(8) << "|"
            << setw(11) << "���ǻ�" << setw(4) << "|"
            << setw(4) << " �Ǽ�" << "|" << endl;
        cout << "=================================================================================" << endl;

        for (unsigned int i = 0; i < book.size(); i++) {

            if ((book[i].bookName.find(keyword)) != string::npos || (book[i].Author.find(keyword)) != string::npos || (book[i].Publisher.find(keyword)) != string::npos)
            {
                cout << setw(40 - (40 - book[i].bookName.size()) / 2) << book[i].bookName << setw((40 - book[i].bookName.size()) / 2) << "|"
                    << setw(20 - (20 - book[i].Author.size()) / 2) << book[i].Author << setw((20 - book[i].Author.size()) / 2) << "|"
                    << setw(15 - (15 - book[i].Publisher.size()) / 2) << book[i].Publisher << setw((15 - book[i].Publisher.size()) / 2) << "|"
                    << setw(4) << book[i].Reserves << setw(2) << "|" << endl
                    << "---------------------------------------------------------------------------------" << endl;
                exist = true;
            }
        }
        if (!exist) cout << "�˻��� å�� �����ϴ�." << endl;
        cout << "����Ϸ��� �ƹ� Ű�� �����ʽÿ�...";
        cin.ignore();
        getchar();

    }
}

int runAdmin(vector<Student>& student, vector<Book>& book) {
    while (1) {
        char runadmin_type;
        cout << "---------------------<<�޴�>>----------------------" << endl;
        cout << "|1. �л� �߰�                                     |" << endl;
        cout << "|2. ���� ���                                     |" << endl;
        cout << "|3. ���� ����                                     |" << endl;
        cout << "|4. �α׾ƿ�                                      |" << endl;
        cout << "|0. ����                                          |" << endl;
        cout << "---------------------------------------------------" << endl << ">>";
        cin >> runadmin_type;
        cin.clear();
        cin.ignore();
        switch (runadmin_type) {
        case('1'):
            addStudent(student);
            break;
        case('2'):
            addBook(book);
            break;
        case('3'):
            deleteBook(book, student);
            break;
        case('4'):
            return -1;
            break;
        case('0'):
            return 0;
            break;
        default:
            cout << "�߸� �Է��ϼ̽��ϴ�." << endl;
            break;
        }
        continue;
    }
}

void addStudent(vector<Student>& student) {
    while (1) {
        string studentid, password, name;
        bool dup_check = false;       //�й� �ߺ��Ǵ� ����
        cout << "---------------------------------------------------" << endl;
        cout << "�й� ";
        cin >> studentid;
        cout << "��й�ȣ ";
        cin >> password;
        cout << "�л� �̸�: ";
        cin >> name;
        cout << "---------------------------------------------------" << endl;
        if (findStudentById(student, studentid) != -1) dup_check = true;
        if (dup_check == true) {
            cout << "�̹� �����ϴ� �й��Դϴ�." << endl << "����Ϸ��� �ƹ� Ű�� �����ʽÿ�....(0 :�ڷΰ���)" << endl;
            cin.ignore();
            if (getchar() == '0')break;
        }
        else {
            Student temp;
            temp.Id = studentid;
            temp.Pw = password;
            temp.Name = name;
            temp.Count = 2;
            student.push_back(temp);

            cout << "���������� ��ϵǾ����ϴ�." << endl << "����Ϸ��� �ƹ� Ű�� �����ʽÿ�....(0 :�ڷΰ���)" << endl;
            cin.ignore();
            if (getchar() == '0') break;
        }
    }
}

void addBook(vector<Book>& book) {
    while (1) {
        string bookname, author, publisher;
        int reserve;
        bool dup_check = false;        //å �ߺ� �˻� ����
        cout << "---------------------------------------------------" << endl;
        cout << "å �̸�: ";
        getline(cin, bookname);

        cout << "����: ";
        getline(cin, author);
        cout << "���ǻ�: ";
        getline(cin, publisher);
        cout << "���� �� ��: ";
        while (!(cin >> reserve)) {
            cout << "������ �Է����ּ���" << endl;
            cin.clear();
            cin.ignore();
        }
        cout << "---------------------------------------------------" << endl;

        if (findBookByName(book, bookname) != -1) dup_check = true;
        if (dup_check == true) {
            cout << "�̹� �����ϴ� å �Դϴ�." << endl << "����Ϸ��� �ƹ� Ű�� �����ʽÿ�....(0: �ڷΰ���)" << endl;
            cin.ignore();
            if (getchar() == '0') break;
        }
        else {
            Book temp;
            temp.bookName = bookname;
            temp.Author = author;
            temp.Publisher = publisher;
            temp.Reserves = reserve;
            book.push_back(temp);
            cout << "���������� å�� ����Ͽ����ϴ�." << endl;
            break;
        }
    }
}

void deleteBook(vector<Book>& book, vector<Student>& student) {
    printBookInfo(student, book, -100);
    while (1) {
        bool delete_possible = false;     //å ��� ���� ���� ���� �Ǵ�
        int book_index = 0;
        string delete_book;
        cout << "��������� å �̸� (0:�ڷΰ���) >>";
        getline(cin, delete_book);
        if (delete_book == "0") break;

        if (findBookByName(book, delete_book) != -1) {
            delete_possible = true;
            book_index = findBookByName(book, delete_book);
        }

        if (delete_possible == false) {
            cout << "��ġ�ϴ� å�� �����ϴ�." << endl;
            cout << "����Ϸ��� �ƹ� Ű�� �����ʽÿ�...." << endl;
            cin.ignore();
            getchar();
            continue;
        }
        if (delete_possible == true) {
            for (unsigned int i = 0; i < student.size(); i++) {
                for (unsigned int j = 0; j < student[i].Appliedbooks.size(); j++) {
                    if (student[i].Appliedbooks[j] == delete_book) {
                        student[i].Appliedbooks.erase(student[i].Appliedbooks.begin() + j);//å�� ��������Ǹ� �л��� �뿩�� å ��Ͽ��� �ش� å�� �����Ѵ�
                        student[i].Count++;
                        break;
                    }
                }
            }
            book.erase(book.begin() + book_index);  //å�� ��� �����ʿ� ���� BookŬ������ �����ϴ� vector ���� �ش� index��° ��ü�� �����Ѵ�.  
            cout << "���������� ��� ���� �Ǿ����ϴ�." << endl << "�Լ��Ϸ��� �ƹ� Ű�� �����ʽÿ�...." << endl;
            cin.ignore();
            getchar();
        }
    }
}