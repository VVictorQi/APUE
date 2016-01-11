#include"apue.h"
#include<dirent.h>
#include<fcntl.h>
#include<limits.h>
#include<malloc.h>
//#include<relloc.h>
#include<stdlib.h>
typedef int Myfunc(const char *,const struct stat *,int);
static  Myfunc myfunc;
static int myftw(char *,Myfunc *);
static int dopath(Myfunc *);
static long nreg,ndir,nblk,nchr,nfifo,nslink,nsock,ntot;




//path_alloc
#include<errno.h>
#ifdef PATH_MAX
static long pathmax=PATH_MAX;
#else
static long pathmax=0;
#endif

static long posix_version=0;
static long xsi_version=0;
#define PATH_MAX_GUESS 1024
char * path_alloc(size_t *sizep)
{
	char *ptr;
	size_t size;
	if((posix_version==0))
		posix_version=sysconf(_SC_VERSION);
	if(xsi_version==0)
		xsi_version=sysconf(_SC_XOPEN_VERSION);
	if(pathmax==0)
	{
		errno=0;
		if((pathmax=pathconf("/",_PC_PATH_MAX))<0)
		{
			if(errno==0)
				pathmax=PATH_MAX_GUESS;
			else
				err_sys("pathconf error for PC_PATH_MAX");
		}
		else
			pathmax++;
	}
	if((posix_version<200112L)&&(xsi_version<4))
		size=pathmax+1;
	else
		size=pathmax;
	if((ptr=malloc(size))==NULL)
		err_sys("malloc error for pathname");
	if(sizep!=NULL)
		*sizep=size;
	return (ptr);
}







int main (int argc,char *argv[])
{
	int		ret;
	if(argc!=2)
		err_quit("usage : ftw<staring-pathname>");
	ret=myftw(argv[1],myfunc);
	ntot=nreg+ndir+nblk+nchr+nfifo+nslink+nsock;
	if(ntot==0)
		ntot=1;
	printf("regular file = \t%7ld, \t%5.2f %% \n",nreg,nreg*100.0/ntot);
	printf("block special= \t%7ld,\t %5.2f %% \n",nblk,nblk*100.0/ntot);
	printf("directory file=\t%7ld,\t%5.2f %%\n",ndir,ndir*100.0/ntot);
	printf("char special =\t%7ld,\t%5.2f %%\n",nchr,nchr*100.0/ntot);
	printf("FIFOs =\t %7ld,\t%5.2f %%\n",nfifo,nfifo*100.0/ntot);
	printf("symbolic links=\t %7ld,\t%5.2f %%\n",nslink,nslink*100.0/ntot);
	printf("sockets =\t %7ld,\t%5.2f %%\n",nsock,nsock*100.0/ntot);
	exit(ret);
}
/*desend thropugh  the hierachy ,staring at 'pathname'
the caller func() is called for every file.
*/
#define  FTW_F 1
#define  FTW_D 2
#define	 FTW_DNR 3
#define  FTW_NS 4 // Fike that we cat stat 


static char *fullpath;
static size_t pathlen;
static int myftw(char *pathname,Myfunc *func)
{
	fullpath=path_alloc(&pathlen);
	if(pathlen<=strlen(pathname))
	{
		pathlen=strlen(pathname)*2;
		
		if((fullpath=realloc(fullpath,pathlen))==NULL)
			err_sys("realloc error");
	}
	strcpy(fullpath,pathname);
	return (dopath(func));
}




static int dopath(Myfunc* func)
{
	struct stat		buf;
	struct dirent	*dirp;
	DIR				*dp;
	int				ret,n;
	if(lstat(fullpath,&buf)<0)
		return (func(fullpath,&buf,FTW_NS));
	if(S_ISDIR(buf.st_mode)==0)
		return (func(fullpath,&buf,FTW_F));
	if(ret=func(fullpath,&buf,FTW_D)!=0)
		return (ret);
	n=strlen(fullpath);
	if(n+NAME_MAX+2>pathlen)
	{
		pathlen*=2;
		if((fullpath=realloc(fullpath,pathlen))==NULL)
			err_sys("relloc error");
	}
	fullpath[n++]='/';
	fullpath[n]=0;
	if((dp=opendir(fullpath))==NULL)
		return (func(fullpath,&buf,FTW_DNR));
	while((dirp=readdir(dp))!=NULL)
	{
		if(strcmp(dirp->d_name,".")==0||strcmp(dirp->d_name,"..")==0)
			continue;
		strcpy(&fullpath[n],dirp->d_name);
		if((ret=dopath(func))!=0)
			break;
	}
	fullpath[n-1]=0;
	if(closedir(dp)<0)
		err_ret("can't close the dir :%s\n",fullpath);
	return (ret);
}
static int myfunc(const char *pathname,const struct stat *statptr,int type)
{
	switch(type)
	{
		case FTW_F:
			switch(statptr->st_mode & S_IFMT)
			{
				case S_IFREG:
					nreg++;break;
				case S_IFBLK:
					nblk++;break;
				case S_IFCHR:
					nchr++;break;
				case S_IFIFO:
					nfifo++;break;
				case S_IFSOCK:
					nsock++;break;
				case S_IFLNK:
					nslink++;break;
				case S_IFDIR:
					err_dump("for S_IFDIR for %s "),pathname;
			}
			break;
		case FTW_D:
			ndir++;
			break;
		case FTW_DNR:
			err_ret("can't read directory %s",pathname);
			break;
		case FTW_NS:
			err_ret("stat error for %s",pathname);
			break;
		default:
			err_dump("unknown type %d for pathname %s ",type ,pathname);
	}
	return (0);
}

