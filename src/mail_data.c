#include "global.h"
#include "gflib.h"
#include "mail.h"
#include "mail_data.h"
#include "constants/items.h"
#include "pokemon_icon.h"

u16 MailSpeciesToSpecies(u16 mailSpecies, u16 *unownLetter)
{
    u16 result;

    if (mailSpecies >= 30000 && mailSpecies < (30000 + UNOWN_FORM_COUNT))
    {
        result = SPECIES_UNOWN;
        *unownLetter = mailSpecies - 30000;
    }
    else
    {
        result = mailSpecies;
    }
    return result;
}

