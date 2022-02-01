//forme de la fenêtre, 500px à gauche pour les menus, le reste pour le rendu de la simulation

/**

Code intégrallement fait par Vincent Callegari (Physic Dev)

On part du principe qu'un pixel vaut 0.5 cm

**/
int Obsnum=30;
int CarNum=30;
Obstacle[] environment = new Obstacle[Obsnum];
Car[] cars = new Car[CarNum];
float menuOffset=500;
float heightOffset=300;
float maxWallL=600;

Button reset;
Slider obstacleControl;

//police d'écriture
PFont mainFont;

float[] target=new float[2];
void setup(){
  
  
  
  //pour ajouter facilment des élement de GUI entre d'autre élément
  float Boff=80;
  
  mainFont=createFont(dataPath("font/main.ttf"),60);
  textFont(mainFont);
  float[] radius={5,5,5,5};
  
  //GUIinit
  reset=new Button("reset",40,Boff,100,40);Boff+=100;
  reset.set_text(color(240),50f,mainFont,0f,-5f);
  reset.set_fill(color(50,110,50));
  reset.set_stroke(color(140),3,radius);
  reset.click_effect = true;
  
  obstacleControl = new Slider("nombres d'obstacles",40,Boff,400,0,4,color(#6991AC),3);Boff+=90;
  obstacleControl.set_text("nombres d'obstacles",color(240),40,mainFont);
  obstacleControl.set_grad(0,30,5,1,10);
  obstacleControl.set_cursor(1,10,20,color(#bfd6e5),color(#bfd6e5),0);
  obstacleControl.round_value = true;
  obstacleControl.show_value = true;
  
  size(displayWidth, displayHeight);
  surface.setResizable(true);
  resetEnv();
  resetCar();
  ellipseMode(CENTER);
}

void resetCar(){
  for(int i=0;i<CarNum;i++){
    cars[i]=new Car((width+menuOffset)/2,100);
    //cars[i].angle=random(2*PI);
    cars[i].setPower(random(10,30),random(10,30));
  }
}

void resetEnv(){
  Obsnum=(int)obstacleControl.value;
  environment= new Obstacle[Obsnum];
  target[0]=(width+menuOffset)/2+random(600)-300;
  target[1]=height-100-random(100);
  for(int i=0;i<Obsnum;i++){
    float x=random(width-menuOffset)+menuOffset;
    float y=random(height-heightOffset)+heightOffset;
    environment[i]=new Obstacle(x,y,x+random(maxWallL)-maxWallL/2,y+random(maxWallL)-maxWallL/2);
  }
}

void draw(){
  //background
  background(80);
  
  for(Car c:cars){
    for(int i=0;i<10;i++){
      c.move(0.0017);
    }
    c.setPower(min(500,max(-500,c.targetV1+random(-2,2))),min(500,max(-500,c.targetV2+random(-2,2))));
    c.draw();
  }
  //environment
  stroke(240);
  strokeWeight(4);
  for(Obstacle o:environment){
    o.draw();
  }
  stroke(240,150,50);
  ellipse(target[0],target[1],5,5);
  
  //menu
  rectMode(CORNER);
  noStroke();
  fill(110);
  rect(0,0,500,height);
  fill(220);
  
  textAlign(LEFT,TOP);
  textSize(70);
  text("CONTROL PANEL",40,15);
  //reset button
  reset.update();
  if(reset.activate){
    resetEnv();
  }
  
  textAlign(LEFT,TOP);
  textSize(20);
  text("FPS : "+frameRate,70,height-50);
  obstacleControl.update();
}
