#ifdef GUARD_CUSTOM_CODE_C

u32 CreateShortCode(u8* gameCode, u8 revision) {
	u8 ascii0 = gameCode[0] & 0x7F;
	u8 ascii1 = gameCode[1] & 0x7F;
	u8 ascii2 = gameCode[2] & 0x7F;
	
	u8 language = 0;
	switch (gameCode[3]) {
		case 'J': language = LANGUAGE_JAPANESE; break;
		case 'E': language = LANGUAGE_ENGLISH; break;
		case 'F': language = LANGUAGE_FRENCH; break;
		case 'I': language = LANGUAGE_ITALIAN; break;
		case 'D': language = LANGUAGE_GERMAN; break;
		case 'S': language = LANGUAGE_SPANISH; break;
	}
	
	u32 ret = (ascii0 << (32-7)) | (ascii1 << (32-14)) | (ascii2 << (32-21)) | (language << 8) | revision;
	return ret;
}

#endif
