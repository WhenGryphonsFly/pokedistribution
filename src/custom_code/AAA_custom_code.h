#ifndef GUARD_AAA_CUSTOM_CODE_H
#define GUARD_AAA_CUSTOM_CODE_H

#include "global.h"

// callbacks.c
void CB2_RootMenu(void);
static void VBlankCB_RootMenu(void);

// root_menu.c
void CB2_MainCodePath(void);
static void MainCodePathInit(void);
static void Task_MakeBackgroundBlue(u8 taskId);
static void Task_RenderRootMenu(u8 taskId);

// utility_functions.c
u32 DrawListMenu(const struct ListMenuItem* listMenuItems, u32 listMenuLength, u32 listMenuMax);
void SetListMenuWidth(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate);
void SetListMenuHeight(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate);
u16 InitializeListMenu(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate);
u32 GetListMenuResponse(u32 windowAndTaskId);
void DestroyListMenu(u32 windowAndtaskId);

#endif//GUARD_AAA_CUSTOM_CODE_H
