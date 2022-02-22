import utilities.*;
import NeuralNet.*;
import java.lang.reflect.*;
import java.util.ArrayList;
import java.util.Random;

//forme de la fenêtre, 500px à gauche pour les menus, le reste pour le rendu de la simulation

/**

Code intégrallement fait par Vincent Callegari (Physic Dev)

On part du principe qu'un pixel vaut 0.5 cm

**/
public static int precision =30;

//base de donnée de bons reflexe
ArrayList<float[][]> DataBase= new ArrayList<float[][]>();

GeneticAlgorithm Brain =new GeneticAlgorithm(precision+5,new int[]{12},4);
int Obsnum=1;
int CarNum=60;
Obstacle[] environment = new Obstacle[Obsnum];
Car[] cars = new Car[CarNum];
float menuOffset=500;
float heightOffset=300;
float maxWallL=600;

float simulationTime=40;
float time=0;
float timeStep=0.05f;
//more iteration for better result
float moveIteration = 10;

Button reset;
Button resetC;
Button SimStart;
Slider obstacleControl;
Slider carControl;

//police d'écriture
PFont mainFont;

float[] target=new float[2];
void setup(){
  frameRate(-1);
  //pour ajouter facilment des élement de GUI entre d'autre élément
  float Boff=80;
  
  mainFont=createFont(dataPath("font/main.ttf"),60);
  textFont(mainFont);
  float[] radius={5,5,5,5};
  
  resetC=new Button("reset cars",170,Boff,170,40);;
  resetC.set_text(color(240),50f,mainFont,0f,-5f);
  resetC.set_fill(color(130,110,50));
  resetC.set_stroke(color(140),3,radius);
  resetC.click_effect = true;
  
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
  
  
  carControl = new Slider("nombres de voitures",40,Boff,400,0,4,color(#6991AC),3);Boff+=90;
  carControl.set_text("nombres de voitures",color(240),40,mainFont);
  carControl.set_grad(3,60,5,1,CarNum);
  carControl.set_cursor(1,10,20,color(#bfd6e5),color(#bfd6e5),0);
  carControl.round_value = true;
  carControl.show_value = true;
  
  
  SimStart=new Button("start simulation",40,Boff,250,40);;
  SimStart.set_text(color(240),50f,mainFont,0f,-5f);
  SimStart.set_fill(color(50,110,50));
  SimStart.set_stroke(color(140),3,radius);
  SimStart.click_effect = true;
  
  size(displayWidth, displayHeight);
  surface.setResizable(true);
  resetEnv();
  resetCar();
  Brain.setActivation(functions.Tanh);
  Brain.learningRate=0.0001f;
  Brain.init(CarNum);
  ellipseMode(CENTER);
}

void resetCar(){
  CarNum=(int)carControl.value;
  cars= new Car[CarNum];
  for(int i=0;i<CarNum;i++){
    cars[i]=new Car((width+menuOffset)/2,100);
    cars[i].getFitness();
    //cars[i].setPower(random(10,30),random(10,30));
  }
}

void resetEnv(){
  Obsnum=(int)obstacleControl.value;
  environment= new Obstacle[Obsnum];
  target[0]=(width+menuOffset)/2+random(800)-400;
  target[1]=height-100-random(300);
  for(int i=0;i<Obsnum;i++){
    float x=random(width-menuOffset)+menuOffset;
    float y=max(600,random(height-heightOffset)+heightOffset);
    environment[i]=new Obstacle(x,y,x+random(maxWallL)-maxWallL/2,y+random(maxWallL)-maxWallL/2);
  }
}

public static boolean running = false;
public static int step=0;
public static Random R=new Random();

void draw(){
  //background
  background(80);
  Car c;
  float[] output={0,0};
  for(int i=0;i<CarNum;i++){
    c=cars[i];
    if(running && !c.collision){
      step++;
      for(int j=0;j<moveIteration;j++){
        c.move(timeStep/moveIteration);
      }
      for(int j=0;j<precision;j++){
        c.rayCast(j*2.0*PI/precision,environment);
      }
      //Brain.getPop();
      if(Brain.getGen()>1 && step%4==0){
        c.better(cars);
      }
      c.computeBrainData();
      output=Brain.computeOutput(i,c.BrainData);
      float max = -2;
      int id=0;
      for(int j=0;j<4;j++){
        if(output[j]>max){
          max=output[j];
          id=j;
        }
        switch(id){
          case 0:
            c.setPower(20,20);
            break;
          case 1:
            c.setPower(-20,-20);
            break;
          case 2:
            c.setPower(25,0);
            break;
          case 3:
            c.setPower(0,25);
            break;
        }
      }
      //c.setPower(output[0]*25,output[1]*25);
    }else if(running){
      c.Fitness-=10;
    }
    //c.setPower(min(250,max(-250,c.targetV1+random(-2,2))),min(250,max(-250,c.targetV2+random(-2,2))));
    c.draw();
    strokeWeight(0.5);
    stroke(0,255,0);
  }
  if(running){
    time+=timeStep;
    if(time>simulationTime){
      for(int i=0;i<CarNum;i++){
        c=cars[i];
        Brain.setFit((int)c.getFitness(),i);
      }
      resetCar();
      println("best : "+Brain.nextGen());
      println("mean : "+Brain.meanFit());
      println("training with database of : " +DataBase.size());
      while(DataBase.size()>4000){
        DataBase.remove(R.nextInt(DataBase.size()));
      }
      println("training with database of : " +DataBase.size());
      if(DataBase.size()>0){
        for(int i=0;i<Brain.getPop();i++){
          System.out.println("training : "+i);
          //Brain.train(i,DataBase,DataBase.size()/4);
        }
      }
      time=0;
      simulationTime+=0.1f;
      simulationTime=min(simulationTime,40);
      //resetEnv();
    }
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
  
  
  resetC.update();
  if(resetC.activate){
    resetCar();
  }
  
  SimStart.update();
  if(SimStart.activate){
    if(running){
      SimStart.name="start simulation";
      SimStart.set_fill(color(50,110,50));
      resetC.eneable();
      reset.eneable();
      running =false;
    }else{
      SimStart.name="stop simulation";
      SimStart.set_fill(color(110,50,50));
      resetC.diseable();
      reset.diseable();
      running =true;
    }
  }
  
  textAlign(LEFT,TOP);
  textSize(20);
  text("time : "+time,70,height-200);
  text("FPS : "+round(frameRate),70,height-250);
  text("GEN : "+Brain.getGen(),70,height-300);
  obstacleControl.update();
  carControl.update();
  //DumpMethods.work();
}

public static class DumpMethods {

    public void foo() { }

    public int bar() { return 12; }

    public String baz(){ 
     return("bruh");
    }

    public static void work() {
        try {
            Class thisClass = GeneticAlgorithm.class;
            Method[] methods = thisClass.getDeclaredMethods();

            for (int i = 0; i < methods.length; i++) {
                System.out.println(methods[i].toString());
            }
        } catch (Throwable e) {
            System.err.println(e);
        }
    }
}
