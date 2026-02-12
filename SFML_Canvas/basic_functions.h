#include <algorithm>
#include <corecrt_math_defines.h>
#include <SFML/Graphics/Color.hpp>

static sf::Color rainbow_function(int _x) {
        int red, green, blue;
        double x = double(_x) / 1023 * 3 * M_PI / 2;
        if (x <= M_PI) red = int(sin(x + M_PI / 2) * 255);
        else red = int(sin(x + M_PI) * 255);
        green = int(sin(x) * 255);
        blue = int(sin(x + 3 * M_PI / 2) * 255);
        red = std::max(red, 0);
        green = std::max(green, 0);
        blue = std::max(blue, 0);
        return sf::Color(red, green, blue);
    }
