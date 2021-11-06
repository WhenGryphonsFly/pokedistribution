#include "global.h"
#include "gflib.h"
#include "scanline_effect.h"
#include "task.h"
#include "save.h"
#include "event_data.h"
#include "menu.h"
#include "link.h"
#include "oak_speech.h"
#include "overworld.h"
#include "quest_log.h"
#include "mystery_gift_menu.h"
#include "strings.h"
#include "title_screen.h"
#include "help_system.h"
#include "pokedex.h"
#include "text_window.h"
#include "text_window_graphics.h"
#include "constants/songs.h"
#include "load_save.h"
#include "new_game.h"
#include "m4a.h"
#include "list_menu.h"
#include "mystery_gift_menu.h"
#include "new_menu_helpers.h"

#define GUARD_CUSTOM_CODE_C
#include "custom_code/constants.h"
#include "custom_code/custom_list_menu.h"
#include "custom_code/constants.c"
#include "custom_code/custom_list_menu.c"

#include "distributions/distributions.h"
#include "distributions/wonder_card_news.h"
#include "distributions/server_cmds.h"
#include "distributions/custom_gifts.c"
#undef GUARD_CUSTOM_CODE_C

/*
	Main Code Path:
	Step 1: Show three option menu: "Berry Fix Program (R/S)", "Mystery Event (R/S)", "Mystery Gift (FR/LG/E)"
	Step 2: If "Berry Fix Program (R/S)"
		Execute CB2_FadeOutTransitionToBerryFix (from title_screen.c)
		Should require no additional work
	Step 3: If "Mystery Event (R/S)"
		??? Emulate eReader output
	Step 4: If "Mystery Gift (FR/LG/E)"
*/

/*
	Regarding custom sripts:
	macros are not visible, and cannot be #included
	Variables (and presumably flags) are in scope. They are #included as a part of global.h
*/


