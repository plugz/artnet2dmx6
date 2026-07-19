#ifndef __PIN_HPP__
#define __PIN_HPP__

#include "gpio.h"

#include <cstdint>

struct Pin {
    GPIO_TypeDef* port = nullptr;
    uint16_t pin;
};

#endif
