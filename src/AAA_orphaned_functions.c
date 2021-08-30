//[[!!!]]

// battle_records.h
#include "global.h"
#include "battle_records.h"
void TryRecordLinkBattleOutcome(s32 battlerId){}

// scrcmd.c
#include "gba/gba.h"
#include <string.h>
#include "constants/global.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/species.h"
void *const gNullScriptPtr = NULL;

// new_game.c / mevent.c
#include "new_game.h"
void CopyTrainerId(u8 *dst, u8 *src)
{
    s32 i;
    for (i = 0; i < 4; i++)
        dst[i] = src[i];
}

// item_menu.c
#include "item_menu.h"
#include "constants/items.h"
EWRAM_DATA u16 gSpecialVar_ItemId = ITEM_NONE;

// random.c
#include "global.h"
u32 gRngValue;

u16 Random(void)
{
    gRngValue = 1103515245 * gRngValue + 24691;
    return gRngValue >> 16;
}

void SeedRng(u16 seed)
{
    gRngValue = seed;
}
