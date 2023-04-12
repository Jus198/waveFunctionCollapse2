#pragma once
#include <iostream>
#include <vector>
using std::vector;
using std::get;
#include "tile.h"
class grid
{
	public:
		int horiz_blocks;
		int vert_blocks;
		int block_len; // can remove just pass to tile on construct
		bool on = false;
		vector<vector<tile>> tiles;
		// vector<vector<int>> vec(n, vector<int>(m, 0));
		//, vector<tuple<string, vector<int>>> spos
		grid(int w, int h, int tilesize, vector<tuple<string, int, vector<int>>> spos) {
			std::cout << "Grid Constructing\n";
			horiz_blocks = w;
			vert_blocks = h;
			block_len = tilesize;

			// Add rows
			for (int i = 0; i < horiz_blocks; ++i) {
				tiles.push_back(std::vector<tile>());
			}

			// Fill in test data
			for (int i = 0; i < horiz_blocks; i++) {
				for (int j = 0; j < vert_blocks; j++) {
					tile t(i, j, tilesize, spos);
					tiles[i].push_back(t);
				}
			}
		}

		bool in_bounds(int x, int y) {
			return x >= 0 and y >= 0 and x < horiz_blocks and y < vert_blocks;
		}

		void tile_linker() {
			for (int i = 0; i < horiz_blocks; i++) {
				for (int j = 0; j < vert_blocks; j++) {
					if (in_bounds(i + 1, j)) {
						tiles[i][j].setRight(&tiles[i + 1][j]);
					}
					if (in_bounds(i - 1, j)) {
						tiles[i][j].setLeft(&tiles[i - 1][j]);
					}
					if (in_bounds(i, j + 1)) {
						tiles[i][j].setUp(&tiles[i][j + 1]);
					}
					if (in_bounds(i, j - 1)) {
						tiles[i][j].setUp(&tiles[i][j - 1]);
					}
				}
			}
		}

		void draw(sf::RenderWindow* w) {
			for (int i = 0; i < horiz_blocks; i++) {
				for (int j = 0; j < vert_blocks; j++) {
					tiles[i][j].draw(w);
				}
			}
		}

		void draw_color(sf::RenderWindow*w) {
			for (int i = 0; i < horiz_blocks; i++) {
				for (int j = 0; j < vert_blocks; j++) {
					tiles[i][j].draw_color(w);
				}
			}
		}

		void draw_spos_text(sf::RenderWindow* w, sf::Font* font) {
			for (int i = 0; i < horiz_blocks; i++) {
				for (int j = 0; j < vert_blocks; j++) {
					tiles[i][j].draw_spos_text(w, font);
				}
			}
		}

		void colorize(int num) {
			int y = 0;
			for (num; num >= 0; num--, y++) {
				if (num < horiz_blocks && y < vert_blocks) {
					tiles[num][y].colorize();
				}
			}
			return;
		}

		void set_all_rand_spos() {
			for (int i = 0; i < horiz_blocks; i++) {
				for (int j = 0; j < vert_blocks; j++) {
					tiles[i][j].set_rand_spos();
				}
			}
		}

		bool set_lowest_spos() {
			int min = 999;
			int minx = -1;
			int miny = -1;
			int mirror_min = 0;
			for (int i = 0; i < horiz_blocks; i++) {
				for (int j = 0; j < vert_blocks; j++) {
					if (tiles[i][j].mSuperposition.mSuperposition.size() < min and tiles[i][j].final_spos_bool == false) {
						mirror_min = 1;
						min = tiles[i][j].mSuperposition.mSuperposition.size();
						minx = i;
						miny = j;
					}
					if (tiles[i][j].mSuperposition.mSuperposition.size() == min and tiles[i][j].final_spos_bool == false) {
						mirror_min += 1;
						if (rand() % mirror_min == 0) {
							min = tiles[i][j].mSuperposition.mSuperposition.size();
							minx = i;
							miny = j;
						}
					}
				}
			}
			if (min == 999) {
				return false;
			}
			std::cout << minx << " " << miny << " setting spos randomly, minsize = "<<min<<"\n";
			tiles[minx][miny].set_rand_spos();
			std::cout << minx << " " << miny << " updating nearby\n";
			update_nearby(minx, miny);
			std::cout << minx << " " << miny << " finished updating nearby\n";
			return true;
		}

		bool set_one_rand_spos() {
			int i = rand() % horiz_blocks;
			int j = rand() % vert_blocks;
			int attempts = 20;
			while (tiles[i][j].is_final_spos() == true and attempts > 0) {
				//std::cout << "searching again\n";
				attempts -= 1;
				int i = rand() % horiz_blocks;
				int j = rand() % vert_blocks;
			}
			if (tiles[i][j].is_final_spos() == false) {
				std::cout << i << " " << j << " setting spos randomly\n";
				tiles[i][j].set_rand_spos();
				std::cout << i << " " << j << " updating nearby\n";
				update_nearby(i, j);
				std::cout << i << " " << j << " finished updating nearby\n";
				/*recursive_update_spos_nearby(i, j);*/
				return true;
			}
			return false;
		}

		template <typename INT, typename T> // INT could be int, unsigned int, char, size_t, etc...
		void removeIndicesFromVector(std::vector<T>& v, std::vector<INT>& rm)
		{
			std::cout << "In rIFV\n";
			// For speed, I am assuming that 'rm' is sorted
			size_t rm_index = 0;
			v.erase(
				std::remove_if(std::begin(v), std::end(v), [&](T& elem)
					{
						if (rm.size() != rm_index && &elem - &v[0] == rm[rm_index])
						{
							rm_index++;
							return true;
						}
						return false;
					}),
				std::end(v)
						);
			std::cout << "Done rIFV\n";
		}

		void recursive_update_spos_nearby(int x, int y) {
			// for position in superpositions
			//		for each adj tile
			//			for superposition in adj tile
			//			remove position if none match
			// if only 1 superposition remains, converge tile
			// if any superpositions removed, call this on all adj non converged tiles
			// converged is final_spos_bool
			// if any superpositions removed is to_remove.size() > 0
			vector<int> to_remove;
			if (tiles[x][y].final_spos_bool) {
				std::cout << "QQQQQQQQQQQQQQQQQQQQQQ\n";
			}
			for (int i = 0; i < tiles[x][y].mSuperposition.spos_size(); i++) {
				// tiles[x][y].mSuperposition.mSuperposition[i];
				vector<int> sides = std::get<2>(tiles[x][y].mSuperposition.mSuperposition[i]);

				bool remove_spos = false;

				if (in_bounds(x + 1, y)) {
					bool remove = true;
					for (int i = 0; i < tiles[x + 1][y].mSuperposition.spos_size(); i++) {
						// if any piece fits, rem1 = false -> dont remove tile, for this direction
						// if piece fits, break loop because it doesn't matter
						vector<int> sides_2 = std::get<2>(tiles[x + 1][y].mSuperposition.mSuperposition[i]);
						if (sides[3] == sides_2[11] and sides[4] == sides_2[10] and sides[5] == sides_2[9]) {
							remove = false;
							break;
						}
					}
					if (remove) {
						remove_spos = true;
					}
				}
				if (in_bounds(x - 1, y)) {
					bool remove = true;
					for (int i = 0; i < tiles[x - 1][y].mSuperposition.spos_size(); i++) {
						vector<int> sides_2 = std::get<2>(tiles[x - 1][y].mSuperposition.mSuperposition[i]);
						if (sides[9] == sides_2[5] and sides[10] == sides_2[4] and sides[11] == sides_2[3]) {
							remove = false;
							break;
						}
					}
					if (remove) {
						remove_spos = true;
					}
				}
				if (in_bounds(x, y + 1)) {
					bool remove = true;
					for (int i = 0; i < tiles[x][y + 1].mSuperposition.spos_size(); i++) {
						vector<int> sides_2 = std::get<2>(tiles[x][y + 1].mSuperposition.mSuperposition[i]);
						if (sides[6] == sides_2[2] and sides[7] == sides_2[1] and sides[8] == sides_2[0]) {
							remove = false;
							break;
						}
					}
					if (remove) {
						remove_spos = true;
					}
				}
				if (in_bounds(x, y - 1)) {
					bool remove = true;
					for (int i = 0; i < tiles[x][y - 1].mSuperposition.spos_size(); i++) {
						vector<int> sides_2 = std::get<2>(tiles[x][y - 1].mSuperposition.mSuperposition[i]);
						if (sides[0] == sides_2[8] and sides[1] == sides_2[7] and sides[2] == sides_2[6]) {
							remove = false;
							break;
						}
					}
					if (remove) {
						remove_spos = true;
					}
				}
				if (remove_spos) {
					to_remove.push_back(i);
				}
			}
			if (to_remove.size() > 0) {
				removeIndicesFromVector(tiles[x][y].mSuperposition.mSuperposition, to_remove);
				std::cout << x << " " << y << " updating nearby recursively\n";
				update_nearby(x, y);
			}
			// remove all in to_remove
			// if to_remove.size() > 0, update all non collapsed blocks around
		}

		void update_nearby(int x, int y) {
			if (in_bounds(x + 1, y)) {
				if (tiles[x + 1][y].is_final_spos() == false) {
					std::cout << x + 1 << " " << y << " updating recursively\n";
					recursive_update_spos_nearby(x + 1, y);
				}
			}
			if (in_bounds(x - 1, y)) {
				if (tiles[x - 1][y].is_final_spos() == false) {
					std::cout << x - 1 << " " << y << " updating recursively\n";
					recursive_update_spos_nearby(x - 1, y);
				}
			}
			if (in_bounds(x, y + 1)) {
				if (tiles[x][y + 1].is_final_spos() == false) {
					std::cout << x << " " << y + 1 << " updating recursively\n";
					recursive_update_spos_nearby(x, y + 1);
				}
			}
			if (in_bounds(x, y - 1)) {
				if (tiles[x][y - 1].is_final_spos() == false) {
					std::cout << x << " " << y - 1 << " updating recursively\n";
					recursive_update_spos_nearby(x, y - 1);
				}
			}
		}
};

