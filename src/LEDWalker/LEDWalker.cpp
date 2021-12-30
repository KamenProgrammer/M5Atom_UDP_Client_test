#include "LEDWalker.h"

LEDWalker::LEDWalker(uint8_t xpos, uint8_t ypos)
{
    _xpos = xpos;
    _ypos = ypos;
};

void LEDWalker::updatePos(int direction)
{
    switch (direction)
    {
    case DIR_L:
        _xpos--;
        if (_xpos > VIRTUAL_POSMAX)
        {
            _xpos = VIRTUAL_POSMAX;
        }
        break;
    case DIR_R:
        _xpos++;
        _xpos %= (VIRTUAL_POSMAX + 1);
        break;
    case DIR_FRONT:
        _ypos--;
        if (_ypos > VIRTUAL_POSMAX)
        {
            _ypos = VIRTUAL_POSMAX;
        }
        break;
    case DIR_BACK:
        _ypos++;
        _ypos %= (VIRTUAL_POSMAX + 1);
        break;
    default:
        break;
    }
};

void LEDWalker::printPos(void)
{
    Serial.printf("%d,%d\r\n", _xpos, _ypos);
}

void LEDWalker::draw(void)
{
    M5.dis.drawpix(_xpos, _ypos, 0xf5f5f5);
}
