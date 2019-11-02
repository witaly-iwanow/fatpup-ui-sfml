#include <iostream>

#include "SFML/Graphics.hpp"

#include "board.h"
#include "movepanel.h"

sf::View GetLetterboxView(sf::View view, int windowWidth, int windowHeight)
{
    float windowRatio = windowWidth / (float) windowHeight;
    float viewRatio = view.getSize().x / (float) view.getSize().y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;

    bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
        horizontalSpacing = false;

    // If horizontalSpacing is true, the black bars will appear on the left and right side.
    // Otherwise, the black bars will appear on the top and bottom.

    if (horizontalSpacing)
    {
        sizeX = viewRatio / windowRatio;
        //posX = (1 - sizeX) / 2.f;
    }
    else
    {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }

    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

    return view;
}

int main()
{
    fatpup::Engine* engine = fatpup::Engine::Create("minimax");
    if (!engine)
    {
        std::cerr << "Can't create minimax engine, terminating...\n";
        return -1;
    }

    const int windowHeight = (std::min(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height) * 3 / 4) & 0xfff0;
    const int windowWidth = windowHeight * 5 / 4;
    const int targetWindowHeight = 1200;
    const int targetWindowWidth = targetWindowHeight * 5 / 4;

    // create the window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Fatpup Chess");
    window.setFramerateLimit(10);
    // SFML docs don't recommend setting setFramerateLimit and setVerticalSyncEnabled(true) simultaneously
    //window.setVerticalSyncEnabled(true);

    sf::View view;
    view.setSize(targetWindowWidth, targetWindowHeight);
    view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
    view = GetLetterboxView(view, windowWidth, windowHeight);

    Board board(sf::Vector2u(targetWindowWidth, targetWindowHeight), true);
    fatpup::Position initialPos;
    initialPos.setInitial();
    board.SetPosition(initialPos);
    engine->SetPosition(initialPos);
    board.SetEngine(engine);

    MovePanel movePanel(board.GetSize().x, board.GetSize().x / 4, board.GetSize().y);
    board.SetMovePanel(&movePanel);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                else if (event.key.code == sf::Keyboard::R)
                    board.SetPosition(initialPos);
            }
            else if (event.type == sf::Event::Resized)
                view = GetLetterboxView(view, event.size.width, event.size.height);
            else if (event.type == sf::Event::MouseButtonPressed)
                board.OnClick(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        window.setView(view);
        board.Draw(window);
        movePanel.Draw(window);

        // end the current frame
        window.display();
    }

    delete engine;

    return 0;
}
