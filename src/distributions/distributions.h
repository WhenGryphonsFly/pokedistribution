#ifndef GUARD_CUSTOM_DISTRIBUTIONS_H
#define GUARD_CUSTOM_DISTRIBUTIONS_H

struct MysteryGiftDistributionData {
	u8 name[40];
	u8 hasBothCardAndNews:1;
	u8 showNews:1;
	u16 pkmnStamps[7];
	
	struct MEWonderCardData wonderCard;
	struct MEWonderNewsData wonderNews;
	
	struct mevent_server_cmd* serverCommands;
};


/*#define OFFICIAL_EVENT_COUNT 0
extern const struct ListMenuItem custom_sListMenuItems_OE_Menu[OFFICIAL_MYSTERY_EVENT_COUNT];
// [[TODO]] Mystery Event list
// [[TODO]] CE, OG*/

#define CUSTOM_GIFT_COUNT 1
extern struct ListMenuItem custom_sListMenuItems_cGiftMenu[CUSTOM_GIFT_COUNT];
extern struct MysteryGiftDistributionData custom_cGiftData[CUSTOM_GIFT_COUNT];

#endif
