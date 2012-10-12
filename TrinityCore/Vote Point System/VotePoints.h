/* 
    Retrieving Vote Points
	By, QQrofl
*/
#include <string>
#include <ace/Singleton.h>
using namespace std;

#include "Chat.h"

class VotePointSystem
{
   public:
	   VotePointSystem();
	   explicit VotePointSystem(const Player * player, uint32 points);
	   ~VotePointSystem();

	   bool GetPlayerAccount(uint32 acctId) const { return acctId == accountId; }
	   bool LoadFromDB(Field * fields);

	   void SaveToDB(SQLTransaction& trans) const;
	   
	   uint32 Points() const { return votePoints; }
	   uint32 GetAccountId() const { return accountId; }
	   
	   

   private:
	   uint32 accountId;
	   uint32 votePoints;
};
typedef map<uint32, VotePointSystem*> VotePointList;

class VotePointSystemMgr
{
	friend class ACE_Singleton<VotePointSystemMgr, ACE_Null_Mutex>;
	
    public: 
		void LoadVoteSystem();

		VotePointSystem * GetPointsByAccountId(uint32 id)
		{
			for(VotePointList::const_iterator itr = _voteList.begin(); itr != _voteList.end(); ++itr)
				if(itr->second && itr->second->GetPlayerAccount(id))
					return itr->second;
			return NULL;
		}

		void SaveToList(VotePointSystem * voteSystem);

    protected:
		VotePointList _voteList;

};

#define sVoteSystemMgr ACE_Singleton<VotePointSystemMgr, ACE_Null_Mutex>::instance()