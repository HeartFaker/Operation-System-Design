#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int main(int argc, char *argv[])
{
    // check if given enough parameter
    if (argc <= 1)
    {
        //report
        printf("usage: sleep [seconds]\n");
        exit(0);
    }
    //sleep according to the given parameter
    sleep(atoi(argv[1]));
    exit(0);
}