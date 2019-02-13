#include "GameSettings.h"

#include <fstream>
#include <string>
#include <algorithm>

#include "cocos2d.h"

namespace Retry
{

std::unordered_map<std::string, bool> Config::settings;

bool Config::loadSettingsFromFile()
{
	// TODO: Change to search nearby directories for the config directory
	std::string configDirectory = cocos2d::FileUtils::getInstance()->getWritablePath() + "\\Config";
	std::string configPath = configDirectory + "\\config.txt";
	std::ifstream config(configPath);

	if (!config.is_open())
	{
		if (!cocos2d::FileUtils::getInstance()->isDirectoryExist(configDirectory))
			cocos2d::FileUtils::getInstance()->createDirectory(configDirectory);

		std::ofstream newFile(configPath);
		newFile << "debug 0\nscreenshake 1\nvibration 1";
		newFile.close();
		config.open(configPath);
	}

	std::string settingName;
	char c;
	while (config.get(c))
	{
		if (c == ' ')
		{
			settings[settingName] = config.get() - 48;
			settingName.clear();
			std::getline(config, std::string());
			config.get(c);
		}
		settingName += c;
	}
	return true;
}

bool Config::saveSettingsToFile()
{
	std::fstream config(cocos2d::FileUtils::getInstance()->getWritablePath() + "\\Config\\config.txt");

	if (config.is_open())
	{
		std::string settingName;
		char c;
		while (config.get(c))
		{
			if (c == ' ')
			{
				config.seekg(config.tellp());
				config.put('0' + (char) settings[settingName]);
				config.seekp(config.tellg());
				settingName.clear();
				std::getline(config, std::string());
				config.get(c);
			}
			settingName += c;
		}
		return true;
	}
	return false;
}

std::string formatForSettings(std::string s)
{
	s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
	for (auto &i : s) i |= 32;
	return s;
}

bool Config::isSetting(const std::string &s)
{
	return settings.find(formatForSettings(s)) != settings.end();
}

bool Config::doSetting(const std::string & s)
{
	return isSetting(s) && settings[formatForSettings(s)];
}

void Config::setSetting(const std::string & s, const bool & b)
{
	if (!isSetting(s)) return;
	settings[formatForSettings(s)] = b;
}

void Config::toggleSetting(const std::string & s)
{
	std::string se = formatForSettings(s);
	settings[se] = !settings[se];
}

}