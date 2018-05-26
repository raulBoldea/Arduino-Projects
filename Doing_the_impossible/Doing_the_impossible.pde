import processing.serial.*; //<>// //<>// //<>// //<>// //<>// //<>// //<>// //<>// //<>// //<>// //<>// //<>// //<>//

Serial myPort;
PImage bg,normal,poluted_inside,poluted_outside;
PFont mono;
int i,value = 0;
String rezultat = "",windowState = "";
String val1 ="";
boolean switcer = true;
String test = "",test1 = "";
color led,buton_on_off,line;

void setup()
{
  //size(1360,680);
  fullScreen();
  myPort = new Serial(this, Serial.list()[0], 57600);
  myPort.bufferUntil('\n');
  led = color(255,0,0);
  line = color (0,0);
  buton_on_off = color(0,0,0);
  bg = loadImage("carbon-fiber.jpg");
  normal = loadImage("air1.png");
  poluted_inside = loadImage("air2.png");
  poluted_outside = loadImage("air3.png");
  println(myPort.readStringUntil('\n'));
}
void draw()
{
  while(myPort.available() > 0)
  {
    rezultat = myPort.readStringUntil('\n');
    println(rezultat);
    if(rezultat != null)
    {
      println(rezultat);
    } //<>// //<>// //<>//
    background(bg);
    mono = createFont("ROCK.TTF",42);
    textFont(mono);
    fill(255);
    rect(120, 50, 1100, 660, 12);
    fill(0);
    text("AiRMechatronics",530,110);
    fill(0);
    textSize(14);
    text("© Raul Boldea - Universitatea \"1 Decembrie 1918\" Alba-Iulia",828,700);
    fill(58,208,216,80);
    rect(470, 280, 460, 180, 12);
    fill(0);
    textSize(20);
    text("Interfata pentru monitorizarea calitatii aerului",480,320);
    fill(0);
    text(windowState,610,450);
    fill(0);
    text("Buton pentru inchidere / deschidere geam",500,650);
    text(""+rezultat,480,360); //<>//
    //println(); //<>//
    ellipse(700, 550, 104, 104);
    fill(40,40,40);
    ellipse(700, 550, 95, 95);
    fill(buton_on_off);
    ellipse(700, 550, 50, 50);
    fill(40,40,40);
    ellipse(700, 550, 35, 35);
    
    strokeWeight(6);
    strokeCap(PROJECT);
    //fill(255,255,255);
    stroke(line);
    line(700, 520, 700,540);
    strokeWeight(0);
    fill(led);
    ellipse(700, 618, 15, 15);
      
    if(rezultat != null)
    {
      val1=""+rezultat+"";
      val1=val1.trim();
      i++;
      if(val1.equals("Calitatea aerului este la valori normale"))
      {
        image(normal, 678, 380);
      }
      else if(val1.equals("Calitatea aerului din incapere este periculoasa!"))
      {
        image(poluted_inside, 678, 380);
      }
      //else if(val1.equals("Second way"))
      //{
      //  println("triggerd");
      //}
    }
    test = ""+rezultat+""; //<>//
    test = test.trim(); //<>//
    
    if(test.equals("Calitatea aerului din incapere este periculoasa !")) //<>//
    { //<>//
      led = color(0,255,0);
      line = color(255,255);
      buton_on_off = color(255,255,255);
      switcer = true; //<>//
      windowState = "Geamul este deschis";
      image(poluted_inside, 678, 380);
    } //<>// 
    test1 = ""+rezultat+"";
    test1 = test1.trim();

    if(test1.equals("Calitatea aerului din exterior este rea"))
    {
      led = color(255,0,0);
      buton_on_off = color(0,0,0);
      line = color(0,0);
      switcer = false;
      windowState = "Geamul este inchis";
      image(poluted_outside, 678, 380);
    }
  }
} //<>//

void mousePressed() 
{
  overCircle(700,550,104);  
}

boolean overCircle(int x, int y, int diameter) 
{
  float disX = x - mouseX;
  float disY = y - mouseY;
  if (sqrt(sq(disX) + sq(disY)) < diameter/2 )  //<>//
  {
    if(switcer == true)
    { //<>//
       windowState = "Geamul este inchis";
       led = color(255,0,0);
       myPort.write("2");
       buton_on_off = color(0,0,0);
       line = color(0,0);
       switcer = false;
    } 
    else if(switcer == false)
    {
       windowState = "Geamul este deschis";
       led = color(0,255,0);
       myPort.write("1");
       line = color(255,255);
       buton_on_off = color(255,255,255);
       switcer = true;
    }
    return true;
  } 
  else 
  {  
    return false;
  }
}
//<>// //<>// //<>// //<>// //<>// //<>//
