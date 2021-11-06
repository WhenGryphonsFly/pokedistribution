#ifndef GUARD_CUSTOM_SERVER_CMDS_H
#define GUARD_CUSTOM_SERVER_CMDS_H

#define SERVER_RETURN(x)                              {.instr =  0, .flag = x}
#define SERVER_WAIT_FOR_SEND_TO_FINISH                {.instr =  1}
#define SERVER_WAIT_FOR_RECEIVE_WITH_MAGIC_NUMBER(x)  {.instr =  2, .flag = x}
#define SERVER_BRANCH_ALWAYS(y)                       {.instr =  3, .parameter = y}
/* 
	Results of server commands (unlisted if result is unmodified)
	
	SERVER_RETURN(x)
		x
	SERVER_VALIDATE_CLIENT_VERSION
		[[???]]
	SERVER_CHECK_IF_CLIENT_HAS_BUFFERED_CARD
	SERVER_CHECK_IF_CLIENT_HAS_CARD_AT(y)
		0, if the client has no card
		1, if the client has a card with the same ID as the server's card
		2, if the client has a card with a different ID as the server's card
	SERVER_READ_WORD_FROM_CLIENT
		The first 32 bits from the receive buffer
	SERVER_CAN_CLIENT_RECEIVE_STICKER_FROM_BUFFERED_WORD
	SERVER_CAN_CLIENT_RECEIVE_STICKER_AT(y)
		1, if the client has no spaces left
		2, if the client has one space left and can receive the sticker
		3, if the client has at least one space left but already has a sticker with the same icon and ID
		4, if the client has more than one space left and can receive the sticker
*/
#define SERVER_BRANCH_IF_RESULT_WAS(x, y)             {.instr =  4, .flag = x, .parameter = y}
#define SERVER_PREPARE_TO_VALIDATE_CLIENT_VERSION     {.instr =  5}
#define SERVER_VALIDATE_CLIENT_VERSION                {.instr =  6}
#define SERVER_CHECK_IF_CLIENT_HAS_BUFFERED_CARD      {.instr =  7, .parameter = 0}
#define SERVER_CHECK_IF_CLIENT_HAS_CARD_AT(y)                {.instr =  7, .parameter = y}
#define SERVER_READ_WORD_FROM_CLIENT                         {.instr =  8}
#define SERVER_CAN_CLIENT_RECEIVE_STICKER_FROM_BUFFERED_WORD {.instr =  9, .parameter = 0}
#define SERVER_CAN_CLIENT_RECEIVE_STICKER_AT(y)              {.instr =  9, .parameter = y}
#define SERVER_GET_CLIENT_WINS
#define SERVER_GET_CLIENT_LOSSES
#define SERVER_GET_CLIENT_TRADES
#define SERVER_GET_CLIENT_STICKERS_RECEIVED
#define SERVER_GET_CLIENT_STICKERS_MAX

/*
    Magic numbers used by server and client commands

*/


#endif//GUARD_CUSTOM_SERVER_CMDS_H
