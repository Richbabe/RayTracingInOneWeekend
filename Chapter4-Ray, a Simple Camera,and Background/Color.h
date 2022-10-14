#pragma once

#include "Vector3.h"

#include <iostream>

void WriteColor(std::ostream &out, const Color& PixelColor) 
{
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999f * PixelColor.X()) << ' '
        << static_cast<int>(255.999f * PixelColor.Y()) << ' '
        << static_cast<int>(255.999f * PixelColor.Z()) << '\n';
}