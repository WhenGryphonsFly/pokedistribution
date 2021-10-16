#ifdef GUARD_CUSTOM_CODE_C

const struct ListMenuTemplate custom_sListMenuTemplate_Base = {
    .items = NULL,
    .moveCursorFunc = ListMenuDefaultCursorMoveFunc,
    .itemPrintFunc = NULL,
    .totalItems = 0, // NULL
    .maxShowed = 0, // NULL
    .windowId = 0,
    .header_X = 0,
    .item_X = 8,
    .cursor_X = 0,
    .upText_Y = 0,
    .cursorPal = 2,
    .fillValue = 1,
    .cursorShadowPal = 3,
    .lettersSpacing = 0,
    .itemVerticalPadding = 0,
    .scrollMultiple = 0,
    .fontId = 2,
    .cursorKind = 0
};
const struct WindowTemplate custom_sWindowTemplate_Base = {
    .bg = 0x00,
    .tilemapLeft = 0x08, // NULL
    .tilemapTop = 0x05, // NULL
    .width = 0x0e, // NULL
    .height = 0x05, // NULL
    .paletteNum = 0x0e,
    .baseBlock = 0x0155
};
const u8 custom_gText_NotImplemented[] = _("This feature has not yet been\nimplemented.");



const u8 custom_gText_BerryFixProgram[] = _("Berry Fix Program (R/S)");
const u8 custom_gText_MysteryEvent[] = _("Mystery Event (R/S)");
const u8 custom_gText_MysteryGift[] = _("Mystery Gift (FR/LG/E)");
const struct ListMenuItem custom_sListMenuItems_RootMenu[] = {
	{ custom_gText_BerryFixProgram, 0 },
	{ custom_gText_MysteryEvent,    1 },
	{ custom_gText_MysteryGift,     2 }
};



const u8 custom_gText_Test4[] = _("Testing 4");
const u8 custom_gText_Test5[] = _("Testing 5");
const u8 custom_gText_Test6[] = _("Testing 6");
const u8 custom_gText_Test7[] = _("Testing 7");
const u8 custom_gText_Test8[] = _("Testing 8");
const u8 custom_gText_Test9[] = _("Testing 9");
const struct ListMenuItem custom_sListMenuItems_HeightTest[] = {
	{ custom_gText_BerryFixProgram, 0 },
	{ custom_gText_MysteryEvent,    1 },
	{ custom_gText_MysteryGift,     2 },
	{ custom_gText_Test4,			3 },
	{ custom_gText_Test5,			4 },
	{ custom_gText_Test6,			5 },
	{ custom_gText_Test7,			6 }, 
	{ custom_gText_Test8,			7 },
	{ custom_gText_Test9,			8 }
};



const u8 custom_gText_OfficialEvents[] = _("Official Events");
const u8 custom_gText_CustomEvents[] = _("Custom Events");
const struct ListMenuItem custom_sListMenuItems_EventCategoryMenu[] = {
	{ custom_gText_OfficialEvents,	0 },
	{ custom_gText_CustomEvents,	1 }
};



const u8 custom_gText_OfficialGifts[] = _("Official Gifts");
const u8 custom_gText_CustomGifts[] = _("Custom Gifts");
const struct ListMenuItem custom_sListMenuItems_GiftCategoryMenu[] = {
	{ custom_gText_OfficialGifts,	0 },
	{ custom_gText_CustomGifts,		1 }
};


#endif
