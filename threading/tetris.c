#include "tetris.h"


static struct sigaction act, oact;


static void InitBlockList(){
	struct list_elem* iter;
	struct Block* b_iter;
	for(iter=list_begin(&b_list);iter!=list_end(&b_list);){
		b_iter=list_entry(iter,struct Block,elem);
		iter=list_remove(iter);
		free(b_iter);
	}
	list_init(&b_list);
	int i;
	for(i=0;i<REC_CAL;i++){
		InitBlock(&b_iter);
		list_push_back(&b_list,&b_iter->elem);
	}
}
// move : absolute postion
// addch : cur posiition + 1(x)
void rs(){
	refresh();
	sleep(2);
}

int main(int argc,char** argv){
	int exit=0;
	
	if(argc==2){
		REC_CAL=atoi(argv[1]);
	}
	InitRank();
	list_init(&b_list);

	initscr();
	noecho();
	keypad(stdscr, TRUE);	
	curs_set(false);
	srand((unsigned int)time(NULL));
	list_init(&b_list);

	InitRec();

	while(!exit){
		recommend_mode=false;
		InitBlockList();
		clear();
		switch(menu()){
		case MENU_PLAY: 
			play(); 
			break;
		case MENU_RANK:
			rank();
			break;
		case MENU_RECM:
			recommend_mode=true;
			InitRec();
			play();
			break;
		case MENU_EXIT: 
			exit=1; 
			break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}
bool CheckGameOver(struct Block* blk){
	int i;
	int j;
	for(i=0;i<BLOCK_HEIGHT;i++){
		for(j=0;j<BLOCK_WIDTH;j++){
			if(block[blk->shape][blk->rotate][i][j]==1){
				
				if(field[blk->y + i][blk->x+j]==1){
					return true;
				}
			}
		}
	}
	return false;
}

void InitTetris(){
	int i,j;
	block_id=0;
	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;


	// GetNewBlock();
	cur_block_=list_entry(list_front(&b_list),struct Block,elem);
	next_block_=list_entry(list_front(&b_list)->next,struct Block,elem);

	score=0;	
	gameOver=false;
	timed_out=0;

	DrawOutline();

	
	DrawField();

	DrawBlock(cur_block_,' ');
	
	DrawNextBlock(next_block_->shape);


	PrintScore(score);

}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(9,WIDTH+10);
	printw("SCORE");
	DrawBox(10,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case KEY_SPACE:	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	bool can_move=0;
	struct Block check_block=*cur_block_;

	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		check_block.rotate=(check_block.rotate+1)%4;
		break;
	case KEY_DOWN:
		check_block.y++;
		break;
	case KEY_RIGHT:
		check_block.x++;
		break;
	case KEY_LEFT:
		check_block.x--;
		break;
	case KEY_SPACE:
		while(CheckToMove(&check_block,field)){
			check_block.y++;
		}
		check_block.y--;
		break;
	default:
		break;
	}
	can_move= CheckToMove(&check_block,field);

	if(can_move==true){ 
		DrawChange(cur_block_,&check_block);
	}

	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(11,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int next_blk_shape){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[next_blk_shape][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(struct Block* cur_blk,char tile){
	int i,j;
	for(i=0;i<BLOCK_HEIGHT;i++)
		for(j=0;j<BLOCK_WIDTH;j++){
			if(block[cur_blk->shape][cur_blk->rotate][i][j]==true && i+cur_blk->y>=0){
				move(i+cur_blk->y+1,j+cur_blk->x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}else{
				// move(i+cur_blk->y+1,j+cur_blk->x+1);
				// printw("x");
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DeleteBlock(struct Block* blk){
	int i,j;
	for(i=0;i<BLOCK_HEIGHT;i++)
		for(j=0;j<BLOCK_WIDTH;j++){
			if(block[blk->shape][blk->rotate][i][j]==true && i+blk->y>=0){
				if(field[i+blk->y][j+blk->x]==0){
				move(i+blk->y+1,j+blk->x+1);
				
				printw("%c",'.');
				}
			}
		}

	move(HEIGHT,WIDTH+10);

}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}
void bf(){
 // for gdb break point
}
void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	// act.sa_handler=bf;
	sigaction(SIGALRM,&act,&oact);
	// signal(SIGALRM,BlockDown);
	InitTetris();
	bool can_move=false;
	do{

		if(recommend_mode){
			
			if(!can_move){

				DeleteBlock(&cur_block_);
				DeleteBlock(&rec_block_);
				struct RecursiveRet rr;
				rr.score=INT32_MIN;
				int i;
				for(i=0;i<PTHREAD_N;i++){
					sem_post(&worker_mutex[i]);
				}
				sem_wait(&global_mutex);

				for(i=0;i<PTHREAD_N;i++){
					if(recommend_result[i].score>=rr.score){
						rr.rotate=i;
						rr.score=recommend_result[i].score;
						rr.x=recommend_result[i].x;
					}
				}
				if(!(rr.rotate<4&&rr.rotate>=0&rr.x>=-2&&rr.x<WIDTH-1)){
					bf();
				}
					// rr=RecursiveCalculateScore(list_front(&b_list),field);
					// if(!(rr.rotate<4&&rr.rotate>=0)){

					// 	rr.rotate=rand()%NUM_OF_ROTATE;
					// 	rr.x=WIDTH/2;
					// }
	
				cur_block_->x=rr.x;
				cur_block_->rotate=rr.rotate;
				
				rec_block_=*cur_block_;
				while(CheckToMove(&rec_block_,field)){
					rec_block_.y++;
				}
				rec_block_.y--;

			}
			DrawBlock(&rec_block_,'R');

		}
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}
		// DeleteBlock(&shadow_);
		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			int i;
			for(i=0;i<PTHREAD_N;i++){
				pthread_cancel((pthread_t)tid[i]);
			}
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
		struct Block check_block=*cur_block_;
		check_block.y++;
		can_move=CheckToMove(&check_block,field);


		// printw("%d",CalCulateScore(check_block.shape,check_block.rotate,check_block.x,field));
		if(!can_move){
			DrawBlock(cur_block_,' ');
			Freeze(field,cur_block_);
			int delta=BreakLine(field);
			if(delta>0){
				DrawField();
				score+=(delta*10);
				PrintScore(score);
			}
	
			list_remove(&cur_block_->elem);
			free(cur_block_);

			struct Block* blk;
			InitBlock(&blk);
			list_push_back(&b_list,&blk->elem);

			cur_block_=list_entry(list_front(&b_list),struct Block,elem);
			next_block_=list_entry(list_front(&b_list)->next,struct Block,elem);


			gameOver=CheckGameOver(cur_block_);
			if(gameOver){
				break;
			}

			DrawNextBlock(next_block_->shape);

		}

	}while(1);

	alarm(0);
	int i;
	for(i=0;i<PTHREAD_N;i++){
		pthread_cancel((pthread_t)tid[i]);
		}
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();

	getch();
	
	NewRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play (REC_CAL:%d)\n",REC_CAL);
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

bool CheckToMove(struct Block* check_block,char f[HEIGHT][WIDTH]){

	int i;
	int j;


	for(i=0;i<BLOCK_HEIGHT;i++){
		for(j=0;j<BLOCK_WIDTH;j++){
			if(block[check_block->shape][check_block->rotate][i][j]==1){
				
				if(check_block->y+i>=HEIGHT ){
					return false;
				}

				if(f[check_block->y+i][check_block->x+j]==1){
					return false;	
				}
				if(check_block->x+j<0 || check_block->x+j>=WIDTH){
					return false;
				}

			}
		}
	}

	return true;
}

void DrawChange(struct Block* prev_block,struct Block* new_block){
	if(shadow_.bid==new_block->bid){
		DeleteBlock(&shadow_);
	}

	DeleteBlock(prev_block);


	DrawBlock(new_block,' ');

	shadow_=*new_block;
	while(CheckToMove(&shadow_,field)){
		shadow_.y++;
	}
	shadow_.y--;
	DrawBlock(&shadow_,'/');

	memcpy(prev_block,new_block,sizeof *prev_block);

}
void BlockDown(int sig){

	timed_out=0;
	bool can_move;



	struct Block check_block=*cur_block_;
	check_block.y++;

	can_move=CheckToMove(&check_block,field);

	if(recommend_mode){
		while (CheckToMove(&check_block,field))
		{
			check_block.y++;
		}
		check_block.y--;
		// DeleteBlock(cur_block_);
		
	}

	if(!can_move){ 

		Freeze(field,cur_block_);


		
		list_remove(&cur_block_->elem);
		

		free(cur_block_);
		
		struct Block* blk;
		InitBlock(&blk);
		list_push_back(&b_list,&blk->elem);	
		cur_block_=list_entry(list_front(&b_list),struct Block,elem);
		next_block_=list_entry(list_front(&b_list)->next,struct Block,elem);
		

		gameOver=CheckGameOver(cur_block_);
		

		DrawNextBlock(next_block_->shape);
		return;
	}else{

		DrawChange(cur_block_,&check_block);
	}


}


void rank() {
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	// int X = 1, Y = score_number
	int ch, i, j;
	clear();
	int X,Y;


	//2. printw()로 3개의 메뉴출력
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by specific name\n");
	printw("3. delete a specifc rank\n");	
	// printw()
	ch=wgetch(stdscr);
	// printw("%d\n",ch);
	if(ch<='0' || ch>'3'){
		printw("Failure :: No Such Mode : %d\n",ch);
		getch();
		return;
	}
	ch-=48;
	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장

	switch (ch)
	{
	case 1:
		printw("X : ");
		getstr(buf);
		move(3,4);
		if(!strcmp(buf,"\0")){
			X=0;
			printw("\n");
		}else{
			X=atoi(buf);
			printw("%d\n",X);
		}


		printw("Y : ");
		getstr(buf);
		move(4,4);
		if(!strcmp(buf,"\0")){
			Y=INT32_MAX;
			printw("\n");
		}else{
			Y=atoi(buf);
			printw("%d\n",Y);
		}

		PrintRankXtoY(X-1,Y);
		break;
	case 2:
		printw("Input the name : ");
		getstr(buf);
		move(3,17);
		printw("%s\n",buf);

		PrintRankByName(buf);
		break;
	case 3:
		printw("Input the rank : ");
		getstr(buf);
		move(3,17);
		printw("%s\n",buf);
		X=atoi(buf);

		DeleteRankByIdx(X-1);
		break;
	default:
		break;
	}
	getch();

}


void Freeze(char f[HEIGHT][WIDTH],struct Block* blk){
	move(50,50);
	int i;
	int j;

	for(i=0;i<BLOCK_HEIGHT;i++){
		for(j=0;j<BLOCK_WIDTH;j++){
			if(block[blk->shape][blk->rotate][i][j]==1){
				f[blk->y+i][blk->x+j]=1;
			}			
		}
	}
}



void InitBlock(struct Block** blk){
	*blk=malloc(sizeof(struct Block));
	assert(blk);
	(*blk)->shape=rand()%7;
	// (*blk)->shape=6;
	(*blk)->rotate=0;
	(*blk)->x=WIDTH/2-2;
	(*blk)->y=-1;
	(*blk)->bid=block_id++;
}

int BreakLine(char f[HEIGHT][WIDTH]){
	int i;
	int j;
	int delta=0;
	for(i=HEIGHT-1;i>=0;i--){
		bool can_break=true;

		for(j=0;j<WIDTH-1;j+=2){
			if(!f[i][j] || !f[i][j+1]){
				// 0,1 / 2,3 / 4,5 / 6,7 / 8,9 
				can_break=false;
			}
		}

		if(can_break){
			// bf();
			memcpy(f[1],f[0],sizeof(char)*WIDTH*(i));
			delta++;
			i++;
		}
	}

	return delta;
}



// thread 1:
//     pthread_mutex_lock(&mutex);
//     while (!condition)
//         pthread_cond_wait(&cond, &mutex);
//     /* do something that requires holding the mutex and condition is true */
//     pthread_mutex_unlock(&mutex);

// thread2:
//     pthread_mutex_lock(&mutex);
//     /* do something that might make condition true */
//     pthread_cond_signal(&cond);
//     pthread_mutex_unlock(&mutex);

void worker(void * arg){
	pthread_detach(pthread_self());
	sem_wait(&worker_completed_mutex);
    int identifier=thread_identifier%PTHREAD_N;
	thread_identifier++;
	sem_post(&worker_completed_mutex);
    // pthread_cond_wait(),
    char cur_field[HEIGHT][WIDTH];
    
    while(1){
        // calculation
		// pthread_cond_wait(&cond,NULL);
		sem_wait(&worker_mutex[identifier]);

        int i;
		int j;

		struct RecursiveRet ret;
		ret.score=INT32_MIN;
		ret.rotate=identifier;
        struct Block* first=list_entry(list_front(&b_list),struct Block,elem);

        struct range* range=&NumOfCase[first->shape][identifier];
		
        // calculate with field
		// if(1){
			for(i=range->min;i<=range->max;i++){
				struct coor coor[4];
				memcpy(cur_field,field,sizeof field);
				int res=CalCulateScore(first->shape,identifier,i,cur_field);

				struct RecursiveRet rr;


				rr=RecursiveCalculateScore(first->elem.next,cur_field);
				
				if(res+rr.score>=ret.score){
					ret.score=res+rr.score;
					ret.x=i;
				}
			}






        sem_wait(&worker_completed_mutex);
		recommend_result[identifier].score=ret.score;
		recommend_result[identifier].x=ret.x;
        worker_completed++;
        sem_post(&worker_completed_mutex);
        if(worker_completed==4){
			worker_completed=0;
            sem_post(&global_mutex);
        }
        
    }
}

void InitRec(){
    int i;
    t_count=0;
    worker_completed=0;
    sem_init(&global_mutex,0,0);
    sem_init(&worker_completed_mutex,0,1);
    // root=malloc(sizeof* malloc);
    // sem_init(&root->recnode_mutex,0,1);
    for(i=0;i<PTHREAD_N;i++){
		sem_init(&worker_mutex[i],0,0);
    }
    for(i=0;i<PTHREAD_N;i++){
        pthread_create(&tid[i],NULL,worker,NULL);
    }

}

int dx[]={-1,1,0};
int dy[]={0,0,1};

int CalCulateScore(int shape,int rotate,int x,char cur_field[HEIGHT][WIDTH]){
    struct Block check_block;
	int i;
	int j;
	int ret;

    check_block.shape=shape;
    check_block.rotate=rotate;
    check_block.x=x;
    check_block.y=-1;
    while (CheckToMove(&check_block,cur_field))
    {
        check_block.y++;
    }
	check_block.y--;




	// score+=100*lowy;
	Freeze(cur_field,&check_block);

	ret=check_block.y;

	ret+=BreakLine(cur_field)*300;
	// ret=delta;
	// if(delta){
	// 	move(50,40);
	// 	printw("%d // %d",check_block.x,delta);
	// }
	return ret;
}

struct RecursiveRet RecursiveCalculateScore(struct list_elem* cur, char cur_field[HEIGHT][WIDTH]){
	struct RecursiveRet ret;
	ret.score=INT32_MIN;
	
	if(cur==list_end(&b_list)){
		return ret;
	}
	struct Block* blk=list_entry(cur,struct Block,elem);
	char new_field[HEIGHT][WIDTH];
	// struct coor candidate[16];
	int i;
	int j;




	for(i=0;i<NUM_OF_ROTATE;i++){ // i= rotate
		struct range range=NumOfCase[blk->shape][i];
		for(j=range.min;j<=range.max;j++){ // j = x
			int res=0;
			struct RecursiveRet rr;
			memcpy(new_field,cur_field,sizeof new_field);

			res=CalCulateScore(blk->shape,i,j,new_field);
			

			rr=RecursiveCalculateScore(cur->next,new_field);	

			if(res+rr.score>=ret.score){
				ret.score=res+rr.score;
				ret.x=j;
				ret.rotate=i;
			}

		}
	}
	return ret;
}

void RecommendNextBlock(void){
	int i;
	int j;
	int index;
	int score=INT32_MIN;
	for(i=0;i<PTHREAD_N;i++){
		sem_post(&worker_mutex[i]);
	}
	sem_wait(&global_mutex);
	for(i=0;i<PTHREAD_N;i++){
		if(recommend_result[i].score>=score){
			index=i;
			score=recommend_result[i].score;
		}
	}
	DeleteBlock(&rec_block_);
	
	DeleteBlock(cur_block_);
	cur_block_->x=recommend_result[index].x;
	cur_block_->rotate=index;
	rec_block_=*cur_block_;
	// while(CheckToMove(&rec_block_,)){
		// rec_block_.y++;
	// }
	rec_block_.y--;
	// move(50,50);
	// printw("%d",score);
}