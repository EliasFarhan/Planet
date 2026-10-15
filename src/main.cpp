//
// Created by efarhan on 1/27/23.
//
#include <numbers>
#include <optional>
#include <vector>

#include "vec.h"
#include "planet.h"
#include "sfml_utils.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/View.hpp>
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
    for (int i = 0; i < circleResolution; i++)
    {
        circleVertices.emplace_back(std::cos(float(i) * twoPi / circleResolution), std::sin(float(i) * twoPi / circleResolution));
    }

    std::vector<sf::Vertex> circles(planetCount*(circleResolution*3));
    for(auto& vertex : circles)
    {
        vertex.color = sf::Color::Blue;
    }

    sf::RenderWindow window(sf::VideoMode({width, height}), "Planets");
    //window.setFramerateLimit(5);

    // The vertex buffer needs the window's OpenGL context, so it is created after the window
    sf::VertexBuffer circleBuffer(sf::PrimitiveType::Triangles, sf::VertexBuffer::Usage::Stream);
    const bool useVertexBuffer = sf::VertexBuffer::isAvailable() && circleBuffer.create(circles.size());

    sf::Clock clock;
    sf::View view = window.getView();
    while (window.isOpen())
    {
        const auto dt = clock.restart();
#ifdef TRACY_ENABLE
        TracyCZoneN(eventHandle, "Event Management", true);
#endif
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                view = sf::View({ 0.0f, 0.0f }, sf::Vector2f(resized->size));
                window.setView(view);
            }
            else if (const auto* scrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                view.zoom((1.0f - zoomFactor * scrolled->delta * dt.asSeconds()));
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
            const auto pixelPosition = planets::ToSf(position*planets::pixelToMeter);
            for(int j = 0; j < circleResolution; j++)
            {
                circles[currentIndex+j*3].position = pixelPosition;
                circles[currentIndex+j*3+1].position = pixelPosition+planets::ToSf(circleVertices[j]*circleRadius);
                circles[currentIndex+j*3+2].position = pixelPosition+planets::ToSf(circleVertices[(j+1)%circleResolution]*circleRadius);
            }

        }
#ifdef TRACY_ENABLE
        TracyCZoneEnd(moveCircles);

        TracyCZoneN(upload, "Upload Circles", true);
#endif
        const bool uploaded = useVertexBuffer && circleBuffer.update(circles.data());
#ifdef TRACY_ENABLE
        TracyCZoneEnd(upload);

        TracyCZoneN(draw, "Draw", true);
#endif
        window.clear();

        if (uploaded)
        {
            window.draw(circleBuffer);
        }
        else
        {
            window.draw(circles.data(), circles.size(), sf::PrimitiveType::Triangles);
        }

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
