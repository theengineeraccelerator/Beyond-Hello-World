The following files make up this demo

.csv file: Chase, ColorWipe, Arbitrary Animation - These are CSV files that allow you to define the animation in time as if it was a bit map. Column represents a LED and each row represents one frame. Chase and ColorWipe recreate some of the effects in the strandtest demo and Arbitratry Animation shows a custom animation for the strip.The table uses 33 columns: 32 for each LED and the 33rd to add a comma at the end of each row. This means the table will match the code syntax with no changes

Frame_Table_Animation.ino - This is the demo code that uses a non-blocking timer to display each frame. 

Frame_Table.h - This contains the frame table derived from the .csv files. You also define the colors and frames per second in this file

LED Strip Adapter - These are the Eagle files for the adapter board that connects the strip to the external power supply and the Arduino

Neopixel Demo BOM - This contains all the parts I used to create the demo. 

Usage: 
	- Define the colors you want to use and update or add them to the Frame_Table.h. Be sure to use the 565RGB format. To convert 888RGB to 565RGB you can used http://www.rinkydinkelectronics.com/calc_rgb565.php 
	- Open the .csv in a spreadsheet editor and adjust the color locations as needed and save again
	- Open the same .csv in a text editor and copy the whole file
	- In Frame_Table.h select all the hex in the PixelData array and replace it with the hex values you copied. 
	- Save and upload to the Arduino. 