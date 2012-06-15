/* pls specify target .asm file as a command line argument while executing */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 100
#define MAX 20
#define OBJECTSIZE 1024

typedef struct mot_table
{
	char inst[SIZE];
	char iclass[SIZE];
	int opcode;
	int len;
} MOT;

typedef struct register_set
{
	char rname[SIZE];
	int value;
} REG;

typedef struct condtional_statement
{
	char cname[SIZE];
	int value;
} CONDITION;

typedef struct symbol_table
{
	char sname[SIZE];
	int index;
	int address;
} SYMBOL;

typedef struct literal_table
{
	char lname[SIZE];
	int index;
	int address;
} LITERAL;

typedef struct pool_table
{
	int index;	
} POOL;

void fetchFromMot(char *, char *, int *, int *);
void fetchFromReg(char *, int *);
void fetchCondition(char *, int *);
void generateIntermediate(char *, char *, int);
void addToSymbolTable(char *, int *, int);
void addToLiteralTable(char *, int *, int);
void addToPoolTable();
void saveOutput(char *);

MOT mot[]= {	{"START", "AD", 1, 0},
		{"END", "AD", 2, 0},
		{"ORIGIN", "AD", 3, 0},
		{"EQU", "AD", 4, 0},
		{"LTORG", "AD", 5, 0},

		{"DC", "DL", 1, 1},
		{"DS", "DL", 2, 1},

		{"STOP", "IS", 0, 1},
		{"ADD", "IS", 1, 1},
		{"SUB", "IS", 2, 1},
		{"MULT", "IS", 3, 1},
		{"MOVER", "IS", 4, 1},
		{"MOVEM", "IS", 5, 1},
		{"COMP", "IS", 6, 1},
		{"BC", "IS", 7, 1},
		{"DIV", "IS", 8, 1},
		{"READ", "IS", 9, 1},
		{"PRINT", "IS", 10, 1}
		};

REG reg[] = {	{"AREG", 1},
		{"BREG", 2},
		{"CREG", 3},
		{"DREG", 4}
		};

CONDITION condition[] = {	{"LT", 1},
				{"LE", 2},
				{"EQ", 3},
				{"GT", 4},
				{"GE", 5},
				{"ANY", 6}
				};

SYMBOL symtable[MAX];
LITERAL littable[MAX];
POOL pooltable[MAX];

int symPtr = 0, litPtr = 0, poolPtr = 0, lc = 0, ln = 0, errorflag = 0;
char errorLog[OBJECTSIZE], opfile[OBJECTSIZE] = "";
char temp[SIZE];

int main(int argc, char *argv[])
{
	
	FILE *fp = NULL;
	char buffer[80], tok1[SIZE], tok2[SIZE], tok3[SIZE], tok4[SIZE], tok5[SIZE] = "";
	int num, i, cnt = 0, s, templc, changeflag = 0, startflag=0, endflag=0;
	char c;
	char icls[5];
	int opcode, length = 0;
	fp=fopen(argv[1], "r");
	do
	{
		c = fgetc(fp);
		if(c=='\n')
			cnt++;
	}while(c!=EOF);
	fp=fopen(argv[1], "r");		
	do
	{	
		strcpy(tok1, "");
		strcpy(tok2, "");
		strcpy(tok3, "");
		strcpy(tok4, "");		
		strcpy(tok5, "");		
		fgets(buffer, 80, fp);
		changeflag = 0;
		num = sscanf(buffer, "%s%s%s%s%s%s", tok1, tok2, tok3, tok4, tok5, temp);
		if(tok2[strlen(tok2)-1]==',')
			tok2[strlen(tok2)-1]='\0';
		if(tok3[strlen(tok3)-1]==',')
			tok3[strlen(tok3)-1]='\0';		
		sprintf(temp, "%d:\t\t", lc);
		strcat(opfile, temp);
		switch(num)
		{
		default:
			errorflag++;
			sprintf(temp, "Line %d:\tInvalid number of mnemonics found.\n", ln+1);
			strcat(errorLog, temp);
			break;
		
		case 1:		
			fetchFromMot(tok1, icls, &opcode, &length);
			if(strcmp(tok1, "START")==0)
			{
				lc = 0;			
				changeflag++;
				startflag++;
			}
			if(opcode==-1)
			{
				errorflag++;
				sprintf(temp, "Line %d:\tUndefined mnemonic '%s'.\n", ln+1, tok1);
				strcat(errorLog, temp);	
			}			
			generateIntermediate(temp, icls, opcode);			
			strcat(opfile, temp);
			if(strcmp(tok1, "LTORG")==0 || strcmp(tok1, "END")==0)
			{				
				for(i=0;i<litPtr;i++)
				{	
					if(littable[i].address==-1)
					{
						strcat(opfile, "\n");	
						sprintf(temp, "%d:\t\t", lc);
						strcat(opfile, temp);						
						addToLiteralTable(littable[i].lname, &opcode, 1);
						generateIntermediate(temp, "LIT", opcode);
						strcat(opfile, temp);
						lc++;		
								
					}					
				}
				changeflag++;
				if(strcmp(tok1, "END")==0)
						endflag++;							
				addToPoolTable();									
			}
			length=0;
			
			break;
			
		case 2:
			fetchFromMot(tok1, icls, &opcode, &length);
			if(strcmp(tok1, "START")==0)
			{
				lc = atoi(tok2);			
				changeflag++;
				startflag++;
				generateIntermediate(temp, icls, opcode);
				strcat(opfile, temp);
				sprintf(temp, ", ");
				strcat(opfile, temp);
				generateIntermediate(temp, "CONST", atoi(tok2));
				strcat(opfile, temp);			
			
			}
			else if(strcmp(tok1, "ORIGIN")==0)
			{
				for(i=0;i<symPtr;i++)
				{
					if(strcmp(symtable[i].sname, tok2)==0)
					{						
						lc = symtable[i].address;						
						break;
					}
				}
				changeflag++;
				generateIntermediate(temp, icls, opcode);
				strcat(opfile, temp);
				sprintf(temp, ", ");
				strcat(opfile, temp);
				generateIntermediate(temp, "SYM", i+1);
				strcat(opfile, temp);							
			}			
			length=0;						
			break;
			
		case 4:
			if(strcmp(tok1, "ORIGIN")==0)
			{				
				for(i=0;i<symPtr;i++)
				{
					if(strcmp(symtable[i].sname, tok2)==0)
					{
						if(strcmp(tok3, "+")==0)
							s = 1;
						else if(strcmp(tok3, "+")==0)						
							s = -1;
						lc = symtable[i].address + (s * atoi(tok4));						
						break;
					}
				}	
				fetchFromMot(tok1, icls, &opcode, &length);			
				generateIntermediate(temp, icls, opcode);
				strcat(opfile, temp);
				strcat(opfile, ", ");				
				generateIntermediate(temp, "SYM", i+1);
				strcat(opfile, temp);
				sprintf(temp, " %s (CONST, %s)", tok3, tok4);
				strcat(opfile, temp);
				length=0;
				changeflag++;
				break;
			}
			addToSymbolTable(tok1, &opcode, 1);
			strcpy(tok1, tok2);
			strcpy(tok2, tok3);
			strcpy(tok3, tok4);			
			
		case 3:
			fetchFromMot(tok1, icls, &opcode, &length);
			if(strcmp(icls,"IS")==0)
			{
				generateIntermediate(temp, icls, opcode);
				strcat(opfile, temp);
				strcat(opfile, ", ");	
				if(strcmp(tok1, "BC")!=0)		
					fetchFromReg(tok2, &opcode);
				else if(strcmp(tok1, "BC")==0)
					fetchCondition(tok2, &opcode);
				sprintf(temp, "0%d", opcode);
				strcat(opfile, temp);
				strcat(opfile, ", ");
				if(tok3[0] == '=')
				{
					addToLiteralTable(tok3, &opcode, 0);
					generateIntermediate(temp, "LIT", opcode);
					strcat(opfile, temp);
				}
				else
				{
					addToSymbolTable(tok3, &opcode, 0);
					generateIntermediate(temp, "SYM", opcode);
					strcat(opfile, temp);
				}
				changeflag++;
			}			
			fetchFromMot(tok2, icls, &opcode, &length);
			if(strcmp(tok2, "DS")==0)
			{	
				generateIntermediate(temp, icls, opcode);
				strcat(opfile, temp);
				strcat(opfile, ", ");	
				addToSymbolTable(tok1, &opcode, 1);
				length = atoi(tok3);
				generateIntermediate(temp, "SYM", opcode);
				strcat(opfile, temp);
				changeflag++;
			}
			else if(strcmp(tok2, "DC")==0)
			{	
				generateIntermediate(temp, icls, opcode);
				strcat(opfile, temp);
				strcat(opfile, ", ");	
				addToSymbolTable(tok1, &opcode, 1);
				length = 1;
				generateIntermediate(temp, "SYM", opcode);
				strcat(opfile, temp);
				changeflag++;
			} 
			else if(strcmp(tok2, "EQU")==0)
			{
				for(i=0;i<symPtr;i++)
				{
					if(strcmp(symtable[i].sname, tok3)==0)					
					{
						templc = lc;
						lc = symtable[i].address;
						addToSymbolTable(tok1, &opcode, 1);
						lc = templc;
					}						
					break;					
				}	
				fetchFromMot(tok2, icls, &opcode, &length);			
				generateIntermediate(temp, icls, opcode);
				strcat(opfile, temp);
				strcat(opfile, ", ");				
				generateIntermediate(temp, "SYM", i+1);
				strcat(opfile, temp);				
				length=0;
				changeflag++;
			}						
			break;		
		case 5:		
			if(strcmp(tok2, "EQU")==0)
			{
				for(i=0;i<symPtr;i++)
				{
					if(strcmp(symtable[i].sname, tok3)==0)					
					{
						templc = lc;
						if(strcmp(tok4, "+")==0)
							s = 1;
						else if(strcmp(tok4, "-")==0)
							s = -1;							
						lc = symtable[i].address + (s * atoi(tok5));
						addToSymbolTable(tok1, &opcode, 1);
						lc = templc;
						break;
					}																
				}	
				if(i==symPtr || symtable[i].address == -1)
				{
					errorflag++;
					sprintf(temp, "Line %d:\tEQU needs to be backward reference.\n", ln+1);
					strcat(errorLog, temp);
				}
				fetchFromMot(tok2, icls, &opcode, &length);			
				generateIntermediate(temp, icls, opcode);
				strcat(opfile, temp);
				strcat(opfile, ", ");				
				generateIntermediate(temp, "SYM", i+1);
				strcat(opfile, temp);
				sprintf(temp, " %s (CONST, %s)", tok4, tok5);
				strcat(opfile, temp);
				changeflag++;
				length=0;				
			}			
			break;
		}
		if(changeflag==0)
		{
			errorflag++;
			sprintf(temp, "Line %d:\tCould not parse statement\n", ln+1);
			strcat(errorLog, temp);	
		}
		strcat(opfile, "\n");
		lc = lc + length;
		ln++;
	}while(cnt!=ln);
	for(i=0;i<symPtr;i++)
	{
		if(symtable[i].address==-1)
		{
			errorflag++;
			sprintf(temp, "%s:\tUndefined symbol '%s'\n", argv[1], symtable[i].sname);
			strcat(errorLog, temp);	
		}
	}
	if(!(startflag==1 && endflag==1))
	{
		errorflag++;
		sprintf(temp, "%s:\tThere should be exactly one START and one END symbol\n", argv[1]);
		strcat(errorLog, temp);	
	}
	if(errorflag==0)
	{
		printf("%s", opfile);
		saveOutput(argv[1]);
	}
	else
	{
		printf("%d error(s) found\n", errorflag);
		printf("%s", errorLog);		
	}
	fclose(fp);
	return 0;
}

void fetchFromMot(char *name, char *cls, int *opcode, int *length)
{
	int i;
	strcpy(cls, "");
	*opcode = -1;	
	for(i=0;i<18;i++)
	{
		if(strcmp(mot[i].inst,name)==0)		
		{
			strcpy(cls, mot[i].iclass);
			*opcode = mot[i].opcode;
			*length = mot[i].len;
		}
	}
}

void fetchFromReg(char *name, int *opcode)
{
	int i;	
	*opcode=-1;
	for(i=0;i<4;i++)
	{
		if(strcmp(reg[i].rname, name)==0)
			*opcode = reg[i].value;
	}
	if(*opcode==-1)
	{
		errorflag++;
		sprintf(temp, "Line %d:\tInvalid register name '%s'\n", ln+1, name);
		strcat(errorLog, temp);		
	}
}

void fetchCondition(char *name, int *opcode)
{
	int i;	
	*opcode=-1;
	for(i=0;i<6;i++)
	{
		if(strcmp(condition[i].cname, name)==0)
			*opcode = condition[i].value;
	}
	if(*opcode==-1)
	{
		errorflag++;
		sprintf(temp, "Line %d:\tInvalid conditional token '%s'\n", ln+1, name);
		strcat(errorLog, temp);		
	}
}

void generateIntermediate(char *target, char *cls, int num)
{	
	if(num<10)
		sprintf(target, "(%s, 0%d)", cls, num);
	else
		sprintf(target, "(%s, %d)", cls, num);

}

void addToSymbolTable(char *token, int *index, int setflag)
{
	int i=0;
	*index = 1;
	for(i=0;i<symPtr;i++)
	{		
		if(strcmp(symtable[i].sname, token)==0)
			break;
		*index = *index + 1;
	}		
	strcpy(symtable[i].sname, token);	
	if(setflag)
		symtable[i].address = lc;	
	else
	{
		if(i==symPtr)		
			symtable[i].address = -1;	
	}
	if(i==symPtr)
		symPtr++;		
}

void addToLiteralTable(char *token, int *index, int setflag)
{
	int i=0;
	*index = poolPtr+1;
	for(i=poolPtr;i<litPtr;i++)
	{		
		if(strcmp(littable[i].lname, token)==0)
			break;
		*index = *index + 1;
	}		
	strcpy(littable[i].lname, token);
	if(setflag)
		littable[i].address = lc;	
	else
	{				
		if(i==litPtr)		
			littable[i].address = -1;	
	}
	if(i==litPtr)
		litPtr++;
}

void addToPoolTable()
{
	static int i = 0;	
	pooltable[i++].index = poolPtr;		
	poolPtr = (litPtr - poolPtr);	
}

void saveOutput(char *argv)
{
	FILE *fp = NULL;
	char fname[50];
	int i;
	strcpy(fname, argv);
	i = strlen(fname);
	while(fname[i]!='\\')
	{
			if(fname[i]=='.')
				fname[i] = '\0';
			i--;
	}
	strcat(fname, ".obj");
	fp = fopen(fname, "w+");
	fputs(opfile, fp);
	fclose(fp);
}
