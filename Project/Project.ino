#include <LedControl.h>

LedControl lc = LedControl(11, 9, 10, 4);

const int button_left   = 2;
const int button_right  = 3;
const int button_down   = 4;
const int button_rotate = 5;
const int button_start  = 6;
 
int led1[8] = {0};
int led2[8] = {0};
long active[16] = {0};
long screen[16] = {0};

int tmpCol = 0;
int lcCols = 8;
int lcRows = 16;
int colCheck = 0;

int shape = 0;
int shape_Next = 0;

int move = 0;
int angle = 0;
int fromLeft = 0;
int fromRight = 0;

int start = 0;
int allLines = 0;
int currLines = 0;

int score = 0;
int level = 0;

int Delays = 0;
int defualt_delay = 500;

void setup() {
  pinMode(button_rotate, INPUT_PULLUP);
  pinMode(button_down,   INPUT_PULLUP);
  pinMode(button_right,  INPUT_PULLUP);
  pinMode(button_left,   INPUT_PULLUP);
  pinMode(button_start,  INPUT_PULLUP);
  for (int i = 0; i < 4; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 5);
    lc.clearDisplay(i);
  }
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void updateColumn(int colnum) {
  led1[colnum] = active[colnum] >> (lcRows / 2);
  led2[colnum] = active[colnum];
  
  lc.setRow(1, colnum, (screen[colnum] >> (lcRows / 2)) | led1[colnum]);
  lc.setRow(0, colnum, (screen[colnum]) | led2[colnum]);
}

void buttonDelay(int bdelay) {
  if (Delays > bdelay) {
      Delays -= bdelay;
  }
  delay(bdelay);
}

void gameOver() {
  int cima[] = {
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111};
  int baixo[] = {
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111};
  for(int rownum = 8; rownum >= 0; rownum--) {
    lc.setRow(1,rownum,cima[rownum]);
    delay(100);
  }
  for(int rownum = 8; rownum >= 0; rownum--) {
    lc.setRow(0,rownum,cima[rownum]);
    delay(100);
  }
  delay(1800);
}

void loop() {
  if(start == 0){
    delay(300);
    if(digitalRead(button_start) == LOW) {    
      start = 1;
    }
  }else {
    lc.clearDisplay(0);
    lc.clearDisplay(1);
    memset(led1, 0, sizeof(led1));
    memset(led2, 0, sizeof(led2));
    memset(active, 0, sizeof(active));
    memset(screen, 0, sizeof(screen));
    tmpCol = 0;
    while(start == 1){
      if(allLines < 100){
        level = 0;
      }else if(allLines < 200) {
        level = 1;
      }else if(allLines < 300) {
        level = 2;
      }else if(allLines < 400) {
        level = 3;
      }else if(allLines < 500) {
        level = 4;
      }else {
        level = 5;
      }
      defualt_delay = (5 - level) * 100;
      Delays = defualt_delay;
      if(shape == 0) {
        shape = random(1,8);
      }else {
        shape = shape_Next;
      }
      shape_Next = random(1,8);
      angle = 0;
      switch(shape) {
        case 1: // O
          active[3] = 131072 + 65536;
          active[4] = 131072 + 65536;
          fromLeft = 3;
          fromRight = 3;
          break;
        case 2: // L
          active[3] = 262144 + 131072 + 65536;
          active[4] = 65536;
          fromLeft = 3;
          fromRight = 3;
          break;
        case 3: // J
          active[3] = 65536;
          active[4] = 262144 + 131072 + 65536;
          fromLeft = 3;
          fromRight = 3;
          break;
        case 4: // T
          active[2] = 131072;
          active[3] = 131072 + 65536;
          active[4] = 131072;
          fromLeft = 2;
          fromRight = 3;
          break;
        case 5: // I
          active[3] = 524288 + 262144 + 131072 + 65536;
          fromLeft = 3;
          fromRight = 4;
          break;
        case 6: // Z
          active[2] = 131072;
          active[3] = 131072 + 65536;
          active[4] = 65536;
          fromLeft = 2;
          fromRight = 3;
          break;
        case 7: // S
          active[2] = 65536;
          active[3] = 131072 + 65536;
          active[4] = 131072;
          fromLeft = 2;
          fromRight = 3;
          break;
      }
      for(int krok = 0; krok < lcRows + 1; krok++) {
        colCheck = 0;
        for(int i = 0; i < (lcCols / 2); i++) {
          if((digitalRead(button_left) == LOW) && (fromLeft > 0)) {
            move = 0;
            for(int colnum = fromLeft; colnum < (lcCols - fromRight); colnum++) {
              if((active[colnum] & screen[colnum - 1]) == 0) {
                move++;
              }
            }
            if(move == (lcCols - fromLeft - fromRight)) {
              for(int colnum = (fromLeft - 1); colnum < (lcCols - fromRight); colnum++) {
                if(colnum < (lcCols - 1)) {
                  active[colnum] = active[colnum+1];
                } else {
                  active[colnum] = 0;
                }
                updateColumn(colnum);
              }
              fromLeft--;
              fromRight++;
              buttonDelay(200);
            }
          }
        }
        for(int i = 0; i < (lcCols / 2); i++) {
          if((digitalRead(button_right) == LOW) && (fromRight > 0)) {
            move = 0;
            for(int colnum = fromLeft; colnum < (lcCols - fromRight); colnum++) {
              if((active[colnum] & screen[colnum + 1]) == 0) {
                move++;
              }
            }
            if(move == (lcCols - fromLeft - fromRight)) {
              for(int colnum = (lcCols - fromRight); colnum > (fromLeft - 1); colnum--) {
                if(colnum > 0) {
                  active[colnum] = active[colnum-1];
                } else {
                  active[colnum] = 0;
                }
                updateColumn(colnum);
              }
              fromLeft++;
              fromRight--;
              buttonDelay(200);
            }
          }
        }
        if(digitalRead(button_down) == LOW) {
          Delays = 75;
        } else {
          Delays = defualt_delay;
        }
        for(int i = 0; i < (lcCols / 2); i++) {
          if(digitalRead(button_rotate) == LOW) {
            switch(shape) {
              case 1: //O
                break;
              case 2: //L
                switch(angle) {
                  case 0:
                    if((fromLeft > 0) && (((active[fromLeft + 1] | (active[fromLeft + 1] << 1)) & screen[fromLeft - 1]) == 0)) {
                      active[fromLeft - 1] = (active[fromLeft + 1] | (active[fromLeft + 1] << 1));
                      updateColumn(fromLeft - 1);
                      active[fromLeft] = (active[fromLeft + 1] << 1);
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = (active[fromLeft + 1] << 1);
                      updateColumn(fromLeft + 1);
                      fromLeft--;
                      angle = 1;
                    }
                    break;
 
                  case 1:
                    if((((active[fromLeft + 2] << 1) & screen[fromLeft]) == 0) && ((((active[fromLeft + 1] << 1) | (active[fromLeft + 1] >> 1)) & screen[fromLeft + 1]) == 0)) {
                      active[fromLeft] = (active[fromLeft + 2] << 1);
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = active[fromLeft + 1] | (active[fromLeft + 1] << 1) | (active[fromLeft + 1] >> 1);
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = 0;
                      updateColumn(fromLeft + 2);
                      fromRight++;
                      angle = 2;
                    }
                    break;
 
                  case 2:
                    if((fromRight > 0) && (((active[fromLeft] >> 1) & screen[fromLeft]) == 0) && ((((active[fromLeft + 1] << 1) & active[fromLeft + 1]) & screen[fromLeft + 1]) == 0)) {
                      active[fromLeft] = (active[fromLeft] >> 1);
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = active[fromLeft];
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = ((active[fromLeft + 1] << 1) | active[fromLeft + 1]);
                      updateColumn(fromLeft + 2);
                      fromRight--;
                      krok--;
                      angle = 3;
                    }
                    break;
                  case 3:
                    if(((((active[fromLeft] << 1) | (active[fromLeft] >> 1)) & screen[fromLeft + 1]) == 0) && (((active[fromLeft] >> 1) & screen[fromLeft + 2]) == 0) && (krok < lcRows)) {
                      active[fromLeft] = 0;
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = (active[fromLeft + 2] | (active[fromLeft + 2] >> 1));
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = ((active[fromLeft + 2] >> 1) & (active[fromLeft + 2] >> 2));
                      updateColumn(fromLeft + 2);
                      fromLeft++;
                      krok++;
                      angle = 0;
                    }
                    break;
                }
                break;
              case 3: // J
                switch(angle) {
                  case 0:
                    if((fromRight > 0) && ((((active[fromLeft] << 2) | (active[fromLeft] << 1)) & screen[fromLeft]) == 0) && (((active[fromLeft] << 1) & screen[fromLeft + 2]) == 0)) {
                      active[fromLeft] = ((active[fromLeft] << 2) | (active[fromLeft] << 1));
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = ((active[fromLeft + 1] << 1) & (active[fromLeft + 1] >> 1));
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = active[fromLeft + 1];
                      updateColumn(fromLeft + 2);
                      fromRight--;
                      krok--;
                      angle = 1;
                    }
                    break;
                  case 1:
                    if((krok < lcRows) && ((((active[fromLeft + 1] << 1) | (active[fromLeft + 1] >> 1)) & screen[fromLeft + 1]) == 0) && (((active[fromLeft + 2] << 1) & screen[fromLeft + 2]) == 0)) {
                        active[fromLeft] = 0;
                        updateColumn(fromLeft);
                        active[fromLeft + 1] = (active[fromLeft + 1] | (active[fromLeft + 1] << 1) | (active[fromLeft + 1] >> 1));
                        updateColumn(fromLeft + 1);
                        active[fromLeft + 2] = (active[fromLeft + 2] << 1);
                        updateColumn(fromLeft + 2);
                        fromLeft++;
                        krok++;
                        angle = 2;
                    }
                    break;
                  case 2:
                    if((fromLeft > 0) && (((active[fromLeft + 1] >> 1) & screen[fromLeft - 1]) == 0) && ((((active[fromLeft + 1] >> 1) | (active[fromLeft + 1] >> 2)) & screen[fromLeft + 1]) == 0)) {
                      active[fromLeft - 1] = (active[fromLeft + 1] >> 1);
                      updateColumn(fromLeft - 1);
                      active[fromLeft] = active[fromLeft - 1];
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = (active[fromLeft] | (active[fromLeft + 1] >> 2));
                      updateColumn(fromLeft + 1);
                      fromLeft--;
                      angle = 3;
                    }
                    break;
                  case 3:
                    if((((active[fromLeft] >> 1) & screen[fromLeft]) == 0) && ((((active[fromLeft] << 1) | (active[fromLeft >> 1])) & screen[fromLeft + 1]) == 0)) {
                      active[fromLeft] = (active[fromLeft] >> 1);
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = ((active[fromLeft + 1] << 1) | active[fromLeft + 2]);
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = 0;
                      updateColumn(fromLeft + 2);
                      fromRight++;
                      angle = 0;
                    }
                    break;
                }
                break;
              case 4: // T
                switch(angle) {
                  case 0:
                    if(((active[fromLeft + 1] << 1) & screen[fromLeft + 1]) == 0) {
                      active[fromLeft + 1] = active[fromLeft + 1] | (active[fromLeft + 1] << 1);
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = 0;
                      updateColumn(fromLeft + 2);
                      fromRight++;
                      angle = 1;
                    }
                    break;
                  case 1:
                    if((fromRight > 0) && ((active[fromLeft] & screen[fromLeft + 2])== 0)) {
                      active[fromLeft + 1] = active[fromLeft + 1] & (active[fromLeft + 1] << 1);
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = active[fromLeft];
                      updateColumn(fromLeft + 2);
                      fromRight--;
                      krok--;
                      angle = 2;
                    }
                    break;
                  case 2:
                    if((((active[fromLeft + 1] >> 1) & screen[fromLeft + 1]) == 0) && (krok < lcRows)) {
                      active[fromLeft] = 0;
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = active[fromLeft + 1] | (active[fromLeft + 1] >> 1);
                      updateColumn(fromLeft + 1);
                      fromLeft++;
                      krok++;
                      angle = 3;
                    }
                    break;
                  case 3:
                    if((fromLeft > 0) && ((active[fromLeft + 1] & screen[fromLeft - 1]) == 0)) {
                      active[fromLeft - 1] = active[fromLeft + 1];
                      updateColumn(fromLeft - 1);
                      active[fromLeft] = active[fromLeft] & (active[fromLeft] >> 1);
                      updateColumn(fromLeft);
                      fromLeft--;
                      angle = 0;
                    }
                    break;
                }
                break;
              case 5: // I
                switch(angle) {
                  case 0:
                    if((fromLeft > 0) && (fromRight > 1) && ((((((active[fromLeft] >> 1) & (active[fromLeft] << 2)) & screen[fromLeft - 1]) & screen[fromLeft + 1]) & screen[fromLeft + 2]) == 0)) {
                      active[fromLeft - 1] = ((active[fromLeft] >> 1) & (active[fromLeft] << 2));
                      updateColumn(fromLeft - 1);
                      active[fromLeft] = active[fromLeft - 1];
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = active[fromLeft];
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = active[fromLeft];
                      updateColumn(fromLeft + 2);
                      fromLeft--;
                      fromRight -= 2;
                      krok -= 2;
                      angle = 1;
                    }
                    break;
                  case 1:
                    if((krok < (lcRows - 1)) && (((active[fromLeft] << 1) | (active[fromLeft] >> 1) | (active[fromLeft] >> 2)) & screen[fromLeft + 2]) == 0) {
                      active[fromLeft] = 0;
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = 0;
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = (active[fromLeft + 2] | (active[fromLeft + 2] << 1) | (active[fromLeft + 2] >> 1) | (active[fromLeft + 2] >> 2));
                      updateColumn(fromLeft + 2);
                      active[fromLeft + 3] = 0;
                      updateColumn(fromLeft + 3);
                      fromLeft += 2;
                      fromRight++;
                      krok += 2;
                      angle = 2;
                    }
                    break;
                  case 2:
                    if((fromLeft > 1) && (fromRight > 0) && ((((((active[fromLeft] << 1) & (active[fromLeft] >> 2)) & screen[fromLeft - 2]) & screen[fromLeft - 1]) & screen[fromLeft + 1]) == 0)) {
                      active[fromLeft - 2] = ((active[fromLeft] << 1) & (active[fromLeft] >> 2));
                      updateColumn(fromLeft - 2);
                      active[fromLeft - 1] = active[fromLeft - 2];
                      updateColumn(fromLeft - 1);
                      active[fromLeft] = active[fromLeft - 1];
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = active[fromLeft];
                      updateColumn(fromLeft + 1);
                      fromLeft -= 2;
                      fromRight--;
                      krok--;
                      angle = 3;
                    }
                    break;
                  case 3:
                    if((krok < (lcRows)) && (((active[fromLeft] >> 1) | (active[fromLeft] << 1) | (active[fromLeft] << 2)) & screen[fromLeft + 1]) == 0) {
                      active[fromLeft] = 0;
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = (active[fromLeft + 1] | (active[fromLeft + 1] >> 1) | (active[fromLeft + 1] << 1) | (active[fromLeft + 1] << 2));
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = 0;
                      updateColumn(fromLeft + 2);
                      active[fromLeft + 3] = 0;
                      updateColumn(fromLeft + 3);
                      fromLeft++;
                      fromRight += 2;
                      krok++;
                      angle = 0;
                    }
                    break;
                }
                break;
              case 6: // Z
                switch(angle) {
                  case 0:
                    if(((active[fromLeft + 1] & screen[fromLeft]) == 0) && (((active[fromLeft + 1] << 1) & screen[fromLeft + 1]) == 0)) {
                      active[fromLeft] = active[fromLeft + 1];
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = (active[fromLeft + 1] << 1);
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = 0;
                      updateColumn(fromLeft + 2);
                      fromRight++;
                      angle = 1;
                    }
                    break;
                  case 1:
                    if((fromRight > 0) && ((((active[fromLeft] << 2) & (active[fromLeft] << 1)) & screen[fromLeft]) == 0) && (((active[fromLeft] & active[fromLeft + 1]) & screen[fromLeft + 2]) == 0)) {
                      active[fromLeft] = ((active[fromLeft] << 2) & (active[fromLeft] << 1));
                      updateColumn(fromLeft);
                      active[fromLeft + 2] = (active[fromLeft] >> 1);
                      updateColumn(fromLeft + 2);
                      fromRight--;
                      krok--;
                      angle = 2;
                    }
                    break;
                  case 2:
                    if((krok < lcRows) && (((active[fromLeft + 1] >> 1) & screen[fromLeft + 1]) == 0) && (((active[fromLeft + 2] << 1) & screen[fromLeft + 2]) == 0)) {
                      active[fromLeft] = 0;
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = (active[fromLeft + 1] >> 1);
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = (active[fromLeft + 2] | (active[fromLeft + 2] << 1));
                      updateColumn(fromLeft + 2);
                      fromLeft++;
                      krok++;
                      angle = 3;
                    }
                    break;
                  case 3:
                    if((fromLeft > 0) && (((active[fromLeft] & active[fromLeft + 1]) & screen[fromLeft - 1]) == 0) && (((active[fromLeft + 1] >> 1) & screen[fromLeft + 1]) == 0)) {
                      active[fromLeft - 1] = (active[fromLeft] & active[fromLeft + 1]);
                      updateColumn(fromLeft - 1);
                      active[fromLeft + 1] = (active[fromLeft - 1] >> 1);
                      updateColumn(fromLeft + 1);
                      fromLeft--;
                      angle = 0;
                    }
                    break;
                }
                break;
              case 7: // S
                switch(angle) {
                  case 0:
                    if(((active[fromLeft + 1] << 1) & screen[fromLeft]) == 0) {
                      active[fromLeft] = (active[fromLeft + 1] << 1);
                      updateColumn(fromLeft);
                      active[fromLeft + 2] = 0;
                      updateColumn(fromLeft + 2);
                      fromRight++;
                      angle = 1;
                    }
                    break;
                  case 1:
                    if((fromRight > 0) && (((active[fromLeft + 1] << 1) & screen[fromLeft + 1]) == 0) && (((active[fromLeft] & (active[fromLeft] << 1)) & screen[fromLeft + 2]) == 0)) {
                      active[fromLeft] = (active[fromLeft] & active[fromLeft + 1]);
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = (active[fromLeft + 1] << 1);
                      updateColumn(fromLeft + 1);
                      active[fromLeft + 2] = (active[fromLeft] << 1);
                      updateColumn(fromLeft + 2);
                      fromRight--;
                      krok--;
                      angle = 2;
                    }
                    break;
                  case 2:
                    if((krok < lcRows) && (((active[fromLeft + 1] >> 1) & screen[fromLeft + 2]) == 0)) {
                      active[fromLeft] = 0;
                      updateColumn(fromLeft);
                      active[fromLeft + 2] = (active[fromLeft + 1] >> 1);
                      updateColumn(fromLeft + 2);
                      fromLeft++;
                      krok++;
                      angle = 3;
                    }
                    break;
                  case 3:
                    if((fromLeft > 0) && ((active[fromLeft + 1] & ((active[fromLeft + 1] >> 1)) & screen[fromLeft - 1]) == 0) && ((active[fromLeft + 1] & screen[fromLeft]) == 0)) {
                      active[fromLeft - 1] = (active[fromLeft + 1] & (active[fromLeft + 1] >> 1));
                      updateColumn(fromLeft - 1);
                      active[fromLeft] = active[fromLeft + 1];
                      updateColumn(fromLeft);
                      active[fromLeft + 1] = (active[fromLeft - 1] << 1);
                      updateColumn(fromLeft + 1);
                      fromLeft--;
                      angle = 0;
                    }
                    break;
                }
                break;
            }
            buttonDelay(200);
          }
        }
        if(digitalRead(button_start) == LOW) {
          memset(led1, 0, sizeof(led1));
          memset(led2, 0, sizeof(led2));
          memset(active, 0, sizeof(active));
          memset(screen, 0, sizeof(screen));
          score = 0;
          allLines = 0;
          shape = 0;
          break;
        }
        for(int colnum = 0; colnum < lcCols; colnum++) {
          if((screen[colnum] & (active[colnum] >> 1)) == 0) {
            colCheck++;
          }else {
            colCheck = 0;
            if(krok == 0){
              start = 0;
            }
          }
        }
        if((colCheck == lcCols) && (krok < lcRows) ){
          for(int colnum = 0; colnum < lcCols; colnum++) {
            active[colnum] = active[colnum] >> 1;
            updateColumn(colnum);
          }
        }else {
          break;
        }
        delay(Delays);
         }
 
        for(int colnum = 0; colnum < lcCols; colnum++) {
          screen[colnum] = screen[colnum] | (led1[colnum] << (lcRows / 2));
          screen[colnum] = screen[colnum] | led2[colnum];
          led1[colnum] = 0;
          led2[colnum] = 0;
          active[colnum] = 0;
        }
        currLines = 0;
        for(int rownum = 0; rownum < lcRows; rownum++) {
          colCheck = 0;
          for(int colnum = 0; colnum < lcCols; colnum++) {
            if(((screen[colnum] >> rownum) & 1) == 1) {
              colCheck++;
            }
          }
          if(colCheck == lcCols) {
            for(int colnum = 0; colnum < lcCols; colnum++) {
              tmpCol = ~((int) round(pow(2, rownum)));
              screen[colnum] = screen[colnum] & tmpCol;
              updateColumn(colnum);
              delay(30);
              tmpCol = (int) (round(pow(2, rownum)) - 1);
              tmpCol = screen[colnum] & tmpCol;
              screen[colnum] = (screen[colnum] >> (rownum + 1));
              screen[colnum] = (screen[colnum] << rownum);
              screen[colnum] = screen[colnum] | tmpCol;
            }
            for(int colnum = 0; colnum < lcCols; colnum++) {
              updateColumn(colnum);
            }
            rownum--;
            currLines++;
            allLines++;
          }
        }
        if(currLines > 0) {
          score += (int) round(pow(4, currLines-1));
        }
  }
  gameOver();
  }
}