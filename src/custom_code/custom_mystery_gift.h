#ifndef GUARD_CUSTOM_MYSTERY_GIFT_H
#define GUARD_CUSTOM_MYSTERY_GIFT_H

#include "global.h"

bool32 CompareShortCodes(u32 clientShortCode, u32 queryShortCode);

#define ASCII_WIDTH 7
#define ASCII_BITS 0x7F
#define REVISION_WIDTH 8
#define REVISION_BITS 0xFF
#define LANGUAGE_BITS 0x7

#define SC_LANGUAGE(language) (( \
	(language == 'J') ? LANGUAGE_JAPANESE : ( \
	(language == 'E') ? LANGUAGE_ENGLISH : ( \
	(language == 'F') ? LANGUAGE_FRENCH : ( \
	(language == 'I') ? LANGUAGE_ITALIAN : ( \
	(language == 'D') ? LANGUAGE_GERMAN : ( \
	(language == 'S') ? LANGUAGE_SPANISH :  \
	0 ))))) \
) & LANGUAGE_BITS)


#define SHORT_CODE(gameCode, revision) ( \
	((gameCode[0] & ASCII_BITS) << (32-ASCII_WIDTH)) | \
	((gameCode[1] & ASCII_BITS) << (32-2*ASCII_WIDTH)) | \
	((gameCode[2] & ASCII_BITS) << (32-3*ASCII_WIDTH)) | \
	((SC_LANGUAGE(gameCode[3])) << REVISION_WIDTH) | \
	revision \
)

#endif//GUARD_CUSTOM_MYSTERY_GIFT_H
