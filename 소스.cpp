#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include<vector>
using namespace std;
struct Book {
    string bookName;    //책이름
    string Author;		//저자
    string Publisher;   //출판사 
    int Reserves;	    //보유 권 수 
};
struct Student {
    string Id;			//학생 ID
    string Pw;			//학생 PW
    string Name;		//학생 이름
    int Count;			//빌릴 수 있는 권 수
    vector<string> Appliedbooks;	//빌린 책들(책을 추가하거나 삭제할 때 배열로는 한계가 있어서 vector를 사용하였습니다.)
};

//파일 읽기
void readStuInfo(vector<Student>& student);    //학생 정보를 읽어오는 함수 
void readBookInfo(vector<Book>& book);        //책 정보를 읽어오는 함수 

//파일 쓰기
void writeStu(vector<Student>& student);       //변경된 모든 정보 불러온 파일에 새로 저장
void writeBookInfo(vector<Book>& book);

//로그인
bool adminLogin();
int  login(vector<Student>& student);
int studentLogin(vector<Student>& student);

//입력
string getInputId();
string getInputPassword();

// 관리자
void addStudent(vector<Student>& student);        //학생 추가
void addBook(vector<Book>& book);                //책 추가
void deleteBook(vector<Book>& book, vector<Student>& student);      //책 삭제
int runAdmin(vector<Student>& student, vector<Book>& book);

//사용자

int runStudent(vector<Student>& student, vector<Book>& book, int user);
void applyBook(vector<Student>& student, vector<Book>& book, const int& user);                //책 대여
void disapplyBook(vector<Student>& student, vector<Book>& book, const int& user);   //책 반납
void changePassword(vector<Student>& student, const int& user);     //비밀번호 변경
void printStudent(vector<Student>& student, const int& user);       //학생정보 출력 
void searchBook(vector<Book>& book, const int& user, vector<Student>& student);       //책 검색 

//공통
void deleteElement(vector<string>& v, int index);       //vector 요소 삭제 
void printBookInfo(vector<Student>& student, vector<Book>& book, const int& user, int book_num);    //책 정보 출력 (모든 책, 대여한 책)
int findBookByName(vector<Book>& book, string bookname);  //책 이름으로 찾기(중복 or 보유 여부 판단)
int findStudentById(vector<Student>& student, string id);  //id로 학생 검색 (중복판단)


int main() {
    int user = -1; //user index
    int status = -1;
    vector<Student> student;               //학생 수 최대 100명 까지 저장
    vector<Book> book;                 //책 수 최대 2000권 저장
    readStuInfo(student);
    readBookInfo(book);
    while (status == -1) {
        user = login(student);

        if (user == -999) { // 로그인 실패 시
            break;
        }
        else if (user == -1) { // 종료 
            break;
        }
        else if (user == -100) { // 관리자 로그인 시
            status = runAdmin(student, book);

        }
        else { // 학생 로그인 시 
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
        int index1 = line.find('\t');                                   //메모장에서 \t 단위로 구분되므로 
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
            cout << "로그인 " << failcount << "회 실패 (3회 실패시 종료)" << endl;
            failcount++;
        }
    }
    cout << "3회 실패하여 종료합니다." << endl;
    return false;
}


string getInputId() {
    string id;
    cout << "아이디: ";
    cin >> id;
    return id;
}

string getInputPassword() {
    string password;
    cout << "비밀번호: ";
    cin >> password;
    return password;
}
void deleteElement(vector<string>& v, int index) {
    v.erase(v.begin() + index);
}

int studentLogin(vector<Student>& student) {
    int failcount = 1; //로그인 실패 횟수 저장 
    while (failcount < 4) {
        string id = getInputId();
        string pw = getInputPassword();
        for (unsigned int i = 0; i < student.size(); i++) {
            if (id == student[i].Id && pw == student[i].Pw) {
                failcount = 0;
                return i;
            }
        }
        cout << "로그인 " << failcount << "회 실패 (3회 실패시 종료)" << endl;
        failcount++;
    }
    cout << "3회 실패하여 종료합니다." << endl;
    return -1;
}

int login(vector<Student>& student) {
    while (1) {
        int login_type;
        cout << " ---------<<로그인>>--------" << endl;
        cout << "|                           |" << endl;
        cout << "|     1. 학생 로그인        |" << endl;
        cout << "|     2. 관리자 로그인      |" << endl;
        cout << "|     0. 종료               |" << endl;
        cout << "|                           |" << endl;
        cout << " ---------------------------" << endl;
        cout << "로그인 종류를 선택해주세요 : ";
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
            cout << "숫자를 다시 입력해주세요" << endl;
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
    cout << "\t\t학번: " << student[user].Id << "    " << "이름: " << student[user].Name << "  대출 가능 횟수: " << student[user].Count << endl;
    cout << "---------------------------------------------------------------------------------" << endl;
}

void printBookInfo(vector<Student>& student, vector<Book>& book, const int& user) {
    if (user == -100)cout << "**********************************보유 도서 목록*********************************" << endl;
    else cout << "**********************************대출 도서 목록*********************************" << endl;
    cout << "=================================================================================" << endl;
    cout << setw(22) << "제목" << setw(18) << "|"
        << setw(12) << "작가" << setw(8) << "|"
        << setw(11) << "출판사" << setw(4) << "|"
        << setw(4) << " 권수" << "|" << endl;
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
        cout << "---------------------<<메뉴>>----------------------" << endl;
        cout << "|1. 도서검색                                      |" << endl;
        cout << "|2. 도서대출                                      |" << endl;
        cout << "|3. 대출목록                                      |" << endl;
        cout << "|4. 도서반납                                      |" << endl;
        cout << "|5. 비밀번호 변경                                 |" << endl;
        cout << "|6. 로그아웃                                      |" << endl;
        cout << "|0. 종료                                          |" << endl;
        cout << "---------------------------------------------------" << endl << "메뉴를 선택해주세요 :";

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
            cout << "계속하려면 아무키나 누르십시오...." << endl;
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
            cout << "잘못 입력하셨습니다." << endl;
            break;
        }
        continue;
    }
}

void applyBook(vector<Student>& student, vector<Book>& book, const int& user) {
    printStudent(student, user);
    printBookInfo(student, book, -100);
    while (1) {
        bool apply_possible = true;                      //대출 가능 여부 결정 변수
        int cnt = 0;                                       //대출 불가 요인이 여러 개일 때 조건검사 순서에 따라 한가지 요인만 출력하게끔 하는 변수
        string applybook_name;
        cout << "띄어쓰기를 구분하여 신청할 책의 이름을 입력해주세요.(0: 뒤로가기)>>";
        getline(cin, applybook_name);
        if (applybook_name == "0") break;
        int index = -1;
        index = findBookByName(book, applybook_name);

        if (index == -1) {
            cout << "잘못입력하셨습니다." << endl;                                              //책 이름이 아닌 다른 값을 입력한 경우
            cout << "계속하려면 아무 키나 누르십시오....";
            cin.ignore();
            getchar();
            continue;
        }
        if (book[index].Reserves == 0) {
            cout << "보유하고 있는 책이 없습니다." << endl;                                          //1. 보유 책 충분 여부
            apply_possible = false;
            continue;
        }
        if (cnt == 0) {
            if (student[user].Count == 0) {
                cout << "최대 2권까지 대출 가능합니다." << endl;                                   //2. 대출잔여 횟수 충분여부
                apply_possible = false;
                continue;
            }
        }
        if (cnt == 0) {
            for (int i = 0; i < 2 - student[user].Count; i++) {
                if (applybook_name == student[user].Appliedbooks[i]) {                                  //3. 중복여부
                    cout << "이미 해당 책을 대출했습니다." << endl;
                    apply_possible = false;
                    cnt++;
                    break;
                }
            }

        }
        if (apply_possible == true) {
            cout << '[' << applybook_name << ']' << "(을)를 성공적으로 대출하였습니다." << endl;
            book[index].Reserves--;                                             //보유 책 1권 감소
            student[user].Count--;       //학생의 대출 가능 횟수 감소
            student[user].Appliedbooks.push_back(book[index].bookName);    //학생의 대출목록 vector에 신청한 책 이름 추가            
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
        bool disapply_possible = false;         //책 반납 가능 여부 결정 변수
        cout << "반납할 책 제목(0: 뒤로가기)>>" << endl;
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
            cout << "책 이름이 올바르지 않습니다." << endl;
            cout << "계속하려면 아무키나 누르십시오...." << endl;
            cin.ignore();
            getchar();
            continue;
        }
        cout << book[book_index].bookName << "(을)를 반납하였습니다." << endl;
        book[book_index].Reserves++;
        student[user].Count++;                              //대출잔여 횟수 증가
        deleteElement(student[user].Appliedbooks, user_index);                                         //대여한 책 목록에서 해당 책 제거 
        printStudent(student, user);
        printBookInfo(student, book, user);
        cout << "계속하려면 아무 키나 누르십시오...." << endl;
        cin.ignore();
        getchar();
    }
}
void changePassword(vector<Student>& student, const int& user) {
    while (1) {
        string password;
        cout << "---------------------------------------------------" << endl << ">>";
        cout << "본인 확인을 위해 비밀번호를 한 번 더 입력해주세요." << endl;
        cout << "비밀번호: ";
        cin >> password;
        if (password == student[user].Pw) {
            cout << "---------------------------------------------------" << endl << ">>";
            cout << "새로 설정할 비밀번호를 입력하세요." << endl;
            cout << "비밀번호: ";
            cin >> password;
            cout << "변경되었습니다." << endl;
            student[user].Pw = password;
            cout << "---------------------------------------------------" << endl << ">>";
        }
        else cout << "비밀번호가 일치하지 않습니다." << endl;
        cout << "계속하려면 아무 키나 누르십시오....(0: 뒤로가기)" << endl;
        cin.ignore();
        if (getchar() == '0') break;;
    }
}

void searchBook(vector<Book>& book, const int& user, vector<Student>& student) {
    while (1) {
        string keyword;
        printBookInfo(student, book, user);
        cout << "검색할 키워드를 입력해 주세요(제목, 글쓴이, 출판사 검색 가능)    뒤로가기:0 " << endl << ">>";
        getline(cin, keyword);
        if (keyword == "0")break;
        cout << endl;
        bool exist = false;
        cout << "*************************************검색 결과***********************************" << endl;
        cout << "=================================================================================" << endl;
        cout << setw(22) << "제목" << setw(18) << "|"
            << setw(12) << "작가" << setw(8) << "|"
            << setw(11) << "출판사" << setw(4) << "|"
            << setw(4) << " 권수" << "|" << endl;
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
        if (!exist) cout << "검색한 책이 없습니다." << endl;
        cout << "계속하려면 아무 키나 누르십시오...";
        cin.ignore();
        getchar();

    }
}

int runAdmin(vector<Student>& student, vector<Book>& book) {
    while (1) {
        char runadmin_type;
        cout << "---------------------<<메뉴>>----------------------" << endl;
        cout << "|1. 학생 추가                                     |" << endl;
        cout << "|2. 도서 등록                                     |" << endl;
        cout << "|3. 도서 삭제                                     |" << endl;
        cout << "|4. 로그아웃                                      |" << endl;
        cout << "|0. 종료                                          |" << endl;
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
            cout << "잘못 입력하셨습니다." << endl;
            break;
        }
        continue;
    }
}

void addStudent(vector<Student>& student) {
    while (1) {
        string studentid, password, name;
        bool dup_check = false;       //학번 중복판단 변수
        cout << "---------------------------------------------------" << endl;
        cout << "학번 ";
        cin >> studentid;
        cout << "비밀번호 ";
        cin >> password;
        cout << "학생 이름: ";
        cin >> name;
        cout << "---------------------------------------------------" << endl;
        if (findStudentById(student, studentid) != -1) dup_check = true;
        if (dup_check == true) {
            cout << "이미 존재하는 학번입니다." << endl << "계속하려면 아무 키나 누르십시오....(0 :뒤로가기)" << endl;
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

            cout << "성공적으로 등록되었습니다." << endl << "계속하려면 아무 키나 누르십시오....(0 :뒤로가기)" << endl;
            cin.ignore();
            if (getchar() == '0') break;
        }
    }
}

void addBook(vector<Book>& book) {
    while (1) {
        string bookname, author, publisher;
        int reserve;
        bool dup_check = false;        //책 중복 검사 변수
        cout << "---------------------------------------------------" << endl;
        cout << "책 이름: ";
        getline(cin, bookname);

        cout << "저자: ";
        getline(cin, author);
        cout << "출판사: ";
        getline(cin, publisher);
        cout << "보유 권 수: ";
        while (!(cin >> reserve)) {
            cout << "정수를 입력해주세요" << endl;
            cin.clear();
            cin.ignore();
        }
        cout << "---------------------------------------------------" << endl;

        if (findBookByName(book, bookname) != -1) dup_check = true;
        if (dup_check == true) {
            cout << "이미 존재하는 책 입니다." << endl << "계속하려면 아무 키나 누르십시오....(0: 뒤로가기)" << endl;
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
            cout << "성공적으로 책을 등록하였습니다." << endl;
            break;
        }
    }
}

void deleteBook(vector<Book>& book, vector<Student>& student) {
    printBookInfo(student, book, -100);
    while (1) {
        bool delete_possible = false;     //책 등록 해제 가능 여부 판단
        int book_index = 0;
        string delete_book;
        cout << "등록해제할 책 이름 (0:뒤로가기) >>";
        getline(cin, delete_book);
        if (delete_book == "0") break;

        if (findBookByName(book, delete_book) != -1) {
            delete_possible = true;
            book_index = findBookByName(book, delete_book);
        }

        if (delete_possible == false) {
            cout << "일치하는 책이 없습니다." << endl;
            cout << "계속하려면 아무 키나 누르십시오...." << endl;
            cin.ignore();
            getchar();
            continue;
        }
        if (delete_possible == true) {
            for (unsigned int i = 0; i < student.size(); i++) {
                for (unsigned int j = 0; j < student[i].Appliedbooks.size(); j++) {
                    if (student[i].Appliedbooks[j] == delete_book) {
                        student[i].Appliedbooks.erase(student[i].Appliedbooks.begin() + j);//책이 등록해제되면 학생의 대여한 책 목록에서 해당 책을 삭제한다
                        student[i].Count++;
                        break;
                    }
                }
            }
            book.erase(book.begin() + book_index);  //책이 등록 해제됨에 따라 Book클래스를 저장하는 vector 에서 해당 index번째 객체를 삭제한다.  
            cout << "성공적으로 등록 해제 되었습니다." << endl << "게속하려면 아무 키나 누르십시오...." << endl;
            cin.ignore();
            getchar();
        }
    }
}