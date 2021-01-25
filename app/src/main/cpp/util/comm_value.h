//
// Created by Lai on 2021/1/22.
//

#ifndef WHITEBOARD_COMM_VALUE_H
#define WHITEBOARD_COMM_VALUE_H

class BaseHolder {
public:
    BaseHolder() {}

    virtual ~BaseHolder() {}

    virtual BaseHolder *clone() = 0;
};

template<typename T>
class Holder : public BaseHolder {
public:
    Holder(const T &t)
            : _value(t) {}

    ~Holder() {}

    BaseHolder *clone() override {
        return new Holder<T>(_value);
    }

public:
    T _value;
};

class Any {
public:
    template<typename ValueType>
    Any(const ValueType &value) {
        _pValue = new Holder<ValueType>(value);
    }

    Any(const Any &any) : _pValue(any._pValue->clone()) {}

    ~Any() {
        if (_pValue)
            delete _pValue;
    }

    template<typename ValueType>
    Any &operator=(const ValueType &value) {
        Any tmp(value);
        if (_pValue)
            delete _pValue;
        _pValue = tmp._pValue;
        tmp._pValue = nullptr;
        return *this;
    }

    template<typename ValueType>
    ValueType *anyCast() {
        Holder<ValueType> *p = dynamic_cast<Holder<ValueType> *>(_pValue);
        return p ? &p->_value : nullptr;
    }

    template<typename ValueType>
    ValueType &anyRefCast() {
        return (dynamic_cast<Holder<ValueType> & >(*_pValue))._value;
    }

private:
    BaseHolder *_pValue;
};

#endif //WHITEBOARD_COMM_VALUE_H
