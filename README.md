# Design and Innovation Assignment 2  
  ---  
By group 17:  
Julia Drabek  
Kojo Vandyck  
Rami Richani Hamdan  
Sakrit Karmacharya  
  
  ---  
  
### Part I. The First  
  
Uploading this code on the M5 Stack Atom allows to use it as a bike light  
Once the device is activated, it can be used in five different modes:  
&ensp; I.	Off: All LEDs are off, and the screen is black  
&ensp; II.	Manual Rear Strobe: All LEDs strobe with red light at a constant interval of 0.5s  
&ensp; III.	Manual Front Strobe: All LEDs strobe with white light at a constant interval of 0.5s  
&ensp; IV.	Automatic Rear Mode: While the bicycle is riding, all LEDs are strobing with red light, in case of braking, the LEDs are solid red  
&ensp; V.	Automatic Front Mode: While the bicycle is riding, all LEDs are strobing with white light, in case of braking, the LEDs are solid white  
Initially, the device is on mode I. In order to change between the modes, the screen of the device should be clicked once.  
  
  ---  
    
### Part I. The Second  
ploading this code on the M5 Stack Atom allows to use it as a multi-purpose thermometer and temperature tracker  
When the screen is deactivated, the device is measuring and recording the temperature every 15 minutes. To activate the device, it should be held vertically upwards and its screen should be pressed. Once the device is activated, it can be used in five different modes:  
&ensp; I.	Active Temperature: current temperature is shown on the screen of the device together with its unit, the current temperature is updated every 15 minutes. The color of the font depends on the temperature value being displayed, according to the following scale:  

&ensp; ![numbers](https://user-images.githubusercontent.com/72690256/121437995-47b02100-c994-11eb-826c-563fe475dfc2.jpg)

&ensp;&ensp;&ensp; 1. &ensp;&ensp; Below -20°C / Below 253.15K / Below -4°F  
&ensp;&ensp;&ensp; 2. &ensp;&ensp; [-15°C, -10°C) / [258.15K, 263.15K) / [5°F, 14°F)  
&ensp;&ensp;&ensp; 3. &ensp;&ensp; [-10°C, -5°C) / [263.15K, 268.15K) / [14°F, 23°F)  
&ensp;&ensp;&ensp; 4. &ensp;&ensp; [-5°C, 0°C) / [268.15K, 273.15K) / [23°F, 32°F)  
&ensp;&ensp;&ensp; 5. &ensp;&ensp; [0°C, 5°C) / [273.15K, 278.15K) / [32°F, 41°F)   
&ensp;&ensp;&ensp; 6. &ensp;&ensp; [5°C, 10°C) / [278.15K, 283.15K) / [41°F, 50°F)     
&ensp;&ensp;&ensp; 7. &ensp;&ensp; [10°C, 15°C) / [283.15K, 288.15K) / [50°F, 59°F)     
&ensp;&ensp;&ensp; 8. &ensp;&ensp; [15°C, 20°C) / [288.15K, 293.15K) / [59°F, 68°F)     
&ensp;&ensp;&ensp; 9. &ensp;&ensp; [20°C, 25°C) / [293.15K, 298.15K) / [68°F, 77°F)    
&ensp;&ensp;&ensp; 10. &ensp; [25°C, 30°C) / [298.15K, 303.15K) / [77°F, 86°F)    
&ensp;&ensp;&ensp; 11. &ensp; [30°C, 35°C) / [303.15K, 308.15K) / [86°F, 95°F)    
&ensp;&ensp;&ensp; 12. &ensp; [35°C, 40°C) / [308.15K, 313.15K) / [95°F, 104°F)      
&ensp;&ensp;&ensp; 13. &ensp; [40°C, 45°C) / [313.15K, 318.15K) / [104°F, 113°F)    
&ensp;&ensp;&ensp; 14. &ensp; [45°C, 50°C) / [318.15K, 323.15K) / [113°F, 122°F)       
&ensp;&ensp;&ensp; 15. &ensp; [50°C, 55°C) / [323.15K, 328.15K) / [122°F, 131°F)      
&ensp;&ensp;&ensp; 16. &ensp; Above or equal to 55°C / Above or equal to 328.15K / Above or equal to 131°F   

&ensp; II.	Average Temperature: the average temperature of the last 24 hours is shown on the screen together with its unit, the average temperature is updated every hour. The color of the font depends on the temperature value being displayed according to the same scale as in mode I  
&ensp; III.	Color Scale of Temperature: the color scale from modes I and II is displayed on the screen of the device, the LED with a color representing the current temperature blinks  
&ensp; IV.	Graph of Temperature: A histogram is displayed on the screen of the device. Each bar of the histogram represents an hourly average temperature calculated based on the four readings taken every 15 minutes. Bars have heights of 1, 2, 3, 4, or 5 LEDs, according to the following scale:  

![graph](https://user-images.githubusercontent.com/72690256/121479292-efe8d880-c9da-11eb-9e19-a2030db04f11.jpg)  
&ensp;&ensp;&ensp; 1. &ensp;&ensp; Below -10°C / Below 263.15K / Below 14°F  
&ensp;&ensp;&ensp; 2. &ensp;&ensp; [-10°C, 10°C) / [263.15K, 283.15K) / [14°F, 50 °F)  
&ensp;&ensp;&ensp; 3. &ensp;&ensp; [10°C, 30°C) / [283.15K, 303.15K) / [50°F, 86°F)  
&ensp;&ensp;&ensp; 4. &ensp;&ensp; [30°C, 50°C) / [303.15K, 323.15K) / [86°F, 122°F)  
&ensp;&ensp;&ensp; 5. &ensp;&ensp; Above or equal to 50°C / Above or equal to 323.15K / Above or equal to 122°F  

&ensp; V.	Change Units: By clicking the screen of the device, units can be changed between degree Celsius, Kelvin and Fahrenheit  

To change between the above modes, the device should be tilted. To increase the mode number, tilt to the right and to decrease, tilt to the left. When a given mode is entered, a particular LED turns on, according to the following system:
 
![modes LED](https://user-images.githubusercontent.com/72690256/121478738-63d6b100-c9da-11eb-8b0c-d8bec353b7c3.jpg)

Once the mode is entered, the screen of the device should be clicked in order to enter it.  
  
  ---  
    
### Part II  

Uploading this code on the M5 Stack Atom allows to use it as an Indoor and Personal Weather station, which measures temperature and humidity.
The temperature and humidity can be viewed on the device screen or through a webpage hosted on the device by using an electronic device such as a phone  
  
  
#### Viewing data on the website hosted by M5 Stack Atom  
In order to view relevant information on an electronic device, first connect to a wifi called ***CkyBlue***  
Open a web browser and load the following website:  
&ensp;&ensp;&ensp; http://192.168.4.1  
  
  
The website updates every 5 seconds, and the following data can be viewed:  
- Current temperature in degrees Celsius with a colored stripe, whose length and color depends on the value of the current temperature. Current temperature is updated every 5 seconds. Below the stripe, there is a chart representing the recorded temperatures. Every 5 seconds, a new value of current temperature is added to the chart
- Average temperature in degrees Celsius with a colored stripe, whose length and color depends on the value of the average temperature. Below the stripe, there is a chart representing the average temperatures from last hours. Average temperature is calculated after every hour, using the 900 temperature values taken during the interval and it is added to the chart.
- Current humidity, expressed in percentage, with a colored stripe, whose length and color depends on the value of the current humidity. Current humidity is updated every 5 seconds. Below the stripe, there is a chart representing the recorded humidity values. Every 5 secondsc a new value of current humidity is added to the chart
- Change units mode, where the units of temperature can be changed between Celsius, Kelvin and Fahrenheit scales. The temperature data with a different unit than Celsius can only be viewed on the screen of M5 Stack Atom  
  
  
#### Viewing data on the screen of M5 Stack Atom  
When the screen is deactivated, the device is measuring and recording the temperature every 5 seconds. To activate the device, its screen should be pressed. Once the device is activated, it shows the current temperature followed by a current humidity value. After showing each value once, the device's screen is deactivated again. By default, temperature is in degrees Celsius and humidity in percenateg. The temperature unit can be changed to Kelvin or Fahrenheit scale through a website as mentioned above
  

