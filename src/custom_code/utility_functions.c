u32 DrawListMenu(void) {
	struct ListMenuTemplate listMenuTemplate;
	struct WindowTemplate windowTemplate;
	u32 windowAndTaskId;

	// set ListMenuTemplate Items
	SetListMenuWidth(&listMenuTemplate, &windowTemplate);
	// set ListMenu height
	windowAndTaskId = InitializeListMenuWindow(&listMenuTemplate, &windowTemplate);

	return windowAndTaskId;
}

void SetListMenuWidth(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate) {
	u32 width;
	s32 finalWidth;
	u32 i;
	
	width = 0;
	for (i = 0; i < listmenuTemplate->totalItems; i++) {
		u32 curWidth = GetStringWidth(2, listMenuTemplate.items[i].label, listMenuTemplate.lettersSpacing);
        if (curWidth > width)
            width = curWidth;
	}
	finalWidth = (((width + 9) / 8) + 2) & ~1;
	windowTemplate.width = finalWidth;
    windowTemplate.tilemapLeft = (30 - finalWidth) / 2;
	return;
}

u16 InitializeListMenu(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate) {
	u16 windowId;
	u8 taskId;
	u32 windowAndTaskId;

	windowId = AddWindow(windowTemplate);
	DrawTextBorderOuter(windowId, 0x00A, 0xE);

	gMultiuseListMenuTemplate = *listMenuTemplate;
	gMultiuseListMenuTemplate.windowId = windowId;
	taskId = ListMenuInit(&gMultiuseListMenuTemplate, 0, 0);
	CopyWindowToVram(windowId, COPYWIN_MAP);

	windowAndTaskId = (windowId << 16) | taskId;
	return windowAndTaskId;
}

u32 GetListMenuResponse(u32 windowAndTaskId) {
	bool8 hasSelected;
	u32 selection;
	u16 windowId;
	u8 taskId;

	windowId = windowAndTaskId >> 16;
	taskId = (u8) (windowAndTaskId & 0xFF);

	selection = ListMenu_ProcessInput(taskId);
	hasSelected = FALSE;
	if (JOY_NEW(A_BUTTON)) {
		hasSelected = TRUE;
	}
	if (JOY_NEW(B_BUTTON)) {
		selection = LIST_CANCEL;
		hasSelected = TRUE;
	}

	if (hasSelected) {
		return selection;
	}
	return LIST_NOTHING_CHOSEN;
}

void DestroyListMenu(u32 windowAndtaskId) {
	u16 windowId;
	u8 taskId;

	windowId = windowAndTaskId >> 16;
	taskId = (u8) (windowAndTaskId & 0xFF);

	ClearStdWindowAndFrame(windowId, FALSE);
	CopyWindowToVram(windowId, COPYWIN_MAP);
	DestroyListMenuTask(taskId, NULL, NULL);
	RemoveWindow(windowId);
}
