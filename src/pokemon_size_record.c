#include "global.h"
#include "event_data.h"

static const u8 sGiftRibbonsMonDataIds[] =
{
    MON_DATA_MARINE_RIBBON, MON_DATA_LAND_RIBBON, MON_DATA_SKY_RIBBON,
    MON_DATA_COUNTRY_RIBBON, MON_DATA_NATIONAL_RIBBON, MON_DATA_EARTH_RIBBON,
    MON_DATA_WORLD_RIBBON
};

void GiveGiftRibbonToParty(u8 index, u8 ribbonId)
{
    s32 i;
    bool32 gotRibbon = FALSE;
    u8 data = 1;
    u8 array[8];
    memcpy(array, sGiftRibbonsMonDataIds, sizeof(sGiftRibbonsMonDataIds));

    if (index < 11 && ribbonId < 65)
    {
        gSaveBlock1Ptr->giftRibbons[index] = ribbonId;
        for (i = 0; i < PARTY_SIZE; i++)
        {
            struct Pokemon * mon = &gPlayerParty[i];

            if (GetMonData(mon, MON_DATA_SPECIES) != SPECIES_NONE && !GetMonData(mon, MON_DATA_SANITY_IS_EGG))
            {
                SetMonData(mon, array[index], &data);
                gotRibbon = TRUE;
            }
        }
        if (gotRibbon)
            FlagSet(FLAG_SYS_RIBBON_GET);
    }
}
