/* 
 * 2014Spring CS416 Homework3
 * File Name:		shell.c
 * Team Members: 	Yihan Qian(yq40), Zhenhua Jia(zj32), Yimeng Li(yl720)
 */
 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>

#define MAXLENGTH 1024		/* Max length of the command */
#define MAXARGUMENT 50		/* Max number of arguments */
/*-------------------------------------Token Linked List(Parsing)-----------------------------*/
/* Token linked list */
struct list_entry
{ 
	char *data; 
	struct list_entry *next; 
} *list = (struct list_entry *)0;

static struct list_entry *tail = NULL; 	/* token list tail */
static int length = 0; 			/* command length */
static int flag = 0;   			/* quote mismatch (token) flag */

/* print the token list */
void dump(void) 
{ 
	struct list_entry *lp; 
	int i = 0;
	for (lp = list; lp != 0; lp=lp->next)
	{
		printf( "the token[%d] is %s \n", i, lp->data); 
		i++;
	}
}

/* delete token list*/
void TokenDelete(void)  
{  
   struct list_entry *current = list;
	struct list_entry * next;
	while (current != NULL) 
	{
		next = current->next;
		free(current);
		current = next;
	}
	list = NULL;    
}

/* insert a node at tail */
struct list_entry * ListInsert( char *token)
{
	struct list_entry *item;
	
	item = malloc(sizeof(struct list_entry)); 
	if (item == 0) 
	{ 
		fprintf(stderr, "malloc failed\n"); 
		exit(1); 
	} /* set the data in the list element */ 
		
	if (list == NULL)
	{
		list = item;
		tail = item;
	}
	else
	{
		tail->next = item;
		tail = item;
	}
				
	item->data = token; /* copy the string */ 
	item->next = NULL;	
		
	return tail;
	
}


/* input a string and skip the white space till it hits the content. */
char *SkipSpace (char *pstart)
{
	while ((*pstart == ' ') || (*pstart == '\t'))	
	{
		pstart++;
		length--;
	}
	return pstart;		/* return the address of the content */
}


/* input a command line and parse it into several tokens. */
char *ParseCommand (char *command)
{
	command = SkipSpace(command);	/* skip white space, if any */
	char *CurrentToken = NULL;		/* define the token pointer */
	char *NextToken;
	
	static char bar[] ="|";			/* predefine a '|' */
		
	if (length <=0 || command == 0)				/* base case, the command ends */
	{
		NextToken = NULL;
		return NULL;
	}
		
	if (*command == '\"' )			/* find the first quotation mark " */
	{
		command++;
		length--;
		CurrentToken = command;
		
		while ( *command != '\"' && length > 0 )		/*find the second quotation mark " */
		{
			command ++;
			length--;
		}
			
		if (length <= 0)		
		{
			if (*command != '\"')	
			{
				printf("Error. No sencond \" mark found.\n");
				flag = 1;
			}	/* no second quotation mark is found */		
				
			*command = 0;
			tail = ListInsert (CurrentToken);
			NextToken = 0;		/* the command ends here */	
		}
		
		else 					/* second quotation mark is found */
		{
			*command = 0;		/* end the first token string */
			tail = ListInsert(CurrentToken);
			if (*(command+1) == '|')
			{
				tail = ListInsert(bar);
				length = length - 2;
				NextToken = command+2;
			}
			else 
			{
				NextToken = command+1;		/* set the next token starting address */
				length--;
			}
		}
		
	}
	
	else if (*command == '\'' )		/* find the first single quotation mark ' */
	{
		command++;
		length--;
		CurrentToken = command;
		
		while ( *command != '\'' && length > 0 )
		{	
			command++;
			length--;
		}
			
		if (length <= 0)			
		{
			if (*command != '\'')
			{
				printf("Error. No sencond \' mark found.\n");
				flag = 1;
				return NULL;
			}	/* no second quotation mark ' is found */
			*command = 0;
			tail = ListInsert (CurrentToken);
			NextToken = 0;		/* the command ends here */
		}
		
		else 						/* the second single quotation mark ' is found */
		{
			*command = 0;
			tail = ListInsert (CurrentToken);
			if (*(command+1) == '|')
			{
				tail = ListInsert(bar);
				length = length -2;
				NextToken = command +2;
			}
			else
			{	
				NextToken = command+1;	/* skip white space and set the next token starting address */
				length--;
			}
		}		
	}
	
	else if (*command =='|')
	{
		CurrentToken = bar;
		length --;
		
		if(length < 1)
		{
			*command = 0;
			printf("Error. Missing a command after |.");
			tail = ListInsert (CurrentToken);
			NextToken = NULL;
		}

		else 
		{
			tail = ListInsert (CurrentToken);
			if (*(command+1) == '|')
			{
				tail = ListInsert(bar);
				length = length -1;
				NextToken = command+1;
			}
			else 
			{	
				NextToken = command+1;		/* set the next token starting address */
			}			
		}
	}
	
	else 
	{
		CurrentToken = command;
		
		while ( length > 0 && (*command != ' ') && (*command != '\t') 
				&& (*command != '|') )
		{
			command++;
			length--;
		}
					 
		if (length <= 0)
		{
			*(command+1) = 0;
			tail = ListInsert (CurrentToken);
			NextToken = NULL;
		}

		else if (*command =='|')
		{
			*command = 0;
			tail = ListInsert (CurrentToken);
			tail = ListInsert (bar);
			NextToken = command+1;
			length--;
		}
	
		else
		{
			*command = 0;
			tail = ListInsert (CurrentToken);
			NextToken = command+1;
			length--;
		}
	}
	
	return NextToken;
}



char * ProcessEnter(char *string)
{
	char *index = string;
	while (*index != '\n')
		index++;
	
	if (*index == '\n')
		*index = 0;
	else
		printf("No Enter is found.\n");
	
	return  string;
}

/*--------------------------------------Command Linked List-----------------------------------*/

static int CMDnumber = 0;  /* total command number */

/* Command Node */
struct Node
{
	int arg_num;        /* the number of arguments */ 
	char *arg[MAXLENGTH];      /* argument list(less than 50 arguments for each command) */ 
	struct Node *next;
}* Cnode = (struct Node *) NULL; /* new command node pointer which points to an empty Node */ 


/* print list */ 
void print(struct Node *p)
{	
	int i =1;
	int j = 0;
	
	for(;p!=NULL;p=p->next) /* for each command */ 
	{
		printf("Command %d is :", i);
		for(j=0; p->arg[j]!=NULL; j++)	
		{
			printf("%s ",p->arg[j]);   /* for each argument */ 
		}
		p->arg_num = j-1;
		
		printf("\n\n");
		i++;
	}
	CMDnumber = i-1; /* calculate Command number */ 
}

/* initialize global variables */
void Initial(void)
{
	list = NULL;
	tail = NULL;
	Cnode = NULL;
	length = 0;
	flag = 0;
	CMDnumber = 0;
}

/* delete Command List */
void DeleteCnode()
{
	struct Node *current = Cnode;
	struct Node * next;
	while (current != NULL) 
	{
		next = current->next;
		free(current);
		current = next;
	}
	list = NULL;
}

/* create list */ 
struct Node * create(struct list_entry * tHead) /* tHead points at the head node of token list */ 
{	
	if(CMDnumber==1)
		return NULL;
	/* add token */
	struct Node *newnode, *head, *tail;
	struct list_entry * curT; /* current pointer for token list */ 
	int j, k = 0;
	head = Cnode;     /* initialie pointers */ 
	tail = head;
	curT = tHead;

	/* Build up a list of commands */ 
	int cmdflag = 1;
	

	for(j=0;curT!=NULL;j++) /* t for token list, j for command */ 
	{	
		newnode = malloc(sizeof(struct Node)); /* add new node */ 			
		if(newnode==0)
		{
			printf("malloc failed\n");
				exit(1);
		}			
		
		if(head==NULL)
		{
			head = tail = newnode;	
		}
		else
		{
			tail->next = newnode; /* add newnode to the end of the list */ 
			tail = newnode;       /* set new tail */ 
		}
  		k = 0; /* k for arg[] */ 			
		
		while(cmdflag)
		{			
			if(strcmp(curT->data,"|") == 0)
			{
				cmdflag=0;
				curT = curT->next;
			}
			else     
			{
				if(k>(MAXARGUMENT-1))
				{
					puts("Error! More than 50 arguments in a command!");
					return NULL;
				}
				newnode->arg[k]= (char *)curT->data; /* store token to newnode */ 
				curT = curT->next;
				k++;
				if(curT==NULL)
				cmdflag=0;
			}
		}
		cmdflag=1;
	}

	print(head); /* output command lines */ 
	
	return head;
}

/*------------------------------------------Built-in Commands----------------------------------------*/
int pid;

/* cd command */ 
int cmdCd(int argc, char** argv)
{
	char *buffer = NULL;
	char *value = NULL;
	char *str = NULL;
	int n = 0;
	if(argc == 0) /* no argument, change to home  */ 
	{
		value = getenv("HOME"); /* return a null-terminated string with the value of the requested environment variable */
		n = chdir(value);  /* if n=0, change directory succeed. current working directory is HOME: value*/
	 }
	else if(argc == 1)
	{
		value = argv[1];
		buffer = getcwd(buffer,80); /* store current directory */
		str = strcat(buffer,"/");      
		str = strcat(str,value);
		n = chdir(str);   /* change the current working directory */
		
		if(n==-1)              /* failure */
			fprintf(stderr, "%s: %s: %s\n", argv[0], strerror(errno), value);
                              /* if n=0, succeed. Current working directory is str */
	}
	else
	{
		fprintf(stderr, "cd error: Too many arguments!\n");
	}
	return 0;
}

/* normal single simple command - default*/ 
int cmdDef(int argc, char** argv) 
{
	int status;
	switch(pid = fork())
	{
		case 0:
		{
			/* in the child process*/
			int x = execvp(argv[0], argv);
			if (x==-1) 
			{
				printf("err: invalid command\n");
				exit(0);
			}				
			break;
		}		
		default:
		{
			while((pid = wait(&status)) != -1)
			fprintf(stderr, "process %d exits with %d\n", pid, WEXITSTATUS(status));
			break;
		}		
		case -1:
		{
			perror("fork");
			exit(1);
		}
	}
	return 0;
}

/* exit command */ 
int cmdExit(int argc, char** argv)
{ 
	int exitcode = 0;
	if(argc == 0)     /* no paramenter */
	{
		exitcode = 0;
	}
	else if (argc == 1)
	{
		int ret = atoi(argv[1]);
		exitcode = ret;		
	}
	else if (argc >1)
	{
		fprintf(stderr, "err: too many arguments\n");	
		exitcode = -1;	
	}
	_exit(exitcode);
}


/* Built-in command Table */ 
struct builtin
{
	char *name;
	int (*function)(int argc, char** argv);
}table[2]={
	{"exit",cmdExit},
	{"cd",cmdCd}
};


/*----------------------------simple Pipe command1 | commands2 -------------------------------*/
/* source */
void runsource(int pfd[], char** argv, int number)
{
	int j;
	switch(pid = fork())
	{
		case 0:
			dup2(pfd[1], 1);
			for(j = 0; j < number; ++j)
			{
				close(pfd[2 * j]);
				close(pfd[2 * j + 1]);
			}
			execvp(argv[0], argv);
			perror(argv[0]);
		default:
			break;
		case -1:
			perror("fork");
			exit(1);
	}
}


/* destination */
void rundest(int pfd[], char **argv, int number)
{
	int j;
	switch(pid = fork())
	{
		case 0:
			dup2(pfd[2 * (number - 1)], 0);
			for(j = 0; j < number; ++j)
			{
				close(pfd[2 * j]);
				close(pfd[2 * j + 1]);
			}
			execvp(argv[0], argv);
			perror(argv[0]);
		default:
			break;
		case -1:
			perror("fork");
			exit(1);
	}
}

/* middle */
void runmid(int pfd[], char **argv, int number, int i)
{
	int j;
	switch(pid = fork())
	{
		case 0:
			dup2(pfd[2 * i], 0);
			dup2(pfd[2 * (i + 1) + 1], 1);
			for(j = 0; j < number; ++j)
			{
				close(pfd[2 * j]);
				close(pfd[2 * j + 1]);
			}
			execvp(argv[0], argv);
			perror(argv[0]);
		default:
			break;
		case -1:
			perror("fork");
			exit(1);
	}
}


/*----------------------------------Test Commands---------------------------------------------*/
/* test commands */
int Command(struct Node * cmdList)
{
	if(cmdList == NULL) 
	{
		return 1;
	}
	struct Node * curPtr;
	curPtr = cmdList;

	if(!strcmp(curPtr->arg[0], table[0].name))             /* exit command */ 
	{
		table[0].function(curPtr->arg_num, curPtr->arg);
	}
	else if(!strcmp(curPtr->arg[0], table[1].name))        /* cd command */ 
	{
		table[1].function(curPtr->arg_num, curPtr->arg);
	}
	else                                                   /* default commands */ 
	{
		if(CMDnumber==1) /* single simple command*/
		{
			cmdDef(curPtr->arg_num, curPtr->arg);
		}
		else if(CMDnumber>1)  /* pipe commands */
		{
			int status;
			int number = CMDnumber-1; /* number of pipes */
			int fd[2 * number];
			int j;
			for(j = 0; j < number; ++j)
			{
				pipe(fd + 2 * j);
			}
			runsource(fd, curPtr->arg, number);
			for(j = 0; j < (number - 1); ++j)
			{
				curPtr = curPtr->next;
				runmid(fd, curPtr->arg, number, j);
			}
			curPtr = curPtr->next;
			rundest(fd, curPtr->arg, number);
			for(j = 0; j < number; ++j)
			{
				close(fd[2 * j]);
				close(fd[2 * j + 1]);
			}
			while((pid = wait(&status)) != -1)
				fprintf(stderr, "process %d exits with %d\n", pid, WEXITSTATUS(status));
		}
	}
	return 0;
}

/*------------------------------------------Main function-------------------------------------*/
int main(int argc, char **argv) 
{ 
	int showprompt = isatty(0);

	while(1)
	{			
		int i = 0;			
		char cmd[MAXLENGTH];
		char *token = cmd;

		if(showprompt)
		{
			fputs("^_^ ", stderr);          /* generate a prompt for shell */ 
			fgets(cmd, MAXLENGTH, stdin);   /* standard input to get cmd */ 
		}
		else                    /* input is not from the terminal */
		{
			fgets(cmd, MAXLENGTH, stdin);
		}
		
		if(feof(stdin))
		{
			exit(0);
		}
		ProcessEnter(cmd);
		length = strlen(cmd);

		for (; i < MAXLENGTH; i++) /* parsing */
		{
			if (token == NULL || length == 0 || !strcmp(token, "\t"))
				break;
			
			token = ParseCommand (token);
		}
		
		if (flag == 1)   /*  Error. Mismatched quote. */
		{			
			flag = 0;	
			TokenDelete();	
			DeleteCnode();
			Initial();
			continue;
		}

		struct list_entry * tokenPtr; 
		tokenPtr = list; 
		struct Node * cmdHead;
		cmdHead = create(tokenPtr); /* create command linked list */
			
		Command(cmdHead);	/* deal with commands */ 
			
		DeleteCnode();
		TokenDelete();
		Initial();
		printf("\n");
	}

	return 0;
}
