#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
    char *p;
    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    return p;
}


void find(char *path,char*filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    //open the file of the path
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    //get the info of the file
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type)
    {
        //if the type is file
        case T_FILE:
            //check if it matches the filename
            if(strcmp(filename,fmtname(path))==0)
            {
                printf("%s\n",path);
            }
            break;
        //if the type is dir
        case T_DIR:
            //if the path is too long
            if(strlen(path)+1+DIRSIZ+1>sizeof(buf))
            {
                printf("find:path too long\n");
                break;
            }
            //copy path to buf
            strcpy(buf,path);
            //p direct to the tail of buf
            p=buf+strlen(buf);
            //current dir
            *p++='/';
            //read every dialog
            while(read(fd,&de,sizeof(de))==sizeof(de))
            {
                if (de.inum == 0)
                    continue;
                //copy de.name to buf
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                //get info of dir and check
                if (stat(buf, &st) < 0)
                {
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                //check if name is "." or ".."
                if (strcmp(fmtname(buf), ".") != 0 && strcmp(fmtname(buf), "..") != 0)
                {
                    find(buf, filename);
                }
            }
            break;
    }
    close(fd);
}

int main(int argc,char *argv[])
{   
    //if the number of parameters is not approved
    if (argc != 3) {
        printf("Usage: find <directory> <filename>\n");
        exit(0);
    }
    //use find function
    find(argv[1], argv[2]);
    exit(0);
}