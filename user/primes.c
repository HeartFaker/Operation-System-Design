#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int p1[2],fdr,fdw;
long p,n;
int isFirst = 1;

int main(int argc,char *argv[])
{
    //if is first into this main
    if(isFirst==1)
    {
        isFirst=0;
        //create pipe,store reading into fdr,store writting into fdw
        pipe(p1);
        fdr=p1[0];
        fdw=p1[1];
        //write number 2-35 into  the pipe
        for(n=2;n<=35;n++)
        {
            write(fdw,(void*)&n,8);
        }
        //finish writing
        close(fdw);
    }
    //judge whether parent progress or child progress
    if(fork()==0)
    {
        //get a number from fdr
        if(read(fdr,(void*)&p,8))
        {
            printf("prime %d\n",p);
            pipe(p1);
            fdw=p1[1];
        }
        //write a number into fdw if it can't be divided by p
        while(read(fdr,(void*)&n,8))
        {
            if(n%p!=0)
            {
                write(fdw,(void*)&n,8);
            }
        }
        //update fdr
        fdr=p1[0];
        close(fdw);
        main(argc,argv);
    }
    else{
        //parent wait for the child progress
        wait((int*)0);
        close(fdr);
    }
    exit(0);
}