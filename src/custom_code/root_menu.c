#ifdef GUARD_CUSTOM_CODE_C

void CB2_MainCodePath(void) {
	MainCodePathInit();
	CreateTask(Task_MakeBackgroundBlue, 0);
	CreateTask(Task_RenderRootMenu, 1);
	SetMainCallback2(CB2_RootMenu);
}

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
    InitBgsFromTemplates(0, custom_sBgTemplate_Init, NELEMS(custom_sBgTemplate_Init));
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
    /*SetGpuReg(REG_OFFSET_WININ, 0x0001);
    SetGpuReg(REG_OFFSET_WINOUT, 0x0021);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_TGT1_OBJ | BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 0));
    SetGpuReg(REG_OFFSET_BLDY, 7);*/
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP/* | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON*/);
}

static void Task_MakeBackgroundBlue(u8 taskId)
{	
	if (WaitDma3Request(-1) != -1) {
		SetVBlankCallback(VBlankCB_RootMenu);
		DestroyTask(taskId);
	}
}

static void Task_RenderRootMenu(u8 taskId) {
}

#endif // GUARD_CUSTOM_CODE_C
