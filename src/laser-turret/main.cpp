#include <modm/board.hpp>

int main()
{
    Board::initialize();
    Board::Leds::setOutput();

    /***** LASER DIODE *****/
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

    /***** SERVOS *****/
    using BottomServo = GpioOutputA9;
    using TopServo = GpioOutputA8;

    Timer1::connect<BottomServo::Ch2, TopServo::Ch1>();
    Timer1::enable();
    Timer1::enableOutput();
    Timer1::setMode(Timer1::Mode::UpCounter);

    // Timer1::setPeriod<Board::SystemClock>(20000);
    Timer1::setPrescaler(84);
    Timer1::setOverflow(20000);
    Timer1::configureOutputChannel(1, Timer1::OutputCompareMode::Pwm, 0);
    Timer1::configureOutputChannel(2, Timer1::OutputCompareMode::Pwm, 0);

    Timer1::applyAndReset();
    Timer1::start();

    Timer1::setCompareValue(1, 1500);
    Timer1::setCompareValue(2, 1500);
    uint8_t i = 0u;

    while (true)
    {
        Board::Leds::toggle();
        // Laser::toggle();
        modm::delay_ms(1000);
        brightness += 5000u;
        Timer2::setCompareValue(1, brightness);

        switch(i)
        {
            case 0:
                Timer1::setCompareValue(1, 2400);
                Timer1::setCompareValue(2, 2400);
                break;
            case 1:
                Timer1::setCompareValue(1, 1500);
                Timer1::setCompareValue(2, 1500);
                break;
            case 2:
                Timer1::setCompareValue(1, 2400);
                Timer1::setCompareValue(2, 600);
                break;
            case 3:
                Timer1::setCompareValue(1, 1500);
                Timer1::setCompareValue(2, 1500);
                break;
            default:
                break;
        }
        i++;
        if(i>3) i=0u;
    }

    return 0;
}
