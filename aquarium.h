#pragma once

enum Food{PLANKTONS, FISHES};
enum Priority{REPRODUCTION, EATING, ESCAPING, WALKING};

typedef struct plankton {
    int foodWeight;
    float speed;
    float lifeTime;
    int maxLifeTime;
    int duplicationTime;
    float lastDuplicationTime;
    float coordX;
    float coordY;
    float coordXWant;
    float coordYWant;
}PLANKTON;

typedef struct fish {
    struct plankton base;
    int viewRange;
    int viewAngle;
    float lastFoodTime;
    int foodTime;
    float boostSpeed;
    int reproductiveDuration;
    enum Food food;
    enum Priority priority[4];
    enum Priority doingNow;
}FISH;

typedef struct shark {
    struct fish brainBase;
}SHARK;

enum structs{p, f, s};

typedef union {
    PLANKTON plankton;
    FISH fish;
    SHARK shark;
}STRUCTS;

typedef struct Node {
    STRUCTS data;
    struct Node* next;
    struct Node* prev;
} NODE;

typedef struct linkedList {
    int size;
    enum structs type;
    NODE* head;
    NODE* last;
} LinkedList;


LinkedList planktonsM;
int planktonsMSize;
LinkedList fishM;
int fishMSize;
LinkedList sharksM;
int sharksMSize;

PLANKTON plankton;
FISH fish;
SHARK shark;

void add_to_end(LinkedList* list, STRUCTS value);
void remove_at_index(LinkedList* list, int index);
int find_index(LinkedList* list, PLANKTON base);

void defaultSettings();
void createArrays();
void deleteArrays();
void updateEntities(float time, float scaleE);