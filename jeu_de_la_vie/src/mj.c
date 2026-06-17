#include "mj.h"

void SwitchTabCellWorld(World *worldNow, Cell **worldTmp) {
    if (worldNow == NULL || worldNow->tab == NULL || worldTmp == NULL)
        return;

    int size = worldNow->size;
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            SetStateCell(&worldNow->tab[x][y], IsAlive(&worldTmp[x][y]));
            SetStateCell(&worldTmp[x][y], false);
        }
    }
}

void SetValueTabCell(Cell **tab, int x, int y, bool val) {
    SetStateCell(&tab[x][y], val);
}
