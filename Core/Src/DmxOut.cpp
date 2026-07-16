#include "DmxOut.hpp"

#include "usart.h"

#define _TXPIN_CHANGE(BANK, PIN, POS) (GPIO##BANK->MODER = (GPIO##BANK->MODER & ~GPIO_MODER_MODER##PIN##_Msk) | (POS << GPIO_MODER_MODER##PIN##_Pos))

// usage example : TXPIN_BREAKMODE(A, 9)
#define TXPIN_BREAKMODE(BANK, PIN) _TXPIN_CHANGE(BANK, PIN, 1)
#define TXPIN_TXMODE(BANK, PIN) _TXPIN_CHANGE(BANK, PIN, 2)

#define UART1_TXBANK A
#define UART1_TXPIN  9
#define UART2_TXBANK D
#define UART2_TXPIN  5
#define UART3_TXBANK D
#define UART3_TXPIN  8
#define UART4_TXBANK C
#define UART4_TXPIN  10
#define UART5_TXBANK C
#define UART5_TXPIN  12

template<int TUart> static inline void _txPinBreakMode();
template<int TUart> static inline void _txPinTxMode();

template<> inline void _txPinBreakMode<1>() { TXPIN_BREAKMODE(UART1_TXBANK, UART1_TXPIN); }
template<> inline void _txPinBreakMode<2>() { TXPIN_BREAKMODE(UART2_TXBANK, UART2_TXPIN); }
template<> inline void _txPinBreakMode<3>() { TXPIN_BREAKMODE(UART3_TXBANK, UART3_TXPIN); }
template<> inline void _txPinBreakMode<4>() { TXPIN_BREAKMODE(UART4_TXBANK, UART4_TXPIN); }
template<> inline void _txPinBreakMode<5>() { TXPIN_BREAKMODE(UART5_TXBANK, UART5_TXPIN); }

template<> inline void _txPinTxMode<1>() { TXPIN_TXMODE(UART1_TXBANK, UART1_TXPIN); }
template<> inline void _txPinTxMode<2>() { TXPIN_TXMODE(UART2_TXBANK, UART2_TXPIN); }
template<> inline void _txPinTxMode<3>() { TXPIN_TXMODE(UART3_TXBANK, UART3_TXPIN); }
template<> inline void _txPinTxMode<4>() { TXPIN_TXMODE(UART4_TXBANK, UART4_TXPIN); }
template<> inline void _txPinTxMode<5>() { TXPIN_TXMODE(UART5_TXBANK, UART5_TXPIN); }

static constexpr UART_HandleTypeDef* UARTS[5] = {&huart1, &huart2, &huart3, &huart4, &huart5};

static constexpr Chrono::Microseconds BREAK_DELAY{92};
static constexpr Chrono::Microseconds AFTER_BREAK_DELAY{12};
static constexpr Chrono::Microseconds AFTER_WRITE_DELAY{4000};

template<int TUart>
DmxOut<TUart>::DmxOut() {}

template<int TUart>
void DmxOut<TUart>::init() {
    _currentStep = 0;
    _timer.reset(Chrono::Microseconds{0});
}

template<int TUart>
void DmxOut<TUart>::tick() {
    if (!_timer.done())
        return;
    if (_currentStep == 0) {
        _txPinBreakMode<TUart>();
        _timer.reset(BREAK_DELAY);
        ++_currentStep;
    }
    else if (_currentStep == 1) {
        _txPinTxMode<TUart>();
        _timer.reset(AFTER_BREAK_DELAY);
        ++_currentStep;
    }
    else if (_currentStep == 2) {
        _lastHalCmdStatus = HAL_UART_Transmit_DMA(UARTS[TUart - 1], _fullBuffer, sizeof(_fullBuffer));
        _transmitDone = false;
        ++_currentStep;
    }
    else if (_currentStep == 3) {
        if (!_transmitDone)
            return;
        _timer.reset(AFTER_WRITE_DELAY);
        _currentStep = 0;
    }
}

// explicit instantiation
template class DmxOut<1>;
template class DmxOut<2>;
template class DmxOut<3>;
template class DmxOut<4>;
template class DmxOut<5>;
