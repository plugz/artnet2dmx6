#include "DmxIn.hpp"

#include "Stats.hpp"

#include "usart.h"

extern Stats a2d6Stats;

#define _RXPIN_CHANGE(BANK, PIN, POS) (GPIO##BANK->MODER = (GPIO##BANK->MODER & ~GPIO_MODER_MODER##PIN##_Msk) | (POS << GPIO_MODER_MODER##PIN##_Pos))

// usage example : RXPIN_BREAKMODE(A, 9)
#define RXPIN_BREAKMODE(BANK, PIN) _RXPIN_CHANGE(BANK, PIN, 0)
#define RXPIN_TXMODE(BANK, PIN) _RXPIN_CHANGE(BANK, PIN, 2)

#define BANK(LETTER) (GPIO##LETTER)
#define PIN(NUMBER) (GPIO_PIN_##NUMBER)

#define UART1_RXBANK A
#define UART1_RXPIN  10
#define UART2_RXBANK D
#define UART2_RXPIN  6
#define UART3_RXBANK D
#define UART3_RXPIN  9
#define UART4_RXBANK C
#define UART4_RXPIN  11
#define UART5_RXBANK D
#define UART5_RXPIN  2
#define UART6_RXBANK C
#define UART6_RXPIN  7


DmxIn::DmxIn() {}

DmxIn::~DmxIn() {}

void DmxIn::init(PacketCallback cb) {
    _callback = cb;
}

void DmxIn::_waitForBreak() {
    //auto val = HAL_GPIO_ReadPin(BANK(UART6_RXBANK), PIN(UART6_RXPIN));
    auto val = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);
    if (val == GPIO_PIN_SET)
        return;

    _timer.reset(Chrono::Microseconds(70)); // dont wait the full 88 us
    _currentStep = &DmxIn::_waitForBreakEnd;
}

void DmxIn::_waitForBreakEnd() {
    auto val = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);
    if (val == GPIO_PIN_RESET)
        return;

    if (!_timer.done()) {
        _currentStep = &DmxIn::_waitForBreak;
        return;
    }

    _timer.reset(Chrono::Microseconds(7));
    _currentStep = &DmxIn::_afterBreak;
}

void DmxIn::_afterBreak() {
    if (!_timer.done())
        return;

    auto result = HAL_UART_Receive_DMA(&huart6, _buffers[_currentBufferIdx].data() + 17, 513, 200);
    if (result == HAL_OK)
        a2d6Stats.incrementCounter(0);
    else
        a2d6Stats.incrementCounter(1);

    a2d6Stats.setCounter(2, _buffers[_currentBufferIdx][18]);
    a2d6Stats.setCounter(3, _buffers[_currentBufferIdx][19]);
    a2d6Stats.setCounter(4, _buffers[_currentBufferIdx][20]);
    a2d6Stats.setCounter(5, _buffers[_currentBufferIdx][21]);

    _currentStep = &DmxIn::_readData;
}

void DmxIn::_readData() {
    _currentStep = &DmxIn::_waitForBreak;
}
