# MSAMS

#NEWS & UPDATES
- Fundamental Optimizations
- Bug Fixes
- RGBLED Class Added
#

This rep includes the .zip of MSAMSComponents.cpp and MSAMSComponents.h files for MSAMS project and the MSAMS.ino example file.
The MSAMS Components library is a small and fast library for reading from and writing to analog electronic components through Arduino boards. It consists of:

- MSAMSComponents.h file (prototype) that contains all the classes for digital representation of many analog components (pots, buttons, LEDs, thermal / light sensors etc)
- MSAMSComponents.cpp file (the init file)

MSAMS stands for Microcontroller Software Application Modular Synthesizer. Clearly, it is a whole project shaped by its hardware and software form to retrieve the data of analog components through an Arduino board and send them via USB or other client port to a PC or any available processor unit.

####################### INCLUDE THE LIBRARY #######################
1. Download the msams.zip folder
2. After downloading, extract the folder into the folder that you downloaded the .zip and rename the extracted one 'MSAMSComponents'. Note: in this step, I recommend that you delete the folder name in the end of the suggested path in the window will appear when the extract button is pressed.
3. Move the extracted folder to your Documents/Arduino/libraries folder
4. Open Aduino IDE
5. Go to the menu board on the top and select Sketch => Include Library => MSAMSComponents
6. In the global scope, create an instance for each corresponding analog component you use on your hardware ( if a pot, use Potentiometer pot(aPinNum); )
7. In void setup() function, create an instance pointer of the base class MSAMSComponent for each instance of the analog components you created in the global scope ( like above, MSAMSComponent *ppot = &pot; )
8. (Continuing 7) call all the pointer instances' void cfg() method ( ppot->cfg(); )
9. Write your desired code or open the MSAMS.ino example
10. Upload your code to your Arduino Board

That's all!

Enjoy :)
