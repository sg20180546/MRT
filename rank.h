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

struct RankElem{
    char name[MAXLINE];
    int point;
    struct list_elem elem;
};

void InitRank();

void RankXtoY(int X,int Y);

void RankByName(char* name);

void DeleteIdx(int idx);

FILE * fp;

struct list rank_list;
int rank_list_size;

#endif