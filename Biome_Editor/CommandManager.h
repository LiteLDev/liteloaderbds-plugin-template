#pragma once
#include <RegCommandAPI.h>
#include <DynamicCommandAPI.h>
#include <MC/Types.hpp>
#include <MC/Player.hpp>
#include <MC/ServerPlayer.hpp>

#include "Config.h"
#include "BiomeManager.h"
#include "ChunkManager.h"

#include <fstream>

class CommandManager {
public:
    static void set() {
        biome();
        moveChunk();
        test();
    }
    static void biome() {
        using ParamType = DynamicCommand::ParameterType;
        //// create a dynamic command
        auto command = DynamicCommand::createCommand("be", "Biome Editor", CommandPermissionLevel::GameMasters);

        auto& optionsBiome = command->setEnum("BiomeOptions", { "biome" });
        auto& optionsBiomes = command->setEnum("BiomesOptions", { "biomes" });
        auto& optionsSetId = command->setEnum("SetOptionsId", { "set_id" });
        auto& optionsSetName = command->setEnum("SetOptionsName", { "set_name" });
        auto& optionsGet = command->setEnum("GetOptions", { "get" });
        auto& optionsSet = command->setEnum("SetOptions", { "set" });
        auto& optionsReplaceId = command->setEnum("ReplaceOptionsId", { "replace_id" });
        auto& optionsReplaceName = command->setEnum("ReplaceOptionsName", { "replace_name" });

        command->mandatory("opEnum1", ParamType::Enum, optionsBiome, CommandParameterOption::EnumAutocompleteExpansion);
        command->mandatory("opEnum1", ParamType::Enum, optionsBiomes, CommandParameterOption::EnumAutocompleteExpansion);
        command->mandatory("opEnum2", ParamType::Enum, optionsGet, CommandParameterOption::EnumAutocompleteExpansion);
        command->mandatory("opEnum2", ParamType::Enum, optionsSet, CommandParameterOption::EnumAutocompleteExpansion);
        command->mandatory("opEnum2", ParamType::Enum, optionsSetId, CommandParameterOption::EnumAutocompleteExpansion);
        command->mandatory("opEnum2", ParamType::Enum, optionsSetName, CommandParameterOption::EnumAutocompleteExpansion);
        command->mandatory("opEnum2", ParamType::Enum, optionsReplaceId, CommandParameterOption::EnumAutocompleteExpansion);
        command->mandatory("opEnum2", ParamType::Enum, optionsReplaceName, CommandParameterOption::EnumAutocompleteExpansion);
        command->mandatory("fileName", ParamType::String);
        command->mandatory("dimId", ParamType::Int);
        command->mandatory("originId", ParamType::Int);
        command->mandatory("originName", ParamType::String);
        command->mandatory("replaceId", ParamType::Int);
        command->mandatory("replaceName", ParamType::String);
        command->mandatory("biomeId", ParamType::Int);
        command->mandatory("biomeName", ParamType::String);
        command->mandatory("pos", ParamType::BlockPos);
        command->mandatory("startPos", ParamType::BlockPos);
        command->mandatory("endPos", ParamType::BlockPos);

        command->addOverload({ optionsBiome, optionsGet, "pos", "dimId" });
        command->addOverload({ optionsBiome, optionsSetId, "pos", "dimId", "biomeId" });
        command->addOverload({ optionsBiome, optionsSetName, "pos", "dimId", "biomeName" });

        // command->addOverload({ optionsBiomes,optionsSet, "pos", "dimId", "fileName" });
        // command->addOverload({ optionsBiomes,optionsGet, "startPos", "endPos", "dimId", "fileName" });
        command->addOverload({ optionsBiomes,optionsReplaceId, "startPos", "endPos", "dimId", "originId", "replaceId" });
        command->addOverload({ optionsBiomes,optionsReplaceName, "startPos", "endPos", "dimId", "originName", "replaceName" });

        command->setCallback([](DynamicCommand const& command, CommandOrigin const& origin, CommandOutput& output, std::unordered_map<std::string, DynamicCommand::Result>& results) {
            switch (do_hash(results["opEnum1"].get<std::string>().c_str()))
            {
            case do_hash("biome"):
                switch (do_hash(results["opEnum2"].get<std::string>().c_str()))
                {
                case do_hash("get"): {
                    BlockPos pos = results["pos"].get<BlockPos>();
                    int dimId = results["dimId"].get<int>();
                    auto res = BiomeManager::getbiomeIdAndName(pos.x, pos.y, pos.z, dimId);
                    output.success(fmt::format("id: {}, name: {}", res.first, res.second));
                    break;
                }
                case do_hash("set_id"): {
                    BlockPos pos = results["pos"].get<BlockPos>();
                    int dimId = results["dimId"].get<int>();
                    int biomeId = results["biomeId"].get<int>();
                    auto res = BiomeManager::setBiomeById(pos.x, pos.y, pos.z, dimId, biomeId);
                    output.success(fmt::format("set biome to: {}", res));
                    break;
                }
                case do_hash("set_name"): {
                    BlockPos pos = results["pos"].get<BlockPos>();
                    int dimId = results["dimId"].get<int>();
                    string biomeName = results["biomeName"].get<std::string>();
                    int res = BiomeManager::setBiomeByName(pos.x, pos.y, pos.z, dimId, biomeName);
                    output.success(fmt::format("set biome to: id {}", res));
                    break;
                }
                default:
                    break;
                }
                break;
            case do_hash("biomes"):
                switch (do_hash(results["opEnum2"].get<std::string>().c_str()))
                {
                case do_hash("get"): {
                    BlockPos startPos = results["startPos"].get<BlockPos>();
                    BlockPos endPos = results["endPos"].get<BlockPos>();
                    int dimId = results["dimId"].get<int>();
                    string fileName = results["fileName"].get<std::string>();
                    string res = BiomeManager::getBiomeIds(startPos.x, startPos.y, startPos.z,
                        endPos.x, endPos.y, endPos.z, dimId).dump();

                    std::ofstream file;
                    file.open("plugins/WheatBuilder/Biomes/" + fileName + ".json", std::ios::out | std::ios::_Noreplace);
                    file << res;
                    output.success("get");
                    break;
                }
                case do_hash("set"): {
                    BlockPos pos = results["pos"].get<BlockPos>();
                    int dimId = results["dimId"].get<int>();
                    string fileName = results["fileName"].get<std::string>();
                    if (_access(("plugins/WheatBuilder/Biomes/" + fileName + ".json").c_str(), 0) == 0) {
                        std::ifstream file;
                        file.open("plugins/WheatBuilder/Biomes/" + fileName + ".json", std::ios::in);

                        std::istreambuf_iterator<char> beg(file), end;
                        string biomesString = std::string(beg, end);
                        file.seekg(0, std::ios::end);//移动的文件尾部
                        file.close();
                        logger.info(biomesString);
                        nlohmann::json biomes = {{{}}};
                        try {
                            biomes = json::parse(biomesString.c_str(), nullptr, true);
                            string res = BiomeManager::setBiomes(pos.x, pos.y, pos.z, dimId, biomes);
                            if (res == "OK") {
                                output.success("set biomes success.");
                            }
                            else {
                                output.success("set biomes failed.");
                            }
                        }
                        catch (const std::exception& ex) {
                            output.error(fmt::format("Error in biomes string:{}", ex.what()));
                        }
                    }
                    else {
                        output.error("File not exist.");
                    }
                    break;
                }
                case do_hash("replace_id"): {
                    BlockPos startPos = results["startPos"].get<BlockPos>();
                    BlockPos endPos = results["endPos"].get<BlockPos>();
                    int dimId = results["dimId"].get<int>();

                    int originId = results["originId"].get<int>();
                    int replaceId = results["replaceId"].get<int>();
                    auto res = BiomeManager::replaceBiomesById(startPos.x, startPos.y, startPos.z,
                        endPos.x, endPos.y, endPos.z, dimId, originId, replaceId);
                    output.success("replace");
                    break;
                }
                case do_hash("replace_name"): {
                    BlockPos startPos = results["startPos"].get<BlockPos>();
                    BlockPos endPos = results["endPos"].get<BlockPos>();
                    int dimId = results["dimId"].get<int>();

                    string originName = results["originName"].get<std::string>();
                    string replaceName = results["replaceName"].get<std::string>();
                    auto res = BiomeManager::replaceBiomesByName(startPos.x, startPos.y, startPos.z,
                        endPos.x, endPos.y, endPos.z, dimId, originName, replaceName);

                    output.success("replace");
                    break;
                }
                default:
                    break;
                }
                break;
            default:
                break;
            }
            });
        // do not forget to setup the command instance
        DynamicCommand::setup(std::move(command));
    }
    static void moveChunk() {
        using ParamType = DynamicCommand::ParameterType;
        auto command = DynamicCommand::createCommand("mvc", "move chunk", CommandPermissionLevel::GameMasters);

        auto& saveOption = command->setEnum("SaveOption", { "save" });
        auto& loadOption = command->setEnum("LoadOption", { "load" });
        command->mandatory("opEnum1", ParamType::Enum, saveOption, CommandParameterOption::EnumAutocompleteExpansion);
        command->mandatory("opEnum1", ParamType::Enum, loadOption, CommandParameterOption::EnumAutocompleteExpansion);

        command->mandatory("start_x"  , ParamType::Int);
        command->mandatory("start_z"  , ParamType::Int);
        command->mandatory("end_x"    , ParamType::Int);
        command->mandatory("end_z"    , ParamType::Int);
        command->mandatory("dim_id"   , ParamType::Int);
        command->mandatory("file_name", ParamType::String);
        command->mandatory("entity"   , ParamType::Bool);
        command->mandatory("biome"    , ParamType::Bool);
        command->mandatory("save"     , ParamType::Bool);
        // 是否包含实体/群系在加载时决定
        command->addOverload({ saveOption, "start_x", "start_z", "end_x", "end_z", "dim_id", "file_name", "save" });
        command->addOverload({ loadOption, "start_x", "start_z", "dim_id", "file_name"});

        command->setCallback([](DynamicCommand const& command, CommandOrigin const& origin, CommandOutput& output, std::unordered_map<std::string, DynamicCommand::Result>& results) {
            switch (do_hash(results["opEnum1"].get<std::string>().c_str()))
            {
            case do_hash("save"): {
                int start_x = results["start_x"].get<int>();
                int start_z = results["start_z"].get<int>();
                int end_x = results["end_x"].get<int>();
                int end_z = results["end_z"].get<int>();
                int dim_id = results["dim_id"].get<int>();
                string file_name = results["file_name"].get<std::string>();
                bool save = results["save"].get<bool>();
                if (ChunkManager::startSaveAreaTask(start_x, start_z, end_x, end_z, file_name, dim_id, false, false, true, save)) {
                    output.success("Start save area task");
                }
                else {
                    output.error("A save task is running.");
                }
                break;
            }
            case do_hash("load"): {
                int start_x = results["start_x"].get<int>();
                int start_z = results["start_z"].get<int>();
                int dim_id = results["dim_id"].get<int>();
                string file_name = results["file_name"].get<std::string>();

                ChunkManager::startLoadAreaTask(start_x, start_z, dim_id, file_name, false, false, true);
                output.success("Start load area task");
                break;
            }
            default:
                break;
            }
            });
        // do not forget to setup the command instance
        DynamicCommand::setup(std::move(command));
    }
    static void test() {
        using ParamType = DynamicCommand::ParameterType;
        //// create a dynamic command
        auto command = DynamicCommand::createCommand("wbtest", "test command for wheat builder", CommandPermissionLevel::GameMasters);
        command->mandatory("x", ParamType::Int);
        command->mandatory("z", ParamType::Int);
        command->mandatory("dim", ParamType::Int);

        command->addOverload({"x", "z", "dim"});
        command->setCallback([](DynamicCommand const& command, CommandOrigin const& origin, CommandOutput& output, std::unordered_map<std::string, DynamicCommand::Result>& results) {
            bool result = true;// ChunkManager::saveChunk(results["x"].get<int>(), results["z"].get<int>(), results["dim"].get<int>(), "");
            if (result) {
                output.success("success");
            }
            else {
                output.error("failed");
            }
            });
        // do not forget to setup the command instance
        DynamicCommand::setup(std::move(command));
    }
};
