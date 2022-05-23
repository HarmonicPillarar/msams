#include <stdint.h>

#ifndef MSAMSComponents_H
#define MSAMSComponents_H
#define ANALOG true

class MSAMSComponent
{
protected:
    bool analog = ANALOG;
    int pin;
    bool configured;

public:
    MSAMSComponent(int x_);
    ~MSAMSComponent();
    void setPin(int _pin);
    int getPin();
    virtual void cfg();
    // void setPState(int state_);
    // int getState();
};

class Button : public MSAMSComponent
{
private:
    int pstate;
    int state;

public:
    Button(int _pin);
    ~Button();
    void cfg() override;
    int getState();
    int getPState();
    int getD();
    void updatePrev();
};

class Potentiometer : public MSAMSComponent
{
private:
    int pstate;
    int state;

public:
    Potentiometer(int _pin);
    ~Potentiometer();
    void cfg() override;
    int getState();
    int getPState();
    int getD();
    void updatePrev();
    int divide(int steps);
};

#endif