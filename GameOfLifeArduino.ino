#include <LiquidCrystal.h>
#include <LedControl.h>

class GameOfLife {
  private :
    boolean mode;
    boolean gol [8][8];
    LedControl lc = LedControl(12,10,11,1);
    
  public : 
    GameOfLife(boolean mode){
      this->mode = mode;
      for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
          gol[y][x] = false;  
        }
      }
      lc.shutdown(0,false);
      lc.setIntensity(0,8);
      lc.clearDisplay(0);
    }

    boolean inBound(int x,int y){
      return x >= 0 and x < 8 and y >= 0 and y < 8;
    }

    boolean activate(int x,int y){
      if(inBound(x,y)) gol[y][x] = true;
    }

    void randomFill(){
      for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
          randomSeed(analogRead(A3));
          gol[y][x] = random(2);
        }
      }
    }
    
    int countNeighbors(int x, int y){
      int a = 0;
      int b = 0;
      int count = 0;
      for(int j = -1; j < 2; j++){
        for(int i = -1; i < 2; i++){
          if(i != 0 or j != 0){
            a = x + i;
            b = y + j;
            if(mode){
              if(a < 0) a = 7;
              if(b < 0) b = 7;
              if(a > 7) a = 0;
              if(b > 7) b = 0;
              if (gol[b][a]) count++;
            } else {
              if(inBound(a,b) and gol[b][a]) count++; 
            }
          }
        }
      }
      return count;
    }
    
    void next(){
      boolean updated[8][8];
      int aliveNeighbors = 0;
      for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
          aliveNeighbors = countNeighbors(x,y);
          if((gol [y][x] and (aliveNeighbors == 2 or aliveNeighbors == 3)) or (not(gol [y][x]) and aliveNeighbors == 3)) updated[y][x] = true;
          else  updated[y][x] = false;
        }
      }
      for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
          gol[y][x] = updated[y][x];
        }
      }
    }

    void show(){
      for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
          lc.setLed(0,x,y,gol[y][x]);
        }
      }
    }
};


GameOfLife gol = GameOfLife(true);

void setup() {
  gol.randomFill();
}

void loop() {
  gol.show();
  gol.next();
  delay(200);
}
