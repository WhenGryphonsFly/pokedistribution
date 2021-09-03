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

// reset_save_heap.c
#include "global.h"
#include "gflib.h"
#include "m4a.h"
#include "load_save.h"
#include "save.h"
#include "new_game.h"
#include "overworld.h"
#include "main_menu.h"

void ResetSaveHeap(void)
{
    u16 imeBackup = REG_IME;
    
    REG_IME = 0;
    RegisterRamReset(RESET_EWRAM);
    ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_FORCED_BLANK);
    REG_IME = imeBackup;
    gMain.inBattle = FALSE;
    InitHeap(gHeap, HEAP_SIZE);
    SetMainCallback2(CB2_InitMainMenu);
}

// mail_data.c
#include "global.h"
#include "gflib.h"
#include "mail.h"
#include "mail_data.h"
#include "constants/items.h"
#include "pokemon_icon.h"

u16 MailSpeciesToSpecies(u16 mailSpecies, u16 *unownLetter)
{
    u16 result;

    if (mailSpecies >= 30000 && mailSpecies < (30000 + UNOWN_FORM_COUNT))
    {
        result = SPECIES_UNOWN;
        *unownLetter = mailSpecies - 30000;
    }
    else
    {
        result = mailSpecies;
    }
    return result;
}

// battle_main.c
#include "global.h"
#include "battle.h"

EWRAM_DATA u32 gBattleTypeFlags = 0;
