#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

const int FRAME_WIDTH = 600;
const int FRAME_HEIGHT = 600;
const int PARTICLE_SIZE = 1;

struct Particle
{
    sf::Vector2f pos;
    sf::RectangleShape shape;
    sf::Color color;

    Particle(sf::Color m_color)
        : shape(sf::Vector2f(PARTICLE_SIZE, PARTICLE_SIZE)), color(m_color)
    {
        shape.setFillColor(color);
    }

    void update() {}

    void render(sf::RenderWindow &window) { window.draw(shape); }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Life Simulation");
    sf::RectangleShape frame(sf::Vector2f(FRAME_WIDTH, FRAME_HEIGHT));
    frame.setOutlineThickness(1);
    frame.setOutlineColor(sf::Color::Blue);
    frame.setFillColor(sf::Color::Black);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            frame.setPosition(window.getSize().x - 650, window.getSize().y - 650);

            window.clear(sf::Color::Black);
            window.draw(frame);
            window.display();
        }
    }

    return EXIT_SUCCESS;
}