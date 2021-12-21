#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define TEST_NUM_VERTICIES 5000
#pragma warning(disable : 4996)
#define RAND_VERT (1+rand()) % TEST_NUM_VERTICIES

//Structures etc.

typedef struct qblock {
	int val;
	struct qblock* next;
} qblock_t;

// queue = first -> ... -> last -> NULL
typedef struct queue {
	struct qblock* first;
	struct qblock* last;
} queue_t;

typedef struct edge {
	int first;
	int second;
} edge_t;

typedef struct Matrix {
	int size;
	int** matrix;
} matrix_t;

typedef struct graph {
	int num_vertex;
	matrix_t* adj_matrix;
} graph_t;


// Queue part
queue_t* QueueCreate(int val) {
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
	if (!queue) return NULL;
	queue->last = (qblock_t*)malloc(sizeof(qblock_t));
	if (!queue->last) return NULL;
	queue->last->val = val;
	queue->last->next = NULL;
	queue->first = queue->last;
	return queue;
};

void QueueDestroy(queue_t* queue) {
	qblock_t* iterblock = NULL;
	while (queue->first != queue->last) {
		iterblock = queue->last;
		queue->last = queue->last->next;
		free(iterblock);
	}
	free(queue->last);
};

queue_t* QueueAddEl(queue_t* queue, int val) {
	if (queue->last == NULL) {
		queue = QueueCreate(val);
		return queue;
	}
	queue->last->next = (qblock_t*)malloc(sizeof(qblock_t));
	if (queue->last->next == NULL) return queue;
	queue->last->val = val;
	queue->last = queue->last->next;
	queue->last->next = NULL;
	return queue;
};

int QueuePop(queue_t* queue) {
	int res = queue->first->val;
	queue->first = queue->first->next;
	if (queue->first == NULL) queue->last = NULL;
	return res;
};

int QueueIsEmpty(queue_t* queue) {
	if (queue == NULL) return -1;
	if (queue->first == queue->last) return 1;
	return 0;
}

//Matrix part
matrix_t* MatrixCreate(int size) {
	matrix_t* res = (matrix_t*)malloc(sizeof(matrix_t));
	if (res == NULL) return NULL;
	res->size = size;
	res->matrix = (int**)malloc(size * sizeof(int*) + size * size * sizeof(int));
	if (res->matrix == NULL) return NULL;

	char* arr_start = (char*)res->matrix + size * sizeof(int*);
	for (int i = 0; i < size; i++) {
		res->matrix[i] = (int*)(arr_start + i * size * sizeof(int));
	}
	return res;
}

void MatrixFill(matrix_t* matrix, int val) {
	if (matrix == NULL) return;
	for (int i = 0; i < matrix->size; i++) {
		for (int j = 0; j < matrix->size; j++) {
			matrix->matrix[i][j] = val;
		}
	}
}

void MatrixDelete(matrix_t* matrix) {
	free(matrix->matrix);
	free(matrix);
}

void MatrixPrint(matrix_t* matrix) {
	if (matrix->matrix == NULL) return;
	for (int i = 0; i < matrix->size; i++) {
		for (int j = 0; j < matrix->size; j++) {
			printf("%d ", matrix->matrix[i][j]);
		}
		printf("\n");
	}
}

//Graph part
graph_t GraphCreate() {
	int check = 0;
	int vertA, vertB;

	graph_t graph;

	check = fscanf(stdin, "%d", &graph.num_vertex);
	if (check != 1) abort();

	graph.adj_matrix = MatrixCreate(graph.num_vertex);
	MatrixFill(graph.adj_matrix, 0);

	while (1) {
		check = fscanf(stdin, "%d %d", &vertA, &vertB);
		if (check == EOF || feof(stdin) != 0) break;
		graph.adj_matrix->matrix[vertA][vertB] = graph.adj_matrix->matrix[vertB][vertA] = 1;
	}
	return graph;
};

void GraphDelete(graph_t graph) {
	MatrixDelete(graph.adj_matrix);
}

void GraphBFS(graph_t graph) {
	if (graph.num_vertex == 0 || graph.adj_matrix->matrix == NULL) return;
	int* is_visited = (int*)calloc(graph.num_vertex, sizeof(int));
	if (is_visited == NULL) {
		MatrixDelete(graph.adj_matrix);
		abort();
	}
	queue_t* vert_queue = QueueCreate(graph.num_vertex);

	is_visited[0] = 1;
	int num_visited = 1;
	QueueAddEl(vert_queue, 0);

	while (!QueueIsEmpty(vert_queue)) {
		//Main path of search
		int vert = QueuePop(vert_queue);
		for (int j = 1; j < graph.num_vertex; j++) {
			if ((graph.adj_matrix->matrix[vert][j] == 1) && (is_visited[j] == 0)) {
				is_visited[j] = 1;
				QueueAddEl(vert_queue, j);
			}
		}
		//To avoid incomplete search when graph isn't connected
		if (QueueIsEmpty(vert_queue)) {
			for (int j = 0; j < graph.num_vertex; j++) {
				if (is_visited[j] == 0) {
					QueueAddEl(vert_queue, j);
					break;
				};
			}
		}
	}
	for (int i = 0; i < graph.num_vertex; i++) {
		if (!is_visited[i]) {
			printf("Search is incomplete!");
			break;
		}
	}
	free(is_visited);
	QueueDestroy(vert_queue);
};



void StressTest() {
	LARGE_INTEGER start, end, counter;

	FILE* test_file = fopen("TestGraph.txt", "w+");
	if (test_file == NULL) return;

	graph_t test_graph;
	test_graph.num_vertex = TEST_NUM_VERTICIES;
	test_graph.adj_matrix = MatrixCreate(TEST_NUM_VERTICIES);
	MatrixFill(test_graph.adj_matrix, 0);

	srand(time(NULL));

	fprintf(test_file, "%d", TEST_NUM_VERTICIES + 1);

	int rand_vert = 0;
	for (int i = 0, vertex = 0; i < TEST_NUM_VERTICIES - 1; i++)
	{
		if ((vertex = RAND_VERT) != (i + 1))
		{
			fprintf(test_file, "%d %d", i + 1, vertex);
			test_graph.adj_matrix->matrix[i + 1][vertex] = 1;
			test_graph.adj_matrix->matrix[vertex][i + 1] = 1;
			--i;
		}
		else
		{
			fprintf(test_file, "%d %d", vertex, (rand_vert = RAND_VERT));
			test_graph.adj_matrix->matrix[vertex][rand_vert] = 1;
			test_graph.adj_matrix->matrix[rand_vert][vertex] = 1;
		}
	}
	fprintf(test_file, "\n");

	fseek(test_file, SEEK_SET, 0);

	QueryPerformanceFrequency(&counter);
	QueryPerformanceCounter(&start);

	GraphBFS(test_graph);

	QueryPerformanceCounter(&end);
	printf("%lf s", (double)((double)(end.QuadPart - start.QuadPart) / (double)counter.QuadPart));

	fclose(test_file);
}


int main()
{

	//graph_t graph = GraphCreate();
	//GraphBFS(graph);
	//GraphDelete(graph);

	StressTest();
	return 0;
}
