#ifndef GUARD_CUSTOM_LIST_MENU_H
#define GUARD_CUSTOM_LIST_MENU_H

#include "global.h"

u32 CreateAndPollListMenu(const struct ListMenuItem* items, u8 totalItems);
void SetListMenuWidth(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate);
void SetListMenuHeight(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate);

#endif

