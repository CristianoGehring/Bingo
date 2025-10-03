#include "M5Cardputer.h"
#include <vector>

// Estados do jogo
enum GameState {
  MENU,
  PLAYING,
  VIEW_BALLS
};

GameState state = MENU;
std::vector<int> drawnBalls;
std::vector<int> availableBalls;
int currentBall = 0;
int menuOption = 0;

// Declaração das funções
void initGame();
void drawMenu();
void drawNextBall();
void drawBallsList();
bool isKey(Keyboard_Class::KeysState status, char key);

void setup() {
  auto cfg = M5.config();
  M5Cardputer.begin(cfg, true);  // true = ativa o teclado
  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setTextSize(2);
  M5Cardputer.Display.setTextColor(WHITE);
  
  initGame();
  drawMenu();
}

void initGame() {
  drawnBalls.clear();
  availableBalls.clear();
  
  for(int i = 1; i <= 75; i++) {
    availableBalls.push_back(i);
  }
  
  currentBall = 0;
}

void drawMenu() {
  M5Cardputer.Display.fillScreen(BLACK);
  M5Cardputer.Display.setCursor(10, 10);
  M5Cardputer.Display.setTextSize(3);
  M5Cardputer.Display.println("BINGO");
  
  M5Cardputer.Display.setTextSize(2);
  M5Cardputer.Display.setCursor(10, 60);
  
  if(menuOption == 0) M5Cardputer.Display.setTextColor(YELLOW);
  else M5Cardputer.Display.setTextColor(WHITE);
  M5Cardputer.Display.println("> Iniciar");
  
  M5Cardputer.Display.setCursor(10, 85);
  if(menuOption == 1) M5Cardputer.Display.setTextColor(YELLOW);
  else M5Cardputer.Display.setTextColor(WHITE);
  M5Cardputer.Display.println("> Ver Bolas");
  
  M5Cardputer.Display.setCursor(10, 110);
  M5Cardputer.Display.setTextColor(DARKGREY);
  M5Cardputer.Display.setTextSize(1);
  M5Cardputer.Display.println("Use W/S e ENTER");
}

void drawNextBall() {
  if(availableBalls.empty()) {
    M5Cardputer.Display.fillScreen(BLACK);
    M5Cardputer.Display.setCursor(20, 50);
    M5Cardputer.Display.setTextSize(2);
    M5Cardputer.Display.setTextColor(RED);
    M5Cardputer.Display.println("Fim do Jogo!");
    M5Cardputer.Display.setCursor(10, 80);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(WHITE);
    M5Cardputer.Display.println("Pressione ESC");
    return;
  }
  
  int idx = random(availableBalls.size());
  currentBall = availableBalls[idx];
  availableBalls.erase(availableBalls.begin() + idx);
  drawnBalls.push_back(currentBall);
  
  M5Cardputer.Display.fillScreen(BLACK);
  
  M5Cardputer.Display.fillCircle(120, 60, 40, BLUE);
  M5Cardputer.Display.drawCircle(120, 60, 40, WHITE);
  
  M5Cardputer.Display.setCursor(95, 45);
  M5Cardputer.Display.setTextSize(4);
  M5Cardputer.Display.setTextColor(WHITE);
  M5Cardputer.Display.println(currentBall);
  
  M5Cardputer.Display.setTextSize(1);
  M5Cardputer.Display.setCursor(10, 110);
  M5Cardputer.Display.print("Sorteadas: ");
  M5Cardputer.Display.print(drawnBalls.size());
  M5Cardputer.Display.print("/75");
  
  M5Cardputer.Display.setCursor(10, 122);
  M5Cardputer.Display.println("SPACE=Proxima ESC=Menu");
}

void drawBallsList() {
  M5Cardputer.Display.fillScreen(BLACK);
  M5Cardputer.Display.setCursor(10, 5);
  M5Cardputer.Display.setTextSize(2);
  M5Cardputer.Display.setTextColor(YELLOW);
  M5Cardputer.Display.println("Bolas Sorteadas");
  
  if(drawnBalls.empty()) {
    M5Cardputer.Display.setCursor(10, 50);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(WHITE);
    M5Cardputer.Display.println("Nenhuma bola sorteada");
  } else {
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.setTextColor(WHITE);
    
    int x = 10, y = 30;
    for(int i = drawnBalls.size() - 1; i >= 0 && i >= (int)drawnBalls.size() - 36; i--) {
      if((drawnBalls.size() - 1 - i) % 6 == 0 && i != (int)drawnBalls.size() - 1) {
        y += 12;
        x = 10;
      }
      
      M5Cardputer.Display.setCursor(x, y);
      M5Cardputer.Display.print(drawnBalls[i]);
      x += 35;
    }
    
    M5Cardputer.Display.setCursor(10, 110);
    M5Cardputer.Display.print("Total: ");
    M5Cardputer.Display.print(drawnBalls.size());
  }
  
  M5Cardputer.Display.setCursor(10, 122);
  M5Cardputer.Display.println("ESC=Voltar R=Reiniciar");
}

bool isKey(Keyboard_Class::KeysState status, char key) {
  if(status.word.empty()) return false;
  return (status.word[0] == key && status.word.size() == 1);
}

void loop() {
  M5Cardputer.update();
  
  if(M5Cardputer.Keyboard.isChange()) {
    if(M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
      
      // ESC (tecla DEL no Cardputer)
      if(status.del) {
        state = MENU;
        menuOption = 0;
        drawMenu();
      }
      
      // Tecla R reinicia
      else if(isKey(status, 'r')) {
        initGame();
        if(state == PLAYING) {
          drawNextBall();
        } else {
          state = MENU;
          drawMenu();
        }
      }
      
      // Menu
      else if(state == MENU) {
        if(isKey(status, 'w')) {
          menuOption = 0;
          drawMenu();
        }
        else if(isKey(status, 's')) {
          menuOption = 1;
          drawMenu();
        }
        else if(status.enter) {
          if(menuOption == 0) {
            state = PLAYING;
            drawNextBall();
          } else {
            state = VIEW_BALLS;
            drawBallsList();
          }
        }
      }
      
      // Jogando
      else if(state == PLAYING) {
        if(status.space && !availableBalls.empty()) {
          drawNextBall();
        }
      }
    }
  }
  
  delay(50);
}