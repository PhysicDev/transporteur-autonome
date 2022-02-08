//rappel : un pixel correspond à 0.5cm

import java.util.Arrays;

//pour l'instant je suppose que le centre de gravité est entre les deux roues

public static float conversion=200;//pour passer des pixels (0.5cm) aux mètres
public static float wheelAxe=30;//distance entre les roues (équivaut à 15cm)
public static float maxAcc=1000;//accélération maximale possible (en pixels/s²)
public static float maxMom=PI/2;//moment maximal possible (en rad/s²)

public static float range= 10000;
public static float MaxMotorSpeed = 200;//vitesse maximale des moteurs en pixel/s

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
  
  
  //on considère que la voiture ne peut pas basculer  
  public float angle;//orientation par rapport à la verticale
  public float moment; //moment axe Z
  public float Vangle;
  
  //vitesse et accélération dans la base de frenet
  public float Anorm;
  public float Atan;
  public float Vtan;
  
  
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
  
  public int rayCast(float direction, Obstacle[] data){
    direction-=angle;
    float Y=sin(direction)*range;
    float X=cos(direction)*range;
    line(x,y,x+X*0.001,y+Y*0.001);
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
    line(x,y,minInter[0],minInter[1]);
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
  
  public void move(float deltaT){
    computeAcc(deltaT);
    /**
    vx+=ax*deltaT;
    vy+=ay*deltaT;
    x+=vx*deltaT;
    y+=vy*deltaT;**/
    Vangle+=moment*deltaT;
    float distance=Vtan*deltaT;
    x+=distance*sin(angle);
    y+=distance*cos(angle);
    angle+=Vangle*deltaT;
    angle=(angle+2*PI)%(2*PI);//on ajoute 2*pi pour éviter les problèmes avec le module de nombre négatif
  }
  
  public void draw(){
    noStroke();
    fill(#66d8db);
    pushMatrix();
    rectMode(CENTER);
    translate(x, y);
    rotate(-angle);
    rect(0, 20, 30, 50);
    popMatrix();
    stroke(1);
    strokeWeight(2);
    line(x,y,x+Vtan*sin(angle),y+Vtan*cos(angle));
    stroke(255,0,0);
    strokeWeight(1);
    line(x,y,x+Atan*sin(angle)+Anorm*cos(angle),y+Atan*cos(angle)-Anorm*sin(angle));
  }
}
