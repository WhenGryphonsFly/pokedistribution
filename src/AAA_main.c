#include "global.h"
#include "gflib.h"
#include "scanline_effect.h"
#include "task.h"
#include "save.h"
#include "event_data.h"
#include "menu.h"
#include "link.h"
#include "oak_speech.h"
#include "overworld.h"
#include "quest_log.h"
#include "mystery_gift_menu.h"
#include "strings.h"
#include "title_screen.h"
#include "help_system.h"
#include "pokedex.h"
#include "text_window.h"
#include "text_window_graphics.h"
#include "constants/songs.h"
#include "load_save.h"
#include "new_game.h"
#include "m4a.h"
#include "list_menu.h"
#include "mystery_gift_menu.h"

#include "AAA_main.h"
#include "custom_code/constants.c"
#include "custom_code/callbacks.c"

/*
	Main Code Path:
	Step 1: Show three option menu: "Berry Fix Program (R/S)", "Mystery Event (R/S)", "Mystery Gift (FR/LG/E)"
	Step 2: If "Berry Fix Program (R/S)"
		Execute CB2_FadeOutTransitionToBerryFix (from title_screen.c)
		Should require no additional work
	Step 3: If "Mystery Event (R/S)"
		??? Emulate eReader output
	Step 4: If "Mystery Gift (FR/LG/E)"
*/

// Step 1a
void CB2_MainCodePath(void) {
	MainCodePathInit();
	u8 taskId = CreateTask(Task_MakeBackgroundBlue, 0);
	SetMainCallback2(CB2_RootMenu);
}

// Step 1b
static void MainCodePathInit(void) {
	SetPokemonCryStereo(1);
    InitHeap(gHeap, HEAP_SIZE);
	SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
    DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
    DmaFill16(3, 0, (void *)(PLTT + 2), PLTT_SIZE - 2);
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetPaletteFade();
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, custom_sBgTemplate, NELEMS(custom_sBgTemplate));
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    /*InitWindows(custom_sWindowTemplate);*/
    DeactivateAllTextPrinters();
    LoadPalette(custom_sBgPal00, 0x00, 0x20);
    LoadPalette(custom_sBgPal15, 0xF0, 0x20);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0x0001);
    SetGpuReg(REG_OFFSET_WINOUT, 0x0021);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0));
    SetGpuReg(REG_OFFSET_BLDY, 7);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
}

// Step 1c
static void Task_MakeBackgroundBlue(u8 taskId)
{	
	if (WaitDma3Request(-1) != -1) {
		SetVBlankCallback(VBlankCB_RootMenu);
		gTasks[taskId].func = Task_RenderRootMenu;
	}
}

// Step 1d
static void Task_RenderRootMenu(u8 taskId) {
	
}
