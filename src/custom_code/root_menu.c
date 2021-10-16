#ifdef GUARD_CUSTOM_CODE_C

void CB2_MainCodePath(void) {
	MainCodePathInit();
	SetMainCallback2(CB2_RootMenu);
	CreateTask(Task_MakeBackgroundBlue, 0);
	CreateTask(Task_RenderRootMenu, 0);	
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
    InitWindows(custom_sWindowTemplate);
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

static void Task_MakeBackgroundBlue(u8 taskId) {	
	if (WaitDma3Request(-1) != -1) {
		SetVBlankCallback(VBlankCB_RootMenu);
		EnableInterrupts(INTR_FLAG_VBLANK | INTR_FLAG_VCOUNT | INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL);
		DestroyTask(taskId);
	}
}

extern const struct WindowTemplate sMainWindows[];

static void Task_RenderRootMenu(u8 taskId) {
	if (WaitDma3Request(-1) != -1) {
        /*ResetPaletteFade();
        ResetSpriteData();
        FreeAllSpritePalettes();
        ResetTasks();
        ResetBgsAndClearDma3BusyFlags(1);

        InitBgsFromTemplates(0, custom_sBGTemplates_RootMenu, NELEMS(custom_sBGTemplates_RootMenu));
        ChangeBgX(0, 0, 0);
        ChangeBgY(0, 0, 0);
        ChangeBgX(1, 0, 0);
        ChangeBgY(1, 0, 0);
        ChangeBgX(2, 0, 0);
        ChangeBgY(2, 0, 0);
        ChangeBgX(3, 0, 0);
        ChangeBgY(3, 0, 0);

        SetBgTilemapBuffer(3, Alloc(0x800));
        SetBgTilemapBuffer(2, Alloc(0x800));
        SetBgTilemapBuffer(1, Alloc(0x800));
        SetBgTilemapBuffer(0, Alloc(0x800));

        LoadUserWindowBorderGfx(0, 10, 0xE0);
        DrawWindowBorderWithStdpal3(0,  1, 0xF0);
        DecompressAndLoadBgGfxUsingHeap(3, gUnkTextboxBorderGfx, 0x100, 0, 0);*/
        InitWindows(sMainWindows);
        /*DeactivateAllTextPrinters();
        ClearGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);

		CopyBgTilemapBufferToVram(3);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(0);

		ShowBg(0);
        ShowBg(3);

		//=======================================//*/

		InitBgsFromTemplates(0, custom_sBGTemplates_RootMenu, NELEMS(custom_sBGTemplates_RootMenu));	
		LoadUserWindowBorderGfx(0, 10, 0xE0);
		u32 windowAndTaskId = DrawListMenu(custom_sListMenuItems_RootMenu, 3, 3);
		DestroyTask(taskId);
	}
}

#endif // GUARD_CUSTOM_CODE_C
