#include "BodyCreator.hpp"
#include "StaticBody/StaticBody.hpp"
#include <SFML/Graphics.hpp>

#include <functional>
#include <iostream>

#include <sstream>
#include <fstream>

#include <imgui-sfml/imgui-SFML.h>

BodyCreator::BodyCreator(unsigned int width, unsigned int height, unsigned int fps)
    : m_Width(width), m_Height(height), m_Fps(fps),
      m_Window(sf::VideoMode(width, height), "Body Creator")
{
    m_Window.setFramerateLimit(fps);
    ImGui::SFML::Init(m_Window);
}

void BodyCreator::Run()
{
    sf::Thread updateThread(std::bind(&Update, this));
    updateThread.launch();

    sf::Time lastUpdate = m_Clock.getElapsedTime();
    int frameTime = 1000/(float)m_Fps;
    while (m_Window.isOpen())
    {
        if ((m_Clock.getElapsedTime()-lastUpdate).asMilliseconds() > frameTime)
        {
            lastUpdate = m_Clock.getElapsedTime();
            m_Mutex.lock();
            Render();
            PollEvents();
            m_Mutex.unlock();
        }
    }
}

void BodyCreator::Update()
{
    sf::Time lastUpdate = m_Clock.getElapsedTime();
    float updateTime = 1000.0f/(float)m_Fps;
    float deltaTime = 1.0f/(float)m_Fps;

    while (m_Window.isOpen())
    {
        if ((m_Clock.getElapsedTime()-lastUpdate).asMilliseconds() > updateTime)
        {
            lastUpdate = m_Clock.getElapsedTime();
            m_Mutex.lock();
            m_StaticBody.Update(deltaTime, &m_Window);
            m_Mutex.unlock();
        }
    }
}

void BodyCreator::SaveBody(const std::string& filepath)
{
    std::stringstream buffer;
    buffer << "resources/" << filepath << ".body";
    std::ofstream filestream(buffer.str());
    filestream << m_StaticBody;
}


