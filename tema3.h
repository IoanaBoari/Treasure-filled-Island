/*BOARI Ioana-Ruxandra - 312CD*/
#ifndef _TEMA_3_H_
#define _TEMA_3_H_
#include <stdlib.h>
#include <stdbool.h>
#define MAX 1000
#define INF 999999

typedef struct harta {
	int key[MAX];
	int adancime[MAX];
	char** obiective; 
} Harta;

typedef struct ListNode {
	int key;
	struct ListNode *next;
}ListNode;

typedef struct List {
	ListNode *head;
}List;

typedef struct Stack {
	ListNode *head;
	long size;
} Stack;

typedef struct Queue {
	ListNode *front;
	ListNode *rear;
	long size;
} Queue;

typedef struct TNode {
	int v; // Destination node index
	int cost;
	struct TNode *next; // Link to the next node in the list
}TNode, *ATNode;

typedef struct {
	int nn; // Number of nodes in the graph
	ATNode *adl; // Adjancey list (TNode**)
}TGraphL;

void eliberareHarta(Harta* harta, int numObiective) {
    if (harta == NULL) {
        return;
    }
    if (harta->obiective != NULL) {
        for (int i = 0; i < numObiective; i++) {
            free(harta->obiective[i]);
        }
        free(harta->obiective);
    }
    free(harta);
}


ListNode *createNode(int key) {
	ListNode *node = malloc(sizeof(struct ListNode));
	node->key = key;
	node->next = NULL;
	return node;
}

List* createList(void){
	List* newList = (List*) malloc(sizeof(List));
	newList->head = (ListNode*) malloc(sizeof(ListNode));
	newList->head->next = newList->head;
	return newList;
}
void destroyList(List* list){
	ListNode* it = list->head->next;
	while(it != list->head){
		ListNode* aux = it;
		it = it->next;
		free(aux);
	}
	free(list->head);
	free(list);
}
int isListEmpty(List *list){
	if (list->head == NULL) {
		return 1;
	} else {
		return 0;
	}
}

Stack* createStack(void){
	Stack *s = malloc(sizeof(struct Stack));
	s->head = NULL;
	s->size = 0;
	return s;
}

int isStackEmpty(Stack* stack){
	if(stack->head == NULL) {
		return 1;
	} else {
		return 0;
	}
}

void push(Stack* stack, int key){
	ListNode *top;
	if(isStackEmpty(stack)) {
		stack->head = createNode(key);
		stack->size++;
		return;
	}
	top = createNode(key);
	top->next = stack->head;
	stack->head = top;
	stack->size++;
	return;
}

int top(Stack *stack){	
	if(isStackEmpty(stack)) {
		exit(1);
	}
	return stack->head->key;
} 

void pop(Stack *stack){
	ListNode *tmp;
	if(isStackEmpty(stack)) {
		return;
	}
	tmp = stack->head;
	stack->head = stack->head->next;
	stack->size--;
	free(tmp);
	return;
}

void destroyStack(Stack *stack){
	while(!isStackEmpty(stack)) {
		pop(stack);
	}
	stack->size = 0;
	free(stack);
	return;

}

Queue* createQueue(void){
	Queue *q = malloc(sizeof(struct Queue));
	q->front = NULL;
	q->rear = NULL;
	q->size = 0;
	return q;
}

int isQueueEmpty(Queue *q){
	if(q->front == NULL && q->rear == NULL) {
		return 1;
	} else {
		return 0;
	}
}

void enqueue(Queue *q, int key){
	ListNode *top;

	if(isQueueEmpty(q)) {
		q->front = q->rear = createNode(key);
		q->size++;
		return;
	}
	top = createNode(key);
	q->rear->next = top;
	q->rear = top;
	q->size++;
	return;
}

int front(Queue* q){
	if(isQueueEmpty(q)) {
		exit(1);
	}
	return q->front->key;
}

void dequeue(Queue* q){
	ListNode *temp;
	if(isQueueEmpty(q)) {
		return;
	}
	if(q->front == q->rear) {
		temp = q->front;
		q->front = q->rear = NULL;
		q->size = 0;
		free(temp);
		return;
	}
	temp = q->front;
	q->front = q->front->next;
	q->size--;
	free(temp);
	return;

}

void destroyQueue(Queue *q){
	while(!isQueueEmpty(q)) {
		dequeue(q);
	}
	q->size = 0;
	free(q);

	return;
}

TGraphL* createGraphAdjList(int numberOfNodes) {
	TGraphL* g = malloc(sizeof(TGraphL));
	g->nn = numberOfNodes;
	g->adl = malloc(g->nn * sizeof(ATNode));
	for (int i = 0; i < g->nn; i++) {
		g->adl[i] = NULL;
	}
	return g;
}

void addEdgeList(TGraphL* graph, int v1, int v2, int cost) {
	ATNode node1 = malloc(sizeof(TNode));
	node1->v = v1;
	node1->cost = cost;
	node1->next = graph->adl[v2];
	graph->adl[v2] = node1;
	ATNode node2 = malloc(sizeof(TNode));
	node2->v = v2;
	node2->cost = cost;
	node2->next = graph->adl[v1];
	graph->adl[v1] = node2;
	return;
}

void addEdgeListOrientated(TGraphL* graph, int v1, int v2, int cost) {
	ATNode node1 = malloc(sizeof(TNode));
	node1->v = v1;
	node1->cost = cost;
	node1->next = graph->adl[v2];
	graph->adl[v2] = node1;
	ATNode node2 = malloc(sizeof(TNode));
	node2->v = v2;
	node2->cost = cost;
	return;
}

int checkEdge(TGraphL* graph, int v1, int v2) {
	ATNode tmp = graph->adl[v1];
	while (tmp != NULL) {
		if (tmp->v == v2) {
			return 1;
		}
		tmp = tmp->next;
	}
	return 0;
}

void dfsRecHelper(TGraphL* graph, int* visited, Stack* path, int s) {
	visited[s] = 1;
	push(path, s);
	ATNode node = graph->adl[s];
	while(node != NULL) {
		if(!visited[node->v]) {
			dfsRecHelper(graph, visited, path, node->v);
		}
		node = node->next;
	}
	return;
}

Stack* dfsRecursive(TGraphL* graph, int s) {
	Stack *path = createStack();
	int *visited = calloc(graph->nn, sizeof(int));
	dfsRecHelper(graph, visited, path, s);
	free(visited);
	return path;
}

void dijkstra(TGraphL* G, int s, int *visited) {
	visited[s] = 1;
	int *distances = malloc (G->nn * sizeof(int));
	for (int i = 0; i < G->nn; i++) {
			distances[i] = INF;
	}
	distances[s] = 0;
	for (int i = 0; i < G->nn - 1; i++) {
		for (int j = 0; j < G->nn; j++) {
			ATNode tmp = G->adl[j];
			while (tmp != NULL) {
				if (distances[tmp->v] > distances[j] + tmp->cost) {
					distances[tmp->v] = distances[j] + tmp->cost;
					visited[tmp->v] = 1;
				}
			tmp = tmp->next;
			}
		}
	}
	for (int i = 0; i < G->nn; i++) {
		printf("%d: %d\n", i, distances[i]);
		
	}
	return;

}

int Prim(TGraphL G, int start, int *parent) {
	int *distances = malloc (G.nn * sizeof(int));
	int *visited = malloc (G.nn * sizeof(int));
	//int *parent = malloc (G.nn * sizeof(int));
	for (int i = 0; i < G.nn; i++) {
		distances[i] = INF;
		visited[i] = 0;
		parent[i] = -1;
	}
	distances[start] = 0;
	parent[start] = -1;
	int stop = 0;
	while(!stop) {
		stop = 0;
		int min_dist = INF;
		int idx_min = -1;
		int good_node = 0;
		for (int i = G.nn - 1; i >= start; i--) {
			if (!visited[i] && distances[i] < min_dist) {
				min_dist = distances[i];
				idx_min = i;
				good_node = 1;
			}
		}  // am cautat nodul cu distanta cea mai mica care nu a fost inca vizitat
		if (!good_node) {
			// conditia de oprire; verific ca nu mai exista noduri nevizitate
			stop = -1;
			break;
		}
		visited[idx_min] = 1;
		// pentru nodul gasit actualizez distantele fata de celelalte noduri
		ATNode tmp = G.adl[idx_min];
		while (tmp != NULL) {
			if (!visited[tmp->v] && tmp->cost < distances[tmp->v]) {
				parent[tmp->v] = idx_min;
				distances[tmp->v] = tmp->cost;
			}
			tmp = tmp->next;
		}
		free(tmp);
	}
	/*for (int i = 1; i < G.nn; i++) {
		printf("%d - %d\n", parent[i], i);
		
	}*/
	int cost_total = 0;
	//int nr = nrZone;
	for (int j = start; j < G.nn; j++) {
		if (*(int *)(parent + j) != -1) {
			cost_total += distances[j];
		}
	}
	
	free(distances);
	free(visited);
	//free(parent);
	return cost_total;
	
}

Stack* bfs(TGraphL* graph, int s){
	Stack *path = createStack();
	Queue *q = createQueue();
	int *visited = calloc(graph->nn, sizeof(int));
	int current;
	visited[s] = 1;
	enqueue(q, s);
	push(path, s);
	while (!isQueueEmpty(q)) {
		current = front(q);
		dequeue(q);
		ATNode node = graph->adl[current];
		while (node != NULL) {
			if (!visited[node->v]) {
				enqueue(q, node->v);
				push(path, node->v);
				visited[node->v] = 1;
			}
			node = node->next;
		}
	}
	free(visited);
	destroyQueue(q);
	return path;
}

int countConnectedComponents(TGraphL* graph, int numVertices) {
    int count = 0;
    int* visited = calloc(numVertices, sizeof(int));
    for (int i = 0; i < numVertices; i++) {
        if (!visited[i]) {
            Stack* path = bfs(graph, i);
            while (!isStackEmpty(path)) {
                int node = top(path);
                pop(path);
                visited[node] = 1;
            }
            destroyStack(path);
            count++;
        }
    }
    
    free(visited);
    return count;
}

void removeEdgeList(TGraphL* graph, int v1, int v2){
	TNode* it = graph->adl[v1];
	TNode* prev = NULL;
	while(it != NULL && it->v != v2){
		prev = it;
		it = it->next;
	}
	if(it == NULL)
		return;
	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v1] = it->next;
	free(it);
	it = graph->adl[v2];
	prev = NULL;
	while(it != NULL && it->v != v1){
		prev = it;
		it = it->next;
	}
	if(it == NULL) return;
	if(prev != NULL)
		prev->next = it->next;
	else
		graph->adl[v2] = it->next;
	free(it);
}

void removeNodeList(TGraphL* graph, int v){
	for(int i = 0; i < graph->nn;i++){
		removeEdgeList(graph,v,i);
	}
}

void destroyGraphAdjList(TGraphL* graph){
	for (int i = 0; i < graph->nn; i++) {
		ATNode current = graph->adl[i];
		while(current != NULL) {
			ATNode aux = current;
			current = current->next;
			free(aux);
		} 
	}
	free(graph->adl);
	free(graph);
}

#endif  //_TEMA_3_H_