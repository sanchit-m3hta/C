#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
int main()
{
int res;
res=open("/tmp/myfifo1",O_WRONLY);
write(res,"Message",7);
printf("Sender Process having PID %d sent the data\n", getpid());
}