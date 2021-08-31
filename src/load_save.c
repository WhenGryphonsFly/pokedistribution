//[[!!!]]
#include "global.h"
#include "gflib.h"
#include "gba/flash_internal.h"
#include "load_save.h"
#include "pokemon.h"
#include "random.h"
#include "item.h"
#include "save_location.h"
#include "berry_powder.h"
#include "overworld.h"
#include "quest_log.h"

#define SAVEBLOCK_MOVE_RANGE    128

// EWRAM DATA
EWRAM_DATA struct SaveBlock2 gSaveBlock2 = {0};
EWRAM_DATA struct SaveBlock1 gSaveBlock1 = {0};

// IWRAM common
struct SaveBlock1 *gSaveBlock1Ptr;
struct SaveBlock2 *gSaveBlock2Ptr;

