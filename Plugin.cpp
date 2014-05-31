#include <string>

#include <steamclientpublic.h>

#include "Plugin.hpp"

namespace
{
	GlobalApprehensivePlugin plugin;

	char const * playerDeathEvent = "player_death";
}

PLUGIN_EXPOSE(GlobalApprehensivePlugin, plugin);

SH_DECL_HOOK2(IGameEventManager2, FireEvent, SH_NOATTRIB, 0, bool, IGameEvent *, bool);

bool GlobalApprehensivePlugin::Load(PluginId id, ISmmAPI * ismm, char * error, std::size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetEngineFactory, _engineServer, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetEngineFactory, _gameEventManager, IGameEventManager2, INTERFACEVERSION_GAMEEVENTSMANAGER2);
	GET_V_IFACE_ANY(GetServerFactory, _playerInfoManager, IPlayerInfoManager, INTERFACEVERSION_PLAYERINFOMANAGER);
	_gameEventManager->AddListener(this, playerDeathEvent, true);
	_globalVars = g_SMAPI->GetCGlobals();
	return true;
}

bool GlobalApprehensivePlugin::Unload(char * error, std::size_t maxlen)
{
	_gameEventManager->RemoveListener(this);
	return true;
}

char const * GlobalApprehensivePlugin::GetAuthor()
{
	return "";
}

char const * GlobalApprehensivePlugin::GetName()
{
	return "GlobalApprehensive";
}

char const * GlobalApprehensivePlugin::GetDescription()
{
	return "Global Offensive statistics plugin";
}

char const * GlobalApprehensivePlugin::GetURL()
{
	return "";
}

char const * GlobalApprehensivePlugin::GetLicense()
{
	return "";
}

char const * GlobalApprehensivePlugin::GetVersion()
{
	return "0";
}

char const * GlobalApprehensivePlugin::GetDate()
{
	return __DATE__;
}

char const * GlobalApprehensivePlugin::GetLogTag()
{
	return "GA";
}

void GlobalApprehensivePlugin::FireGameEvent(IGameEvent * event)
{
	if (event == NULL)
		return;
	std::string name(event->GetName());
	if (name == playerDeathEvent)
	{
		int attackerId = event->GetInt("attacker");
		std::string attackerName = GetPlayerName(attackerId);
		uint32 attackerAccountId = GetAccountID(attackerId);
		int assisterId = event->GetInt("assister");
		int victimId = event->GetInt("userid");
		std::string victimName = GetPlayerName(victimId);
		uint32 victimAccountId = GetAccountID(victimId);
		std::string attackerWeapon(event->GetString("weapon"));
		bool headshot = event->GetBool("headshot");
		META_CONPRINTF("%s (%d) killed %s (%d) with weapon %s\n", attackerName.c_str(), attackerAccountId, victimName.c_str(), victimAccountId, attackerWeapon.c_str());
	}
}

int GlobalApprehensivePlugin::GetEventDebugID()
{
	return EVENT_DEBUG_ID_INIT;
}

edict_t * GlobalApprehensivePlugin::GetEntity(int index)
{
	if (!(index >= 0 && index < MAX_EDICTS))
		return NULL;
	if (_globalVars->pEdicts)
	{
		edict_t * enitity = _globalVars->pEdicts + index;
		if (enitity->IsFree())
			return NULL;
		return enitity;
	}
	return NULL;
}

int GlobalApprehensivePlugin::GetEntityIndexFromUserId(int userId)
{
	for (int i = 1; i <= _globalVars->maxClients; ++i)
	{
		edict_t * player = GetEntity(i);
		if (_engineServer->GetPlayerUserId(player) == userId)
			return i;
	}
	return 0;
}

std::string GlobalApprehensivePlugin::GetPlayerName(int userId)
{
	int index = GetEntityIndexFromUserId(userId);
	edict_t * player = GetEntity(index);
	std::string name(_playerInfoManager->GetPlayerInfo(player)->GetName());
	return name;
}

uint32 GlobalApprehensivePlugin::GetAccountID(int userId)
{
	int index = GetEntityIndexFromUserId(userId);
	edict_t * player = GetEntity(index);
	if (_playerInfoManager->GetPlayerInfo(player)->IsFakeClient())
		return 0;
	CSteamID const * steamId = _engineServer->GetClientSteamID(player);
	if (steamId == NULL)
		return 0;
	return steamId->GetAccountID();
}