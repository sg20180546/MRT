#ifndef _TETRIS_H_
#define _TETRIS_H_


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include <unistd.h>
#include <fcntl.h>
#include "rank.h"

#include <setjmp.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define WIDTH	10
#define HEIGHT	22
#define NOTHING	0
#define QUIT	'q'
#define NUM_OF_SHAPE	7
#define NUM_OF_ROTATE	4
#define BLOCK_HEIGHT	4
#define BLOCK_WIDTH	4
#define BLOCK_NUM	2

#define KEY_SPACE ' '

#define MENU_PLAY '1'
#define MENU_RANK '2'
#define MENU_RECM '3'
#define MENU_EXIT '4'



/* RECOMMONED */
int REC_CAL=4;
#define PTHREAD_N 4
#define CHILDREN_MAX 36
jmp_buf jbuf;
int t_count;
sem_t worker_completed_mutex;
bool recommend_mode=false;
static int worker_completed;



pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
sem_t global_mutex;
typedef struct _RecNode{
	int lv,score;
	char (*f)[WIDTH];
	struct _RecNode *c[CHILDREN_MAX];
} RecNode;
pthread_t tid[PTHREAD_N];
int thread_identifier=0;
sem_t worker_mutex[PTHREAD_N];

struct recommend_res{
	int score;
	int x;
};

struct recommend_res recommend_result[PTHREAD_N];
struct range{
    int min;
    int max;
};
struct coor{
	int x;
	int y;
};

struct RecursiveRet{
	int score;
	int x;
	int rotate;
};
const struct range NumOfCase[NUM_OF_SHAPE][NUM_OF_ROTATE]={
    {
        {0,6},{-1,8},{0,6},{-1,8}
    },
    {
        {-1,6},{-2,6},{-1,6},{-1,7}
    },
    {
        {-1,6},{-2,6},{-1,6},{-1,7}
    },
    {
        {0,7},{0,8},{0,7},{-1,7}
    },
    {
        {-1,7},{-1,7},{-1,7},{-1,7}
    },
    {
        {-1,-6},{-1,7},{-1,6},{-1,7}
    },
    {
        {-1,6},{-1,7},{-1,6},{-1,7}
    }
};





int fd;
long block_id;
/* [blockShapeID][# of rotate][][]*/
const char block[NUM_OF_SHAPE][NUM_OF_ROTATE][BLOCK_HEIGHT][BLOCK_WIDTH] ={
	{/*[0][][][]					▩▩▩▩*/
		{/*[][0][][]*/
			{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}			
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}			
		},
		{/*[][3][][]*/
			{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}
		}
	},
	{/*[1][][][];					  ▩▩▩*/
		{/*[][0][][]				    ▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0 ,1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}
		}
	},
	{/*[2][][][];					  ▩▩▩*/
		{/*[][0][][]				  ▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}
		}
	},
	{/*[3][][][];					   ▩*/
		{/*[][0][][]				  ▩▩▩*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		}
	},
	{/*[4][][][];					  ▩▩*/
		{/*[][0][][]				  ▩▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}
		}
	},
	{/*[5][][][];					 ▩▩*/
		{/*[][0][][]				▩▩*/
			{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}			
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}			
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}			
		}
	},
	{/*[6][][][];					▩▩*/
		{/*[][0][][]				 ▩▩*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][1][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		},
		{/*[][2][][]*/
			{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}
		},
		{/*[][3][][]*/
			{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}
		}
	}
};


/*
  
(0,0)------------	
	|			|
	|			|
	|			|
	|			|
	|			|
	|			|
	|			|
	|			|
	-------------(10,22)
*/


char field[HEIGHT][WIDTH];	/* 테트리스의 메인 게임 화면 */

// int nextBlock[BLOCK_NUM];	/* 현재 블럭의 ID와 다음 블럭의 ID들을 저장; [0]: 현재 블럭; [1]: 다음 블럭 */
// int blockRotate,blockY,blockX;	/* 현재 블럭의 회전, 블럭의 Y 좌표, 블럭의 X 좌표*/

struct Block{
	long bid;
	int shape;
	int rotate;
	int x;
	int y;
	struct list_elem elem;
};

struct Block shadow_;
struct Block rec_block_;
struct Block* cur_block_;
struct Block* next_block_;

struct list b_list;

int score;			/* 점수가 저장*/
bool gameOver=false;			/* 게임이 종료되면 1로 setting된다.*/
int timed_out;
int recommendR,recommendY,recommendX; // 추천 블럭 배치 정보. 차례대로 회전, Y 좌표, X 좌표
RecNode *recRoot;

/***********************************************************
 *	테트리스의 모든  global 변수를 초기화 해준다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void InitTetris();

/***********************************************************
 *	테트리스의 모든  interface를 그려준다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void DrawOutline();

/***********************************************************
 *	테트리스와 관련된 키입력을 받는다.
 *	input	: none
 *	return	: (int) 입력받은 command
 *		  KEY_DOWN : 방향키 아래
 *		  KEY_UP   : 방향키 위
 *		  KEY_RIGHT: 방향키 오른쪽
 *		  KEY_LEFT : 방향키 왼쪽
 *		  ' '	   : Space bar
 *		  'q'/'Q'  : quit
 ***********************************************************/
int GetCommand();

/***********************************************************
 *	GetCommand로 입력받은 command에 대한 동작을 수행한다.
 *	input	: (int) GetCommand로 받은 command
 *	return	: (int) quit에 대한 입력을 받을 경우 0,
 * 		  그외의 경우 1을 return한다.
 ***********************************************************/
int ProcessCommand(int command);

/***********************************************************
 *	블럭이 일정 시간(1초)마다 내려가도록 호출되는 함수
 *	더이상 내릴수 없을 경우,
 *		블럭을 field에 합친다.
 *		완전이 채워진 line을 지운다.
 *		next block을 current block으로 바꿔주고
 *		block의 좌표를 초기화 한다.
 *		다음 블럭을 화면에 그리고 갱신된 score를 
 *		화면에 display한다.
 *	input	: (int) sig
 *	return	: none
 ***********************************************************/
void BlockDown(int sig);

/***********************************************************
 *	입력된 움직임이 가능한지를 판단해주는 함수.
 *	input	: (char[][]) 블럭의 움직임을 확인할 필드
 *		  (int) 현재 블럭의 모양 ID
 *		  (int) 블럭의 회전 횟수
 *		  (int) 블럭의 Y좌표
 *		  (int) 블럭의 X좌표
 *	return	: (int) 입력에 대한 블럭 움직임이 가능하면 1
 *		  가능하지 않으면 0을 return 한다.
 ***********************************************************/
bool CheckToMove(struct Block* check_block,char f[HEIGHT][WIDTH]);

/***********************************************************
 *	테트리스에서 command에 의해 바뀐 부분만 다시 그려준다.
 *	input	: (char[][]) command의해 바뀐 블럭을 확인할 필드
 *		  (int) 바뀌기 전 모양을 알기 위해 command를 입력으로 받는다.
 *		  (int) 현재 블럭의 모양 ID
 *		  (int) 블럭의 회전 횟수
 *		  (int) 블럭의 Y좌표
 *		  (int) 블럭의 X좌표
 *	return	: none
 ***********************************************************/
void DrawChange(struct Block* prev_block,struct Block* new_block);


/***********************************************************
 *	테트리스의 블럭이 쌓이는 field를 그려준다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void DrawField();

/***********************************************************
 *	떨어지는 블럭을 field에 더해준다.
 *	input	: (char[][]) 블럭을 쌓을 필드
 *		  (int) 현재 블럭의 모양 ID
 *		  (int) 블럭의 회전 횟수
 *		  (int) 블럭의 Y좌표
 *		  (int) 블럭의 X좌표
 *	return	: none
 ***********************************************************/
void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX);

/***********************************************************
 *	완전히 채워진 Line을 삭제하고 점수를 매겨준다.
 *	input	: (char[][]) 완전히 채워진 line을 확인할 필드
 *	return	: (int) 삭제된 라인의 갯수에 대한 점수
 ***********************************************************/


/***********************************************************
 *	테트리스의 화면 오른쪽상단에 다음 나올 블럭을 그려준다..
 *	input	: (int*) 블럭의 모양에 대한 ID 배열
 *	return	: none
 ***********************************************************/
void DrawNextBlock(int next_blk_shape);

/***********************************************************
 *	테트리스의 화면 오른쪽 하단에 Score를 출력한다.
 *	input	: (int) 출력할 점수
 *	return	: none
 ***********************************************************/
void PrintScore(int score);

/***********************************************************
 *	해당 좌표(y,x)에 원하는 크기(height,width)의 box를 그린다.
 *	input	: (int) 그리고자 하는 박스의 왼쪽 상단모서리의 y 좌표
 *		  (int) 왼쪽 상단 모서리의 x 좌표
 *		  (int) 박스의 높이
 *		  (int) 박스의 넓이
 *	return	: none
 ***********************************************************/
void DrawBox(int y,int x, int height, int width);

/***********************************************************
 *	해당 좌표(y,x)에 원하는 모양의 블록을 그린다.
 *	input	: (int) 그리고자 하는 박스의 왼쪽 상단모서리의 y 좌표
 *		  (int) 왼쪽 상단 모서리의 x 좌표
 *		  (int) 블록의 모양
 *		  (int) 블록의 회전 횟수
 *		  (char) 블록을 그릴 패턴 모양
 *	return	: none
 ***********************************************************/
void DrawBlock(struct Block* cur_blk,char tile);
void DeleteBlock(struct Block* blk);
/***********************************************************
 *	블록이 떨어질 위치를 미리 보여준다.
 *	input	: (int) 그림자를 보여줄 블록의 왼쪽 상단모서리의 y 좌표
 *		  (int) 왼쪽 상단 모서리의 x 좌표
 *		  (int) 블록의 모양
 *		  (int) 블록의 회전 횟수
 *	return	: none
 ***********************************************************/
void DrawShadow(struct Block blk);

/***********************************************************
 *	테트리스 게임을 시작한다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void play();

/***********************************************************
 *	메뉴를 보여준다.
 *	input	: none
 *	return	: 사용자가 입력한 메뉴 번호
 ***********************************************************/
char menu();




/***********************************************************
 *	화면에 랭킹 기록들을 보여준다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void rank();



/***********************************************************
 *	추천 블럭 배치를 구한다.
 *	input	: (RecNode*) 추천 트리의 루트
 *	return	: (int) 추천 블럭 배치를 따를 때 얻어지는 예상 스코어
 ***********************************************************/
int recommend(RecNode *root);

/***********************************************************
 *	추천 기능에 따라 블럭을 배치하여 진행하는 게임을 시작한다.
 *	input	: none
 *	return	: none
 ***********************************************************/
void recommendedPlay();

bool CheckGameOver(struct Block* block);
void Freeze(char f[HEIGHT][WIDTH],struct Block* blk);

int BreakLine(char f[HEIGHT][WIDTH]);
void InitBlock(struct Block** blk);

int CalCulateScore(int shape,int rotate,int x,char cur_field[HEIGHT][WIDTH]);
struct RecursiveRet RecursiveCalculateScore(struct list_elem* cur, char cur_field[HEIGHT][WIDTH]);
void RecommendNextBlock(void);
#endif
