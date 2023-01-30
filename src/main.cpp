//
// Created by efarhan on 1/27/23.
//
#include "vec.h"
#include "planet.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Time.hpp>

constexpr unsigned int width = 1280;
constexpr unsigned int height = 720;
constexpr auto planetCount = 1024;

int main()
{
    planets::PlanetSystem planetSystem(planetCount);
    std::vector<sf::CircleShape> circles(planetCount);

    sf::RenderWindow window(sf::VideoMode(width, height), "Planets");
    //window.setFramerateLimit(5);
    sf::Clock clock;
    sf::View view = window.getView();
    while (window.isOpen())
    {
        const auto dt = clock.restart();
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::Resized)
            {
                view = sf::View({ 0,0 }, { static_cast<float>(event.size.width), static_cast<float>(event.size.height) });
                window.setView(view);
            }
        }

        planetSystem.Update(dt.asSeconds());
        auto planets = planetSystem.GetPlanets();
        for(int i = 0; i < planetCount; i++)
        {
            circles[i].setPosition(static_cast<sf::Vector2f>(planets[i].position));
        }

        window.clear();
        for(const auto& circle: circles)
        {
            window.draw(circle);
        }
        window.display();
    }
    return 0;
}