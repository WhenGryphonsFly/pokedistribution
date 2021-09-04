#ifndef GUARD_AAA_MAIN_H
#define GUARD_AAA_MAIN_H

#include "global.h"

void CB2_RootMenu(void);
static void VBlankCB_RootMenu(void);

void CB2_MainCodePath(void);
static void MainCodePathInit(void);
static void Task_MakeBackgroundBlue(u8 taskId);
static void Task_RenderRootMenu(u8 taskId);

#endif//GUARD_AAA_MAIN_H
