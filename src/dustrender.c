#include "dustrender.h"

#define DEFAULT_COL 0x24081CFF;
#define NO_SHADER 0xFFFF00FF;

void DustRender_renderWorld(PixBuffer* buffer, DustWorld* world, int offset_x, int offset_y)
{
    for (int y = offset_y; y < world->dust->height && y < buffer->height; y++)
    {
        for (int x = offset_x; x < world->dust->width && x < buffer->width; x++)
        {
            uint32_t color;
            DustID typeID = DustGrid_get(world->dust, (DustPos){x, y});
            if (typeID == DUST_EMPTY)
            {
                color = DEFAULT_COL;
            }
            else if (world->types[typeID].shadeFunction == NULL)
            {
                color = NO_SHADER;
                PixBuffer_drawPixAlpha(buffer, x, y, color, 1.0);
            }
            else
            {
                color = world->types[typeID].shadeFunction(world->dust, (DustPos){x, y});
                PixBuffer_drawPixAlpha(buffer, x, y, color, 1.0);
            }
        }
    }
}
