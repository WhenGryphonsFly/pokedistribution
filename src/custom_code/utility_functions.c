u32 DrawListMenu(const struct ListMenuItem* listMenuItems, u32 listMenuLength, u32 listMenuMax) {
	struct ListMenuTemplate listMenuTemplate = custom_sListMenuTemplate_Base;
	struct WindowTemplate windowTemplate = custom_sWindowTemplate_Base;
	u32 windowAndTaskId;

	listMenuTemplate.items = listMenuItems;
	listMenuTemplate.totalItems = listMenuLength;
	listMenuTemplate.maxShowed = listMenuMax;
	
	SetListMenuWidth(&listMenuTemplate, &windowTemplate);
	SetListMenuHeight(&listMenuTemplate, &windowTemplate);
	windowAndTaskId = InitializeListMenu(&listMenuTemplate, &windowTemplate);

	return windowAndTaskId;
}

void SetListMenuWidth(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate) {
	u32 width;
	s32 finalWidth;
	u32 i;
	
	width = 0;
	for (i = 0; i < listMenuTemplate->totalItems; i++) {
		u32 curWidth = GetStringWidth(2, listMenuTemplate->items[i].label, listMenuTemplate->lettersSpacing);
        if (curWidth > width)
            width = curWidth;
	}
	finalWidth = (((width + 9) / 8) + 2) & ~1;
	windowTemplate->width = finalWidth;
    windowTemplate->tilemapLeft = (30 - finalWidth) / 2;
	return;
}

void SetListMenuHeight(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate) {
	u32 height;
	u32 delta;

	height = 0;
	switch (listMenuTemplate->maxShowed) {
		case 2:
			height = 4;
			break;
		case 3:
			height = 5;
			break;
		case 4:
			height = 7;
			break;
		default:
			height = 0;
			break;
	}
	
	windowTemplate->height = height;
	delta = height / 2;
	windowTemplate->tilemapTop = 0x09 - delta; // 0x13 / 2 = 0x09
	return;
}

u16 InitializeListMenu(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate) {
	u16 windowId;
	u8 taskId;
	u32 windowAndTaskId;

	windowId = AddWindow(windowTemplate);
	DrawTextBorderOuter((windowId & 0xFF), 0x00A, 0xE);
	TextWindow_SetUserSelectedFrame((windowId & 0xFF), 0x00A, 0xE0);

	gMultiuseListMenuTemplate = *listMenuTemplate;
	gMultiuseListMenuTemplate.windowId = (windowId & 0xFF);
	taskId = ListMenuInit(&gMultiuseListMenuTemplate, 0, 0);
	ShowBg(0);
	CopyWindowToVram((windowId & 0xFF), COPYWIN_MAP);

	CopyBgTilemapBufferToVram(0);
	CopyBgTilemapBufferToVram(1);
	CopyBgTilemapBufferToVram(2);
	CopyBgTilemapBufferToVram(3);

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

void DestroyListMenu(u32 windowAndTaskId) {
	u16 windowId;
	u8 taskId;

	windowId = windowAndTaskId >> 16;
	taskId = (u8) (windowAndTaskId & 0xFF);

	ClearStdWindowAndFrame(windowId, FALSE);
	CopyWindowToVram(windowId, COPYWIN_MAP);
	DestroyListMenuTask(taskId, NULL, NULL);
	RemoveWindow(windowId);
}
