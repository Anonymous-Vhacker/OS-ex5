#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"shell.h"
#include"../global/global.h"
#include"../util/str.h"
#include"../util/time.h"
#include"../function/api.h"
extern int BLOCK_NUMS;
extern int BLOCK_SIZE;
extern int FAT_ITEM_NUM;
extern int FCB_ITEM_NUM;
char * header(){
    char *buff;
    buff = (char *)malloc(sizeof(char)*100);
    sprintf(buff,"%s:%s $ ",sysname,pwd);
    return buff;
}
char ** getInstruction(int *argc){
    char *buff;
    char **Ins;
    buff = (char *)malloc(sizeof(char)*100);
    Ins = (char **)malloc(sizeof(char *)*10);
    for(int i=0;i<10;i++)
        Ins[i] = (char *)malloc(sizeof(char)*10);
    printf("%s",header());
    fgets(buff,100,stdin);
    buff[strlen(buff)-1]='\0';
    buff = trim(buff);
    //printf("#%s\n",buff);
    Ins = split(buff," ",argc);
    return Ins;
}
void help(){
    printf("**********************HELP**********************\n");
    printf("%-10s - %s\n","exit","退出并保存文件系统");
    printf("%-10s - %s\n","pwd","打印当前工作目录");
    printf("%-10s - %s\n","ls","文件目录信息");
    printf("%-10s - %s\n","mkdir","新建文件夹");
    printf("%-10s - %s\n","cd","进入工作目录");
    printf("%-10s - %s\n","create","新建文件");
    printf("%-10s - %s\n","rm","删除文件");    
    printf("%-10s - %s\n","rmdir","删除文件夹");
    printf("%-10s - %s\n","open","打开文件");
    printf("%-10s - %s\n","close","关闭文件");
    printf("%-10s - %s\n","write","写文件 [id][w-覆盖写 a-截断写 c-追加写]");
    printf("%-10s - %s\n","read","读文件 [id]");
    printf("%-10s - %s\n","opl","当前正在被使用的文件列表");
    printf("%-10s - %s\n","block0","引导块信息");
    printf("%-10s - %s\n","fat","打印FAT表 起始-终止");
    printf("%-10s - %s\n","fcb","查看FCB信息 盘块+偏移");
    printf("%-10s - %s\n","pfcb","查看当前FCB");
    printf("%-10s - %s\n","sbc","显示盘块连 [起始]");
    printf("%-10s - %s\n","sbd","显示当前盘块数据");
    printf("%-10s - %s\n","format","格式化磁盘 [扇区大小]");
/*
    printf("%-10s - %s\n","in","read file to disk from outer file");
    printf("%-10s - %s\n","out","write file from disk to outer file");
    printf("%-10s - %s\n","time","show current time");
*/
}
int doOpration(int argc,char ** argv){
    //printf("%d **%s**\n",argc,argv[0]);

if(strcmp(argv[0],"format")==0){
        if(argc!=2){
            printf("usage %s [sector size]\n",argv[0]);
            return -1;
        }
        else{
            int size=atoi(argv[1]);
            block0.blocksize=size;
	        BLOCK_SIZE=size;
            BLOCK_NUMS=size;
            FAT_ITEM_NUM=size;
            FCB_ITEM_NUM=size/24;
            format();
            otherinit();
            return 0;
        }  
    }

if(strcmp(argv[0],"blocksize")==0){
        if(argc>1){
            printf("%s : too many arguments\n",argv[0]);
            return -1;
        }
        else{
            showsize();
            return 0;
        }  
    }
    if(strcmp(argv[0],"help")==0){
        if(argc>1){
            printf("%s : too many arguments\n",argv[0]);
            return -1;
        }
        else{
            help();
            return 0;
        }  
    }

    if(strcmp(argv[0],"exit")==0){
        if(argc>1){
            printf("%s : too many arguments\n",argv[0]);
            return -1;
        }
        else{
            exitsys();
            return -2;
        }  
    }

    if(strcmp(argv[0],"pwd")==0){
        if(argc>1){
            printf("%s : too many arguments\n",argv[0]);
            return -1;
        }
        else{
            printf("%s\n",getPwd());
            return 0;
        }  
    }

    if(strcmp(argv[0],"ls")==0){
        if(argc>1){
            printf("%s : too many arguments\n",argv[0]);
            return -1;
        }
        else{
            my_ls();
            return 0;
        }  
    }

    if(strcmp(argv[0],"mkdir")==0){
        if(argc!=2){
            printf("usage %s [directory name]\n",argv[0]);
            return -1;
        }
        else{
            my_mkdir(argv[1]);
            return 0;
        }  
    }

    if(strcmp(argv[0],"cd")==0){
        if(argc!=2){
            printf("usage %s [directory name]\n",argv[0]);
            return -1;
        }
        else{
            my_cd(argv[1]);
            return 0;
        }  
    }

    if(strcmp(argv[0],"create")==0){
        if(argc!=2){
            printf("usage %s [file name]\n",argv[0]);
            return -1;
        }
        else{
            my_create(argv[1]);
            return 0;
        }  
    }

    if(strcmp(argv[0],"rm")==0){
        if(argc!=2){
            printf("usage %s [file name]\n",argv[0]);
            return -1;
        }
        else{
            my_rm(argv[1]);
            return 0;
        }  
    }

    if(strcmp(argv[0],"rmdir")==0){
        if(argc!=2){
            printf("usage %s [directory name]\n",argv[0]);
            return -1;
        }
        else{
            my_rmdir(argv[1]);
            return 0;
        }  
    }

    if(strcmp(argv[0],"open")==0){
        if(argc!=2){
            printf("usage %s [file name]\n",argv[0]);
            return -1;
        }
        else{
            my_open(argv[1]);
            return 0;
        }
    }

    if(strcmp(argv[0],"close")==0){
        if(argc!=2){
            printf("usage %s [fd num]\n",argv[0]);
            return -1;
        }
        else{
            int a;
            a = atoi(argv[1]);
            if(strcmp(argv[1],"0")&&a==0){
                printf("usage %s [fd num]\n",argv[0]);
                return -1;
            }
            my_close(a);
            return 0;
        }  
    }

    if(strcmp(argv[0],"write")==0){
        if(argc!=3){
            printf("usage %s [fd] [write method]\n",argv[0]);
            return -1;
        }
        else{
            int a1,len=0;
            char a2;
            //printf("a1%s a2%s\n",argv[1],argv[2]);
            a1 = atoi(argv[1]);
            a2 = argv[2][0];
            if((strcmp(argv[1],"0")&&a1==0)){
                printf("usage %s [fd] [write method]\n",argv[0]);
                return -1;
            }
            if(strlen(argv[1])!=1){
                printf("usage %s [fd] [write method]\n",argv[0]);
                return -1;
            }
            if(my_write(a1,&len,a2)==0){
                printf("succeed write to fd %d with %d bytes\n",a1,len);
                return 0;
            }
            return 0;
        }  
    }

    if(strcmp(argv[0],"read")==0){
        if(argc!=2){
            printf("usage %s [fd num]\n",argv[0]);
            return -1;
        }
        else{
            int a,len=0;
            a = atoi(argv[1]);
            if(strcmp(argv[1],"0")&&a==0){
                printf("usage %s [fd num]\n",argv[0]);
                return -1;
            }
            if(my_read(a,&len)==0)
                printf("read fd %d with %d bytes\n",a,len);
            return 0;
        }  
    }

    if(strcmp(argv[0],"block0")==0){
         if(argc>1){
            printf("%s : too many arguments\n",argv[0]);
            return -1;
        }
        else{
            showBlock0();
            return 0;
        }
    }

    if(strcmp(argv[0],"fat")==0){
         if(argc!=3){
            printf("usage %s [blocknum start][blocknum end]\n",argv[0]);
            return -1;
        }
        else{
            int a1,a2;
            //printf("a1%s a2%s\n",argv[1],argv[2]);
            a1 = atoi(argv[1]);
            a2 = atoi(argv[2]);
            if((strcmp(argv[1],"0")&&a1==0)||(strcmp(argv[2],"0")&&a2==0)){
                printf("usage %s [blocknum start][blocknum end]\n",argv[0]);
                return -1;
            }
            showFAT(a1,a2);
            return 0;
        }
    }

    if(strcmp(argv[0],"fcb")==0){
        if(argc!=3){
            printf("usage %s [blocknum] [FCB offset in block]\n",argv[0]);
            return -1;
        }
        else{
            int a1,a2;
            //printf("a1%s a2%s\n",argv[1],argv[2]);
            a1 = atoi(argv[1]);
            a2 = atoi(argv[2]);
            if((strcmp(argv[1],"0")&&a1==0)||(strcmp(argv[2],"0")&&a2==0)){
                printf("usage %s [blocknum] [FCB offset in block]\n",argv[0]);
                return -1;
            }
            showFCB(a1,a2);
            return 0;
        }  
    }

    if(strcmp(argv[0],"opl")==0){
         if(argc>1){
            printf("%s : too many arguments\n",argv[0]);
            return -1;
        }
        else{
            showfdList();
            return 0;
        }
    }

    if(strcmp(argv[0],"pfcb")==0){
         if(argc>1){
            printf("%s : too many arguments\n",argv[0]);
            return -1;
        }
        else{
            showPresentFCB();
            return 0;
        }
    }

    if(strcmp(argv[0],"sbc")==0){
        if(argc!=2){
            printf("usage %s [blocknum]\n",argv[0]);
            return -1;
        }
        else{
            int a;
            a = atoi(argv[1]);
            if(strcmp(argv[1],"0")&&a==0){
                printf("usage %s [blocknum]\n",argv[0]);
                return -1;
            }
            showBlockChain(a);
            return 0;
        }  
    }

    if(strcmp(argv[0],"sbd")==0){
        if(argc!=2){
            printf("usage %s [blocknum]\n",argv[0]);
            return -1;
        }
        else{
            int a;
            a = atoi(argv[1]);
            if(strcmp(argv[1],"0")&&a==0){
                printf("usage %s [blocknum]\n",argv[0]);
                return -1;
            }
            showBlockData(a);
            return 0;
        }  
    }

    if(strcmp(argv[0],"in")==0){
        if(argc!=3){
            printf("usage %s [fd] [outer filename]\n",argv[0]);
            return -1;
        }
        else{
            int a1,len=0;
            char *a2;
            //printf("a1%s a2%s\n",argv[1],argv[2]);
            a1 = atoi(argv[1]);
            a2 = argv[2];
            if((strcmp(argv[1],"0")&&a1==0)){
                printf("usage %s [fd] [outer filename]\n",argv[0]);
                return -1;
            }
            if(strlen(argv[1])!=1){
                printf("usage %s [fd] [write method]\n",argv[0]);
                return -1;
            }
            if(my_in(a1,a2,&len)==0){
                printf("succeed write to fd %d with %d bytes\n",a1,len);
                return 0;
            }
            return 0;
        }  
    }

    if(strcmp(argv[0],"out")==0){
        if(argc!=3){
            printf("usage %s [fd num] [outer filename]\n",argv[0]);
            return -1;
        }
        else{
            int a,len=0;
            a = atoi(argv[1]);
            if(strcmp(argv[1],"0")&&a==0){
                printf("usage %s [fd num] [outer filename]\n",argv[0]);
                return -1;
            }
            if(my_out(a,argv[2],&len)==0)
                printf("read fd %d with %d bytes\n",a,len);
            return 0;
        }  
    }

    if(strcmp(argv[0],"time")==0){
        if(argc>1){
            printf("%s : too many arguments\n",argv[0]);
            return -1;
        }
        else{
            showCurrentTime();
            return 0;
        }  
    }
    printf("%s: command not found\n",argv[0]);
    return 0;
}
void go(){
    char buff[100];
    char **argv;
    int argc,flag;
    argv = (char **)malloc(sizeof(char *)*10);
    for(int i=0;i<10;i++)
        argv[i] = (char *)malloc(sizeof(char)*10);
    while(1){
        argv = getInstruction(&argc);
        //printf("%s\n", *argv);
        flag = doOpration(argc,argv);
        //printf("flag%d\n",flag);
        if(flag==-2)
            break;
    }
    return;
}
