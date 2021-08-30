#include "global.h"
#include "battle_tower.h"
#include "random.h"
#include "text.h"
#include "event_data.h"
#include "string_util.h"
#include "battle_message.h"
#include "data.h"
#include "pokedex.h"
#include "overworld.h"
#include "battle_transition.h"
#include "easy_chat.h"
#include "battle_setup.h"
#include "battle_main.h"
#include "task.h"
#include "battle.h"
#include "party_menu.h"
#include "new_game.h"
#include "save.h"
#include "item.h"
#include "script_pokemon_util.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/event_objects.h"

static EWRAM_DATA u16 sSpecialVar_0x8004_Copy = 0;

#define TakeBravoTrainerBattleTowerOffTheAir()
#define ewram160FB (*((u8 *)&gBattleStruct->field_DA))

static void ResetBattleTowerStreak(u8 levelType);
static void SetBattleTowerTrainerGfxId(u8 trainerClass);
static void CheckMonBattleTowerBanlist(u16 species, u16 heldItem, u16 hp, u8 battleTowerLevelType, u8 monLevel, u16 *validPartySpecies, u16 *validPartyHeldItems, u8 *numValid);
static void SaveCurrentWinStreak(void);
static void ValidateBattleTowerRecordChecksums(void);
static void SetBattleTowerRecordChecksum(struct BattleTowerRecord * record);
static void ClearBattleTowerRecord(struct BattleTowerRecord * record);
static void PopulateBravoTrainerBattleTowerLostData(void);
static u16 GetCurrentBattleTowerWinStreak(u8 levelType);
static void SetEReaderTrainerChecksum(struct BattleTowerEReaderTrainer * eReaderTrainer);
static void PrintEReaderTrainerFarewellMessage(void);

const u8 unref_83FFAAC[] = {
    0x05, 0x04, 0x01, 0x10, 0x04, 0x02, 0x05, 0x06,
    0x03, 0x10, 0x06, 0x04, 0x00, 0x01, 0x02, 0x03,
    0x00, 0x02, 0x03
};

const u8 unref_83FFABF[] = _("100");

#include "data/battle_tower/trainers.h"

static const u16 sBattleTowerHeldItems[] = {
    ITEM_NONE,
    ITEM_KINGS_ROCK,
    ITEM_SITRUS_BERRY,
    ITEM_ORAN_BERRY,
    ITEM_CHESTO_BERRY,
    ITEM_HARD_STONE,
    ITEM_FOCUS_BAND,
    ITEM_PERSIM_BERRY,
    ITEM_MIRACLE_SEED,
    ITEM_BERRY_JUICE,
    ITEM_MACHO_BRACE,
    ITEM_SILVER_POWDER,
    ITEM_CHERI_BERRY,
    ITEM_BLACK_GLASSES,
    ITEM_BLACK_BELT,
    ITEM_SOUL_DEW,
    ITEM_CHOICE_BAND,
    ITEM_MAGNET,
    ITEM_SILK_SCARF,
    ITEM_WHITE_HERB,
    ITEM_DEEP_SEA_SCALE,
    ITEM_DEEP_SEA_TOOTH,
    ITEM_MYSTIC_WATER,
    ITEM_SHARP_BEAK,
    ITEM_QUICK_CLAW,
    ITEM_LEFTOVERS,
    ITEM_RAWST_BERRY,
    ITEM_LIGHT_BALL,
    ITEM_POISON_BARB,
    ITEM_NEVER_MELT_ICE,
    ITEM_ASPEAR_BERRY,
    ITEM_SPELL_TAG,
    ITEM_BRIGHT_POWDER,
    ITEM_LEPPA_BERRY,
    ITEM_SCOPE_LENS,
    ITEM_TWISTED_SPOON,
    ITEM_METAL_COAT,
    ITEM_MENTAL_HERB,
    ITEM_CHARCOAL,
    ITEM_PECHA_BERRY,
    ITEM_SOFT_SAND,
    ITEM_LUM_BERRY,
    ITEM_DRAGON_SCALE,
    ITEM_DRAGON_FANG,
    ITEM_IAPAPA_BERRY,
    ITEM_WIKI_BERRY,
    ITEM_SEA_INCENSE,
    ITEM_SHELL_BELL,
    ITEM_SALAC_BERRY,
    ITEM_LANSAT_BERRY,
    ITEM_APICOT_BERRY,
    ITEM_STARF_BERRY,
    ITEM_LIECHI_BERRY,
    ITEM_STICK,
    ITEM_LAX_INCENSE,
    ITEM_AGUAV_BERRY,
    ITEM_FIGY_BERRY,
    ITEM_THICK_CLUB,
    ITEM_MAGO_BERRY,
    ITEM_METAL_POWDER,
    ITEM_PETAYA_BERRY,
    ITEM_LUCKY_PUNCH,
    ITEM_GANLON_BERRY
};

#include "data/battle_tower/level_50_mons.h"
#include "data/battle_tower/level_100_mons.h"

static const u8 sMaleTrainerClasses[] =
{

};

static const u8 sFemaleTrainerClasses[] =
{

};

static const u8 sMaleTrainerGfx[] =
{

};

static const u8 sFemaleTrainerGfx[] =
{

};

const u16 gBattleTowerBannedSpecies[] = {
    SPECIES_MEW,
    SPECIES_MEWTWO,
    SPECIES_HO_OH,
    SPECIES_LUGIA,
    SPECIES_CELEBI,
    SPECIES_KYOGRE,
    SPECIES_GROUDON,
    SPECIES_RAYQUAZA,
    SPECIES_JIRACHI,
    SPECIES_DEOXYS,
    0xFFFF
};

static const u16 sShortStreakPrizes[] = {
    ITEM_HP_UP,
    ITEM_PROTEIN,
    ITEM_IRON,
    ITEM_CALCIUM,
    ITEM_CARBOS,
    ITEM_ZINC
};

static const u16 sLongStreakPrizes[] = {
    ITEM_BRIGHT_POWDER,
    ITEM_WHITE_HERB,
    ITEM_QUICK_CLAW,
    ITEM_LEFTOVERS,
    ITEM_MENTAL_HERB,
    ITEM_KINGS_ROCK,
    ITEM_FOCUS_BAND,
    ITEM_SCOPE_LENS,
    ITEM_CHOICE_BAND
};

// Checks if the saved E-Reader trainer is valid.
void ValidateEReaderTrainer(void)
{
    u32 i;
    u32 checksum;
    struct BattleTowerEReaderTrainer *ereaderTrainer;

    gSpecialVar_Result = 0;
    ereaderTrainer = &gSaveBlock2Ptr->battleTower.ereaderTrainer;

    checksum = 0;
    for (i = 0; i < sizeof(struct BattleTowerEReaderTrainer) / sizeof(u32) - 1; i++)
        checksum |= ((u32 *)ereaderTrainer)[i];

    if (checksum == 0)
    {
        gSpecialVar_Result = 1;
        return;
    }

    checksum = 0;
    for (i = 0; i < sizeof(struct BattleTowerEReaderTrainer) / sizeof(u32) - 1; i++)
        checksum += ((u32 *)ereaderTrainer)[i];

    if (gSaveBlock2Ptr->battleTower.ereaderTrainer.checksum != checksum)
    {
        ClearEReaderTrainer(&gSaveBlock2Ptr->battleTower.ereaderTrainer);
        gSpecialVar_Result = 1;
    }
}

static void SetEReaderTrainerChecksum(struct BattleTowerEReaderTrainer *ereaderTrainer)
{
    s32 i;

    ereaderTrainer->checksum = 0;
    for (i = 0; i < sizeof(struct BattleTowerEReaderTrainer) / sizeof(u32) - 1; i++)
        ereaderTrainer->checksum += ((u32 *)ereaderTrainer)[i];
}

void ClearEReaderTrainer(struct BattleTowerEReaderTrainer *ereaderTrainer)
{
    u32 i;

    for (i = 0; i < sizeof(struct BattleTowerEReaderTrainer) / sizeof(u32); i++)
        ((u32 *)ereaderTrainer)[i] = 0;
}

