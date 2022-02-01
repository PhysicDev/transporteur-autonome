
import java.util.Collections;

public class Graphic{
  
  float x,y,w,h ;
  color fill_c = color(240);
  color stroke_c = color(70);
  float stroke_w = 1;
  ArrayList<Float> ordonee = new ArrayList<Float>();
  ArrayList<Float> abscice = new ArrayList<Float>();
  
  PFont t_f = createFont("Georgia",30);
  
  float lenght = 0;
  float start  = 0;
  
  String name,abs_unit,ord_unit;
  
  boolean absice_ponderee = false;
  
  boolean show_name = true;
  String name_pos = "CENTER";
  
  boolean Lsubdivide_1 = true;
  boolean Csubdivide_1 = true;
  
  float sub_Lsubdivide_ratio = 0.2;
  float sub_Csubdivide_ratio = 0.33;
  float pow_Lsubdivide_ratio = 1/sub_Lsubdivide_ratio;
  float pow_Csubdivide_ratio = 1/sub_Csubdivide_ratio;
  
  float unit_refC = 1;
  float unit_refL = 1;
  
  float subdivide_stroke = 3;
  
  color subdivision_color = color(20);
  color line_color = color(240,0,0);
  float line_weight = 1.5;
  
  Graphic(String Name, float x_, float y_ , float w_, float h_ ,color background){
   
     name = Name;
     x = x_;
     y = y_;
     w = w_;
     h = h_;
     fill_c = background;
    
  }
  
    
  Graphic(String Name, float x_, float y_ , float w_, float h_ ,color background, color stroke_color , color stroke_weight){
   
     name = Name;
     x = x_;
     y = y_;
     w = w_;
     h = h_;
     fill_c = background;
     stroke_c = stroke_color;
     stroke_w = stroke_weight;

    
  }
  
  void set_font( PFont font){
    
    t_f = font;
    
  }
  
  void set_lines(boolean O, boolean C , float ratio_subO , float ratio_subC , color div_c, float stroke_w, color line_c, float line_w,float refl,float refc,String unitl,String unitc){
    
      
    Lsubdivide_1 = O;
    Csubdivide_1 = C;
    
    sub_Lsubdivide_ratio = ratio_subO;
    sub_Csubdivide_ratio = ratio_subC;
    pow_Lsubdivide_ratio = 1/sub_Lsubdivide_ratio;
    pow_Csubdivide_ratio = 1/sub_Csubdivide_ratio;
    
    subdivide_stroke = stroke_w;
    
    subdivision_color = div_c;
    line_color = line_c;
    line_weight = line_w;
    
    
    unit_refC = refc;
    unit_refL = refl;
    
    
    abs_unit = unitl;
    ord_unit = unitc;
    
  }
  
  void set_style(color background, color stroke_color , float stroke_weight ){
    
     fill_c = background;
     stroke_c = stroke_color;
     stroke_w = stroke_weight;
     
  }
  
  void add(float value_1,float value_2){
    
    ordonee.add(value_1);
    abscice.add(value_2);
    lenght += 1;
    
  }
  
  void add(float value){
    
    ordonee.add(value);
    if(absice_ponderee){
      if(lenght == 0){
        abscice.add((float)0);
        start = millis();
      }else{
        abscice.add((float)(millis()-start));
      }
    }else{
        abscice.add((float)(lenght));
    }
    lenght += 1;
    
  }
  
  void show(){
   
    rectMode(CENTER);
    textFont(t_f);
    
    fill(fill_c);
    stroke(stroke_c);
    strokeWeight(stroke_w);
    
    rect(x,y,w,h);
    
    float max_A = Collections.max(abscice);
    float max_O = Collections.max(ordonee);
    
    
    float ratio_A = w/max_A;
    float ratio_O = h/max_O;
    textAlign(CENTER,CENTER);
    textSize(9);
    
    if(max_A != 0){
      float ratio = (w)/max_A;
      float base = 1;
      while(ratio < w){
         ratio *= pow_Lsubdivide_ratio;
         base *= pow_Lsubdivide_ratio;
      }
      while(ratio > 3){
          stroke(red(subdivision_color),green(subdivision_color),blue(subdivision_color),(float)Math.pow(500*(ratio/w),1.5));
          fill(red(subdivision_color),green(subdivision_color),blue(subdivision_color),(float)Math.pow(500*(ratio/w),1.5));
          strokeWeight(15*(ratio/w));
          if(15*(ratio/w) > subdivide_stroke){
            strokeWeight(subdivide_stroke);
          }
          for(int i = 1; i*ratio < w; i++){
            if(ratio > 20){
               float value_f = i*base/unit_refL;
               String value = value_f + "";
               if(value.length() > 4){
                  value = value.substring(0,4);
               }
               text(value,i*ratio+x-w/2,y+h/2+15);
            }
            line(i*ratio+x-w/2,y-h/2,i*ratio+x-w/2,y+h/2);
            
          }
          ratio *= sub_Lsubdivide_ratio;
          base *= sub_Lsubdivide_ratio;
      }
      
      
    }
    
    
    
    if(max_O != 0){
      float ratio = (h)/max_O;
      float base = 1;
      while(ratio < h){
         ratio *= pow_Csubdivide_ratio;
         base *= pow_Csubdivide_ratio;
      }
      while(ratio > 3){
          stroke(red(subdivision_color),green(subdivision_color),blue(subdivision_color),(float)Math.pow(500*(ratio/h),1.5));
          fill(red(subdivision_color),green(subdivision_color),blue(subdivision_color),(float)Math.pow(500*(ratio/h),1.5));
          strokeWeight(15*(ratio/h));
          if(15*(ratio/h) > subdivide_stroke){
            strokeWeight(subdivide_stroke);
          }
          for(int i = 1; i*ratio < h; i++){
            if(ratio > 20){
               float value_f = i*base/unit_refC;
               String value = value_f + "";
               if(value.length() > 4){
                  value = value.substring(0,4);
               }
               text(value,x-w/2-textWidth(value)-10,y+h/2-i*ratio);
            }
            line(x-w/2,y+h/2-i*ratio,x+w/2,y+h/2-i*ratio);
            
          }
          ratio *= sub_Csubdivide_ratio;
          base *= sub_Csubdivide_ratio;
      }
      
    }
    
    
    strokeWeight(2);
    stroke(255,0,0);
    for(int i = 0; i < lenght-1; i++){
     
      line(abscice.get(i)*ratio_A+x-w/2,y+h/2-ordonee.get(i)*ratio_O,abscice.get(i+1)*ratio_A+x-w/2,y+h/2-ordonee.get(i+1)*ratio_O);
      
    }
    
    if(show_name){
      if(name_pos == "CENTER"){
       
          fill(150);
          textSize(20);
          strokeWeight(0);
          rect(x,y-h/2+10,textWidth(name),20);
          stroke(15);
          fill(15);
          strokeWeight(1);
          text(name,x,y-h/2+10);
        
      }
    }
    
    
    
    
    rectMode(CORNER);
  }
  
  
}  
