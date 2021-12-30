#include "M5Atom.h"

#define DIR_L (1)
#define DIR_R (2)
#define DIR_FRONT (3)
#define DIR_BACK (4)
#define VIRTUAL_POSMAX (5)

class LEDWalker
{
private:
    uint8_t _xpos;
    uint8_t _ypos;

public:
    LEDWalker(uint8_t xpos = 0, uint8_t ypos = 0);
    void updatePos(int direction);
    void printPos(void);
    void draw(void);
};
