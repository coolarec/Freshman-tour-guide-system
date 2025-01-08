
#include "busRoute.h"
BusRoute::busRoute::busRoute(int id, std::vector<int> r) : id(id), route(r) {}
BusRoute::busRoute BusRoute::busRoute::from_json(const nlohmann::json& j){
	return BusRoute::busRoute(j.at("id").get<int>(), j.at("route").get<std::vector<int>>());
}
nlohmann::json BusRoute::busRoute::to_json() {
	return {
		{"id", id},
		{"route", route}
	};
}
std::vector<int> BusRoute::busRoute::getRoute(int from, int to) const {
	std::vector<int> res;
	int i = 0;
	while (i < route.size() && i != from)i++;
	while (route[(++i)%route.size()] != to) {
		res.push_back(route[i]);
	}
	return res;
}

void BusRoute::loadFromFile(const std::string busFilename) {
	std::ifstream busFile(busFilename);
	if (!busFile) {
		std::cerr << "File not found,Creating new file" << std::endl;
		std::ofstream file(busFilename);
		file << "[]"; // 写入一个空的 JSON 对象
	}
	nlohmann::json j;
	busFile >> j;
	for (auto& element : j) {
		bus[element.at("id").get<int>()] = busRoute::from_json(element);
	}
}
void BusRoute::saveToFile(const std::string busFilename) {
	nlohmann::json j;
	for (auto& [id, route] : bus) {
		j.push_back(route.to_json());
	}
	std::ofstream busFile(busFilename);
	busFile << j.dump(4);
}
