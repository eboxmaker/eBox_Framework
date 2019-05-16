/*
 * enum.h
 *
 *  Created on: 2019��5��13��
 *      Author: shentq
 */

#ifndef __ENUM_H
#define __ENUM_H



#define DECLARE_ENUM(E) \
struct E \
{ \
public: \
    E(int value = 0) : _value((__Enum)value) { \
    } \
    E& operator=(int value) { \
        this->_value = (__Enum)value; \
        return *this; \
    } \
    operator int() const { \
        return this->_value; \
    } \
\
    enum __Enum {

#define END_ENUM() \
    }; \
\
private: \
    __Enum _value; \
};

#endif /* __ENUM_H */
