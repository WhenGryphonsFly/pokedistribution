#ifndef GUARD_CUSTOM_CONSTANTS_H
#define GUARD_CUSTOM_CONSTANTS_H

extern const struct ListMenuTemplate custom_sListMenuTemplate_Base;
extern const struct WindowTemplate custom_sWindowTemplate_Base;
extern const u8 custom_gText_NotImplemented[];

extern const struct ListMenuItem custom_sListMenuItems_RootMenu[3];
extern const struct ListMenuItem custom_sListMenuItems_HeightTest[9];
extern const struct ListMenuItem custom_sListMenuItems_EventCategoryMenu[2];
extern const struct ListMenuItem custom_sListMenuItems_GiftCategoryMenu[2];

/*#define OFFICIAL_EVENT_COUNT 0
extern const struct ListMenuItem custom_sListMenuItems_OE_Menu[OFFICIAL_MYSTERY_EVENT_COUNT];
// [[TODO]] Mystery Event list
// [[TODO]] CE, OG*/
#define CUSTOM_GIFT_COUNT 1
extern const struct ListMenuItem custom_sListMenuItems_CG_Menu[CUSTOM_GIFT_COUNT];

#endif
