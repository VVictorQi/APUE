#include <stdio.h>
#include <unistd.h>
int main (void)
{
	putchar('a');        
	write(1, "b", 1);
	putchar('a');
	write(1, "b", 1);
	putchar('a');
	write(1, "b", 1);
	printf("\n");
	return 0;
}
 //window 下生成ababab
//linux下是bbbaaa
