#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<dos.h>
#include<string.h>

char buffer[1024];
int len;
char fname[20];
void newFile();
void open();
void save();
void fileMenu();
void editMenu();
void helpMenu();
void menuInput();


void clearbuffer()
{
	buffer[0]='/0';
	len=0;
}

void border()
{
	int n,i;
	clrscr();
	window(0,0,80,25);
	textbackground(BLACK);
	textcolor(WHITE);

	n=201;
	printf("\n\n%c",n);
	n=205;
	for(i=0;i<78;i++)
		printf("%c",n);
	n=187;
	printf("%c",n);
	n=186;
	for(i=4;i<49;i++)
	{
		gotoxy(80,i);
		printf("%c",n);
	}
	n=188;
	gotoxy(80,i);
	printf("%c",n);
	n=205;
	for(i=79;i>1;i--)
	{
		gotoxy(i,49);
		printf("%c",n);
	}
	n=200;
	gotoxy(i,49);
	printf("%c",n);

	n=186;
	for(i=4;i<49;i++)
	{
		gotoxy(1,i);
		printf("%c",n);
	}
}

void menu()
{
	gotoxy(4,2);
	textbackground(WHITE);
	textcolor(BLACK);
	cprintf(" FILE ");

	gotoxy(12,2);
	textbackground(WHITE);
	textcolor(BLACK);
	cprintf(" EDIT ");

	gotoxy(20,2);
	textbackground(WHITE);
	textcolor(BLACK);
	cprintf(" HELP ");

	gotoxy(34,3);
	cprintf("%s",fname);

	textbackground(BLACK);
	textcolor(WHITE);

}

void restore()
{
	clrscr();
	int i;
	border();
	menu();
//	menuInput();

	gotoxy(2,5);
	for(i=0;i<len;i++)
	{
		if(buffer[i]=='^'||wherex()==80)	gotoxy(2,(wherey()+2));//cprintf("%c",13);
		else	cprintf("%c",buffer[i]);
	}
}


void menuInput()
{
	char ch;
	gotoxy(2,5);

	while(1)
	{
		ch=getch();
		if (ch==0)
		{
			ch=getch();
			switch(ch)
			{
				case 33:
					fileMenu();
					break;
				case 18:
					editMenu();
					break;
				case 35:
					helpMenu();
					break;
			}
		}
		else
		{
			switch(ch)
			{
				case 13:
					buffer[len++]='^';
					restore();
					break;
				case 8:
					if(wherex()==2)
					{
						gotoxy(78,(wherey()-1));
					}
					else
					{
						len--;
						restore();
					}
					break;
				default:
					buffer[len++]=ch;
					restore();
					break;
			}
		}
	}
}

void draw(int x)
{
	int curry,i;
	char ch;

	border();
	menu();

	textbackground(BLACK);
	textcolor(WHITE);

	gotoxy(x,4);
	cprintf("%c",201);
	for(i=(x+1);i<(x+13);i++)
	{
		gotoxy(i,4);
		cprintf("%c",205);
	}
	gotoxy(i,4);
	cprintf("%c",187);
	for(i=5;i<15;i++)
	{
		gotoxy((x+13),i);
		cprintf("%c",186);
	}
	gotoxy((x+13),i);
	cprintf("%c",188);
	for(i=(x+1);i<(x+13);i++)
	{
		gotoxy(i,15);
		cprintf("%c",205);
	}
	gotoxy(x,15);
	cprintf("%c",200);
	for(i=5;i<15;i++)
	{
		gotoxy(x,i);
		cprintf("%c",186);
	}
	gotoxy((x+2),5);

}

void subMenuInput(int x)
{
	char ch;
	int curry=wherey();
	int currx=wherex();
	while(1)
	{
		ch=getch();
		if(ch==0)
		{
			ch=getch();
			switch(ch)
			{
				case 33:
					fileMenu();
					break;
				case 35:
					helpMenu();
					break;

				case 18:
					editMenu();
					break;

				case 72:
					curry=curry-2;
					if(curry<5) curry=11;
					gotoxy(x,curry);
					break;
				case 80:
					curry=curry+2;
					if(curry>11) curry=5;
					gotoxy(x,curry);
					break;
				default:
		//			exit(0);
					break;
			}
		}
		else
		{
			switch(ch)
			{
				case 13:
				switch(currx)
				{
					case 4:

						switch(curry)
						{
						case 5:
							newFile();
							break;
						case 7:
							open();
							break;
						case 9:
							save();
							break;
						case 11:
							exit(0);
							break;
						}
					break;

					case 12:

						switch(curry)
						{
						case 5:
				//			newFile();
							break;
						case 7:
				//			open();
							break;
						case 9:
				//			save();
							break;
						case 11:
				//			exit(0);
							break;
						}
					break;

					case 20:

						switch(curry)
						{
						case 5:
				//			newFile();
							break;
						case 7:
				//			open();
							break;
						case 9:
				//			save();
							break;
						case 11:
				//			exit(0);
							break;
						}
					break;
				}
				break;

				case 27:
					border();
					restore();
					menu();
					menuInput();
					break;
			}
		}
	}
}

void fileMenu()
{
	draw(2);
	int curry;
	cprintf("NEW");
	gotoxy(4,7);
	cprintf("OPEN");
	gotoxy(4,9);
	cprintf("SAVE");
	gotoxy(4,11);
	cprintf("EXIT");
	curry=5;
	gotoxy(4,curry);
	subMenuInput(4);
}

void editMenu()
{
	int curry;
	draw(10);

	cprintf("CUT");
	gotoxy(12,7);
	cprintf("COPY");
	gotoxy(12,9);
	cprintf("PASTE");
	gotoxy(12,11);
	cprintf("CLEAR");
	curry=5;
	gotoxy(12,curry);
	subMenuInput(12);
}

void helpMenu()
{
	int curry;
	draw(18);

	cprintf("CONTENTS");
	gotoxy(20,7);
	cprintf("INDEX");
	gotoxy(20,9);
	cprintf("TOPIC SEARCH");
	gotoxy(20,11);
	cprintf("ABOUT");
	curry=5;
	gotoxy(20,curry);
	subMenuInput(20);
}

void main()
{
	textbackground(BLACK);
	textcolor(WHITE);
	border();
	menu();
	gotoxy(34,3);
	cprintf("Nonamed00.CPP");
	menuInput();
}

void newFile()
{
	clearbuffer();
	border();
	menu();
	restore();
	gotoxy(34,3);
	cprintf("Nonamed00.CPP");
	menuInput();

}

void box()
{
	int i;
	gotoxy(2,16);

	cprintf("%c",201);

	for(i=0;i<76;i++)
		cprintf("%c",205);

	cprintf("%c",187);

	for(i=17;i<38;i++)
	{
		gotoxy(79,i);
		cprintf("%c",186);
	}

	gotoxy(79,i);
	cprintf("%c",188);

	for(i=78;i>2;i--)
	{
		gotoxy(i,38);
		cprintf("%c",205);
	}

	gotoxy(i,38);
	cprintf("%c",200);

	for(i=17;i<38;i++)
	{
		gotoxy(2,i);
		cprintf("%c",186);
	}

}

void open()
{
	FILE *fp;
	char fname[20];
	char ch;

//	clearbuffer();
	clrscr();
	box();
	gotoxy(20,20);
	cprintf("\nEnter the file name : ");
	scanf("%s",fname);

	fp=fopen(fname,"r+");
	if(fp==NULL)
	{
		cprintf("\tUnable to open file!");
		return;
	}
	clearbuffer();
	while(1)
	{
		ch=fgetc(fp);
		if(ch==EOF)
			break;
		buffer[len++]=ch;
	}
	border();
	menu();
	restore();
	gotoxy(34,3);
	cprintf("%s",fname);

	fclose(fp);

	menuInput();
}

void save()
{
	FILE *fp;
	char fname[20];
	char ch;

	clrscr();
	box();
	gotoxy(20,20);
	cprintf("\nEnter the file name : ");
	scanf("%s",fname);

	fp=fopen(fname,"w+");
	fputs(buffer,fp);
	fputs("\n",fp);

	fclose(fp);

	border();
	menu();
	gotoxy(34,3);
	cprintf("%s",fname);
	restore();
	menuInput();
}
