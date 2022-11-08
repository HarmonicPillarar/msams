/*
Copyright 2022 - 2023 @GranuMuse

Permission is hereby granted, free of charge,
to any person obtaining a copy of this software
and associated documentation files (the "Software"),
to deal in the Software without restriction,
including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice
shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Arduino.h"
#include "MSAMSComponents.h"

MSAMSComponent::MSAMSComponent(int _pin)
{
    setPin(_pin);
}

MSAMSComponent::~MSAMSComponent()
{
    ;
}

void MSAMSComponent::setPin(int _pin)
{
    int p;
    if (!analog)
    {
        if (_pin < 2)
            p = 2;
        else if (_pin > 13)
            p = 12;
        else
            p = _pin;
        pin = p;
    }
    else
    {
        if (_pin < 23)
            p = 23;
        else if (_pin > 28)
            p = 28;
        else
            p = _pin;
        pin = p;
    }
}

int MSAMSComponent::getPin()
{
    return pin;
}

int MSAMSComponent::getState()
{
    switch (analog)
    {
    case ANALOG:
        state = analogRead(pin);
        break;
    default:
        state = digitalRead(pin);
    }
    return state;
}

int MSAMSComponent::getPState()
{
    return pstate;
}

int MSAMSComponent::getD()
{
    int d = state - pstate;
    return d;
}

void MSAMSComponent::updatePrev()
{
    pstate = state;
}

int MSAMSComponent::update()
{
    getState();
    updatePrev();
    return getD();
}

void MSAMSComponent::cfg()
{
    pinMode(pin, INPUT);
    this->configured = true;
    Serial.println(configured);
    delay(20);
}

int MSAMSComponent::avg(int n = 100)
{
    int sum;
    for (int i = 0; i < n; i++)
    {
        sum += (getState() + getPState()) / 2;
    }
    sum /= n;
    return sum;
}

/*
**  LEDs PROJECT  **
*   VERSION 1.0.0
*   AUTHOR: ARGYRIOS POURNARIS
*   DESCRIPTION:
*       This application is the basic software structure
*       for controlling a channel station.
*       A rotary potentiometer switches between channels
*       and only when selecting one it can be controlled.
*       The control panel includes a set of indicators
*       (LEDs), one for channel selection and one for
*       channel enabling.
*       The channel setup is 100% abstract though:
*       the common rotary pot, a toggle button, a blink
*       button (defined short enabling duration) and an
*       toggle automation button are responsible for all
*       channels modes, without the need for local components.
*   CLASSES:
*       LED
*       Button
*       Potentiometer
*       Sensor
*       Channel
*       Board (as an Interface)
*/

LED::LED(int _pin) : MSAMSComponent(_pin)
{
    analog = false;
    setPin(_pin);
}

LED::~LED()
{
    ;
}

void LED::cfg()
{
    pinMode(pin, OUTPUT);
    configured = true;
    Serial.println(configured);
    delay(20);
}

bool LED::isON()
{
    return isOn;
}

void LED::ON()
{
    if (!isOn)
        digitalWrite(pin, 1);
    isOn = true;
}

void LED::OFF()
{
    if (isOn)
        digitalWrite(pin, 0);
    isOn = false;
}

bool LED::isBlinking()
{
    return blinking;
}

void LED::enableBlinking()
{
    if (!blinking)
        blinking = true;
}

void LED::disableBlinking()
{
    if (blinking)
        blinking = false;
}

void LED::blink(int t = 1000)
{
    if (blinking)
    {
        ON();
        delay(t);
        OFF();
    }
}

// t_oscillate method, provides a 100% PWM
// control of blink_ON method. ON-state period
//(t1 arg) and OFF-state period (t2 arg) give
// the total period (T) of the blink pulse in ms,
// with t1 / t2 the PWM ratio between the two time values.
void LED::t_oscillate(int t1 = 500, int t2 = 500)
{
    blink(t1);
    delay(t2);
}

// f_oscillate, calculates dynamicaly the two time values
// of t_oscillate method, through the f (frequency) and pw (pulse width) args
// (pw is value unit based percentage (always between 0.01 and 0.99 of 1.00))
// and then passes them to the invoked t_oscillate method.
void LED::f_oscillate(int f = 1, float pw = 0.5)
{
    float T, t1, t2;
    int it1, it2;
    if (pw > 0.99)
    {
        pw = 0.99;
    }
    else if (pw < 0.01)
    {
        pw = 0.01;
    }
    T = 1000.00 / f; // T in ms
    t1 = T * pw;
    t2 = T * (1.00 - pw);
    it1 = (int)t1;
    it2 = (int)t2;
    t_oscillate(it1, it2);
}

// BUTTON
Button::Button(int _pin) : MSAMSComponent(_pin)
{
    analog = false;
    setPin(_pin);
}

Button::~Button()
{
    ;
}

void Button::cfg()
{
    pinMode(pin, INPUT);
    configured = true;
    Serial.println(configured);
    delay(20);
}

bool Button::toggle()
{
    update();
    if (state)
        if (pstate != state)
            toggleOn = !toggleOn ? true : false;
    return toggleOn;
}

int Button::getDt()
{
    unsigned long int ms = millis();
    dt = ms - dt;
    return dt;
}

bool Button::clicked()
{
    unsigned long int ms;
    unsigned long int currms;
    bool CLICKED;
    CLICKED = false;
    clicks = 0;
    while (getState())
    {
        int times;
        ms = millis();
        while (currms <= 300)
        {
            times = 1;
            currms = millis() - ms;
            getState();
            break;
        }
        if (currms > 300)
        {
            times = 0;
            break;
        }
        clicks = times;
    }
    CLICKED = (clicks == 1) ? true : false;
    update();
    return CLICKED;
}

bool Button::doubleClicked()
{
    unsigned long int before_1st_click_dt;
    unsigned long int after_1st_click_dt;
    bool DBL_CLICKED;
    before_1st_click_dt = getDt();
    DBL_CLICKED = false;
    if (clicked())
    {
        int times;
        unsigned long int clickDt;
        after_1st_click_dt = getDt();
        clickDt = abs(after_1st_click_dt - before_1st_click_dt);
        if (clickDt >= 100 && clickDt <= 300)
        {
            times = 1;
            if (clicks == 1)
            {
                clicked();
                if (clicks == 1)
                    times = 2;
            }
            clicks = times;
        }
    }
    DBL_CLICKED = (clicks == 2) ? true : false;
    return DBL_CLICKED;
}

// POTENTIOMETER
Potentiometer::Potentiometer(int _pin) : MSAMSComponent(_pin)
{
    analog = ANALOG;
    setPin(_pin);
}
Potentiometer::~Potentiometer()
{
    ;
}

void Potentiometer::cfg()
{
    pinMode(pin, INPUT);
    configured = true;
    Serial.println(configured);
    delay(20);
}

int Potentiometer::divide(int steps)
{
    int s, d, result;
    s = getState();
    d = 1023 / steps;
    result = s / d;
    return result;
}

// SENSOR
Sensor::Sensor(int _pin) : MSAMSComponent(_pin)
{
    analog = ANALOG;
    setPin(_pin);
}

Sensor::~Sensor()
{
    ;
}

void Sensor::calibrate()
{
    if (configured)
    {
        int inMin, inMax, time;
        while (time < 5000)
        {
            int s = getState();
            if (s >= 8 && s < 1023)
                inMax = map(s, 0, s, 0, 1023);
            else if (s < 8)
                inMin = map(s, 0, s, 0, 8);
            time++;
        }
        min = inMin;
        max = inMax / 2;
    }
    delay(20);
}

void Sensor::cfg()
{
    pinMode(pin, INPUT);
    configured = true;
    calibrate();
    Serial.println(configured);
    delay(20);
}

// PHOTISTOR
Photistor::Photistor(int _pin) : Sensor(_pin)
{
    analog = ANALOG;
    setPin(_pin);
}

Photistor::~Photistor()
{
    ;
}

void Photistor::cfg()
{
    pinMode(pin, INPUT);
    configured = true;
    calibrate();
    Serial.println(configured);
    delay(20);
}

// THERMISTOR
Thermistor::Thermistor(int _pin) : Sensor(_pin)
{
    analog = ANALOG;
    setPin(_pin);
}

Thermistor::~Thermistor()
{
    ;
}

void Thermistor::cfg()
{
    pinMode(pin, INPUT);
    configured = true;
    calibrate();
    Serial.println(configured);
    delay(20);
}

// class Channel
// {
// private:
//     unsigned long int index = 0;
//     int pinin;
//     int pinout;
//     bool configured = false;
//     bool selected = false;
//     bool enabled = false;

// public:
//     bool savedState;
//     Channel()
//     {
//         saveState();
//     }
//     Channel(unsigned long int &aTotal)
//     {
//         setPinin(0);
//         aTotal++;
//         setIndex(aTotal);
//         saveState();
//     }
//     Channel(int _pinin, unsigned long int &aTotal)
//     {
//         setPinin(_pinin);
//         aTotal++;
//         setIndex(aTotal);
//         saveState();
//     }
//     void cfg()
//     {
//         configured = true;
//     }
//     bool isConfigured()
//     {
//         return configured;
//     }
//     void setPinin(int _pin)
//     {
//         pinin = _pin;
//         pinout = pinin + 1;
//     }
//     int getPinin()
//     {
//         return pinin;
//     }
//     int getPinout()
//     {
//         return pinout;
//     }
//     void setIndex(unsigned long int &aTotal)
//     {
//         index = aTotal - 1;
//     }
//     int getIndex()
//     {
//         int i = (int)index;
//         return i;
//     }
//     bool isSelected()
//     {
//         return selected;
//     }
//     void select()
//     {
//         if (!selected)
//         {
//             selected = true;
//         }
//     }
//     void unselect()
//     {
//         if (selected)
//         {
//             selected = false;
//         }
//     }
//     void enable()
//     {
//         if (selected)
//         {
//             if (!enabled)
//             {
//                 enabled = true;
//             }
//         }
//     }
//     void disable()
//     {
//         if (selected)
//         {
//             if (enabled)
//             {
//                 enabled = false;
//             }
//         }
//     }
//     bool isEnabled()
//     {
//         return enabled;
//     }
//     void saveState()
//     {
//         savedState = enabled;
//     }
//     bool getSavedState()
//     {
//         return savedState;
//     }
//     // a user friendly analysis block for the any channel status (only if any output port is available)
//     void getStatus()
//     {
//         Serial.println("CHANNEL " + /*to_string*/ String(index) + " STATUS");
//         // cout << "CHANNEL " << index << " STATUS" << endl;
//         Serial.println("Input Pin: " + /*to_string*/ String(pinin));
//         // cout << "Input Pin: " << pinin << endl;
//         Serial.println("Output Pin: " + /*to_string*/ String(pinout));
//         // cout << "Output Pin: " << pinout << endl;
//         if (!configured) /*cout << "Configured: false" << endl;*/
//             Serial.println("Configured: false");
//         else /*cout << "Configured: true" << endl;*/
//             Serial.println("Configured: true");
//         if (!selected) /*cout << "Selected: false" << endl;*/
//             Serial.println("Selected: false");
//         else /*cout << "Selected: true" << endl;*/
//             Serial.println("Selected: true");
//         if (!enabled) /*cout << "Enabled: false" << endl;*/
//             Serial.println("Enabled: false");
//         else /*cout << "Enabled: true" << endl;*/
//             Serial.println("Enabled: true");
//         if (!savedState) /*cout << "Saved State In Memory: disabled" << endl;*/
//             Serial.println("Saved State In Memory: disabled");
//         else /*cout << "Saved State In Memory: enabled" << endl;*/
//             Serial.println("Saved State In Memory: enabled");
//         Serial.println("");
//         /*cout << "" << endl;*/
//     }
// };

// class Board
// {
//     // no instantiation for Board. It exists
//     // just as a convinience for a readable
//     // and reduced code in several distinct
//     // functional components. Just a mechanism.
// public:
//     static void updateButtons(Button buttonArr[], int aButtonsNum)
//     {
//         for (int i = 0; i < aButtonsNum; i++)
//         {
//             buttonArr[i].updatePrev();
//         }
//     }
//     static void selectChannel(Channel channelArr[], int &aChannelIndex, int aLen, LED leds[], Potentiometer &aPot)
//     {
//         int state = aPot.divide(aLen); // 0 <-> aLen - 1
//         for (int i = 0; i < aLen; i++)
//         {
//             if (state == i /* && state == aStrip.channels[i].getIndex()*/)
//             {
//                 channelArr[i].select();
//                 leds[channelArr[i].getPinin() - 2].enableLoop();
//                 leds[channelArr[i].getPinin() - 2].f_oscillate(20);
//                 if (aChannelIndex != channelArr[i].getIndex())
//                     aChannelIndex = channelArr[i].getIndex();
//                 // and now the autoToggle, only if savedState == true
//                 if (channelArr[aChannelIndex].getSavedState())
//                 {
//                     autoToggle(channelArr, aChannelIndex, leds);
//                 }
//             }
//             else
//             {
//                 channelArr[i].disable();
//                 channelArr[i].unselect();
//                 leds[channelArr[i].getPinin() - 2].disableLoop();
//                 leds[channelArr[i].getPinout() - 2].OFF();
//             }
//         }
//     }
//     static void toggleState(Channel channelArr[], int channelIndex, int aButtonCurrState, int aButtonPrevState)
//     {
//         if (channelArr[channelIndex].isSelected())
//         {
//             if (!channelArr[channelIndex].getSavedState())
//             {
//                 channelArr[channelIndex].enable();
//             }
//             else
//             {
//                 channelArr[channelIndex].disable();
//             }
//             channelArr[channelIndex].saveState();
//         }
//     }
//     static void toggleChannel(Channel channelArr[], int channelIndex, LED leds[], int aButtonCurrState, int aButtonPrevState)
//     {
//         if (!channelArr[channelIndex].getSavedState())
//         {
//             if (aButtonCurrState == 1)
//             {
//                 if (aButtonCurrState != aButtonPrevState)
//                 {
//                     if (channelArr[channelIndex].isSelected() && !channelArr[channelIndex].isEnabled())
//                     {
//                         channelArr[channelIndex].enable();
//                         leds[channelArr[channelIndex].getPinout() - 2].ON();
//                     }
//                     else
//                     {
//                         channelArr[channelIndex].disable();
//                         leds[channelArr[channelIndex].getPinout() - 2].OFF();
//                     }
//                 }
//             }
//         }
//     }
//     static void blinkChannel(Channel channelArr[], int channelIndex, LED leds[], int aButtonCurrState, int aButtonPrevState, int duration)
//     {
//         if (aButtonCurrState == 1)
//         {
//             if (aButtonPrevState != aButtonCurrState)
//             {
//                 if (!channelArr[channelIndex].isEnabled())
//                 {
//                     leds[channelArr[channelIndex].getPinout() - 2].blink_ON(duration);
//                 }
//             }
//         }
//     }
//     static void registerState(Channel channelArr[], int channelIndex, int aButtonCurrState, int aButtonPrevState)
//     {
//         if (aButtonCurrState == 1)
//         {
//             if (aButtonPrevState != aButtonCurrState)
//             {
//                 toggleState(channelArr, channelIndex, aButtonCurrState, aButtonPrevState);
//             }
//         }
//     }
//     static void autoToggle(Channel channelArr[], int channelIndex, LED leds[])
//     {
//         if (channelArr[channelIndex].isSelected())
//         {
//             if (channelArr[channelIndex].getSavedState())
//             {
//                 channelArr[channelIndex].enable();
//                 leds[channelArr[channelIndex].getPinout() - 2].ON();
//             }
//             else
//             {
//                 channelArr[channelIndex].disable();
//                 leds[channelArr[channelIndex].getPinout() - 2].OFF();
//             }
//         }
//     }
// };

// unsigned long int total = 0;
// const int channelsNum = 2;
// const int buttonsNum = 3;

// Channel channels[channelsNum];
// LED indicators[2 * channelsNum];
// Potentiometer pot(A5);
// Button buttons[buttonsNum] = {6, 7, 8};

// void setup()
// {
//     delay(1000); // a safety delay just in case :)
//     // we are going to configure all channels and components manually
//     for (int i = 1; i <= channelsNum; i++)
//     {
//         int i1, i2, p1, p2;
//         i1 = 2 * i;
//         i2 = 2 * i + 1;
//         channels[i - 1].setPinin(i1);
//         total++;
//         channels[i - 1].setIndex(total);
//         channels[i - 1].saveState();
//         p1 = channels[i - 1].getPinin();
//         p2 = channels[i - 1].getPinout();
//         indicators[i1 - 2].setPin(p1);
//         indicators[i2 - 2].setPin(p2);
//         indicators[i1 - 2].cfg();
//         indicators[i2 - 2].cfg();
//         channels[i - 1].cfg();
//     }
//     pot.cfg();
//     for (int i = 0; i < buttonsNum; i++)
//     {
//         buttons[i].cfg();
//     }
// }

// void loop()
// {
//     /*
//      * In each loop run, Arduino is going to make sure that it's able to (simple AI):
//      *    1. select any available compiled channel and toggle it according to automation state
//      *    2. toggle the selected one
//      *    3. blink the selected one
//      *    4. register its current state to the memory
//      * After that, it updates the control components
//      */
//     int channelIndex = 0;
//     Board::selectChannel(channels, channelIndex, channelsNum, indicators, pot);
//     Board::toggleChannel(channels, channelIndex, indicators, buttons[0].getState(), buttons[0].getPrev());
//     Board::blinkChannel(channels, channelIndex, indicators, buttons[1].getState(), buttons[1].getPrev(), 500);
//     Board::registerState(channels, channelIndex, buttons[2].getState(), buttons[2].getPrev());
//     Board::updateButtons(buttons, buttonsNum);
//     pot.updatePrev();
// }
