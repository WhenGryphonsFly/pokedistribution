#include "AAA_custom_code.h"

#define GUARD_CUSTOM_CODE_C
#include "custom_code/constants.c"
#include "custom_code/custom_list_menu.c"
#include "custom_code/custom_mystery_gift.c"


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


