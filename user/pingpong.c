#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//to store what is read or written
char buf[8];

int main(int argc, char *argv[])
{
    //declare two pipe
    int p1[2],p2[2];
    pipe(p1);
    pipe(p2);

    //create a child progress
    int pid=fork();
    if(pid==0)
    {
        //close the reading of p1 and writing of p2
        close(p1[0]);
        close(p2[1]);
        pid = getpid();
        //if successfully read byte from p2
        if(read(p2[0],buf,1)==1)
        {
            printf("%d: received ping\n", pid);
            //write buf into reading of pi
            write(p1[1], buf, 1);
        }
    }
    else
    {
        //close the writting of p1 and reading of p2
        close(p1[1]);
        close(p2[0]);
        pid = getpid();
        //whrite buf into p2
        write(p2[1],buf,1);
        //if successfully read byte
        if(read(p1[0],buf,1)==1)
        {
            //pong successfully
            printf("%d: received pong\n",pid);
        }
    }
    exit(0);
}