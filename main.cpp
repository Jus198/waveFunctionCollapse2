#include <SFML/Graphics.hpp>
#include "grid.h"
#include "tile.h"
#include "superposition_factory.h"
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <tuple>
using std::tuple;
using namespace std;

int main() {

    int horiz_tiles = 35;
    int vert_tiles = 18;
    int tile_size = 50;
    /*int horiz_tiles = 20;
    int vert_tiles = 12;
    int tile_size = 50;*/
    /*int horiz_tiles = 8;
    int vert_tiles = 8;
    int tile_size = 50;*/
    bool draw_spos_num = true;

    sf::RenderWindow window(sf::VideoMode(horiz_tiles*tile_size, vert_tiles*tile_size), "tbd");
    sf::CircleShape shape;
    window.setFramerateLimit(20);
    shape.setRadius(40.f);
    shape.setPosition(100.f, 100.f);
    shape.setFillColor(sf::Color::Cyan);

    int colorize_counter = 0;

    sf::Texture texture;
    texture.loadFromFile("BlueCenter.png");
    sf::Sprite sprite(texture);
    /*sf::Transform transform; // rotation code, pass transform into draw
    transform.rotate(45, sf::Vector2f(75, 75));*/
    sprite.setPosition(50, 50);

    sf::Font font;
    font.loadFromFile("arial.ttf");

    superposition_factory factory();
    vector<tuple<string, int, vector<int>>> t;
    superposition_factory s;
    t = s.get_superpositions();

    grid g(horiz_tiles, vert_tiles, tile_size, t);
    g.tile_linker();
    g.set_one_rand_spos();
    //g.set_all_rand_spos();

    int full_count = 0;
    bool stop_trying = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        /*if (colorize_counter < horiz_tiles + vert_tiles - 1) {
            g.colorize(colorize_counter);
            colorize_counter++;
        }
        g.draw_color(&window);*/
        /*if (!stop_trying) {
            if (!g.set_one_rand_spos()) {
                full_count += 1;
                if (full_count == 3) {
                    stop_trying = true;
                }
            }
        }*/
        //g.set_one_rand_spos();
        g.set_lowest_spos();
        g.draw(&window);

        if (draw_spos_num) {
            g.draw_spos_text(&window, &font);
        }
        window.display();
    }

}