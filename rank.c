#include "rank.h"

// #include <unistd.h>
// #include <fcntl.h>
// #include <stdio.h>
int main(){
    // int fd= open("rank.txt",O_WRONLY);
    // FILE* fp=fopen("rank.txt","w+");
    // getline(fp)
    FILE*fp;
    char buf[100];
    // fscanf(fp,"%d\n",)
    if(access("rank.txt",F_OK)==0){
        fp=fopen("rank.txt","r+");
        printf("File exists\n");
        fgets(buf,100,fp);
        printf("%s",buf);
        char bb[100][2];
        int len;
        getdelim(bb,&len,' ',fp);
        printf("%s %d",buf,len);
    }else{
        printf("no file\n");
        // fprintf(fp,"%d",0);
    }   
}

// void InitRank(){
//     char buf[MAXLINE];
//     if(access(FILENAME,F_OK)==0){
//         fp=fopen(FILENAME,"r+");
//         fgets(buf,MAXLINE,fp);
//         rank_list_size=atoi(buf);
//         getdelim()

//     }else{
//         fp=fopen(FILENAME,"w");
//         fprintf(fp,"%d\n",0);
//     }

// }