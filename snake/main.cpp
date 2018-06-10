//
//  main.cpp
//  tanchishe
//
//  Created by 薛梓赫 on 2018/4/8.
//  Copyright © 2018年 薛梓赫. All rights reserved.
//

//贪吃蛇游戏
#include<stdio.h>
#include<stdlib.h>
#include <GLUT/GLUT.h>
#include<time.h>
#define UP 10 //上
#define DOWN -10//下
#define LEFT -1//左
#define RIGHT 1//右
#define NODE_WIDTH 10;//蛇身节点正方形的宽度
#define STEP_SIZE 10;//运动步长
#define PANEL_WIDTH  100;//面板长度
#define PANEL_HEIGHT  100;//面板宽度
int dir = RIGHT;//运动方向,初始化为右
struct node//节点结构
{
    int x;//x坐标
    int y;//y坐标
    struct node *prev;//前一个节点
    struct node *next;//后一个节点
};
struct snake//蛇结构
{
    struct node *head;//头部
    struct node *tail;//尾部
    int node_count;//节点个数
};
struct snake *sn;//蛇
struct node  *food;//食物
void snake_init()//蛇的初始化
{
    sn = (struct snake *)malloc(sizeof(struct snake));
    food = (struct node *)malloc(sizeof(struct node));
    food->x = 150;
    food->y = 100;
    node *head = (struct node *)malloc(sizeof(struct node));
    node *tail = (struct node *)malloc(sizeof(struct node));
    head->x = 100;
    head->y = 100;
    tail->x = 90;
    tail->y = 100;
    head->next = tail;
    tail->prev = head;
    sn->head = head;
    sn->tail = tail;
    sn->node_count = 2;
}
//添加一个节点到头部
void add_head(node *n)
{
    n->next = sn->head;
    sn->head->prev = n;
    sn->head = n;
}
//删除尾部
void remove_tail()
{
    node *old_tail = sn->tail;
    sn->tail = sn->tail->prev;//重新指定尾节点
    free(old_tail);//释放删除的尾节点
}
bool snake_contain(int x, int y)
{
    node *node = sn->head;
    for (int i = 0; i < sn->node_count; i++)
    {
        if ((abs(node->x - x)<=6&&abs(node->y - y)<=6))//食物与蛇身的距离不能同时小于6才不会碰到，反正同时小于6就会包含
        {
            return true;
        }
        node = node->next;
    }
    return false;
}
//新食物
void new_food()
{
    int width = PANEL_WIDTH;
    int height = PANEL_HEIGHT;
    int node_width = NODE_WIDTH;
    int x = rand()%width;
    int y = rand()%height;
    //要满足的条件
    // x < PANEL_WIDTH - NODE_WIDTH; PANEL_HEIGHT-NODE_WIDTH< y
    // abs(x- sn->head)&&abs(x->)
    while (snake_contain(x, y) && x < (height - node_width) && y>(height - node_width))
    {
        x = rand()%PANEL_WIDTH;
        y = rand()%PANEL_HEIGHT;
    }
    food->x = x;
    food->y = y;
}

bool is_eat(node * n)
{
    return abs(n->x - food->x)<6 &&abs(n->y - food->y)<6 ;
}
//前进
void step(int go_dir)
{
    if ((dir + go_dir) != 0)//如果不是朝相反的方向
    {
        dir = go_dir;
        //求蛇运动下一步的坐标点,这里运用了数字技巧
        int x = sn->head->x + (dir%10)*STEP_SIZE;
        int y = sn->head->y + (dir/10)*STEP_SIZE;
        struct node *n = (struct node *)malloc(sizeof(struct node));
        n->x = x;
        n->y = y;
        if (is_eat(n))
        {
            add_head(n);
            new_food();
            sn->node_count++;
        }
        else
        {
            add_head(n);
            remove_tail();
        }
        
    }
    
}
//重绘
void repaint(int a)
{
    //画蛇身
    node *node = sn->head;
    for (int i = 0; i < sn->node_count; i++)
    {
        int x = node->x;
        int y = node->y;
        int lx = x-NODE_WIDTH;
        int ly = y;
        int rx = x;
        int ry = y-NODE_WIDTH;
        //glColor3f(1.0f, 0.0f, 0.0f);
        glRectf(lx,ly,rx,ry);
        node = node->next;
    }
    //画食物
    int lx = food->x - NODE_WIDTH;
    int ly = food->y;
    int rx = food->x;
    int ry = food->y - NODE_WIDTH;
    glRectf(lx,ly,rx,ry);
}
void time_func(int a)
{
    step(dir);
    glutPostRedisplay();//重绘
    glutTimerFunc(100,time_func,0);
}


void Initial(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //设置窗口背景色为白色
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,500.0,0.0,400.0);
}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);//用当前背景色填充窗口
    glColor3f(1.0f, 0.0f, 0.0f);//设置当前的绘图颜色为红色
    //glRectf(50.0f, 100.0f, 60.0f, 90.0f);//绘制一个矩形
    /*glBegin(GL_TRIANGLES);
     glColor3f(1, 1, 0);
     glVertex2f(250, 400);
     glColor3f(0, 1, 1);
     glVertex2f(50, 300);
     glColor3f(1, 0, 1);
     glVertex2f(450, 300);
     glEnd();
     */
    repaint(dir);
    glFlush();
}

void key_turn(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w':
            step(UP);
            break;
        case 's':
            step(DOWN);
            break;
        case 'a':
            step(LEFT);
            break;
        case 'd':
            step(RIGHT);
            break;
    }
}
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    snake_init();
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);//初始化窗口的显示模式
    glutInitWindowSize(500, 500);//设置窗口的尺寸
    glutInitWindowPosition(100, 120); //设置窗口的位置
    glutCreateWindow("矩形");  //创建一个名为矩形的窗口
    glutDisplayFunc(Display);
    Initial();
    glutKeyboardFunc(key_turn);
    glutTimerFunc(100,time_func,0);//计数器
    glutMainLoop();
    return 0;
}
