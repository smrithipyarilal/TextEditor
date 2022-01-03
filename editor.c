/*
Text Editor - sp3573
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

typedef struct line {
	char *beg; //pointer to buffer at newline
	int len;   //length of the line from beginning to next-1
	struct line* next; //pointer to next line
} line;

int numlines(char*, int);

void setlines(line*,char*,int);

void printlist(line*, int);

void appendline(line*, char *, int); 

void printline(line*, int);

void freemem(line*, char *);

int
main(int argc, char *argv[])
{
	int fd, cap, cur, used, n, i ,c;
	char *buf, *p;
	line *head;

	if(argc != 2){
		printf("usage: editor <file>\n");
		exit(1);
	}
	
	used = 0;
	cap = 128;
	buf = malloc(cap);
	p = buf;

	fd = open(argv[1], O_RDONLY | O_CREAT);
	while((n=read(fd, p, cap-used)) > 0) {
		used += n;
		p += n;
		if(cap-used < 32) {
			cap *=2;
			buf = realloc(buf,cap);
			p = buf+used;
		}
	}
	n = numlines(buf, used);
	head = (line *) malloc(sizeof(line));
	setlines(head, buf, used);
	system("clear");
	cur = 1;
	printf("Successfully opened \"%s\"\n", argv[1]);
	fflush(stdin);		
	char inp[10];
	while(1){
		printf("------------------------------------------------\n");
		printline(head,cur);
		printf(">");
		fflush(stdin);
		fgets(inp,10,stdin);
		printf("------------------------------------------------\n");
		//system("clear");
		//printf("input was: %s\n",inp);
		if (atoi(inp)){
			if(atoi(inp) <= n) {
				cur = atoi(inp);
				printf("line changed to %d\n",cur);
				}
			else{
			printf("line out of range. file \"%s\" contains %d lines.\n",argv[1],n);
			}
		} else
		if (inp[0] == 'q'){
			printf("Take care.\n");
			break;
		} else
		if (inp[0] == 'p' || inp[0] == 'n'){
			printlist(head,inp[0]);
		} else {
			printf("unrecognized input %s.usage: p | n | q | <line#>\n",inp);
		}

	}

	freemem(head, buf);
	close(fd);
	exit(0);
}

void printlist(line *l, int flag)
{
	int n;
	line* current = l;
	n = 1;
	while (current != NULL) {
		if (flag == 'n'){
			printf("%3d: ",n);
			fflush(stdout);
		}
		write(1, current->beg+1, current->len);
		fflush(stdout);
		current = current->next;
		n++;
	}
}

int 
numlines(char *buf, int nchar)
{
	int i, nline;
	char *p;

	p = buf;
	nline = 0;
	for(i = 0; i < nchar; i++) {
		if(*p == '\n')
			nline++;
		p++;
	}
	return nline;
}

void	
setlines(line *head, char *buf, int nchar)
{
	int i, n, len;
	char *p;
	line *h;

	h = head;
	p = buf;
	n = 0;
	len = 0;
	for (i=0; i<nchar; i++) {
		len++;
		if (*p == '\n'){
			if(n==0) {
				h->beg = p-len;
				h->len = len;
				h->next = NULL;
			} else {
				appendline(h, p-len, len);
			}
			len = 0;
			n++;
		}
		p++;
	}
}

void
appendline(line *head, char *p, int len) {
	line* current = head;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = (line *) malloc(sizeof(line));
	current->next->beg = p;
	current->next->len = len;
	current->next->next = NULL;
}

void
printline(line *head, int num) {
	line* current;
	int i;
	current = head;
	i = 1;
	for(i = 1; i != num; i++) {
		current = current->next;
	}
	printf("Line %d-> ",num);
	fflush(stdout);
	write(1, current->beg+1, current->len);
	fflush(stdout);
}

void
freemem(line *head, char *buf) {
	free(buf);
	line* current;
	while(head != NULL) {
		current = head;
		head = head->next;
		free(current);
		}
}
