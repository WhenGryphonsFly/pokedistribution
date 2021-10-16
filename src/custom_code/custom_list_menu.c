#ifdef GUARD_CUSTOM_CODE_C

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

#endif
