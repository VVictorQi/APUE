#include<iostream>
#include<stdlib.h>
#include <unistd.h>
using namespace std;
int main()
{
	char *env="USER_HOME";
	putenv("USER_HOME=/user/root");
	char *a=getenv(env);
	if(a!=NULL)
	   cout<<"env :"<<a<<endl;
	pause();
	return 0;
}
