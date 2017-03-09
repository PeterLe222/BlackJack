/*
 * Game BlackJack
 * Using: Arduino Uno, LCD display 16x2, 2 pressbuttons
 */

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define BUTTON_1 7  // pin 7 connects to button 1
#define BUTTON_2 8  // pin 8 connects to button 2

boolean buttonState1 = LOW; 
boolean buttonState2 = LOW;

int pressedButton1 = 0; // variable store button 1 pressed
int pressedButton2 = 0; // variable store button 2 pressed

// function debounce press button
boolean debounceButton(boolean state, int button);

// function reset the game
void reset();
// function create card deck
void cardDeck();
// function count the same card
int Check(int *p, int takeCard);  
// function calculate the sum of cards
int sumCard(int *p, int min, int max);
// function print card to lcd display
void printCard(int *p, int min, int max);
// function check if there is Ace on deck
int checkAce(int *p, int min, int max);
// function show player's card
void playerHand(int *p, int min, int max);
// function show computer's card and result
void checkGame(int *array, int min);

int array[10];  // array of deck
int i = 1;  // variable store array's index
int sumPlayer;// variable store the sum of card value on player's hand
int sumCom; // variable store the sum of card value on computer's hand

void setup() {
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  randomSeed(analogRead(0));  // create random seed
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setCursor(3, 0);
  lcd.print("BlackJack");
  delay(1000);  // delay 1 second
  lcd.clear();  // clear everything on display
  lcd.setCursor(0, 1);
  // print introduction
  lcd.print("P:PlayerD:Dealer");
  lcd.setCursor(0, 0);
  lcd.print("Game starts in ");
  // countdown in 5 seconds
  for (int i = 5; 0 < i; i--)
  {
    lcd.setCursor(15, 0);
    lcd.print(i);
    delay(1000);
  }
  lcd.clear();
}

void loop() {
  
  Serial.print("Btn1= ");Serial.println(pressedButton1);
  Serial.print("Btn2= ");Serial.println(pressedButton2);
  // count pressed on button 1
  if(debounceButton(buttonState1, BUTTON_1) == HIGH && buttonState1 == LOW)
  {
    pressedButton1++;
    buttonState1 = HIGH;
  }
  else if(debounceButton(buttonState1, BUTTON_1) == LOW && buttonState1 == HIGH)
  {   
    buttonState1 = LOW;
  }
  // count pressed on button 2
   if(debounceButton(buttonState2, BUTTON_2) == HIGH && buttonState2 == LOW)
  {
    pressedButton2++;
    buttonState2 = HIGH;
  }
  else if(debounceButton(buttonState2, BUTTON_2) == LOW && buttonState2 == HIGH)
  {   
    buttonState2 = LOW;
  }
  /* show first 2 cards */
  if (pressedButton1 == 0 && pressedButton2 == 0)
  {
    pressedButton1 = 1;
    cardDeck(); // draw cards
  }
  if (pressedButton1 == 1 && pressedButton2 == 0)
  {
    playerHand(array,0, pressedButton1 + 1);  // print player's card
    sumPlayer = sumCard(array,0,pressedButton1 + 1);  // calculate the point of player's card
  } 
  /* draw third card */
  if (pressedButton1 == 2  && pressedButton2 == 0)
  {
    playerHand(array,0, pressedButton1 + 1);
    sumPlayer = sumCard(array,0,pressedButton1 + 1);
    /* if player's card bigger than 21, show result */
    if (sumPlayer > 21)
    {
      lcd.setCursor(11,0);
      lcd.print("Busts");
      lcd.setCursor(0,1);
      lcd.print("Dealer Win");
      reset();
    }
  }
  /* draw forth card */
  if (pressedButton1 == 3)
  {
    playerHand(array,0, pressedButton1 + 1);
    sumPlayer = sumCard(array,0,pressedButton1 + 1);
    if (sumPlayer > 21)
    {
      lcd.setCursor(11,0);
      lcd.print("Busts");
      lcd.setCursor(0,1);
      lcd.print("Dealer Win");
      reset();
    }
  }
  /* draw fifth card and show result*/
  if (pressedButton1 == 4)
  {
    playerHand(array,0, pressedButton1 + 1);
    sumPlayer = sumCard(array,0,pressedButton1 + 1);
    if (sumPlayer > 21)
    {
      lcd.setCursor(11,0);
      lcd.print("Busts");
      lcd.setCursor(0,1);
      lcd.print("Dealer Win");
      reset();
    }
    pressedButton2 = 1;
  }
  /* press button 2 to see result */
  if (pressedButton2 == 1 && pressedButton1 == 1)
  {
    checkGame(array,pressedButton1);
  }
  if (pressedButton2 == 1 && pressedButton1 == 2)
  {
    checkGame(array,pressedButton1);
  }  
  if (pressedButton2 == 1 && pressedButton1 == 3)
  {
    checkGame(array,pressedButton1);
  }  
  if (pressedButton2 == 1 && pressedButton1 == 4)
  {
    checkGame(array,pressedButton1);
  }  
}

boolean debounceButton(boolean state, int button)
{
  boolean stateNow = digitalRead(button);
  if(state!=stateNow)
  {
    delay(10);
    stateNow = digitalRead(button);
  }
  return stateNow;
}
int checkAce(int *p, int min, int max){
  int ace = 0;  // variable store number of card ACE
  for (int i = min; i < max; i++) // run through the drawed cards
  {
    if (p[i] == 1)  // ACE value in array is 1
    {
      ace++;
    }
  }
  return ace;
}
void printCard(int *p, int min, int max){
    
    for (int i = min; i < max; i++) // run through the drawed cards
    {
      if (*(p+i) == 11) // print J when value in card deck is 11
      {
        lcd.print("J ");
      }
      else if (*(p+i) == 12)  // print Q when value in card deck is 12
      {
        lcd.print("Q ");
      }
      else if (*(p+i) == 13)  // print K when value in card deck is 13
      {
        lcd.print("K ");
      }
      else if (*(p+i) == 1) // print A when value in card deck is 1
      {
        lcd.print("A ");
      }
      else  // print normal cards
      {
        lcd.print(*(p+i));
        lcd.print(" ");
      }
    }
}

int Check(int *p, int takeCard){
  int checkCard = 0;  // variable store number of same cards
  // check and return the number of same card
  for (int i = 0; i < 10; i++)  // run through the card deck
  {
    // if there is same card with drawed card, increase the number of checkCard
    if (p[i] == takeCard) 
    {
      checkCard++;
    }
  }
  return checkCard;
}

int sumCard(int *p, int min, int max){
  int sum = 0;  // variable store total point of drawed cards
  for (int i = min; i < max; i++)
  {
    if (p[i] == 11) // plus 10 when card is J
    {
      sum += 10;
    }
    else if (p[i] == 12)  // plus 10 when card is Q
    {
      sum += 10;
    }
    else if (p[i] == 13)  // plus 10 when card is K
    {
      sum += 10;
    }
    else  // other cards add the same value in card deck
    {
      sum += p[i];
    }
  }
  return sum;
}
void playerHand(int *p, int min, int max){
      lcd.setCursor(0,0); // set position on display to print
      lcd.print("P:");  // print 'P' for player
      printCard(p, min, max); // print drawed cards
    }
void cardDeck(){
  for (int i = 0; i < 10; i++)
  {
    array[i] = random(1,14);  // take random number from 1 to 13
    int checkCard = Check(array, array[i]); // check same cards
    // if there are more than 4 same cards, draw again
    if (checkCard == 4)
    {
      i--;
    }
  }
}
void reset(){
  delay(5000);  // wait 5 seconds
  lcd.clear();  // clear everything on display
  i = 1;  // reset i value
  pressedButton1 = pressedButton2 = 0;  // reset 2 buttons
}
void checkGame(int *array, int min){
  /* +10 when there is an Ace for player 
   * when total point of drawed cards smaller than 12 
   */
  if (sumCard(array,0,min+1) < 12 && checkAce(array, 0, min+1)==1)
  {
    sumPlayer += 10;
  }
  lcd.setCursor(0,1);
  lcd.print("D:");  // print 'D' as Dealer
  printCard(array,min+1, min+2+i);
  sumCom = sumCard(array,min+1,min+2+i);
  /* +10 when there is an Ace for computer
   * when total point +10 equal or bigger than 17
   */
  if ((sumCom + 10) >= 17 && checkAce(array,min+1,min+2+i) == 1)
  {
    sumCom += 10;
    // check if point of player and computer are equal
    if (sumCom == sumPlayer)
    {
      lcd.setCursor(12,0);
      lcd.print("Draw");  // print Draw for Player
      lcd.setCursor(12,1);
      lcd.print("Draw");  // print Draw for Dealer
      reset();
    }
    // check point of player and computer
    else if (sumCom > sumPlayer)
    {
      lcd.setCursor(13,1);
      lcd.print("Win"); // print Win for Dealer
      reset();
    }
    // player win
    else
    {
      lcd.setCursor(13,0);
      lcd.print("Win"); // print Win for Player
      reset();
    }
  }
  else  // if there are more than an ACE or there is no ACE
  {
    // total point of drawed card smaller than 17, draw 1 more card
    if (sumCom < 17)  
    {
      i++;
    }
    // Dealer busts
    else if (sumCom > 21)
    {
      lcd.setCursor(13,0);
      lcd.print("Win"); // print Win for Player 
      reset();
    }
    // drawed cards point of Dealer bigger than 16 and smaller than 21
    else  
    {
      // Dealer and Player get the same point
      if (sumCom == sumPlayer)
      {
        lcd.setCursor(12,0);
        lcd.print("Draw");
        lcd.setCursor(12,1);
        lcd.print("Draw");
        reset();
      }
      // Dealer's point smaller than Player's
      else if (sumCom < sumPlayer)
      {
        lcd.setCursor(13,0);
        lcd.print("Win");
        reset();
      }
      // Dealer's point bigger than Player's 
      else
      {
        lcd.setCursor(13,1);
        lcd.print("Win");
        reset();
      }
    }
  }
}
