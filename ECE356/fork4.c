#include<stdio.h>
#include<unistd.h>
int main()
{
int id=fork();
if(id==0)
sleep(1);
printf("\n hello from process with PID=%d and parent PID=%d\n",getpid(), getppid());
return 0;
}