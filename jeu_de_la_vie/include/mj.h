#ifndef MJ_H
#define MJ_H

#include "world.h"
#include <stdlib.h>
#include <stdio.h>

void SwitchTabCellWorld(World *worldNow, Cell **worldTmp);
void SetValueTabCell(Cell **tab, int x, int y, bool val);
World *InitWorld(int size);

#endif
