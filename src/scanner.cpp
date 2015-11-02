/* 
   nicole cranon <nicole.cranon@ucdenver.edu>
   csci 4640, fall 2015
   assignment 1 - micro scanner
 */

#include "scanner.h"
#include <cctype>
#include <iostream>

namespace scanner {

  std::ifstream file;

  // for use in printing the token type
  std::string token_type[]= {

    [BeginSym] = "BeginSym", 
    [EndSym] = "EndSym", 
    [ReadSym] = "ReadSym", 
    [WriteSym] = "WriteSym", 
    [Id] = "Id",
    [IntLiteral] = "IntLiteral",
    [LParen] = "LParen",
    [RParen] = "RParen",
    [SemiColon] = "SemiColon",
    [Comma] = "Comma",
    [AssignOp] = "AssignOp",
    [ExpOp] = "ExpOp",
    [PlusOp] = "PlusOp",
    [MinusOp] = "MinusOp",
    [EqualOp] = "EqualOp",
    [EofSym] = "EofSym"

  };

  char inspect () {

    char c;
    c = file.get();
    file.unget();

    return c;

  }

  void advance () {

    file.get();

  }

  char read () {

    char next_char = inspect();
    advance();

    return next_char;

  }

  token checkReserved (const std::string& checkString) {

    if (checkString.compare ("BEGIN") == 0) {

      return BeginSym;

    }

    else if (checkString.compare ("END") == 0) {

      return EndSym;

    }

    else if (checkString.compare ("READ") == 0) {

      return ReadSym;

    }

    else if (checkString.compare ("WRITE") == 0) {

      return WriteSym;

    }

    else {

      return Id;

    }

  }

  std::string tokenBuffer = ""; 
  token scan () {

    char currentChar;
    bool done;
    do {

      clearBuffer();

      // read current character
      currentChar = read();

      // determine token being read
      switch (currentChar) {

        // do nothing with whitespace 
        case ' ':
        case '\t':
        case '\n':
        case '\r':

          break;

          // identifier or reserved symbol
        case 'a' ... 'z':
        case 'A' ... 'Z':

          bufferChar(currentChar) ;

          done = false; 

          // while not done keep reading identifier or reserved symbol
          while (!done) {

            switch (inspect ()) {

              case 'a' ... 'z':
              case 'A' ... 'Z':
              case '0' ... '9':
              case '_':

                currentChar = inspect ();
                bufferChar(currentChar) ;
                advance ();
                break;

              default:

                done = true;
                //std::cout << tokenBuffer << ' ' ;
                 
                // check if an id or a reserved word
                return checkReserved (tokenBuffer);

            }

          }

          break;

          // Integer string
        case '0' ... '9':

          bufferChar(currentChar) ;

          done = false; 

          while (!done) {

            switch (inspect()) {

              case '0' ... '9':

                currentChar = inspect ();
                bufferChar(currentChar) ;
                advance ();

                break;

              default:

                done = true;
                 //std::cout << tokenBuffer << ' ';
                 
                return IntLiteral;

            }
          }

          break;

          // Left Paren
        case '(':

          bufferChar(currentChar) ;

           //std::cout << tokenBuffer << ' ' ;
           

          return LParen;

          break;

          // Right Paren
        case ')':

          bufferChar(currentChar) ;

           //std::cout << tokenBuffer << ' ' ;
           

          return RParen;

          break;

          // Semi Colon
        case ';':

          bufferChar(currentChar) ;

           //std::cout << tokenBuffer << ' ' ;
           

          return SemiColon;

          break;


          // Comma
        case ',':

          bufferChar(currentChar) ;

           //std::cout << tokenBuffer << ' ' ;
           

          return Comma;

          break;

          // Check for Assign Operator
        case ':':

          // Is AssignOp
          if (inspect () == '=') { 

            bufferChar(currentChar) ;

            currentChar = inspect ();
            bufferChar(currentChar) ;
            advance ();

             //std::cout << tokenBuffer << ' ' ;
             

            return AssignOp;

          }

          // Is lexical error
          else {

            std::cerr << "\nLexical Error\n";

          }

          break;

          // Check for muliplication or exponentiation operator 
        case '*':

          bufferChar(currentChar) ;

          // Exponentiation operator
          if (inspect () == '*') { 

            currentChar = inspect ();
            bufferChar(currentChar) ;
            advance ();

             //std::cout << tokenBuffer << ' ' ;
             

            return ExpOp;

          }

          // lexical error, there is no multiplication
          else {

            std::cerr << "\nLexical Error\n";

          }

          break;

          // Plus operator
        case '+':

          bufferChar(currentChar) ;

           //std::cout << tokenBuffer << ' ' ;
           

          return PlusOp;

          break;

          // Minus operator or comment
        case '-':

          bufferChar(currentChar) ;
          // Is Comment ignore it
          if (inspect () == '-') {

            while (inspect() != '\n' && inspect() != '\r') {

              currentChar = read();

              bufferChar(currentChar) ;
            }

             //std::cout << tokenBuffer << '\n';

          }

          // Is minus operator
          else {

             //std::cout << tokenBuffer << ' ' ;
             

            return MinusOp;

          }  

          break;

          // Equality Operator
        case '=':

          bufferChar(currentChar) ;

           //std::cout << tokenBuffer << ' ' ;
           

          return EqualOp;

          break;

          // End of file encountered
        case EOF:

          bufferChar(currentChar) ;
           

          return EofSym;
          break;

        default:

          std::cerr << "\nLexical Error\n";

      }

    }while (currentChar && file.good());

  }

  void clearBuffer () {

    tokenBuffer = "";

  }

  void bufferChar(const char& c) {

    tokenBuffer += c;

  }

}
