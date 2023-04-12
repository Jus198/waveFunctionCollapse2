#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "superposition.h"
#include <vector>
using std::vector;
#include <tuple>
using std::tuple;
#include <string>
using std::string;
class tile
{
	public:
		sf::Uint8 r = rand() % 256;
		sf::Uint8 g = rand() % 256;
		sf::Uint8 b = rand() % 256;
		//sf::Uint8 o = rand() % 256;
		sf::Color color = sf::Color{ r, g, b, 255};

		//sf::Texture texture;
		//sf::Sprite sprite;

		int x_pixel;
		int y_pixel;
		int x_gridpos;
		int y_gridpos;
		int size;

		tile* up = NULL;
		tile* down = NULL;
		tile* left = NULL;
		tile* right = NULL;

		sf::RectangleShape square;

		superposition mSuperposition;

		bool final_spos_bool = false;
		tuple<string, int, vector<int>> final_spos;

		sf::Texture texture;
		sf::Sprite sprite;
		sf::Transform transform;

		tile(int x, int y, int s, vector<tuple<string, int, vector<int>>> spos) {
			std::cout << "Constructing tile " << x << " " << y << "\n";
			x_gridpos = x;
			x_pixel = x * s;
			y_gridpos = y;
			y_pixel = y * s;
			size = s;
			sf::RectangleShape rectangle(sf::Vector2f(size, size));
			square = rectangle;
			square.setPosition(x_pixel, y_pixel);
			square.setFillColor(sf::Color::White);

			mSuperposition = superposition(spos);

			/*texture.loadFromFile("BlueCenter.png");
			sprite.setTexture(texture);
			sf::Vector2u size = texture.getSize();
			sprite.setOrigin((size.x / 2) + x_pixel, (size.y / 2) + y_pixel);
			
			square.setTexture(&texture);*/
		}

		void colorize() {
			square.setFillColor(color);
			return;
		}

		void draw(sf::RenderWindow* w) {
			w->draw(sprite, transform);
			return;
		}

		void draw_color(sf::RenderWindow*w) {
			w->draw(square);
			//w->draw(sprite);
			return;
		}

		void draw_spos_text(sf::RenderWindow* w, sf::Font* font) {
			sf::Text text;

			text.setString(std::to_string(mSuperposition.spos_size()));
			text.setFont(*font);
			text.setCharacterSize(20);
			text.setFillColor(sf::Color::Black);
			text.setPosition(x_pixel, y_pixel);

			w->draw(text);
		}

		void set_rand_spos() {
			if (final_spos_bool) {
				std::cout << "AHHHHHHHHHHHHHHHHHHHHHHHH\n";
			}
			final_spos_bool = true;
			final_spos = mSuperposition.set_random_spos();
			texture.loadFromFile(std::get<0>(final_spos));
			int rotation = std::get<1>(final_spos);
			transform.rotate(rotation, sf::Vector2f(x_pixel + size/2, y_pixel + size / 2));
			sf::Sprite sprite2(texture);
			sprite = sprite2;
			sprite.setPosition(x_pixel, y_pixel);
			std::cout << "set random spos\n";
		}

		bool is_final_spos() {
			return final_spos_bool;
		}

		void setUp(tile*upp) {
			up = upp;
		}

		void setDown(tile* downn) {
			down = downn;
		}

		void setLeft(tile* leftt) {
			left = leftt;
		}

		void setRight(tile* rightt) {
			right = rightt;
		}
};

