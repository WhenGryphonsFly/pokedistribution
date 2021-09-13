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

void SetListMenuWidth(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate);

#endif//GUARD_AAA_CUSTOM_CODE_H
