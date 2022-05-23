/*
 * License
---------------------------------------------------------------------
    Copyright (C) 2009-2014  likooh.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
annex
---------------------------------------------------------------------
licensetoken.dict: 417 sentence-token expression.
rules.dict: the rule of license.
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
        else if (_pin > 12)
            p = 12;
        else
            p = _pin;
        pin = p;
    }
    else
    {
        if (_pin < 14)
            p = 14;
        else if (_pin > 19)
            p = 19;
        else
            p = _pin;
        pin = p;
    }
}

int MSAMSComponent::getPin()
{
    return pin;
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
*       Channel
*       Board (as an Interface)
*/

// class LED : MSAMSComponent
// {
// public:
//     // default
//     LED()
//     {
//         pin = 0;
//     }
//     // custom (pin selection)
//     LED(int _pin)
//     {
//         pin = _pin;
//     }
//     void cfg()
//     {
//         pinMode(pin, OUTPUT);
//         delay(10);
//         ON();
//         delay(20);
//         OFF();
//         delay(10);
//     }
//     void setPin(int _pin)
//     {
//         if (_pin < 2)
//             _pin = 2;
//         else if (_pin > 12)
//             _pin = 12;
//         else
//             _pin = _pin;
//         const int p = _pin;
//         pin = p;
//     }
//     int getPin()
//     {
//         return pin;
//     }
//     // ON and OFF methods are maximum optimized:
//     //  they run only if isOn condition is the
//     //  appropriate one
//     void ON()
//     {
//         if (!isOn)
//         {
//             digitalWrite(pin, HIGH);
//             isOn = true;
//         }
//     }
//     void OFF()
//     {
//         if (isOn)
//         {
//             digitalWrite(pin, LOW);
//             isOn = false;
//         }
//     }
//     // publicly dynamic iOn check
//     bool isON()
//     {
//         return isOn;
//     }
//     // timed duration for aLED ON method
//     //  (in ms). After that it's OFF
//     void blink_ON(int ms)
//     {
//         ON();
//         delay(ms);
//         OFF();
//     }
//     // enable & didable methods control the
//     // periodic repeatedly blinking state
//     void enableLoop()
//     {
//         if (!looping)
//         {
//             looping = true;
//         }
//     }
//     void disableLoop()
//     {
//         if (looping)
//         {
//             looping = false;
//         }
//     }
//     bool isLooping()
//     {
//         return looping;
//     }
//     // t_oscillate method, provides a 100% PWM
//     // control of blink_ON method. ON-state period
//     //(t1 arg) and OFF-state period (t2 arg) give
//     // the total period (T) of the blink pulse in ms,
//     // with PWM ratio the ratio between the two time values.
//     void t_oscillate(int t1 = 10, int t2 = 10)
//     {
//         if (looping)
//         {
//             blink_ON(t1);
//             delay(t2);
//         }
//     }
//     // f_oscillate, calculates dynamicaly the two time values
//     // of t_oscillate method, through the f (frequency) arg and pw arg
//     // value unit based percentage (always between 0.01 and 0.99 of 1.00)
//     // and then passes them to the invoked t_oscillate method.
//     void f_oscillate(int f = 50, float pw = 0.5)
//     {
//         float T, t1, t2;
//         int it1, it2;
//         if (pw > 0.99)
//         {
//             pw = 0.99;
//         }
//         else if (pw < 0.01)
//         {
//             pw = 0.01;
//         }
//         T = 1000.00 / f; // T in ms
//         t1 = T * pw;
//         t2 = T * (1.00 - pw);
//         it1 = (int)t1;
//         it2 = (int)t2;
//         it1 /= 2;
//         it2 /= 2;
//         t_oscillate(it1, it2);
//     }
//     void ostinato(int n_times = 10, int min_time = 100)
//     {
//         for (int i = 0; i < n_times; i += min_time)
//         {
//             t_oscillate(min_time + i, min_time + i);
//         }
//     }
// };

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

int Button::getState()
{
    state = digitalRead(pin);
    return state;
}

int Button::getPState()
{
    return pstate;
}

void Button::updatePrev()
{
    pstate = state;
}

int Button::getD()
{
    int d = state - pstate;
    return d;
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

int Potentiometer::getState()
{
    state = analogRead(pin);
    return state;
}

int Potentiometer::getPState()
{
    return pstate;
}

void Potentiometer::updatePrev()
{
    pstate = state;
}

int Potentiometer::getD()
{
    int d = state - pstate;
    return d;
}

int Potentiometer::divide(int steps)
{
    int s, d, result;
    s = getState();
    d = 1023 / steps;
    result = s / d;
    return result;
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
