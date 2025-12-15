#ifndef EVENT_MACRO_H
#define EVENT_MACRO_H

#define NoParamEventDataDeclaration(eventName, superEventName)  \
class eventName: public superEventName                      \
{                                                           \
public:                                                     \
    eventName() = default;                                  \
    ~eventName() = default;                                 \
};                                                          \

#endif // EVENT_MACRO_H