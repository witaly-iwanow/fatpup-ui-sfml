#ifndef FATPUP_UI_COLORS_H
#define FATPUP_UI_COLORS_H

#include "SFML/Graphics.hpp"

static const sf::Color LIGHT_SQUARE_COLOR(255, 219, 153);
static const sf::Color DARK_SQUARE_COLOR(145, 94, 55);
static const sf::Color SELECTED_SQUARE_COLOR(255, 81, 0);

static const sf::Color MOVE_PANEL_COLOR((LIGHT_SQUARE_COLOR.r + DARK_SQUARE_COLOR.r) / 2,
                                        (LIGHT_SQUARE_COLOR.g + DARK_SQUARE_COLOR.g) / 2,
                                        (LIGHT_SQUARE_COLOR.b + DARK_SQUARE_COLOR.b) / 2);

#endif // #define FATPUP_UI_COLORS_H
