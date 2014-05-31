#pragma once

#include <ISmmPlugin.h>
#include <igameevents.h>
#include <iplayerinfo.h>

PLUGIN_GLOBALVARS();

class GlobalApprehensivePlugin : public ISmmPlugin, public IGameEventListener2
{
public:
	// ISmmPlugin
	bool Load(PluginId id, ISmmAPI * ismm, char * error, std::size_t maxlength, bool late);
	bool Unload(char * error, std::size_t maxlen);
	char const * GetAuthor();
	char const * GetName();
	char const * GetDescription();
	char const * GetURL();
	char const * GetLicense();
	char const * GetVersion();
	char const * GetDate();
	char const * GetLogTag();

	// IGameEventListener2
	void FireGameEvent(IGameEvent * event);
	int	GetEventDebugID();

private:
	CGlobalVars * _globalVars;
	IVEngineServer * _engineServer;
	IPlayerInfoManager * _playerInfoManager;
	IGameEventManager2 * _gameEventManager;

	edict_t * GetEntity(int index);
	int GetEntityIndexFromUserId(int userId);
	std::string GetPlayerName(int userId);
	uint32 GetAccountID(int userId);
};