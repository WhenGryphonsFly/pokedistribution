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

struct LoadedSaveData
{
 struct ItemSlot items[BAG_ITEMS_COUNT];
 struct ItemSlot keyItems[BAG_KEYITEMS_COUNT];
 struct ItemSlot pokeBalls[BAG_POKEBALLS_COUNT];
 struct ItemSlot TMsHMs[BAG_TMHM_COUNT];
 struct ItemSlot berries[BAG_BERRIES_COUNT];
 struct MailStruct mail[MAIL_COUNT];
};

// EWRAM DATA
EWRAM_DATA struct SaveBlock2 gSaveBlock2 = {0};
EWRAM_DATA u8 gSaveBlock2_DMA[SAVEBLOCK_MOVE_RANGE] = {0};

EWRAM_DATA struct SaveBlock1 gSaveBlock1 = {0};
EWRAM_DATA u8 gSaveBlock1_DMA[SAVEBLOCK_MOVE_RANGE] = {0};

EWRAM_DATA struct PokemonStorage gPokemonStorage = {0};
EWRAM_DATA u8 gSaveBlock3_DMA[SAVEBLOCK_MOVE_RANGE] = {0};

EWRAM_DATA struct LoadedSaveData gLoadedSaveData = {0};
EWRAM_DATA u32 gLastEncryptionKey = 0;

// IWRAM common
bool32 gFlashMemoryPresent;
struct SaveBlock1 *gSaveBlock1Ptr;
struct SaveBlock2 *gSaveBlock2Ptr;
struct PokemonStorage *gPokemonStoragePtr;

