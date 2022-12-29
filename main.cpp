#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <list>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 2000;
const int WINDOW_HEIGHT = 1000;
const float TARGET_FRAME_TIME = 1.f / 144.f; // target 144 FPS
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

    Particle* m_ptrNext = nullptr;

    Particle()
    :   m_position(800.f, 800.f), m_speed(-1.f, 0.f), m_acceleration(0.f, 0.f), m_radius(PARTICLE_RADIUS), m_color(sf::Color::Red)
    {
        m_shape.setRadius(m_radius);
        m_shape.setPosition(m_position);
        m_shape.setFillColor(m_color);
        m_shape.setOrigin(- WORLD_ORIGIN + sf::Vector2f(m_radius, m_radius));
    }
    
    ~Particle(){}

    // Operator overload to enable comparisons when using std::list::remove(Particle p_particle) to find the right element
    bool operator==(const Particle& other) const
    {
        // Return true if the particles are equal, false otherwise
        return  m_position == other.m_position && 
                m_speed == other.m_speed && 
                m_acceleration == other.m_acceleration && 
                m_radius == other.m_radius && 
                m_color == other.m_color;
    }

    // TODO: Implement
    void interact(const Particle &p_particle)
    {

    }

    void tick(float p_dt)
    {
        // Update the particle's position based on its speed and acceleration
        m_position += m_speed * p_dt;
        m_speed += m_acceleration * p_dt;

        // Set particle boundaries based on World frame
        if (m_position.x < 0)
            { m_position.x = 0.f;                   m_speed.x = 0.f;    m_acceleration.x = 0.f; }
        else if (m_position.x >= WORLD_SIZE)
            { m_position.x = WORLD_SIZE - 0.001f;   m_speed.x = 0.f;    m_acceleration.x = 0.f; }
        else if (m_position.y < 0)
            { m_position.y = 0.f;                   m_speed.y = 0.f;    m_acceleration.y = 0.f; }
        else if (m_position.y >= WORLD_SIZE)
            { m_position.y = WORLD_SIZE - 0.001f;   m_speed.y = 0.f;    m_acceleration.y = 0.f; }

        // Update the particle's shape to match its new position
        m_shape.setPosition(m_position);
    }

    void display(sf::RenderWindow &window)
    {
        window.draw(m_shape);
    }
};

// Hash function enabling choice of grid slot AKA table position
const int hashFunction(Particle &p_particle)
{
    return (WORLD_SIZE / GRID_SCALE) * std::floor(p_particle.m_position.y / GRID_SCALE) + std::floor(p_particle.m_position.x / GRID_SCALE);
}

// TODO: create array of linked lists of particles and visualization grid. Table is 800 x 800, grid 10 x 10
struct HashTable
{
    int m_size;
    std::vector<std::list<Particle>> m_table;

    HashTable()
    :   m_size(100)
    {
        m_table.resize(m_size);
    }

    ~HashTable(){}

    void insertItem(Particle &p_particle)
    {
        int index = hashFunction(p_particle);
        m_table[index].push_back(p_particle);
    }

    void deleteItem(Particle &p_particle)
    {
        int index = hashFunction(p_particle);
        m_table[index].remove(p_particle);
    }
};

int main()
{
    // Window size matters, grid visualization of Hash Table and World Area both at 800 x 800
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
    sf::Font font;
    if (!font.loadFromFile("resources\\arial.ttf")){ std::cout << "Couldn't load font from file";}

    sf::RectangleShape worldArea(sf::Vector2f(WORLD_SIZE + 2 * PARTICLE_RADIUS, WORLD_SIZE + 2 * PARTICLE_RADIUS));
    worldArea.setFillColor(sf::Color::Black);
    worldArea.setPosition(WORLD_ORIGIN - sf::Vector2f(PARTICLE_RADIUS, PARTICLE_RADIUS));

    float dt = 0.f;
    float elapsedTime = 0.f;
    sf::Clock clock;
    int frameCount = 0;
    sf::Text frameTimeLabel(" ms", font);

    Particle particle;
    
    // Main window loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        dt = clock.restart().asMicroseconds();
        frameCount++;
        elapsedTime += dt;

        // Display the average time taken to process a single iteration
        if (elapsedTime >= 1000000.0f)
        {
            frameTimeLabel.setString(std::to_string(0.001f * elapsedTime / frameCount) + " ms");
            elapsedTime = 0.0f; frameCount = 0;
        }

        window.clear(sf::Color(0x222222FF));
        window.draw(worldArea);
        particle.tick(dt);
        window.draw(particle.m_shape);
        window.draw(frameTimeLabel);
        window.display();
    }

    return 0;
}