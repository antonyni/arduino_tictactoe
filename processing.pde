import processing.serial.*;
Serial myPort;

void setup(){
  size(450, 300);
  myPort = new Serial(this, "COM5", 9600); // Starts the serial communication
}

boolean clicked = false;
void draw(){
  background(237, 240, 241);
  fill(20, 160, 133); // Green Color
  stroke(33);
  strokeWeight(1);
  rect(75, 100, 100, 50, 10);  // Left Button
  rect(275, 100, 100, 50, 10); // Right Button
  rect(175,200,100,50,10);
  fill(255);
  
  textSize(32);
  text("Left",95, 135);
  text("Right", 280, 135);
text("Select", 180, 235);
  textSize(24);
  fill(33);
  textSize(30);
  textSize(16);

  
  
  if(mousePressed && mouseX>75 && mouseX<175 && mouseY>100 && mouseY<150){
    if(clicked == false ){
      myPort.write('2'); 
      stroke(255,0,0);
      strokeWeight(2);
      noFill();
      rect(75, 100, 100, 50, 10);
      clicked = true;
    }
   
  }
  else if(mousePressed && mouseX>275 && mouseX<375 && mouseY>100 && mouseY<150){
    if(clicked == false){
      myPort.write('1'); 
      stroke(255,0,0);
      strokeWeight(2);
      noFill();
      rect(275, 100, 100, 50, 10);
      clicked = true;
    }
  }
  else if(mousePressed && mouseX > 175 && mouseX < 275 && mouseY > 200 && mouseY < 250){
    if(clicked == false){
      myPort.write('3'); 
      stroke(255,0,0);
      strokeWeight(2);
      noFill();
      rect(175, 200, 100, 50, 10);
      clicked = true;
    }
  }
  else {
      clicked = false;
  }
}
