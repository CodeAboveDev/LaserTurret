#include "laser.h"
#include "servo.h"

#include <modm/board.hpp>
#include <modm/driver/display/hd44780.hpp>
#include <modm/driver/gpio/pca8574.hpp>
    
    using LcdI2c = I2cMaster1;

	using GpioExpander = modm::Pca8574<LcdI2c>;
	GpioExpander gpioExpander;

	// Instances for each pin
	using expRs = GpioExpander::P0< gpioExpander >;
	using expRw = GpioExpander::P1< gpioExpander >;
	using expE = GpioExpander::P2< gpioExpander > ;
	using expBacklight = GpioExpander::P3< gpioExpander > ;
	using expPin4 = GpioExpander::P4< gpioExpander > ;
	using expPin5 = GpioExpander::P5< gpioExpander > ;
	using expPin6 = GpioExpander::P6< gpioExpander > ;
	using expPin7 = GpioExpander::P7< gpioExpander > ;
	using expData4BitGpio = GpioExpander::Port< gpioExpander, GpioExpander::Pin::P4, 4 >;

int main()
{
    Board::initialize();
    Board::Leds::setOutput();

    /***** LASER DIODE *****/
    Timer2::connect<GpioA5::Ch1>();
    Timer2::enable();
    Timer2::setMode(Timer2::Mode::UpCounter);

    Timer2::setPrescaler(84);
    Timer2::setOverflow(100);
    Timer2::configureOutputChannel(1, Timer2::OutputCompareMode::Pwm, 0);

    Timer2::applyAndReset();
    Timer2::start();
    Laser<Timer2> laser{1};
    
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

    /***** LCD *****/
    using LcdScl = GpioB6;
    using LcdSda = GpioB7;

    LcdI2c::connect<LcdScl::Scl, LcdSda::Sda>();
    LcdI2c::initialize<Board::SystemClock, 420_kHz>();

	modm::Hd44780< expData4BitGpio, expRw, expRs, expE  > lcd{16,2};

	expBacklight::setOutput();
	expE::setOutput();
	expRs::setOutput();
	expRw::setOutput();
	expData4BitGpio::setOutput();

	lcd.initialize();
	lcd.setCursor(0, 0);

	lcd << "Laser Turret\n";

    uint8_t brightness{0u};
    while (true)
    {
        Board::Leds::toggle();
        // Laser::toggle();
        modm::delay_ms(1000);
        brightness += 20u;
        Timer2::setCompareValue(1, brightness);
        laser.setBrightness(brightness % 100);

        switch(i)
        {
            case 0:
                // Timer1::setCompareValue(1, 2400);
                // Timer1::setCompareValue(2, 2400);
                break;
            case 1:
                // Timer1::setCompareValue(1, 1500);
                // Timer1::setCompareValue(2, 1500);
                break;
            case 2:
                // Timer1::setCompareValue(1, 2400);
                // Timer1::setCompareValue(2, 600);
                break;
            case 3:
                // Timer1::setCompareValue(1, 1500);
                // Timer1::setCompareValue(2, 1500);
                break;
            default:
                break;
        }
        i++;
        if(i>3) i=0u;
    }

    return 0;
}
