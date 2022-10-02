#pragma once

#include <Nlohmann/json.hpp>
#include <math.h>

#include <MC/Types.hpp>
#include <Global.h>
#include <MC/Level.hpp>
#include <MC/ChunkSource.hpp>
#include <MC/LevelChunk.hpp>
#include <MC/Biome.hpp>

#include <MC/Dimension.hpp>
#include <MC/ChunkPos.hpp>
#include <MC/BlockPos.hpp>
#include <MC/ChunkBlockPos.hpp>

#include "Tools.h"

class BiomeManager
{
public:
	// For single block
	static int    getBiomeId(int x, int y, int z, int dim_id) {
		try {
			std::pair<ChunkPos*, ChunkBlockPos> chunkPos = PositionTool::block2chunkPos(x, y, z);

			std::shared_ptr<class LevelChunk> levelChunk = Global<Level>->getDimension(
				AutomaticID<Dimension, int>(dim_id))->getChunkSource().getExistingChunk(*chunkPos.first);

			if (levelChunk != nullptr) {
				return levelChunk->getBiome(chunkPos.second).getId();
			}
			else {
				return -1;
			}
		}
		catch (const std::exception& ex) {
			return -1;
		}
	}
	static string getBiomeName(int x, int y, int z, int dim_id) {
		try {
			std::pair<ChunkPos*, ChunkBlockPos> chunkPos = PositionTool::block2chunkPos(x, y, z);

			std::shared_ptr<class LevelChunk> levelChunk = Global<Level>->getDimension(
				AutomaticID<Dimension, int>(dim_id))->getChunkSource().getExistingChunk(*chunkPos.first);

			if (levelChunk != nullptr) {
				return levelChunk->getBiome(chunkPos.second).getName();
			}
			else {
				return "error";
			}
		}
		catch (const std::exception& ex) {
			return "error";
		}
	}
	static std::pair<int, string> getbiomeIdAndName(int x, int y, int z, int dim_id) {
		try {
			std::pair<ChunkPos*, ChunkBlockPos> chunkPos = PositionTool::block2chunkPos(x, y, z);

			std::shared_ptr<class LevelChunk> levelChunk = Global<Level>->getDimension(
				AutomaticID<Dimension, int>(dim_id))->getChunkSource().getExistingChunk(*chunkPos.first);

			if (levelChunk != nullptr) {
				auto& biome = levelChunk->getBiome(chunkPos.second);
				return std::pair(biome.getId(), biome.getName());
			}
			else {
				return std::pair(-1, "error");
			}
		}
		catch (const std::exception& ex) {
			return std::pair(-1, "error");
		}
	}
	static string   setBiomeById(int x, int y, int z, int dim_id, int biome_id) {
		try {
			std::pair<ChunkPos*, ChunkBlockPos> chunkPos = PositionTool::block2chunkPos(x, y, z);

			std::shared_ptr<class LevelChunk> levelChunk = Global<Level>->getDimension(
				AutomaticID<Dimension, int>(dim_id))->getChunkSource().getExistingChunk(*chunkPos.first);

			if (levelChunk != nullptr) {
				levelChunk->setBiome2d(*Biome::fromId(biome_id), chunkPos.second);
				levelChunk->setUnsaved();
				return levelChunk->getBiome(chunkPos.second).getName();
			}
			else {
				return "error";
			}
		}
		catch (const std::exception& ex) {
			return "error";
		}
	}
	static int   setBiomeByName(int x, int y, int z, int dim_id, string biome_name) {
		try {
			std::pair<ChunkPos*, ChunkBlockPos> chunkPos = PositionTool::block2chunkPos(x, y, z);

			std::shared_ptr<class LevelChunk> levelChunk = Global<Level>->getDimension(
				AutomaticID<Dimension, int>(dim_id))->getChunkSource().getExistingChunk(*chunkPos.first);

			if (levelChunk != nullptr) {
				levelChunk->setBiome2d(*Biome::fromName(biome_name), chunkPos.second);
				// levelChunk->setBiome2d(*Biome::fromName(biome_name), chunkPos.second);
				levelChunk->setUnsaved();
				return levelChunk->getBiome(chunkPos.second).getId();
			}
			else {
				return -1;
			}
		}
		catch (const std::exception& ex) {
			return -1;
		}
	}
	// For blocks in a rectangle area
	// [[[id]]] [x][z][y]
	static nlohmann::json getBiomeIds(int s_x, int s_y, int s_z, int e_x, int e_y, int e_z, int dim_id) {
		try {
			int start_x, start_y, start_z, end_x, end_y, end_z;
			if (s_x < e_x) {
				start_x = s_x;
				end_x = e_x;
			}
			else {
				start_x = e_x;
				end_x = s_x;
			}
			if (s_y < e_y) {
				start_y = s_y;
				end_y = e_y;
			}
			else {
				start_y = e_y;
				end_y = s_y;
			}
			if (s_z < e_z) {
				start_z = s_z;
				end_z = e_z;
			}
			else {
				start_z = e_z;
				end_z = s_z;
			}
			if (start_y < -64) start_y = -64;
			if (start_y > 319) start_y = 319;
			if (end_y < -64) end_y = -64;
			if (end_y > 319) end_y = 319;
			int x_offset = end_x - start_x + 1;
			int y_offset = end_y - start_y + 1;
			int z_offset = end_z - start_z + 1;

			nlohmann::json biomeIds;
			for (int ix = 0; ix < x_offset; ix++) {
				nlohmann::json temp_x;
				for (int iz = 0; iz < x_offset; iz++) {
					nlohmann::json temp_z;
					for (int iy = 0; iy < y_offset; iy++) {
						temp_z.push_back(-1);
					}
					temp_x.push_back(temp_z);
				}
				biomeIds.push_back(temp_x);
			}
			std::pair<ChunkPos*, ChunkBlockPos> startChunkPos = PositionTool::block2chunkPos(start_x, start_y, start_z);
			std::pair<ChunkPos*, ChunkBlockPos> endChunkPos = PositionTool::block2chunkPos(end_x, end_y, end_z);
			for (int chunk_x = startChunkPos.first->x; chunk_x <= endChunkPos.first->x; chunk_x++) {
				for (int chunk_z = startChunkPos.first->z; chunk_z <= endChunkPos.first->z; chunk_z++) {
					int chunk_start_x = 0;
					int chunk_start_z = 0;
					int chunk_end_x = 15;
					int chunk_end_z = 15;
					if (chunk_x == startChunkPos.first->x) chunk_start_x = startChunkPos.second.x;
					if (chunk_z == startChunkPos.first->z) chunk_start_z = startChunkPos.second.z;
					if (chunk_x == endChunkPos.first->x) chunk_end_x = endChunkPos.second.x;
					if (chunk_z == endChunkPos.first->z) chunk_end_z = endChunkPos.second.z;
					// std::cout << "chunk:" << chunk_x << chunk_z << "\n";
					try {
						std::shared_ptr<class LevelChunk> levelChunk = Global<Level>->getDimension(
							AutomaticID<Dimension, int>(dim_id))->getChunkSource().getExistingChunk(ChunkPos(chunk_x, chunk_z));
						if (levelChunk != nullptr) {
							for (int chunk_block_x = chunk_start_x; chunk_block_x <= chunk_end_x; chunk_block_x++) {
								for (int chunk_block_z = chunk_start_z; chunk_block_z <= chunk_end_z; chunk_block_z++) {
									for (int chunk_block_y = start_y; chunk_block_y <= end_y; chunk_block_y++) {
										// std::cout << "point:" << chunk_block_x << " " << chunk_block_z << " " << chunk_block_y << "\n";
										biomeIds[chunk_x * 16 + chunk_block_x - start_x]
											[chunk_z * 16 + chunk_block_z - start_z][chunk_block_y] = 
										levelChunk->getBiome(ChunkBlockPos(chunk_block_x, chunk_block_y, chunk_block_z)).getId();
									}
								}
							}
						}
					}
					catch (const std::exception& ex) {
						return "error";
					}
				}
			}
			return biomeIds;
		}
		catch (const std::exception& ex) {
			return nlohmann::json{ {{}} };
		}
	}
	// static nlohmann::json getBiomeNames(int start_x, int start_y, int start_z, int end_x, int end_y, int end_z);
	static string setBiomes(int start_x, int start_y, int start_z, int dim_id, nlohmann::json biomeIds) {
		int x_offset = biomeIds.size();
		int z_offset = biomeIds[0].size();
		int y_offset = biomeIds[0][0].size();
		
		int end_x = start_x + x_offset;
		int end_z = start_z + z_offset;
		int end_y = start_y + y_offset;

		std::pair<ChunkPos*, ChunkBlockPos> startChunkPos = PositionTool::block2chunkPos(start_x, start_y, start_z);
		std::pair<ChunkPos*, ChunkBlockPos> endChunkPos = PositionTool::block2chunkPos(end_x, end_y, end_z);
		for (int chunk_x = startChunkPos.first->x; chunk_x <= endChunkPos.first->x; chunk_x++) {
			for (int chunk_z = startChunkPos.first->z; chunk_z <= endChunkPos.first->z; chunk_z++) {
				int chunk_start_x = 0;
				int chunk_start_z = 0;
				int chunk_end_x = 15;
				int chunk_end_z = 15;
				if (chunk_x == startChunkPos.first->x) chunk_start_x = startChunkPos.second.x;
				if (chunk_z == startChunkPos.first->z) chunk_start_z = startChunkPos.second.z;
				if (chunk_x == endChunkPos.first->x) chunk_end_x = endChunkPos.second.x;
				if (chunk_z == endChunkPos.first->z) chunk_end_z = endChunkPos.second.z;
				// std::cout << "chunk:" << chunk_x << chunk_z << "\n";
				std::shared_ptr<class LevelChunk> levelChunk = Global<Level>->getDimension(
					AutomaticID<Dimension, int>(dim_id))->getChunkSource().getExistingChunk(ChunkPos(chunk_x, chunk_z));
				if (levelChunk != nullptr) {
					for (int chunk_block_x = chunk_start_x; chunk_block_x <= chunk_end_x; chunk_block_x++) {
						for (int chunk_block_z = chunk_start_z; chunk_block_z <= chunk_end_z; chunk_block_z++) {
							for (int chunk_block_y = start_y; chunk_block_y <= end_y; chunk_block_y++) {
								if (biomeIds[chunk_x * 16 + chunk_block_x][chunk_z * 16 + chunk_block_z][chunk_block_y] != -1) {
									if (-64 <= chunk_block_y && chunk_block_y <= 319) {
										try {
											int biomeId = biomeIds[chunk_x * 16 + chunk_block_x - start_x]
												[chunk_z * 16 + chunk_block_z - start_z][chunk_block_y];
											if (biomeId != -1) {
												levelChunk->setBiome2d(*Biome::fromId(biomeId)
													, ChunkBlockPos(chunk_block_x, chunk_block_y, chunk_block_z));
												levelChunk->setUnsaved();
											}
										}
										catch (const std::exception& ex) {
											return "Error";
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return "OK";
	}
	// if origin_id is -1, then replace all.
	static string replaceBiomesById(int s_x, int s_y, int s_z, int e_x, int e_y, int e_z, int dim_id, int origin_id, int replace_id) {
		int start_x, start_y, start_z, end_x, end_y, end_z;
		if (s_x < e_x) {
			start_x = s_x;
			end_x = e_x;
		}
		else {
			start_x = e_x;
			end_x = s_x;
		}
		if (s_y < e_y) {
			start_y = s_y;
			end_y = e_y;
		}
		else {
			start_y = e_y;
			end_y = s_y;
		}
		if (s_z < e_z) {
			start_z = s_z;
			end_z = e_z;
		}
		else {
			start_z = e_z;
			end_z = s_z;
		}
		if (start_y < -64) start_y = -64;
		if (start_y > 319) start_y = 319;
		if (end_y < -64) end_y = -64;
		if (end_y > 319) end_y = 319;
		int x_offset = end_x - start_x + 1;
		int y_offset = end_y - start_y + 1;
		int z_offset = end_z - start_z + 1;


		std::pair<ChunkPos*, ChunkBlockPos> startChunkPos = PositionTool::block2chunkPos(start_x, start_y, start_z);
		std::pair<ChunkPos*, ChunkBlockPos> endChunkPos = PositionTool::block2chunkPos(end_x, end_y, end_z);
		for (int chunk_x = startChunkPos.first->x; chunk_x <= endChunkPos.first->x; chunk_x++) {
			for (int chunk_z = startChunkPos.first->z; chunk_z <= endChunkPos.first->z; chunk_z++) {
				int chunk_start_x = 0;
				int chunk_start_z = 0;
				int chunk_end_x = 15;
				int chunk_end_z = 15;
				if (chunk_x == startChunkPos.first->x) chunk_start_x = startChunkPos.second.x;
				if (chunk_z == startChunkPos.first->z) chunk_start_z = startChunkPos.second.z;
				if (chunk_x == endChunkPos.first->x) chunk_end_x = endChunkPos.second.x;
				if (chunk_z == endChunkPos.first->z) chunk_end_z = endChunkPos.second.z;
				// std::cout << "chunk:" << chunk_x << chunk_z << "\n";
				try {
					std::shared_ptr<class LevelChunk> levelChunk = Global<Level>->getDimension(
						AutomaticID<Dimension, int>(dim_id))->getChunkSource().getExistingChunk(ChunkPos(chunk_x, chunk_z));
					if (levelChunk != nullptr) {
						for (int chunk_block_x = chunk_start_x; chunk_block_x <= chunk_end_x; chunk_block_x++) {
							for (int chunk_block_z = chunk_start_z; chunk_block_z <= chunk_end_z; chunk_block_z++) {
								for (int chunk_block_y = start_y; chunk_block_y <= end_y; chunk_block_y++) {
									// std::cout << "point:" << chunk_block_x << chunk_block_z << chunk_block_y << "\n";
									if (origin_id == -1 || levelChunk->getBiome(ChunkBlockPos(chunk_block_x, chunk_block_y, chunk_block_z)).getId() == origin_id) {
										levelChunk->setBiome2d(*Biome::fromId(replace_id), ChunkBlockPos(chunk_block_x, chunk_block_y, chunk_block_z));
										levelChunk->setUnsaved();
									}
								}
							}
						}
					}
				}
				catch (const std::exception& ex) {
					return "error";
				}
			}
		}
		return "OK";
	}
	static string replaceBiomesByName(int s_x, int s_y, int s_z, int e_x, int e_y, int e_z, int dim_id, string origin_name, string replace_name) {
		int start_x, start_y, start_z, end_x, end_y, end_z;
		if (s_x < e_x) {
			start_x = s_x;
			end_x = e_x;
		}
		else {
			start_x = e_x;
			end_x = s_x;
		}
		if (s_y < e_y) {
			start_y = s_y;
			end_y = e_y;
		}
		else {
			start_y = e_y;
			end_y = s_y;
		}
		if (s_z < e_z) {
			start_z = s_z;
			end_z = e_z;
		}
		else {
			start_z = e_z;
			end_z = s_z;
		}
		if (start_y < -64) start_y = -64;
		if (start_y > 319) start_y = 319;
		if (end_y < -64) end_y = -64;
		if (end_y > 319) end_y = 319;
		int x_offset = end_x - start_x + 1;
		int y_offset = end_y - start_y + 1;
		int z_offset = end_z - start_z + 1;


		std::pair<ChunkPos*, ChunkBlockPos> startChunkPos = PositionTool::block2chunkPos(start_x, start_y, start_z);
		std::pair<ChunkPos*, ChunkBlockPos> endChunkPos = PositionTool::block2chunkPos(end_x, end_y, end_z);
		for (int chunk_x = startChunkPos.first->x; chunk_x <= endChunkPos.first->x; chunk_x++) {
			for (int chunk_z = startChunkPos.first->z; chunk_z <= endChunkPos.first->z; chunk_z++) {
				int chunk_start_x = 0;
				int chunk_start_z = 0;
				int chunk_end_x = 15;
				int chunk_end_z = 15;
				if (chunk_x == startChunkPos.first->x) chunk_start_x = startChunkPos.second.x;
				if (chunk_z == startChunkPos.first->z) chunk_start_z = startChunkPos.second.z;
				if (chunk_x == endChunkPos.first->x) chunk_end_x = endChunkPos.second.x;
				if (chunk_z == endChunkPos.first->z) chunk_end_z = endChunkPos.second.z;
				// std::cout << "chunk:" << chunk_x << chunk_z << "\n";
				try {
					std::shared_ptr<class LevelChunk> levelChunk = Global<Level>->getDimension(
						AutomaticID<Dimension, int>(dim_id))->getChunkSource().getExistingChunk(ChunkPos(chunk_x, chunk_z));
					if (levelChunk != nullptr) {
						for (int chunk_block_x = chunk_start_x; chunk_block_x <= chunk_end_x; chunk_block_x++) {
							for (int chunk_block_z = chunk_start_z; chunk_block_z <= chunk_end_z; chunk_block_z++) {
								for (int chunk_block_y = start_y; chunk_block_y <= end_y; chunk_block_y++) {
									// std::cout << "point:" << chunk_block_x << chunk_block_z << chunk_block_y << "\n";
									if (origin_name == "every" || levelChunk->getBiome(ChunkBlockPos(chunk_block_x, chunk_block_y, chunk_block_z)).getName() == origin_name) {
										levelChunk->setBiome2d(*Biome::fromName(replace_name), ChunkBlockPos(chunk_block_x, chunk_block_y, chunk_block_z));
										levelChunk->setUnsaved();
									}
								}
							}
						}
					}
				}
				catch (const std::exception& ex) {
					return "error";
				}
			}
		}
		return "OK";
	}
};

