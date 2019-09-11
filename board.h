#ifndef FATPUP_UI_BOARD_H
#define FATPUP_UI_BOARD_H

#include <map>

#include "SFML/Graphics.hpp"

#include "fatpup/position.h"

class MovePanel;

class Board
{
public:
    explicit Board(const sf::Vector2u windowSize, bool playingWhite = true);

    void SetMovePanel(MovePanel* movePanel);
    void SetPosition(const fatpup::Position& pos);
    void Move(fatpup::Move move);

    void OnClick(const sf::Vector2f pos);

    void Draw(sf::RenderWindow& window) const;

    sf::Vector2f GetSize() const { return sf::Vector2f(fatpup::BOARD_SIZE * _squareSize, fatpup::BOARD_SIZE * _squareSize); }

private:
    void UpdatePieces();
    void ProcessUserMove(const int destSquareIdx);

    inline int DisplayRowToFatpup(int row) const { return (_playingWhite ? (fatpup::BOARD_SIZE - 1 - row) : row); }
    inline int DisplayColToFatpup(int col) const { return (_playingWhite ? col : (fatpup::BOARD_SIZE - 1 - col)); }

    sf::RectangleShape _squares[fatpup::BOARD_SIZE * fatpup::BOARD_SIZE];
    mutable sf::Sprite _pieces[fatpup::BOARD_SIZE * fatpup::BOARD_SIZE];
    sf::RectangleShape _selectedSquare;
    int _selectedSquareIdx = -1;

    std::map<unsigned char, sf::Texture> _textures;
    fatpup::Position _position;
    MovePanel* _movePanel = nullptr;

    float _squareSize;
    bool _playingWhite;

    static constexpr float SELECTED_BORDER_WIDTH = 8.0f;
};

#endif // #define FATPUP_UI_BOARD_H
