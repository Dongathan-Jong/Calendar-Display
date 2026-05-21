# Calendar Dashboard
<img width="2326" height="1615" alt="Display_Case_2026-May-13_07-05-28PM-000_CustomizedView21459932709_png" src="https://github.com/user-attachments/assets/b20602f8-7cbd-4790-bde0-ddf8f9e87abf" />

### Inspiration
I really liked the TRMNL dashboard, but after finding out their proprietary firmware was locked behind a paywall, I decided to build my own! I originally bought the epaper display thinking the firmware was free, since it was open source. With the parts in hand, I decided to code the firmware myself to what I like. The display consists of 3 main parts:
- Next Calendar Event
- Current Weather
- Stock Info

### How Can I Build One? 
Here are the following parts I'm using:
- $61.99 | 7.5inch HD e-Paper E-Ink Display | https://www.waveshare.com/7.5inch-hd-e-paper-hat.htm
- $14.99 | ESP32 EPD Driver Board | https://www.waveshare.com/e-paper-esp32-driver-board.htm
- $1 - $5 | 3x Cherry MX Switches
  
### Setup & Customization
##### Google Apps Script Setup 
First, head over to http://script.google.com/ and create a new script! Paste in the code under the GApps Script folder, and change lines 83 and 84 to the correct coordinates of your location to get the proper weather.   
Next, deploy the script, and get the link that resembles https://script.google.com/macros/s/xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx/exec  

##### Code Setup
In arduino IDE or any IDE, input your link on line 1647, and the WiFi credentials on line 1645 and 1646.   
To change the stocks that you are watching, head over to http://finnhub.io/ and get a API key. Insert that API key on lines 1754, 1780, and 1806. To change the ticker of the stock, change the stock ticker in the same line that the api key was put on.   
Note that the first stock is made to be a 2 digit stock, while the second and third stock is made to be 3 digit stocks. Ex. 2 digits = $12.00, 3 digits = $123.00.
You are free to change it to whatever you'd like, however a lot of the drawings are hardcoded to the digits, so you may have to tinker around with the coordinates. On lines 2004, 2052, and 2100 are where you can change the name of stock that is written on the display.   

##### Icons
To change the icons, I used [this tutorial](https://www.instructables.com/Converting-Images-to-Flash-Memory-Iconsimages-for-/) to convert images into bitmaps. I used the FastStone Image Resizer to resize the images and the lcd-image-converter with the settings: (monochrome, threshold dither, and inverse) to get the bitmaps. You can add your own in the top!  

##### Fonts
I really like this font that I'm using right now, but you're free to use your own. To change the font, get the .ttf from somewhere, and go here: https://rop.nl/truetype2gfx/. Upload your .ttf, and press upload, and download the following sizes:  
- 8 pt
- 16 pt
- 16 pt bold
- 24 pt  

Drop those files into the same folder as your code! 

### Whats Next?
I need to implement the calendar tab and the integration with the buttons, but thats for the future! It currently reloads every 7 minutes, and works amazing! 
