#ifndef H_COLOURIZE
#define H_COLOURIZE

#include <array>
#include <cstdint>
#include <vector>

struct Color {
    std::array<uint8_t, 3> rgb; // RGB color
};

struct ByteColorMapping {
    uint8_t startByte;
    uint8_t endByte;
    Color color;

    ByteColorMapping(uint8_t s, uint8_t e, Color c) : startByte(s), endByte(e), color(c) {}
};

class Colourize {
public:
    Colourize() = default;
    ~Colourize() = default;

    static Color getColour(uint8_t byte);

private:
    static const std::vector<ByteColorMapping> colorMappings;
};

#endif  // H_COLOURIZE
