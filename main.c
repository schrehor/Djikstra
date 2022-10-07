#include<stdio.h>
#include<stdlib.h>


typedef struct vertex {
	int x, y;
	char type;
	int value;
	int index;
	int distance;
	int inHeap;
	int visited;
	int done;
	struct vertex* prev;
	struct vertex* top;
	struct vertex* bottom;
	struct vertex* left;
	struct vertex* right;
}VERTEX;

typedef struct heap {
	VERTEX** vertex;
	int index;
}HEAP;

/*
*
*   MAPA
*
*/

inline int setValue(VERTEX* map) {
	switch (map->type)
	{
	case 'C':
	case 'P':
	case 'D':
		return 1;
		break;
	case 'H':
		return 2;
		break;
	case 'N':
		return -1;
	}
}

inline void testMap(VERTEX* map, int n, int m) {
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			printf("%c ", map[i * n + j].type);
		}
		printf("\n");
	}
}

inline VERTEX* createMap(char** mapa, int n, int m, int* numPrincess) {
	VERTEX* map = (VERTEX*)malloc(sizeof(VERTEX) * m * n);
	int a = 0;

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			map[i * n + j].type = mapa[i][j];
			map[i * n + j].x = j;
			map[i * n + j].y = i;
			map[i * n + j].value = setValue(&map[i * n + j]);
			map[i * n + j].distance = 0;
			map[i * n + j].prev = NULL;
			map[i * n + j].inHeap = 0;
			map[i * n + j].visited = 0;
			map[i * n + j].done = 0;
			map[i * n + j].index = i * n + j;


			if (map[i * n + j].type == 'P')
			{
				(*numPrincess)++;
			}
		}
	}

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			if (map[i * n + j].type == 'N')
			{
				map[i * n + j].top = NULL;
				map[i * n + j].bottom = NULL;
				map[i * n + j].left = NULL;
				map[i * n + j].right = NULL;
				continue;
			}

			if (i == 0)
			{
				map[i * n + j].top = NULL;
			}
			else
			{
				if (map[i * n + j - n].type == 'N')
				{
					map[i * n + j].top = NULL;
				}
				else
				{
					map[i * n + j].top = &map[i * n + j - n];
				}
			}

			if (i == n - 1)
			{
				map[i * n + j].bottom = NULL;
			}
			else
			{
				if (map[i * n + j + n].type == 'N')
				{
					map[i * n + j].bottom = NULL;
				}
				else
				{
					map[i * n + j].bottom = &map[i * n + j + n];
				}
			}

			if (j == 0)
			{
				map[i * n + j].left = NULL;
			}
			else
			{
				if (map[i * n + j - 1].type == 'N')
				{
					map[i * n + j].left = NULL;
				}
				else
				{
					map[i * n + j].left = &map[i * n + j - 1];
				}
			}

			if (j == m - 1)
			{
				map[i * n + j].right = NULL;
			}
			else
			{
				if (map[i * n + j + 1].type == 'N')
				{
					map[i * n + j].right = NULL;
				}
				else
				{
					map[i * n + j].right = &map[i * n + j + 1];
				}

			}
		}
	}

	return map;
}

inline VERTEX* resetMap(VERTEX* map, int n, int m) {
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			map[i * n + j].distance = 0;
			map[i * n + j].prev = NULL;
			map[i * n + j].inHeap = 0;
			map[i * n + j].visited = 0;
		}
	}

	return map;
}

/*
*
*   HEAP
*
*/


inline HEAP createHeap(int size) {
	HEAP heap;
	heap.vertex = (VERTEX*)malloc(size * sizeof(VERTEX));
	heap.index = 0;

	return heap;
}

inline void repairInsert(HEAP* heap, int index) {
	int parent = (index / 2 < 1) ? 1 : index / 2;

	if (heap->vertex[parent]->distance > heap->vertex[index]->distance)
	{
		VERTEX* hold = heap->vertex[parent];
		heap->vertex[parent] = heap->vertex[index];
		heap->vertex[index] = hold;
		repairInsert(heap, parent);
	}
}

inline void insertHeap(HEAP* heap, VERTEX* vertex) {
	heap->index++;
	vertex->inHeap = 1;
	heap->vertex[heap->index] = vertex;
	repairInsert(heap, heap->index);
}

inline void testHeap(HEAP heap) {
	for (size_t i = 1; i <= heap.index; i++)
	{
		printf("%d ", heap.vertex[i]->distance);
	}
	putchar('\n');
}

inline int comapareVertex(VERTEX* left, VERTEX* right, int leftInt, int rightInt) {
	if (left->distance < right->distance)
	{
		return leftInt;
	}
	else
	{
		return rightInt;
	}

}

inline void repairDelete(HEAP* heap, int index) {
	int lChild = 2 * index;
	int rChild = 2 * index + 1;
	VERTEX* hold;

	if (heap->index == lChild)
	{
		if (heap->vertex[index]->distance > heap->vertex[lChild]->distance)
		{
			hold = heap->vertex[index];
			heap->vertex[index] = heap->vertex[lChild];
			heap->vertex[lChild] = hold;
		}
		else
		{
			return;
		}
	}
	else if (heap->index == rChild)
	{
		if (heap->vertex[index]->distance > heap->vertex[rChild]->distance)
		{
			hold = heap->vertex[index];
			heap->vertex[index] = heap->vertex[rChild];
			heap->vertex[rChild] = hold;
		}
		else
		{
			return;
		}
	}
	else if (heap->index > lChild && heap->index > rChild)
	{
		int minIndex = comapareVertex(heap->vertex[lChild], heap->vertex[rChild], lChild, rChild);
		if (heap->vertex[index]->distance > heap->vertex[minIndex]->distance)
		{
			hold = heap->vertex[index];
			heap->vertex[index] = heap->vertex[minIndex];
			heap->vertex[minIndex] = hold;
			repairDelete(heap, minIndex);
		}
		else
		{
			return;
		}
	}
}

inline VERTEX* deleteHeap(HEAP* heap) {
	if (heap->index == 0)
	{
		printf("Prazdna halda.");
		exit(0);
	}

	VERTEX* min = heap->vertex[1];
	heap->vertex[1] = heap->vertex[heap->index--];
	min->inHeap = 0;
	repairDelete(heap, 1);

	return min;
}

/*
*
*   Dijkstra
*
*/

inline VERTEX* arraySort(int arr[], VERTEX current) {
	if (arr[0] != -1 && arr[0] <= arr[1] && arr[0] <= arr[2] && arr[0] <= arr[3])
	{
		return current.top;
	}
	if (arr[1] != -1 && arr[1] <= arr[0] && arr[1] <= arr[2] && arr[1] <= arr[3])
	{
		return current.bottom;
	}
	if (arr[2] != -1 && arr[2] <= arr[1] && arr[2] <= arr[0] && arr[2] <= arr[3])
	{
		return current.left;
	}
	if (arr[3] != -1 && arr[3] <= arr[1] && arr[3] <= arr[2] && arr[3] <= arr[0])
	{
		return current.right;
	}
}

inline VERTEX* findPrev(VERTEX* current) {
	int bestWay[4] = { 1000000,1000000,1000000,1000000 };
	if (current->top != NULL && current->top->visited == 1)
	{
		bestWay[0] = current->top->distance;
	}
	if (current->bottom != NULL && current->bottom->visited == 1)
	{
		bestWay[1] = current->bottom->distance;
	}
	if (current->left != NULL && current->left->visited == 1)
	{
		bestWay[2] = current->left->distance;
	}
	if (current->right != NULL && current->right->visited == 1)
	{
		bestWay[3] = current->right->distance;
	}

	return arraySort(bestWay, *current);
}

inline void relax(VERTEX* current, HEAP* heap) {
	VERTEX hold;


	if (current->distance != 0) {
		current->prev = findPrev(current);
	}

	if (current->top != NULL && current->top != current->prev)
	{
		if (current->distance + current->top->value < current->top->distance || current->top->distance == 0)
		{
			current->top->distance = current->distance + current->top->value;
		}
		if (current->top->inHeap == 0 && current->top->visited == 0)
		{
			insertHeap(heap, current->top);
		}
	}
	if (current->bottom != NULL && current->bottom != current->prev)
	{
		if (current->distance + current->bottom->value < current->bottom->distance || current->bottom->distance == 0)
		{
			current->bottom->distance = current->distance + current->bottom->value;
		}
		if (current->bottom->inHeap == 0 && current->bottom->visited == 0)
		{
			insertHeap(heap, current->bottom);
		}
	}
	if (current->left != NULL && current->left != current->prev)
	{
		if (current->distance + current->left->value < current->left->distance|| current->left->distance == 0)
		{
			current->left->distance = current->distance + current->left->value;
		}
		if (current->left->inHeap == 0 && current->left->visited == 0)
		{
			insertHeap(heap, current->left);
		}
	}
	if (current->right != NULL && current->right != current->prev)
	{
		if (current->distance + current->right->value < current->right->distance || current->right->distance == 0)
		{
			current->right->distance = current->distance + current->right->value;
		}
		if (current->right->inHeap == 0 && current->right->visited == 0)
		{
			insertHeap(heap, current->right);
		}
	}

}

inline int getNumberOfVertex(VERTEX finish) {
	VERTEX* current = &finish;
	int count = 1;

	while (current->prev != NULL)
	{
		count++;
		current = current->prev;
	}

	return count;
}

inline int* getPath(VERTEX* current, int* path) {
	int index = 0;

	while (current->prev != NULL)
	{
		path[index++] = current->x;
		path[index++] = current->y;
		current = current->prev;
	}
	path[index++] = current->x;
	path[index++] = current->y;

	return path;
}

inline void printPath(int path[], int size) {
	while (size != -1)
	{
		printf("%d %d\n", path[size--], path[size--]);
	}
	putchar('\n');
}

inline int* dijkstraDrake(VERTEX* start, int numberOfNodes, int* distance, int* sizePath, int* newStart, int time) {
	HEAP heap = createHeap(numberOfNodes);
	VERTEX* current = NULL;

	insertHeap(&heap, start);
	while (heap.index != 0) {
		current = deleteHeap(&heap);
		if (current->type == 'D')
		{
			current->prev = findPrev(current);
			break;
		}
		if (current->distance > time)
		{
			return NULL;
		}
		current->visited = 1;
		relax(current, &heap);
	}

	*distance = current->distance + 1;
	*sizePath = 2 * getNumberOfVertex(*current);
	*newStart = current->index;
	int* path = (int*)malloc(*sizePath * sizeof(int));
	return getPath(current, path);
}

inline int factorial(int n) {
	if (n == 0)
	{
		return 1;
	}
	return n * factorial(n - 1);
}

inline int** dijkstraPrincess(VERTEX* start, int numberOfNodes, int numberOfPrincess, int index, int*** sizePath, int*** distance, int** newStart) {
	HEAP heap = createHeap(numberOfNodes);
	VERTEX* current = NULL;
	VERTEX* princess = (VERTEX*)malloc(numberOfPrincess * sizeof(VERTEX));
	int foundPrincess = 0;
	int** allPaths = (int**)malloc(sizeof(int*));

	insertHeap(&heap, start);
	while (heap.index != 0) {
		current = deleteHeap(&heap);
		if (current->type == 'P' && start->index != current->index && current->done != 1)
		{
			current->prev = findPrev(current);
			newStart[0][foundPrincess] = current->index;
			distance[0][index][foundPrincess] = current->distance;
			sizePath[0][index][foundPrincess] = 2 * getNumberOfVertex(*current);
			princess[foundPrincess] = *current;
			foundPrincess++;
			if (foundPrincess == numberOfPrincess)
			{
				break;
			}

		}
		current->visited = 1;
		relax(current, &heap);
	}

	if (foundPrincess != numberOfPrincess)
	{
		exit(0);
	}

	for (size_t i = 0; i < numberOfPrincess; i++)
	{
		int* path = (int*)malloc(sizePath[0][index][i] * sizeof(int));
		allPaths[i] = getPath(&princess[i], path);
	}

	return allPaths;
}

inline int* distancePermutation(int** distance, int top) {
	int* result = (int*)calloc(2 * top, sizeof(int));
	int* compare = (int*)calloc(factorial(top), sizeof(int));
	int min, index = 0, index1 = 0;

	for (int i = 0; i < top; i++)
	{
		int j;
		for (j = 0; j < top - i * 1; j++)
		{
			if (j == 0)
			{
				min = distance[i][j];
				compare[index1++] = j;
				compare[index1++] = i;
			}
			else
			{
				if (min > distance[i][j])
				{
					min = distance[i][j];
					compare[index1++] = j;
					compare[index1++] = i;
				}
			}
		}
		result[index++] = compare[--index1];
		result[index++] = compare[--index1];
	}

	return result;
}

inline void printFinalPath(int* finalPath, int lengthFinal) {
	for (size_t i = 0; i < lengthFinal;)
	{
		printf("%d %d\n", finalPath[i++], finalPath[i++]);
	}
	putchar('\n');
	putchar('\n');
}

inline void flipPath(int lengthOther, int** otherPath) {
	int hold;
	int* arr = *otherPath;
	for (int i = 0; i < lengthOther; i += 2)
	{
		hold = arr[i];
		arr[i] = arr[i + 1];
		arr[i + 1] = hold;
	}
}

inline void extendPathTop(int** finalPath, int* otherPath, int* lengthFinal, int lengthOther) {
	int* arr = *finalPath;
	int move = 0;
	if (arr[*lengthFinal - 1] == otherPath[0] && arr[*lengthFinal - 2] == otherPath[1])
	{
		move += 2;
	}
	flipPath(lengthOther, &otherPath);
	for (int i = 0; i < lengthOther; i++)
	{
		arr[i + *lengthFinal] = otherPath[i + move];
	}
	*lengthFinal += lengthOther + 1;
}

inline void extendPathBot(int** finalPath, int* otherPath, int* lengthFinal, int lengthOther) {
	int* arr = *finalPath;
	if (arr[*lengthFinal - 2] == otherPath[lengthOther] && arr[*lengthFinal - 1] == otherPath[lengthOther - 1])
	{
		lengthOther -= 2;
	}
	for (int i = lengthOther; i >= 0; i--)
	{
		arr[lengthOther - i + *lengthFinal] = otherPath[i];
	}
	*lengthFinal += lengthOther + 1;
}

inline int* joinPath(int totalDistance, int* pathDrake, int*** pathPrincess, int* minPrincessPath, int princessCount, int drakeSize, int** princessSize) {

	int currentDistance = 0;
	int* path = (int*)malloc(totalDistance * sizeof(int));
	int index = 0, index1 = 1;
	int* arr = (int*)malloc(princessCount * sizeof(int));
	int next[2];

	next[0] = pathDrake[0];
	next[1] = pathDrake[1];

	for (size_t i = 0; i < princessCount; i++)
	{
		arr[i] = princessSize[minPrincessPath[index]][minPrincessPath[index1]] - 1;
		index += 2;
		index1 += 2;
	}
	index = 0;
	index1 = 1;

	extendPathBot(&path, pathDrake, &currentDistance, drakeSize - 1);

	for (size_t i = 0; i < princessCount; i++)
	{
		if (next[0] == pathPrincess[minPrincessPath[index]][minPrincessPath[index1]][arr[i] - 1] && next[1] == pathPrincess[minPrincessPath[index]][minPrincessPath[index1]][arr[i]])
		{
			extendPathBot(&path, pathPrincess[minPrincessPath[index]][minPrincessPath[index1]], &currentDistance, arr[i]);
			next[0] = pathPrincess[minPrincessPath[index]][minPrincessPath[index1]][0];
			next[1] = pathPrincess[minPrincessPath[index]][minPrincessPath[index1]][1];
		}
		else if (next[0] == pathPrincess[minPrincessPath[index]][minPrincessPath[index1]][0] && next[1] == pathPrincess[minPrincessPath[index]][minPrincessPath[index1]][1])
		{
			extendPathTop(&path, pathPrincess[minPrincessPath[index]][minPrincessPath[index1]], &currentDistance, arr[i]);
			next[0] = pathPrincess[minPrincessPath[index]][minPrincessPath[index1]][arr[i]];
			next[1] = pathPrincess[minPrincessPath[index]][minPrincessPath[index1]][arr[i] - 1];
		}
		index += 2;
		index1 += 2;
	}

	return path;
}

inline int* zachran_princezne(char** mapa, int n, int m, int t, int* dlzka_cesty) {
	int numPrincess = 0, distanceDrake, pathDrakeSize, newStart, ** pathPrincessSize, ** distance, * princessStart, *** pathPrincess;
	int totalVertex = 0;
	VERTEX* map = createMap(mapa, n, m, &numPrincess);
	int* pathDrake = dijkstraDrake(&map[0], n * m, &distanceDrake, &pathDrakeSize, &newStart, t);
	if (pathDrake == NULL)
	{
		return NULL;
	}

	map = resetMap(map, n, m);

	pathPrincessSize = (int**)malloc(numPrincess * sizeof(int*));
	distance = (int**)malloc(numPrincess * sizeof(int*));
	pathPrincess = (int***)malloc(numPrincess * sizeof(int**));
	princessStart = (int*)malloc(numPrincess * sizeof(int));

	for (size_t i = 0; i < numPrincess; i++)
	{
		pathPrincessSize[i] = (int*)malloc((numPrincess - i * 1) * sizeof(int));
		distance[i] = (int*)malloc((numPrincess - i * 1) * sizeof(int));
		pathPrincess[i] = dijkstraPrincess(&map[newStart], n * m, (numPrincess - i * 1), i, &pathPrincessSize, &distance, &princessStart);
		newStart = princessStart[i];
		map = resetMap(map, n, m);
	}

	int* minPath = distancePermutation(distance, numPrincess);
	totalVertex = pathDrakeSize;
	for (size_t i = 0; i < numPrincess; i++)
	{
		int index = 1;
		totalVertex += pathPrincessSize[i][minPath[index]];
		index += 2;
	}
	totalVertex -= 2 * numPrincess;
	int* path = joinPath(totalVertex, pathDrake, pathPrincess, minPath, numPrincess, pathDrakeSize, pathPrincessSize);

	*dlzka_cesty = totalVertex / 2;
	flipPath(totalVertex, &path);

	//printFinalPath(path, totalVertex);
	return path;
}

//int main() {
//	int n = 10, m = 10, t = 12, *cesta, dlzka_cesty;
//	char** mapa = (char**)malloc(n * sizeof(char*));
//    mapa[0] = strdup("CCHCNHCCHN");
//    mapa[1] = strdup("NNCCCHHCCC");
//    mapa[2] = strdup("DNCCNNHHHC");
//    mapa[3] = strdup("CHHHCCCCCC");
//    mapa[4] = strdup("CCCCCNHHHH");
//    mapa[5] = strdup("PCHCCCNNNN");
//    mapa[6] = strdup("NNNNNHCCCC");
//    mapa[7] = strdup("CCCCCPCCCC");
//    mapa[8] = strdup("CCCNNHHHHH");
//    mapa[9] = strdup("HHHPCCCCCC");
//    cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
//}