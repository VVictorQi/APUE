#include"apue.h"
int main(void)
{
	char name[L_tmpnam],line[MAXLINE];
	FILE*fp;
	printf("%s \n",tmpnam(NULL));
	tmpnam(name);
	printf("%s\n",name);
	if((fp=tmpfile())==NULL)
		err_sys("tmpfile error");
	fputs("one line of output \n",fp);
	rewind(fp);
	if(fgets(line,sizeof(line),fp)==NULL)
		err_sys("fgets error");
	fputs(line,stdout);// print the line we wrote
	exit(0);
}
//每次产生不同的临时文件，但是临时文件对我们来说是不可兼得的，所以说是不安全的，对于ISO 从规范来说 ，建议使用它更安全的mkstemp函数。
