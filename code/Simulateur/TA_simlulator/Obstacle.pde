//mur defini par un segment
public class Obstacle{
  public float X1,X2,Y1,Y2;
  public Obstacle(float x1,float y1,float x2,float y2){
    /**pour simplifier le calcul de raycasting
    on fait en sorte que le point (X1;Y1) soit toujours le point à droite
    **/
    if(x1<x2){
      X1=x1;
      Y1=y1;
      X2=x2;
      Y2=y2;
    }else{
      X2=x1;
      Y2=y1;
      X1=x2;
      Y1=y2;
    }
  }
  
  public void draw(){
    line(X1,Y1,X2,Y2);
  }
}
