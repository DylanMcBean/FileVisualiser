#include "Colourize.h"

const std::vector<ByteColorMapping> Colourize::colorMappings = {
    {0x00, 0x00, {{0, 0, 0}}},             // Null Byte - Black
    {0x01, 0x1F, {{50,200,100}}},          // Control Characters - Light Green
    {0x20, 0x20, {{200,200,200}}},         // Space - Light Gray
    {0x21, 0x2F, {{100,200,250}}},         // Punctuation (part 1) - Light Blue
    {0x30, 0x39, {{240,240,100}}},         // Numbers - Yellow
    {0x3A, 0x40, {{100,200,250}}},         // Punctuation (part 2) - Light Blue
    {0x41, 0x5A, {{50,100,200}}},          // Uppercase ASCII Characters - Dark Blue
    {0x5B, 0x60, {{100,200,250}}},         // Punctuation (part 3) - Light Blue
    {0x61, 0x7A, {{50,150,250}}},          // Lowercase ASCII Characters - Mid Blue
    {0x7B, 0x7E, {{100,200,250}}},         // Punctuation (part 4) - Light Blue
    {0x7F, 0x7F, {{50,200,100}}},          // DEL Control Character - Green
    {0x80, 0xFE, {{200,100,50}}},          // Extended ASCII Characters - Red
    {0xFF, 0xFF, {{255,255,255}}}          // Non-ASCII Characters - White
};

Color Colourize::getColour(uint8_t byte) {
    for (const auto& mapping : colorMappings) {
        if (byte >= mapping.startByte && byte <= mapping.endByte) {
            return mapping.color;
        }
    }
    return {{255, 0, 0}};
}