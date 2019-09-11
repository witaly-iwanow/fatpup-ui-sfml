#include "colors.h"

#include "movepanel.h"

MovePanel::MovePanel(float xOff, float width, float height)
{
    _background.setPosition(xOff, 0);
    _background.setSize(sf::Vector2f(width, height));
    _background.setFillColor(MOVE_PANEL_COLOR);
    _background.setOutlineThickness(0);

    _font.loadFromFile("resources/Lekton-Bold.ttf");

    int turnCharacterSize = ((int)width) / 8;
    if (turnCharacterSize < 2)
        turnCharacterSize = 2;
    int moveCharacterSize = turnCharacterSize * 13 / 16;
    if (moveCharacterSize < 2)
        moveCharacterSize = 2;

    const sf::Glyph turnGlyph = _font.getGlyph('W', turnCharacterSize, false);
    const sf::Glyph moveGlyph = _font.getGlyph('K', moveCharacterSize, false);
    const auto xOffTurn = xOff + turnGlyph.bounds.width / 2;
    const auto yOffTurn = turnGlyph.bounds.height / 2;
    const auto xOffMoves = xOffTurn;
    const auto yOffMoves = yOffTurn + turnGlyph.bounds.height * 2;

    for (int c = White; c <= Black; ++c)
    {
        _turnIndicator[c].setFont(_font);
        _turnIndicator[c].setCharacterSize(turnCharacterSize);
        _turnIndicator[c].setPosition(xOffTurn, yOffTurn);
        _turnIndicator[c].setFillColor(c == White ? sf::Color::White : sf::Color::Black);
        _turnIndicator[c].setString(c == White ? "White turn" : "Black turn");

        _moves[c].setFont(_font);
        _moves[c].setCharacterSize(moveCharacterSize);
        _moves[c].setPosition(xOffMoves + (c == White ? 0 : moveGlyph.bounds.width * 9), yOffMoves);
        _moves[c].setFillColor(c == White ? sf::Color::White : sf::Color::Black);
    }

    const auto xOffWinDraw = xOffTurn;
    const auto yOffWinDraw = height - turnGlyph.bounds.height * 5 / 2;
    _winDrawIndicator.setFont(_font);
    _winDrawIndicator.setCharacterSize(turnCharacterSize);
    _winDrawIndicator.setPosition(xOffWinDraw, yOffWinDraw);

    _position.setInitial();
}

void MovePanel::SetPosition(const fatpup::Position& pos)
{
    for (int c = White; c <= Black; ++c)
    {
        _movesString[c].clear();
        if (!pos.isWhiteTurn() && c == White)
            _movesString[c] += "...\n";
        _moves[c].setString(_movesString[c]);
    }

    _position = pos;
    PositionUpdated();
}

void MovePanel::Move(fatpup::Move move)
{
    const auto c = (_position.isWhiteTurn() ? White : Black);
    _movesString[c] += _position.moveToString(move) + "\n";
    _moves[c].setString(_movesString[c]);

    _position += move;
    PositionUpdated();
}

void MovePanel::PositionUpdated()
{
    _positionState = _position.getState();

    if (_positionState == fatpup::Position::State::Checkmate)
    {
        _winDrawIndicator.setString(std::string(_position.isWhiteTurn() ? "Black" : "White") +  " won");
        _winDrawIndicator.setFillColor(_position.isWhiteTurn() ? sf::Color::Black : sf::Color::White);
    }
    else if (_positionState == fatpup::Position::State::Stalemate)
    {
        _winDrawIndicator.setString("Draw");
        _winDrawIndicator.setFillColor(sf::Color(128, 128, 128));
    }
}

void MovePanel::Draw(sf::RenderWindow& window) const
{
    window.draw(_background);
    if (_positionState != fatpup::Position::State::Checkmate && _positionState != fatpup::Position::State::Stalemate)
        window.draw(_turnIndicator[_position.isWhiteTurn() ? White : Black]);
    window.draw(_moves[White]);
    window.draw(_moves[Black]);

    if (_positionState == fatpup::Position::State::Checkmate || _positionState == fatpup::Position::State::Stalemate)
        window.draw(_winDrawIndicator);
}
