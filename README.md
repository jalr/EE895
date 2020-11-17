# EE895
[![Build Status](https://jalr.semaphoreci.com/badges/EE895.svg?style=shields)](https://jalr.semaphoreci.com/projects/EE895)

An Arduino library for the [EE895 CO2 sensor](https://www.epluse.com/en/products/co2-measurement/co2-sensor/ee895/) manufactured by E+E Elektronik Ges.m.b.H.

## Hardware setup

### D1 Mini
![D1 Mini connections](docs/connections-d1mini.jpg)

on WeMos D1 Mini
* EE895 is supplied with 5V
* SDA on GPIO 4 / D2, 4k7 Pullup to 3.3V
* SCL on GPIO 5 / D1, 4k7 Pullup to 3.3V

## Development
If you want to contribute to this project please read this section. I've also added some CI tests to ensure
* run unit tests
* compile examples
* ensure Arduino code style is matched
* `keywords.txt` are up to date

### git hooks
To ease the development, you may install a pre-commit git hook using the following command:

```bash
make .git/hooks/pre-commit
```

Please keep in mind that you should always review the code before executing someone's code you found on the internet ;)

### Code style
I've copied [Arduino code style configuration](https://raw.githubusercontent.com/arduino/Arduino/1.8.13/build/shared/examples_formatter.conf) to [codestyle.conf](codestyle.conf). You can use [astyle](http://astyle.sourceforge.net/) to automatically apply these style rules. You can apply the code style by executing the following command:
```bash
make codestyle
```

### keywords.txt
If you removed, added or renamed methods, it is necessary to update the `keywords.txt` file. To automatically do this, you need to have the python package `arduinokeywords` installed. After doing that, you can run:
```bash
make keywords.txt
```

## References
* [EE895 Manual](https://www.epluse.com/fileadmin/data/product/ee895/BA_EE895.pdf)
* [Modbus Application Protocol](https://modbus.org/docs/Modbus_Application_Protocol_V1_1b3.pdf)
