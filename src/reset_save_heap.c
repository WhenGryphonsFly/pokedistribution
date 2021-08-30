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
    SetSaveBlocksPointers();
    Save_ResetSaveCounters();
    Save_LoadGameData(SAVE_NORMAL);
    SetPokemonCryStereo(gSaveBlock2Ptr->optionsSound);
    InitHeap(gHeap, HEAP_SIZE);
    SetMainCallback2(CB2_InitMainMenu);
}
