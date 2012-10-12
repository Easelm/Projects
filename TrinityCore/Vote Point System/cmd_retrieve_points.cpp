/* 
    Retrieving Vote Points
	By, QQrofl
*/
#include "ScriptPCH.h"
#include "VotePoints.h"

class cmd_vote_points : public CommandScript
{
   public:
	   cmd_vote_points() : CommandScript("cmd_vote_points") { }

	   ChatCommand * GetCommands() const
	   {
		   static ChatCommand pointCommandTable[] =
		   {
			   { "getpoints",      SEC_PLAYER,         false, &HandleGetPointsCommand,           "", NULL }, 
			   { NULL,             0,                  false, NULL,                              "", NULL }
		   };

		   static ChatCommand commandTable[]  =
		   {
			   { "vote",           SEC_PLAYER,         true,  NULL,                              "", pointCommandTable }, 
			   { NULL,             0,                  false, NULL,                              "", NULL }
		   };
		   return commandTable;
	   }

	   static bool HandleGetPointsCommand(ChatHandler * handler, char const * args)
	   {
		   Player * player = handler->GetSession()->GetPlayer();
		   VotePointSystem * voteSystem = sVoteSystemMgr->GetPointsByAccountId(player->GetSession()->GetAccountId());
		   ChatHandler(player).PSendSysMessage("You have %u vote points", voteSystem->Points());
		   return true;
	   }
};

void AddSC_vote_points()
{
	new cmd_vote_points;
}