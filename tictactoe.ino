//ani3 AI Controlled TicTacToe
// Uses a minimax algorithm to come up with the best moves for the AI to make and is modified for TTT
//https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-1-introduction/ 
//https://runtimeprojects.com/2017/04/artificial-intelligence-on-arduino-an-invincible-tic-tac-toe-player/



#define A 12
#define B 11
#define C 10
#define D 9
#define E 8
#define left A0
#define right A2
#define select A1
boolean test = false;

#define PIN_COUNT 5
#define PIN_CONFIG 0
#define PIN_STATE 1


#define LED_COUNT 20


int matrix[LED_COUNT][2][PIN_COUNT] = {
    /*
     *  Each row in this matrix respresents the pin modes and pin states for a single LED
     */
    //              PIN_CONFIG                          PIN_STATE
    //    A       B       C      D      E         A     B    C    D    E
    { { OUTPUT, OUTPUT, INPUT, INPUT, INPUT }, { HIGH, LOW, LOW, LOW, LOW } }, // AB 0
    { { OUTPUT, OUTPUT, INPUT, INPUT, INPUT }, { LOW, HIGH, LOW, LOW, LOW } }, // BA 1
    { { OUTPUT, INPUT, OUTPUT, INPUT, INPUT }, { HIGH, LOW, LOW, LOW, LOW } }, // AC 2
    { { OUTPUT, INPUT, OUTPUT, INPUT, INPUT }, { LOW, LOW, HIGH, LOW, LOW } }, // CA 3
    { { OUTPUT, INPUT, INPUT, OUTPUT, INPUT }, { HIGH, LOW, LOW, LOW, LOW } },  // AD 4
    { { OUTPUT, INPUT, INPUT, OUTPUT, INPUT }, { LOW, LOW, LOW, HIGH, LOW } },  // DA 5
    { { OUTPUT, INPUT, INPUT, INPUT, OUTPUT }, { HIGH, LOW, LOW, LOW, LOW } },  // AE 6
    { { OUTPUT, INPUT, INPUT, INPUT, OUTPUT }, { LOW, LOW, LOW, LOW, HIGH } },  // EA 7
    { { INPUT, OUTPUT, OUTPUT, INPUT, INPUT }, { LOW, HIGH, LOW, LOW, LOW } }, // BC 8
    { { INPUT, OUTPUT, OUTPUT, INPUT, INPUT }, { LOW, LOW, HIGH, LOW, LOW } }, // CB 9
    { { INPUT, OUTPUT, INPUT, OUTPUT, INPUT }, { LOW, HIGH, LOW, LOW, LOW } },  // BD 10
    { { INPUT, OUTPUT, INPUT, OUTPUT, INPUT }, { LOW, LOW, LOW, HIGH, LOW } },  // DB 11
    { { INPUT, OUTPUT, INPUT, INPUT, OUTPUT }, { LOW, HIGH, LOW, LOW, LOW } },  // BE 12
    { { INPUT, OUTPUT, INPUT, INPUT, OUTPUT }, { LOW, LOW, LOW, LOW, HIGH } },  // EB 13
    { { INPUT, INPUT, OUTPUT, OUTPUT, INPUT }, { LOW, LOW, HIGH, LOW, LOW } }, // CD 14

    }
}



//checks the board if there is a win on the screen, which are the three across, three down, and 2 diagonals
int winCheck(int brd[9]){
  Serial.println("Win");
   int winBoards [8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
   for (int i = 0;  i < 8; i++){ //if the board matches a winning set, return the piece owned by player
     if ((brd[winBoards[i][0]] != 0) && (brd[winBoards[i][1]] == brd[winBoards[i][2]]) && (brd[winBoards[i][0]] == brd[winBoards[i][1]])) 
       return  brd[winBoards[i][0]];       
   }
       return 0; //no winning board
}



int mm(int board[9], int turn, int left){//minimax algorithm
  Serial.println("Beginning of minimax");
  int win = winCheck(board);
  if (win !=0)
    return win * turn;
  int action = -1; 
  int rating = -2; //score of the move
  for (int i = 0; i<9; i++){
      //checks if legal move
      if (board[i] ==0){
        board[i] = turn; //x or o
        int theoScore = 0; //theoretical score
        if (left < 8)
          theoScore = -1*mm(board,-1*turn,left+1);
        if (theoScore > rating){
          rating = theoScore;
          action = i;}
      }
  }
   if (action == -1)
     return 0 ;//invalid move
   return rating;
  }
//Function for Ai's turn  
void ai(int board[9]){
    int brd[9];
    for(int i = 0; i < 9; ++i){
      brd[i] = board[i];
    }
    int action = -5;
    int rating = -2;
Serial.println("AI");
    for (int i =0; i<9; i++){
      Serial.println("AI for loop");
      if (brd[i] ==0){
        brd[i] = 1;
        int theoScore = -mm(brd,-1,0);
        brd[i] = 0; //reset after calling
        if (theoScore > rating){
          rating = theoScore;
          action = i;      }
      }
    }
    board[action] = 1;}
//Gets input from player with option for bluetooth input
void human(int brd[9]){ //move based on button press
   boolean selected = false;
   int curSelection = 0;
   int state = -1;
   boolean buttonPressed = false;
   int prevButton = 0;

   do {
    while(brd[curSelection] != 0){
      if(prevButton == 0){
        curSelection++;
        if(curSelection > 8){
          curSelection = 0;
        }
      }
      else{
        curSelection--;
        if(curSelection < 0){
          curSelection = 8;
        }
      }
    }
    if(Serial.available() > 0){
    state = Serial.read();
    }
      if(digitalRead(right) == HIGH || state == '1'){
        if(!buttonPressed){
          curSelection++;
          buttonPressed = true;
          state = -1;
          prevButton = 0;
        }
      }
      if(digitalRead(left) == HIGH || state == '2'){
        if(!buttonPressed){
          curSelection--;
          buttonPressed = true;
          state = -1;
  prevButton = 1;
        }
      }
      if(digitalRead(select) == HIGH || state == '3'){
        if(!buttonPressed){
          selected = true;
          buttonPressed = true;
          state = -1;
        }
      }
      if(digitalRead(right) == LOW && digitalRead(left) == LOW && digitalRead(select) == LOW){
        buttonPressed = false;
      }
      if(curSelection > 8){
        curSelection = 0;
      }
      if(curSelection < 0){
        curSelection = 8;
      }
      displayBoard(brd, curSelection * 2, true);
   } while (selected == false);

   brd[curSelection] = -1;
   displayBoard(brd, -1, false);
   delay(1000);
}

int winner = 0;
int gameBoard[9] = {0,0,0,0,0,0,0,0,0};
void loop(){
  for (int i = 0; i < 9 ; i++){
    winner = winCheck(gameBoard);
    if(winner != 0){
      break;
    }
    if (i %2  == 0){//alternating turns
      ai(gameBoard);
      displayBoard(gameBoard, -1, false);
  }
    else {
      
      displayBoard(gameBoard, -1, true);
      human(gameBoard);
    }           
  }
  int start = 0;

  if(winner == 1){
    turnOn(19);
    if(digitalRead(select) == HIGH || Serial.read() == 3){
      winner = 0;
    for(int i = 0; i < 9; ++i){
        gameBoard[i] = 0;
      }
      delay(1000);
    }
  }else if(winner == -1){
    turnOn(18);
    if(digitalRead(select) == HIGH || Serial.read() == 3){
      winner = 0;
      for(int i = 0; i < 9; ++i){
        gameBoard[i] = 0;
      }
      delay(1000);
    }
  }

  
}
