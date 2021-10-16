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

#endif
