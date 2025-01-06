#pragma once
#include "nlohmann/json.hpp"
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <array>
#include<queue>
class SchoolMap {
public:
    struct SchoolAttraction {
        int id; // 景点编号
        std::string name; // 景点名称
        std::string info; // 景点信息介绍
        std::array<double, 2> position; // 景点坐标
        int tag; // 景点标签，是否为教学楼，体育馆等
        bool visable; // 景点是否可见
    
        SchoolAttraction(int id, const std::string& name, const std::string& info,
            const std::array<double, 2>& position, int tag, bool visable);
		SchoolAttraction() = default;
        nlohmann::json to_json() const;
        static SchoolAttraction from_json(const nlohmann::json& j);
    };

    struct SchoolPath {
        int id; // 道路编号
        std::string name;
		int from;//起点景点编号
		int to;//终点景点编号
		double length; // 道路长度
		bool walk=0, sharebike=0, bus=0; // 是否可步行，骑行，乘坐校车
        SchoolPath(int id,std::string name, int from, int to, double length, bool walk, bool sharebike, bool bus);
        SchoolPath() = default;
        nlohmann::json to_json() const;
        static SchoolPath from_json(const nlohmann::json& j);

		bool operator<(const SchoolPath& path) const {
			return length < path.length;
		}
    };
    
private:
    std::map<int, SchoolAttraction> attractions;
    std::map<int, SchoolPath>paths;
    std::map<int, std::vector<SchoolPath>> destinations;

public:
	int current_attraction_id = 0,current_path_id=0;
    SchoolMap();

    //文件操作，初始化
    void loadFromFile(const std::string& attractionFilename = "attraction.json", const std::string& pathFilename="path.json");
    void saveToFile(const std::string& attractionFilename = "attraction.json", const std::string& pathFilename = "path.json");
	
    //景点操作
    nlohmann::json getVisibleAttractions() const;
    nlohmann::json getAllAttractions() const;
    void addAttraction(const std::string& name, const std::string& info,
        const std::array<double, 2>& position, int tag, bool visable);
    void deleteAttraction(int id);
	int updateAttraction(int id, const std::string& name, const std::string& info,
		const std::array<double, 2>& position, int tag, bool visable);
	
    //道路操作
    nlohmann::json getPaths(int from);
    nlohmann::json getAllPaths()const;
	void addPath(std::string name,int from, int to, double length, bool walk, bool sharebike, bool bus);
	void deletePath(int id);
	void updatePath(int id,std::string name, int from, int to, double length, bool walk, bool sharebike, bool bus);
	 
	//完成最短路相关操作
    //nlohmann::json getShortestPath(int from, int to);
	nlohmann::json getShortestPath(int from, int to, bool walk=true, bool sharebike=true, bool bus=true);//限制交通方式
    nlohmann::json getFastestPath(int from, int to, bool walk = true, bool sharebike = true, bool bus = true);
	//获取一个点到另一个点的所有路径
	nlohmann::json getAllPath(int from, int to, bool walk = true, bool sharebike = true, bool bus = true);
    //nlohmann::json dijkstra(int from, int to);
};
