#include"apue.h"
#include<sys/acct.h>
#if defined(LINUX)
#define acct acct_v3
#endif
#define ACORE 0
#define AXSIG 0
#define FMT "\t\t%-*.*s\t e=%6ld \tchars=%7ld\t%c\t %c \t%c \t%c\t \n"
#if !defined(BSD)
static unsigned long compt2ulong(comp_t comptime)
{	
	unsigned long val;
	int exp;
	val= comptime &0x1fff;
	exp=(comptime>>13)&7;
	while(exp-->0)
		val*=8;
	return (val);
}

#endif


int main(int argc,char ** argv)
{
	struct	acct	acdata;
	FILE		*fp;
	if(argc!=2)
		err_quit("usage:pracct file name ");
	
	if((fp=fopen(argv[1],"r"))==NULL)
		err_sys("open file error");
	while(fread(&acdata,sizeof(acdata),1,fp)==1)
		printf(FMT,(int)sizeof(acdata.ac_comm),(int )sizeof(acdata.ac_comm),acdata.ac_comm,compt2ulong(acdata.ac_etime),compt2ulong(acdata.ac_io),
				acdata.ac_flag&ACORE ? 'D':' ',
				acdata.ac_flag&AXSIG ? 'X' : ' ',
				acdata.ac_flag&AFORK ? 'F' : ' ',
				acdata.ac_flag&ASU ?	'S':' ');

	if(ferror(fp))
		err_sys("read error");
	exit(0);
}
