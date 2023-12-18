#include "widget.h"
#include<QPushButton>
#include <QApplication>
#include <QMouseEvent>
#include <unistd.h>
#include<random>
#include <QMessageBox>
#include<iostream>

#define MAX_SIZE 20
#define REAL_SIZE 13

void rechange(int stu, int x, int y);
bool checkIfMove(int x, int y, int toX, int toY);
void choiceBestAns();
void moveLeft(int sta1, int sta2, int l);
void moveRight(int sta1, int sta2, int l);
void choiceRandom();
bool checkIfLose(int sta);
void creatWinWIndow();
void creatLoseWindow();
void choice();


int leftPointX = -1, leftPointY = -1;
int rightPointX = -1, rightPointY = -1;
int stau[MAX_SIZE][MAX_SIZE];
int diffic = 4;


class Point : public QPushButton {
public :
    Point(const QString &text, QWidget *parent = nullptr,
          int x = 0, int y = 0, int posX = 0, int posY = 0) : QPushButton(text, parent) {
        this->x = x; this->y = y; this->status = 0;
        setGeometry(posX, posY, 30, 30);
    }
    void changeNone() {
        setStyleSheet("background-color:none");
        this->status = 0;
    }
    void changeGray() {
        setStyleSheet("background-color:gray");
        this->status = 1;
    }
    void changeRed() {
        setStyleSheet("background-color:red");
        this->status = 2;
    }
    void changeChoice() {
        setStyleSheet("background-color:pink");
    }
protected:
    void mousePressEvent(QMouseEvent *event) override {
        if(event->button() == Qt::LeftButton) {
            if(rightPointX == -1 && rightPointY == -1)
                return;
            leftPointX = this->x; leftPointY = this->y;
            if(! checkIfMove(rightPointX, rightPointY, leftPointX, leftPointY)) {
                rechange(stau[rightPointY][rightPointX], rightPointX, rightPointY);
                rightPointX = -1; rightPointY = -1; leftPointX = -1; leftPointY = -1;
                return;
            }
            rechange(0, rightPointX, rightPointY);
            rechange(stau[rightPointY][rightPointX], leftPointX, leftPointY);
            int tem = stau[rightPointY][rightPointX];
            stau[leftPointY][leftPointX] = tem;
            stau[rightPointY][rightPointX] = 0;
            leftPointX = -1; leftPointY = -1; rightPointX = -1; rightPointY = -1;
            if(checkIfLose(2)) {
                creatWinWIndow(); return;
            }
            choice();
            if(checkIfLose(1)) {
                creatLoseWindow(); return;
            }
        }
        else if(event->button() == Qt::RightButton){
            if(stau[y][x] == 2) return;
            if(rightPointX != -1 && rightPointY != -1) {
                rechange(stau[rightPointY][rightPointX], rightPointX, rightPointY);
            }
            rightPointX = this->x; rightPointY = this->y;
            changeChoice();
        }
    }
private :
    int x, y;
    int status; // 0 : none, 1 : gray, 2 : red
};


Point* info[MAX_SIZE][MAX_SIZE];

void rechange(int stu, int x, int y) {
    switch (stu) {
        case 0:
            info[y][x]->changeNone();
            break;
        case 1:
            info[y][x]->changeGray();
            break;
        case 2:
            info[y][x]->changeRed();
    }
}


void creatWinWIndow() {
    QMessageBox::information(nullptr, "WIN!!!", "恭喜你取得胜利");
}

void creatLoseWindow() {
    QMessageBox::information(nullptr, "LOSE~~", "啊噢， 你输了~~~");
}

bool checkIfMove(int x, int y, int toX, int toY) {
    if(stau[toY][toX] != 0) return false;
    if(x != toX && y != toY) return false;
    if(x == toX && y == toY) return false;
    if(stau[y][x] == 0) return false;
    if(y != toY) return false;
    int sta = stau[y][x];
    int beg = toX < x ? toX : x;
    int end = toX < x ? x : toX;
    for(int i = beg + 1 ; i <= end ; i ++) {
        if(stau[y][i] != 0 && stau[y][i] != sta)
            return false;
    }
    return true;
}

void move(int x, int y, int toX, int toY) {
    int tem = stau[y][x];
    stau[y][x] = stau[toY][toX];
    stau[toY][toX] = tem;
    info[y][x]->changeChoice();
    rechange(stau[toY][toX], toX, toY);
    rechange(stau[y][x], x, y);
}

bool checkifLineLose(int i, int sta) {
    if(stau[i][1] == sta || stau[i][REAL_SIZE] == sta) {
        if(stau[i][1] == sta) {
            if(stau[i][2] != 0)
                return true;
        }else {
            if(stau[i][REAL_SIZE - 1] != 0)
                return true;
        }
    }
    return false;
}

bool checkIfLose(int sta) {
    bool ans = true;
    for(int i = 1 ; i <= REAL_SIZE ; i ++) {
        if(! checkifLineLose(i, sta))
            ans = false;
    }
    return ans;
}

void moveLeft(int sta1, int sta2, int l) {
    std::default_random_engine e;
    e.seed(time(0));
    int llen;
    if(sta1 < sta2)
        llen = sta2 - sta1 - 1;
    else  llen = sta2 - 1;
    if(llen != 0) {
        int moveLen = e() % llen; moveLen ++;
        move(sta2, l, sta2 - moveLen, l);
    } else
        moveRight(sta1, sta2, l);
}

void moveRight(int sta1, int sta2, int l) {
    std::default_random_engine e;
    e.seed(time(0));
    int llen;
    if(sta1 < sta2)
        llen = REAL_SIZE - sta2;
    else  llen = sta1 - sta2;
    if(llen > 0) {
        int moveLen = e() % llen; moveLen ++;
        assert(e() >= 0);
        assert(moveLen >= 0);
        assert(sta2 + moveLen <= REAL_SIZE);
        move(sta2, l, sta2 + moveLen, l);
    } else{
        moveLeft(sta1, sta2, l);
    }
}

void choiceRandom() {
    std::default_random_engine e;
    e.seed(time(0));
    int l= (e() % REAL_SIZE);
    for(int i = 0, j = l ; i < REAL_SIZE ; i++, j ++, j %= REAL_SIZE) {
        if(! checkifLineLose(j + 1, 2)) {
            int sta1, sta2;
            for(int k = 1 ; k <= REAL_SIZE ; k ++) {
                if(stau[j + 1][k] == 1)
                    sta1 = k;
                if(stau[j + 1][k] == 2)
                    sta2 = k;
            }
            int dir = e() >> 5 & 1;
            if(dir == 0) moveLeft(sta1, sta2, j + 1);
            else moveRight(sta1, sta2, j + 1);
            break;
        }
    }
}

void choiceBestAns() {
    int len[MAX_SIZE];
    for(int i = 1 ; i <= REAL_SIZE ; i++) {
        int sta1, sta2;

        for(int j = 1 ; j <= REAL_SIZE ; j ++) {
            if(stau[i][j] == 1)
                sta1 = j;
            if(stau[i][j] == 2)
                sta2 = j;

        }
        len[i] = abs(sta1 - sta2) - 1;
    }
    int ans = 0;
    for(int i = 1 ; i <= REAL_SIZE ; i++)
        ans ^= len[i];

    if(ans == 0) {
        choiceRandom(); return;
    }
    int l, del;
    for(int i = 1 ; i <= REAL_SIZE ; i ++) {
        if((ans ^ len[i]) >= len[i]) continue;
        l = i; del = len[i] - (ans ^ len[i]);
        break;
    }

    int sta1, sta2;
    for(int j = 1 ; j <= REAL_SIZE ; j ++) {
        if(stau[l][j] == 1)
            sta1 = j;
        if(stau[l][j] == 2)
            sta2 = j;
    }

    if(sta2 > sta1)
        move(sta2, l, sta2 - del, l);
    else
        move(sta2, l, sta2 + del, l);
}

int pos = 0;
void choice() {
    int cc[5][10] = {
        {0,0,0,0,0,0,0,0,0,0}, // 0
        {0,0,1,0,0,0,0,1,0,0}, // 2
        {0,0,1,0,1,0,0,0,1,1}, // 4
        {0,1,1,0,1,0,1,0,1,1}, // 6
        {1,1,1,1,1,1,1,1,1,1}, // 10
    };
    if(cc[diffic][pos] == 1)
        choiceBestAns();
    else choiceRandom();
    pos ++; pos %= 10;
}
void changeDif(int num) {
    diffic = num;
}


void Init() {
    for(int i = 1 ; i <= REAL_SIZE ; i ++) {
        for(int j = 1 ; j <= REAL_SIZE ; j ++)
           stau[i][j] = 0;
    }

    std::default_random_engine e;
    e.seed(time(0));
    for(int i = 1 ; i <= REAL_SIZE ; i ++) {
        int num1 = e(), num2 = e();
        num1 %= REAL_SIZE; num1 ++;
        num2 %= REAL_SIZE; num2 ++;
        if(num1 == num2) {
            if(num1 + 1 <= REAL_SIZE) num1 ++;
            else num1 --;
        }
        stau[i][num1] = 1; stau[i][num2] = 2;
    }

    for(int i = 1 ; i <= REAL_SIZE ; i ++) {
        for(int j = 1 ; j <= REAL_SIZE ; j ++) {
            switch (stau[i][j]) {
            case 0:
                info[i][j]->changeNone();
                break;
            case 1:
                info[i][j]->changeGray();
                break;
            case 2:
                info[i][j]->changeRed();
                break;
            }
        }
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    int beginX = 50, beginY = 210;
    for(int i = 1 ; i <= REAL_SIZE ; i ++) {
        for(int j = 1 ; j <= REAL_SIZE ; j++) {
            info[i][j] = new Point("", &w, j, i, beginX + (j - 1) * 30, beginY + (i - 1) * 30);
        }
    }
    stau[1][1] = stau[2][2] = stau[5][9] = 1;
    stau[8][1] = stau[7][2] = stau[3][9] = 2;
    Init();
    w.show();
    return a.exec();
}

void Widget::on_pushButton_2_clicked(){
    changeDif(0);
    Init();
}

void Widget::on_pushButton_3_clicked(){
    changeDif(1);
    Init();
}

void Widget::on_pushButton_4_clicked(){
    changeDif(2);
    Init();
}

void Widget::on_pushButton_5_clicked(){
    changeDif(3);
    Init();
}

void Widget::on_pushButton_6_clicked(){
    changeDif(4);
    Init();
}

void Widget::on_pushButton_clicked(){
    QMessageBox::information(nullptr, "游戏规则",
                             "在一个正方形的棋盘上，你持灰子，AI持红子，你先行动，每次你可以将你的棋子移动到同一行的任何一个空格上，当然这过程中不许越过该行的敌方棋子。双方轮流移动，直到某一方无法行动为止，移动最后一步的玩家获胜\n\n\n"
                             "操作：你按右键选择棋子， 左键放置棋子");
}
