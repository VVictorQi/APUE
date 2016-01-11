#include"apue.h"
#include<setjmp.h>
static void f1(int ,int ,int ,int );
static void f2(void);

static jmp_buf  jmpbuffer;
static int		globval;


int main(void)
{
	int	autoval;
	register int regival;
	volatile int volval;
	static	 int staval;

	globval=1,autoval=2,regival=3,volval=4,staval=5;
	if(setjmp(jmpbuffer)!=0)
	{
		printf("after longjmp:\n");
		printf("globval=%d\tautoval=%d\tregival=%d \tvolval=%d\t staval=%d\t\n",globval,autoval,regival,volval,staval);
		exit(0);
	}
		globval=95;autoval=96;regival=97;volval=98;staval=99;
		f1(autoval,regival,volval,staval);
		exit(0);
}


static void f1(int i,int j,int k,int l)
{
	printf("in f1():\n");
	printf("glob=%d,auto=%d,regi=%d,vol=%d,stat=%d\n",globval,i,j,k,l);
	f2();
}
static void f2()
{
	longjmp(jmpbuffer,1);


}
