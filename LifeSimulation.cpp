#include <iostream>
#include <vector>
#include <random>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Display parameters
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;
const int FRAME_WIDTH = 800;
const int FRAME_HEIGHT = 800;
const int ZONE_SCALE = 80;
const int PARTICLE_SIZE = 2;
const int FRAME_X_OFFSET = WINDOW_WIDTH - (FRAME_WIDTH + 50);
const int FRAME_Y_OFFSET = WINDOW_HEIGHT - (FRAME_HEIGHT + 50);

// Random number generator to initialize random caracteristics.
std::random_device random_device;
std::mt19937 gen(random_device());
std::uniform_real_distribution<> random_dist(0, 799);
// Create random number with : random_number = random_dist(gen);

struct Particle
{
    sf::Vector2f position;
    sf::Vector2f speed;
    sf::CircleShape shape;
    sf::Color color;

    Particle(sf::Color m_color, float m_x, float m_y)
        : shape(PARTICLE_SIZE), position(sf::Vector2f(m_x, m_y)), color(m_color)
    {
        shape.setFillColor(color);
        shape.setOrigin(PARTICLE_SIZE, PARTICLE_SIZE);
        shape.setPosition(position);
    }

    void tick()
    {

    }

    void render(sf::RenderWindow &window) { window.draw(shape); }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Life Simulation");
    sf::RectangleShape frame(sf::Vector2f(FRAME_WIDTH, FRAME_HEIGHT));
    frame.setFillColor(sf::Color::Black);
    frame.setPosition(FRAME_X_OFFSET, FRAME_Y_OFFSET);
    sf::Color window_background = sf::Color(30, 30, 30);

    // Init 2D Array of zones containing vector of particles
    // Enables local search for proximity interactions
    std::vector<Particle> container_zones[FRAME_WIDTH / ZONE_SCALE][FRAME_HEIGHT / ZONE_SCALE];

    for (int i = 0; i < 10; i++)
    {
        sf::Vector2f rand_pos(random_dist(gen),random_dist(gen));
        container_zones[(int)(rand_pos.x / ZONE_SCALE)][(int)(rand_pos.y / ZONE_SCALE)].emplace_back(sf::Color::Red, rand_pos.x + FRAME_X_OFFSET, rand_pos.y + FRAME_Y_OFFSET);
        std::cout << rand_pos.x << " " << rand_pos.y << '\n';
        std::cout << (int)(rand_pos.x / ZONE_SCALE) << " " << (int)(rand_pos.y / ZONE_SCALE) << '\n';
        std::cout << container_zones[(int)(rand_pos.x / ZONE_SCALE)][(int)(rand_pos.y / ZONE_SCALE)].size() << "\n\n";
    }
  
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();    
        
        window.clear(window_background);
        window.draw(frame);

        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                if (container_zones[i, j]->size() > 0)
                    std::for_each(container_zones[i, j]->begin(), container_zones[i, j]->end(), [&window](Particle &p){ p.render(window); });

        window.display();
    }

    return EXIT_SUCCESS;
}