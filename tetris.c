#include "tetris.h"

static struct sigaction act, oact;


// move : absolute postion
// addch : cur posiition + 1(x)
void rs(){
	refresh();
	sleep(2);
}

int main(){
	int exit=0;
	fd=open("rank.txt",O_WRONLY);
	initscr();
	noecho();
	keypad(stdscr, TRUE);	
	curs_set(false);
	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: 
			play(); 
			break;
		case MENU_RANK:
			rank();
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
	InitBlock(&cur_block_);
	InitBlock(&next_block_);


	score=0;	
	gameOver=false;
	timed_out=0;

	DrawOutline();

	
	DrawField();

	DrawBlock(&cur_block_,' ');
	
	DrawNextBlock(next_block_.shape);


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
	struct Block check_block=cur_block_;

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
		while(CheckToMove(&check_block)){
			check_block.y++;
		}
		check_block.y--;
		break;
	default:
		break;
	}
	can_move= CheckToMove(&check_block);

	if(can_move==true){ 
		DrawChange(&cur_block_,&check_block);
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
				move(i+blk->y+1,j+blk->x+1);

				printw("%c",'.');

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
	sigaction(SIGALRM,&act,&oact);
	// signal(SIGALRM,BlockDown);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}
		// DeleteBlock(&shadow_);
		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
		struct Block check_block=cur_block_;
		check_block.y++;
		bool can_move=CheckToMove(&check_block);


		if(!can_move){
			DrawBlock(&cur_block_,' ');
			Freeze();
			int delta=BreakLine();
			if(delta>0){
				DrawField();
				score+=delta;
				PrintScore(score);
			}

			cur_block_=next_block_;
			InitBlock(&next_block_);
			gameOver=CheckGameOver(&cur_block_);
			DrawNextBlock(next_block_.shape);
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

bool CheckToMove(struct Block* check_block){

	int i;
	int j;


	for(i=0;i<BLOCK_HEIGHT;i++){
		for(j=0;j<BLOCK_WIDTH;j++){
			if(block[check_block->shape][check_block->rotate][i][j]==1){
				
				if(check_block->y+i>=HEIGHT ){
					return false;
				}

				if(field[check_block->y+i][check_block->x+j]==1){
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
	while(CheckToMove(&shadow_)){
		shadow_.y++;
	}
	shadow_.y--;
	DrawBlock(&shadow_,'/');

	memcpy(prev_block,new_block,sizeof *prev_block);

}
void BlockDown(int sig){


	bool can_move;

	struct Block check_block=cur_block_;
	check_block.y++;
	can_move=CheckToMove(&check_block);
	if(!can_move){ 

		Freeze();

		gameOver=CheckGameOver(&cur_block_);
		cur_block_=next_block_;
		InitBlock(&next_block_);
		DrawNextBlock(next_block_.shape);
		return;
	}else{

		DrawChange(&cur_block_,&check_block);
	}
	timed_out=0;

}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){

}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(struct Block changed){



}


void createRankList() {
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt"열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
	FILE* fp;
	int i, j;

	//1. 파일 열기
	// fp = fopen("rnak.txt", "r");

	// 2. 정보읽어오기
	/* int fscanf(FILE* stream, const char* format, ...);
	stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
	format: 형식지정자 등등
	변수의 주소: 포인터
	return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
	// EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
	// if (fscanf() != EOF) {




	// }
	// else {


	// }
	// 4. 파일닫기
	fclose(fp);
}

void rank() {
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	// int X = 1, Y = score_number
	int ch, i, j;
	clear();
	int X,Y;
	char inputname[NAMELEN+1];
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
		X=wgetch(stdscr);
		// X-=48;
		printw("%c\n", X==10 ? ' ' :X);
		printw("Y : ");
		Y=wgetch(stdscr);
		// Y-=48;
		printw("%c\n", Y==10 ? ' ': Y);
		/* code */
		
		break;
	case 2:
		printw("Input the name : ");
		getstr(inputname);
		printw("\b%s",inputname);
		break;
	case 3:
		break;
	default:
		break;
	}

	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	if (ch == '1') {

	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if (ch == '2') {
		char str[NAMELEN + 1];
		int check = 0;


	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if (ch == '3') {
		int num;

	}
	getch();

}

void writeRankFile() {
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	int sn, i;
	//1. "rank.txt" 연다
	// FILE* fp = fopen("rank.txt", "r");

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록

	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	// if (sn == score_number) return;
	// else {


	// }
	// for (i = 1; i < score_number + 1; i++) {
	// 	free(a.rank_name[i]);
	// }
	// free(a.rank_name);
	// free(a.rank_score);
}

void newRank(int score) {
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN + 1];
	int i, j;
	clear();
	//1. 사용자 이름을 입력받음

	//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
	// if () {

	// }
	// else {

	// }
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}

void Freeze(){
	int i;
	int j;

	for(i=0;i<BLOCK_HEIGHT;i++){
		for(j=0;j<BLOCK_WIDTH;j++){
			if(block[cur_block_.shape][cur_block_.rotate][i][j]==1){
				field[cur_block_.y+i][cur_block_.x+j]=1;
			}			
		}
	}
}



void InitBlock(struct Block* blk){
	blk->shape=rand()%7;
	blk->rotate=0;
	blk->x=WIDTH/2-2;
	blk->y=-1;
	blk->bid=block_id++;
}

int BreakLine(){
	int i;
	int j;
	int delta=0;
	for(i=HEIGHT-1;i>=0;i--){
		bool can_break=true;

		for(j=0;j<WIDTH-1;j+=2){
			if(!field[i][j] || !field[i][j+1]){
				// 0,1 / 2,3 / 4,5 / 6,7 / 8,9 
				can_break=false;
			}
		}

		if(can_break){
			// bf();
			memcpy(field[1],field[0],sizeof(char)*WIDTH*(i));
			delta++;
			i++;
		}
	}

	return delta;
}