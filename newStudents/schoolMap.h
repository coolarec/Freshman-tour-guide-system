#pragma once
#include "nlohmann/json.hpp"
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <array>

class SchoolMap {
public:
    struct SchoolAttraction {
        int id; // ������
        std::string name; // ��������
        std::string info; // ������Ϣ����
        std::array<double, 2> position; // ��������
        int tag; // �����ǩ���Ƿ�Ϊ��ѧ¥�������ݵ�
        bool visable; // �����Ƿ�ɼ�

        SchoolAttraction(int id, const std::string& name, const std::string& info,
            const std::array<double, 2>& position, int tag, bool visable);
		SchoolAttraction() = default;
        nlohmann::json to_json() const;
        static SchoolAttraction from_json(const nlohmann::json& j);
    };

    struct SchoolPath {
        int id; // ��·���
		int from;//��㾰����
		int to;//�յ㾰����
		double length; // ��·����
		bool walk=0, sharebike=0, bus=0; // �Ƿ�ɲ��У����У�����У��
        SchoolPath(int id, int from, int to, double length, bool walk, bool sharebike, bool bus);
        SchoolPath() = default;
        nlohmann::json to_json() const;
        static SchoolPath from_json(const nlohmann::json& j);
    };
private:
    std::map<int, SchoolAttraction> attractions;
    std::map<int, SchoolPath>paths;
    std::map<int, std::vector<SchoolPath>> destination;

public:
	int current_attraction_id = 0,current_path_id=0;
    SchoolMap();

    //�ļ�����
    void loadFromFile(const std::string& attractionFilename = "attraction.json", const std::string& pathFilename="path.json");
    void saveToFile(const std::string& attractionFilename = "attraction.json", const std::string& pathFilename = "path.json");
	//�������
    nlohmann::json getVisibleAttractions() const;
    nlohmann::json getAllAttractions() const;
    void addAttraction(const std::string& name, const std::string& info,
        const std::array<double, 2>& position, int tag, bool visable);
    void deleteAttraction(int id);
	int updateAttraction(int id, const std::string& name, const std::string& info,
		const std::array<double, 2>& position, int tag, bool visable);
};
