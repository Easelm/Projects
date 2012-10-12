/* 
    Retrieving Vote Points
	By, QQrofl
*/
#include "VotePoints.h"

VotePointSystem::VotePointSystem() { }

VotePointSystem::VotePointSystem(const Player * player, uint32 points)
{
	accountId = player->GetSession()->GetAccountId();
	votePoints = points;
}

VotePointSystem::~VotePointSystem() { }

bool VotePointSystem::LoadFromDB(Field * fields)
{
	uint8 index = 0;
	accountId = fields[index].GetUInt32();
	votePoints = fields[++index].GetUInt32();
	return true;
}

void VotePointSystem::SaveToDB(SQLTransaction& trans) const
{
	uint8 index = 0;
	PreparedStatement * stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_POINTS);
	PreparedStatement * _stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_POINTS);
	stmt->setUInt32(0, accountId);
	PreparedQueryResult result = LoginDatabase.Query(stmt);

	uint32 _votePoints;
	if(result)
	{
		do
		{
			Field * fields = result->Fetch();
			_votePoints = fields[0].GetUInt32();
			if(_votePoints < 1 || _votePoints < votePoints)
				return;
			else
				_votePoints -= votePoints;
		}while(result->NextRow());
	}
	_stmt->setUInt32(index, _votePoints);
	_stmt->setUInt32(index++, accountId);
	CharacterDatabase.Execute(_stmt);
}

void VotePointSystemMgr::LoadVoteSystem()
{
	uint32 oldMSTime = getMSTime();

	for (VotePointList::const_iterator itr = _voteList.begin(); itr != _voteList.end(); ++itr)
		delete itr->second;
	_voteList.clear();

	PreparedStatement * stmt = LoginDatabase.GetPreparedStatement(LOGIN_LOAD_POINTS);
	PreparedQueryResult result = LoginDatabase.Query(stmt);
	if(!result)
	{
		sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 Account Vote Points Data");
		return;
	}

	uint32 count = 0;
	do
	{
		Field * fields = result->Fetch();
		VotePointSystem * voteSystem = new VotePointSystem();
		if(!voteSystem->LoadFromDB(fields))
		{
			delete voteSystem;
			continue;
		}
		_voteList[voteSystem->GetAccountId()] = voteSystem;
		++count;
	}while(result->NextRow());
	sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u Player Vote Data in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void VotePointSystemMgr::SaveToList(VotePointSystem * voteSystem)
{
	_voteList[voteSystem->GetAccountId()] = voteSystem;
	SQLTransaction trans = SQLTransaction(NULL);
	voteSystem->SaveToDB(trans);
}





