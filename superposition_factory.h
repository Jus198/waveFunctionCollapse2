#pragma once
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <tuple>
using std::tuple;
using namespace std;
class superposition_factory
{
	public:
		vector<tuple<string, int, vector<int>>> superpositions;
		
		superposition_factory() {
			superpositions.push_back({ "Grey.png", 0, { 0,0,0,0,0,0,0,0,0,0,0,0 } });
			superpositions.push_back({ "BlueCorner.png", 0, { 0,0,0,0,1,0,0,1,0,0,0,0 } });
			superpositions.push_back({ "BlueCorner.png", 90, { 0,0,0,0,0,0,0,1,0,0,1,0 } });
			superpositions.push_back({ "BlueCorner.png", 180, { 0,1,0,0,0,0,0,0,0,0,1,0 } });
			superpositions.push_back({ "BlueCorner.png", 270, { 0,1,0,0,1,0,0,0,0,0,0,0 } });
			superpositions.push_back({ "BlueCenter.png", 0, { 0,1,0,0,1,0,0,1,0,0,1,0 } });
		}

		vector<tuple<string, int, vector<int>>> get_superpositions() {
			return superpositions;
		}
};

