#pragma once
#include "nlohmann/json.hpp"
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <array>

class BusRoute {
public:
	struct busRoute {
		int id;
		std::vector<int> route;
		busRoute(int id, std::vector<int> r);
		busRoute() = default;
		std::vector<int> getRoute(int from, int to) const;
		nlohmann::json to_json();
		static busRoute from_json(const nlohmann::json& j);
	};
private:
	std::map<int, busRoute> bus;
public:
	int current_busRoute_id = 0;
	void loadFromFile(const std::string busFilename = "bus.json");
	void saveToFile(const std::string busFilename = "bus.json");
};