#include "sandbox_test.h"

int** color_offsets;

// Private update functions
void Test_updateSAND(DustGrid* grid, DustPos pos);
void Test_updateSMOKE(DustGrid* grid, DustPos pos);
void Test_updateSTONE(DustGrid* grid, DustPos pos);
void Test_updateBRICK(DustGrid* grid, DustPos pos);
void Test_updateANTIS(DustGrid* grid, DustPos pos);
void Test_updateWATER(DustGrid* grid, DustPos pos);
void Test_updateLAVA(DustGrid* grid, DustPos pos);
void Test_updateSTEAM(DustGrid* grid, DustPos pos);

int Test_shadeSAND(DustGrid* grid, DustPos pos);
int Test_shadeSMOKE(DustGrid* grid, DustPos pos);
int Test_shadeSTONE(DustGrid* grid, DustPos pos);
int Test_shadeBRICK(DustGrid* grid, DustPos pos);
int Test_shadeANTIS(DustGrid* grid, DustPos pos);
int Test_shadeWATER(DustGrid* grid, DustPos pos);
int Test_shadeLAVA(DustGrid* grid, DustPos pos);
int Test_shadeSTEAM(DustGrid* grid, DustPos pos);

void Test_updateDEFAULT(DustGrid* grid, DustPos pos);
int Test_shadeDEFAULT(DustGrid* grid, DustPos pos);

void Test_updateSAND(DustGrid* grid, DustPos pos)
{
    DustNeighbor sand_nbs = DustGrid_checkNeighbors(grid, pos);
    int melt_prob = !(rand() % 500);
    if (melt_prob && (sand_nbs.U == LAVA ||
        sand_nbs.D == LAVA ||
        sand_nbs.L == LAVA ||
        sand_nbs.R == LAVA))
    {
        DustGrid_set(grid, LAVA, pos);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_D) < SAND)
    {
        DustGrid_move(grid, pos, D_OFFSET_D);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_DL) == DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_DR) == DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_L) == DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_R) == DUST_EMPTY)
    {
        DustGrid_move(grid, pos, D_RAND_LR);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_L) != DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_R) == DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_DR) == DUST_EMPTY)
    {
        DustGrid_move(grid, pos, D_OFFSET_DR);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_R) != DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_L) == DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_DL) == DUST_EMPTY)
    {
        DustGrid_move(grid, pos, D_OFFSET_DL);
    }
    else
    {
        DustGrid_static(grid, pos);
    }
}


void Test_updateSMOKE(DustGrid* grid, DustPos pos)
{
    if (DustGrid_check(grid, pos, D_OFFSET_U) < SMOKE)
    {
        if (rand() % 3)
        {
            DustGrid_move(grid, pos, D_OFFSET_U);
        }
        else if (DustGrid_check(grid, pos, D_OFFSET_D) == DUST_EMPTY &&
            !(rand() % 5))
        {
            DustGrid_move(grid, pos, D_OFFSET_D);
        }
        else
        {
            DustGrid_static(grid, pos);
        }
    }
    else if (!(rand() % 50))
    {
        DustGrid_clear(grid, pos);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_D) == DUST_EMPTY &&
             !(rand() % 4))
    {
        DustGrid_move(grid, pos, D_OFFSET_D);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_U) != DUST_EMPTY ||
            !(rand() % 10))
    {
        if (DustGrid_check(grid, pos, D_OFFSET_R) == DUST_EMPTY &&
                DustGrid_check(grid, pos, D_OFFSET_L) == DUST_EMPTY)
        {
            DustGrid_move(grid, pos, D_RAND_LR);
        }
        else if (DustGrid_check(grid, pos, D_OFFSET_L) != DUST_EMPTY &&
                DustGrid_check(grid, pos, D_OFFSET_R) == DUST_EMPTY)
        {
            DustGrid_move(grid, pos, D_OFFSET_R);
        }
        else if (DustGrid_check(grid, pos, D_OFFSET_R) != DUST_EMPTY &&
                DustGrid_check(grid, pos, D_OFFSET_L) == DUST_EMPTY)
        {
            DustGrid_move(grid, pos, D_OFFSET_L);
        }
        else
        {
            DustGrid_static(grid, pos);
        }
    }
    else
    {
        DustGrid_static(grid, pos);
    }
}

void Test_updateBRICK(DustGrid* grid, DustPos pos)
{
    DustGrid_static(grid, pos);
}

void Test_updateANTIS(DustGrid* grid, DustPos pos)
{
    DustNeighbor nbs = DustGrid_checkNeighbors(grid, pos);
    int erode_prob = !(rand() % 1000);
    if (erode_prob && (nbs.U == WATER ||
        nbs.D == WATER ||
        nbs.L == WATER ||
        nbs.R == WATER))
    {
        DustGrid_set(grid, STONE, pos);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_U) < ANTI_SAND)
    {
        DustGrid_move(grid, pos, D_OFFSET_U);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_UL) < ANTI_SAND &&
             DustGrid_check(grid, pos, D_OFFSET_UR) < ANTI_SAND &&
             DustGrid_check(grid, pos, D_OFFSET_L) < ANTI_SAND &&
             DustGrid_check(grid, pos, D_OFFSET_R) < ANTI_SAND)
    {
        DustGrid_move(grid, pos, D_RAND_LR);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_L) >= ANTI_SAND &&
             DustGrid_check(grid, pos, D_OFFSET_R) < ANTI_SAND &&
             DustGrid_check(grid, pos, D_OFFSET_UR) < ANTI_SAND)
    {
        DustGrid_move(grid, pos, D_OFFSET_UR);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_R) >= ANTI_SAND &&
             DustGrid_check(grid, pos, D_OFFSET_L) < ANTI_SAND &&
             DustGrid_check(grid, pos, D_OFFSET_UL) < ANTI_SAND)
    {
        DustGrid_move(grid, pos, D_OFFSET_UL);
    }
    else
    {
        DustGrid_static(grid, pos);
    }
}

void Test_updateWATER(DustGrid* grid, DustPos pos)
{
    if (DustGrid_check(grid, pos, D_OFFSET_D) < WATER)
    {
        DustGrid_move(grid, pos, D_OFFSET_D);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_D) != DUST_EMPTY)// ||
    //        !(rand() % 10))
    {
        if (DustGrid_check(grid, pos, D_OFFSET_R) == DUST_EMPTY &&
                DustGrid_check(grid, pos, D_OFFSET_L) == DUST_EMPTY)
        {
            DustGrid_move(grid, pos, D_RAND_LR);
        }
        else if (DustGrid_check(grid, pos, D_OFFSET_L) != DUST_EMPTY &&
                DustGrid_check(grid, pos, D_OFFSET_R) == DUST_EMPTY)
        {
            DustGrid_move(grid, pos, D_OFFSET_R);
        }
        else if (DustGrid_check(grid, pos, D_OFFSET_R) != DUST_EMPTY &&
                DustGrid_check(grid, pos, D_OFFSET_L) == DUST_EMPTY)
        {
            DustGrid_move(grid, pos, D_OFFSET_L);
        }
        else
        {
            DustGrid_static(grid, pos);
        }
    }
    else
    {
        DustGrid_static(grid, pos);
    }
}

void Test_updateSTONE(DustGrid* grid, DustPos pos)
{
    DustNeighbor stone_nbs = DustGrid_checkNeighbors(grid, pos);
    int melt_prob = !(rand() % 1000);
    if (melt_prob && (stone_nbs.U == WATER ||
        stone_nbs.D == WATER ||
        stone_nbs.L == WATER ||
        stone_nbs.R == WATER))
    {
        DustGrid_set(grid, SAND, pos);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_D) < STONE)
    {
        DustGrid_move(grid, pos, D_OFFSET_D);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_L) != DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_R) == DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_DR) == DUST_EMPTY)
    {
        DustGrid_move(grid, pos, D_OFFSET_DR);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_R) != DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_L) == DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_DL) == DUST_EMPTY)
    {
        DustGrid_move(grid, pos, D_OFFSET_DL);
    }
    else
    {
        DustGrid_static(grid, pos);
    }
}

void Test_updateLAVA(DustGrid* grid, DustPos pos)
{
    DustNeighbor lava_nbs = DustGrid_checkNeighbors(grid, pos);
    int anti_prob = !(rand() % 500);
    if (anti_prob && (lava_nbs.U == ANTI_SAND ||
        lava_nbs.D == ANTI_SAND ||
        lava_nbs.L == ANTI_SAND ||
        lava_nbs.R == ANTI_SAND))
    {
        DustGrid_set(grid, STONE, pos);
    }
    else if (lava_nbs.U == WATER)
    {
        DustGrid_set(grid, STEAM, DustPos_offset(D_OFFSET_U, pos.x, pos.y));
        DustGrid_set(grid, STONE, pos);
    }
    else if (lava_nbs.D == WATER)
    {
        DustGrid_set(grid, STEAM, DustPos_offset(D_OFFSET_D, pos.x, pos.y));
        DustGrid_set(grid, STONE, pos);
    }
    else if (lava_nbs.L == WATER)
    {
        DustGrid_set(grid, STEAM, DustPos_offset(D_OFFSET_L, pos.x, pos.y));
        DustGrid_set(grid, STONE, pos);
    }
    else if (lava_nbs.R == WATER)
    {
        DustGrid_set(grid, STEAM, DustPos_offset(D_OFFSET_R, pos.x, pos.y));
        DustGrid_set(grid, STONE, pos);
    }
    else
    {
        if (DustGrid_check(grid, pos, D_OFFSET_U) == DUST_EMPTY &&
            !(rand() % 1000))
        {
            DustGrid_set(grid, SMOKE, DustPos_offset(D_OFFSET_U, pos.x, pos.y));
        }
        if (DustGrid_check(grid, pos, D_OFFSET_D) < LAVA &&
            rand() % 2)
        {
            DustGrid_move(grid, pos, D_OFFSET_D);
        }
        else if (DustGrid_check(grid, pos, D_OFFSET_D) != DUST_EMPTY &&
                !(rand() % 10))
        {
            if (DustGrid_check(grid, pos, D_OFFSET_R) == DUST_EMPTY &&
                    DustGrid_check(grid, pos, D_OFFSET_L) == DUST_EMPTY)
            {
                DustGrid_move(grid, pos, D_RAND_LR);
            }
            else if (DustGrid_check(grid, pos, D_OFFSET_L) != DUST_EMPTY &&
                    DustGrid_check(grid, pos, D_OFFSET_R) == DUST_EMPTY)
            {
                DustGrid_move(grid, pos, D_OFFSET_R);
            }
            else if (DustGrid_check(grid, pos, D_OFFSET_R) != DUST_EMPTY &&
                    DustGrid_check(grid, pos, D_OFFSET_L) == DUST_EMPTY)
            {
                DustGrid_move(grid, pos, D_OFFSET_L);
            }
            else
            {
                DustGrid_static(grid, pos);
            }
        }
        else
        {
            DustGrid_static(grid, pos);
        }
    }
}

void Test_updateSTEAM(DustGrid* grid, DustPos pos)
{
    DustNeighbor steam_nbs = DustGrid_checkNeighbors(grid, pos);
    if (steam_nbs.U < STEAM)
    {
        if (rand() % 3)
        {
            DustGrid_move(grid, pos, D_OFFSET_U);
        }
        else if (steam_nbs.D == DUST_EMPTY &&
            !(rand() % 5))
        {
            DustGrid_move(grid, pos, D_OFFSET_D);
        }
        else
        {
            DustGrid_static(grid, pos);
        }
    }
    else if (steam_nbs.D == DUST_EMPTY &&
             !(rand() % 4))
    {
        if (steam_nbs.L == DUST_EMPTY &&
            steam_nbs.R == DUST_EMPTY &&
            !(rand() % 200))
        {
            DustGrid_set(grid, WATER, pos);
        }
        else
        {
            DustGrid_move(grid, pos, D_OFFSET_D);
        }
    }
    else if (steam_nbs.U != DUST_EMPTY ||
            !(rand() % 10))
    {
        if (steam_nbs.R == DUST_EMPTY &&
            steam_nbs.L == DUST_EMPTY)
        {
            DustGrid_move(grid, pos, D_RAND_LR);
        }
        else if (steam_nbs.L != DUST_EMPTY &&
                steam_nbs.R == DUST_EMPTY)
        {
            DustGrid_move(grid, pos, D_OFFSET_R);
        }
        else if (steam_nbs.R != DUST_EMPTY &&
                steam_nbs.L == DUST_EMPTY)
        {
            DustGrid_move(grid, pos, D_OFFSET_L);
        }
        else
        {
            DustGrid_static(grid, pos);
        }
    }
    else
    {
        DustGrid_static(grid, pos);
    }
}

/*
void Test_updateGRASS(DustGrid* grid, DustPos pos)
{
    if (DustGrid_check(grid, pos, D_OFFSET_D) == DUST_EMPTY &&
        DustGrid_check(grid, pos, D_OFFSET_DL) != GRASS &&
        DustGrid_check(grid, pos, D_OFFSET_DR) != GRASS)
    {
        DustGrid_move(grid, pos, D_OFFSET_D);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_DL) == DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_DR) == DUST_EMPTY)
    {
        DustGrid_move(grid, pos, DustPos_offset(D_RAND_LR, pos.x, pos.y - 1));
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_L) != DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_R) == DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_DR) == DUST_EMPTY)
    {
        DustGrid_move(grid, pos, D_OFFSET_DR);
    }
    else if (DustGrid_check(grid, pos, D_OFFSET_R) != DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_L) == DUST_EMPTY &&
             DustGrid_check(grid, pos, D_OFFSET_DL) == DUST_EMPTY)
    {
        DustGrid_move(grid, pos, D_OFFSET_DL);
    }
    else
    {
        DustGrid_static(grid, pos);
        DustGrid_set(grid, GRASS, DustPos_offset(D_RAND_LR, pos.x, pos.y - 1));
    }
}
*/

// Shaders
int Test_shadeSAND(DustGrid* grid, DustPos pos)
{
    int color_offset = color_offsets[pos.y][pos.x] & 0x1F;
    return 0xFF + 
            ((0xEF - color_offset) << 24) +
            ((0xDF - color_offset) << 16) +
            ((0xA0 - color_offset) << 8);
}
int Test_shadeSMOKE(DustGrid* grid, DustPos pos)
{
    //int color_offset = color_offsets[pos.y][pos.x] & 0xF;
    int color_offset = rand() & 0xF;
    return 0x4F + 
            0x30 * (DustGrid_check(grid, pos, D_OFFSET_U) == SMOKE) +
            0x30 * (DustGrid_check(grid, pos, D_OFFSET_D) == SMOKE) +
            0x30 * (DustGrid_check(grid, pos, D_OFFSET_L) == SMOKE) +
            0x30 * (DustGrid_check(grid, pos, D_OFFSET_R) == SMOKE) + 
            ((0x1F - color_offset) << 24) +
            ((0x0F - color_offset) << 16) +
            ((0x1F - color_offset) << 8);
}
int Test_shadeSTONE(DustGrid* grid, DustPos pos)
{
    int color_offset = color_offsets[pos.y][pos.x] % 0x8F;
    return 0xFF + 
            ((0x10 + color_offset) << 24) +
            ((0x10 + color_offset) << 16) +
            ((0x10 + color_offset) << 8);
}
int Test_shadeANTIS(DustGrid* grid, DustPos pos)
{
int color_offset = color_offsets[pos.y][pos.x] & 0x1F;
    return 0xFF + 
            ((0xFF - (0xEF - color_offset)) << 24) +
            ((0xFF - (0xDF - color_offset)) << 16) +
            ((0xFF - (0xA0 - color_offset)) << 8);
}
int Test_shadeBRICK(DustGrid* grid, DustPos pos)
{
    int color_offset = color_offsets[pos.y][pos.x] & 0x7;
    return 0xFF + 
            ((0x90 + color_offset) << 24) +
            ((0x52 + color_offset) << 16) +
            ((0x57 + color_offset) << 8);
}
int Test_shadeWATER(DustGrid* grid, DustPos pos)
{
    //int color_offset = color_offsets[pos.y][pos.x] & 0xF;
    int color_offset = rand() & 0x1F;
    return 0x2F + 
            0x20 * (DustGrid_check(grid, pos, D_OFFSET_U) == WATER) +
            0x20 * (DustGrid_check(grid, pos, D_OFFSET_D) == WATER) +
            0x20 * (DustGrid_check(grid, pos, D_OFFSET_L) == WATER) +
            0x20 * (DustGrid_check(grid, pos, D_OFFSET_R) == WATER) + 
            ((0x22 - color_offset) << 24) +
            ((0x44 - color_offset) << 16) +
            ((0x88 - color_offset) << 8);
}
int Test_shadeLAVA(DustGrid* grid, DustPos pos)
{
    int color_offset = rand() & 0x2F;
    return 0x7F + 
            0x20 * (DustGrid_check(grid, pos, D_OFFSET_U) == LAVA) +
            0x20 * (DustGrid_check(grid, pos, D_OFFSET_D) == LAVA) +
            0x20 * (DustGrid_check(grid, pos, D_OFFSET_L) == LAVA) +
            0x20 * (DustGrid_check(grid, pos, D_OFFSET_R) == LAVA) + 
            ((0xF8) << 24) +
            ((0x88 - color_offset) << 16) +
            ((0x22) << 8);
}

int Test_shadeSTEAM(DustGrid* grid, DustPos pos)
{
    //int color_offset = color_offsets[pos.y][pos.x] & 0xF;
    int color_offset = rand() & 0xF;
    return 0x4F + 
            0x10 * (DustGrid_check(grid, pos, D_OFFSET_U) == STEAM) +
            0x10 * (DustGrid_check(grid, pos, D_OFFSET_D) == STEAM) +
            0x10 * (DustGrid_check(grid, pos, D_OFFSET_L) == STEAM) +
            0x10 * (DustGrid_check(grid, pos, D_OFFSET_R) == STEAM) + 
            ((0x88 - color_offset) << 24) +
            ((0x88 - color_offset) << 16) +
            ((0x88 - color_offset) << 8);
}

DustType* Test_createConfig(int width, int height)
{
    // Generate color offsets table
    color_offsets = malloc(sizeof(int*) * height);
    for (int y = 0; y < height; y++)
    {
        color_offsets[y] = malloc(sizeof(int) * width);
        for (int x = 0; x < width; x++)
        {
            color_offsets[y][x] = rand() % 0xFF;
        }
    }
    // Add functions for types
    DustType* testList = DustType_createNewList(NUM_TYPES);
    
    DustType_addEntry(testList, SAND, 0, 0, 0, Test_shadeSAND, Test_updateSAND);
    DustType_addEntry(testList, SMOKE, 0, 0, 0, Test_shadeSMOKE, Test_updateSMOKE);
    DustType_addEntry(testList, BRICK, 0, 0, 0, Test_shadeBRICK, Test_updateBRICK);
    DustType_addEntry(testList, WATER, 0, 0, 0, Test_shadeWATER, Test_updateWATER);
    DustType_addEntry(testList, ANTI_SAND, 0, 0, 0, Test_shadeANTIS, Test_updateANTIS);
    DustType_addEntry(testList, STONE, 0, 0, 0, Test_shadeSTONE, Test_updateSTONE);
    DustType_addEntry(testList, LAVA, 0, 0, 0, Test_shadeLAVA, Test_updateLAVA);
    DustType_addEntry(testList, STEAM, 0, 0, 0, Test_shadeSTEAM, Test_updateSTEAM);

    return testList;
}

void Test_destroyConfig(DustType* testList, int height)
{
    DustType_destroyList(testList);
    for (int y = 0; y < height; y++)
    {
        free(color_offsets[y]);
    }
    free(color_offsets);
}