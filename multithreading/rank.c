#include "rank.h"

static bool rank_cmp(const struct list_elem* a,const struct list_elem* b, void *aux){
    struct RankElem* ra=list_entry(a,struct RankElem,elem);
    struct RankElem* rb=list_entry(b,struct RankElem,elem);
    if(ra->point>rb->point){
        return true;
    }
    return false;
}

void InitRank(){
    // char buf[MAXLINE];
    list_init(&rank_list);
    char* lineptr;
    size_t len;
    char buffer[MAXLINE];
    struct RankElem* r_elem;
    if(access(FILENAME,F_OK)==0){
        fp=fopen(FILENAME,"r+");
        fgets(buffer,MAXLINE,fp);
        rank_list_size=atoi(buffer);

        while(1){
            if(getdelim(&lineptr,&len,' ',fp)==EOF){
                break;
            }
            r_elem=malloc(sizeof *r_elem);
            strncpy(r_elem->name,lineptr,strlen(lineptr)-1);

            getdelim(&lineptr,&len,'\n',fp);
            r_elem->point=atoi(lineptr);

            list_insert_ordered(&rank_list,&r_elem->elem,rank_cmp,NULL);
        }
        fclose(fp);
    }
}

void PrintRankXtoY(int X,int Y){

    int i;
    bool exist=false;
    struct list_elem* iter;
    struct RankElem* r_iter;

    printw("                name          |    score\n");
    printw("--------------------------------------------\n");
    for(iter=list_begin(&rank_list),i=0;iter!=list_end(&rank_list)&&i<X ; iter=list_next(iter),i++  );

    for(;iter!=list_end(&rank_list)&&i<Y ;iter=list_next(iter),i++){

        r_iter=list_entry(iter,struct RankElem,elem);
        printw("  %-27s |  ",r_iter->name);
        printw("%-8d\n",r_iter->point);
        exist=true;
    }
    if(!exist){
        printw("search failure: no rank in the list");
    }
}

void SyncRankFile(){
    char line[MAXLINE];
    struct list_elem* iter;
    struct RankElem* r_iter;
    size_t cnt=0;
    memset(buf,0,BUFSIZE);
    for(iter=list_begin(&rank_list);iter!=list_end(&rank_list);iter=list_next(iter)){
        r_iter=list_entry(iter,struct RankElem,elem);
        sprintf(line,"%s %d\n",r_iter->name,r_iter->point);
        strcat(buf,line);
        cnt++;
    }
    fp=fopen(FILENAME,"w+");
    rank_list_size=cnt;
    fprintf(fp,"%ld\n",cnt);
    fprintf(fp,buf);
    fflush(fp);
    fclose(fp);
}



void PrintRankByName(char * name){
    bool exist=false;
    struct list_elem* iter;
    struct RankElem* r_iter;


    printw("                name          |    score\n");
    printw("--------------------------------------------\n");

    for(iter=list_begin(&rank_list);iter!=list_end(&rank_list);iter=list_next(iter)){
        r_iter=list_entry(iter,struct RankElem,elem);
        if(!strcmp(r_iter->name,name)){
            printw("  %-27s |  ",r_iter->name);
            printw("%-8d\n",r_iter->point);
            exist=true;
        }
    }
    if(!exist){
        printw("\nsearch failure: no rank in the list");
    }
}

void DeleteRankByIdx(int idx){
    int i;
    struct list_elem* iter;
    struct RankElem* r_iter;
    if(idx+1>rank_list_size){
        printw("\nsearch failure: no rank in the list");
        return;
    }    
    for(i=0,iter=list_begin(&rank_list);iter!=list_end(&rank_list)&&i<idx ;iter=list_next(iter),i++);

    list_remove(iter); 
    r_iter=list_entry(iter,struct RankElem,elem);
    free(r_iter);
    SyncRankFile();
    printw("\nresult: the rank deleted\n");
}

void NewRank(int score) {

	char str[NAMELEN + 1];
	int i, j;
	clear();

	printw("your name : ");
	move(0,10);
	getstr(str);
	printw("%s",str);

	struct RankElem* r_elem=malloc(sizeof* r_elem);
	strcpy(r_elem->name,str);
	r_elem->point=score;

    list_insert_ordered(&rank_list,&r_elem->elem,rank_cmp,NULL);

    SyncRankFile();
    getch();
}
