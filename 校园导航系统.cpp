#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXEX 20
#define INFINITY 32767
typedef struct Stack{
	int data[MAXEX];
	int top;
}SeqStack; 
SeqStack* InitStack()
{
	SeqStack *S;
	S = (SeqStack*)malloc(sizeof(SeqStack));
	if(!S)
	exit(1);
	S->top = -1;
	return S;
}
int Empty(SeqStack *S)
{
	if(S->top == -1)
	return 1;
	else
	return 0;
}
int Push(SeqStack *S, int data)
{
	if(S->top == MAXEX - 1)
	{
		printf("栈满\n");
		exit(0);
	}
	S->data[++S->top] = data;
	return 1;
}
int Pop(SeqStack *S)
{
	if(!Empty(S))
	{
		return S->data[S->top--];
	}
	return 0; 
}
int Top(SeqStack *S)
{
	if(!Empty(S))
	return S->data[S->top];
	return 0;
}
typedef struct ArcNode{
	int adjvex;
	int weight;
	struct ArcNode *next;
}ArcNode;
typedef struct VertexNode{
	char id;//地点编号
	char name[20];//地点名称 
	char des[100];//地点描述 
	ArcNode *head;
}VertexNode;
typedef struct {
	VertexNode vertex[MAXEX + 1];
	int vexnum;
	int arcnum;
}AdjList;
//创建图 
AdjList* Create()
{
	AdjList *G = NULL;
	G = (AdjList*)malloc(sizeof(AdjList));
	if(G == NULL)
	exit(1);
	G->arcnum = 0;
	G->vexnum = 0;
	FILE *fp = NULL;
	fp = fopen("map.txt","r");
	if(fp == NULL)
	exit(1);
	char id, name[20], des[100];
	int i;
	i = 0;
	while(fscanf(fp, "%c %s %s\n", &id, name, des) != EOF)//读取顶点信息 
	{
		i++;
		G->vertex[i].id = id;
		strcpy(G->vertex[i].name, name);
		strcpy(G->vertex[i].des, des);
		G->vertex[i].head = NULL;
		//printf("%c %s %s\n", id, name, des);
	}
	G->vexnum = i;
	fclose(fp);
	fp = NULL;
	fp = fopen("map2.txt","r");
	if(fp == NULL)
	exit(1);
	int v1, v2, weight;
	i = 0;
	ArcNode *p, *q;
	while(fscanf(fp, "%d %d %d\n", &v1, &v2, &weight) != EOF)
	{
		i++;
		if(i == 1)
		{
			G->vertex[v1].head = q = (ArcNode*)malloc(sizeof(ArcNode));
			q->adjvex = v2;
			q->weight = weight;
			q->next = NULL;
		}
		else
		{
			p = (ArcNode*)malloc(sizeof(ArcNode));
			q = G->vertex[v1].head;
			p->adjvex = v2;
			p->weight = weight;
			p->next = q;
			q = p;
			G->vertex[v1].head = q;
		}
	}
	G->arcnum = i;
	return G;
}
//打印图
void Print(AdjList *G)
{
	ArcNode *p;
	int i;
	for(i = 1; i <= G->vexnum; i++)
	{
		//printf("%c %s\n", G->vertex[i].id, G->vertex[i].name);
		p = G->vertex[i].head;
		while(p != NULL)
		{
			printf("%d %d %d\n", i, p->adjvex, p->weight);
			p = p->next;
		}
	}
}
//显示map
void ShowMap(AdjList *G)
{
	int i, j;
	for(i = 1; i <= G->vexnum; i++)
	{ 
		ArcNode *p;
		p = G->vertex[i].head;
		printf("%-10s", G->vertex[i].name);
		j = 1;
		if(p == NULL)
		printf("\n");
		else
		printf("--");
		while(p != NULL)
		{
			if(j++ == 1)
			printf("%d-->%s\n", p->weight, G->vertex[p->adjvex].name);
			else
			printf("%-10s--%d-->%s\n", " ", p->weight, G->vertex[p->adjvex].name);
			p = p->next;
		}
	}
}
//查询信息
void FindInformation(AdjList *G)
{
	printf("输入要查询的【地点编号】或【地点名称】(退出#):");
	char str[20];
	scanf("%s",str);
	if(strcmp(str, "#") == 0)
	return;
	int i;
	if(strlen(str) == 1)
	{
		for(i = 1; i <= G->vexnum; i++)
		{
			if(G->vertex[i].id == str[0])
			{
				printf("%c %s %s\n", G->vertex[i].id, G->vertex[i].name, G->vertex[i].des);
				return; 
			}
			
		}
	}
	else
	{
		for(i = 1; i <= G->vexnum; i++)
		{
			if(strcmp(str, G->vertex[i].name) == 0)
			{
				printf("%c %s %s\n", G->vertex[i].id, G->vertex[i].name, G->vertex[i].des);
				return; 
			}
		}		
	}
	printf("输入信息不存在\n"); 
}
//寻找索引 
int SelectIndex(AdjList *G, char str[])
{
	int i;
	if(strlen(str) == 1)
	{
		for(i = 1; i <= G->vexnum; i++)
		{
			if(G->vertex[i].id == str[0])
			{
				return i;
			}
			
		}
	}
	else
	{
		for(i = 1; i <= G->vexnum; i++)
		{
			if(strcmp(str, G->vertex[i].name) == 0)
			{
				return i;
			}
		}		
	}
	return 0;	
}
//任意两地点间权值
int SelectWeight(AdjList *G, int star, int end)
{
	ArcNode *p;
	p = G->vertex[star].head;
	while(p != NULL)
	{
		if(p->adjvex == end)
		{
			return p->weight;
		}
		p = p->next;
	}
	return INFINITY;
} 
//最短路径
void  Dijkstra(AdjList *G)
{
	 int dist[MAXEX + 1], path[MAXEX + 1][MAXEX + 1];
	 char star[20], end[20];
	 printf("输入【起点】-->【终点】:");
	 scanf("%s %s", star, end);
	 int starIndex = SelectIndex(G, star);
	 int endIndex = SelectIndex(G, end);
	 if(!starIndex || !endIndex)
	 {
	 	printf("输入地点不存在\n"); 
	 	return;
	 }
	 int i, j, k, min, t, flag;
	 for(i = 1; i <= G->vexnum; i++)
	 	for(j = 0; j <= G->vexnum; j++)
	 		path[i][j] = 0;
	 for(i = 1; i <= G->vexnum; i++)
	 {
	 	dist[i] = INFINITY;
	 	ArcNode *p;
	 	p = G->vertex[starIndex].head;
	 	while(p != NULL)
	 	{
	 		if(p->adjvex == i)
	 		{
	 			dist[i] = p->weight;
			}
			p = p->next;
		}
		if(dist[i] != INFINITY)
		path[i][1] = starIndex;
	 }
	 path[starIndex][0] = 1;
	 for(i = 2; i <= G->vexnum; i++)
	 {
	 	min = INFINITY;
	 	for(j = 1; j <= G->vexnum; j++)
	 	{
	 		if(path[j][0] == 0 && dist[j] < min)
	 		{
	 			k = j;
	 			min = dist[j];
			}
		}
		if(min == INFINITY)
		{
			break;
		}
		path[k][0] = 1;
		for(j = 1; j <= G->vexnum; j++)
		{
			if(!path[j][0] && SelectWeight(G, k, j) < INFINITY && SelectWeight(G, k, j) + dist[k] < dist[j])
			{
				dist[j] = SelectWeight(G, k, j) + dist[k];
				t = 1;
				while(path[k][t] != 0)
				{
					path[j][t] = path[k][t];
					t++;
				}
				path[j][t] = k;
				path[j][t +1 ] = 0;
			}
		}
	 }
	flag = 0;
	printf("最短路径长度【%d】-->路径【", dist[endIndex]);
	for(j = 1; path[endIndex][j] != 0; j++)
	{
	 	printf("%s-->", G->vertex[path[endIndex][j]].name);
	 	flag = 1;
	}
	if(flag == 0)
	{
		printf("无路径】\n"); 
	}
	else 
	printf("%s】\n", G->vertex[endIndex].name);
}
//寻找所有简单路径
void FindAllPath(AdjList *G)
{
	char star[20], end[20];
	printf("输入【起点】-->【终点】:");
	scanf("%s %s", star, end);
	int starIndex = SelectIndex(G, star);
	int endIndex = SelectIndex(G, end);
	if(!starIndex || !endIndex)
	{
	 	printf("输入地点不存在\n"); 
	 	return;
	}
	int i, w, v, instack[MAXEX + 1];
	v = starIndex;
	w = -1;
	for(i = 1; i <= G->vexnum; i++)
	instack[i] = 0;
	SeqStack *S = InitStack();
	Push(S, starIndex);
	instack[v] = 1;
	ArcNode *p;
	while(!Empty(S))
	{
		v = Top(S);
		p = G->vertex[v].head;
		while(p != NULL)
		{
			
			if(w != -1)
			{
				while(p != NULL && p->adjvex != w)
				p = p->next;
				if(p != NULL)
				{
					p = p->next;
					while(p != NULL && instack[p->adjvex] == 1)
					p = p->next;
				}	
			}
			else
			{
				while(p != NULL && instack[p->adjvex] == 1)
				p = p->next;
			}
			if(p == NULL)
			break;
			w = p->adjvex;
			Push(S, w);
			instack[w] = 1;
			if(w == endIndex)
			{
				printf("【");
				for(i = 0; i <= S->top; i++ )
				{
					printf("%s ", G->vertex[S->data[i]].name);
				}
				printf("\b】\n");
				break;
			}
			p = G->vertex[w].head;
			w = -1;
		}
		w = Pop(S);
		instack[w] = 0;
	}	
}
typedef struct SeqQueue{
	int data[MAXEX];
	int front;
	int rear;
}SeqQueue;
SeqQueue * InitQueue()
{
	SeqQueue *Q = (SeqQueue*)malloc(sizeof(SeqQueue));
	if(!Q)
	exit(1);
	Q->front = Q->rear = 0;
	return Q;
}
void InQueue(SeqQueue *Q, int data)
{
	if((Q->rear + 1) % MAXEX == Q->front)
	{
		printf("队满\n");
		exit(1);
	}
	Q->rear = (Q->rear + 1) % MAXEX;
	Q->data[Q->rear] = data;
}
int OutQueue(SeqQueue *Q)
{
	if(Q->front == Q->rear)
	{
		printf("队空\n");
		exit(0);
	}
	Q->front = (Q->front + 1) % MAXEX;
	return Q->data[Q->front];
}
int EmptyQueue(SeqQueue *Q)
{
	if(Q->front == Q->rear)
	return 1;
	return 0;
}
//找中转次数最少的路径
void FindMinPath(AdjList *G)
{
	char star[20], end[20];
	printf("输入【起点】-->【终点】:");
	scanf("%s %s", star, end);
	int starIndex = SelectIndex(G, star);
	int endIndex = SelectIndex(G, end);
	if(!starIndex || !endIndex)
	{
	 	printf("输入地点不存在\n"); 
	 	return;
	}
	int count[MAXEX + 1], path[MAXEX + 1][MAXEX + 1];
	int i, j, data, w;
	for(i = 1; i <= G->vexnum; i++)
	{
		count[i] = 0;
	}
	for(i = 1; i <= G->vexnum; i++)
		for(j = 0; j <= G->vexnum; j++)
		path[i][j] = 0;
	SeqQueue *Q = InitQueue();
	InQueue(Q, starIndex);
	count[starIndex] = 1;
	path[starIndex][1] = starIndex;
	ArcNode *p;
	while(!EmptyQueue(Q))
	{
		data = OutQueue(Q);
		p = G->vertex[data].head;
		while(p != NULL)
		{
			if(count[p->adjvex] == 0)
			{
				w = p->adjvex;
				count[w] = count[data] + 1;
				path[w][0] = path[data][0] + p->weight;
				i = 1;
				while(1)
				{
					if(path[data][i] != 0)
					{
						path[w][i] = path[data][i];
						i++;
					}
					else
					break;
				}
				path[w][i] = w;
				InQueue(Q, w);
			}
			else if(count[p->adjvex] != 0)
			{
				
				w = p->adjvex;
				if(count[data] + 1 == count[w] && path[w][0] > path[data][0] + p->weight)
				{
					printf("A\n");
					path[w][0] = path[data][0] + p->weight;
					i = 1;
					while(1)
					{
						if(path[data][i] != 0)
						{
							path[w][i] = path[data][i];
							i++;
						}
						else
						break;
					}
					path[w][i] = w;
				}
			}
			p = p->next;
		}
	}
	printf("中转次数【%d】 ",count[endIndex] - 1);
	i = 1;
	printf("路径【"); 
	while(i != 0)
	{
		if(path[endIndex][i] != 0)
		printf("%s ", G->vertex[path[endIndex][i++]].name);
		else
		i = 0;
	}
	printf("\b】\n");
}
typedef struct{
	int adjvex;
	int lowcost;
}Closedge;
//prim
void Prim(AdjList *G)
{
	Closedge closedge[MAXEX + 1];
//	char star[20], end[20];
//    printf("输入【起点】-->【终点】:");
//    scanf("%s %s", star, end);
    int starIndex = 1;//SelectIndex(G, star);
//    int endIndex = SelectIndex(G, end);
	int i, min, m, n;
	//初始化 
	for(i = 1; i <= G->vexnum; i++)
	{
		closedge[i].adjvex = starIndex;
		closedge[i].lowcost =  INFINITY;
	} 
	ArcNode *p = G->vertex[starIndex].head;
	while(p != NULL)
	{
		closedge[p->adjvex].lowcost = starIndex;
		p = p->next;
	}
	closedge[starIndex].lowcost = 0;
	
	for(n = 1; n < G->vexnum; n++)
	{
		min = INFINITY;
		for(i = 1; i <= G->vexnum; i++)
		{
			if(closedge[i].lowcost != 0 && closedge[i].lowcost < min)
			{
				m = i;
				min = closedge[i].lowcost;
			}
		}
		if(min == INFINITY)
		{
			printf("无最小生成树\n"); 
			break;
		}
		closedge[m].lowcost = 0;
		printf("%s--%s\n", G->vertex[m].name, G->vertex[closedge[m].adjvex].name);
		for(i = 1; i <= G->vexnum; i++)
		{
			if(closedge[i].lowcost != 0 && SelectWeight(G, i, m) < closedge[i].lowcost)
			{
				closedge[i].adjvex = m;
				closedge[i].lowcost = SelectWeight(G, i, m);
			}
		}
	}
	
}
//菜单 
void Menu(AdjList *G)
{
	printf("\n没错我是菜单：\n\n");
	printf("0.地点信息\n");
	printf("1.显示地图\n");
	printf("2.中转最少\n");
	printf("3.最佳路径\n");
	printf("4.全部路径\n");
	printf("5.最佳布网\n");
	int i;
	scanf("%d",&i);
	switch(i)
	{
		case 0:{
			FindInformation(G);
			break;
		}
		case 1:{
			ShowMap(G);
			break;
		}
		case 2:{
			FindMinPath(G);
			break;
		}
		case 3:{
			Dijkstra(G);
			break;
		}
		case 4:{
			FindAllPath(G);
			break;
		}
		case 5:{
			Prim(G);
			break;
		}
		default :{
			printf("输入错误\n"); 
			break;
		}
	}
}
int main()
{
	AdjList *G = Create();
	while(1)
	{
		Menu(G);
		printf("---------------------------------------\n");
	}
}
