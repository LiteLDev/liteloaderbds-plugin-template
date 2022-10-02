#pragma once
#include <map>
#include <stack>
#include <ScheduleAPI.h>

// Save schedule
namespace SS {
	ScheduleTask task;

	wb::AreaList area_process_list;
	string floder_name;
	int dim_id;
	bool block;
	bool entity;
	bool biome;
	bool save;

	int status = 0; // 状态, 0为开始处理, 1为等待加载
	int onProcess_locate_x;
	int onProcess_locate_z;
	int onProcess_i = 0;
	int onProcess_start_x;
	int onProcess_start_z;
	int onProcess_end_x;
	int onProcess_end_z;

	int empty_try = 0;
	int empty_try_max = 3;
}

// Load schedule

namespace LS {
	ScheduleTask task;
	nlohmann::json areaUnit = {};
	string floder_name;
	int dim_id;
	bool block;
	bool entity;
	bool biome;
	int start_x;
	int start_z;

	int status = 0; // 状态, 0为开始处理, 1为等待加载
	int onProcess_i = 0;
	int empty_try = 0;
	int empty_try_max = 10;
	int onProcess_start_x;
	int onProcess_start_z;
	int onProcess_end_x;
	int onProcess_end_z;
	int emptyTryAmount = 0;
}