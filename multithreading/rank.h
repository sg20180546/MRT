#ifndef RANK_H_
#define RANK_H_
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <ncurses.h>

#define FILENAME "rank.txt"
#define MAXLINE 256
#define BUFSIZE 4096
#define NAMELEN 16
struct RankElem{
    char name[MAXLINE];
    int point;
    struct list_elem elem;
};

FILE * fp;

struct list rank_list;
int rank_list_size;

char buf[BUFSIZE];

void InitRank();

void PrintRankXtoY(int X,int Y);

void PrintRankByName(char * name);

void DeleteRankByIdx(int idx);

void SyncRankFile();

void NewRank(int score);


#endif