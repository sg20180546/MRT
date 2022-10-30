#ifndef RANK_H_
#define RANK_H_
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "list.h"
#include <stdlib.h>
#define FILENAME "rank.txt"
#define MAXLINE 1024
void InitRank();

void RankXtoY(int X,int Y);

void RankByName(char* name);

void DeleteIdx(int idx);

FILE * fp;

struct list rank_list;
int rank_list_size;

#endif