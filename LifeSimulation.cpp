#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Display parameters
const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;
const int FRAME_WIDTH = 800;
const int FRAME_HEIGHT = 800;
const int ZONE_SCALE = 80;
const int FRAME_X_OFFSET = WINDOW_WIDTH - (FRAME_WIDTH + 50);
const int FRAME_Y_OFFSET = WINDOW_HEIGHT - (FRAME_HEIGHT + 50);
const int ZONE_GRID_WIDTH = FRAME_WIDTH / ZONE_SCALE;
const int ZONE_GRID_HEIGHT = FRAME_HEIGHT / ZONE_SCALE;

// Particles parameters
const int PARTICLE_SIZE = 1;
const int PART_NB_AT_START = 1000;

// Random number generator to initialize random caracteristics.
std::random_device random_device;
std::mt19937 gen(random_device());
std::uniform_real_distribution<> random_dist(0, 799);
// Create random number with : rand_nb()
auto rand_nb = std::bind(random_dist, gen);

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

    // Init 1D Array of zones containing vector of particles 
    // Enables local search for proximity interactions
    std::vector<Particle> container_zones[ZONE_GRID_WIDTH * ZONE_GRID_HEIGHT];

    // Generate a certain number of particles
    for (int i = 0; i < PART_NB_AT_START; i++)
    {
        int randX = rand_nb();
        int randY = rand_nb();

        int zoneX = randX / ZONE_SCALE;
        int zoneY = randY / ZONE_SCALE;

        randX += FRAME_X_OFFSET + 1;
        randY += FRAME_Y_OFFSET + 1;

        // std::cout << randX << "     " << randY << "     " << zoneX << "     " << zoneY << std::endl;
        container_zones[zoneX + zoneY * ZONE_GRID_WIDTH].emplace_back(sf::Color::Red, randX, randY);
    }
    
    // Start window loop
    // Separate Window loop and simulation loop
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();    
        
        window.clear(window_background);
        window.draw(frame);

        // Render all particles
        for (int i = 0; i < ZONE_GRID_WIDTH * ZONE_GRID_HEIGHT; i++)
            std::for_each(container_zones[i].begin(), container_zones[i].end(), [&window](Particle p){ p.render(window); });

        window.display();
    }

    return EXIT_SUCCESS;
}