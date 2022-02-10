#include <modm/board.hpp>

int main()
{
    Board::initialize();
    Board::Leds::setOutput();

    while (true)
    {
        Board::Leds::toggle();
        modm::delay_ms(Board::Button::read() ? 250 : 500);
    }
    
    return 0;
}
