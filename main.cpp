#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

struct Particle
{
    sf::Vector2f position;
    sf::Vector2f speed;
    sf::Vector2f acceleration;
    
    sf::CircleShape shape;
    sf::Vector2f radius;
    sf::Color color;


};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 800), "SFML works!");
    Particle* particle = new Particle;

    // Main window loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();
        //window.draw(a.m_shape);
        window.display();
    }

    return 0;
}