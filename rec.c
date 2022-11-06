#include "rec.h"

sem_t global_mutex;

static sem_t worker_completed_mutex;
static int worker_completed;



void worker(void * arg){
    int identifier=gettid()%4;
    // pthread_cond_wait(),
    char cur_field[HEIGHT][WIDTH];
    
    while(1){
        // calculation

        int index=0;

        memcpy(cur_field,field,sizeof *field);
        struct Block* first=list_entry(list_front(&b_list),struct Block,elem);

        struct coor* coor=&NumOfCase[first->shape][identifier];

        // calculate with field
        for(index=coor->min;index<coor->max;index++){
            
        }


        sem_wait(&worker_completed_mutex);
        worker_completed++;
        sem_post(&worker_completed_mutex);
        if(worker_completed==4){
            sem_post(&global_mutex);
        }
        pthread_cond_wait(&cond,NULL);
    }
}

void InitRec(){
    int i;
    t_count=0;
    worker_completed=0;
    sem_init(&global_mutex,0,0);
    sem_init(&worker_completed_mutex,0,1);
    root=malloc(sizeof* malloc);
    sem_init(&root->recnode_mutex,0,1);
    for(i=0;i<PTHREAD_N;i++){

        pthread_create(&tid[i],NULL,worker,NULL);
    }
}


int CalCulateScore(int shape,int rotate,int x){
    struct Block check_block;
    check_block.shape=shape;
    check_block.rotate=rotate;
    check_block.x=x;
    check_block.y=-1;
    while (CheckToMove(&check_block))
    {
        check_block.y++;
    }
    
    
    
}