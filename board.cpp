#include "colors.h"
#include "movepanel.h"

#include "board.h"

Board::Board(const sf::Vector2u windowSize, bool playingWhite):
    _playingWhite(playingWhite)
{
    auto smallestDimension = std::min(windowSize.x, windowSize.y) / fatpup::BOARD_SIZE;
    if (smallestDimension < 16)
        smallestDimension = 16;
    _squareSize = smallestDimension;

    sf::RectangleShape lightSquare(sf::Vector2f(_squareSize, _squareSize));
    lightSquare.setFillColor(LIGHT_SQUARE_COLOR);
    lightSquare.setOutlineThickness(0);

    sf::RectangleShape darkSquare(sf::Vector2f(_squareSize, _squareSize));
    darkSquare.setFillColor(DARK_SQUARE_COLOR);
    darkSquare.setOutlineThickness(0);

    _selectedSquare.setSize(sf::Vector2f(_squareSize - SELECTED_BORDER_WIDTH * 2, _squareSize - SELECTED_BORDER_WIDTH * 2));
    _selectedSquare.setFillColor(sf::Color::Transparent);
    _selectedSquare.setOutlineColor(SELECTED_SQUARE_COLOR);
    _selectedSquare.setOutlineThickness(SELECTED_BORDER_WIDTH);

    _textures[fatpup::White | fatpup::King].loadFromFile("resources/WhiteKing.png");
    _textures[fatpup::White | fatpup::Queen].loadFromFile("resources/WhiteQueen.png");
    _textures[fatpup::White | fatpup::Rook].loadFromFile("resources/WhiteRook.png");
    _textures[fatpup::White | fatpup::Bishop].loadFromFile("resources/WhiteBishop.png");
    _textures[fatpup::White | fatpup::Knight].loadFromFile("resources/WhiteKnight.png");
    _textures[fatpup::White | fatpup::Pawn].loadFromFile("resources/WhitePawn.png");

    _textures[fatpup::Black | fatpup::King].loadFromFile("resources/BlackKing.png");
    _textures[fatpup::Black | fatpup::Queen].loadFromFile("resources/BlackQueen.png");
    _textures[fatpup::Black | fatpup::Rook].loadFromFile("resources/BlackRook.png");
    _textures[fatpup::Black | fatpup::Bishop].loadFromFile("resources/BlackBishop.png");
    _textures[fatpup::Black | fatpup::Knight].loadFromFile("resources/BlackKnight.png");
    _textures[fatpup::Black | fatpup::Pawn].loadFromFile("resources/BlackPawn.png");

    for (auto t: _textures)
        t.second.setSmooth(true);

    const sf::Texture& primingTexture = _textures[fatpup::White | fatpup::King];
    for (int row = 0; row < fatpup::BOARD_SIZE; ++row)
    {
        for (int col = 0; col < fatpup::BOARD_SIZE; ++col)
        {
            const auto idx = row * fatpup::BOARD_SIZE + col;

            _squares[idx] = (((row ^ col) & 1) ? darkSquare : lightSquare);
            _squares[idx].move(col * _squareSize, row * _squareSize);

            _pieces[idx] = sf::Sprite(primingTexture);
            _pieces[idx].setScale(_squareSize / primingTexture.getSize().x, _squareSize / primingTexture.getSize().y);
            _pieces[idx].move(col * _squareSize, row * _squareSize);
        }
    }

    _position.setEmpty();
}

void Board::SetMovePanel(MovePanel* movePanel)
{
    _movePanel = movePanel;
    if (_movePanel)
        _movePanel->SetPosition(_position);
}

void Board::SetPosition(const fatpup::Position& pos)
{
    if (_movePanel)
        _movePanel->SetPosition(pos);

    _position = pos;
    UpdatePieces();
}

void Board::Move(fatpup::Move move)
{
    if (_movePanel)
        _movePanel->Move(move);

    _position += move;
    UpdatePieces();
}

void Board::UpdatePieces()
{
    for (int row = 0; row < fatpup::BOARD_SIZE; ++row)
    {
        for (int col = 0; col < fatpup::BOARD_SIZE; ++col)
        {
            const auto piece = _position.square(DisplayRowToFatpup(row), DisplayColToFatpup(col)).pieceWithColor();
            if (piece)
                _pieces[row * fatpup::BOARD_SIZE + col].setTexture(_textures.at(piece));
        }
    }
}

void Board::ProcessUserMove(const int destSquareIdx)
{
    if (_selectedSquareIdx == -1)
    {
        // new square/piece selected
        const int row = DisplayRowToFatpup(destSquareIdx / fatpup::BOARD_SIZE);
        const int col = DisplayColToFatpup(destSquareIdx % fatpup::BOARD_SIZE);
        const auto piece = _position.square(row, col).pieceWithColor();
        if (piece & fatpup::PieceMask)
        {
            // at least it's not empty, check the color - only allow to pick white pieces
            // on white turn, black ones on black turn
            if (_position.isWhiteTurn() == ((piece & fatpup::ColorMask) == fatpup::White))
                _selectedSquareIdx = destSquareIdx;
        }
    }
    else if (destSquareIdx == _selectedSquareIdx)
    {
        // same square/piece clicked again, remove selection
        _selectedSquareIdx = -1;
    }
    else
    {
        const int src_row = DisplayRowToFatpup(_selectedSquareIdx / fatpup::BOARD_SIZE);
        const int src_col = DisplayColToFatpup(_selectedSquareIdx % fatpup::BOARD_SIZE);
        const int dst_row = DisplayRowToFatpup(destSquareIdx / fatpup::BOARD_SIZE);
        const int dst_col = DisplayColToFatpup(destSquareIdx % fatpup::BOARD_SIZE);
        const auto moves = _position.possibleMoves(src_row, src_col, dst_row, dst_col);

        if (!moves.empty())
            Move(moves[0]);

        _selectedSquareIdx = -1;
    }
}

void Board::OnClick(const sf::Vector2f pos)
{
    int xIdx = (int)(pos.x / _squareSize);
    int yIdx = (int)(pos.y / _squareSize);

    if (xIdx >= 0 && xIdx < fatpup::BOARD_SIZE && yIdx >= 0 && yIdx < fatpup::BOARD_SIZE)
    {
        ProcessUserMove(yIdx * fatpup::BOARD_SIZE + xIdx);

        if (_selectedSquareIdx != -1)
            _selectedSquare.setPosition(xIdx * _squareSize + SELECTED_BORDER_WIDTH, yIdx * _squareSize + SELECTED_BORDER_WIDTH);
    }
}

void Board::Draw(sf::RenderWindow& window) const
{
    int idx = 0;
    for (int row = 0; row < fatpup::BOARD_SIZE; ++row)
    {
        for (int col = 0; col < fatpup::BOARD_SIZE; ++col)
        {
            window.draw(_squares[idx]);

            if (idx == _selectedSquareIdx)
                window.draw(_selectedSquare);

            const auto piece = _position.square(DisplayRowToFatpup(row), DisplayColToFatpup(col)).pieceWithColor();
            if (piece)
                window.draw(_pieces[idx]);

            ++idx;
        }
    }
}

