import processing.serial.*;

Serial myPort;
PImage bg,normal,poluted_inside;
PFont mono;
int i = 0;
String rezultat = "";
String val1 ="";
void setup()
{
//size(1360,680);
fullScreen();
myPort = new Serial(this, Serial.list()[0], 57600);
myPort.bufferUntil('\n');
bg = loadImage("carbon-fiber.jpg");
normal = loadImage("air1.png");
poluted_inside = loadImage("air2.png");
}
void draw()
{
while(myPort.available() > 0)
{
  rezultat = myPort.readStringUntil('\n');
  if(rezultat != null)
  {
  //println(rezultat);
  }
  background(bg);
  mono = createFont("ROCK.TTF",42);
  textFont(mono);
  fill(255);
  rect(120, 50, 1100, 660, 12);
  fill(0);
  text("AiRMechatronics",510,110);
  fill(0);
  textSize(14);
  text("© Raul Boldea - Universitatea \"1 Decembrie 1918\" Alba-Iulia",828,700);
  fill(58,208,216,80);
  rect(140, 170, 450, 150, 12);
  fill(0);
  textSize(20);
  text("Interfata pentru monitorizarea calitatii aerului",150,200);
  fill(0);
  text(""+rezultat,150,230); //<>//
  //println(); //<>//
  ellipse(700, 550, 100, 100);
  fill(128,128,128);
  ellipse(700, 550, 95, 95);
  fill(255,255,255);
  ellipse(700, 550, 50, 50);
  fill(128,128,128);
  ellipse(700, 550, 35, 35);
  fill(0);
  strokeCap(ROUND);
  line(700, 520, 700,540);
  text("Buton pentru inchidere / deschidere geam",500,650);
  
  if(rezultat != null){
  val1=""+rezultat+"";
  val1=val1.trim();
  i++;
  if(val1.equals("Calitatea aerului este la valori normale"))
    {image(normal, 350, 245);}
  else if(val1.equals("Calitatea aerului din incapere este periculoasa"))
  {
  image(poluted_inside, 350, 245);
  }
  else if(val1.equals("Second way"))
  {println("triggerd");}
    
  }
}
} //<>// //<>// //<>// //<>// //<>// //<>// //<>//
