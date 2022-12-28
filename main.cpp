#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 2000;
const int WINDOW_HEIGHT = 1000;
const int WORLD_SIZE = std::min(WINDOW_WIDTH, WINDOW_HEIGHT) - 200;
const sf::Vector2f WORLD_ORIGIN = sf::Vector2f(WINDOW_WIDTH - WORLD_SIZE - 100, 100);
const int GRID_SCALE = WORLD_SIZE / 10; // Always grid 10 x 10
const float PARTICLE_RADIUS = 10.f;

struct Particle
{
    sf::Vector2f m_position;
    sf::Vector2f m_speed;
    sf::Vector2f m_acceleration;
    int m_radius;
    sf::Color m_color;
    sf::CircleShape m_shape;

    Particle()
    :   m_position(800.f, 800.f), m_speed(0.f, 0.f), m_acceleration(0.f, 0.f), m_radius(PARTICLE_RADIUS), m_color(sf::Color::Red)
    {
        m_shape.setRadius(m_radius);
        m_shape.setPosition(m_position);
        m_shape.setFillColor(m_color);
        m_shape.setOrigin(- WORLD_ORIGIN + sf::Vector2f(m_radius, m_radius));
    }
    
    ~Particle(){}

    // TODO: update and shape display functions
};

// Hash function enabling choice of grid slot AKA table position
const int hashFunction(Particle* p_particle)
{
    return (WORLD_SIZE / GRID_SCALE) * std::floor(p_particle->m_position.y / GRID_SCALE) + std::floor(p_particle->m_position.x / GRID_SCALE);
}

// TODO: create table of linked lists of particles and visualization grid. Table is 800 x 800, grid 10 x 10
struct HashTable
{
    
};

int main()
{
    // Window size matters, grid visualization of Hash Table and World Area both at 800 x 800
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")){ std::cout << "Couldn't load font from file";}

    sf::RectangleShape worldArea(sf::Vector2f(WORLD_SIZE + 2 * PARTICLE_RADIUS, WORLD_SIZE + 2 * PARTICLE_RADIUS));
    worldArea.setFillColor(sf::Color::Black);
    worldArea.setPosition(WORLD_ORIGIN - sf::Vector2f(PARTICLE_RADIUS, PARTICLE_RADIUS));

    Particle particle;
    
    // Main window loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear(sf::Color(0x222222FF));
        window.draw(worldArea);
        window.draw(particle.m_shape);
        window.display();
    }

    return 0;
}