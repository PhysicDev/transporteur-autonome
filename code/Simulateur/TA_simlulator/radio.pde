public class Radio{
  
  String name;
  
  float size;
  float x,y,s_w,c_s;
  
  boolean click = false;
  boolean activate = false;
  
  color c_c = color(255,0,0);
  color background = color(255);
  color stroke = color(255);
  String type = "POINT";
  String shape = "RECT";
  
  Radio(String Name , float x_ , float y_, float Size , color fill_c , color stroke_c , float stroke_w , String TYPE){
   
      name = Name;
      x = x_;
      y = y_;
      size = Size;
      c_s = c_s * 0.8;
      background = fill_c;
      stroke = stroke_c;
      s_w = stroke_w;
      shape = TYPE;
    
  }
  
  void set_cursor(String TYPE, float Size, color color_c){
    
    type = TYPE;
    c_s = Size;
    c_c = color_c;
    
  }
  
  void update(){
    
     maths_update();
     graphics_update();
     
  }
  
    boolean hover(){
        boolean reponse = false;
        
        if(mouseX < x+size/2 && mouseX > x-size/2 && mouseY < y+size/2 && mouseY > y-size/2){
          
          reponse = true;
          
        }
          
          return(reponse);
    }
      
  
  
  void maths_update(){
     if(mousePressed && !click && mouseButton == LEFT){
         if(hover()){
           click = true;
         }
       }
       if(!mousePressed && click){
         if(hover()){
           if(activate){
             activate = false;
           }else{
             activate = true;
           }
         }
         click = false;
       }
    
    
    
  }
  
  void graphics_update(){
    
    fill(background);
    stroke(0);
    strokeWeight(s_w);
    rectMode(CENTER);
    if(shape == "RECT"){
      rect(x,y,size,size);
    }else{
      ellipse(x,y,size,size);
    }
    
    noStroke();
    if(hover()){
      
      fill(240,100);
       if(shape == "RECT"){
         rect(x,y,size+s_w,size+s_w);
       }else{
         ellipse(x,y,size+s_w,size+s_w);
       }
      
    }
    
    if(click){
      
      fill(80,100);
       if(shape == "RECT"){
         rect(x,y,size+s_w,size+s_w);
       }else{
         ellipse(x,y,size+s_w,size+s_w);
       }
      
    }
    
    if(activate){
      
      fill(c_c);
      if(type  == "POINT"){
         ellipse(x+1,y+1,c_s,c_s);
      }else if(type == "RECT"){
         rect(x+1,y+1,c_s,c_s);
      }else if(type == "CROIX"){
         quad(x-c_s/2+1,y+1-(c_s/4),x+1-c_s/4,y+1-(c_s/2),x+1+c_s/2,y+1+(c_s/4),x+1+c_s/4,y+1+(c_s/2));
         quad(x-c_s/2+1,y+1+(c_s/4),x+1-c_s/4,y+1+(c_s/2),x+1+c_s/2,y+1-(c_s/4),x+1+c_s/4,y+1-(c_s/2));
      }else{
         ellipse(x,y,c_s,c_s);
      }
      
    }
    
    rectMode(CORNER);
  }

}
