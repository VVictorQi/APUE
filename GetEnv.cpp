#include<iostream>
using namespace std;
int main(int argc,char **argv,char **env)
{	
	while(env&&*env)
	{
		cout<<*env<<endl;
		++env;
	}

}
