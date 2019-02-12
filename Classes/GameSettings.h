#ifndef GAME_SETTINGS
#define GAME_SETTINGS

#include <unordered_map>

namespace Retry
{

class Config abstract
{
public:

	static bool loadSettingsFromFile();

	static bool saveSettingsToFile();

	static bool isSetting(const std::string &s);
	static bool doSetting(const std::string &s);
	static void setSetting(const std::string &s, const bool &b);
	static void toggleSetting(const std::string &s);

	static bool doDebug() { return settings["debug"]; }
	static void setDebug(const bool &b) { settings["debug"] = b; }
	static void toggleDebug() { settings["debug"] = !settings["debug"]; }

	static bool doScreenShake() { return settings["screenshake"]; }
	static void setScreenShake(const bool &b) { settings["screenshake"] = b; }
	static void toggleScreenShake() { settings["screenshake"] = !settings["screenshake"]; }

	static bool doVibration() { return settings["vibration"]; }
	static void setVibration(const bool &b) { settings["vibration"] = b; }
	static void toggleVibration() { settings["vibration"] = !settings["vibration"]; }

private:

	static std::unordered_map<std::string, bool> settings;

};


}


#endif