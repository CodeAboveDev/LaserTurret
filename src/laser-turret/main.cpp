#include <modm/board.hpp>

int main()
{
    Board::initialize();
    Board::Leds::setOutput();

    using Laser = GpioOutputA5;
    Laser::setOutput();

    while (true)
    {
        Board::Leds::toggle();
        Laser::toggle();
        modm::delay_ms(Board::Button::read() ? 250 : 500);
    }

    return 0;
}
