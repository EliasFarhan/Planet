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
constexpr float zoomFactor = 1.5f;

int main()
{
    planets::PlanetSystem planetSystem(planetCount);
    std::vector<sf::CircleShape> circles(planetCount);
    for(auto& circle: circles)
    {
        circle.setRadius(3.0f);
        circle.setFillColor(sf::Color::Blue);
        circle.setOrigin(3.0f, 3.0f);
    }

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
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                view.zoom((1.0f - zoomFactor * event.mouseWheelScroll.delta * dt.asSeconds()));
                window.setView(view);
            }
        }

        planetSystem.Update(dt.asSeconds());
        auto planets = planetSystem.GetPlanets();
        for(int i = 0; i < planetCount; i++)
        {
            const auto position = planetSystem.GetPosition(i);
            circles[i].setPosition(static_cast<sf::Vector2f>(planets[i].position*planets::pixelToMeter));
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