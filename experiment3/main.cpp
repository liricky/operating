#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <math.h>

using namespace std;

#define MAX_RMSIZE 32

int Page[256];
int Pageno[256] = {0};
int pos1[64];
int pos2[64];
struct pageinfo {
    int pagenum;
    int phynum;
    pageinfo *next = NULL;
    int firstappear;
};

bool test1(int i, int temp) {
    for (int j = 0; j < i - 1; ++j) {
        if (pos1[j] == temp)
            return true;
    }
    return false;
}

bool test2(int i, int temp) {
    for (int j = 0; j < i - 1; ++j) {
        if (pos2[j] == temp)
            return true;
    }
    return false;
}

void Pro_Page() {
    int ran[16];
    for (int i = 0; i < 16; ++i) {
        ran[i] = rand() % 32768;
    }
    for (int j = 0; j < 16; ++j) {
        for (int i = j + 1; i < 16; ++i) {
            if (ran[j] > ran[i]) {
                int temp = ran[j];
                ran[j] = ran[i];
                ran[i] = temp;
            }
        }
    }
    for (int i = 0; i < 64; ++i) {
        int temp = rand() % 128;
        while (test1(i, temp))
            temp = rand() % 128;
        pos1[i] = temp;
    }
    for (int j = 0; j < 64; ++j) {
        for (int i = j + 1; i < 64; ++i) {
            if (pos1[j] > pos1[i]) {
                int temp = pos1[j];
                pos1[j] = pos1[i];
                pos1[i] = temp;
            }
        }
    }
    for (int k = 0; k < 64; ++k) {
        Page[pos1[k]] = ran[k % 16];
    }
    for (int l = 0; l < 128; ++l) {
        if (Page[l] == -1)
            Page[l] = rand() % 32768;
    }
    for (int i = 0; i < 64; ++i) {
        int temp = 128 + rand() % 128;
        while (test2(i, temp))
            temp = 128 + rand() % 128;
        pos2[i] = temp;
    }
    for (int j = 0; j < 64; ++j) {
        for (int i = j + 1; i < 64; ++i) {
            if (pos2[j] > pos2[i]) {
                int temp = pos2[j];
                pos2[j] = pos2[i];
                pos2[i] = temp;
            }
        }
    }
    for (int k = 0; k < 64; ++k) {
        Page[pos2[k]] = ran[k % 16];
    }
    for (int l = 128; l < 256; ++l) {
        if (Page[l] == -1)
            Page[l] = rand() % 32768;
    }
}

void Print_Create() {
    for (int i = 0; i < 256; ++i) {
        cout << "a[" << setw(3) << left << i << "]=" << setw(10) << Page[i] << flush;
        if ((i + 1) % 4 == 0)
            cout << endl;
    }
    cout << "===============================================================" << endl;
}


void Print_Page(int pagesize) {
    cout << "PAGE NUMBER WITH SIZE " << pagesize << "k FOR EACH ADRESS IS:" << endl;
    for (int i = 0; i < 256; ++i) {
        Pageno[i] = ceil(Page[i] / 1024.0 / pagesize);
        cout << "pageno[" << setw(3) << left << i << "]=" << setw(10) << Pageno[i] << flush;
        if ((i + 1) % 4 == 0)
            cout << endl;
    }
    cout << "===============================================================" << endl;
}

//void opt() {
//    int time = 0;
//    int pagesize;
//    int rmsize;
//    cout << "虚存容量：32K" << endl;
//    cout << "页面尺寸(1-8K)：" << flush;
//    cin >> pagesize;
//    cout << "实存容量(4-32页)：" << flush;
//    cin >> rmsize;
//    int rm[MAX_RMSIZE];
//    int last[MAX_RMSIZE];
//    int now_size = 0;
//    Print_Page(pagesize);
//    for (int i = 0; i < 256; ++i) {
//        int flag = 0;
//        for (int m = 0; m < now_size; ++m) {
//            if (rm[m] == Pageno[i]) {
//                flag = 1;
//                break;
//            }
//        }
//        if (flag == 1)
//            continue;
//        if (now_size < rmsize) {
//            rm[now_size] = Pageno[i];
//            now_size++;
//        } else {
//            for (int j = 0; j < now_size; ++j) {
//                for (int k = i + 1; k < 256; ++k) {
//                    if (Pageno[k] == rm[j]) {
//                        last[j] = k;
//                        break;
//                    }
//                }
//            }
//            int max = 0;
//            for (int l = 1; l < now_size; ++l) {
//                if (last[l] > last[max])
//                    max = l;
//            }
//            rm[max] = Pageno[i];
//            time++;
//        }
//    }
//    cout << "页面失效次数：" << time << endl;
//    cout << "命中率=" << (1 - time / 256.0) << endl;
//}

void opt() {
    int time = 0;
    int pagesize;
    int rmsize;
    cout << "虚存容量：32K" << endl;
    cout << "页面尺寸(1-8K)：" << flush;
    cin >> pagesize;
    cout << "实存容量(4-32页)：" << flush;
    cin >> rmsize;
    pageinfo exam[MAX_RMSIZE];
    for (int n = 0; n < rmsize; ++n) {
        exam[n].phynum = n;
    }
    int now_size = 0;
    pageinfo *head = &exam[0];
    pageinfo *temp = NULL;
    Print_Page(pagesize);
    for (int i = 0; i < 256; ++i) {
        int flag = 0;
        temp = head;
        while (temp != NULL) {
            if (temp->pagenum == Pageno[i]) {
                flag = 1;
                break;
            } else {
                temp = temp->next;
            }
        }
        if (flag == 1)
            continue;
        temp = head;
        if (now_size < rmsize) {
            exam[now_size].pagenum = Pageno[i];
            now_size++;
            if (now_size != rmsize)
                exam[now_size - 1].next = &exam[now_size];
        } else {
            while (temp != NULL) {
                for (int k = i + 1; k < 256; ++k) {
                    if (Pageno[k] == temp->pagenum) {
                        temp->firstappear = k;
                        break;
                    }
                }
                temp = temp->next;
            }
            pageinfo *max = &exam[0];
            temp = head->next;
            while (temp != NULL) {
                if (temp->firstappear > max->firstappear)
                    max = temp;
                temp = temp->next;
            }
            max->pagenum = Pageno[i];
            time++;
        }
    }
    cout << "页面失效次数：" << time << endl;
    cout << "命中率=" << (1 - time / 256.0) << endl;
}

//void lru() {
//    int time = 0;
//    int pagesize;
//    int rmsize;
//    cout << "虚存容量：32K" << endl;
//    cout << "页面尺寸(1-8K)：" << flush;
//    cin >> pagesize;
//    cout << "实存容量(4-32页)：" << flush;
//    cin >> rmsize;
//    int rm[MAX_RMSIZE];
//    int now_size = 0;
//    Print_Page(pagesize);
//    for (int i = 0; i < 256; ++i) {
//        int flag = 0;
//        for (int m = 0; m < now_size; ++m) {
//            if (rm[m] == Pageno[i]) {
//                flag = 1;
//                int temp;
//                temp = rm[m];
//                rm[m] = rm[now_size - 1];
//                rm[now_size - 1] = temp;
//                break;
//            }
//        }
//        if (flag == 1)
//            continue;
//        if (now_size < rmsize) {
//            rm[now_size] = Pageno[i];
//            now_size++;
//        } else {
//            rm[now_size] = Pageno[i];
//            time++;
//        }
//    }
//    cout << "页面失效次数：" << time << endl;
//    cout << "命中率=" << (1 - time / 256.0) << endl;
//}

void lru() {
    int time = 0;
    int pagesize;
    int rmsize;
    cout << "虚存容量：32K" << endl;
    cout << "页面尺寸(1-8K)：" << flush;
    cin >> pagesize;
    cout << "实存容量(4-32页)：" << flush;
    cin >> rmsize;
    int rm[MAX_RMSIZE];
    int now_size = 0;
    Print_Page(pagesize);
    for (int i = 0; i < 256; ++i) {
        int flag = 0;
        for (int m = 0; m < now_size; ++m) {
            if (rm[m] == Pageno[i]) {
                flag = 1;
                int temp;
                temp = rm[m];
                rm[m] = rm[now_size - 1];
                rm[now_size - 1] = temp;
                break;
            }
        }
        if (flag == 1)
            continue;
        if (now_size < rmsize) {
            rm[now_size] = Pageno[i];
            now_size++;
        } else {
            rm[now_size] = Pageno[i];
            time++;
        }
    }
    cout << "页面失效次数：" << time << endl;
    cout << "命中率=" << (1 - time / 256.0) << endl;
}

int main() {
    for (int j = 0; j < 256; ++j) {
        Page[j] = -1;
    }
    for (int i = 0; i < 64; ++i) {
        pos1[i] = -1;
        pos2[i] = -1;
    }
    cout << "自动生成指令流" << endl;
    srand(time(NULL));
    Pro_Page();
    Print_Create();
    cout << "The algorithm is:" << flush;
    string function;
    cin >> function;
    if (function == "opt")
        opt();
    else if (function == "lru")
        lru();
    else {
        cout << "error input" << endl;
    }

    return 0;
}