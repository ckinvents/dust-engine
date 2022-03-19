#include "dustengine.h"
#include <string.h>

void DustGrid_setIntern(Dust**, DustPos pos, DustID type);

// High-level management functions
DustWorld* DustWorld_createNew(int width, int height)
{
    DustWorld* newWorld = malloc(sizeof(DustWorld));
    newWorld->dust = DustGrid_createNew(width, height);
    newWorld->types = NULL;
    return newWorld;
}
DustWorld* DustWorld_init(DustWorld* world, DustType* types)
{
    world->types = types;
    return world;
}
DustWorld* DustWorld_update(DustWorld* world)
{
    DustID current;
    for (int y = 0; y < world->dust->height; y++)
    {
        for (int x = 0; x < world->dust->width; x++)
        {
            current = DustGrid_get(world->dust, (DustPos){x, y});
            if (current != DUST_EMPTY && current != DUST_WALL)
            {
                world->types[current].updateFunction(world->dust, (DustPos){x, y});
            }
        }
    }
    DustGrid_swap(world->dust);
    return world;
}
DustWorld* DustWorld_destroy(DustWorld* world)
{
    DustGrid_destroy(world->dust);
    free(world);
    return NULL;
}
// Interaction functions
DustWorld* DustWorld_draw(DustWorld* world, DustID type, DustPos pos)
{
    DustGrid_setIntern(world->dust->current, pos, type);
    return world;
}
DustWorld* DustWorld_drawRect(DustWorld* world, DustID type, DustRect rect)
{
    int draw_end_x = rect.pos.x + rect.width;
    int draw_end_y = rect.pos.y + rect.height;
    for (int y = rect.pos.y; y < draw_end_y; y++)
    {
        for (int x = rect.pos.x; x < draw_end_x; x++)
        {
            DustGrid_set(world->dust, type, (DustPos){x, y});
        }
    }
    return world;
}
DustWorld* DustWorld_erase(DustWorld* world, DustPos pos)
{
    DustGrid_clear(world->dust, pos);
    return world;
}

// DustPos functions
DustPos DustPos_offset(DustPos pos, int x, int y)
{
    return (DustPos){pos.x + x, pos.y + y};
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// DustGrid functions
// Private
/**
 * Sets a type for a particle in a DustGrid data buffer
 **/
void DustGrid_setIntern(Dust** data, DustPos pos, int typeID)
{
    data[pos.y][pos.x].type = typeID;
}

/**
 * Extension of setInternal to improve readability of offsets
 **/
void DustGrid_setInternOffset(Dust** data, DustPos pos, DustPos offset, int typeID)
{
    pos = DustPos_offset(pos, offset.x, offset.y);
    data[pos.y][pos.x].type = typeID;
}

int DustGrid_checkIntern(Dust** data, DustPos pos)
{
    return data[pos.y][pos.x].type;
}

int DustGrid_checkInternOffset(Dust** data, DustPos pos, DustPos offset, int typeID)
{
    pos = DustPos_offset(pos, offset.x, offset.y);
    return data[pos.y][pos.x].type;
}

int DustGrid_resolvePriority(DustGrid* grid, DustPos pos, DustID typeID)
{
    // Placeholder
    return rand() % 2;
}

void DustGrid_setDelta(DustGrid* grid, DustPos pos, DustPos offset)
{
    grid->new[pos.y][pos.x].delta.x = -offset.x;
    grid->new[pos.y][pos.x].delta.y = -offset.y;
}

DustPos DustGrid_getDelta(DustGrid* grid, DustPos pos)
{
    return grid->new[pos.y][pos.x].delta;
}

// Public
DustGrid* DustGrid_createNew(int width, int height)
{
    DustGrid* newGrid = malloc(sizeof(DustGrid));
    newGrid->width = width;
    newGrid->height = height;
    newGrid->current = malloc(sizeof(Dust*) * height);
    newGrid->new = malloc(sizeof(Dust*) * height);
    for (int i = 0; i < height; i++)
    {
        newGrid->current[i] = malloc(sizeof(Dust) * width);
        newGrid->new[i] = malloc(sizeof(Dust) * width);
        for (int j = 0; j < width; j++)
        {
            newGrid->current[i][j].delta = D_OFFSET_NO;
            newGrid->current[i][j].type = DUST_EMPTY;
            newGrid->new[i][j].delta = D_OFFSET_NO;
            newGrid->new[i][j].type = DUST_EMPTY;
        }
    }
    return newGrid;
}
DustGrid* DustGrid_set(DustGrid* grid, DustID type, DustPos pos)
{
    if (pos.x >= 0 && pos.x < grid->width && 
        pos.y >= 0 && pos.y < grid->height)
    {
        // Check for existing particle in location
        // Use density as a comparison factor (temp: random if matching)
        //int inNewPos = DustGrid_checkIntern(grid->new, pos);
        //if (type == DUST_EMPTY || inNewPos == DUST_EMPTY || DustGrid_resolvePriority(grid, pos, type))
        //{
            DustGrid_setIntern(grid->new, pos, type);
            DustGrid_setIntern(grid->current, pos, DUST_WALL);
            DustGrid_setDelta(grid, pos, D_OFFSET_NO);
        //}
    }
    return grid;
}
DustID DustGrid_get(DustGrid* grid, DustPos pos)
{
    return DustGrid_checkIntern(grid->current, pos);
}
DustGrid* DustGrid_clear(DustGrid* grid, DustPos pos)
{
    DustGrid_setIntern(grid->current, pos, DUST_EMPTY);
    return grid;
}
DustGrid* DustGrid_move(DustGrid* grid, DustPos pos, DustPos offset)
{   
    DustPos dest = DustPos_offset(pos, offset.x, offset.y);
    DustID current = DustGrid_checkIntern(grid->current, pos);
    DustID current_dest = DustGrid_checkIntern(grid->current, dest);
    if (dest.x >= 0 && dest.x < grid->width && 
        dest.y >= 0 && dest.y < grid->height)
    {
        if (current_dest == DUST_EMPTY)
        {
            if (DustGrid_checkIntern(grid->new, dest) == DUST_EMPTY)
            {
                DustGrid_setIntern(grid->new, dest, current);
                DustGrid_setIntern(grid->new, pos, DUST_EMPTY);
                DustGrid_setDelta(grid, dest, offset);
            }
            else if (DustGrid_resolvePriority(grid, dest, current))
            {
                DustID other = DustGrid_checkIntern(grid->new, dest);
                DustPos other_delta = DustGrid_getDelta(grid, dest);
                DustPos other_dest = DustPos_offset(dest, other_delta.x, other_delta.y);
                DustGrid_setIntern(grid->new, dest, current);
                DustGrid_setIntern(grid->new, pos, DUST_EMPTY);
                DustGrid_setDelta(grid, dest, offset);
                DustGrid_setInternOffset(grid->new, dest, other_delta, other);
                DustGrid_setDelta(grid, other_dest, other_delta);
            }
            else
            {
                DustGrid_static(grid, pos);
            }
        }
        else if (current_dest < current)
        {
            DustGrid_setIntern(grid->new, dest, current);
            DustGrid_setIntern(grid->new, pos, current_dest);
            DustGrid_setIntern(grid->current, dest, DUST_WALL);
            DustGrid_setIntern(grid->current, pos, DUST_WALL);
            DustGrid_setDelta(grid, dest, D_OFFSET_NO);
            DustGrid_setDelta(grid, pos, D_OFFSET_NO);
        }
    }
    else
    {
        DustGrid_static(grid, pos);
    }
    return grid;
}
DustGrid* DustGrid_static(DustGrid* grid, DustPos pos)
{
    DustGrid_setIntern(grid->new, pos, DustGrid_checkIntern(grid->current, pos));
    DustGrid_setDelta(grid, pos, D_OFFSET_NO);
    return grid;
}
DustID DustGrid_check(DustGrid* grid, DustPos pos, DustPos offset)
{
    DustID type;
    DustPos check_pos = DustPos_offset(pos, offset.x, offset.y);
    if (check_pos.x >= 0 && check_pos.x < grid->width && 
        check_pos.y >= 0 && check_pos.y < grid->height)
    {
        type = DustGrid_checkIntern(grid->current, check_pos);
    }
    else
    {
        type = DUST_WALL;
    }
    return type;
}
DustNeighbor DustGrid_checkNeighbors(DustGrid* grid, DustPos pos)
{
    DustNeighbor dust_nbs;
    dust_nbs.U = DustGrid_check(grid, pos, D_OFFSET_U);
    dust_nbs.D = DustGrid_check(grid, pos, D_OFFSET_D);
    dust_nbs.L = DustGrid_check(grid, pos, D_OFFSET_L);
    dust_nbs.R = DustGrid_check(grid, pos, D_OFFSET_R);
    return dust_nbs;
}
DustGrid* DustGrid_swap(DustGrid* grid)
{
    Dust** temp = grid->current;
    grid->current = grid->new;
    grid->new = temp;
    for (int i = 0; i < grid->height; i++)
    {
        for (int j = 0; j < grid->width; j++)
        {
            grid->new[i][j].type = DUST_EMPTY;
        }
    }
    return grid;
}
DustGrid* DustGrid_destroy(DustGrid* grid)
{
    for (int i = 0; i < grid->height; i++)
    {
        free(grid->current[i]);
        free(grid->new[i]);
    }
    free(grid->current);
    free(grid->new);
    free(grid);
    return NULL;
}


// DustType functions
void DustType_updateDefault(DustGrid* grid, DustPos pos)
{
    DustGrid_static(grid, pos);
}

int DustType_shadeDefault(DustGrid* grid, DustPos pos)
{
    return 0xFF00FFFF;
}

DustType* DustType_createNewList(int count)
{
    DustType* new_types = malloc(count*sizeof(DustType));
    for (int i = 0; i < count; i++)
    {
        DustType_addEntry(new_types, i, 0, 0, 0, DustType_shadeDefault, DustType_updateDefault);
    }
    return new_types;
}
DustType* DustType_addEntry(DustType* types,
    DustID id, float air_res, float flam, float dens, int (*shade)(DustGrid*, DustPos), void (*update)(DustGrid*, DustPos))
{
    types[id].air_res = air_res;
    types[id].flammability = flam;
    types[id].density = dens;
    types[id].shadeFunction = shade;
    types[id].updateFunction = update;
    return types;
}
DustType* DustType_destroyList(DustType* types)
{
    free(types);
    return NULL;
}