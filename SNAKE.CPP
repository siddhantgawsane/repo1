#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<dos.h>
#include<graphics.h>
#include<time.h>
#include<ctype.h>
 
 
typedef struct pos_queue
{
int x, y;
struct pos_queue *next;
} pqueue;
 
 
void push(int, int);
void pop(int *, int *);
 
 
pqueue *front = NULL;
pqueue *rear = NULL;
 
 
void main()
{
int gd=0, gm=0;
int xmax, ymax, spd, ch, i;
int nx, ny, ox, oy, mov, nc = WHITE;
int ax, ay, bx, by;
int score=0,len=3,time=0, hscore, pts=50;
char num[10], name[20];
FILE *fp=NULL;
char dir = 'r';
 
 
initgraph(&gd, &gm, "C:\\BGI");
xmax = getmaxx();
ymax = getmaxy();
 
 
/////////////////////////
// INTRODUCTION SCREEN //
/////////////////////////
 
 
clrscr();
cleardevice();
settextstyle(4,0,7);
outtextxy(xmax/3, 0, "SNAKE");
settextstyle(1,0,1);
outtextxy(xmax/3, 64, "By Siddhant Gawsane & Girish Kamath");
cout<<"\n\n\n\n\n\n\n\n";
cout<<"Rules:\n1. Tradtional Snake game. Eat the apple to grow.\n2. Do not eat yourself.\n3. Do not cross white border\n4. Eat blue ball to earn bonus points without growing.\n   Hurry up, as the blue ball disappears after some time.\n\n\nPress any key to cntinue...";
getch();
 
 
/////////////////////////
// LEVEL CHOOSE SCREEN //
/////////////////////////
 
 
 
clrscr();
cleardevice();
cout<<"Choose Level:\n1. Super Slow\t(50 pts per apple, 150 pts bonus)\n2. Slow\t\t(100 pts per apple, 300 pts bonus)\n3. Medium\t(150 pts per apple, 450 pts bonus)\n4. Fast\t\t(200 pts per apple, 600 pts bonus)\n5. Very Fast\t(250 pts per apple, 750 pts bonus)\n6. Super Fast\t(300 pts per apple, 900 pts bonus)\n7. GODSPEED!!\t(500 pts per apple, 1500 pts bonus)\n\nEnter here:";
cin>>ch;
switch(ch)
{
case 1:
spd = 350;
pts = 50;
break;
case 2:
spd = 300;
pts = 100;
break;
case 3:
spd = 250;
pts = 150;
break;
case 4:
spd = 200;
pts = 200;
break;
case 5:
spd = 150;
pts = 250;
break;
case 6:
spd = 100;
pts = 300;
break;
case 7:
spd = 50;
pts = 500;
break;
default:
spd = 200;
pts = 200;
break;
}
 
 
///////////////////////////////
// GAMEPLAY INTERFACE SCREEN //
///////////////////////////////
 
 
clrscr();
cleardevice();
 
 
// SNAKE BOX //
line(0,20,xmax,20);
line(xmax,20, xmax, ymax);
line(xmax, ymax, 0, ymax);
line(0, ymax, 0, 20);
 
 
// RANDOM POSITION ALLOCATION OF FIRST APPLE //
while(1)
{
randomize();
ax = random(xmax / 20 - 1);
ay = random(ymax / 20 - 1);
ax*=20;
ay*=20;
ax+=10;
ay+=10;
if(ay==10)
ay=30;
nc = getpixel(ax,ay);
if(nc==WHITE) // COLLISION RESOLUTION
continue;
break;
}
 
 
setcolor(RED);
circle(ax,ay,8);
setfillstyle(1,RED);
floodfill(ax,ay,RED);
setcolor(WHITE);
 
 
// SNAKE INITIALISATION
 
 
push(10,30);
push(30,30);
push(50,30);
circle(10,30,8);
circle(30,30,8);
circle(50,30,8);
setfillstyle(1,WHITE);
floodfill(10,30,WHITE);
floodfill(30,30,WHITE);
floodfill(50,30,WHITE);
nx = 50;
ny = 30;
 
 
 
while(1)
{
flushall();
if(kbhit())   // int kbhit(void); from conio.h returns 1 if keyboard buffer is not empty
{
while(kbhit())
mov = getch(); // char getch(void); fetches character from stdin buffer
fflush(stdin);
flushall();
 
 
if(mov==27)
goto gameover; // label to end of game
if(mov==72 && dir!='d')
dir='u';
if(mov==75 && dir!='r')
dir='l';
if(mov==80 && dir!='u')
dir='d';
if(mov==77 && dir!='l')
dir='r';
if(mov==' ')
{
cout<<"\rPAUSED                                                   ";
getch();
}
 
 
 
}
 
 
 
if(dir=='r')
nx+=20;
if(dir=='l')
nx-=20;
if(dir=='u')
ny-=20;
if(dir=='d')
ny+=20;
 
 
nc = getpixel(nx,ny);	// check next pixel colour
if(nc==WHITE)	 // if snake is at next position, end game
goto gameover;
if(nx<0 || nx>xmax || ny<20 || ny>ymax)	// if next position is outside box, end game
goto gameover;
if(nc==BLUE)	 // if next position is blue, bonus point awarded.
{
score+=(3*pts);
time=0;
}
if(nc!=RED)	 // if next position is not red, delete last position
pop(&ox, &oy);
else
{
while(1)
{
randomize();
ax = random(xmax / 20 - 1);
ay = random(ymax / 20 - 1);
ax*=20;
ay*=20;
ax+=10;
ay+=10;
if(ay==10)
ay=30;
nc = getpixel(ax,ay);
if(nc==WHITE || nc==RED || nc == BLUE || ax>xmax || ay>ymax) // immediate collision resolution
{
if(ax<xmax/2)
ax+=60;
else
ax-=60;
if(ay<ymax/2)
ay+=60;
else
ay-=60;
 
 
nc = getpixel(ax,ay);
}
if(nc==WHITE || nc==RED || nc == BLUE || ax>xmax || ay>ymax) // if collision persists, repeat previous steps
continue;
break;
}
len++;	 // increment length
score += pts;	// add points to score
 
 
 
setcolor(RED);
circle(ax,ay,8);
setfillstyle(1,RED);
floodfill(ax,ay,RED);
setcolor(WHITE);
if((len-3)%5==0)
{
while(1)
{
time=15;	 // set timer for bonus point
randomize();
bx = random(xmax / 20 - 1);
by = random(ymax / 20 - 1);
bx*=20;
by*=20;
bx+=10;
by+=10;
if(by==10)
by=30;
nc = getpixel(bx,by);
if(nc==WHITE || nc==RED || bx>xmax || by>ymax) // immediate collision resolution
{
if(bx<xmax/2)
bx+=60;
else
bx-=60;
if(by<ymax/2)
by+=60;
else
by-=60;
nc = getpixel(bx,by);
}
 
 
if(nc==WHITE || nc==RED || bx>xmax || by>ymax) // if collision persistes, continue loop
continue;
break;
}
 
 
setcolor(BLUE);
circle(bx,by,8);
setfillstyle(1,BLUE);
floodfill(bx,by,BLUE);
setcolor(WHITE);
}
 
 
 
}
setfillstyle(1,WHITE);
circle(nx,ny,8);
floodfill(nx,ny,WHITE);
push(nx, ny);
 
 
setfillstyle(1,BLACK);
floodfill(ox, oy, BLACK);
 
 
// refresh box, just in case snake or piece collides
line(0,20,xmax,20);
line(xmax,20, xmax, ymax);
line(xmax, ymax, 0, ymax);
line(0, ymax, 0, 20);
if(time>0)
{
time--;   // decrement bonus timer
if(time==0)
{
setcolor(BLACK);
setfillstyle(1,BLACK);
floodfill(bx,by,BLACK);
setcolor(WHITE);
}
}
cout<<"\rYOUR SCORE: "<<score<<". Press [SPACE] to Pause.";
delay(spd);
}
 
 
/////////////////////////
/// GAME OVER SCREEN ////
/////////////////////////
 
 
gameover:
clrscr();
cleardevice();
fflush(stdin);
settextstyle(10,0,5);
outtextxy(xmax/4,ymax/3,"GAME OVER");
getch();
 
 
/////////////////////////
/// HIGH SCORE SCREEN ///
/////////////////////////
 
 
clrscr();
cleardevice();
moveto((xmax/2)-4, ymax);
cout<<"YOUR SCORE:";
cout<<score;
fp=fopen("hiscore.txt", "r");
if(fp!=NULL)
{
i=0;
do
{
dir = fgetc(fp);
num[i] = dir;
i++;
}while(isalnum(dir));
num[i] = '\0';
hscore = atoi(num);
 
 
}
if(fp==NULL || score>=hscore)
{
fclose(fp);
if(score==hscore)
cout<<"\n\nYOU HAVE EQUALLED THE HIGH SCORE!!";
else
cout<<"\n\nYOU HAVE A HIGH SCORE!!";
fp = fopen("hiscore.txt", "w+");
cout<<"\nEnter your name: ";
cin>>name;
fprintf(fp, "%d %s", score, name);
fclose(fp);
cout<<"\n\nCONGRATULATIONS!!!";
}
else if(fp!=NULL && hscore>score)
{
cout<<"\n\nPrevious high score: "<<hscore;
cout<<"\nHigh score holder: ";
while(dir!=EOF)
{
dir = fgetc(fp);
cout<<dir;
}
}
fclose(fp);
cout<<"\n\n\nTHANK YOU FOR PLAYING :)";
getch();
 
 
}
 
 
void push(int x, int y)
{
pqueue *p;
p = (pqueue *)malloc(sizeof(pqueue));
p->x = x;
p->y = y;
p->next = NULL;
if(front==NULL)
{
front = p;
rear = p;
return;
}
front->next = p;
front = p;
}
 
 
void pop(int *x, int *y)
{
pqueue *p;
p = rear;
*x = p->x;
*y = p->y;
rear = rear->next;
free(p);
}
 
 
/*
 
 DIRECTION ASCII
 
     72
75	80 77
 
 
*/