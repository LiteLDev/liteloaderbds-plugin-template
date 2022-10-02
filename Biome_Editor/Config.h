#pragma once
#include <string>
#include <Nlohmann/json.hpp>
#include <fstream>
#include <LoggerAPI.h>
#include "Version.h"

Logger logger(PLUGIN_NAME);
class Config
{
public:
	string dataPath;
public:
	void load_default() {
		dataPath = "plugins/WheatBuilder/";
	};
	int load(string filePath) {
		nlohmann::json configJson;
		std::ifstream configFile;
		configFile.open(filePath);
		if (configFile.is_open()) {
			std::istreambuf_iterator<char> beg(configFile), end;
			string strFileData = std::string(beg, end);
			configFile.seekg(0, std::ios::end);//移动的文件尾部
			configFile.close();
			try
			{
				configJson = json::parse(strFileData.c_str(), nullptr, false);
				if (configJson.is_discarded())
				{
					logger.fatal("Parse config json data failed.");
					return -1;
				}
				dataPath = configJson["dataPath"].get<std::string>();
				return 1;
			}
			catch (std::exception& e)
			{
				logger.fatal(fmt::format("Failed to load config: {}", e.what()));
				return -1;
			}
		}
		else {
			load_default();
			configJson = {
				{"dataPath", dataPath}
			};

			std::ofstream configFileOut;
			configFileOut.open(filePath);
			configFileOut << configJson.dump();
			return 1;
		}

	}
};
Config config;