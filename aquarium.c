#include "aquarium.h"
#include <stdlib.h>
#include "vectors.h"
#include <time.h>

void defaultSettings() {
    planktonsMSize = 8;
    planktonsM.type = p;
    fishMSize = 10;
    fishM.type = f;
    sharksMSize = 2;
    sharksM.type = s;

    // plankton
    plankton.coordXWant = -1;
    plankton.coordYWant = -1;
    plankton.lastDuplicationTime = 0;
    plankton.lifeTime = 0;

    plankton.speed = 0.05;
    plankton.foodWeight = 70;
    plankton.duplicationTime = 1000;
    plankton.maxLifeTime = 3000;

    //fish
    fish.base.coordXWant = -1;
    fish.base.coordYWant = -1;
    fish.base.lastDuplicationTime = 0;
    fish.base.lifeTime = 0;
    fish.lastFoodTime = 0;

    fish.base.duplicationTime = 1600;
    fish.food = PLANKTONS;
    fish.base.foodWeight = 60;
    fish.base.maxLifeTime = 4000;
    fish.base.speed = 0.1;
    fish.boostSpeed = 0.4;
    fish.doingNow = WALKING;
    fish.foodTime = 1000;
    fish.viewRange = 100;
    fish.viewAngle = 270;
    fish.reproductiveDuration = 1;
    fish.priority[0] = ESCAPING;
    fish.priority[1] = EATING;
    fish.priority[2] = REPRODUCTION;
    fish.priority[3] = WALKING;

    // shark
    shark.brainBase.base.coordXWant = -1;
    shark.brainBase.base.coordYWant = -1;
    shark.brainBase.base.lastDuplicationTime = 0;
    shark.brainBase.base.lifeTime = 0;
    shark.brainBase.lastFoodTime = 0;

    shark.brainBase.base.duplicationTime = 2500;
    shark.brainBase.food = FISHES;
    shark.brainBase.base.foodWeight = 0;
    shark.brainBase.base.maxLifeTime = 6000;
    shark.brainBase.base.speed = 0.2;
    shark.brainBase.boostSpeed = 5;
    shark.brainBase.doingNow = WALKING;
    shark.brainBase.foodTime = 2000;
    shark.brainBase.viewRange = 300;
    shark.brainBase.viewAngle = 360;
    shark.brainBase.reproductiveDuration = 2;
    shark.brainBase.priority[0] = EATING;
    shark.brainBase.priority[1] = REPRODUCTION;
    shark.brainBase.priority[2] = WALKING;
    shark.brainBase.priority[3] = ESCAPING;
}

void createArrays() {
    srand(time(NULL));
    NODE* node;
    for (int i = 0; i < planktonsMSize; ++i) {
        add_to_end(&planktonsM, (STRUCTS)plankton);
    }
    node = planktonsM.head;
    for (int i = 0; i < planktonsMSize; ++i) {
        PLANKTON* p = &node->data.plankton;
        p->coordX = rand() % 1281;
        p->coordY = rand() % 721;
        node = node->next;
    }
    for (int i = 0; i < fishMSize; ++i) {
        add_to_end(&fishM, (STRUCTS)fish);
    }
    node = fishM.head;
    for (int i = 0; i < fishMSize; ++i) {
        FISH* f = &node->data.fish;
        f->base.coordX = rand() % 1281;
        f->base.coordY = rand() % 721;
        node = node->next;
    }
    for (int i = 0; i < sharksMSize; ++i) {
        add_to_end(&sharksM, (STRUCTS)shark);
    }
    node = sharksM.head;
    for (int i = 0; i < sharksM.size; ++i) {
        SHARK* s = &node->data.shark;
        s->brainBase.base.coordX = rand() % 1281;
        s->brainBase.base.coordY = rand() % 721;
        node = node->next;
    }
}

void deleteArrays() {
    while (planktonsM.head != NULL) {
        remove_at_index(&planktonsM, 0);
    }
    while (fishM.head != NULL) {
        remove_at_index(&fishM, 0);
    }
    while (sharksM.head != NULL) {
        remove_at_index(&sharksM, 0);
    }
}

void moveEntity(PLANKTON* base, float x, float y, float time) {
    base->coordX += x * time;
    base->coordY += y * time;
}

void updateBase(PLANKTON* base, float time, float scaleE, enum structs type) {
    base->lifeTime += 0.1 * time * scaleE;
    if (base->lifeTime >= base->maxLifeTime) {
        switch (type) {
            case p:
                remove_at_index(&planktonsM, find_index(&planktonsM, *base));
                break;
            case f:
                remove_at_index(&fishM, find_index(&fishM, *base));
                break;
            case s:
                remove_at_index(&sharksM, find_index(&sharksM, *base));
                break;
        }
    }
    base->lastDuplicationTime += 0.1 * time * scaleE;
    if (type == p && base->lastDuplicationTime >= base->duplicationTime) {
        add_to_end(&planktonsM, (STRUCTS) plankton);
        planktonsM.last->data.plankton.coordX = base->coordX;
        planktonsM.last->data.plankton.coordY = base->coordY;
        base->lastDuplicationTime = 0;
    }
    if (base->coordXWant < 0 && base->coordYWant < 0) {
        base->coordXWant = rand() % 1281;
        base->coordYWant = rand() % 721;
    }
    float distance;
    vec vec1, vec2;
    vec1.x = base->coordX;
    vec1.y = base->coordY;
    vec2.x = base->coordXWant;
    vec2.y = base->coordYWant;
    distance = findSqrtDistance(vec1, vec2);
    if (distance <= base->speed * time * scaleE) {
        base->coordX = base->coordXWant;
        base->coordY = base->coordYWant;
        base->coordXWant = rand() % 1281;
        base->coordYWant = rand() % 721;
    } else {
        vec v = findVector(vec1, vec2);
        moveEntity(base, base->speed * v.x / distance, base->speed * v.y / distance, time * scaleE);
    }
}

int min(int a, int b) {
    return a > b ? b : a;
}

int max(int a, int b) {
    return a < b ? b : a;
}

void updateBrainBase(FISH* bBase, float time, float scaleE) {
    bBase->lastFoodTime += 0.1 * time * scaleE;
    // if (bBase->lastFoodTime >= 2.5f * bBase->foodTime) {
    //     switch (bBase->food) {
    //         case PLANKTONS:
    //             remove_at_index(&fishM, find_index(&fishM, bBase->base));
    //             break;
    //         case FISHES:
    //             remove_at_index(&sharksM, find_index(&sharksM, bBase->base));
    //             break;
    //     }
    // }
    float distance;
    vec vec1, vec2;
    vec1.x = bBase->base.coordX;
    vec1.y = bBase->base.coordY;
    int step = 0;
    NODE* node;
    pri:
    switch (bBase->priority[step]) {
        case ESCAPING:
            node = sharksM.head;
            for (int i = 0; i < sharksM.size; ++i) {
                SHARK s = node->data.shark;
                vec2.x = s.brainBase.base.coordX;
                vec2.y = s.brainBase.base.coordY;
                if (findDistance(vec1, vec2) <= bBase->viewRange * bBase->viewRange) {
                    float d = findSqrtDistance(vec2, vec1);
                    vec v = findVector(vec1, vec2);
                    bBase->base.coordXWant = max(min(bBase->base.coordX - v.x, 1280), 0);
                    bBase->base.coordYWant = max(min(bBase->base.coordY - v.y, 720), 0);   
                    updateBase(&bBase->base, time, scaleE, f);
                    return;
                }
                node = node->next;
            }
            break;
        case EATING:
            if (bBase->lastFoodTime >= bBase->foodTime) {
                switch(bBase->food) {
                    case PLANKTONS:
                        node = planktonsM.head;
                        for (int i = 0; i < planktonsM.size; ++i) {
                            PLANKTON p = node->data.plankton;
                            vec2.x = p.coordX;
                            vec2.y = p.coordY;
                            distance = findDistance(vec1, vec2);
                            if (findDistance(vec1, vec2) <= bBase->viewRange * bBase->viewRange) {
                                float d = findSqrtDistance(vec2, vec1);
                                vec v = findVector(vec1, vec2);
                                bBase->base.coordXWant = vec2.x;
                                bBase->base.coordYWant = vec2.y;
                                updateBase(&bBase->base, time, scaleE, f);
                                if ((int)bBase->base.coordX == (int)vec2.x && (int)bBase->base.coordY == (int)vec2.y) {
                                    remove_at_index(&planktonsM, find_index(&planktonsM, p));
                                    bBase->lastFoodTime = 0;
                                }
                                return;
                            }
                            node = node->next;
                        }
                        break;
                    case FISHES:
                        node = fishM.head;
                        for (int i = 0; i < fishM.size; ++i) {
                            FISH f = node->data.fish;
                            vec2.x = f.base.coordX;
                            vec2.y = f.base.coordY;
                            distance = findDistance(vec1, vec2);
                            if (findDistance(vec1, vec2) <= bBase->viewRange * bBase->viewRange) {
                                float d = findSqrtDistance(vec2, vec1);
                                vec v = findVector(vec1, vec2);
                                bBase->base.coordXWant = vec2.x;
                                bBase->base.coordYWant = vec2.y;
                                updateBase(&bBase->base, time, scaleE, s);                      
                                if ((int)bBase->base.coordX == (int)vec2.x && (int)bBase->base.coordY == (int)vec2.y) {
                                    remove_at_index(&fishM, find_index(&fishM, f.base));
                                    bBase->lastFoodTime = 0;
                                }
                                return;
                            }
                            node = node->next;
                        }
                        break;
                }
            }
            break;
        case REPRODUCTION:
            if (bBase->base.lastDuplicationTime >= bBase->base.duplicationTime) {
                switch(bBase->food) {
                    case PLANKTONS:
                        node = fishM.head;
                        for (int i = 0; i < fishM.size; ++i) {
                            FISH* fi = &node->data.fish;
                            if (!((int)fi->base.coordX == (int)bBase->base.coordX && (int)fi->base.coordY == (int)bBase->base.coordY && (int)fi->base.coordXWant == (int)bBase->base.coordXWant && (int)fi->base.coordYWant == (int)bBase->base.coordYWant)) {
                                vec2.x = fi->base.coordX;
                                vec2.y = fi->base.coordY;
                                distance = findDistance(vec1, vec2);
                                if (findDistance(vec1, vec2) <= bBase->viewRange * bBase->viewRange) {
                                    float d = findSqrtDistance(vec2, vec1);
                                    vec v = findVector(vec1, vec2);
                                    bBase->base.coordXWant = vec2.x;
                                    bBase->base.coordYWant = vec2.y;
                                    updateBase(&bBase->base, time, scaleE, f);
                                    if ((int)bBase->base.coordX == (int)vec2.x && (int)bBase->base.coordY == (int)vec2.y) {
                                        add_to_end(&fishM, (STRUCTS) fish);
                                        fishM.last->data.fish.base.coordX = fi->base.coordX;
                                        fishM.last->data.fish.base.coordY = fi->base.coordY;
                                        fi->base.lastDuplicationTime = 0;
                                        bBase->base.lastDuplicationTime = 0;
                                    }
                                    return;
                                }
                            }
                            node = node->next;
                        }
                    case FISHES:
                        node = sharksM.head;
                        for (int i = 0; i < sharksM.size; ++i) {
                            SHARK* sh = &node->data.shark;
                            if (!((int)sh->brainBase.base.coordX == (int)bBase->base.coordX && (int)sh->brainBase.base.coordY == (int)bBase->base.coordY && (int)sh->brainBase.base.coordXWant == (int)bBase->base.coordXWant && (int)sh->brainBase.base.coordYWant == (int)bBase->base.coordYWant)) {
                                vec2.x = sh->brainBase.base.coordX;
                                vec2.y = sh->brainBase.base.coordY;
                                distance = findDistance(vec1, vec2);
                                if (findDistance(vec1, vec2) <= bBase->viewRange * bBase->viewRange) {
                                    float d = findSqrtDistance(vec2, vec1);
                                    vec v = findVector(vec1, vec2);
                                    bBase->base.coordXWant = vec2.x;
                                    bBase->base.coordYWant = vec2.y;
                                    updateBase(&bBase->base, time, scaleE, s);
                                    if ((int)bBase->base.coordX == (int)vec2.x && (int)bBase->base.coordY == (int)vec2.y) {
                                        add_to_end(&sharksM, (STRUCTS) shark);
                                        sharksM.last->data.shark.brainBase.base.coordX = sh->brainBase.base.coordX;
                                        sharksM.last->data.shark.brainBase.base.coordY = sh->brainBase.base.coordY;
                                        sh->brainBase.base.lastDuplicationTime = 0;
                                        bBase->base.lastDuplicationTime = 0;
                                    }
                                    return;
                                }
                            }
                            node = node->next;
                        }
                        break;
                }
            }
            break;
        case WALKING:
            updateBase(&bBase->base, time, scaleE, bBase->food == PLANKTONS ? f : s);
            return;
    }
    step++;
    if (step <= 3)
        goto pri;
}

void updateEntities(float time, float scaleE) {
    NODE* node = planktonsM.head;
    while (node != NULL) {
        PLANKTON* pl = &node->data.plankton;
        updateBase(pl, time, scaleE, p);
        if (node->next != NULL)
            node = node->next;
        else break;
    }
    node = fishM.head;
    while (node != NULL) {
        FISH* fi = &node->data.fish;
        updateBrainBase(fi, time, scaleE);
        node = node->next;
    }
    node = sharksM.head;
    while (node != NULL) {
        SHARK* sh = &node->data.shark;
        updateBrainBase(&sh->brainBase, time, scaleE);
        node = node->next;
    }
}


void add_to_end(LinkedList* list, STRUCTS data) {
    NODE* new_node = malloc(sizeof(NODE));
    new_node->data = data;
    new_node->next = NULL;
    
    if (list->head == NULL) {
        list->size = 1;
        new_node->prev = NULL;
        list->head = new_node;
        list->last = new_node;
        return;
    }
    list->size++;
    new_node->prev = list->last;
    list->last->next = new_node;
    list->last = new_node;
}

void remove_at_index(LinkedList* list, int index) {
    if (index < 0) return;
    if (list->head == NULL) {
        return;
    }
    NODE* curr_node = list->head;
    
    if (index == 0) {
        if (list->size >= 2)
            curr_node->next->prev = NULL;
        else list->last = NULL;
        list->head = curr_node->next;
        goto r;
    }
    
    if (index == list->size - 1) {
        curr_node = list->last;
        list->last = list->last->prev;
        list->last->next = NULL;
        goto r;
    }

    for (int i = 0; i < index; i++) {
        if (curr_node->next == NULL) {
            return;
        }
        curr_node = curr_node->next;
    }
    curr_node->prev->next = curr_node->next;
    curr_node->next->prev = curr_node->prev;
    r:
    list->size--;
    free(curr_node);
}

int find_index(LinkedList* list, PLANKTON base) {
    NODE* curr_node = list->head;
    PLANKTON pl; FISH fi; SHARK sh;
    for (int i = 0; curr_node != NULL; i++) {
        switch (list->type) {
            case p:
                pl = curr_node->data.plankton;
                if ((int)pl.coordX == (int)base.coordX && (int)pl.coordY == (int)base.coordY && (int)pl.coordXWant == (int)base.coordXWant && (int)pl.coordYWant == (int)base.coordYWant) {
                    return i;
                }
                break;
            case f:
                fi = curr_node->data.fish;
                if ((int)fi.base.coordX == (int)base.coordX && (int)fi.base.coordY == (int)base.coordY && (int)fi.base.coordXWant == (int)base.coordXWant && (int)fi.base.coordYWant == (int)base.coordYWant) {
                    return i;
                }
                break;
            case s:
                sh = curr_node->data.shark;
                if ((int)sh.brainBase.base.coordX == (int)base.coordX && (int)sh.brainBase.base.coordY == (int)base.coordY && (int)sh.brainBase.base.coordXWant == (int)base.coordXWant && (int)sh.brainBase.base.coordYWant == (int)base.coordYWant) {
                    return i;
                }
                break;
        }
        curr_node = curr_node->next;
    }
    return -1;
}
