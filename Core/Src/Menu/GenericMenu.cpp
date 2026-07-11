#include "GenericMenu.hpp"

#include "LiquidCrystalI2C.hpp"

#include <charconv>

namespace Menu {

enum {
    LINE_BUFF_SIZE = LiquidCrystalI2C::MAX_COLS + 1,
};


template <typename T>
GenericMenu<T>::GenericMenu(CommonStuff const& common, Conf const& c)
    : Menu(common)
    , _config(c) {}

template <typename T> GenericMenu<T>::~GenericMenu() {}

template <typename T> char const* GenericMenu<T>::name() { return _config.name; }

template <typename T> void GenericMenu<T>::up(Button::Event e) {
    if (e != Button::Event::PRESS && e != Button::Event::REPEAT)
        return;
    if constexpr (std::is_same_v<T, bool>) {
        _value = !_value;
    } else {
        ++_value;
        if (_value > _config.max || _value < _config.min)
            _value = _config.min;
    }
    _displayValue();
}

template <typename T> void GenericMenu<T>::down(Button::Event e) {
    if (e != Button::Event::PRESS && e != Button::Event::REPEAT)
        return;
    if constexpr (std::is_same_v<T, bool>) {
        _value = !_value;
    } else {
        --_value;
        if (_value < _config.min || _value > _config.max)
            _value = _config.max;
    }
    _displayValue();
}

template <typename T> void GenericMenu<T>::right(Button::Event e) {
    if (e != Button::Event::PRESS)
        return;
    applyConfig();
}

template <typename T> void GenericMenu<T>::_applyConfig() {
    if (_config.setValueCallback)
        _config.setValueCallback(_value);
}

template <typename T> void GenericMenu<T>::_enable() {
    if (_config.getValueCallback)
        _value = _config.getValueCallback();
    _display();
}

template <typename T> void GenericMenu<T>::_disable() {}

template <typename T> void GenericMenu<T>::_display() {
    _common.screen->printLine(0, _config.inName);
    _common.screen->printLine(1, "");
    _common.screen->printLine(3, _config.desc);
    _displayValue();
}

template <typename T> void GenericMenu<T>::_displayValue() {
    char buff[LINE_BUFF_SIZE] = "  ";
    auto res = std::to_chars(buff + 2, buff + (LINE_BUFF_SIZE - 3), _value);
    if (res.ec != std::errc()) // to_chars error
        return;
    *res.ptr = '\0'; // to_chars does not null-terminate
    _common.screen->printLine(2, buff);
}

// to_chars does not do bool, need specific instantiation
template <> void GenericMenu<bool>::_displayValue() {
    char buff[LINE_BUFF_SIZE];
    snprintf(buff, sizeof(buff), "  %s", (_value ? "YES" : "NO"));
    _common.screen->printLine(2, buff);
}

// explicit instantiation
template class GenericMenu<bool>;
template class GenericMenu<int>;
template class GenericMenu<unsigned int>;
template class GenericMenu<int64_t>;
template class GenericMenu<uint64_t>;

} // namespace Menu
