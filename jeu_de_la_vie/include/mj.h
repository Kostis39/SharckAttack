#ifndef MJ_H
#define MJ_H

#include "world.h"
#include <stdio.h>
#include <stdlib.h>

void SwitchTabCellWorld(World *worldNow, Cell **worldTmp);
void SetValueTabCell(Cell **tab, int x, int y, bool val);

#endif
