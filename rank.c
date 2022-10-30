#include "rank.h"

// #include <unistd.h>
// #include <fcntl.h>
// #include <stdio.h>
// int main(){
//     // int fd= open("rank.txt",O_WRONLY);
//     // FILE* fp=fopen("rank.txt","w+");
//     // getline(fp)
//     FILE*fp;
//     char buf[100];
//     size_t len;
//     // fscanf(fp,"%d\n",)
//     char *lineptr=NULL;
//     if(access("rank.txt",F_OK)==0){
//         fp=fopen("rank.txt","r+");
//         printf("File exists\n");
//         // fgets(buf,100,fp);
//         getdelim(&lineptr,&len,'\n',fp);
//         printf("%s",lineptr);
//         while (1)
//         {
//             int eof=getdelim(&lineptr,&len,' ',fp); 
//             if(eof==-1){
//                 printf("end 1\n");
//                 break;
//             }
//             printf("%s :: ",lineptr);
//             eof=getdelim(&lineptr,&len,'\n',fp);
//             if(eof==-1){
//                 printf("End 2\n");
//                 break;
//             }
//             printf("%s\n",lineptr);
//         }
        
    
//     }else{
//         printf("no file\n");
//         // fprintf(fp,"%d",0);
//     }   
// }

static bool rank_cmp(struct list_elem* a, struct list_elem* b, void *aux){
    struct RankElem* ra=list_entry(a,struct RankElem,elem);
    struct RankElem* rb=list_entry(b,struct RankElem,elem);
    if(ra->point>ra->point){
        return false;
    }
    return true;
}

void InitRank(){
    // char buf[MAXLINE];
    char* lineptr;
    size_t len;
    struct RankElem* r_elem;
    if(access(FILENAME,F_OK)==0){
        fp=fopen(FILENAME,"r+");
        getdelim(&lineptr,&len,'\n',fp);
        rank_list_size=atoi(lineptr);
        // while(getdelim(&lineptr,&))
        while(1){
            if(getdelim(&lineptr,&len,' ',fp)==EOF){
                break;
            }
            r_elem=malloc(sizeof *r_elem);
            strncpy(r_elem->name,lineptr,len-1);

            getdelim(&lineptr,&len,'\n',fp);
            r_elem->point=atoi(lineptr);

            list_insert_ordered(&rank_list,&r_elem->elem,rank_cmp,NULL);
        }

    }else{
        fp=fopen(FILENAME,"w");
        fprintf(fp,"%d\n",0);
    }
}

void PrintRank(int X,int Y){
    X=0;
    X=5;
    int i,j;
    struct list_elem* iter;
    struct RankElem* r_iter;
    for(iter=list_begin(&rank_list),i=0;iter!=list_end(&rank_list)&&i<X ; iter=list_next(iter),i++  );
    if(i<X){
        // error handle
    }
    for(;iter!=list_end(&rank_list);iter=list_next(iter)){
        r_iter=list_entry(iter,struct RankElem,elem);
        printw("%s ",r_iter->name);
        printf("%d\n",r_iter->point);
    }
}

void SyncFile(){

}

struct RankElem* FindByName(char * name){

}

void DeleteByName(char *name){
    // find by name
    // delete
    // Syncfile
}

void PrintRankByName(){
    // find by name
    // printit
}