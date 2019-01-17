#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

#define MAX_FILENUM 100
string filecontent[MAX_FILENUM];

int filecontentno = 0;

struct UFD {
    string filename;
    int protectcode[3];
    int length = 0;
    int address;    //对应于全局中的文件内容下标号
//    UFD *prev = NULL;    //便于删除的操作
    UFD *next = NULL;
};

struct MFD {
    int MFD_no;
    string username;
    UFD *link = NULL;
    MFD *next = NULL;
};

struct AFD {
    string file_name;
    int fileno;
    int protectcode_enter[3];
//    int sign = -1;   //读操作为1，写操作为0，默认为-1;
    UFD *link = NULL;
};

AFD afd[5];
int AFD_openstatus[5];
MFD mfd[10];
MFD *MFD_head = &mfd[0];
int user_filenum[10];

void init() {
    for (int i = 0; i < 5; ++i) {
        afd[i].fileno = i;
    }
    mfd[0].username = "赵一";
    mfd[0].MFD_no = 0;
    mfd[0].link = NULL;
    mfd[0].next = &mfd[1];
    mfd[1].username = "钱二";
    mfd[1].MFD_no = 1;
    mfd[1].link = NULL;
    mfd[1].next = &mfd[2];
    mfd[2].username = "张三";
    mfd[2].MFD_no = 2;
    mfd[2].link = NULL;
    mfd[2].next = &mfd[3];
    mfd[3].username = "李四";
    mfd[3].MFD_no = 3;
    mfd[3].link = NULL;
    mfd[3].next = &mfd[4];
    mfd[4].username = "王五";
    mfd[4].MFD_no = 4;
    mfd[4].link = NULL;
    mfd[4].next = &mfd[5];
    mfd[5].username = "周六";
    mfd[5].MFD_no = 5;
    mfd[5].link = NULL;
    mfd[5].next = &mfd[6];
    mfd[6].username = "孙七";
    mfd[6].MFD_no = 6;
    mfd[6].link = NULL;
    mfd[6].next = &mfd[7];
    mfd[7].username = "朱八";
    mfd[7].MFD_no = 7;
    mfd[7].link = NULL;
    mfd[7].next = &mfd[8];
    mfd[8].username = "徐九";
    mfd[8].MFD_no = 8;
    mfd[8].link = NULL;
    mfd[8].next = &mfd[9];
    mfd[9].username = "曾十";
    mfd[9].MFD_no = 9;
    mfd[9].link = NULL;
    mfd[9].next = NULL;
}

string input_username() {
    cout << setw(50) << left << "YOUR NAME ?   " << flush;
    string name;
    cin >> name;
    return name;
}

bool check_username(string name) {
    MFD *temp = MFD_head;
    while (temp) {
        if (temp->username == name) {
            return true;
        } else {
            temp = temp->next;
        }
    }
    return false;
}

int get_UFD_no(string name) {
    MFD *temp = MFD_head;
    while (temp) {
        if (temp->username == name) {
            return temp->MFD_no;
        } else {
            temp = temp->next;
        }
    }
    return -1;
}

void print_filedirectory(string name) {
    MFD *use;
    MFD *temp = MFD_head;
    while (temp) {
        if (temp->username == name) {
            use = temp;
            break;
        } else {
            temp = temp->next;
        }
    }
    cout << "YOUR FILE DIRECTORY" << endl;
    cout << setw(16) << left << "FILE NAME" << setw(16) << left << "PROTECTION" << setw(16) << left << "CODE LENGTH"
         << endl;
    UFD *temp_1 = use->link;
    while (temp_1) {
        cout << setw(16) << left << temp_1->filename << temp_1->protectcode[0] << temp_1->protectcode[1]
             << temp_1->protectcode[2] << setw(13) << " " << setw(16) << left << temp_1->length << endl;
        temp_1 = temp_1->next;
    }
}

void command(string name, int MFD_no) {
    while (true) {
        string command = "";
        while (command != "CREATE" && command != "DELETE" && command != "OPEN" && command != "CLOSE" &&
               command != "READ" &&
               command != "WRITE" && command != "BYE") {
            cout << setw(50) << left << "COMMAND NAME? " << flush;
            cin >> command;
            if (command != "CREATE" && command != "DELETE" && command != "OPEN" && command != "CLOSE" &&
                command != "READ" &&
                command != "WRITE" && command != "BYE") {
                cout << "IT SHOULD BE ONE OF FOLLOWING : CREATE, DELETE, OPEN, CLOSE, READ, WRITE, BYE.TRY AGAIN!"
                     << endl;
            }
        }
        if (command == "CREATE") {
            if (user_filenum[MFD_no] < 10) {
                UFD* temp = new UFD;
                string file_name;
                int protectioncode;
                cout << setw(50) << left << "THE NEW FILE'S NAME(LESS THAN 9 CHARS)?" << flush;
                cin >> file_name;
                temp->filename = file_name;
                cout << setw(50) << left << "THE NEW FILE’S PROTECTION CODE?" << flush;
                cin >> protectioncode;
                cout << "THE NEW FILE IS CREATED." << endl;
                temp->protectcode[0] = protectioncode / 100;
                temp->protectcode[1] = (protectioncode / 10) % 10;
                temp->protectcode[2] = protectioncode % 10;
                temp->address = filecontentno++;
                //把新建的结构体挂到该用户的文件目录中
                temp->next = mfd[MFD_no].link;
                mfd[MFD_no].link = temp;
                user_filenum[MFD_no]++;
//                cout << mfd[MFD_no].link->filename << endl;
                if (temp->protectcode[2] == 1) {
                    cout << setw(50) << left << "ENTER THE OPEN MODE? " << flush;
                    int protectioncode_enter;
                    cin >> protectioncode_enter;
                    if (protectioncode_enter == 0 || protectioncode_enter == 1 || protectioncode_enter == 10 ||
                        protectioncode_enter == 11 || protectioncode_enter == 100 || protectioncode_enter == 101 ||
                        protectioncode_enter == 110 || protectioncode_enter == 111) {
                        int flag = -1;
                        for (int i = 0; i < 5; ++i) {
                            if (AFD_openstatus[i] == 0) {
                                AFD_openstatus[i] = 1;
                                flag = i;
                                break;
                            }
                        }
                        if (flag == -1) {
                            cout << "CAN'T OPEN MORE FILES" << endl;
                        } else {
                            if(protectioncode_enter / 100 > protectioncode / 100 || (protectioncode_enter/10)%10 > (protectioncode/10)%10 || protectioncode_enter % 10 > protectioncode % 10 ){
                                cout << "NO ENOUGH AUTHORITY" << endl;
                            } else {
                                afd[flag].file_name = file_name;
                                afd[flag].protectcode_enter[0] = (protectioncode_enter / 100) && (temp->protectcode[0]);
                                afd[flag].protectcode_enter[1] = ((protectioncode_enter / 10) % 10) && (temp->protectcode[1]);
                                afd[flag].protectcode_enter[2] = (protectioncode_enter % 10) && (temp->protectcode[2]);
                                afd[flag].link = temp;
                                cout << setw(50) << "THIS FILE IS OPENED,ITS OPEN NUMBER IS" << afd[flag].fileno + 1
                                     << endl;
                            }
                        }
                    } else{
                        cout << "INVALID INPUT" << endl;
                        cout << "EXIT" << endl;
                    }
                }
            } else {
                cout << "YOU CAN'T SAVE MORE FILES" << endl;
            }
        } else if (command == "DELETE") {
            MFD *temp = &mfd[MFD_no];
            string file_name;
            cout << setw(50) << left << "ENTER DELETE FILE NAME" << flush;
            cin >> file_name;
            UFD *temp_ufd = temp->link;
            UFD *prev_ufd;
            int sign = 0;
            int flag = 0;
            int signal = 0;
            while (temp_ufd) {
                if (temp_ufd->filename == file_name) {
                    sign = 1;
                    for (int i = 0; i < 5; ++i) {
                        if (AFD_openstatus[i] == 1 && afd[i].file_name == file_name) {
                            cout << "THE FILE IS OPENED,CLOSE IT BEFORE DELETE IT!" << endl;
                            signal = 1;
                            break;
                        }
                    }
                    if(signal == 1)
                        break;
                    user_filenum[MFD_no]--;
                    if (flag == 0) {
                        temp->link = temp_ufd->next;
                        cout << "FILE " << file_name << " HAS BEEN DELETED!" << endl;
                        break;
                    } else {
                        prev_ufd->next = temp_ufd->next;
                        cout << "FILE " << file_name << " HAS BEEN DELETED!" << endl;
                        break;
                    }
                } else {
                    flag = 1;
                    prev_ufd = temp_ufd;
                    temp_ufd = temp_ufd->next;
                }
            }
            if (sign == 0) {
                cout << "NO THIS FILE" << endl;
            }
        } else if (command == "READ") {
            int file_no;
            cout << setw(50) << left << "ENTER READ FILE NO" << flush;
            cin >> file_no;
            if(AFD_openstatus[file_no - 1] == 0){
                cout << "NO THIS FILE" << endl;
            } else {
                if (afd[file_no - 1].protectcode_enter[1] == 1) {
                    cout << filecontent[afd[file_no - 1].link->address] << endl;
                } else {
                    cout << "UNPERMITTED" << endl;
                }
            }
//            MFD *temp = &mfd[MFD_no];
//            string file_name;
//            cout << "ENTER READ FILE NAME" << flush;
//            cin >> file_name;
//            UFD *temp_ufd = temp->link;
//            int flag = 0;
//            while (temp_ufd) {
//                if (temp_ufd->filename == file_name) {
//                    flag = 1;
//                    if (temp_ufd->protectcode[1] == 1) {
//                        cout << filecontent[temp_ufd->address] << endl;
//                        break;
//                    } else {
//                        cout << "UNPERMITTED" << endl;
//                        break;
//                    }
//                } else {
//                    temp_ufd = temp_ufd->next;
//                }
//            }
//            if (flag == 0) {
//                cout << "NO THIS FILE" << endl;
//            }
        } else if (command == "WRITE") {
            int file_no;
            cout << setw(50) << left << "ENTER WRITE FILE NO" << flush;
            cin >> file_no;
            if(AFD_openstatus[file_no - 1] == 0){
                cout << "NO THIS FILE" << endl;
            } else {
                if (afd[file_no - 1].protectcode_enter[0] == 1) {
                    string input;
                    cout << "WRITE WHAT CONTENT TO THIS FILE?" << endl;
                    cin >> input;
                    filecontent[afd[file_no - 1].link->address].append(input);
                } else {
                    cout << "UNPERMITTED" << endl;
                }
            }
//            MFD *temp = &mfd[MFD_no];
//            string file_name;
//            cout << "ENTER WRITE FILE NAME" << flush;
//            cin >> file_name;
//            UFD *temp_ufd = temp->link;
//            int flag = 0;
//            while (temp_ufd) {
//                if (temp_ufd->filename == file_name) {
//                    flag = 1;
//                    if (temp_ufd->protectcode[0] == 1) {
//                        string input;
//                        cout << "WRITE WHAT CONTENT TO THIS FILE?" << endl;
//                        cin >> input;
//                        filecontent[temp_ufd->address].append(input);
//                        break;
//                    } else {
//                        cout << "UNPERMITTED" << endl;
//                        break;
//                    }
//                } else {
//                    temp_ufd = temp_ufd->next;
//                }
//            }
//            if (flag == 0) {
//                cout << "NO THIS FILE" << endl;
//            }
        } else if (command == "OPEN") {
            MFD *temp = &mfd[MFD_no];
            string file_name;
            cout << setw(50) << left << "ENTER OPEN FILE NAME" << flush;
            cin >> file_name;
            UFD *temp_ufd = temp->link;
            int flag = 0;
            while (temp_ufd) {
                if (temp_ufd->filename == file_name) {
                    flag = 1;
                    if (temp_ufd->protectcode[2] == 1) {
                        int sign = -1;
                        int mark = 0;
                        for (int j = 0; j < 5; ++j) {
                            if (AFD_openstatus[j] == 1 && afd[j].file_name == file_name) {
                                cout << "FILE ALREADY OPENED" << endl;
                                mark = 1;
                                break;
                            }
                        }
                        if (mark == 1)
                            break;
                        cout << setw(50) << left << "ENTER OPEN MODE CODE" << flush;
                        int protectioncode_enter;
                        cin >> protectioncode_enter;
                        for (int i = 0; i < 5; ++i) {
                            if (AFD_openstatus[i] == 0) {
                                AFD_openstatus[i] = 1;
                                sign = i;
                                break;
                            }
                        }
                        if (sign == -1) {
                            cout << "CAN'T OPEN MORE FILES" << endl;
                        } else {
                            afd[sign].file_name = file_name;
                            afd[sign].protectcode_enter[0] = protectioncode_enter / 100;
                            afd[sign].protectcode_enter[1] = (protectioncode_enter / 10) % 10;
                            afd[sign].protectcode_enter[2] = protectioncode_enter % 10;
                            afd[flag].link = temp_ufd;
                            cout << setw(50) << left << "THIS FILE IS OPENED,ITS OPEN NUMBER IS" << afd[sign].fileno + 1 << endl;
                        }
                        break;
                    } else {
                        cout << "UNPERMITTED" << endl;
                        break;
                    }
                } else {
                    temp_ufd = temp_ufd->next;
                }
            }
            if (flag == 0) {
                cout << "NO THIS FILE" << endl;
            }
        } else if (command == "CLOSE") {
            int file_no;
            cout << setw(50) << left << "ENTER CLOSE FILE NO" << flush;
            cin >> file_no;
            if(AFD_openstatus[file_no - 1] == 0){
                cout << "NO THIS FILE" << endl;
            } else {
                AFD_openstatus[file_no - 1] = 0;
                cout << "FILE NO " << file_no << " HAS CLOSED!" << endl;
            }
        } else if (command == "BYE") {
            print_filedirectory(name);
            cout << "GOOD BYE!" << endl;
            break;
        }
    }
}

int main() {
    init();
    cout << "RUN" << endl;
    string user_name;
    while (!check_username(user_name = input_username()))
        cout << "YOUR NAME IS NOT IN THE USER NAME TABLE, TRY AGAIN!" << endl;
    print_filedirectory(user_name);
    command(user_name, get_UFD_no(user_name));
    return 0;
}