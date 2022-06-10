#include "laser.h"
#include "motion.h"
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

    Timer1::setPrescaler(84);
    Timer1::setOverflow(20000);
    Timer1::configureOutputChannel(1, Timer1::OutputCompareMode::Pwm, 0);
    Timer1::configureOutputChannel(2, Timer1::OutputCompareMode::Pwm, 0);

    Timer1::applyAndReset();
    Timer1::start();

    Servo<Timer1> topServo{1, 1500, 1000, 2000};
    Servo<Timer1> bottomServo{2, 1500, 1000, 2000};

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

    Motion<Timer1> motion{bottomServo, topServo, 10};
    motion.Initialize();
    modm::delay_ms(100);
    motion.SetTargetPosition({2000, 2000}, 5000);

    while (true)
    {
        modm::delay_ms(10);
        motion.Perform();
    }

    return 0;
}
