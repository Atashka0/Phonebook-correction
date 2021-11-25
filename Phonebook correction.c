#include <stdio.h>
#include <string.h> 
#include <ctype.h>
#include <stdlib.h>

// Advance p to the first non-whitespace character.
// You can use isspace( ) from ctype.h. It's even better. 
// A note about the word 'const'. It protects against accidentily overwriting
// the memory that p points to. If the 'const' confuses you, you can remove 
// all occurrences of it from the template and not use it in your code. 
// If you understand how it works, it's better to use it. 

char* skipwhitespace( char* p ){// WORKS
	while ( isspace(*p) ) {
    	p++;
	}
  return p;
}


// Copy inp into data until either a (,) or the 0-character is
// enountered. 

char* readunquoteddata( char* inp, char* data ){// WORKS
	int i = 0;
	while ( *inp != ',' && *inp != 0 ) {
		*(data+i) = *(inp++);
		i++;
	}
	*(data+i) = 0;
	return inp;
}


// If the first non-space character is " 
// we copy everything until the next " into str, but
// remove the first and final quote. 
// We never read beyond a 0-character. 
// Skipping a possible (,) is not part of our job. 

char* readquoteddata(char* inp, char* data) {// WORKS
	if ( !inp || *inp != '\"' ) abort( );
  else{
	  int i;
	  inp++;
 	for ( i = 0; *(inp) != '\"' && *(inp) != 0; i++ ) {
		*( data+i ) = *(inp++);
	}
	*(data+i)=0;
	if ( *(inp) == 0 ) {
		return inp;
	}
	return ++inp;
  }	
}


char* readdata( char* inp, char* data) {// WORKS
	inp = skipwhitespace(inp);
  if (*(inp) == '\"'){
    inp = skipwhitespace(inp);
    inp = readquoteddata(inp, data);
  }
  else{
    inp = readunquoteddata(inp, data);
  }
  if (*inp == ','){
    inp++;
  }
  return inp;
}


void correctfield(char *fld, char* corrected) { //works
	// removes excess spaces/TABs
	fld = skipwhitespace( fld );
	int j = 0;
	for ( int i = 0; *(fld + i) != 0; i++ ) {
    	while (*(fld+i+1) == ' ' && *(fld+i) == ' '){
		    i++;
      	}
		*(corrected+j) = *(fld+i);
		j++;
	}
    corrected[j] = 0;
}


// You are not obliged to use this function, but you can: 
char USAcode( char c ){ //works
    if (c > '`' && c < 'p'){
        return 48 + ( 2 + (c - 97)/3 ); 
    }
	else if (c>111&&c<116){
        return '7';

    }
   else if(c>115&&c<119)
   {
     return '8';
   }
   else if(c>118 &&c<123)
   {
     return '9';
   }
   return c;
}



void correctPhoneNumber( char* number, char* corrected ) {
	int i = 0, j = 1;
	if (*number == '0' && *(number+1) == '0'){
		i+=2;
  	} else if (*number != '+' ){
      	i++;
  	}
	*corrected = '+';

  	while (*(number + (i)) != 0){
    	if ( isalpha( *(number + i) )) {
      	*(number+i) = tolower(*(number+i));
      	*(number + i) = USAcode(*(number + i));
    	}
    	if (*(number + i) <= '9' && *(number + i) >= '0'){
      	*(corrected + (j++)) = *(number + i);
    	}
		i++;
  	}
  *(corrected+j) = 0;
}


// Put your functions for task 2 here (make sure that you include tests 
// in main( ) ). 


// Determine the length of a file, by reading it once to the end
// If the file does not exist, return -1 as length.
// Don't forget to close the file.

int countlines( char* fname ) { //WORKS
	FILE* book = fopen(fname,"r");
  	if (book == NULL){
  		return -1;
  	}
	char c;
  char p;
  char string[20];
	int counter = 0;
	while(!feof(book)) {
	 	c = getc(book);
		if (c == '\n'){
      counter++;
      
    }
  }
	fclose(book);
	return counter;
}



// I find the interface to fgets annoying, so we write this function.
// We always assume that every line has 500 characters.

int readline( FILE* f, char line[] ){ // WORKS
	fgets(line,500,f);
 	if (line[strlen(line)-1] == '\n'){
    	line[strlen(line)-1] = 0;
		return '\n';
  	}
  	return EOF;
}


typedef struct 
{
   char firstname[30];
   char secondname[30];
   char address[50];    
   char phonenumber[30];

} PhoneEntry; 


// Eto podarok:

void initPhoneEntry( PhoneEntry* e ) //WORKS
{
   e -> firstname[0] = 0;
   e -> secondname[0] = 0;
   e -> address[0] = 0;
   e -> phonenumber[0] = 0;
}

void printPhoneEntry( FILE* f, PhoneEntry* e ){ //WORKS
  fprintf(f,"%-20s", e -> firstname);
  fprintf(f,"%-20s", e -> secondname);
  fprintf(f,"%-42s", e -> address);
  fprintf(f,"%-13s\n", e -> phonenumber);
} 


void printPhoneEntryCSV( FILE* f, PhoneEntry* e ){
  fprintf(f,"\"%s\", ", e -> firstname);
  fprintf(f,"\"%s\", ", e -> secondname);
  fprintf(f,"\"%s\", ", e -> address);
  fprintf(f,"\"%s\"\n", e -> phonenumber);
}


void printPhoneBook( FILE* f, PhoneEntry* start, int size ) //WORKS
{
	for (int i = 0; i < 45; i++)
		fprintf(f," ");
    fprintf( f, "Phonebook\n" );
	for (int i = 0; i < (20+20+50+14); i++)
		fprintf(f,"=");
	fprintf(f,"\n");
    fprintf(f,"Firstname           Surname             Address                                   Phone number \n");
	for (int i = 0; i < (20+20+50+14); i++)
		fprintf(f,"-");
	fprintf(f,"\n");
    for (int i = 0; i < size; i++){
      printPhoneEntry(f,start+i);
    }
}
void printPhoneBookCSV( FILE* f, PhoneEntry* start, int size ){
    for (int i = 0; i < size; i++){
        printPhoneEntryCSV( f, start+i);
    }
}

char* readwithcutoff(char* inp, char* fld, int flen ){ //WORKS
	// gets one data field with a limit of flen
  	char data[500];
  	inp = readdata(inp,data);
  	inp = skipwhitespace( inp );
	data[flen]=0;
	strcpy(fld, data);
	return inp;
}


void parseEntry( PhoneEntry* e, char* inp ) //WORKS
// reads one line of the phonebook.csv
// and inserts the data into a phoneEntry
{
  initPhoneEntry(e);
  inp = readwithcutoff(inp, e->firstname, 30);
  inp = readwithcutoff(inp, e->secondname, 30);
  inp = readwithcutoff(inp, e->address, 50);
  inp = readwithcutoff(inp, e->phonenumber, 30);
}
// Your code for Task 3 comes here: 

void checkPhoneEntry( PhoneEntry* e, int linenr )
{
   char inp[500];
   PhoneEntry corrected;
   int accepted = 0;
   while( !accepted )
   {  accepted = 1; 
      correctfield(e -> firstname, corrected.firstname);
      correctfield(e -> secondname, corrected.secondname);
      correctfield(e -> address, corrected.address);
      correctPhoneNumber(e -> phonenumber, corrected.phonenumber);
      if (strlen(corrected.firstname) == 0){
        printf("Firstname isn't set in %i line. Please, set one: ", linenr+1);
        scanf("%s", inp);
        readwithcutoff(inp, e -> firstname, 30);
        accepted = 0;
      }
      if (strlen(corrected.secondname) == 0){
        printf("Secondname isn't set in %i line. Please, set one: ",linenr+1);
        scanf("%s", inp);
        readwithcutoff(inp, e -> secondname, 30);
        accepted = 0;
      }
      if (strlen(corrected.address) == 0){
        printf("Address isn't set in %i line. Please, set one: ",linenr+1);
        scanf("%s", inp);
        readwithcutoff(inp, e -> address, 50);
        accepted = 0;
      }
      if (strlen(corrected.phonenumber) != 13){
        printf("Phone number isn't set in line %i. Please, set one: ",linenr+1);
        scanf("%s", inp);
        readwithcutoff(inp, e -> phonenumber, 50);
        accepted = 0;
      }
   }
   *e = corrected;
}


int main( ){
   printf( "CSCI 151, Assignment 3\n" );  // Don't change. 
   printf( "Ilyas Kudaibergenov" );           // Replace by your Imya Familiya 
   printf( "student ID = 202059098\n" );   // Your student number as string.
   printf("------\n");
   printf("TASK 1\n");
   printf("------\n");

   /////////////////////////////////////////////////////////
   // Put your tests for your functions of Task 1 here: 
    char test1[20] = "    xyz";
    char *test = test1;
      test = skipwhitespace(test);
      printf("%s\n",test);
      char test2[20] = "x    y      z";
      test = test2;
      char data[20];
      test = readunquoteddata(test,data);
      printf("%s\n",data);
       char test3[20] = "\"x    y      z\"";
      test = test3;
      test = readquoteddata(test,data);
      printf("%s\n",data);
      char test4[20] = "\"x,y,z\"";
      test = test4;
      readdata( test, data );
      printf("%s\n",data);
      char test5[20] = "x    y      z";
      test = test5;
       correctfield(  test,  data );
       printf("%s\n",data);
       char test6[20] = "00345yk33";
      test = test6;
      correctPhoneNumber( test, data );
      printf("%s\n",data);


   printf("\n\n\n------\n");
   printf("TASK 2\n");
   printf("------\n");
   ////////////////////////////////////////////////////////
   // Put your tests for your functions of Task 2 here: 
   FILE* text = fopen( "text.txt", "w" );
   fprintf(text, "My cousin Ben bought his daughter a dog.\nHe is professional footballer\nAttack on Titan is the best anime ever.");
   fclose(text);
    FILE* text1 = fopen( "text.txt", "r" );
    char fine[500];
    char k = readline( text1, fine );
    if (k == '\n'){
        printf("Test has been passed successfully\n");
    }
    k = readline( text1, fine );
     if (k == '\n'){
        printf("Test has been passed successfully\n");
    }
    k = readline( text1, fine );
     if (k == '\n'){
        printf("Test has been passed successfully\n");
    }
    fclose(text1);
    char fld[20];
    char inp[30] = "Test for readwithcutoff";
    readwithcutoff( inp, fld, 20);
    printf("%s\n", fld);
   // When you are finished testing, you can use the code below, by
   // uncommenting it:
   
   char bookname[] = "phoneobook.csv";  // When you submit, don't change this name!!

   int lines = countlines( bookname );
   printf( "number of lines = %d\n", lines );

   if( lines < 0 )
   {
      printf( "could not open the phonebook %s\n", bookname );
      return -1;
   }

   // Now we know how much to allocate, so let's do it: 

   PhoneEntry* thebook  = malloc( ( lines + 1 ) * sizeof( PhoneEntry ));
   int thebooksize = 0;
   if( !thebook )
   {
      printf( "could not reserve memory\n" );
      return 0;
   }

   FILE* f = fopen( bookname, "r" );
   if( !f ) 
   {
      printf( "could not open the phonebook %s\n", bookname );
      free( thebook ); 
      return -1;
   }

   int linenr = 1;

   char line[500];
   char c = readline( f, line );
   while( c != EOF )
   {
      char* p = line;
      p = skipwhitespace(p);

      if( *p )
      {
         parseEntry( thebook + thebooksize, p );
         ++ thebooksize;
      }
      c = readline( f, line ); 
      ++ linenr; 
      
   }

   


   printf("\n\n\n------\n");
   printf("TASK 3\n");
   printf("------\n");
   /////////////////////////////////////////////////////////////////
   // Put your code for Task 3 here: 

   FILE* output = fopen("output.txt","w");
   FILE* outputcsv = fopen("output.csv","w");
   for (int i = 0; i < thebooksize; i++){
    checkPhoneEntry(&thebook[i], i);
   }
   printPhoneBook(output, thebook, thebooksize);
   printPhoneBookCSV(outputcsv, thebook,thebooksize);
   fclose(output);
  
   // Don't forget to do this:
   free( thebook );


   // DO NOT WRITE ANYTHING BELOW THIS LINE!
   // DO NOT REMOVE OR CHANGE THIS LINE, IT IS USED FOR AUTOMATIC PROCESSING.

   return 0;
}




