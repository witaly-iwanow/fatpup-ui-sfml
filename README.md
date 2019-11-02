<p float="left">
    <img src="resources/WhiteKing.png" width=40 />
    <img src="resources/WhiteQueen.png" width=40 />
    <img src="resources/WhiteRook.png" width=40 />
    <img src="resources/WhiteBishop.png" width=40 />
    <img src="resources/WhiteKnight.png" width=40 />
    <img src="resources/WhitePawn.png" width=40 />
    <img src="resources/BlackKing.png" width=40 />
    <img src="resources/BlackQueen.png" width=40 />
    <img src="resources/BlackRook.png" width=40 />
    <img src="resources/BlackBishop.png" width=40 />
    <img src="resources/BlackKnight.png" width=40 />
    <img src="resources/BlackPawn.png" width=40 />
</p>

Sample UI for fatpup chess library based on SFML 2. It's currently a user - engine game using a simple 3-ply minimax chess engine. You always play white.

## Get the code
fatpup chess engine is submodule'd, so you have to pull it separately:

    git clone https://github.com/witaly-iwanow/fatpup-ui-sfml.git
    cd fatpup-ui-sfml
    git submodule update --init --recursive

## Build (macOS)
Install SFML with Homebrew:

    brew install sfml

Throw it under CMake, make it and run:

    mkdir _cmake && cd _cmake
    cmake ..
    make && ./fatpup-ui

or if you need an Xcode project:

    cmake -GXcode ..
    Open fatpup-ui.xcodeproj

## Build (Linux)
<coming up>

![Screenshot](screenshots/chess-game-ui.png)
