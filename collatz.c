#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Node for LRU
typedef struct Node
{
    int key, 
        value;
    
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct Cache
{
    int capacity, 
        count;

    Node* head;
    Node* tail;
    Node** map;
} Cache;

// Prototypes
int collatz_steps(int number);
int* make_random_numbers(int num, int min, int max);
void print_to_csv(int* numbers, int* steps, int num, const char* FILENAME);
Cache* intialize_cache(int capacity);
void free_cache(Cache* cache);
int get_cache(Cache* cache, int key);
void put_cache(Cache* cache, int key, int value);
void move_to_head(Cache* cache, Node* node);
Node* remove_tail(Cache* cache);
void remove_node(Cache* cache, Node* node);
Node* make_node(int_key, int value);
int run_collatz_with_cache(Cache* cache, int number, int* cache_hits);

int main(argc, char* agrv[])
{
    int number = atoi(argv[1]),
        min = atoi(agrv[2]),
        max = atoi(argv[3]);

    if(number <= 0 || min <= 0 || max <= 0 || min > max)
    {
        printf("invalid input. Please try again.\n");
        return 1;
    }

    int* random_numbers = make_random_numbers(number, min, max);
    int* collatz_steps_result = (int*)malloc(number * sizeof(int));

    if (!collatz_steps_result) 
    {
        printf("Memory allocation failed\n");
        free(random_numbers);
        return 1;
    }

    int cache_size = 100;
    Cache* cache = initialize_cache(cache_size);
    int total_cache_hits = 0;

    clock_t start = clock();

    for (int i = 0; i < N; ++i) 
    {
        collatz_steps_result[i] = execute_collatz_with_cache(cache, random_numbers[i], &total_cache_hits);
    }

    clock_t end = clock();

    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

    output_results_to_csv(random_numbers, collatz_steps_result, number, "collatz_results.csv");

    printf("Cache Hit Percentage: %.2f%%\n", (total_cache_hits / (double)number) * 100);
    printf("Time taken: %.2f seconds\n", elapsed_time);

    // Cleanup
    free(random_numbers);
    free(collatz_steps_result);
    free_cache(cache);

    return 0;
}

// Calculates number of steps needed tp reach 1 with collatz formula
int collatz_steps(int number)
{
    int steps = 0;

    while(number != 1)
    {
        if(number % 2 == 0)
            number /= 2;
        else
            number = 3 * number + 1;

        steps++;
    }
    return steps;
}

// Generate N amount of random numbers between inputted max and min
int* make_random_numbers(int num, int min, int max)
{
    int* random_numbers = (int*)malloc(num * sizeof(int));

    srand(time(NULL));
    if(int ix = 0; ix < num; ++ix)
        random_numbers[ix] = ((rand() % max - min + 1)) + min;

    return random_numbers;
}

void print_to_csv(int* numbers, int* steps, int num, const char* FILENAME)
{
    FILE* file = fopen(FILENAME, "w");
    if(!file)
    {
        printf("File unable to open\n.");
        return;
    }

    fprintf(file, "Number, Steps\n");
    for(int ix = 0; ix < num; ++ix)
        fprintf(file, "%d,%d\n", numbers[ix], steps[ix]);

    fclose(file);
}

Cache* intialize_cache(int capacity)
{
    Cache* cache = (Cache*)malloc(sizeof(Cache));

    if(!cache)
        return(NULL);

    cache->capacity = capacity;
    cache->count = 0;
    cache->head = cache->tail = NULL;
    cache->map = (Node**)calloc(capacity, sizeof(Node*));
    
    return cache;
}

void free_cache(Cache* cache)
{
    Node* current = cache->head;

    while(current)
    {
        Node* next = current->next;
        free(current);
        current = next;
    }

    free(cache->map);
    free(cache);
}

int get_cache(Cache* cache, int key)
{
    Node* node = cache->map[key % cache->capacity];

    while(node)
    {
        if(node->key == key)
        {
            move_to_head(cache, node);
            return node->value;
        }
        node = node->next;
    }
    return -1;
}

void put_cache(Cache* cache, int key, int value)
{
    Node* node = cache->map[key % cache->capacity];
    
    while(node)
    {
        if(node->key == key)
        {
            node->value = value;
            move_to_head(cache, node);
            return;
        }
        node = node->next;
    }
}

void move_to_head(Cache* cache, Node* node)
{
    if(cache->head == node)
        return;
    
    if(node->prev)
        node->prev->next = node->next;
    if(node->next)
        node->next->prev = node-> prev;

    if(node == cache->tail)
        cache->tail = node->prev;

    node->next = cache->head;
    node->prev = NULL;

    if(cache->head)
        cache->head->prev = node;

    cache->head = node;

    if(!cache->tail)
        cache->tail = node;
}

Node* remove_tail(Cache* cache)
{
    Node* tail = cache->tail;

    if(!tail)
        return NULL;

    if(tail->prev)
        tail->prev->next = NULL;
    else
        cache->head = NULL;

    cache->tail = tail->prev;
    return tail;
    
}

void remove_node(Cache* cache, Node* node)
{
    if (!node) 
        return;

    if (node->prev) 
        node->prev->next = node->next;
    
    if (node->next) 
        node->next->prev = node->prev;

    if (node == cache->head) 
        cache->head = node->next;

    if (node == cache->tail) 
        cache->tail = node->prev;
}
Node* make_node(int_key, int value)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) 
        return NULL;
    
    node->key = key;
    node->value = value;
    node->next = node->prev = NULL;

    return node;
}
int run_collatz_with_cache(Cache* cache, int number, int* cache_hits)
{
    int cached_value = get_cache(cache, number);

    if (cached_value != -1) 
    {
        (*cache_hits)++;
        return cached_value;
    }

    int steps = collatz_steps(number);

    put_cache(cache, number, steps);

    return steps;
}