#ifndef FATPUP_UI_MOVEPANEL_H
#define FATPUP_UI_MOVEPANEL_H

#include <map>

#include "SFML/Graphics.hpp"

#include "fatpup/position.h"

class MovePanel
{
public:
    explicit MovePanel(float xOff, float width, float height);

    void SetPosition(const fatpup::Position& pos);
    void Move(fatpup::Move move);

    void Draw(sf::RenderWindow& window) const;

private:
    void PositionUpdated();

    fatpup::Position _position;
    fatpup::Position::State _positionState;

    sf::RectangleShape _background;
    sf::Font _font;

    enum
    {
        White = 0,
        Black = 1
    };
    sf::Text _turnIndicator[2];
    sf::Text _moves[2];
    std::string _movesString[2];
    sf::Text _winDrawIndicator;
};

#endif // #define FATPUP_UI_MOVEPANEL_H
