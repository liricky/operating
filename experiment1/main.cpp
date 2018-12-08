#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

struct PCB {
    int id;
    PCB *link = NULL;
    int pri;
    int use;
    int need;
    char status;
    int haveuse;
};

int main() {
    while(true) {
        PCB p[5];
        p[0].id = 1;
        p[1].id = 2;
        p[2].id = 3;
        p[3].id = 4;
        p[4].id = 5;

        //实验书的样例
//        p[0].pri = 9;
//        p[1].pri = 38;
//        p[2].pri = 30;
//        p[3].pri = 29;
//        p[4].pri = 0; //使用简单轮转法时，不能为0

        //自动生成样例
        //优先数法
//        for (int k = 0; k < 5; ++k) {
//            p[k].pri = rand()%4+1;
//        }
        //简单轮转法
//        for (int k = 0; k < 5; ++k) {
//            p[k].pri = rand()%4 + 2;
//        }

        p[0].pri = 1;
        p[1].pri = 0;
        p[2].pri = 1;
        p[3].pri = 2;
        p[4].pri = 0;

        p[0].use = 0;
        p[1].use = 0;
        p[2].use = 0;
        p[3].use = 0;
        p[4].use = 0;

        //实验书的样例
//        p[0].need = 3;
//        p[1].need = 3;
//        p[2].need = 6;
//        p[3].need = 3;
//        p[4].need = 4;

        //自动生成样例
//        for (int l = 0; l < 5; ++l) {
//            p[l].need = rand()%8 + 1;
//        }
        //FB反馈排队法
        for (int l = 0; l < 5; ++l) {
            p[l].need = rand()%6 + 1;
        }

        p[0].status = 'W';
        p[1].status = 'W';
        p[2].status = 'W';
        p[3].status = 'W';
        p[4].status = 'W';
        p[0].haveuse = 0;
        p[1].haveuse = 0;
        p[2].haveuse = 0;
        p[3].haveuse = 0;
        p[4].haveuse = 0;
        PCB *RUN = NULL;
        PCB *HEAD = NULL;
        PCB *TAIL = NULL;
        cout << "TYPE THE ALGORITHM:" << flush;
        string type;
        cin >> type;
        if (type == "PRIORITY") {
            cout << "OUTPUT OF " << type << endl;
            cout << "===============================" << endl;
            int q = 0;
            int a[5] = {0, 1, 2, 3, 4};
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 4; ++j) {
                    if (p[a[j]].pri < p[a[j + 1]].pri) {
                        int temp;
                        temp = a[j];
                        a[j] = a[j + 1];
                        a[j + 1] = temp;
                    }
                }
                q++;
            }
            HEAD = &p[a[0]];
            p[a[0]].link = &p[a[1]];
            p[a[1]].link = &p[a[2]];
            p[a[2]].link = &p[a[3]];
            p[a[3]].link = &p[a[4]];
            p[a[4]].link = NULL;
            TAIL = &p[a[4]];
            while (HEAD != NULL) {
                RUN = HEAD;
                RUN->status = 'R';
                HEAD = HEAD->link;
                cout << "RUNNING PROC    WAITING QUEUE" << endl;
                cout << RUN->id << "               " << flush;
                PCB *tmp = HEAD;
                if (tmp == NULL)
                    cout << setw(4) << "EMP" << flush;
                else
                    while (tmp != NULL) {
                        cout << setw(4) << tmp->id << flush;
                        tmp = tmp->link;
                    }
                cout << endl;
                cout << "===============================" << endl;
                cout << "ID              " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].id << flush;
                }
                cout << endl;
                cout << "PRIORITY        " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].pri << flush;
                }
                cout << endl;
                cout << "CPUTIME         " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].use << flush;
                }
                cout << endl;
                cout << "ALLTIME         " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].need << flush;
                }
                cout << endl;
                cout << "STATE           " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].status << flush;
                }
                cout << endl;
                cout << "NEXT            " << flush;
                for (int i = 0; i < 5; ++i) {
                    if (p[i].link != NULL)
                        cout << setw(4) << p[i].link->id << flush;
                    else
                        cout << setw(4) << 0 << flush;
                }
                cout << endl;
                cout << "===============================" << endl;
                RUN->use++;
                RUN->need--;
                RUN->pri -= 3;
                if (RUN->need == 0) {
                    RUN->status = 'F';
                    RUN->link = NULL;
                    RUN = NULL;
                } else {
                    RUN->status = 'W';
                    if (HEAD != NULL) {
                        PCB *next = HEAD;
                        PCB *last = NULL;
                        if (RUN->pri > HEAD->pri) {
                            RUN->link = HEAD;
                            HEAD = RUN;
                        } else {
                            int flag = 0;
                            while (next != NULL) {
                                if (RUN->pri > next->pri) {
                                    last->link = RUN;
                                    RUN->link = next;
                                    flag = 1;
                                    break;
                                } else {
                                    last = next;
                                    next = next->link;
                                }
                            }
                            if (flag == 0) {
                                last->link = RUN;
                                TAIL = RUN;
                                RUN->link = NULL;
                            }
                            RUN = NULL;
                        }
                    } else {
                        HEAD = RUN;
                    }
                }
                if (HEAD == NULL && RUN == NULL) {
                    cout << "RUNNING PROC    WAITING QUEUE" << endl;
                    cout << "EMP" << "             " << flush;
                    cout << setw(4) << "EMP" << flush;
                    cout << endl;
                    cout << "===============================" << endl;
                    cout << "ID              " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].id << flush;
                    }
                    cout << endl;
                    cout << "PRIORITY        " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].pri << flush;
                    }
                    cout << endl;
                    cout << "CPUTIME         " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].use << flush;
                    }
                    cout << endl;
                    cout << "ALLTIME         " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].need << flush;
                    }
                    cout << endl;
                    cout << "STATE           " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].status << flush;
                    }
                    cout << endl;
                    cout << "NEXT            " << flush;
                    for (int i = 0; i < 5; ++i) {
                        if (p[i].link != NULL)
                            cout << setw(4) << p[i].link->id << flush;
                        else
                            cout << setw(4) << 0 << flush;
                    }
                    cout << endl;
                    cout << "===============================" << endl;
                }
            }
            cout << "所有进程调度完成，算法结束" << endl;
            cout << "===============================" << endl;
            cout << endl;
        } else if (type == "ROUNDROBIN") {
            cout << "OUTPUT OF " << type << endl;
            cout << "===============================" << endl;
            HEAD = &p[0];
            p[0].link = &p[1];
            p[1].link = &p[2];
            p[2].link = &p[3];
            p[3].link = &p[4];
            p[4].link = NULL;
            TAIL = &p[4];
            while(HEAD != NULL){
                RUN = HEAD;
                HEAD = HEAD->link;
                RUN->status = 'R';
                cout << "RUNNING PROC    WAITING QUEUE" << endl;
                cout << RUN->id << "               " << flush;
                PCB *tmp = HEAD;
                if (tmp == NULL)
                    cout << setw(4) << "EMP" << flush;
                else
                    while (tmp != NULL) {
                        cout << setw(4) << tmp->id << flush;
                        tmp = tmp->link;
                    }
                cout << endl;
                cout << "===============================" << endl;
                cout << "ID              " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].id << flush;
                }
                cout << endl;
                cout << "PRIORITY        " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].pri << flush;
                }
                cout << endl;
                cout << "CPUTIME         " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].use << flush;
                }
                cout << endl;
                cout << "ALLTIME         " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].need << flush;
                }
                cout << endl;
                cout << "STATE           " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].status << flush;
                }
                cout << endl;
                cout << "NEXT            " << flush;
                for (int i = 0; i < 5; ++i) {
                    if (p[i].link != NULL)
                        cout << setw(4) << p[i].link->id << flush;
                    else
                        cout << setw(4) << 0 << flush;
                }
                cout << endl;
                cout << "===============================" << endl;
                RUN->need--;
                RUN->use++;
                if(RUN->need == 0){
                    RUN->haveuse = 0;
                    RUN->link = NULL;
                    RUN->status = 'F';
                    RUN = NULL;
                } else{
                    RUN->status = 'W';
                    RUN->haveuse++;
                    if(HEAD == NULL){
                        HEAD = RUN;
                        RUN = NULL;
                    } else {
                        if (RUN->haveuse == RUN->pri) {
                            RUN->haveuse = 0;
                            TAIL->link = RUN;
                            TAIL = TAIL->link;
                            TAIL->link = NULL;
                            RUN = NULL;
                        } else {
                            RUN->link = HEAD;
                            HEAD = RUN;
                            RUN = NULL;
                        }
                    }
                }
                if (HEAD == NULL && RUN == NULL) {
                    cout << "RUNNING PROC    WAITING QUEUE" << endl;
                    cout << "EMP" << "             " << flush;
                    cout << setw(4) << "EMP" << flush;
                    cout << endl;
                    cout << "===============================" << endl;
                    cout << "ID              " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].id << flush;
                    }
                    cout << endl;
                    cout << "PRIORITY        " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].pri << flush;
                    }
                    cout << endl;
                    cout << "CPUTIME         " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].use << flush;
                    }
                    cout << endl;
                    cout << "ALLTIME         " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].need << flush;
                    }
                    cout << endl;
                    cout << "STATE           " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].status << flush;
                    }
                    cout << endl;
                    cout << "NEXT            " << flush;
                    for (int i = 0; i < 5; ++i) {
                        if (p[i].link != NULL)
                            cout << setw(4) << p[i].link->id << flush;
                        else
                            cout << setw(4) << 0 << flush;
                    }
                    cout << endl;
                    cout << "===============================" << endl;
                }
            }
            cout << "所有进程调度完成，算法结束" << endl;
            cout << "===============================" << endl;
            cout << endl;
        } else if (type == "FB"){
            cout << "OUTPUT OF " << type << endl;
            cout << "===============================" << endl;
            int v = 0;
            PCB *HEAD1 = NULL;
            PCB *HEAD2 = NULL;
            PCB *HEAD3 = NULL;
            PCB *TAIL1 = NULL;
            PCB *TAIL2 = NULL;
            PCB *TAIL3 = NULL;
            PCB *tep = NULL;
            int count1 = 0;
            int count2 = 0;
            int count3 = 0;
            for (int i = 0; i < 5; ++i) {
                if(p[i].pri == 0 && v == 0){
                    v = 1;
                    HEAD1 = &p[i];
                    tep = &p[i];
                    count1++;
                } else if(p[i].pri == 0 && v == 1){
                    tep->link = &p[i];
                    tep = tep->link;
                    count1++;
                }
            }
            TAIL1 = tep;
            v = 0;
            for (int i = 0; i < 5; ++i) {
                if(p[i].pri == 1 && v == 0){
                    v = 1;
                    HEAD2 = &p[i];
                    tep->link = &p[i];
                    tep = tep->link;
                    count2++;
                } else if(p[i].pri == 1 && v == 1){
                    tep->link = &p[i];
                    tep = tep->link;
                    count2++;
                }
            }
            TAIL2 = tep;
            v = 0;
            for (int i = 0; i < 5; ++i) {
                if(p[i].pri == 2 && v == 0){
                    v = 1;
                    HEAD3 = &p[i];
                    tep->link = &p[i];
                    tep = tep->link;
                    count3++;
                } else if(p[i].pri == 2 && v == 1){
                    tep->link = &p[i];
                    tep = tep->link;
                    count3++;
                }
            }
            TAIL3 = tep;
            tep->link = NULL;
            int time1 = 1;
            while(count1 != 0) {
                count1--;
                RUN = HEAD1;
                RUN->status = 'R';
                HEAD1 = HEAD1->link;
                cout << "RUNNING PROC    WAITING QUEUE" << endl;
                cout << RUN->id << "               " << flush;
                PCB *tmp = HEAD1;
                if (tmp == NULL)
                    cout << setw(4) << "EMP" << flush;
                else
                    while (tmp != NULL) {
                        cout << setw(4) << tmp->id << flush;
                        tmp = tmp->link;
                    }
                cout << endl;
                cout << "===============================" << endl;
                cout << "ID              " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].id << flush;
                }
                cout << endl;
                cout << "PRIORITY        " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].pri << flush;
                }
                cout << endl;
                cout << "CPUTIME         " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].use << flush;
                }
                cout << endl;
                cout << "ALLTIME         " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].need << flush;
                }
                cout << endl;
                cout << "STATE           " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].status << flush;
                }
                cout << endl;
                cout << "NEXT            " << flush;
                for (int i = 0; i < 5; ++i) {
                    if (p[i].link != NULL)
                        cout << setw(4) << p[i].link->id << flush;
                    else
                        cout << setw(4) << 0 << flush;
                }
                cout << endl;
                cout << "===============================" << endl;
                if(RUN->need - time1 >= 0) {
                    RUN->use += time1;
                    RUN->need -= time1;
                } else{
                    RUN->use += RUN->need;
                    RUN->need = 0;
                }
                if (RUN->need <= 0) {
                    RUN->link = NULL;
                    RUN->status = 'F';
                    RUN = NULL;
                } else {
                    RUN->status = 'W';
                    TAIL2->link = RUN;
                    TAIL2 = TAIL2->link;
                    TAIL2->link = HEAD3;
                    RUN = NULL;
                    count2++;
                }
                if (HEAD2 == NULL) {
                    cout << "RUNNING PROC    WAITING QUEUE" << endl;
                    cout << "EMP" << "             " << flush;
                    cout << setw(4) << "EMP" << flush;
                    cout << endl;
                    cout << "===============================" << endl;
                    cout << "ID              " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].id << flush;
                    }
                    cout << endl;
                    cout << "PRIORITY        " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].pri << flush;
                    }
                    cout << endl;
                    cout << "CPUTIME         " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].use << flush;
                    }
                    cout << endl;
                    cout << "ALLTIME         " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].need << flush;
                    }
                    cout << endl;
                    cout << "STATE           " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].status << flush;
                    }
                    cout << endl;
                    cout << "NEXT            " << flush;
                    for (int i = 0; i < 5; ++i) {
                        if (p[i].link != NULL)
                            cout << setw(4) << p[i].link->id << flush;
                        else
                            cout << setw(4) << 0 << flush;
                    }
                    cout << endl;
                    cout << "===============================" << endl;
                }
            }
            int time2 = 2;
            while(count2 != 0){
                count2--;
                RUN = HEAD2;
                RUN->status = 'R';
                HEAD2 = HEAD2->link;
                cout << "RUNNING PROC    WAITING QUEUE" << endl;
                cout << RUN->id << "               " << flush;
                PCB *tmp = HEAD2;
                if (tmp == NULL)
                    cout << setw(4) << "EMP" << flush;
                else
                    while (tmp != NULL) {
                        cout << setw(4) << tmp->id << flush;
                        tmp = tmp->link;
                    }
                cout << endl;
                cout << "===============================" << endl;
                cout << "ID              " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].id << flush;
                }
                cout << endl;
                cout << "PRIORITY        " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].pri << flush;
                }
                cout << endl;
                cout << "CPUTIME         " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].use << flush;
                }
                cout << endl;
                cout << "ALLTIME         " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].need << flush;
                }
                cout << endl;
                cout << "STATE           " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].status << flush;
                }
                cout << endl;
                cout << "NEXT            " << flush;
                for (int i = 0; i < 5; ++i) {
                    if (p[i].link != NULL)
                        cout << setw(4) << p[i].link->id << flush;
                    else
                        cout << setw(4) << 0 << flush;
                }
                cout << endl;
                cout << "===============================" << endl;
                if(RUN->need - time2 >= 0) {
                    RUN->use += time2;
                    RUN->need -= time2;
                } else{
                    RUN->use += RUN->need;
                    RUN->need = 0;
                }
                if(RUN->need <= 0){
                    RUN->link = NULL;
                    RUN->status = 'F';
                    RUN = NULL;
                } else {
                    RUN->status = 'W';
                    TAIL3->link = RUN;
                    TAIL3 = TAIL3->link;
                    TAIL3->link = NULL;
                    RUN = NULL;
                    count3++;
                }
                if (HEAD3 == NULL) {
                    cout << "RUNNING PROC    WAITING QUEUE" << endl;
                    cout << "EMP" << "             " << flush;
                    cout << setw(4) << "EMP" << flush;
                    cout << endl;
                    cout << "===============================" << endl;
                    cout << "ID              " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].id << flush;
                    }
                    cout << endl;
                    cout << "PRIORITY        " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].pri << flush;
                    }
                    cout << endl;
                    cout << "CPUTIME         " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].use << flush;
                    }
                    cout << endl;
                    cout << "ALLTIME         " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].need << flush;
                    }
                    cout << endl;
                    cout << "STATE           " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].status << flush;
                    }
                    cout << endl;
                    cout << "NEXT            " << flush;
                    for (int i = 0; i < 5; ++i) {
                        if (p[i].link != NULL)
                            cout << setw(4) << p[i].link->id << flush;
                        else
                            cout << setw(4) << 0 << flush;
                    }
                    cout << endl;
                    cout << "========================(=======" << endl;
                }
            }
            int time3 = 4;
            while(HEAD3 != NULL){
                RUN = HEAD3;
                RUN->status = 'R';
                HEAD3 = HEAD3->link;
                cout << "RUNNING PROC    WAITING QUEUE" << endl;
                cout << RUN->id << "               " << flush;
                PCB *tmp = HEAD3;
                if (tmp == NULL)
                    cout << setw(4) << "EMP" << flush;
                else
                    while (tmp != NULL) {
                        cout << setw(4) << tmp->id << flush;
                        tmp = tmp->link;
                    }
                cout << endl;
                cout << "===============================" << endl;
                cout << "ID              " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].id << flush;
                }
                cout << endl;
                cout << "PRIORITY        " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].pri << flush;
                }
                cout << endl;
                cout << "CPUTIME         " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].use << flush;
                }
                cout << endl;
                cout << "ALLTIME         " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].need << flush;
                }
                cout << endl;
                cout << "STATE           " << flush;
                for (int i = 0; i < 5; ++i) {
                    cout << setw(4) << p[i].status << flush;
                }
                cout << endl;
                cout << "NEXT            " << flush;
                for (int i = 0; i < 5; ++i) {
                    if (p[i].link != NULL)
                        cout << setw(4) << p[i].link->id << flush;
                    else
                        cout << setw(4) << 0 << flush;
                }
                cout << endl;
                cout << "===============================" << endl;
                if(RUN->need - time3 >= 0) {
                    RUN->use += time3;
                    RUN->need -= time3;
                } else{
                    RUN->use += RUN->need;
                    RUN->need = 0;
                }
                if(RUN->need <= 0){
                    RUN->link = NULL;
                    RUN->status = 'F';
                    RUN = NULL;
                } else {
                    RUN->status = 'W';
                    TAIL3->link = RUN;
                    TAIL3 = TAIL3->link;
                    TAIL3->link = NULL;
                    RUN = NULL;
                }
                if (HEAD3 == NULL) {
                    cout << "RUNNING PROC    WAITING QUEUE" << endl;
                    cout << "EMP" << "             " << flush;
                    cout << setw(4) << "EMP" << flush;
                    cout << endl;
                    cout << "===============================" << endl;
                    cout << "ID              " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].id << flush;
                    }
                    cout << endl;
                    cout << "PRIORITY        " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].pri << flush;
                    }
                    cout << endl;
                    cout << "CPUTIME         " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].use << flush;
                    }
                    cout << endl;
                    cout << "ALLTIME         " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].need << flush;
                    }
                    cout << endl;
                    cout << "STATE           " << flush;
                    for (int i = 0; i < 5; ++i) {
                        cout << setw(4) << p[i].status << flush;
                    }
                    cout << endl;
                    cout << "NEXT            " << flush;
                    for (int i = 0; i < 5; ++i) {
                        if (p[i].link != NULL)
                            cout << setw(4) << p[i].link->id << flush;
                        else
                            cout << setw(4) << 0 << flush;
                    }
                    cout << endl;
                    cout << "===============================" << endl;
                }
            }
            cout << "所有进程调度完成，算法结束" << endl;
            cout << "===============================" << endl;
            cout << endl;
        } else if (type == "exit") {
            cout << "退出" << endl;
            cout << "===============================" << endl;
            cout << endl;
            break;
        } else {
            cout << "error input,try again!" << endl;
            cout << "===============================" << endl;
            cout << endl;
        }
    }
    return 0;
}