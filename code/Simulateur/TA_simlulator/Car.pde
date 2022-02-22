//rappel : un pixel correspond à 0.5cm

import java.util.Arrays;
import java.util.ArrayList;

//pour l'instant je suppose que le centre de gravité est entre les deux roues

public static float conversion=200;//pour passer des pixels (0.5cm) aux mètres
public static float wheelAxe=30;//distance entre les roues (équivaut à 15cm)
public static float maxAcc=1000;//accélération maximale possible (en pixels/s²)
public static float maxMom=PI/2;//moment maximal possible (en rad/s²)
public static float collisionDistance = 40;
public static float range= 10000;
public static float MaxMotorSpeed = 200;//vitesse maximale des moteurs en pixel/s

//distance à partir de laquelle on considère les voitures comme voisines
public static float neighDist = 100;

public static float[] empty =  {0f,0f};

public static float Mass=1.5;//masse de la voiture en Kg

public class Car{
  //certaines de ces variables ne seront peût être pas utiles et je les supprimerais 
  // lors de l'optimisation du calcul
  
  //positions, vitesses et accélérations coordonées cartésiennes
  public float x,y,vx,vy;
  public float ax=0,ay=0;
  
  //note : le moteur 1 est le moteur droit et le moteur 2 est le moteur gauche
  private float V1,V2;//vitesse des moteurs
  private float A1,A2;//accélération des moteurs
  public float targetV1;
  public float targetV2;
  public float parcours=0;
  public boolean collision=false;
  
  //on considère que la voiture ne peut pas basculer  
  public float angle;//orientation par rapport à la verticale
  public float moment; //moment axe Z
  public float Vangle;
  
  //vitesse et accélération dans la base de frenet
  public float Anorm;
  public float Atan;
  public float Vtan;
  
  public float[] BrainData = new float[precision+5];
  
  public float Fitness=0;
  public float lastFitness=0;
  public float deltaFit=0;
  public float getFitness(){
    float Fit= Fitness-dist(target[0],target[1],x,y);
    //float Fit=0;
    //si la voiture n'a presque pas bougé c'est une mauvais réseaux
    if(dist(x,y,(width+menuOffset)/2,100)<600){
      if(dist(x,y,(width+menuOffset)/2,100)<300){
        Fit-=1500;
      }
      Fit-=1000;
    } 
    //si il n'y a pas eu de collision c'est bien
    if(!collision){
      Fit+=500;
    }
    return(Fit);
  }
  
  //verifie si le robot est "mieux" que ses voisins
  public void better(Car[] voisins){
    float deltaFit = getFitness()-lastFitness;
    float meanFit=0;
    float maxFit=-Float.MAX_VALUE;
    float minFit=Float.MAX_VALUE;
    int neigh=0;
    for(Car c:voisins){
      if(dist(x,y,c.x,c.y)<neighDist && c!=this){
        if(c.deltaFit>maxFit){
          maxFit=c.deltaFit;
        }
        if(c.deltaFit<minFit){
          minFit=c.deltaFit;
        }
        meanFit=((meanFit*neigh)+c.deltaFit)/(neigh+1);
        neigh++;
      }
    }
    //s'il y a asser de voisins pour pouvoir se comparer, on le fait
    if(neigh>5){
      if(deltaFit>4*(meanFit+maxFit)/5){
          DataBase.add(new float[][]{BrainData,{targetV1,targetV2}});
      }
    }
    lastFitness=getFitness();
  }
  
  public Car(float X,float Y){
    x=X;y=Y;
    vx=0;vy=0;
    V1=0;V2=0;
    angle=0;moment=0;Vangle=0;
    Anorm=0;Atan=0;Vtan=0;
  }
  public void setPower(float Motor1,float Motor2){
    targetV1=Motor1;
    targetV2=Motor2;
    targetV1=min(MaxMotorSpeed,targetV1);
    targetV1=max(-MaxMotorSpeed,targetV1);
    targetV2=min(MaxMotorSpeed,targetV2);
    targetV2=max(-MaxMotorSpeed,targetV2);
  }
  
  //la variable deltaT correspond au temps passé depuis la mise à jour du mouvement
  //des valeurs de deltaT plus faibles donnerons de meilleurs résultats (mais ça prendra plus de temps de calcul) 
  
  
  /**calcule les accélérations X,Y et angulaire en fonction de V1,V2 et de la distance entre les roues
  * avec les formules suivantes : Vangle = (V1-V2)/wheelAxe
  *                               Aangle = (newVangle-Vangle)/deltaT
  *                               Ax=Anormal*cos(angle)
  *                               Ay=Anormal*sin(angle)
  *                               Anormal=(A1+A2)/2
  **/
  private void computeAcc(float deltaT){
    A1=(targetV1-V1)/deltaT;
    A2=(targetV2-V2)/deltaT;
    //on limite les accélérations
    A1=min(maxAcc,A1);
    A1=max(-maxAcc,A1);
    A2=min(maxAcc,A2);
    A2=max(-maxAcc,A2);
    //on met à jour les vitesses des moteurs
    V1+=A1*deltaT;
    V2+=A2*deltaT;
    
    float NewVangle=(V2-V1)/wheelAxe;
    //on recalcule l'accélération avec la nouvelle vitesse pour pouvoir la limiter
    moment=(NewVangle-Vangle)/deltaT;
    moment=min(maxMom,moment);
    moment=max(-maxMom,moment);
    
    //on calcule les accélerations dans la base de frenet  
    Atan=(A1+A2)/2;
    Vtan=(V1+V2)/2;
    
    //accélération normale
    if(Vangle==0){
      //pour éviter les problèmle de division par zéro
      Anorm=0;
    }else{
      //accélération normale
      Anorm=Vtan*Vangle;
      Anorm=min(maxAcc,Anorm);
      Anorm=max(-maxAcc,Anorm);
    }
    
    //conversions des accélérations en coordonées cartésiennes
    ax = Anorm*cos(angle)+Atan*sin(angle);
    ay = Anorm*sin(angle)+Atan*cos(angle);
  }
  public void computeBrainData(){
  
    float Tdist=dist(target[0],target[1],x,y);
    float difangle = acos((x-target[0])/Tdist)-angle;
    if(difangle>PI){
      difangle-=2*PI;
    }
    if(difangle<-PI){
      difangle+=2*PI;
    }
    BrainData[precision+2]=difangle;
    difangle+=PI;
    if(difangle>PI){
      difangle-=2*PI;
    }
    BrainData[precision+3]=difangle;
    BrainData[precision]=Vtan;
    BrainData[precision+1]=Anorm;
    BrainData[precision+4]=dist(target[0],target[1],x,y);
  
  }
  
  public void move(float deltaT){
    Fitness+=2;
    computeAcc(deltaT);
    /**
    vx+=ax*deltaT;
    vy+=ay*deltaT;
    x+=vx*deltaT;
    y+=vy*deltaT;**/
    Vangle+=moment*deltaT;
    float distance=Vtan*deltaT;
    parcours+=distance;
    x+=distance*sin(angle);
    y+=distance*cos(angle);
    angle+=Vangle*deltaT;
    if(angle>PI){
      angle-=2*PI;
    }
    if(angle<-PI){
      angle+=2*PI;
    }
  }
  
  public int rayCast(float direction, Obstacle[] data){
    direction-=angle;
    float Y=sin(direction)*range;
    float X=cos(direction)*range;
    //line(x,y,x+X*0.001,y+Y*0.001);
    float minDistance=range;
    float[] minInter={x+X,y+Y};
    float[] Inter;
    float distance;
    for(Obstacle O:data){
      Inter=intersection(O.X1,O.Y1,O.X2,O.Y2,x,y,x+X,y+Y);
      if(Inter!=empty){
        distance=dist(x,y,Inter[0],Inter[1]);
        if(distance<minDistance){
          minDistance=distance;
          minInter=Inter.clone();
        }
      }
    }
    if(direction<0){
      direction+=precision;
    }
    if(minDistance==range){
      BrainData[(int)direction]=999;
      //set distance to zero
    }else if(minDistance<collisionDistance*3){
      BrainData[(int)direction]=minDistance;//500/minDistance-1;
      if(minDistance<collisionDistance){
        Fitness-=1;
        collision=true;
      }
      //collision so set bad fitness
    }else{
      BrainData[(int)direction]=minDistance;//50/minDistance-0.7;
    }
    //line(x,y,minInter[0],minInter[1]);
    return(0);
  }
  
  /**
    p=x,y
    q=X1,Y1
    r=X-x,Y-y
    s=X2-X1,Y2-Y1
  **/
  
  public float[] intersection(float Cx,float Cy,float Dx,float Dy,float Ax,float Ay,float Bx,float By) {
    
    float V1x=Bx-Ax;
    float V1y=By-Ay;
    float V2x=Cx-Dx;
    float V2y=Cy-Dy;
    if(V1x*V2y-V1y*V2x==0){
      return(empty);
    }
    float kab=(V2x*(Ay-Cy)-(Ax-Cx)*V2y)/
           (V2y*V1x-V1y*V2x);
    float kcd=(V1x*(Cy-Ay)-(Cx-Ax)*V1y)/
           (V1y*V2x-V2y*V1x);
    if(kab>1||kab<0) {
      return(empty);
    }
    if(kcd>0||kcd<-1) {
      return(empty);
    }
    float[] result={V1x*kab+Ax,V1y*kab+Ay};
    return(result);
    //return(new Vecteur2(orig,fin).produit(kab).somme(new Vecteur2(orig)).getPoint());
  }
  
  
  public void draw(){
    noStroke();
    if(collision){
      fill(102-Fitness/10,216+(Fitness)/10+50,102);
    }else{
      fill(102,216,219);
    }
    pushMatrix();
    rectMode(CENTER);
    translate(x, y);
    rotate(-angle);
    rect(0, -20, 30, 50);
    popMatrix();
    stroke(1);
    strokeWeight(2);
    line(x,y,x+Vtan*sin(angle),y+Vtan*cos(angle));
    stroke(255,0,0);
    strokeWeight(1);
    line(x,y,x+Atan*sin(angle)+Anorm*cos(angle),y+Atan*cos(angle)-Anorm*sin(angle));
    //line(x,y,x+10,y);
    //text(angle,x,y);
  }
}
