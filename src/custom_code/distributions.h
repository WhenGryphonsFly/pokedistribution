#ifndef GUARD_CUSTOM_DISTRIBUTIONS_H
#define GUARD_CUSTOM_DISTRIBUTIONS_H

struct MysteryGift600Data { 
	u8 data[1024];
};

struct MysteryGiftDistributionData {
	struct MEWonderCardData wc;
	struct MEWonderNewsData wn;
	struct MysteryGift600Data me[3];
};


/*#define OFFICIAL_EVENT_COUNT 0
extern const struct ListMenuItem custom_sListMenuItems_OE_Menu[OFFICIAL_MYSTERY_EVENT_COUNT];
// [[TODO]] Mystery Event list
// [[TODO]] CE, OG*/
#define CUSTOM_GIFT_COUNT 1
extern const struct ListMenuItem custom_sListMenuItems_CgMenu[CUSTOM_GIFT_COUNT];

#endif
