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
const int PARTICLE_SIZE = 5;
const int PART_NB_AT_START = 5;

const float FRICTION = 0.001;

// Random number generator to initialize random caracteristics.
std::random_device random_device;
std::mt19937 gen(random_device());
std::uniform_real_distribution<> random_dist(0, 799);

// Create random number with : rand_nb()
// std::bind requires #include <functional>
auto rand_nb = std::bind(random_dist, gen);

struct Particle
{
    int zone;
    sf::Vector2f position;
    sf::Vector2f speed;
    sf::Vector2f acceleration;

    sf::CircleShape shape;
    sf::Color color;

    Particle(sf::Color m_color, float m_x, float m_y)
        : shape(PARTICLE_SIZE), position(sf::Vector2f(m_x, m_y)), color(m_color), speed(0, 1), acceleration(0, 0), zone((position.x + position.y * ZONE_GRID_WIDTH) / ZONE_SCALE)
    {
        shape.setFillColor(color);
        shape.setOrigin(PARTICLE_SIZE, PARTICLE_SIZE);
        shape.setPosition(position + sf::Vector2f(FRAME_X_OFFSET + 1, FRAME_Y_OFFSET + 1));
    }

    // TODO: TICK
    void tick()
    {
        speed *= (1 - FRICTION);
        speed += acceleration;
        position += speed;

        if (position.x >= FRAME_WIDTH)
            position.x -= FRAME_WIDTH;
        else if (position.x < 0)
            position.x += FRAME_WIDTH;

        if (position.y >= FRAME_HEIGHT)
            position.y -= FRAME_HEIGHT;
        else if (position.y < 0)
            position.y += FRAME_HEIGHT;

        shape.setPosition(position + sf::Vector2f(FRAME_X_OFFSET + 1, FRAME_Y_OFFSET + 1));
        zone = (position.x + position.y * ZONE_GRID_WIDTH) / ZONE_SCALE;
    }

    void render(sf::RenderWindow &window) { window.draw(shape); }
};

// TODO: Look for other particles in neighboring zones for interactions
// [-1,-1] [ 0,-1] [ 1,-1]
// [-1, 0] [ 0, 0] [ 1, 0]
// [-1, 1] [ 0, 1] [ 1, 1]
void update(std::vector<Particle> *m_container_zones)
{
    int zone;

    for (int i = 0; i < ZONE_GRID_WIDTH * ZONE_GRID_HEIGHT; i++)
    {
        if (m_container_zones[i].size() == 0)
            continue;

        for (int j = 0; j < m_container_zones[i].size(); j++)
        {
            std::cout << "ZONE " << i << "  P" << j << "  X : " << m_container_zones[i][j].position.x << "  Y : " << m_container_zones[i][j].position.y << std::endl;
            m_container_zones[i][j].acceleration = sf::Vector2f(0, 0);

            // for (int l = -1; l <= 1; l++)
            // {
            //     for (int m = -1; m <= 1; m++)
            //     {
            //         if (i + l + (m * ZONE_GRID_WIDTH) < 0)
            //             zone = i + l + (m * ZONE_GRID_WIDTH) + ZONE_GRID_WIDTH * ZONE_GRID_HEIGHT;
            //         else if (i + l + (m * ZONE_GRID_WIDTH) >= ZONE_GRID_WIDTH * ZONE_GRID_HEIGHT)
            //             zone = i + l + (m * ZONE_GRID_WIDTH) - ZONE_GRID_WIDTH * ZONE_GRID_HEIGHT;
                    
            //         for (int id = 0; id < m_container_zones[zone].size(); id++)
            //         {
            //             if (zone == i && id == j)
            //                 continue;

            //             m_container_zones[i][j].acceleration += sf::Vector2f(   0.01 * (m_container_zones[i][j].position.x - m_container_zones[zone][id].position.x),
            //                                                                     0.01 * (m_container_zones[i][j].position.y - m_container_zones[zone][id].position.y));
            //         }
            //     }
            // }

            // tick all particles to update positions
            m_container_zones[i][j].tick();

            // If required update particle Zone to re-enable selective interactions
            if (m_container_zones[i][j].zone != i)
            {
                m_container_zones[m_container_zones[i][j].zone].emplace_back(m_container_zones[i][j]);
                m_container_zones[i].erase(m_container_zones[i].begin() + j);
            }
        }
    }

    std::cout << std::endl;
}

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

        // std::cout << randX << "     " << randY << "     " << zoneX << "     " << zoneY << std::endl;
        container_zones[(randX + randY * ZONE_GRID_WIDTH) / ZONE_SCALE].emplace_back(sf::Color::Red, randX, randY);
    }
    
    // Start window loop
    // Separate Window loop and simulation loop
    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();    
        
        update(container_zones);

        window.clear(window_background);
        window.draw(frame);

        // Render all particles
        for (int i = 0; i < ZONE_GRID_WIDTH * ZONE_GRID_HEIGHT; i++)
            std::for_each(container_zones[i].begin(), container_zones[i].end(), [&window, &container_zones](Particle p) { p.render(window); });

        window.display();
    }

    return EXIT_SUCCESS;
}