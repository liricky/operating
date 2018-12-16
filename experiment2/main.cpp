#include <iostream>
#include <iomanip>
#include <time.h>
#include <stdlib.h>

using namespace std;

const int MAX_TIME = 20;
const int MAX_PROG = 10;
int ERROR = 0;
int option;
int resource;
int number;
int p[MAX_PROG][MAX_TIME];
int big[MAX_PROG];
int large[MAX_PROG];
int count[MAX_PROG];

void input() {
    cout << "INPUT:" << endl;
    do{
        cout << "OPTION(0表示选用“防止死锁”算法;1表示不用“防止死锁”算法)=" << flush;
        cin >> option;
        if(option != 0 && option != 1){
            cout << "输入无效" << endl;
        }
    }while(option != 0 && option != 1);
    cout << "RESOURCE=" << flush;
    cin >> resource;
    cout << "NUMBER OF PROGRESS=" << flush;
    cin >> number;
    for (int k = 0; k < number; ++k) {
        count[k] = 0;
    }
    for (int j = 0; j < number; ++j) {
        int i = 0;
        big[j] = 0;
        cout << "CLAIM OF PROCESS " << j + 1 << " IS:" << flush;
        do {
            cin >> p[j][i++];
            if(p[j][i-1] > 0){
                big[j] += p[j][i-1];
            }
            count[j]++;
        } while (p[j][i - 1]);
        //以0作为输入结尾标志位
    }
    for (int j = 0; j < number; ++j) {
        cout << "MAXCLAIM OF PROCESS " << j + 1 << " IS:" << flush;
        cin >> large[j];
    }
    for (int j = 0; j < number; ++j) {
        if(big[j] > resource){
            cout << "必然会发生死锁" << endl;
            ERROR = 1;
            return;
        }
    }
    int small = 0;
    for (int j = 0; j < number; ++j) {
        small += big[j];
    }
    if(small <= resource){
        cout << "必然不会发生死锁" << endl;
        ERROR = 1;
        return;
    }
    return;
}

void title(){
    cout << "THE SYSTEM ALLOCTION PROCESS IS AS FOLLOWS:" << endl;
    cout << std::left << setw(5) << " " << std::left << setw(12) << "PROCESS" << std::left << setw(10) << "CLAIM" << std::left << setw(15) << "ALLOCATION" << "REMAINDER" << endl;
    return;
}

void deadlock(){
    int temp = 1;
    int allo[number];
    int status[number];
    int times[number];
    int remain = resource;
    for (int i = 0; i < number; ++i) {
        allo[i] = 0;
        status[i] = 0;
        times[i] = 0;
    }
    srand(time(NULL));
    while(true){
        int ran;
        int check = 0;
        for (int i = 0; i < number; ++i) {
            check += status[i];
        }
        if(check == number) {
            cout << "THE WHOLE WORK IS COMPLETED" << endl;
            cout << "***********************************" << endl;
            break;
        }
        while(true){
            ran = random()%number;
            if(status[ran] != 1)
                break;
        }
        if(allo[ran] + p[ran][times[ran]] > large[ran]){
            cout << '(' << temp << left << setw(3) << ')' << right << setw(4) << ran+1 << setw(8) << " " << right << setw(3) << p[ran][times[ran]] << setw(7) << " " << right << setw(6) << allo[ran] << setw(9) << " " << right << setw(5) << remain << endl;
            cout << "CLAIM IS BIGGER THAN MAXCLAIM" << endl;
            cout << "***********************************" << endl;
            break;
        } else {
            if (remain - p[ran][times[ran]] >= 0) {
                allo[ran] += p[ran][times[ran]];
                remain -= p[ran][times[ran]];
                cout << '(' << temp++ << left << setw(3) << ')' << right << setw(4) << ran + 1 << setw(8) << " "
                     << right << setw(3) << p[ran][times[ran]] << setw(7) << " " << right << setw(6) << allo[ran]
                     << setw(9) << " " << right << setw(5) << remain << endl;
                if (p[ran][times[ran]] > 0) {
                    cout << "RESOURCE IS ALLOCATED TO PROCESS " << ran + 1 << endl;
                    times[ran]++;
                } else if (p[ran][times[ran]] < 0) {
                    cout << "PROGRESS " << ran + 1 << " RETURN ITS RESOURCE" << endl;
                    times[ran]++;
                } else {
                    cout << "PROGRESS " << ran + 1 << " HAS  FINISHED" << endl;
                    status[ran] = 1;
                }
            } else {
                cout << '(' << temp << left << setw(3) << ')' << right << setw(4) << ran + 1 << setw(8) << " " << right
                     << setw(3) << p[ran][times[ran]] << setw(7) << " " << right << setw(6) << allo[ran] << setw(9)
                     << " " << right << setw(5) << remain << endl;
                cout << "THE REMAINDER IS LESS THAM PROCESS " << ran + 1 << " CLAIMS" << endl;
                cout << "DEADLOCK OCCURS!" << endl;
                cout << "***********************************" << endl;
                break;
            }
        }
    }
    return;
}

void pretenddeadlock(){
    int temp = 1;
    int allo[number];
    int need[number];
    int status[number];
    int times[number];
    int point[number];
    int remain = resource; //remain == available
    for (int i = 0; i < number; ++i) {
        allo[i] = 0;
        status[i] = 0;
        times[i] = 0;
        point[i] = 0;
        need[i] = large[i]; //应该是最大需求减去已分配
    }
    srand(time(NULL));
    while(true){
        int ran;
        int check1 = 0;
        int check2 = 0;
        for (int i = 0; i < number; ++i) {
            check1 += status[i];
        }
        if(check1 == number) {
            cout << "THE WHOLE WORK IS COMPLETED" << endl;
            cout << "***********************************" << endl;
            break;
        }
        for (int l = 0; l < number; ++l) {
            if(status[l] == 1)
                check2++;
            else
                check2 += point[l];
        }
        if(check2 == number){
            cout << "THE WHOLE WORK CAN'T COMPLETED,DEADLOCK ALWAYS OCCUR" << endl;
            cout << "***********************************" << endl;
            break;
        }
        while(true){
            ran = random()%number;
            if(status[ran] != 1 && point[ran] == 0)
                break;
        }
        if(allo[ran] + p[ran][times[ran]] > large[ran]){
            cout << '(' << temp << left << setw(3) << ')' << right << setw(4) << ran+1 << setw(8) << " " << right << setw(3) << p[ran][times[ran]] << setw(7) << " " << right << setw(6) << allo[ran] << setw(9) << " " << right << setw(5) << remain << endl;
            cout << "CLAIM IS BIGGER THAN MAXCLAIM" << endl;
            cout << "***********************************" << endl;
            break;
        } else {
            int work = remain - p[ran][times[ran]];
            need[ran] -= p[ran][times[ran]];
            allo[ran] += p[ran][times[ran]];
            int finish[number];
            for (int k = 0; k < number; ++k) {
                finish[k] = 1;
            }
            int i = 0;
            int flag;
            while(i<number){
                flag = i;
                for (int j = 0; j < number; ++j) {
                    if(need[j] <= work && finish[j]){
                        work += allo[j];
                        finish[j] = 0;
                        i++;
                    }
                }
                if(flag == i){
                    flag = -1;
                    break;
                }
            }
            allo[ran] -= p[ran][times[ran]];
            if(flag == -1){
                cout << '(' << temp << left << setw(3) << ')' << right << setw(4) << ran + 1 << setw(8) << " "
                     << right << setw(3) << p[ran][times[ran]] << setw(7) << " " << right << setw(6) << allo[ran]
                     << setw(9) << " " << right << setw(5) << remain << endl;
                cout << "DO PROGRESS " << ran+1 << " IS UNSAFE,RELEASE" << endl;
                point[ran] = 1;
                need[ran] += p[ran][times[ran]];
                continue;
            } else {
                for (int j = 0; j < number; ++j) {
                    point[j] = 0;
                }
                if (remain - p[ran][times[ran]] >= 0) {
                    allo[ran] += p[ran][times[ran]];
                    remain -= p[ran][times[ran]];
                    if(p[ran][times[ran]] == 0){
                        remain += allo[ran];
                        allo[ran] = 0;
                        cout << '(' << temp++ << left << setw(3) << ')' << right << setw(4) << ran + 1 << setw(8) << " "
                             << right << setw(3) << p[ran][times[ran]] << setw(7) << " " << right << setw(6)
                             << allo[ran]
                             << setw(9) << " " << right << setw(5) << remain << endl;
                    } else {
                        cout << '(' << temp++ << left << setw(3) << ')' << right << setw(4) << ran + 1 << setw(8) << " "
                             << right << setw(3) << p[ran][times[ran]] << setw(7) << " " << right << setw(6)
                             << allo[ran]
                             << setw(9) << " " << right << setw(5) << remain << endl;
                    }
                        if (p[ran][times[ran]] > 0) {
                        cout << "RESOURCE IS ALLOCATED TO PROCESS " << ran + 1 << endl;
                        times[ran]++;
                    } else if (p[ran][times[ran]] < 0) {
                        cout << "PROGRESS " << ran + 1 << " RETURN ITS RESOURCE" << endl;
                        times[ran]++;
                    } else {
                        cout << "PROGRESS " << ran + 1 << " HAS  FINISHED" << endl;
                        status[ran] = 1;
                    }
                } else {
                    cout << '(' << temp << left << setw(3) << ')' << right << setw(4) << ran + 1 << setw(8) << " "
                         << right
                         << setw(3) << p[ran][times[ran]] << setw(7) << " " << right << setw(6) << allo[ran] << setw(9)
                         << " " << right << setw(5) << remain << endl;
                    cout << "THE REMAINDER IS LESS THAM PROCESS " << ran + 1 << " CLAIMS" << endl;
                    cout << "DEADLOCK OCCURS!" << endl;
                    cout << "***********************************" << endl;
                    break;
                }
            }
        }
    }
    return;
}

int main() {
    input();
    if (ERROR == 1)
        return 0;
    else{
        title();
        if(option == 0)
            pretenddeadlock();
        else
            deadlock();
    }
    return 0;
}

//数据：四个进程
/*
1 2 3 2 -1 -3 0
1 4 1 1 1 -2 0
1 1 5 2 -2 -2 0
2 3 1 2 -2 -3 0
8
8
9
8

1 2 3 2 -1 -3 0
1 4 1 1 1 -2 0
1 1 5 2 -2 0
2 3 1 2 -2 -3 0
8
8
9
8
*/