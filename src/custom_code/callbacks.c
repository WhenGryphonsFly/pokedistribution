#ifdef GUARD_CUSTOM_CODE_C

void CB2_RootMenu(void)
{
    RunTasks();
    RunTextPrinters();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_RootMenu(void) {
	LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

#endif // GUARD_CUSTOM_CODE_C
