#ifdef GUARD_CUSTOM_CODE_C

inline u8 inline_getAscii0(u32 shortCode) {
	return (shortCode >> (32-ASCII_WIDTH)) & ASCII_BITS;
}
inline u8 inline_getAscii1(u32 shortCode) {
	return (shortCode >> (32-2*ASCII_WIDTH)) & ASCII_BITS;
}
inline u8 inline_getAscii2(u32 shortCode) {
	return (shortCode >> (32-3*ASCII_WIDTH)) & ASCII_BITS;
}
inline u8 inline_getLanguage(u32 shortCode) {
	return (shortCode >> REVISION_WIDTH) & LANGUAGE_BITS;
}
inline u8 inline_getRevision(u32 shortCode) {
	return shortCode & REVISION_BITS;
}

//=============================================================//


bool32 CompareShortCodes(u32 clientShortCode, u32 queryShortCode) {
	u8 queryAscii0 = inline_getAscii0(queryShortCode);
	u8 clientAscii0 = inline_getAscii0(clientShortCode);
	if (queryAscii0) if (queryAscii0 != clientAscii0) return FALSE;
	
	u8 queryAscii1 = inline_getAscii1(queryShortCode);
	u8 clientAscii1 = inline_getAscii1(clientShortCode);
	if (queryAscii1) if (queryAscii1 != clientAscii1) return FALSE;
	
	u8 queryAscii2 = inline_getAscii2(queryShortCode);
	u8 clientAscii2 = inline_getAscii2(clientShortCode);
	if (queryAscii2) if (queryAscii2 != clientAscii2) return FALSE;
	
	u8 queryLanguage = inline_getLanguage(queryShortCode);
	u8 clientLanguage = inline_getLanguage(clientShortCode);
	if (queryLanguage) if (queryLanguage != clientLanguage) return FALSE;
	
	u8 queryRevision = inline_getRevision(queryShortCode);
	u8 clientRevision = inline_getRevision(clientShortCode);
	if (queryRevision != REVISION_BITS) if (queryRevision != clientRevision) return FALSE;
	
	return TRUE;
}

#endif
