#pragma once
#include <vector>
using std::vector;
#include <tuple>
using std::tuple;
#include <string>
using std::string;
class superposition
{
	public:
		int positions = -1;

		vector<tuple<string, int, vector<int>>> mSuperposition = {};

		superposition(vector<tuple<string, int, vector<int>>> spos) {
			mSuperposition = spos;
		}

		superposition() {

		}

		bool elim(superposition s) {
			return true;
		}

		int spos_size() {
			return mSuperposition.size();
		}

		tuple<string, int, vector<int>> set_random_spos() {
			tuple<string, int, vector<int>> pos = mSuperposition[rand() % mSuperposition.size()];
			mSuperposition.clear();
			mSuperposition.push_back(pos);
			return pos;
		}
};
//#pragma once
//#include <vector>
//using std::vector;
//#include <tuple>
//using std::tuple;
//class superposition
//{
//	public:
//		int positions = -1;
//
//		superposition(int i) {
//			positions = i;
//		}
//};
