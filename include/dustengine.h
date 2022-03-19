#ifndef DUSTENGINE_H
#define DUSTENGINE_H

#include <stdlib.h>
#include "misc_util.h"

#define DUST_EMPTY -1
#define DUST_WALL 0xFFFF
#define DUST_CYCLE_MASK 0x80

#define D_OFFSET_NO (DustPos){0,  0}
#define D_OFFSET_U  (DustPos){0, -1}
#define D_OFFSET_D  (DustPos){0,  1}
#define D_OFFSET_L  (DustPos){-1, 0}
#define D_OFFSET_R  (DustPos){1,  0}
#define D_OFFSET_UL (DustPos){-1,-1} 
#define D_OFFSET_UR (DustPos){1, -1}
#define D_OFFSET_DL (DustPos){-1, 1}
#define D_OFFSET_DR (DustPos){1,  1}


typedef enum {
    DSTAT_NONE = 0b0000,
    DSTAT_U = 0b0001,
    DSTAT_D = 0b0010,
    DSTAT_L = 0b0100,
    DSTAT_R = 0b1000
} DSTAT_FLAGS;

//#define D_RAND_LR (DustPos){-1 + (((rand() * rand()) & 1) << 1), 0}
#define D_RAND_LR (DustPos){rand() % 2 ? 1 : -1, 0}
#define D_RAND_UD (DustPos){0, -1 + ((rand() & 1) << 1)}

typedef int DustID;

typedef struct _DustVec {
    float x;
    float y;
} DustVec;

typedef struct _DustPos {
    int x;
    int y;
} DustPos;

typedef struct _DustRect {
    DustPos pos;
    int width;
    int height;
} DustRect;

typedef struct _Dust {
    DustID type;
    //int state;
    DustPos delta;
} Dust;

typedef struct _DustNeighbor {
    DustID U;
    DustID D;
    DustID L;
    DustID R;
} DustNeighbor;

typedef struct _DustGrid {
    int width;
    int height;
    Dust** current;
    Dust** new;
} DustGrid;

typedef struct _DustType {
    float air_res;
    float flammability;
    float density;
    int (*shadeFunction)(DustGrid*, DustPos);
    void (*updateFunction)(DustGrid*, DustPos);
} DustType;

typedef struct _DustWorld {
    DustGrid* dust;
    DustType* types;
} DustWorld;

// High-level management functions
DustWorld* DustWorld_createNew(int, int);
DustWorld* DustWorld_init(DustWorld*, DustType*);
DustWorld* DustWorld_update(DustWorld*);
DustWorld* DustWorld_destroy(DustWorld*);
// Interaction functions
DustWorld* DustWorld_draw(DustWorld*, DustID, DustPos);
DustWorld* DustWorld_drawRect(DustWorld*, DustID, DustRect);
DustWorld* DustWorld_erase(DustWorld*, DustPos);
// DustPos functions
DustPos DustPos_offset(DustPos, int x, int y);
// DustGrid functions
DustGrid* DustGrid_createNew(int, int);
DustID DustGrid_get(DustGrid*, DustPos);
DustGrid* DustGrid_set(DustGrid*, DustID, DustPos);
DustGrid* DustGrid_clear(DustGrid*, DustPos);
DustGrid* DustGrid_move(DustGrid*, DustPos, DustPos);
DustGrid* DustGrid_static(DustGrid* grid, DustPos pos);
DustID DustGrid_check(DustGrid*, DustPos, DustPos);
DustNeighbor DustGrid_checkNeighbors(DustGrid*, DustPos);
DustGrid* DustGrid_swap(DustGrid*);
DustGrid* DustGrid_destroy();
// DustType functions
DustType* DustType_createNewList(int count);
DustType* DustType_addEntry(DustType*, DustID, float, float, float, int (*)(DustGrid*, DustPos), void (*)(DustGrid*, DustPos));
DustType* DustType_destroyList(DustType*);

#endif //DUSTENGINE_H