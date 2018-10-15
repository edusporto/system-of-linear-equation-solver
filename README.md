# system-of-linear-equation-solver
## C program to solve systems of linear equations. (In Portuguese)

### About
  * Made by Eduardo Sandalo Porto for the Topics in Object Oriented Programming class;
  * Colégio Técnico de Campinas - Unicamp, 2017;
  * The code is in Portuguese;
  * Compiled using GCC;
  * The program has certain restrictions given by the teacher, such as:
    * Arrays and matrices can only be accessed by pointer arithmetic;
    * All the variables in a system of linear equations must be stored in linked lists.

### Usage
  * The equations must be in a text file;
  * The first line of the line contains an integer, which corresponds to the number of equations to read;
  * The following lines contain the equations in the following format:
    * nameone + nametwo = 1
    * 2nameone - nametwo = -1
  * The numbers can be any double value, which can be written using '.';
  * The name of the variables can be of any size up to 1024, and each line must be smaller than 2048 characters;
  * The name of the variables can contain any characters, except numbers, dots, +, and -.
  
### There are example systems in the "sistemas" folder
