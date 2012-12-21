#include "project.h"

/**
Copyright (C) 2012 John B. Wyatt IV
License: GPLv3 by the Free Software Foundation

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
Main function, not much to it.
The majority of the program is accessed through checkInput
*/
int main(int argc, char * argv[])
{
  enum ERRORCODES temp;
  if( (temp = checkInput(argc, argv)) != 0)
  {
    return temp;
  } else {
    return 0;
  }
}

/**
  A test driver, use with --driver flag.
*/
void thedriver() {
  BYTE sometest[4][4] = { {0xd4, 0xbf, 0x5d, 0x30}, 
                          {0xe0, 0xb4, 0x52, 0xae},
                          {0xb8, 0x41, 0x11, 0xf1},
                          {0x1e, 0x27, 0x98, 0xe5}};
  //sometest[0][0] = 0x0;
  //mixColumns(sometest);
  //mixColBytes(sometest[0]);
  //BYTE temp = 0x80;
  //multiby2(temp);
  //printOutByte(temp);
  printf("\n");
  printOutWord(sometest[0]);
  //printOutWord(sometest[1]);
  //printOutWord(sometest[2]);
  //printOutWord(sometest[3]);
}

/**
  Checks the input at the beginning of the program. Controls the program flow based on which flags have been set.
  returns an errorcode
*/
int checkInput(int argc, char * argv[])
{
  // encrypts only
  char * option1 = "-e";
  // decrypts only
  char * option2 = "-d";
  // encrypts, decryption and compares
  char * option3 = "-c";
  // use built in tests
  char * option1test = "-t1";
  char * option2test = "-t2";
  char * optiondriver = "-driver";
  BYTE finalmsg[16];

  // Built in test - using example string and key by the standard
  if(argc == 2 && (strcmp(option1test, argv[1])==0)) {
    printf("Using builtin test and Encryption\n");
    aesencrypt(test1msg, test1key, finalmsg);
    int i;
    for(i = 0; i < 16; i++) {
      printf("%02x",finalmsg[i]);
      if(i % 4 == 3) {
        printf("\n");
      }
    }
    return 0;
  } // Second builtin test
  else if(argc == 2 && (strcmp(option2test, argv[1])==0)) {
    printf("Using builtin test and Encryption\n");
    aesencrypt(test2msg, test2key, finalmsg);
    int i;
    for(i = 0; i < 16; i++) {
      printf("%02x",finalmsg[i]);
      if(i % 4 == 3) {
        printf("\n");
      }
    }
    return 0;
  } // option driver
  else if(argc == 2 && (strcmp(optiondriver, argv[1])==0)) {
    thedriver();
    return 0;
  } // option 1
  else if(argc == 4 && (strcmp(option1, argv[1]) == 0)){
    printf("Encrypting Message\n");
    return 0;
  } // option 2
  else if(argc == 4 && (strcmp(option2, argv[1])==0)) {
    printf("Decrypting Message\n");
    return 0;
  } // option 3
  else if(argc == 4 && (strcmp(option3, argv[1])==0)) {
    printf("Encrypting Message\n");
    printf("Decrypting Message\n");
    return 0;
  } // if no other options
  else if(argc != 4)
  {
    printf("\nYou need to input a message and a key in this format:\n jwyattaes -e message key\nor for the default test\n jwyattaes -t\n");
    return E_BADINPUT;
  }
  else {
    return 0;
  }
}

/**
  Main aes encryption function
  msg is the plaintextmsg to be passed
  key is the key to be passed in
  cipherMsg is the encrypted message returned

  returns int, unimplemented error code
*/
int aesencrypt(BYTE msg[16], BYTE key[16], BYTE cipherMsg[16]) {

  BYTE expKey[44][4];
  keyExpansion(key, expKey);

  int r;
  BYTE round[4][4];
  // copy the msg into the round
  round[0][0] = msg[0];
  round[0][1] = msg[1];
  round[0][2] = msg[2];
  round[0][3] = msg[3];
  round[1][0] = msg[4];
  round[1][1] = msg[5];
  round[1][2] = msg[6];
  round[1][3] = msg[7];
  round[2][0] = msg[8];
  round[2][1] = msg[9];
  round[2][2] = msg[10];
  round[2][3] = msg[11];
  round[3][0] = msg[12];
  round[3][1] = msg[13];
  round[3][2] = msg[14];
  round[3][3] = msg[15];

  // XOR the round and the first 16 bytes of the exp key
  xorWord(round[0], expKey[0]);
  xorWord(round[1], expKey[1]);
  xorWord(round[2], expKey[2]);
  xorWord(round[3], expKey[3]);

  for(r = 0; r < 10; r++) { // the round message
    int i;
    for(i = 0; i < 4; i++) {
      subWord(round[i]);
      
    }
    shiftRows(round);

    if(r != 9) { // for the 10th round
      mixColumns(round);
    }

    // add round key step
    xorWord(round[0], expKey[(r+1)*4]);
    xorWord(round[1], expKey[(r+1)*4 + 1]);
    xorWord(round[2], expKey[(r+1)*4 + 2]);
    xorWord(round[3], expKey[(r+1)*4 + 3]);
    
  }
  cipherMsg[0] = round[0][0];
  cipherMsg[1] = round[0][1];
  cipherMsg[2] = round[0][2];
  cipherMsg[3] = round[0][3];
  cipherMsg[4] = round[1][0];
  cipherMsg[5] = round[1][1];
  cipherMsg[6] = round[1][2];
  cipherMsg[7] = round[1][3];
  cipherMsg[8] = round[2][0];
  cipherMsg[9] = round[2][1];
  cipherMsg[10] = round[2][2];
  cipherMsg[11] = round[2][3];
  cipherMsg[12] = round[3][0];
  cipherMsg[13] = round[3][1];
  cipherMsg[14] = round[3][2];
  cipherMsg[15] = round[3][3];

  return 0;
}

/**
  Key expansion function
  key is the key passed in by aesencrypt
  w is the expanded key returned
  returns 0, can be expanded into an errorcode
*/
int keyExpansion(BYTE key[16], BYTE w[44][4])
{
  int round = 1;

  w[0][0] = key[0];
  w[0][1] = key[1];
  w[0][2] = key[2];
  w[0][3] = key[3];
  w[1][0] = key[4];
  w[1][1] = key[5];
  w[1][2] = key[6];
  w[1][3] = key[7];
  w[2][0] = key[8];
  w[2][1] = key[9];
  w[2][2] = key[10];
  w[2][3] = key[11];
  w[3][0] = key[12];
  w[3][1] = key[13];
  w[3][2] = key[14];
  w[3][3] = key[15];

  int i;
  BYTE temp[4];

  for( i = 4; i < 44; i++) {

    temp[0] = w[i-1][0];
    temp[1] = w[i-1][1];
    temp[2] = w[i-1][2];
    temp[3] = w[i-1][3];

    if(i % 4 != 0) {
      // When the i is not a mutiple of 4
      xorWord(temp, w[i - 4]);

      w[i][0] = temp[0];
      w[i][1] = temp[1];
      w[i][2] = temp[2];
      w[i][3] = temp[3];
    } else {
      // When it is a mutiple of 4

      shiftWord(temp);

      subWord(temp);
      rconFun(temp, rCon[round]);
      round++;
      xorWord(temp, w[i-4]);
      w[i][0] = temp[0];
      w[i][1] = temp[1];
      w[i][2] = temp[2];
      w[i][3] = temp[3];
    }
  }

  //printOutKey(w);
  return 0;
}

/**
Shifts a word by one byte to the left (the largest value is moved to become the smallest)
  Returns value as x
*/
int shiftWord(BYTE x[4]) {
  BYTE temp = x[0];
  x[0] = x[1];
  x[1] = x[2];
  x[2] = x[3];
  x[3] = temp;
  return 0;
}

/**
  Translates a single byte based on the S-box lookup table
    Returns the byte
*/
BYTE subByte(BYTE x) {
  //BYTE lower = 0;
  //BYTE upper = 0;
  // the lower 4 bits, y
  //lower = x % 16;
  // the upper 4 bits, x
  //upper = x / 16; // notice we assume it rounds down here

  //return sbox[upper * 16 + lower]; // actually... maybe we could have just put it as x here
  return sbox[x];
}

/**
  SubWord does subByte times 4
  Returns value as x
*/
int subWord(BYTE x[4]) {
  //printf("%x", x[0]);
  x[0] = subByte(x[0]);
  //printf("%x\n", x[0]);
  x[1] = subByte(x[1]);
  x[2] = subByte(x[2]);
  x[3] = subByte(x[3]);
  return 0;
}

/**
  Takes a word and xor's it
  Returns value as x
    Note: does not track mutiplication
*/
int rconFun(BYTE x[4], int rcon) {
  x[0] = x[0] ^ rcon;
  return 0;
}

/**
 XOR's x and y
  Returns value as x
*/
int xorWord(BYTE x[4], BYTE y[4]) {
  x[0] = x[0] ^ y[0];
  x[1] = x[1] ^ y[1];
  x[2] = x[2] ^ y[2];
  x[3] = x[3] ^ y[3];
  return 0;
}

/**
  Implements the shiftrows step.
  Returns the output as x
*/
int shiftRows(BYTE x[4][4]) {
  BYTE temp = x[0][1];
  x[0][1] = x[1][1];
  x[1][1] = x[2][1];
  x[2][1] = x[3][1];
  x[3][1] = temp;

  temp = x[0][2];
  x[0][2] = x[1][2];
  x[1][2] = x[2][2];
  x[2][2] = x[3][2];
  x[3][2] = temp;

  temp = x[0][2];
  x[0][2] = x[1][2];
  x[1][2] = x[2][2];
  x[2][2] = x[3][2];
  x[3][2] = temp;

  temp = x[0][3];
  x[0][3] = x[1][3];
  x[1][3] = x[2][3];
  x[2][3] = x[3][3];
  x[3][3] = temp;

  temp = x[0][3];
  x[0][3] = x[1][3];
  x[1][3] = x[2][3];
  x[2][3] = x[3][3];
  x[3][3] = temp;

  temp = x[0][3];
  x[0][3] = x[1][3];
  x[1][3] = x[2][3];
  x[2][3] = x[3][3];
  x[3][3] = temp;

  return 0;
}

/**
  Implements the mixColumns step
  returns the output as x
*/
int mixColumns(BYTE x[4][4]) {
  mixColBytes(x[0]);
  mixColBytes(x[1]);
  mixColBytes(x[2]);
  mixColBytes(x[3]);
  return 0;
}

int mixColBytes(BYTE x[4]) {
  BYTE temp[4];
  temp[0] = x[0];
  temp[1] = x[1];
  temp[2] = x[2];
  temp[3] = x[3];

  //x[0] = 2 * temp[0] ^ 3 ^ temp[1] ^ temp[2] ^ temp[3];  
  x[0] = multiby2(temp[0]) ^ multiby3(temp[1]) ^ temp[2] ^ temp[3];
  //x[1] = temp[0] ^ 2 * temp[1] ^ 3* temp[2] ^ temp[3];
  x[1] = temp[0] ^ multiby2(temp[1]) ^ multiby3(temp[2]) ^ temp[3];
  //x[2] = temp[0] ^ temp[1] ^ 2 * temp[2] ^ 3 * temp[3];
  x[2] = temp[0] ^ temp[1] ^ multiby2(temp[2]) ^ multiby3(temp[3]);
  //x[3] = 3 * temp[0] ^ temp[1] ^ temp[2] ^ 2 * temp[3];
  x[3] = multiby3(temp[0]) ^ temp[1] ^ temp[2] ^ multiby2(temp[3]);
  return 0;
}

/*
  handles the multiplication by two
  in mixcolumns by shifting the byte left
    returns the output as a BYTE
*/
BYTE multiby2(BYTE x) {
  BYTE temp = x;

  // we check, the topmost bit 
  // if 1  
  if( 0b10000000 == (temp & 0b10000000) ) {
    temp = temp << 1;
    //printf("equal to 1 in top most bit\n");
    temp = temp ^ 0x1b;
  } else {
    temp = temp << 1;
  }
  return temp;
}

/*
  handles the multiplcation by three
  in mixcolumns by calling mulby2 and XORing itself
    returns the output as a BYTE
*/
BYTE multiby3(BYTE x) {
  BYTE temp = multiby2(x);
  temp = temp ^ x;
  return temp;
}

/**
  Prints out the key, a newline for each word
*/
void printOutKey(BYTE w[44][4]) {
  int i;
  for(i=0;i<44;i++) {
    printf("Round %2d: ", i);
    printOutWord(w[i]);
  }
}

/**
  Special debug message, never used.
  Set with #define DEBUG 1
*/
void printmsg(char * msg) {
  #ifdef DEBUG
  printf("%s\n", msg);
  #endif
  return;
}

/**
  Print out byte
*/
void printOutByte(BYTE x) {
  printf("%02x",x);
}

/**
  Print out word
*/
void printOutWord(BYTE w[4]) {
  printf("%02x",w[0]);
  printf("%02x",w[1]);
  printf("%02x",w[2]);
  printf("%02x\n",w[3]);
}

/**
  Print out msg
    convience function for printing out 16byte msgs
*/
void printOutMsg(BYTE w[16]) {
  int i;
  for(i = 0; i < 16; i++) {
    printf("%02x",w[i]);
  }
  printf("\n");
}
