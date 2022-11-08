#include <stdint.h>

#ifndef MSAMSComponents_H
#define MSAMSComponents_H
#define ANALOG true

class MSAMSComponent
{
protected:
    bool analog = ANALOG;
    int pin;
    int pstate;
    int state;
    bool configured;

public:
    MSAMSComponent(int _pin);
    ~MSAMSComponent();
    void setPin(int _pin);
    int getPin();
    virtual void cfg();
    int getState();
    int getPState();
    int getD();
    void updatePrev();
    int update();
    int avg(int n);
};

class LED : public MSAMSComponent
{
private:
    bool isOn = false;
    bool blinking = false;

public:
    LED(int _pin);
    ~LED();
    void cfg() override;
    bool isON();
    void ON();
    void OFF();
    bool isBlinking();
    void enableBlinking();
    void disableBlinking();
    void blink(int t);
    void t_oscillate(int t1, int t2);
    void f_oscillate(int f, float pw);
};

class Button : public MSAMSComponent
{
private:
    bool toggleOn = false;
    unsigned long int dt;
    int clicks;
    unsigned long int getDt();

public:
    Button(int _pin);
    ~Button();
    void cfg() override;
    bool toggle();
    bool clicked();
    bool doubleClicked();
};

class Potentiometer : public MSAMSComponent
{
public:
    Potentiometer(int _pin);
    ~Potentiometer();
    void cfg() override;
    int divide(int steps);
};

// class Joystick : public Potentiometer
// {
//     private:
//         int x;
//         int y;
//         bool pressed = false;
//         int getX();
//         int getY();

//     public:
//         Joystick(int _pin);
//         ~Joystick();
//         void cfg() override;
//         int mag();
// };

class Sensor : public MSAMSComponent
{
protected:
    int min = 0;
    int max = 1023;
    void calibrate();

public:
    Sensor(int _pin);
    ~Sensor();
    void cfg() override;
};

class Photistor : public Sensor
{
public:
    Photistor(int _pin);
    ~Photistor();
    void cfg() override;
};

class Thermistor : public Sensor
{
public:
    Thermistor(int _pin);
    ~Thermistor();
    void cfg() override;
};

#endif