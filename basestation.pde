import processing.serial.*; // this library handles the serial talk 
import java.util.*;
import java.util.ArrayList;
import java.nio.charset.Charset;
import processing.sound.*;
PrintWriter output;


Serial myPort;  // Create object from Serial class
String data="" ; // empty string to gather the pot values
PFont  myFont;  // object from the font class


int co;


int test_state=3;//0=blank, 1=start loadcel, 2=idle, 3=activtest, 4=test finished


PImage img;
PImage ned;

void setup()// this runs just once
{
fullScreen(); // size of processing window
background(0);// setting background color to black
fill(#15c318);// fillinf green color on the text blue is #4B5DCE
myPort = new Serial(this, "COM8", 115200);// giving parameters to object of serial class,put the com to which your arduino is connected and the baud rate
myPort.bufferUntil('\n');// gathers data till new line character
img = loadImage("deg.jpg");
ned = loadImage("ned.jpeg");
}
void draw()
 {
      textSize(40);
      //text(data,50,100);
      //text(nf(co, 0, 0), 570,100);
      if(co>50){
      image(img, 140, 200);
      delay(500);
      }else{
      image(ned, 140, 200);
      }
 }
void serialEvent(Serial myPort)// whenever serial event happens it runs
{
  data = myPort.readString();
   
   if(data != null){
       data = trim(data);
       String[] Values = split(data, ",");
           co = int(Values[0]);         
   }    
}
