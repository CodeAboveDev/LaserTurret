#include <modm/board.hpp>

int main()
{
    Board::initialize();
    Board::Leds::setOutput();

    using Laser = GpioOutputA5;
    // Laser::setOutput();

    Timer2::connect<Laser::Ch1>();
    Timer2::enable();
    Timer2::setMode(Timer2::Mode::UpCounter);

    Timer2::setPrescaler(8);
	Timer2::setOverflow(65535);
    Timer2::configureOutputChannel(1, Timer2::OutputCompareMode::Pwm, 0);

    Timer2::applyAndReset();
    Timer2::start();

    uint16_t brightness = 0u;
    while (true)
    {
        Board::Leds::toggle();
        // Laser::toggle();
        modm::delay_ms(Board::Button::read() ? 250 : 500);
        brightness += 5000u;
        Timer2::setCompareValue(1, brightness);
    }

    return 0;
}
