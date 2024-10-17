//
// Created by efarhan on 1/27/23.
//
#include <numbers>

#include "vec.h"
#include "planet.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Time.hpp>

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#include <tracy/TracyC.h>
#endif


constexpr unsigned int width = 1280;
constexpr unsigned int height = 720;
constexpr auto planetCount = 10'000;
constexpr float zoomFactor = 1.5f;
constexpr auto circleResolution = 12;
constexpr auto circleRadius = 3.0f;
constexpr auto twoPi = 2.0f * std::numbers::pi_v<float>;

int main()
{
    planets::PlanetSystem4 planetSystem(planetCount);

	std::vector<planets::Vec2f> circleVertices;
	circleVertices.reserve(circleResolution);
	for (int i = 1; i < circleResolution; i++)
	{
		circleVertices.emplace_back(std::cos(float(i) * twoPi / circleResolution), std::sin(float(i) * twoPi / circleResolution));
	}

	sf::VertexArray circles;
	circles.setPrimitiveType(sf::Triangles);
	circles.resize(planetCount*(circleResolution*3));
	for(std::size_t i = 0; i < circles.getVertexCount(); i++)
	{
		circles[i].color = sf::Color::Blue;
	}


    sf::RenderWindow window(sf::VideoMode(width, height), "Planets");
    //window.setFramerateLimit(5);
    sf::Clock clock;
    sf::View view = window.getView();
    while (window.isOpen())
    {
        const auto dt = clock.restart();
#ifdef TRACY_ENABLE
        TracyCZoneN(eventHandle, "Event Management", true);
#endif
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
#ifdef TRACY_ENABLE
        TracyCZoneEnd(eventHandle);
#endif
        planetSystem.Update(dt.asSeconds());

#ifdef TRACY_ENABLE
        TracyCZoneN(moveCircles, "Move Circles", true);
#endif
        for(int i = 0; i < planetCount; i++)
        {
            const auto position = planetSystem.GetPosition(i);
        	const auto currentIndex = circleResolution*3*i;
        	const auto pixelPosition = static_cast<sf::Vector2f>(position*planets::pixelToMeter);
        	for(int j = 0; j < circleResolution; j++)
        	{
        		circles[currentIndex+j*3].position = pixelPosition;
        		circles[currentIndex+j*3+1].position = pixelPosition+static_cast<sf::Vector2f>(circleVertices[j]*circleRadius);
        		circles[currentIndex+j*3+2].position = pixelPosition+static_cast<sf::Vector2f>(circleVertices[(j+1)%circleResolution]*circleRadius);
        	}

        }
#ifdef TRACY_ENABLE
    	TracyCZoneEnd(moveCircles);

    	TracyCZoneN(draw, "Draw", true);
#endif
        window.clear();

        window.draw(circles);

#ifdef TRACY_ENABLE
    	TracyCZoneEnd(draw);
#endif
        window.display();

#ifdef TRACY_ENABLE
        FrameMark;
#endif
    }
    return 0;
}