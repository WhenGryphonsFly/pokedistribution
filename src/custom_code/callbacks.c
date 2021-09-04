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
