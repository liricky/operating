#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <math.h>

using namespace std;

#define MAX_RMSIZE 32

int Page[256] = {0};
int Pageno[256] = {0};
int count = 0;

void Pro_Page() {
    int m = 0;
    m = rand() % 32768;
    Page[count] = m;
    count++;
    if (count == 256)
        return;
    Page[count] = m + 1;
    count++;
    if (count == 256)
        return;

    int m1 = 0;
    m1 = rand() % m;
    Page[count] = m1;
    count++;
    if (count == 256)
        return;

    int m2 = 0;
    m2 = (m + 2) + rand() % (32768 - (m + 2));
    Page[count] = m2;
    count++;
    if (count == 256)
        return;
    Pro_Page();
}

void Print_Create() {
    for (int i = 0; i < 256; ++i) {
        cout << "a[" << setw(3) << left << i << "]=" << setw(10) << Page[i] << flush;
        if ((i + 1) % 4 == 0)
            cout << endl;
    }
    cout << "===============================================================" << endl;
}

bool test() {
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; ++j) {
            if (i != j && Page[i] == Page[j]) {
                return true;
            }
        }
    }
    return false;
}

void clear() {
    for (int i = 0; i < 256; ++i) {
        Page[i] = 0;
    }
    count = 0;
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

void opt() {
    int time = 0;
    int pagesize;
    int rmsize;
    cout << "虚存容量：32K" << endl;
    cout << "页面尺寸(1-8K)：" << flush;
    cin >> pagesize;
    cout << "实存容量(4-32页)：" << flush;
    cin >> rmsize;
    int rm[MAX_RMSIZE];
    int last[MAX_RMSIZE];
    int now_size = 0;
    Print_Page(pagesize);
    for (int i = 0; i < 256; ++i) {
        int flag = 0;
        for (int m = 0; m < now_size; ++m) {
            if (rm[m] == Pageno[i]) {
                flag = 1;
                break;
            }
        }
        if (flag == 1)
            continue;
        if (now_size < rmsize) {
            rm[now_size] = Pageno[i];
            now_size++;
        } else {
            for (int j = 0; j < now_size; ++j) {
                for (int k = i + 1; k < 256; ++k) {
                    if (Pageno[k] == rm[j]) {
                        last[j] = k;
                        break;
                    }
                }
            }
            int max = 0;
            for (int l = 1; l < now_size; ++l) {
                if (last[l] > last[max])
                    max = l;
            }
            rm[max] = Pageno[i];
            time++;
        }
    }
    cout << "页面失效次数：" << time << endl;
    cout << "命中率=" << (1 - time / 256.0) << endl;
}

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
    cout << "自动生成指令流" << endl;
    srand(time(NULL));
    do {
        clear();
        Pro_Page();
    } while (test());
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