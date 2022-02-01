public class Slider{
  
  public float x,y,l,min,max,s_w,h_g,b_g;
  public float t_s = 15;
  public float value = 0;
  public float c_s_w =1;
  public float c_x = 15,c_y = 15;
  public color c_c,c_s_c = color(0);
  public color s_c;
  public int type = 0;
  public String name;
  public color t_c =color(0);
  public PFont t_f = createFont("Georgia",30);
  public float ghost_value = 0;
  
  public float drag = x;  
  public boolean click = false;
  
  public boolean round_value = false;
  public boolean show_value= false;
  
  
  
  public Slider(String n,float x_ ,float y_ , float l_ , float mi , float ma){
     
     x = x_; 
     y = y_;
     l = l_;
     min = mi;
     max = ma;
     name = n;
      
  }
  
  public Slider(String n,float x_ ,float y_ , float l_ , float mi , float ma,color stroke_c, float stroke_w){
     
     x = x_; 
     y = y_;
     l = l_;
     min = mi;
     value= min;
     max = ma;
     s_c = stroke_c;
     s_w = stroke_w;
     name = n;
      
      
  }
  
  public void set_text(String n,  color text_c, float text_s , PFont text_f){
    
    name = n;
    t_c = text_c;
    t_s = text_s;
    t_f = text_f;
    
  }
  
  public void set_cursor(int t,float  cursor_x , float cursor_y, color cursor_c,color cursor_stroke_c,float cursor_stroke_w){
    
    type = t;
    c_x = cursor_x;
    c_y = cursor_y;
    c_c = cursor_c;
    c_s_c = cursor_stroke_c;
    c_s_w = cursor_stroke_w;
    
  }
  
  public void set_grad(float mi, float ma, float haut_g, float bas_g, float pos){
    
     min = mi;
     max = ma;
     h_g = haut_g;
     b_g = bas_g;
     value = pos;
     ghost_value = value;
    
  }
  
  public void graphics_update(){
    
    rectMode(CORNER);
    stroke(s_c);
    strokeWeight(s_w);
    line(x,y,x+l,y);
    line(x,y-7,x,y+7);
    line(x+l,y-7,x+l,y+7);
    
    float ratio = l/(max-min);
    if( h_g !=0){
      strokeWeight(s_w*0.7);
      for(float i = min; i < max ; i += h_g){
       
        line((i-min)*ratio+x,y-5,(i-min)*ratio+x,y+5);
       
      } 
    }
    if(b_g != 0){
      strokeWeight(s_w*0.4);
      for(float i = min; i < max ; i += b_g){
       
        line((i-min)*ratio+x,y-4,(i-min)*ratio+x,y+4);
       
      } 
    }
    
    fill(c_c);
    stroke(c_s_c);
    strokeWeight(c_s_w);
    if(type == 0){
      ellipse((value-min)*ratio+x,y,c_x,c_y);
    }else{
      rect((value-min)*ratio+x-c_x/2,y-c_y/2,c_x,c_y);
    }
    noStroke();
    textFont(t_f);
    textSize(t_s);
    fill(t_c);
    textAlign(LEFT, BOTTOM);
    text(name+" : "+(show_value?(round_value?int(value):""+value):""),x,y-10);
   // textAlign(LEFT, CENTER);
   // text(name+" :",x,y-10);
   // textAlign(LEFT, TOP);
   // text(name+" :",x,y-10);
    
    
  }
  
  public void update(){
   physics_update();
   graphics_update();    
  }
  
  public void physics_update(){
    
    
    if(mousePressed && !click && mouseButton == LEFT){
     if(hover()){
       click = true;
       drag = mouseX;
       ghost_value = value;
     }
   }
   if(!mousePressed && click){
       click = false;
       drag = x;
   }
   if(click){
     float ratio = (max-min)/l;
     
     float movement = mouseX-drag;
     
     ghost_value += movement*ratio;
     if(round_value){
       value = Math.round(clamp(ghost_value,min,max));
     }else{
       value = clamp(ghost_value,min,max);
     }
    
     drag = mouseX;
   }
  }
  
  public float clamp(float v, float mi,float ma){
     float result = v;
     if(v > ma){
          result = ma; 
           System.out.println("too big");
     }
     if(v < mi){
          result = mi; 
     }
     return(result);
  }
  
  public boolean hover(){
    float ratio = l/(max-min);
    boolean reponse = false;
    if(mouseX < (value-min)*ratio+x+c_x/2 && mouseX > (value-min)*ratio+x-c_x/2 && mouseY < y+c_y/2 && mouseY > y-c_y/2){
      reponse = true;
    }
    return(reponse);
  }
  
}
