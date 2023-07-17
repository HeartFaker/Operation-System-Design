#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

char args[MAXARG][512];
char *pass_args[MAXARG];
int preargnum, argnum;
char ch;
char arg_buf[512];
int n;

int readline()
{
    //record current parameter index
    argnum = preargnum;
    //set 0 for arg_buf
    memset(arg_buf, 0, sizeof(arg_buf));
    //infinity reading
    for (;;)
    {
        n = read(0, &ch, sizeof(ch));
        //the end of the file
        if (n == 0)
        {
            //no more output
            return 0;
            break;
        }
        //error happen
        else if (n < 0)
        {
            fprintf(2, "read error\n");
            exit(1);
        }
        else
        {
            if (ch == '\n')
            {
                //store the parameter into args
                memcpy(args[argnum], arg_buf, strlen(arg_buf) + 1);
                argnum++;
                //read successfully
                return 1;
            }
            else if (ch == ' ')
            {
                //store the parameter into args
                memcpy(args[argnum], arg_buf, strlen(arg_buf) + 1);
                argnum++;
                //set 0 for next parameter
                memset(arg_buf, 0, sizeof(arg_buf));
            }
            else
            {
                //add ch to arg_buf
                arg_buf[strlen(arg_buf)] = ch;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    //check the number of parameters
    if (argc < 2)
    {
        printf("usage: xargs [command] [arg1] [arg2] ... [argn]\n");
        exit(0);
    }
    preargnum = argc - 1;
    //copy parameters innto args
    for (int i = 0; i < preargnum; i++)
        memcpy(args[i], argv[i + 1], strlen(argv[i + 1]));
    //read the input parameters
    while (readline())
    {
        //child progress
        if (fork() == 0)
        {
            //mark the end of the file
            *args[argnum] = 0;
            int i = 0;
            //store every address into pass_args
            while (*args[i])
            {
                pass_args[i] = (char *)&args[i];
                i++;
            }
            //mark the end of the parameter index
            *pass_args[argnum] = 0;
            //execute the order
            exec(pass_args[0], pass_args);
            //error happen
            printf("exec error\n");
            exit(0);
        }
        else
        {
            //parent wait
            wait((int *)0);
        }
    }
    exit(0);
}