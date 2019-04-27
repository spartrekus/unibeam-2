
//////////////////////////////////
//////////////////////////////////
// unibeam 
//////////////////////////////////
//////////////////////////////////
// AUTHOR: Spartrekus           //
// GNU LICENCE                  //
// Date: 2017 - June - 25       //
// Aim: 2017 - June - 25        //
//   For Making Thesis Easily   //
//   and more                   //
// Destined to Amiga, Sun, .... //
// Atari, CPC, ... having C     //
//////////////////////////////////
//////////////////////////////////

//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////
/// Note: include, Unibeam is just tiny, isn't it?
//////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////


//////////
// manual
/*
> content
!qu item list with nbr
- item list
- item list
!li item list with - 
!ln item list without - 
*/
//////////

#include <stdio.h>
#if defined(__linux__)
#define MYOS 1
#elif defined(_WIN32)
#define MYOS 2
#elif defined(_WIN64)
#define MYOS 3
#elif defined(__unix__)
#define MYOS 4
#define PATH_MAX 2500
#else
#define MYOS 0
#endif
#include <string.h>
#include <stdlib.h>

#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h> 

#include <time.h>



#define VERSIONNBR "0.11" 


char strlabel[PATH_MAX];
char cursubsection[PATH_MAX];
char lastsubsection[PATH_MAX];


/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/// global vars which can be removed for better readability 
//  note that !img are images (similar) for classical easy to use work (quick, simple).
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
int table_col_nbr     = 0; 
int table_col_nbr_max = 0; 
char doc_version[PATH_MAX];
char slidebufferdata[200][PATH_MAX];
char slidebufferfigfile[PATH_MAX];
char slidebufferfigfile1[PATH_MAX];
char slidebufferfigfile2[PATH_MAX];
char slidebufferfignote[PATH_MAX];
int  slidebufferfig=0;
int  slidebuffernot=0;
int  foxy = 0;
int  mynumber = 1;

int fp_content_eof = 0;

int  slidebufferfoundsection = 0;
char slidemysection[PATH_MAX];
char mygraphicspath[PATH_MAX];
char myinputspath[PATH_MAX];

// for questions, to make active
int list_numbering = 1;
int question_qucounter = 1; 
int question_setpts = 1;
int answer_show = 0;  // for !an for answer, below !qu



//////// LANGUAGE 
int markup_output_format = 1; // type of doc: 1:tex, 2:html, 3:exam (pts), 4:book, 5:opendoc , 6:exam+ (with points) new and testing,    7: for maths with enumitem,  8: exam list i.e. the first approach, stable (exam in english), 
/// format 10 is for !qu, format 10 is new and for fast !qu pkt document (without >).
//// format 11 is for a book with conv the > and - 

// en+ gives doc language 4
// 1: tex (default)
// 2: html
// 3: exam
// 6: exam+ (new, default, started with !exam+) 
// 7: math documentation  

int markup_language = 1; // 1:english (standard, international), 2:french, 3:german, 4:en+ english i.e. en+ (for exam, e.g. 3 points)
// with !set lang=en

/////////////// (useful quick workaround)
int markup_item = 1;    //  1:default , 2:3pkt,  //for rapid update



int txtbeginreview = 0; 
int txtrawcode = 0; 
int beamercode = 0; 
int bookcode = 0; 
int notercode  = 0; 
int contentcode = 0; 
int option_system_call = 0;           // this will be 0 by default (depreciated)
int option_strtxt2tex_linefeed = 1;   // this shall be 0 by default for compact documents



/*
!= NOTES: 
=========

Bullet list: (!ul)
------------
> my list                         <ul> my list
!ul apples                        <li>apples</li>
!ul oranges                       <li>oranges</li>                                   Horizontal
!ul pears                         <li>pears</li>                                     rule:
                                  </ul>

 
Numbered list: (!ol)                 
> my list
!ol wash
!ol rinse
!ol repeat
                                <p>Numbered list:</p>
                                <ol> my list
                                  <li>wash</li>
                                  <li>rinse</li>
                                  <li>repeat</li>
                                </ol>

!= Example of usage

> my list 
!ul test1
!ul test2
!ul test3
!ul test4

>> my list 
!ol test1
!ol test2
!ol test3
!ol test4
*/





/////////////////////////////////
/////////////////////////////////
/////////////////////////////////
int fexist(char *a_option)
{
  char dir1[PATH_MAX]; 
  char *dir2;
  DIR *dip;
  strncpy( dir1 , "",  PATH_MAX  );
  strncpy( dir1 , a_option,  PATH_MAX  );

  struct stat st_buf; 
  int status; 
  int fileordir = 0 ; 

  status = stat ( dir1 , &st_buf);
  if (status != 0) {
    fileordir = 0;
  }

  // this is compatible to check if a file exists
  FILE *fp2check = fopen( dir1  ,"r");
  if( fp2check ) {
  // exists
  fileordir = 1; 
  fclose(fp2check);
  } 

  if (S_ISDIR (st_buf.st_mode)) {
    fileordir = 2; 
  }
return fileordir;
/////////////////////////////
}







//////////////////
//////////////////
//////////////////
void ncp( char *filetarget,  char *  filesource )
{
  // fread
  char            buffer[1];
  size_t          n;
  size_t          m;
  FILE *fp;
  FILE *fp1; 
  FILE *fp2;
  int counter = 0 ; 
  int freader = 1 ; 
  int i , j ,posy, posx ; 
  if ( fexist( filesource ) == 1 )
  {
        fp = fopen( filesource, "rb");
        fp2 = fopen( filetarget, "wb");
        counter = 0; 
        while(  !feof(fp) && ( freader == 1)   ) {
           if (  feof(fp)   ) {
                freader = 0 ; 
            }
            n = fread(  buffer, sizeof(char), 1 , fp);
            m = fwrite( buffer, sizeof(char), 1,  fp2);
        }
        fclose(fp2);
        fclose(fp);
      }
}










/////////////////////////////////
/////////////////////////////////
/////////////////////////////////
char *strampersand2txt(char *str)
{  
      char ptr[ 5* strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( str[i] == '&' ) 
	{
          ptr[j++]=':';
	}
        else
	{
          ptr[j++]=str[i];
	}
      } 
      ptr[j]='\0';
      size_t siz = 1 + sizeof ptr ; 
      char *r = malloc( 1 +  sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}






/////////////////////////////////
/////////////////////////////////
/////////////////////////////////
char *strtxt2tex(char *str)
{  
      char ptr[ 5* strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {

        if ( str[i] == '{' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='{';
	}

        else if ( str[i] == '^' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='^';
	}

        else if ( str[i] == '%' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='%';
	}

        else if ( str[i] == '>' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='e';
          ptr[j++]='n';
          ptr[j++]='s';
          ptr[j++]='u';
          ptr[j++]='r';
          ptr[j++]='e';
          ptr[j++]='m';
          ptr[j++]='a';
          ptr[j++]='t';
          ptr[j++]='h';
          ptr[j++]='{';
          ptr[j++]='>';
          ptr[j++]='}';
          //ptr[j++]=' ';
	}

        else if ( str[i] == '<' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='e';
          ptr[j++]='n';
          ptr[j++]='s';
          ptr[j++]='u';
          ptr[j++]='r';
          ptr[j++]='e';
          ptr[j++]='m';
          ptr[j++]='a';
          ptr[j++]='t';
          ptr[j++]='h';
          ptr[j++]='{';
          ptr[j++]='<';
          ptr[j++]='}';
          //ptr[j++]=' ';
	}


        else if ( str[i] == '}' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='}';
	}
        else if ( str[i] == '_' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='_';
	}
        else if ( str[i] == '%' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='%';
	}
        else if ( str[i] == '#' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='#';
	}

        else if ( str[i] == '&' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='&';
	}

        else if ( str[i] == '%' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='%';
	}

        else
	{
          ptr[j++]=str[i];
	}
      } 
      ptr[j]='\0';
      size_t siz = 1 + sizeof ptr ; 
      char *r = malloc( 1 +  sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}



/////////////////////////////////
/////////////////////////////////
/////////////////////////////////
char *strtext2tex(char *str)
{  
      char ptr[ 5* strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( str[i] == '&' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='&';
	}
        else if ( str[i] == '_' ) 
	{
          ptr[j++]=' ';
	}
        else if ( str[i] == '%' ) 
	{
          ptr[j++]='\\';
          ptr[j++]='%';
	}
        else
	{
          ptr[j++]=str[i];
	}
      } 
      ptr[j]='\0';
      size_t siz = 1 + sizeof ptr ; 
      char *r = malloc( 1 +  sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}




/////////////////////////////////
/////////////////////////////////
/////////////////////////////////
char *strcsv2tex(char *str)
{  
      char ptr[ 5* strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( str[i] == ';' ) 
	{
          ptr[j++]=' ';
          ptr[j++]='&';
          ptr[j++]=' ';
	}

        else if ( str[i] == '|' ) 
	{
          if ( table_col_nbr < table_col_nbr_max-1 ) 
          {
            ptr[j++]=' ';
            ptr[j++]='&';
            ptr[j++]=' ';
            table_col_nbr++;
          }
	}

        else if ( str[i] == 9 ) 
	{
          ptr[j++]=' ';
          ptr[j++]='&';
          ptr[j++]=' ';
	}

        else
	{
          ptr[j++]=str[i];
	}
      } 
      ptr[j]='\0';
      size_t siz = 1 + sizeof ptr ; 
      char *r = malloc( 1 +  sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}












char *fbasewithoutfilename(char* mystr)
{
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '/');
    if (lastdot != NULL)
        *lastdot = '\0';
    return retstr;
}





char *fbasenoext(char* mystr)
{
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    return retstr;
}








////////////////////////////
////////////////////////////
////////////////////////////
////////////////////////////
////////////////////////////
///// some cat!! 
#include <fcntl.h>
#include <unistd.h>
static int cat_fd(int fd) 
{
  char buf[4096];
  ssize_t nread;

  while ((nread = read(fd, buf, sizeof buf)) > 0) 
  {
    ssize_t ntotalwritten = 0;
    while (ntotalwritten < nread) {
      ssize_t nwritten = write(STDOUT_FILENO, buf + ntotalwritten, nread - ntotalwritten);
      if (nwritten < 1)
        return -1;
      ntotalwritten += nwritten;
    }
  }
  return nread == 0 ? 0 : -1;
}
/////////////////
static int ncat_static(const char *fname) 
{
  int fd, success;
  if ((fd = open(fname, O_RDONLY)) == -1)
    return -1;

  success = cat_fd(fd);

  if (close(fd) != 0)
    return -1;

  return success;
}
////////////////////////////
////////////////////////////
////////////////////////////
////////////////////////////
////////////////////////////










///////////////////////////////
///////////////////////////////
///////////////////////////////
char *strtimestamp()
{
      long t;
      struct tm *ltime;
      time(&t);
      ltime=localtime(&t);
      char charo[50];  int fooi ; 
      fooi = snprintf( charo, 50 , "%04d%02d%02d%02d%02d%02d", 1900 + ltime->tm_year, ltime->tm_mon +1 , ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec );
    size_t siz = sizeof charo ; 
    char *r = malloc( sizeof charo );
    return r ? memcpy(r, charo, siz ) : NULL;
}





int strcount( char *str , int mychar )
{  
      int return_strcount = 0;
      char ptr[strlen(str)+1];
      int i,j = 0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( str[i] == mychar ) return_strcount++;
      } 
      return return_strcount;
}






void nrunwith( char *thecmd , char *thefile )
{
       char cmdi[PATH_MAX];
       printf( "<CMD: %s>\n" , thecmd ) ; 
       printf( "<FILE: %s>\n" , thefile ) ; 
       strncpy( cmdi , "  " , PATH_MAX );
       strncat( cmdi , thecmd , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , " \"" , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , thefile , PATH_MAX - strlen( cmdi ) -1 );
       strncat( cmdi , "\"" , PATH_MAX - strlen( cmdi ) -1 );
       system( cmdi );
}







char *strcut( char *str , int myposstart, int myposend )
{  
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( ( str[i] != '\0' ) && ( str[i] != '\0') )
         if ( ( i >=  myposstart-1 ) && (  i <= myposend-1 ) )
           ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}






char *strtrim(char *str)
{  
      // right side to to finish
      char ptr[strlen(str)+1];
      int strposmax = strlen( str );
      int lastposchar = strposmax;
      int i,j=0;
      int foundspace = 1;

      /// find last char
      foundspace = 1;
      for( i= strposmax-1 ; i >= 0 ; i--)
      {
         //printf( "|%d-%d-%c| ", i , lastposchar , str[i] );
	 // find where to space
         if ( foundspace == 1 ) 
         if ( str[i] == ' ' ) 
   	    lastposchar = i-1;

         if ( str[i] != ' ' ) 
           foundspace = 0;
      } 

      // add the content
      foundspace = 1;
      for(i=0; i <= lastposchar ; i++)
      {
        if ( foundspace == 1 ) 
         if ( ( str[i] != ' ' )  && ( str[i] != 9 ) ) //added! with 9 for a TAB!!
          foundspace = 0;

        if ( foundspace == 0 ) 
           ptr[j++]=str[i];
      } 
      ptr[j]='\0';

      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}







/////////////////////////////////////
void fileappendbegin( char *fileout)
{
    FILE *fp5;
    fp5 = fopen( fileout , "ab+");
      fputs( "\\documentclass{article}\n", fp5 );
      fputs( "\\begin{document}\n", fp5 );
    fclose( fp5 );
}
/////////////////////////////////////
void fileappendend( char *fileout)
{
    FILE *fp5;
    fp5 = fopen( fileout , "ab+");
      fputs( "\\end{document}\n", fp5 );
    fclose( fp5 );
}




/////////////////////////////////////
void filenew_begin( char *fileout)
{
    FILE *fp5;
    fp5 = fopen( fileout , "wb+");
    fputs( "\n", fp5 );
    fputs( "\\documentclass[11pt]{article}\n", fp5 );
    fputs( "\\usepackage{grffile}\n" , fp5 );
    fputs( "\\usepackage{graphicx}\n" , fp5 );
    fputs( "\\usepackage{xcolor}\n" , fp5 );
    fputs( "\\usepackage[utf8]{inputenc}\n", fp5 );
    fputs( "\\begin{document}\n" , fp5 );
    fputs( "\n" , fp5 );
    fputs( "\n", fp5 );
    fclose( fp5 );
}



/////////////////////////////////////
void filenew( char *fileout)
{
    FILE *fp5;
    fp5 = fopen( fileout , "wb+");
    fclose( fp5 );
}






///////////////////////////////////////////////////////////////////
void filerawcat(  char *fileout, char *filein )
{
  int fetchi;
  FILE *fp5;
  FILE *fp6;
  char fetchline[PATH_MAX];
  char fetchlinetmp[PATH_MAX];

  /////////////////////////////////////////////////
  if ( fexist( filein ) == 1 )
  {
    fp5 = fopen( fileout , "ab+");
    fp6 = fopen( filein , "rb");
    while( !feof(fp6) ) 
    {
          fgets(fetchlinetmp, PATH_MAX, fp6); 
          strncpy( fetchline, "" , PATH_MAX );
          for( fetchi = 0 ; ( fetchi <= strlen( fetchlinetmp ) ); fetchi++ )
            if ( fetchlinetmp[ fetchi ] != '\n' )
              fetchline[fetchi]=fetchlinetmp[fetchi];

         if ( !feof(fp6) )
         {
 	      fputs( fetchline , fp5 );
  	      fputs( "\n", fp5 );
	 }
     }
     fclose( fp5 );
     fclose( fp6 );
   }
}




/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
int strcharcount( char *str )
{  
      int rstrcharcount = 0;
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
         rstrcharcount++;
      return rstrcharcount;
}




/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
char *strrlf(char *str)
{  
      char ptr[strlen(str)+1];
      int i,j=0;
      for(i=0; str[i]!='\0'; i++)
      {
        if (str[i] != '\n' && str[i] != '\n') 
        ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}







/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
char *strsplit(char *str , int mychar , int myitemfoo )
{  
      char ptr[strlen(str)+1];
      int myitem = myitemfoo +1;
      int i,j=0;
      int fooitem = 0;
      for(i=0; str[i]!='\0'; i++)
      {
        if ( str[i] == mychar ) 
           fooitem++;
        else if ( str[i] != mychar &&  fooitem == myitem-2  ) 
           ptr[j++]=str[i];
      } 
      ptr[j]='\0';
      size_t siz = sizeof ptr ; 
      char *r = malloc( sizeof ptr );
      return r ? memcpy(r, ptr, siz ) : NULL;
}

/// customed one
char *strdelimit(char *str , int mychar1, int mychar2,  int mycol )
{ 
      char ptr[strlen(str)+1];
      char ptq[strlen(str)+1];
      strncpy( ptr, strsplit( str, mychar1 , mycol+1 ), strlen(str)+1 );
      strncpy( ptq, strsplit( ptr, mychar2 , 1 ), strlen(str)+1 );
      size_t siz = sizeof ptq ; 
      char *r = malloc( sizeof ptq );
      return r ? memcpy(r, ptq, siz ) : NULL;
}










char *fbasename(char *name)
{
  char *base = name;
  while (*name)
    {
      if (*name++ == '/')
	{
	  base = name;
	}
    }
  return (base);
}
















//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////
char *cat_returnline( char *thefilefoo, int theline  )
{
  char catfetchlinetmp[PATH_MAX];
  char catfetchline[PATH_MAX];
  char buf[PATH_MAX];
  char ptr[PATH_MAX];
  ssize_t nread;
  int i,j,k;
  double linenbr = 0;
  i = 0; 
  j = 0;
  int beginend = 0;
    FILE *fp9;
    fp9 = fopen( thefilefoo , "rb");
    while( !feof(fp9) ) 
    {
          strncpy( catfetchline, "" , PATH_MAX );
          fgets(catfetchlinetmp, PATH_MAX, fp9); 
          linenbr++;
          for( i = 0 ; ( i <= strlen( catfetchlinetmp ) ); i++ )
            if ( catfetchlinetmp[ i ] != '\n' )
              catfetchline[i]=catfetchlinetmp[i];

         if ( !feof(fp9) )
         {
              if ( linenbr == theline ) 
                 strncpy( ptr, catfetchline, PATH_MAX );
	 }
     }
     fclose( fp9 );

     size_t siz = 1 + sizeof ptr ; 
     char *r = malloc( 1 +  sizeof ptr );
     return r ? memcpy(r, ptr, siz ) : NULL;
}











//////////////////////////
/// depreciated - will be replaced!!
//////////////////////////
char *filename_newext( char *str , char *newext )
{
      char ptr[strlen(str)+1];
      int i,j=0;
      int foundpoint = 0; 
      for(i=strlen(str)-1 ; i!=-1 ; i--)
      {
          if ( str[i] == '.' ) foundpoint = i; 
      } 

     if ( foundpoint >= 1 ){
      int maxsize = strlen(str)+1+strlen(newext) ;
      char ptrnew[ maxsize ];
      strncpy( ptrnew, strcut( str, 1, foundpoint+1 ),  maxsize  ); 

      ptrnew[ foundpoint ] = '.';
      for(i=0 ; i<= strlen( newext)-1 ; i++)
         ptrnew[ foundpoint+1 +i ] = newext[ i ];
      ptrnew[ foundpoint +i+1] = '\0';

       size_t siz = sizeof ptrnew ; 
       char *r = malloc( sizeof ptrnew );
       return r ? memcpy(r, ptrnew, siz ) : NULL;
    } else return ""; 
}








///////////////////////////////////////////////////////////////////
////////// 
void nfileunimark( char *fileout, char *filein )
{
  char poolfilename[PATH_MAX];

  int fetchi;
  FILE *fp5;
  FILE *fp6;
  char fetchline[PATH_MAX];
  char fetchlinefoo[PATH_MAX];
  char fetchlinetmp[PATH_MAX];
  char fetchcmdline[PATH_MAX];
  int fetchcmdlinelen = 0;

  char charin[PATH_MAX];
  char charout[PATH_MAX];
  char lineout[PATH_MAX];

  char mkdirdir[PATH_MAX];
  char ndirfig[PATH_MAX];
  char usertextfieldone[PATH_MAX];
  char usertextfieleps[PATH_MAX];
  char usertextfieldtwo[PATH_MAX];
  char wgetcmd[PATH_MAX];
  char extcmd[PATH_MAX];
  int fooc; 
  char fooccharo[PATH_MAX];

  int foundcode = 0; 
  int advancedcode = 1; 
  int commenton = 0; 
  int beginitemize = 0; 

  int itemlevel = 0; 

  // !bull !im 
  int itemimlevel = 0;

  int beamerlevel = 0; 
  int numberinglevel = 0; 
  int unilistemptyline = 0; 
  int fooi; 
  //int fetchcmdpos=0;
  int foundcmd = 0;

  char foostr[PATH_MAX];
  int  foodo = 0;

  char mycurrenttag[PATH_MAX];
  strncpy( mycurrenttag, "", PATH_MAX );

  /////////////////////////////////////////////////
  if ( fexist( filein ) == 1 )
  {
    fp5 = fopen( fileout , "ab");
    fp6 = fopen( filein , "rb");
    while( !feof(fp6) ) 
    {
          // vars
	  foundcode = 0; 
	  foundcmd = 0; 

	  // this fgets is there to be easy to port to any systems
	  // fgets can be easily ported to things such as PHP !!

          fgets( fetchlinetmp, PATH_MAX, fp6); 
          strncpy( fetchline , "" , PATH_MAX );
          strncpy( fetchcmdline , "" , PATH_MAX );

          if ( fp_content_eof == 1 )
             foundcode = 1;

          //////////////////////////////////////
          if ( foundcode == 0 ) // low-level low level
          if ( fetchlinetmp[0] == '/' )
          if ( fetchlinetmp[1] == '/' )
          {
  	      foundcode = 1;
          }

          //////////////////////////////////
          //////////////////////////////////
          /// low-level comments
          //////////////////////////////////
          //////////////////////////////////
          if ( foundcode == 0 ) // low-level
          if ( fetchlinetmp[0] == '/' )
          if ( fetchlinetmp[1] == '/' )
          {
  	      foundcode = 1;
          }








	    /////////////////
            if ( foundcode == 0 ) // 
            if ( fetchline[0] == '#' ) // low-level
            //if ( fetchline[1] == ' ' ) // low-level
            if ( fetchline[1] != 'i' ) // different!!!
            if ( fetchline[2] != 'n' )
            if ( fetchline[3] != 'c' )
            if ( fetchline[4] != 'l' )
            if ( fetchline[5] != 'u' )
            if ( fetchline[6] != 'd' )
            if ( fetchline[7] != 'e' )
            {  
              /*
 	      fputs( "{\\color{blue}" , fp5 );
 	      fputs( strcut( fetchlinetmp, 0+2, strlen(fetchlinetmp)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "\n", fp5 ); */
              //datecom-20180202-002359, color was changed from red to blue
       	      //fputs( "\\textcolor{blue}{\\hl{" , fp5 );
       	      fputs( "\\textcolor{blue}{" , fp5 );
              fputs( "[", fp5 );
       	      //fputs( strcut( fetchline, 0+2, strlen( fetchline )) , fp5 );
              if ( txtrawcode == 1 )
       	         fputs(  strtxt2tex(strcut( fetchline, 0+2, strlen(fetchline))) , fp5 );
              else
       	         fputs( strcut( fetchline, 0+2, strlen(fetchline)) , fp5 );
              fputs( "]", fp5 );
              //fputs( "}}", fp5 );
              fputs( "}", fp5 );
              fputs( "\n", fp5 );
  	      foundcode = 1;
            } 









          // 20170811-180203
          // remove lf and more
          // there is only two modifiers !
          for( fetchi = 0 ; ( fetchi <= strlen( fetchlinetmp ) ); fetchi++ )
            if ( fetchlinetmp[ fetchi ] != '\n' )
	    {
              fetchline[fetchi]=fetchlinetmp[fetchi];
              if ( fetchlinetmp[ fetchi ] == '!' ) 
	         foundcmd = 1 ;
	    }






	    /////////////////
            // quick, easy review work with adding just !# 
	    /////////////////
            if ( foundcode == 0 ) // for review
            if ( fetchline[0] == '!' ) // low-level
            if ( fetchline[1] == '#' )
            if ( fetchline[2] == ' ' )
            {
       	         fputs( "\\textcolor{blue}{" , fp5 );
        	 fputs( "[", fp5 );
                 if ( txtrawcode == 1 )
       	            fputs( strtxt2tex(strcut( fetchline, 2+2, strlen(fetchline))) , fp5 );
                 else
       	            fputs( strcut( fetchline, 2+2, strlen(fetchline)) , fp5 );
        	 fputs( "]", fp5 );
        	 fputs( "}", fp5 );
        	 fputs( "\n", fp5 );
  	         foundcode = 1;
            }


         



         /////// txtrawcode, activated with !beginraw
         // you may let this section
          if ( txtrawcode == 1 )
          {
             if ( fetchline[0] != '!' ) 
             if ( fetchline[0] != '>' ) 
             {
                 strncpy( fetchlinefoo, fetchline , PATH_MAX );
                 strncpy( fetchline , strtxt2tex( fetchlinefoo ) , PATH_MAX );
             }
          }

          //// make a cmdline
          strncpy( fetchcmdline, strtrim( fetchline ) , PATH_MAX );
          fetchcmdlinelen = strlen(fetchcmdline);









          if ( !feof(fp6) )
          if ( foundcode == 0 )
          {
            /////////////////////////////////////
            ////  for comment mycomment
	    ////  type 1 like in C language
            //////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '/' )
            if ( fetchline[1] == '/' )
            {
  	      foundcode = 1;
            }


           ////////////////////////////////////
           if ( strcmp( fetchline, "" ) == 0 )
	        unilistemptyline++;
	      else
	        unilistemptyline = 0;




            //// closer, numbering itemize
            if ( foundcode == 0 )
	    if ( contentcode == 1 )
	    if ( numberinglevel >= 1 )
	    if ( unilistemptyline >= 1 )   // let set to one (1) line empty (testing)
            {
	      for ( fooi = 1 ; fooi <= numberinglevel ; fooi++)
	      {
                 if ( markup_output_format == 6 ) 
	           fputs( "\\end{enumerate}\n" , fp5 );
                 else
	           fputs( "\\end{itemize}\n" , fp5 );
	      }
 	      fputs( "\n" , fp5 );
 	      fputs( "\n" , fp5 );
	      numberinglevel = 0;
  	      foundcode = 1;
            }
	    if ( numberinglevel <= 0 ) numberinglevel = 0;


          /*
            /////////////////////////////////////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '#' )   // bou like markdown :( 
            if ( fetchline[1] == ' ' ) 
            {
 	        fputs( "\\section{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen( fetchline ))) , fp5 );
  	        fputs( "}\n", fp5 );
  	      foundcode = 1;
            }
            /////////////////////////////////////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '#' )   // bou like markdown :( 
            if ( fetchline[1] == '#' ) 
            if ( fetchline[2] == ' ' ) 
            {
 	        fputs( "\\subsection{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 2+2, strlen( fetchline ))) , fp5 );
  	        fputs( "}\n", fp5 );
  	      foundcode = 1;
            }
            /////////////////////////////////////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '#' )   // bou like markdown :( 
            if ( fetchline[1] == '#' ) 
            if ( fetchline[2] == '#' ) 
            if ( fetchline[3] == ' ' ) 
            {
 	        fputs( "\\subsubsection{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen( fetchline ))) , fp5 );
  	        fputs( "}\n", fp5 );
  	      foundcode = 1;
            }
            */










            if ( foundcode == 0 )
            if ( fetchline[0] == '#' )
            if ( strstr( fetchline, "#include" ) == 0 )
            if ( strstr( fetchline, "#input" ) == 0 )
            {
  	      foundcode = 1;
            }

            //////////////////////////////////
            //////////////////////////////////
            //////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '/' )
            if ( fetchcmdline[1] == '/' )
            {
  	      foundcode = 1;
            }



            /////////////////////////////////////
            /////////////////////////////////////
            //////// COMMENTS !!!!!!!!!!
            /////////////////////////////////////
            /////////////////////////////////////
            if ( foundcode == 0 )
            if (( fetchcmdline[0] == '/' ) && ( fetchcmdline[1] == '*' ))
            {
  	      commenton = 1;
    	      foundcode = 1;
            }
            ////////////////////////////////////
            if ( foundcode == 0 )
            if ( ( fetchcmdline[ fetchcmdlinelen -2 ] == '*' ) && ( fetchcmdline[ fetchcmdlinelen -1 ] == '/' ))
            {
  	      commenton = 0;
    	      foundcode = 1;
            }

            ////////////////////////////////////
            if ( foundcode == 0 )
            if ( ( fetchcmdline[0] == '*' ) && ( fetchcmdline[1] == '/' ))
            {
  	      commenton = 0;
    	      foundcode = 1;
            }
	    //// active if needed
            if ( commenton == 1 )
            {
    	      foundcode = 1;
            }




            /////////////////////////////////////
            if ( foundcode == 0 ) // !language de
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'l' )
            if ( fetchline[2] == 'a' )
            if ( fetchline[3] == 'n' )
            if ( fetchline[4] == 'g' )
            if ( fetchline[5] == 'u' )
            if ( fetchline[6] == 'a' )
            if ( fetchline[7] == 'g' )
            if ( fetchline[8] == 'e' )
            if ( fetchline[9] == ' ' )
            if ( fetchline[10] == 'd' )
            if ( fetchline[11] == 'e' )
            {
                markup_language = 3;
	        if ( contentcode == 0 ) 
                   fputs( "\\usepackage[figurename=Abb.]{caption}\n" , fp5 );
  	        foundcode = 1;
            }
            /////////////////////////////////////
            if ( foundcode == 0 ) // !lang de
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'l' )
            if ( fetchline[2] == 'a' )
            if ( fetchline[3] == 'n' )
            if ( fetchline[4] == 'g' )
            if ( fetchline[5] == ' ' )
            if ( fetchline[6] == 'd' )
            if ( fetchline[7] == 'e' )
            {
                markup_language = 3;
	        if ( contentcode == 0 ) 
                   fputs( "\\usepackage[figurename=Abb.]{caption}\n" , fp5 );
  	        foundcode = 1;
            }





            /////////
            if ( foundcode == 0 )  // !lang en+
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'l' ) 
            if ( fetchline[2] == 'a' ) 
            if ( fetchline[3] == 'n' ) 
            if ( fetchline[4] == 'g' ) 
            if ( fetchline[5] == ' ' ) 
            if ( fetchline[6] == 'e' ) 
            if ( fetchline[7] == 'n' ) 
            if ( fetchline[8] == '+' ) 
            {
              markup_language = 4; 
  	      foundcode = 1;
            }


            /////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'l' ) 
            if ( fetchline[2] == 'a' ) 
            if ( fetchline[3] == 'n' ) 
            if ( fetchline[4] == 'g' ) 
            if ( fetchline[5] == ' ' ) 
            if ( fetchline[6] == 'e' ) 
            if ( fetchline[7] == 'n' ) 
            {
              markup_language = 1; 
  	      foundcode = 1;
            }






            //////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////
            if ( foundcode == 0 )  // !pdf{ }, note that you may need !packages
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'p' ) 
            if ( fetchline[2] == 'd' ) 
            if ( fetchline[3] == 'f' ) 
            if ( fetchline[4] == '{' ) 
            {
 	      fputs( "\\includepdf{" , fp5 );
              fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }





            /////////////////////////////////
            /////////////////////////////////
            /////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( markup_output_format == 8 ) 
            // int markup_output_format = 1; // type of doc: 1:tex, 2:html, 3:exam (pts), 4:book, 5:opendoc , 6:exam+ (with points) new and testing,    7: for maths with enumitem,  8: exam list i.e. the first approach, stable (exam in english), 
            // 6 is exam+, but above 8 is exam (regular one)
            // 6 is actually nicer to use.
            if ( fetchline[1] == 'q' ) // used for english type of questions (single question list, with pts)
            if ( fetchline[2] == 'u' )
            if ( fetchline[3] == ' ' )
            {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item[{\\bfseries {" , fp5 );
                fooc = snprintf( fooccharo, 50 , "%d", question_qucounter );
 	        fputs( fooccharo  , fp5 );
 	        fputs( "}}.)]{" , fp5 );
                question_qucounter++;
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
                if      ( markup_language == 1 ) fputs( " (3 Points)}" , fp5 );  //the one that is usually being used.
                else if ( markup_language == 2 ) fputs( " (3 Pts.)}" , fp5 );
                else if ( markup_language == 3 ) fputs( " (3 Pkt.)}" , fp5 );
                else if ( markup_language == 0 ) fputs( "}" , fp5 );
 	        fputs( "\n" , fp5 );
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\vspace{4cm}\n" , fp5 );  // the trick is to have 4cm to allow answers.
  	      foundcode = 1;
            }







            /////////////////////////////////////
            /////////////////////////////////////
            if ( foundcode == 0 ) // !answer on
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'a' )
            if ( fetchline[2] == 'n' )
            if ( fetchline[3] == 's' )
            if ( fetchline[4] == 'w' )
            if ( fetchline[5] == 'e' )
            if ( fetchline[6] == 'r' )
            if ( fetchline[7] == ' ' )
            if ( fetchline[8] == 'o' )
            if ( fetchline[9] == 'n' )
            {
                answer_show = 1;
  	        foundcode = 1;
            }
            //// !qu for exam, with possible !an answers below.
            //// !mathlist, which is too working with >> and !qu 
            if ( foundcode == 0 )
            if ( answer_show == 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'a' )
            if ( fetchline[2] == 'n' )
            if ( fetchline[3] == ' ' )
            {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strcut( fetchline, 3+2, strlen(fetchline)) , fp5 );
 	        fputs( "\n" , fp5 );
 	        fputs( "\\end{itemize}\n" , fp5 );
  	        foundcode = 1;
            }
            //// !qu for exam, with possible !an answers below.
            //// !mathlist, which is too working with >> and !qu 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'a' )
            if ( fetchline[2] == 'n' )
            if ( fetchline[3] == ' ' )
            if ( answer_show == 0 )
            {
  	        foundcode = 1;
            }


            //// !list with !li for mit with for parameters in maths, with - before
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'l' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == ' ' )
            {
 	        fputs( "\\item " , fp5 );
 	        fputs( strcut( fetchline, 3+2, strlen(fetchline)) , fp5 );
 	        fputs( "\n" , fp5 );
  	        foundcode = 1;
            }

            //// !list with !ln for mit with for parameters in maths, without - 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'l' )
            if ( fetchline[2] == 'n' )
            if ( fetchline[3] == ' ' )
            {
 	        fputs( "\\item[] " , fp5 );
 	        fputs( strcut( fetchline, 3+2, strlen(fetchline)) , fp5 );
 	        fputs( "\n" , fp5 );
  	        foundcode = 1;
            }








            //// Math list 
            //// !mathlist, which is too working with >> and !qu 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'm' )
            if ( fetchline[2] == 'a' )
            if ( fetchline[3] == 't' )
            if ( fetchline[4] == 'h' )
            if ( fetchline[5] == 'l' )
            if ( fetchline[6] == 'i' )
            if ( fetchline[7] == 's' )
            if ( fetchline[8] == 't' )
            {
              markup_output_format = 7;
  	      foundcode = 1;
            } 
            if ( foundcode == 0 )
            if ( fetchline[0] == '>' )
            if ( fetchline[1] == '>' )
            if ( fetchline[2] == ' ' )
            if ( markup_output_format == 7 ) //!mathlist
            {
	      if ( numberinglevel == 1)  
	      {
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 2+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 1;
                list_numbering++;
	      }
	      else if ( numberinglevel == 0)  
	      {
 	        //fputs( "\\begin{enumerate}[resume]\n" , fp5 );
 	        fputs( "\\begin{enumerate}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 2+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 1;
                list_numbering++;
	      }
  	      foundcode = 1;
            }









            ////////////////////////////////////////////
            ////////////////////////////////////////////
            ////////////////////////////////////////////
            if ( foundcode == 0 )         // !qu  (main for maths)
            if ( beamercode == 0 ) 
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'q' )
            if ( fetchline[2] == 'u' )
            if ( fetchline[3] == ' ' )       // ! for maths with enumitems,... could be removed?
            if ( markup_output_format == 7 ) //this is for maths  (note here left output format)
            {
	      if ( numberinglevel == 2)  
	      {
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 2;
                list_numbering++;
	      }
	      else if ( numberinglevel == 1)  
	      {
 	        fputs( "\\begin{enumerate}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 2;
                list_numbering++;
	      }
  	      foundcode = 1;
            }





            ///////////////////////////////////////////////
            ///////////////////////////////////////////////
            if ( foundcode == 0 )   // !qu questions, for type !exam+ 
            if ( beamercode == 0 ) 
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'q' )
            if ( fetchline[2] == 'u' )
            if ( fetchline[3] == ' ' )
            if ( markup_output_format == 6 ) //for !exam+  // important area, exam+, to keep
            {
	      if ( numberinglevel == 2)  
	      {
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
                if ( question_setpts == 1 )
                {
                  if      ( markup_language == 4 )    fputs( " (3 points)" , fp5 );
                  else if ( markup_language == 3 )    fputs( " (3 Pkt.)" , fp5 );
                  else if ( markup_language == 1 )    fputs( " (3 points)" , fp5 );
                  else fputs( " (3 Pkt.)" , fp5 );
                }
 	        fputs( "\n" , fp5 );
		numberinglevel = 2;
                list_numbering++;
	      }
	      else if ( numberinglevel == 1)  
	      {
 	        fputs( "\\begin{enumerate}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
                if ( question_setpts == 1 )
                {
                  if      ( markup_language == 4 )    fputs( " (3 points)" , fp5 );
                  else if ( markup_language == 1 )    fputs( " (3 points)" , fp5 );
                  else if ( markup_language == 3 )    fputs( " (3 Pkt.)" , fp5 );
                  else fputs( " (3 Pkt.)" , fp5 );
                }
 	        fputs( "\n" , fp5 );
		numberinglevel = 2;
                list_numbering++;
	      }
  	      foundcode = 1;
            }


            /// !nqu 
            if ( foundcode == 0 )  
            if ( beamercode == 0 ) 
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'n' )
            if ( fetchline[2] == 'q' )
            if ( fetchline[3] == 'u' )
            if ( fetchline[4] == ' ' )
            {
	      if ( numberinglevel == 2)  
	      {
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 2;
                list_numbering++;
	      }
	      else if ( numberinglevel == 1)  
	      {
 	        fputs( "\\begin{enumerate}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 2;
                list_numbering++;
	      }
  	      foundcode = 1;
            }



            /////////////////////////////////
            ///////////////////////////////////////////////
            ///////////////////////////////////////////////
            if ( foundcode == 0 )   // !qu questions, for type !exam+, but !nqu has no points (for custom change) 
            if ( beamercode == 0 ) 
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'n' )
            if ( fetchline[2] == 'q' )
            if ( fetchline[3] == 'u' )
            if ( fetchline[4] == ' ' )
            if ( markup_output_format == 6 ) //for !exam+
            {
	      if ( numberinglevel == 2)  
	      {
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 2;
                list_numbering++;
	      }
	      else if ( numberinglevel == 1)  
	      {
 	        fputs( "\\begin{enumerate}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 2;
                list_numbering++;
	      }
  	      foundcode = 1;
            }
            /////////////////////////////////
            //
            //
            //
            //
            //
            //
            /////////////////////////////////
            /// for exams, especially !exam+ 
            if ( foundcode == 0 )
            if ( beamercode == 0 )
            if ( fetchline[0] == '>' )
            if ( fetchline[1] == '>' )
            if ( fetchline[2] == ' ' )
            if ( markup_output_format == 6 ) //!exam+
            {
	      if ( numberinglevel == 1)  
	      {
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 2+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 1;
                list_numbering++;
	      }
	      else if ( numberinglevel == 0)  
	      {
 	        fputs( "\\begin{enumerate}[resume]\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 2+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 1;
                list_numbering++;
	      }
  	      foundcode = 1;
            }







            /////////////////////////////
            if ( foundcode == 0 )
            if ( beamercode == 0 )
            if ( fetchline[0] == '>' )
            if ( fetchline[1] == '>' )
            if ( fetchline[2] == ' ' )
            {
	      if ( itemlevel == 0)  
	      {
                mynumber = 1;
 	        fputs( "\\begin{itemize}[leftmargin=0cm,label={}]\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 2+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 1;
	      }
  	      foundcode = 1;
             }







            /////////////////////////////////
            /////////////////////////////////
            // Try \begin{figure}[!htb]. In nearly all cases it helps.
            // % trick to easy figures (for beginners)
            // \usepackage[section]{placeins}
            /////////////////////////////////
            if ( foundcode == 0 )   // !placeins 
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'p' )
            if ( fetchline[2] == 'l' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'c' )
            if ( fetchline[5] == 'e' )
            if ( fetchline[6] == 'i' )
            if ( fetchline[7] == 'n' )
            if ( fetchline[8] == 's' )
            {
	      if ( contentcode == 0 ) 
              {
 	        fputs( "\n" , fp5 );
 	        fputs( "\\usepackage[section]{placeins}\n" , fp5 ); // new 
 	        fputs( "\n" , fp5 );
              }
  	      foundcode = 1;
            }











            // =================================
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '=' ) 
	    if ( fetchcmdline[1] == '=' ) 
	    if ( fetchcmdline[2] == '=' ) 
	    if ( fetchcmdline[3] == '=' ) 
	    if ( fetchcmdline[4] == '=' ) 
	    if ( fetchcmdline[5] == '=' ) 
	    if ( fetchcmdline[6] == '=' ) 
	    if ( fetchcmdline[7] == '=' ) 
	    if ( fetchcmdline[8] == '=' )
            {
  	      foundcode = 1;
            }





            // !institute text 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'i' )
            if ( fetchline[2] == 'n' )
            if ( fetchline[3] == 's' )
            if ( fetchline[4] == 't' )
            if ( fetchline[5] == 'i' )
            if ( fetchline[6] == 't' )
            if ( fetchline[7] == 'u' )
            if ( fetchline[8] == 't' )
            if ( fetchline[9] == 'e' )
            if ( fetchline[10] == ' ' )
            {
 	      fputs( "\\institute{" , fp5 );
 	      fputs( strcut( fetchline, 10+2, strlen(fetchline)) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }


            /// !inputline{ }
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'i' )
            if ( fetchline[2] == 'n' )
            if ( fetchline[3] == 'p' )
            if ( fetchline[4] == 'u' )
            if ( fetchline[5] == 't' )
            if ( fetchline[6] == 'l' )
            if ( fetchline[7] == 'i' )
            if ( fetchline[8] == 'n' )
            if ( fetchline[9] == 'e' )
            if ( fetchline[10] == '{' )
            {
              char fooreturnchar[PATH_MAX];
              char foofilefound[PATH_MAX];
              strncpy( foofilefound, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
              if ( fexist( foofilefound ) == 1 ) 
              {
                strncpy( fooreturnchar , cat_returnline(  foofilefound , atoi( strdelimit( fetchline,  '{' ,'}' ,  2 )))  , PATH_MAX );
 	        fputs(   fooreturnchar , fp5 );
  	        fputs( "\n", fp5 );
              }
  	      foundcode = 1;
            }



            /// new, easy conv from beamer to mrk 
            // !nullclr      // this is easy to conv !clr to !nullskip, to skip line 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'n' )
            if ( fetchline[2] == 'u' )
            if ( fetchline[3] == 'l' )
            if ( fetchline[4] == 'l' )
            if ( fetchline[5] == 'c' )
            if ( fetchline[6] == 'l' )
            if ( fetchline[7] == 'r' )
            {
  	      foundcode = 1; //just skip
            }
            // !nullskip  // this is easy to conv !bigskip to !nullskip, to skip line 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'n' )
            if ( fetchline[2] == 'u' )
            if ( fetchline[3] == 'l' )
            if ( fetchline[4] == 'l' )
            if ( fetchline[5] == 's' )
            if ( fetchline[6] == 'k' )
            if ( fetchline[7] == 'i' )
            if ( fetchline[8] == 'p' )
            {
  	      foundcode = 1; //just skip
            }



            // !FILE , to tag and mark the doc (!FILE: will result in skip)
            // note: uppercase / this line: skip
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'F' )
            if ( fetchline[2] == 'I' )
            if ( fetchline[3] == 'L' )
            if ( fetchline[4] == 'E' )
            if ( ( fetchline[5] == ':' ) || ( fetchline[5] == ' ' ) )
            {
  	      foundcode = 1; //just skip
            }


            // !author text 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'a' )
            if ( fetchline[2] == 'u' )
            if ( fetchline[3] == 't' )
            if ( fetchline[4] == 'h' )
            if ( fetchline[5] == 'o' )
            if ( fetchline[6] == 'r' )
            if ( fetchline[7] == ' ' )
            {
 	      fputs( "\\author{" , fp5 );
 	      fputs( strcut( fetchline, 7+2, strlen(fetchline)) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }




            // !date text 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'd' )
            if ( fetchline[2] == 'a' )
            if ( fetchline[3] == 't' )
            if ( fetchline[4] == 'e' )
            if ( fetchline[5] == ' ' )
            {
 	      fputs( "\\date{" , fp5 );
 	      fputs( strcut( fetchline, 5+2, strlen(fetchline)) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }


            // !version text 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'v' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'r' )
            if ( fetchline[4] == 's' )
            if ( fetchline[5] == 'i' )
            if ( fetchline[6] == 'o' )
            if ( fetchline[7] == 'n' )
            if ( fetchline[8] == ' ' )
            {
              strncpy( doc_version, strcut( fetchline, 8+2, strlen(fetchline)) ,  PATH_MAX );
  	      foundcode = 1;
            }


            // force raw mode for given area/line
            // raw here 
            if ( foundcode == 0 )
            if ( fetchline[0] ==  '!' )
            if ( fetchline[1] ==  'r' ) 
            if ( fetchline[2] ==  'a' ) 
            if ( fetchline[3] ==  'w' ) 
            if ( fetchline[4] ==  ' ' ) 
            {
 	      fputs(  strtxt2tex( strcut( fetchline, 4+2, strlen( fetchline)) ) , fp5 );
  	      foundcode = 1;
            }



            // !title text 
            if ( foundcode == 0 )
            if ( (( fetchline[0] == '!' )
            && ( fetchline[1] == 't' )
            && ( fetchline[2] == 'i' )
            && ( fetchline[3] == 't' )
            && ( fetchline[4] == 'l' )
            && ( fetchline[5] == 'e' )
            && ( fetchline[6] == ' ' )))
            {
 	      fputs( "\\title{" , fp5 );
              if ( fetchline[0] == '!' )
 	        fputs( strcut( fetchline, 6+2, strlen(fetchline)) , fp5 );
              else
 	        fputs(  strtxt2tex( strcut( fetchline, 7+2, strlen(fetchline)) ) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }





            // !maketitle
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'm' )
            if ( fetchcmdline[2] == 'a' )
            if ( fetchcmdline[3] == 'k' )
            if ( fetchcmdline[4] == 'e' )
            if ( fetchcmdline[5] == 't' )
            if ( fetchcmdline[6] == 'i' )
            if ( fetchcmdline[7] == 't' )
            if ( fetchcmdline[8] == 'l' )
            if ( fetchcmdline[9] == 'e' )
            {
 	      fputs( "\\maketitle" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }






            /////////////////////////////////////
            /// for !line 
            /////////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'l' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'n' )
            if ( fetchline[4] == 'e' )
            if ( fetchline[5] == ' ' )
            {
 	      fputs( strcut( fetchline , 5+2, strlen(  fetchline ))    , fp5 );
  	      fputs( "\\", fp5 );
  	      fputs( "\\", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }




            /////////////////////////////////////
            /// for !centerline 
            /////////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'c' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'n' )
            if ( fetchline[4] == 't' )
            if ( fetchline[5] == 'e' )
            if ( fetchline[6] == 'r' )
            if ( fetchline[7] == 'l' )
            if ( fetchline[8] == 'i' )
            if ( fetchline[9] == 'n' )
            if ( fetchline[10] == 'e' )
            if ( fetchline[11] == ' ' )
            {
              fputs( "\\begin{center}\n" , fp5 );
 	      fputs( strcut( fetchline, 11+2, strlen(  fetchline ))    , fp5 );
  	      fputs( "\\", fp5 );
  	      fputs( "\\", fp5 );
  	      fputs( "\n", fp5 );
              fputs( "\\end{center}\n" , fp5 );
  	      foundcode = 1;
            }




            /////////////////////////////////////
            /// for CSV or tables !!!!!!!!!!!!  !csv for tables
            /////////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'c' )
            if ( fetchline[2] == 's' )
            if ( fetchline[3] == 'v' )
            if ( fetchline[4] == ' ' )
            {
 	      fputs( strcsv2tex(  strtext2tex( strcut( fetchline , 4+2, strlen(  fetchline )) ) )   , fp5 );
  	      fputs( " ", fp5 );
  	      fputs( "\\", fp5 );
  	      fputs( "\\", fp5 );
  	      fputs( " \\hline ", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }
            /// !tsv 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 't' )
            if ( fetchline[2] == 's' )
            if ( fetchline[3] == 'v' )
            if ( fetchline[4] == ' ' )
            {
 	      fputs( strcsv2tex(  strtext2tex( strcut( fetchline , 4+2, strlen(  fetchline )) ) )   , fp5 );
  	      fputs( " ", fp5 );
  	      fputs( "\\", fp5 );
  	      fputs( "\\", fp5 );
  	      fputs( " \\hline ", fp5 );
  	      fputs( "\n", fp5 );
  	      fputs( " \\hline ", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }




            ///////////////////////////////////
            if ( foundcode == 0 )  //// !btable{  
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'b' )
            if ( fetchline[2] == 't' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'b' )
            if ( fetchline[5] == 'l' )
            if ( fetchline[6] == 'e' )
            if ( fetchline[7] == '{' )
            {
              fputs( "\\begin{center}\n" , fp5 );
              fputs( "\\begin{tabular}{|*{" , fp5 );
              fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
              fputs( "}{c|}}\n" , fp5 );
  	      fputs( "\\hline\n", fp5 );
  	      foundcode = 1;
            }
            ///////////////////////////////////
            if ( foundcode == 0 )  // !btable #colnbr
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'b' )
            if ( fetchline[2] == 't' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'b' )
            if ( fetchline[5] == 'l' )
            if ( fetchline[6] == 'e' )
            if ( fetchline[7] == ' ' )
            {
              fputs( "\\begin{center}\n" , fp5 );
              fputs( "\\begin{tabular}{|*{" , fp5 );
 	      fputs( strtrim( strcut( fetchline, 7+2, strlen( fetchline ))) , fp5 );
              table_col_nbr_max = atoi(  strtrim( strcut( fetchline, 7+2, strlen( fetchline ))) );
              fputs( "}{c|}}\n" , fp5 );
  	      fputs( "\\hline\n", fp5 );
  	      foundcode = 1;
            }

            ///////////////////////////////////
            if ( foundcode == 0 )   //// !etable 
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'e' )
            if ( fetchline[2] == 't' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'b' )
            if ( fetchline[5] == 'l' )
            if ( fetchline[6] == 'e' )
            {
 	      fputs( "\\end{tabular}\n" , fp5 );
              fputs( "\\end{center}\n" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }









            /////////////////////////////////
            if ( foundcode == 0 )  // !vsp 
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'v' )
            if ( fetchline[2] == 's' )
            if ( fetchline[3] == 'p' )
            if ( fetchline[4] == ' ' )
            {
 	      fputs( "\\vspace{" , fp5 );
 	      fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
 	      fputs( "}" , fp5 );
  	      fputs( "\n", fp5 );
	      unilistemptyline++;
  	      foundcode = 1;
            }







            //// !toc
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
	    if ( strcmp( fetchcmdline, "!toc" ) == 0 )
            {
 	      fputs( "\\tableofcontents" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            } 




            /////////////////////////////////
            /////////////////////////////////
            // \usepackage{url}
            // \usepackage{hyperref}
            // \usepackage{graphicx}
            // \usepackage{grffile}
            //  xcolor ? for xfig fig2dev
            /////////////////////////////////
            /////////////////////////////////
            if ( foundcode == 0 )   // !gfx 
            if ( beamercode != 1 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'g' )
            if ( fetchline[2] == 'f' )
            if ( fetchline[3] == 'x' )
            {
 	      fputs( "\n" , fp5 );
 	      fputs( "\\usepackage{url}\n" , fp5 );
 	      fputs( "\\usepackage{hyperref}\n" , fp5 );
 	      fputs( "\\usepackage{grffile}\n" , fp5 );
 	      fputs( "\\usepackage{graphicx}\n" , fp5 );
 	      fputs( "\\usepackage{xcolor}\n" , fp5 );
 	      fputs( "\n" , fp5 );
  	      foundcode = 1;
            }




            if ( foundcode == 0 )   // !thankyou (easy addon)
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 't' )
            if ( fetchline[2] == 'h' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'n' )
            if ( fetchline[5] == 'k' )
            if ( fetchline[6] == 'y' )
            if ( fetchline[7] == 'o' )
            if ( fetchline[8] == 'u' )
            {
              fputs( "\\clearpage", fp5 );
              fputs( "\\begin{frame}", fp5 );
              fputs( "\\vspace{3.3cm}", fp5 );
              fputs( "\\begin{center}", fp5 );
              fputs( "{\\fontsize{30}{40}\\selectfont {\\it Thank you for your attention}}", fp5 );
              fputs( "\\end{center}", fp5 );
              fputs( "\\end{frame}", fp5 );
  	      foundcode = 1;
            }






            /////////////////////////////////////////
            /////////////////////////////////////////
            /////////////////////////////////////////
            if ( foundcode == 0 )   // !postit   
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'p' )
            if ( fetchline[2] == 'o' )
            if ( fetchline[3] == 's' )
            if ( fetchline[4] == 't' )
            if ( fetchline[5] == 'i' )
            if ( fetchline[6] == 't' )
            {
 	      //fputs( "\n" , fp5 );
              /// to cont
 	      //fputs( "\n" , fp5 );
  	      foundcode = 1;
            }


            /////////////////////////////////////////
            if ( foundcode == 0 )   // !reset section
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'r' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 's' )
            if ( fetchline[4] == 'e' )
            if ( fetchline[5] == 't' )
	    if ( strcmp( fetchcmdline, "!reset section" ) == 0 )
            {
                 fputs(  "\n" , fp5 );
                 fputs( "\\setcounter{section}{0}", fp5 );
  	         fputs( "\n", fp5 );
  	         foundcode = 1;
            }




            /////////////////////////////////////////
            if ( foundcode == 0 )   // !part text
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'p' )
            if ( fetchline[2] == 'a' )
            if ( fetchline[3] == 'r' )
            if ( fetchline[4] == 't' )
            if ( fetchline[5] == ' ' )
            {
  	      fputs( "\\vspace{0.5cm}\n", fp5 );
 	      fputs( "\\noindent\n" , fp5 );
 	      fputs( "\\textit{" , fp5 );
 	      fputs( "{\\bfseries " , fp5 );
 	      fputs( strcut( fetchline, 5+2, strlen(fetchline)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "}\n", fp5 );
  	      //fputs( "\\\\", fp5 );
  	      fputs( "\\vspace{0.2cm}\n", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }







            /////////////////////////////////////////
            /////////////////////////////////////////
            /////////////////////////////////////////
            if ( foundcode == 0 )   // !doc or !unidoc   <- auto document   
            if (( ( fetchline[0] == '!' ) 
            && ( fetchline[1] == 'd' )
            && ( fetchline[2] == 'o' )
            && ( fetchline[3] == 'c' )
	    && ( strcmp( fetchcmdline, "!doc" ) == 0 )) 
            || ( ( fetchline[0] == '!' ) 
            && ( fetchline[1] == 'u' )
            && ( fetchline[2] == 'n' )
            && ( fetchline[3] == 'i' )
            && ( fetchline[4] == 'd' )
	    && ( strcmp( fetchcmdline, "!unidoc" ) == 0 )) )
            {
                 fputs( "\n" , fp5 );
                 fputs( "\\documentclass[11pt]{article}\n", fp5 );
 	         fputs( "\\usepackage{grffile}\n" , fp5 );
 	         fputs( "\\usepackage{graphicx}\n" , fp5 );
 	         fputs( "\\usepackage{xcolor}\n" , fp5 );
                 fputs( "\\usepackage[utf8]{inputenc}\n", fp5 );
                 fputs( "\\begin{document}\n" , fp5 );
                 fputs( "\n" , fp5 );
  	         foundcode = 1;
            }

            /////////////////////////////////////////
            if ( foundcode == 0 )   // !unibook   <- auto document   
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'u' )
            if ( fetchline[2] == 'n' )
            if ( fetchline[3] == 'i' )
            if ( fetchline[4] == 'b' )
	    if ( strcmp( fetchcmdline, "!unibook" ) == 0 )
            {
                 fputs( "\n" , fp5 );
                 fputs( "\\documentclass[11pt]{book}\n", fp5 );
 	         fputs( "\\usepackage{grffile}\n" , fp5 );
 	         fputs( "\\usepackage{graphicx}\n" , fp5 );
 	         fputs( "\\usepackage{xcolor}\n" , fp5 );
                 fputs( "\\begin{document}\n" , fp5 );
                 fputs( "\n" , fp5 );
  	         foundcode = 1;
            }





            /////////////////////////////////////////
            /////////////////////////////////////////
            /////////////////////////////////////////
            if ( foundcode == 0 )   // !epstopdf  
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'e' )
            if ( fetchline[2] == 'p' )
            if ( fetchline[3] == 's' )
            if ( fetchline[4] == 't' )
            if ( fetchline[5] == 'o' )
            if ( fetchline[6] == 'p' )
            if ( fetchline[7] == 'd' )
            if ( fetchline[8] == 'f' )
            {
 	      fputs( "\n" , fp5 );
 	      fputs( "\\usepackage{epstopdf}\n" , fp5 );
 	      fputs( "\n" , fp5 );
  	      foundcode = 1;
            }


            ////////////////////////////////////////////
            if ( foundcode == 0 )   // !placeins  or !easy (same)
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'e' )
            if ( fetchline[2] == 'a' )
            if ( fetchline[3] == 's' )
            if ( fetchline[4] == 'y' )
            {
 	      fputs( "\n" , fp5 );
 	      fputs( "\\usepackage[section]{placeins}\n" , fp5 ); // new 
 	      fputs( "\n" , fp5 );
  	      foundcode = 1;
            }






            /////////////////////////////////
            /////////////////////////////////
            if ( foundcode == 0 )   // !space 
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 's' )
            if ( fetchline[2] == 'p' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'c' )
            if ( fetchline[5] == 'e' )
            {
 	      fputs( "\n" , fp5 );
              //fputs( "//\\renewcommand{\\baselinestretch}{1.5}\n", fp5);
              //fputs( "%//\\setlength{\\parindent}{10ex}\n", fp5);
              fputs( "\\setlength{\\parskip}{10pt plus 1pt minus 1pt}\n" , fp5 );
 	      fputs( "\n" , fp5 );
  	      foundcode = 1;
            }



            if ( foundcode == 0 )   // !tikz 
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 't' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'k' )
            if ( fetchline[4] == 'z' )
            {
 	      fputs( "\n" , fp5 );
 	      fputs( "\\usepackage{tikz}\n" , fp5 );
 	      fputs( "\n" , fp5 );
  	      foundcode = 1;
            }





            //////////////////////////////////
            //// !pdfpage
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) // for left at 0 char pos
            if ( fetchline[1] == 'p' )
            if ( fetchline[2] == 'd' )
            if ( fetchline[3] == 'f' )
            if ( fetchline[4] == 'p' )
            if ( fetchline[5] == 'a' )
            if ( fetchline[6] == 'g' )
            if ( fetchline[7] == 'e' )
            ////
            {
 	      fputs( "\n" , fp5 );
 	      fputs( "\\usepackage{pdfpages}\n" , fp5 );
 	      fputs( "\n" , fp5 );
  	      foundcode = 1;
            }




            ///////////// !pdf for pdfpages
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'p' )
            if ( fetchline[2] == 'd' )
            if ( fetchline[3] == 'f' )
            if ( fetchline[4] == '{' )
            {
  	      fputs( "\\includepdf{", fp5 );
 	      fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }


            //////////////////////////////////
            //// !pdfpage 
            if ( foundcode  == 0 )
            if ( fetchline[0] == '!' ) // for left at 0 char pos
            if ( fetchline[1] == 'p' )
            if ( fetchline[2] == 'd' )
            if ( fetchline[3] == 'f' )
            if ( fetchline[4] == 'p' )
            if ( fetchline[5] == 'a' )
            if ( fetchline[6] == 'g' )
            if ( fetchline[7] == 'e' )
            {
       	         fputs( "\\usepackage{pdfpages}\n" , fp5 );
  	         foundcode = 1;
            } 


            //////////////////////////////////
            //// !package !packages 
            if ( foundcode  == 0 )
            if ( fetchline[0] == '!' ) // for left at 0 char pos
            if ( fetchline[1] == 'p' )
            if ( fetchline[2] == 'a' )
            if ( fetchline[3] == 'c' )
            if ( fetchline[4] == 'k' )
            if ( fetchline[5] == 'a' )
            if ( fetchline[6] == 'g' )
            if ( fetchline[7] == 'e' )
            ////
            {
              if ( beamercode != 1 ) 
              { // necessary for bullets of beamer!, ideally !beamer !packages 
       	         fputs( "\n" , fp5 );
       	         fputs( "\\usepackage{url}\n" , fp5 );
       	         fputs( "\\usepackage{hyperref}\n" , fp5 );
       	         fputs( "\\usepackage{graphicx}\n" , fp5 );
       	         fputs( "\\usepackage{grffile}\n" , fp5 );
       	         fputs( "\\usepackage{pdfpages}\n" , fp5 );
       	         //fputs( "\\usepackage{gensymb}\n" , fp5 ); //for symbols
       	         fputs( "\\usepackage{wallpaper}\n" , fp5 );
       	         fputs( "\n" , fp5 );
       	         fputs( "\\usepackage{epstopdf}\n" , fp5 );
       	         fputs( "\\usepackage{enumitem}\n" , fp5 );  // for >> and items 
       	         fputs( "\n" , fp5 );
       	         fputs( "\\usepackage{soul}\n" , fp5 );      // for reviews
       	         fputs( "\n" , fp5 );
                 fputs( "\\sloppy\n", fp5 );
                 fputs( "\\usepackage[none]{hyphenat}\n", fp5 );
       	         fputs( "\n" , fp5 );
                 fputs( "\\usepackage[utf8]{inputenc}", fp5 );
       	         fputs( "\n" , fp5 );
              }
  	      foundcode = 1;
            } 





            //////////////////////////////////
            //// !utf8 or !utf (utf8 is today working)
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
	    if ( strcmp( fetchcmdline, "!utf8" ) == 0 )
            {
  	      fputs( "\n", fp5 );
              fputs( "\\usepackage[utf8]{inputenc}\n", fp5 );
  	      fputs( "\n", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            } 
            //// !utf
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
	    if ( strcmp( fetchcmdline, "!utf" ) == 0 )
            {
              fputs( "\\usepackage[utf8]{inputenc}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            } 







            /////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'c' )
            if ( fetchline[2] == 'h' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'r' )
            if ( fetchline[5] == 'b' )
            if ( fetchline[6] == 'c' )
            if ( fetchline[7] == ' ' )
            {
 	        strncpy( foostr, strcut( fetchline, 7+2, strlen(fetchline)) , PATH_MAX );
 	        //strncpy( foostr, fetchline, PATH_MAX );
                foodo = snprintf( fooccharo, 50 , "%d", strcharcount( foostr ) );
 	        fputs( fooccharo , fp5 );
                fputs( ": "   , fp5 );
 	        fputs( foostr , fp5 );
 	        fputs( "\n"   , fp5 );
  	        foundcode = 1;
            } 




            //////////////////////////////////////////////
            // !textline  with a lf
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 't' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'x' )
            if ( fetchline[4] == 't' )
            if ( fetchline[5] == 'l' )
            if ( fetchline[6] == 'i' )
            if ( fetchline[7] == 'n' )
            if ( fetchline[8] == 'e' )
            if ( fetchline[9] == ' ' )
            {
       	         fputs( strtxt2tex(strcut( fetchline, 9+2, strlen(fetchline))) , fp5 );
  	         fputs( "\\", fp5 );
  	         fputs( "\\", fp5 );
  	         fputs( "\n", fp5 );
  	         foundcode = 1;
            }

            //////////////////////////////////////////////
            // !rawline  with a lf
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'r' )
            if ( fetchline[2] == 'a' )
            if ( fetchline[3] == 'w' )
            if ( fetchline[4] == 'l' )
            if ( fetchline[5] == 'i' )
            if ( fetchline[6] == 'n' )
            if ( fetchline[7] == 'e' )
            if ( fetchline[8] == ' ' )
            {
       	         fputs( strtxt2tex(strcut( fetchline, 8+2, strlen(fetchline))) , fp5 );
  	         fputs( "\\", fp5 );
  	         fputs( "\\", fp5 );
  	         fputs( "\n", fp5 );
  	         foundcode = 1;
            }
            //////////////////////////////////////////////
            // !raw   my text for a given line 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'r' )
            if ( fetchline[2] == 'a' )
            if ( fetchline[3] == 'w' )
            if ( fetchline[4] == ' ' )
            {
       	         fputs( strtxt2tex(strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
        	 fputs( "\n", fp5 );
  	         foundcode = 1;
            }
            //////////////////////////////////////////////





            ////// !beginraw 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'b' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == 'i' )
            if ( fetchline[5] == 'n' )
            if ( fetchline[6] == 'r' )
            if ( fetchline[7] == 'a' )
            if ( fetchline[8] == 'w' )
            if ( fetchline[9] == ' ' )
            {
	      txtrawcode = 1;
  	      foundcode = 1;
            }
            // !endraw
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'e' )
            if ( fetchline[2] == 'n' )
            if ( fetchline[3] == 'd' )
            if ( fetchline[4] == 'r' )
            if ( fetchline[5] == 'a' )
            if ( fetchline[6] == 'w' )
            {
	      txtrawcode = 0;
  	      foundcode = 1;
            }







            // !input 
            if ( foundcode == 0 )
	    if ( 
            ( (  fetchline[0] ==  '!' ) || (  fetchline[0] ==  '#' ) )
            && ( fetchline[1] == 'i' )
            && ( fetchline[2] == 'n' )
            && ( fetchline[3] == 'p' )
            && ( fetchline[4] == 'u' )
            && ( fetchline[5] == 't' )
            && ( fetchline[6] == '{' )
	    )
            {
              fclose( fp5 );                 /// writer
              long saved = ftell( fp6 );     /// reader
              fclose( fp6 );                 /// reader

              printf( ">COMMAND INPUT<\n"); 
	      char fileinputsrc[PATH_MAX];
	      strncpy( fileinputsrc,  myinputspath , PATH_MAX );
              strncat( fileinputsrc , strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX - strlen( fileinputsrc ) -1 );
              printf( ">COMMAND INPUT Src %s<\n", fileinputsrc ); 

	        if ( fileinputsrc[0] == '~' )
	        {
	         strncpy( fileinputsrc, "" , PATH_MAX );
                 strncat( fileinputsrc , getenv( "HOME" ) , PATH_MAX - strlen( fileinputsrc ) -1 );
                 strncat( fileinputsrc , strdelimit( fetchline,  '~' ,'}' ,  1 ) , PATH_MAX - strlen( fileinputsrc ) -1 );
	        }
                //filetexiex( fileout, strdelimit( fetchline,  '{' ,'}' ,  1 ) );
		printf( "!input{%s} (fexist:%d) (...)\n", fileinputsrc , fexist( fileinputsrc ) );
                printf( "> READ FROM IPATH: %s\n", myinputspath );
                nfileunimark( fileout, fileinputsrc );
              fp5 = fopen( fileout , "ab+"); /// writer
              fp6 = fopen( filein , "r");  /// reader
              fseek( fp6 , saved , SEEK_SET); // reader 
  	      foundcode = 1;
            }










            ////////////////// 
            //// #include{file.mrk}  or !include{doc.mrk}
            //// it will load the content from the ~/include/mrk/ 
            //////////////////
            if ( foundcode == 0 )
	    if ( ( (  fetchline[0] ==  '!' ) || (  fetchline[0] ==  '#' ) )
            && ( fetchline[1] == 'i' )
            && ( fetchline[2] == 'n' )
            && ( fetchline[3] == 'c' )
            && ( fetchline[4] == 'l' )
            && ( fetchline[5] == 'u' )
            && ( fetchline[6] == 'd' )
            && ( fetchline[7] == 'e' )
            && ( fetchline[8] == '{' )
	    )
            {
              fclose( fp5 );                 /// writer
              long saved = ftell( fp6 );     /// reader
              fclose( fp6 );                 /// reader

	      char fileinputsrc[PATH_MAX];
	      strncpy( fileinputsrc, "", PATH_MAX );
              strncat( fileinputsrc , getenv( "HOME" ) , PATH_MAX - strlen( fileinputsrc ) -1 );
              strncat( fileinputsrc , "/include/mrk/" , PATH_MAX - strlen( fileinputsrc ) -1 );
              strncat( fileinputsrc , strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX - strlen( fileinputsrc ) -1 );
 	      printf( " =>!include{%s} (fexist:%d) (...)\n", fileinputsrc , fexist( fileinputsrc ) );
              nfileunimark( fileout, fileinputsrc );
              fp5 = fopen( fileout , "ab+"); /// writer
              fp6 = fopen( filein , "r");  /// reader
              fseek( fp6 , saved , SEEK_SET); // reader 
  	      foundcode = 1;
            }



	    /////////////////
            ////////////////
            //// !begin here
            ////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
	    if ( strcmp( fetchcmdline, "!begin" ) == 0 )  // !begin
            {
	      contentcode = 1;
 	      fputs( "\\begin{document}" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }

            ////////////////
            ////////////////
            //// !enddoc here
            ////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
	    if ( strcmp( fetchcmdline, "!enddoc" ) == 0 )
            {
	      contentcode = 0;
 	      fputs( "\\end{document}" , fp5 );
  	      fputs( "\n", fp5 );
              fp_content_eof = 1;
  	      foundcode = 1;
            }

            ////////////////
            ////////////////
            //// !end here, to be compatible with rose (2019)
            ////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
	    if ( strcmp( fetchcmdline, "!end" ) == 0 )
            {
	      contentcode = 0;
 	      fputs( "\\end{document}" , fp5 );
  	      fputs( "\n", fp5 );
              //fp_content_eof = 1;
  	      foundcode = 1;
            }




            ////////////////
            //// !EOF here
            ////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
	    if ( strcmp( fetchcmdline, "!EOF" ) == 0 )
            {
              fp_content_eof = 1;
	      contentcode = 0;
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }





            //// !book  (content)      // !book start a book
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'b' )
            if ( fetchline[2] == 'o' )
            if ( fetchline[3] == 'o' )
            if ( fetchline[4] == 'k' )
            {
	      bookcode = 1;
              //\documentclass{book}
              fputs( "\\documentclass[11pt]{book}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }



            //// !beamer      !beamer activate beamer 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'b' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'm' )
            if ( fetchline[5] == 'e' )
            if ( fetchline[6] == 'r' )
            {
	      beamercode = 1;
  	      foundcode = 1;
            }
            //// !:set beamer=0
            /*
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == ':' )
            if ( fetchline[2] == 'b' )
            if ( fetchline[3] == 'e' )
            if ( fetchline[4] == 'a' )
            if ( fetchline[5] == 'm' )
            if ( fetchline[6] == 'e' )
            if ( fetchline[7] == 'r' )
            if ( fetchline[8] == '=' )
            if ( fetchline[9] == '0' )
            {
	      beamercode = 0;
  	      foundcode = 1;
            }
            */

            //// !noter
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'n' )
            if ( fetchline[2] == 'o' )
            if ( fetchline[3] == 't' )
            if ( fetchline[4] == 'e' )
            if ( fetchline[5] == 'r' )
            {
	      notercode = 1;
  	      foundcode = 1;
            }
            ///////////////////////////////////
            //// !bhtml
            if ( foundcode == 0 )  //!bhtml 
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'b' )
            if ( fetchline[2] == 'h' )
            if ( fetchline[3] == 't' )
            if ( fetchline[4] == 'm' )
            if ( fetchline[5] == 'l' )
            {
              markup_output_format = 2;
  	      foundcode = 1;
            } 

            //// !html
            // 20180101-105417
            if ( foundcode == 0 )  //!html for simplicity
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'h' )
            if ( fetchline[2] == 't' )
            if ( fetchline[3] == 'm' )
            if ( fetchline[4] == 'l' )
            {
              markup_output_format = 2;
  	      foundcode = 1;
            } 


            /////////////////////////////////
            /////////////////////////////////
            /////////////////////////////////
            /////////////////////////////////
            //// !format 10 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'o' )
            if ( fetchline[3] == 'r' )
            if ( fetchline[4] == 'm' )
            if ( fetchline[5] == 'a' )
            if ( fetchline[6] == 't' )
            if ( fetchline[7] == ' ' )
            if ( fetchline[8] == '1' )
            if ( fetchline[9] == '0' )
            {
              // !format 10   
              // !format 10  will enable it.
              markup_output_format = 10;   //format 10
  	      foundcode = 1;
            } 
            /////////////////////////////////
            if ( foundcode == 0 )
            if ( markup_output_format == 10 ) 
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'q' ) 
            if ( fetchline[2] == 'u' )
            if ( fetchline[3] == ' ' )
            {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item[{\\bfseries {" , fp5 );
                fooc = snprintf( fooccharo, 50 , "%d", question_qucounter );
 	        fputs( fooccharo  , fp5 );
 	        fputs( "}}.)]{" , fp5 );
                question_qucounter++;
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
                fputs( " (3 Pkt.)}" , fp5 );
 	        fputs( "\n" , fp5 );
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\vspace{4cm}\n" , fp5 );  // the trick is to have 4cm to allow answers.
  	        foundcode = 1;
            }
            /////////////////////////////////
            /////////////////////////////////
            /////////////////////////////////
            /////////////////////////////////
            //// !format 10 end
            /////////////////////////////////
            /////////////////////////////////
            /////////////////////////////////









            //// !exam list   or !exam-list 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'e' )
            if ( fetchline[2] == 'x' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'm' )
            if (( fetchline[5] == ' ' ) || ( fetchline[5] == '-' ))
            if ( fetchline[6] == 'l' )
            if ( fetchline[7] == 'i' )
            if ( fetchline[8] == 's' )
            if ( fetchline[9] == 't' )
            {
              markup_output_format = 8;
  	      foundcode = 1;
            } 


            //////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'e' )
            if ( fetchline[2] == 'x' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'm' )
            if ( fetchline[5] == 'l' )
            if ( fetchline[6] == 'i' )
            if ( fetchline[7] == 's' )
            if ( fetchline[8] == 't' )
            {
              markup_output_format = 8;
  	      foundcode = 1;
            } 


            //// !exam+  //
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'e' )
            if ( fetchline[2] == 'x' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'm' )
            if ( fetchline[5] == '+' )
            {
              markup_output_format = 6;
  	      foundcode = 1;
            } 

            //// !exam  for en exams !!
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'e' )
            if ( fetchline[2] == 'x' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'm' )
            {
              markup_output_format = 3;
  	      foundcode = 1;
            } 




            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 's' )
            if ( fetchline[2] == 't' )
            if ( fetchline[3] == 'b' )
            if ( fetchline[4] == 'o' )
            if ( fetchline[5] == 'o' )
            if ( fetchline[6] == 'k' )
            {
              // !stbook
              markup_output_format = 11 ;
  	      foundcode = 1; //just skip
            }

            ///////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '>' )
            if ( fetchline[1] == ' ' )
            if ( markup_output_format == 11 ) //!bookcode
            {
 	      strncpy( cursubsection,  strtrim( strcut( fetchline, 1+2, strlen(fetchline))) ,  PATH_MAX );
              if ( strcmp( cursubsection, lastsubsection ) != 0 ) 
              {
 	          fputs( "\%subsection320\n" , fp5 );
 	          fputs( "\\subsection{" , fp5 );
 	          fputs( cursubsection ,  fp5 );
 	          fputs( "}\n" , fp5 );
                  strncpy( lastsubsection, "", PATH_MAX );
              }
  	      foundcode = 1;
            } 

            ///////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '-' )
            if ( fetchline[1] == ' ' )
            if ( markup_output_format == 11 ) //!bookcode
            {
 	      fputs( strcut( fetchline, 1+2, strlen(fetchline)) , fp5 );
 	      fputs( "\n" , fp5 );
  	      foundcode = 1;
            } 



   // !fild
   ////////////////////////////////////
   // beamer starts here 
   // beamer here
   // BEAMER HERE
   // it might work too with != 
   ////////////////////////////////////
   if ( foundcode == 0 )
   if ( beamercode == 1 )
   {
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'l' )
            if ( fetchline[4] == 'd' )
            if ( fetchline[5] == ' ' )
            {
                fputs( "\\begin{frame}\n" , fp5);
                fputs( "\\fitimage{\n" , fp5);
                fputs( "\\frametitle{\\thesection.~\\insertsection}\n" , fp5 );
                fputs( "\\begin{itemize}\n", fp5 );
                fputs( "\\item ", fp5 );
     	        fputs( strtxt2tex( strtrim( strcut( fetchline, 5+2, strlen( fetchline )))) , fp5 );
                fputs( "\n", fp5 );
                fputs( "\\end{itemize}\n", fp5 );
                fputs( "}{", fp5 );
     	        fputs( strtrim( strcut( fetchline, 5+2, strlen( fetchline ))) , fp5 );
                fputs( "}[]\n" , fp5 );
                fputs( "\\end{frame}\n" , fp5 );
  	        foundcode = 1;
            }
   }


   ////////////////////////////////////
   // beamer starts here 
   ////////////////////////////////////
   if ( foundcode == 0 )
   if ( beamercode == 1 )
   {
	    // opener !clr
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( ( fetchline[0] == '!' ) || ( fetchline[0] == '$' ) )
            if ( fetchline[1] == 'c' )
            if ( fetchline[2] == 'l' )
            if ( fetchline[3] == 'r' )
            {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{frame}" , PATH_MAX );
	      beamerlevel = 0;
  	      foundcode = 1;
            }


            if ( foundcode == 0 ) // !toc Inhalt
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 't' )
            if ( fetchline[2] == 'o' )
            if ( fetchline[3] == 'c' )
            if ( fetchline[4] == ' ' )
            {
 	      fputs( "\\clearpage\n" , fp5 );
 	      fputs( "\\begin{frame}\n" , fp5 );
 	      fputs( "\\frametitle{" , fp5 );
 	      fputs( strtrim( strcut( fetchline, 4+2, strlen( fetchline ))) , fp5 );
  	      fputs( "}\n", fp5 );
 	      fputs( "\\tableofcontents\n" , fp5 );
 	      fputs( "\\end{frame}\n" , fp5 );
 	      fputs( "\n" , fp5 );
 	      fputs( "\n" , fp5 );
	      beamerlevel = 0;
  	      foundcode = 1;
            }


            if ( foundcode == 0 ) // !frame 
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'r' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'm' )
            if ( fetchline[5] == 'e' )
            if ( fetchline[6] == ' ' )
	    if ( beamercode == 1 ) 
            {
              foxy++;
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\frametitle{"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 6+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , "}"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
  	      foundcode = 1;
            }


            ////////////////////
            // 20170930-175843 this is the default, without auto section numbering
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'h' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'd' )
            if ( fetchline[5] == 'e' )
            if ( fetchline[6] == 'r' )
            if ( fetchline[7] == ' ' )
	    if ( beamercode == 1 ) 
            {
              foxy++;
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\frametitle{"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 7+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , "}"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
  	      foundcode = 1;
            }

            ////////////////////
            // this is the short !header default
            /*
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'h' )
            if ( fetchline[2] == 'd' )
            if ( fetchline[3] == ' ' )
	    if ( beamercode == 1 ) 
            {
              foxy++;
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\frametitle{"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , "}"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
  	      foundcode = 1;
            } */





            if ( foundcode == 0 )
	    if ( beamercode == 1 ) 
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'n' )
            if ( fetchline[2] == 'o' )
            if ( fetchline[3] == 't' )
            if ( fetchline[4] == 'e' )
            if ( fetchline[5] == ' ' )
            {
              foxy++;
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\note{"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 5+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , "}"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
  	      foundcode = 1;
            }



            ////////////////////
            //// this is beamer
            ////////////////////
            // 20170930-175843 this is the advanced, with auto section numbering
            /*
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) // !sec sectionname for beamer
            if ( fetchline[1] == 's' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'c' )
            if ( fetchline[4] == ' ' )
	    if ( beamercode == 1 ) 
            {
              strncpy( slidemysection, strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , PATH_MAX );
  	      foundcode = 1;
            }
            */

            /////////////////////////////
            /// for beamer 
            /////////////////////////////
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchline[0] == '>' )
            if ( fetchline[1] == ' ' )
            {
	      if ( beamerlevel == 1)  
	      {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
	      }
	      else if ( beamerlevel == 2)  
	      {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\end{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
	        beamerlevel = 1;
	      }
	      else if ( beamerlevel == 0)  
	      {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
	        beamerlevel = 1;
	      }
  	      foundcode = 1;
            }





            //////////////////////////////////
            // for beamer 
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchline[0] == '*' )
            if ( fetchline[1] == ' ' )
            {
	      if ( beamerlevel == 2)  
	      {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
		beamerlevel = 3;
	      }
	      else if ( beamerlevel == 3)  
	      {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
	        beamerlevel = 3;
	      }
	      else if ( beamerlevel == 1)  
	      {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////

	        beamerlevel = 3;
	      }
	      else if ( beamerlevel == 0)  
	      {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
	        beamerlevel = 3;
	      }
  	      foundcode = 1;
            }





            /// the regular point
            /// beamer  for txt to tex !
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if (   (( fetchline[0] == '-' ) && ( fetchline[1] == ' ' ))
            || (( fetchline[0] == '|' )  && ( fetchline[1] == '-' ) && ( fetchline[2] == ' ' )) )   // |- blabla
            {
	     if ( beamerlevel == 1)  
	     {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 1+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////

		beamerlevel = 2;
	      }
	      else if ( beamerlevel == 2)  
	      {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  strampersand2txt( fetchline ) , PATH_MAX);
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 1+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////

	        beamerlevel = 2;
	      }
	      else if ( beamerlevel == 3)  
	      {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\end{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  strampersand2txt( fetchline ) , PATH_MAX);
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 1+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
	        beamerlevel = 2;
	      }
	      else if ( beamerlevel == 0)  
	      {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  strampersand2txt( fetchline ) , PATH_MAX);
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 1+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
	        beamerlevel = 2;
	      }
  	      foundcode = 1;
            }





            // !info bla for beamer
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
	    if ( beamerlevel >= 1 )
            if ( fetchline[0] == '!' ) // !info without {} is for one
            if ( fetchline[1] == 'i' )
            if ( fetchline[2] == 'n' )
            if ( fetchline[3] == 'f' )
            if ( fetchline[4] == 'o' )
            if ( fetchline[5] == ' ' )
            {
              ///////////////
              //strncpy( slidebufferfignote,  strtrim( strcut( fetchline, 5+2, strlen( fetchline ))) , PATH_MAX );
 	      //fputs( "\\textit{" , fp5 );
              //strncpy( slidebufferfignote, "\\textit{"  , PATH_MAX );
              strncpy( slidebufferfignote, "\\begin{center}\\textit{"  , PATH_MAX );
              strncat( slidebufferfignote , 
              strtrim( strcut( fetchline, 5+2, strlen( fetchline ))) 
              , PATH_MAX - strlen( slidebufferfignote  ) -1 );
              //strncat( slidebufferfignote , "}" , PATH_MAX - strlen( slidebufferfignote  ) -1 );
              strncat( slidebufferfignote , "}\\end{center}" , PATH_MAX - strlen( slidebufferfignote  ) -1 );
              slidebuffernot = 1;
  	      foundcode = 1;
            }






            // !info{ for beamer, this will be !infoab implicit, !info with {} {} is for two right and left
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
	    if ( beamerlevel >= 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'i' )
            if ( fetchline[2] == 'n' )
            if ( fetchline[3] == 'f' )
            if ( fetchline[4] == 'o' )
            if ( fetchline[5] == '{' )
            {
              if ( markup_language == 3 )
              {
                ///////////////
                strncpy( slidebufferfignote,  ""  , PATH_MAX );
                strncat( slidebufferfignote , "\\begin{center}{\\small " , PATH_MAX - strlen( slidebufferfignote  ) -1 );
                strncat( slidebufferfignote , fbasenoext( strtxt2tex( strdelimit( fetchline,  '{' ,'}' ,  1 ) )) , PATH_MAX - strlen( slidebufferfignote  ) -1 );
  
                strncat( slidebufferfignote , " (Links) und " , PATH_MAX - strlen( slidebufferfignote  ) -1 );
  
                strncat( slidebufferfignote , fbasenoext(strtxt2tex( strdelimit( fetchline,  '{' ,'}' ,  2 ) ) ) , PATH_MAX - strlen( slidebufferfignote  ) -1 );
  
                strncat( slidebufferfignote , " (Rechts)" , PATH_MAX - strlen( slidebufferfignote  ) -1 );
                strncat( slidebufferfignote , "}\\end{center}" , PATH_MAX - strlen( slidebufferfignote  ) -1 );
                slidebuffernot = 1;
              }
              else
              {
                ///////////////
                strncpy( slidebufferfignote,  ""  , PATH_MAX );
                strncat( slidebufferfignote , "\\begin{center}{\\small " , PATH_MAX - strlen( slidebufferfignote  ) -1 );
                //strncat( slidebufferfignote , "{\\small " , PATH_MAX - strlen( slidebufferfignote  ) -1 );
                strncat( slidebufferfignote , fbasenoext( strtxt2tex( strdelimit( fetchline,  '{' ,'}' ,  1 ) )) , PATH_MAX - strlen( slidebufferfignote  ) -1 );
  
                strncat( slidebufferfignote , " (Left) and " , PATH_MAX - strlen( slidebufferfignote  ) -1 );
  
                strncat( slidebufferfignote , fbasenoext(strtxt2tex( strdelimit( fetchline,  '{' ,'}' ,  2 ) ) ) , PATH_MAX - strlen( slidebufferfignote  ) -1 );
  
                strncat( slidebufferfignote , " (Right)" , PATH_MAX - strlen( slidebufferfignote  ) -1 );
                //strncat( slidebufferfignote , "}" , PATH_MAX - strlen( slidebufferfignote  ) -1 );
                strncat( slidebufferfignote , "}\\end{center}" , PATH_MAX - strlen( slidebufferfignote  ) -1 );
                slidebuffernot = 1;
              }
  	      foundcode = 1;
            }







            // !fig for beamer (i.e. !fig{pic.png})
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == '{' )
            {
              slidebufferfig = 1; 
              ///////////////
              //char foofilefound[PATH_MAX];
              //strncpy( foofilefound, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
              //if ( strstr( foofilefound , "/" ) != 0 ) 
              //if ( fexist( foofilefound ) == 1 ) 
              //{
              //}
	      if ( beamerlevel >= 1 )
              {
                  // if you wanna give regular slides
                  strncpy( slidebufferfigfile, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
              }
	      else if ( strcount( fetchline, '}' ) >= 2 )
	      {
                  // or, new automatic, allows to build faster slides
                  strncpy( slidebufferfigfile, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
                  // after pic, bring text
                  foxy++;//
                  strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );

                  foxy++;//
                  strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
                  strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );

                  strncat( slidebufferdata[foxy] , strdelimit( fetchline,  '{' ,'}' ,  2 )   , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
                  strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
                  //foxy++;//
                  //strncpy( slidebufferdata[foxy] , "\\end{itemize}" , PATH_MAX );
                  //beamerlevel = 0;
                  beamerlevel = 1;
              }
              else 
              {
                  // in any other choices
                  strncpy( slidebufferfigfile, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
              }
              ////////////////
  	      foundcode = 1;
              slidebufferfig = 1; 
            }






            // !figab for beamer
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
	    if ( beamerlevel >= 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == 'a' )
            if ( fetchline[5] == 'b' )
            if ( fetchline[6] == '{' )
            {
              ///////////////
              char foofilefound[PATH_MAX];
              strncpy( foofilefound, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
              if ( strstr( foofilefound , "/" ) != 0 ) 
              {
              }
              strncpy( slidebufferfigfile1 , strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
              strncpy( slidebufferfigfile2 , strdelimit( fetchline,  '{' ,'}' ,  2 ) , PATH_MAX );
  	      foundcode = 1;
              slidebufferfig = 2; 
            }





            // !figat for beamer, image and text
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
	    if ( beamerlevel >= 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == 'a' )
            if ( fetchline[5] == 't' )
            if ( fetchline[6] == '{' )
            {
              ///////////////
              char foofilefound[PATH_MAX];
              strncpy( foofilefound, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
              if ( strstr( foofilefound , "/" ) != 0 ) 
              {
              }
              strncpy( slidebufferfigfile1 , strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
              strncpy( slidebufferfigfile2 , strdelimit( fetchline,  '{' ,'}' ,  2 ) , PATH_MAX );
  	      foundcode = 1;
              slidebufferfig = 3; 
            }


            // !textfig for beamer
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
	    if ( beamerlevel >= 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 't' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'x' )
            if ( fetchline[4] == 't' )
            if ( fetchline[5] == 'f' )
            if ( fetchline[6] == 'i' )
            if ( fetchline[7] == 'g' )
            if ( fetchline[8] == '{' )
            {
              slidebufferfig = 1; 
              ///////////////
              char foofilefound[PATH_MAX];
              strncpy( foofilefound, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
              if ( strstr( foofilefound , "/" ) != 0 ) 
              if ( fexist( foofilefound ) == 1 ) 
              {
              }
              strncpy( slidebufferfigfile, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
  	      foundcode = 1;
              slidebufferfig = 4; 
            }






            // !maketitle
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'm' )
            if ( fetchcmdline[2] == 'a' )
            if ( fetchcmdline[3] == 'k' )
            if ( fetchcmdline[4] == 'e' )
            if ( fetchcmdline[5] == 't' )
            if ( fetchcmdline[6] == 'i' )
            if ( fetchcmdline[7] == 't' )
            if ( fetchcmdline[8] == 'l' )
            if ( fetchcmdline[9] == 'e' )
            {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\begin{frame}\\maketitle\\end{frame}"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
  	      foundcode = 1;
            }




            // !box
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'b' )
            if ( fetchline[2] == 'o' )
            if ( fetchline[3] == 'x' )
            if ( fetchline[4] == ' ' )
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            {
	     if ( beamerlevel == 1)  
	     {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item \\boxed{"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 4+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , "}"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////

		beamerlevel = 2;
	      }

	      else if ( beamerlevel == 2)  
	      {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item \\boxed{"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  strampersand2txt( fetchline ) , PATH_MAX);
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 4+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , "}"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////

	        beamerlevel = 2;
	      }
	      else if ( beamerlevel == 3)  
	      {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\end{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item \\boxed{"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  strampersand2txt( fetchline ) , PATH_MAX);
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 4+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , "}"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
	        beamerlevel = 2;
	      }
	      else if ( beamerlevel == 0)  
	      {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item \\boxed{"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  strampersand2txt( fetchline ) , PATH_MAX);
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 4+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , "}"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
	        beamerlevel = 2;
	      }
  	      foundcode = 1;
            }
           // end of !box 





            // !equ 
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'e' )
            if ( fetchline[2] == 'q' )
            if ( fetchline[3] == 'u' )
            if ( fetchline[4] == ' ' )
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            {
	     if ( beamerlevel == 1)  
	     {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 4+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////

		beamerlevel = 2;
	      }
	      else if ( beamerlevel == 2)  
	      {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  strampersand2txt( fetchline ) , PATH_MAX);
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 4+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////

	        beamerlevel = 2;
	      }
	      else if ( beamerlevel == 3)  
	      {
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\end{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  strampersand2txt( fetchline ) , PATH_MAX);
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 4+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
	        beamerlevel = 2;
	      }
	      else if ( beamerlevel == 0)  
	      {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "\\begin{itemize}" , PATH_MAX );
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\item "  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              /////////////////////////////////
              char goofetchline[PATH_MAX];
              strncpy( goofetchline,  strampersand2txt( fetchline ) , PATH_MAX);
              strncpy( goofetchline,  fetchline  , PATH_MAX);
              if ( fetchline[0] == '|' )  
                 strncat( slidebufferdata[foxy] , strtxt2tex( strtrim( strcut( goofetchline, 4+2, strlen( goofetchline )))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              else
                 strncat( slidebufferdata[foxy] , strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              strncat( slidebufferdata[foxy] , ""  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
	        beamerlevel = 2;
	      }
  	      foundcode = 1;
            }
           // end of !equ 





            // !equ  (beamer)
            /*
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'e' )
            if ( fetchline[2] == 'q' )
            if ( fetchline[3] == 'u' )
            if ( fetchline[4] == ' ' )
            {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy], 
 	      strtrim( strcut( fetchline, 4+2, strlen( fetchline ))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
  	      foundcode = 1;
            }
            */


            // !raw  (beamer)
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'r' )
            if ( fetchline[2] == 'a' )
            if ( fetchline[3] == 'w' ) // new, beta testing
            if ( fetchline[4] == ' ' )
            {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy], 
 	      strtrim( strcut( fetchline, 4+2, strlen( fetchline ))) , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
  	      foundcode = 1;
            }



            // !linefeed (beamer)
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'l' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'n' )
            if ( fetchline[4] == 'e' )
            if ( fetchline[5] == 'f' )
            if ( fetchline[6] == 'e' )
            if ( fetchline[7] == 'e' )
            if ( fetchline[8] == 'd' )
            {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\bigskip"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
  	      foundcode = 1;
            }

            // !smallskip (beamer)
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchcmdline[0] == '!' ) 
            if ( fetchcmdline[1] == 's' )
            if ( fetchcmdline[2] == 'm' )
            if ( fetchcmdline[3] == 'a' )
            if ( fetchcmdline[4] == 'l' )
            if ( fetchcmdline[5] == 'l' )
            if ( fetchcmdline[6] == 's' )
            if ( fetchcmdline[7] == 'k' )
            if ( fetchcmdline[8] == 'i' )
            if ( fetchcmdline[9] == 'p' )
            {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\smallskip"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
  	      foundcode = 1;
            }



            // !medskip (beamer)
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchcmdline[0] == '!' ) 
            if ( fetchcmdline[1] == 'm' )
            if ( fetchcmdline[2] == 'e' )
            if ( fetchcmdline[3] == 'd' )
            if ( fetchcmdline[4] == 's' )
            if ( fetchcmdline[5] == 'k' )
            if ( fetchcmdline[6] == 'i' )
            if ( fetchcmdline[7] == 'p' )
            {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\medskip"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
  	      foundcode = 1;
            }


            // !bigskip (beamer)
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchcmdline[0] == '!' ) 
            if ( fetchcmdline[1] == 'b' )
            if ( fetchcmdline[2] == 'i' )
            if ( fetchcmdline[3] == 'g' )
            if ( fetchcmdline[4] == 's' )
            if ( fetchcmdline[5] == 'k' )
            if ( fetchcmdline[6] == 'i' )
            if ( fetchcmdline[7] == 'p' )
            {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\bigskip"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
  	      foundcode = 1;
            }


            //datecom-20180117-012026
            // !skip (beamer)
            /*
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchcmdline[0] == '!' ) 
            if ( fetchcmdline[1] == 's' )
            if ( fetchcmdline[2] == 'k' )
            if ( fetchcmdline[3] == 'i' )
            if ( fetchcmdline[4] == 'p' )
            {
              ///////////////
              foxy++;//
              strncpy( slidebufferdata[foxy] , "" , PATH_MAX );
              strncat( slidebufferdata[foxy] , "\\bigskip"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
              ///////////////
  	      foundcode = 1;
            }*/


            // process the content of the slide
	    // closer (beamer)
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
	    if ( beamerlevel >= 1 )
	    //if ( unilistemptyline >= 1 )
	    if ( unilistemptyline >= 1 )
            {
	      printf ( " Found unilistemptyline (closer): beamer level %d \n", beamerlevel );
	      for ( fooi = 1 ; fooi <= beamerlevel ; fooi++)
	      {
                 foxy++;//
                 strncpy( slidebufferdata[foxy] , "\\end{itemize}" , PATH_MAX );
	      }




              //////////////////////////////////////////////
              // let's begin
              //////////////////////////////////////////////
              slidebufferfoundsection = 0;
	      for ( fooi = 1 ; fooi <= foxy ; fooi++)
	      {
                   if ( strstr( slidebufferdata[ fooi ] , "\\frametitle" ) != 0 )
                   if ( slidebufferdata[ fooi ][0] != '%' ) 
                   {
                      printf( "a frametitle found.\n" );
                      slidebufferfoundsection = 1;
                   }
              }



              if ( strcmp( slidemysection , "" ) != 0 )
              {
                      fputs( "\n" , fp5 );
                      fputs( "\%slidemysectionslide\n" , fp5 );
                      fputs( "\\section{"  , fp5 );
                      fputs( slidemysection , fp5 );
                      fputs( "}\n"  , fp5 );
              }
             

              ////////////////////////////////////////////////
              /// loop according to line content
	      printf ( " Print out foxy \n" );
	      for ( fooi = 1 ; fooi <= foxy ; fooi++)
	      {
                 fputs( slidebufferdata[ fooi ] , fp5 );

                 if ( slidebufferfig >= 1 )
                 if ( strcmp( slidebufferdata[ fooi ] , "\\begin{frame}" ) == 0 ) //ok
                 {
                      fputs( "\n" , fp5 );
                      if ( slidebufferfig == 1 )
                         fputs( "\\fitimage{"  , fp5 );    // one pic
                      else if ( slidebufferfig == 2 )
                         fputs( "\\fitimageab{"  , fp5 );  // two pics
                      else if ( slidebufferfig == 3 )
                         fputs( "\\fitimageat{"  , fp5 );  // pic and text
                      else if ( slidebufferfig == 4 )
                         fputs( "\\textfig{"  , fp5 );  // pic and text
                 }


                 if ( slidebufferfoundsection == 0 ) 
                 {
                      fputs(  "\n" , fp5 );
                      fputs(  "\\frametitle{\\thesection.~\\insertsection}"  , fp5 );
                      slidebufferfoundsection = 1; 
                 }
                 fputs( "\n" , fp5 );
	      } // end of loop



              if ( slidebufferfig == 4 ) //textfig
              {
                foxy++;//
                fputs( "\\begin{minipage}{0.5\\textwidth}\n" , fp5 );
                strncpy( slidebufferdata[foxy] , "\\includegraphics[width=\\textwidth]{" , PATH_MAX );
                strncat( slidebufferdata[foxy] ,  slidebufferfigfile  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 ); 
                strncat( slidebufferdata[foxy] , "}"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
                fooi = foxy ; 
                fputs( slidebufferdata[ fooi ] , fp5 );
                fputs( "\n" , fp5 );
                fputs( "\\end{minipage}\n" , fp5 );
                slidebufferfig=0;
              } 


              /// add fig fitimage if indicated
              if ( ( slidebufferfig == 1 ) || ( slidebufferfig == 4 ))
	      {
                foxy++;//
                strncpy( slidebufferdata[foxy] , "}{" , PATH_MAX );
                strncat( slidebufferdata[foxy] ,  slidebufferfigfile  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 ); 
                strncat( slidebufferdata[foxy] , "}["  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
                if ( slidebuffernot == 1 ) strncat( slidebufferdata[foxy] , slidebufferfignote  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
                strncat( slidebufferdata[foxy] , "]"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );

                fooi = foxy ; 
                fputs( slidebufferdata[ fooi ] , fp5 );
                fputs( "\n" , fp5 );
                slidebufferfig=0;
	      }

              else if (( slidebufferfig == 2 ) || ( slidebufferfig == 3 ))
	      {
                foxy++;//
                strncpy( slidebufferdata[foxy] , "}{" , PATH_MAX );
                strncat( slidebufferdata[foxy] ,  slidebufferfigfile1  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 ); 
                strncat( slidebufferdata[foxy] , "}"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
                strncat( slidebufferdata[foxy] ,  "{"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 ); 
                strncat( slidebufferdata[foxy] ,  slidebufferfigfile2  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 ); 
                strncat( slidebufferdata[foxy] , "}["  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
                if ( slidebuffernot == 1 ) strncat( slidebufferdata[foxy] , slidebufferfignote  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );
                strncat( slidebufferdata[foxy] , "]"  , PATH_MAX - strlen( slidebufferdata[foxy]  ) -1 );

                fooi = foxy ; 
                fputs( slidebufferdata[ fooi ] , fp5 );
                fputs( "\n" , fp5 );
                slidebufferfig=0;
	      }


              /// close the frame
 	      fputs( "\\end{frame}\n" , fp5 );
 	      fputs( "\n" , fp5 );
 	      fputs( "\n" , fp5 );

              /// reset slide values
              foxy = 0;
              slidebufferfig=0;
              slidebuffernot=0;
              strncpy( slidemysection, "", PATH_MAX );

	      beamerlevel = 0;
  	      foundcode = 1;
            }
            //// debugger
	    if ( beamerlevel <= 0 ) beamerlevel = 0;
      }
     // end of beamer
     // end of beamer







            // !hline  some html 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'h' )
            if ( fetchline[2] == 'l' )
            if ( fetchline[3] == 'i' )
            if ( fetchline[4] == 'n' )
            if ( fetchline[5] == 'e' )
            {
 	      fputs( "\\clearpage" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }


            // !rem for a remark 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'r' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'm' )
            if ( fetchline[4] == ' ' )
            {
               foundcode = 1;
            }





         ///////////////////////////////////////
         /// SOME HTML  !url 
         ///////////////////////////////////////
            if ( foundcode == 0 )
            if ( markup_output_format == 2 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'u' )
            if ( fetchline[2] == 'r' )
            if ( fetchline[3] == 'l' )
            if ( fetchline[4] == ' ' )
            {
 	       fputs( "<a href=\"" , fp5 );
 	       fputs( strtrim( strcut( fetchline, 4+2, strlen( fetchline ))) , fp5 );
 	       fputs( "\">" , fp5 );
 	       fputs( "Link" , fp5 );
 	       fputs( "</a>" , fp5 );
  	       fputs( "<br>\n", fp5 );
               foundcode = 1;
            }











        ///////////////////////////////////////
        ///////////////////////////////////////
        // begin of html
        // begin of html
        ///////////////////////////////////////
        ///////////////////////////////////////
        ///////////////////////////////////////
        //Bullet list: (!ul)  <ul>
        //Numbered list: (!ol)  <ol>
        ///////////////////////////////////////
        /// SOME HTML !! ul li 
        ///////////////////////////////////////
            if ( foundcode == 0 )
            if ( markup_output_format == 2 ) // html 
            if ( fetchline[0] == '-' )
            if ( fetchline[1] == ' ' )
            {
	     if ( itemlevel == 1)  
             {
 	        fputs( "<li>" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen( fetchline ))) , fp5 );
  	        fputs( "</li>\n", fp5 );
             }
             foundcode = 1;
           }
          ///////////////////////////////////////
          ///////////////////////////////////////
          ///////////////////////////////////////
           if ( foundcode == 0 )
           if ( markup_output_format == 2 )  //html 
           if ( fetchline[0] == '>' )
           if ( fetchline[1] == ' ' )
           {
	     if ( itemlevel == 0)  
             {
 	        fputs( "<ul>" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen( fetchline ))) , fp5 );
  	        fputs( "\n", fp5 );
                itemlevel = 1 ;
             }
             foundcode = 1;
          }
          ///////////////////////////////////////
	  // item closer 
          ///////////////////////////////////////
            if ( foundcode == 0 )
            if ( markup_output_format == 2 ) //html 
	    if ( itemlevel >= 1 )
            if ( fetchline[0] != '\\' )
            if ( fetchline[0] != '!' )
            {
	      for ( fooi = 1 ; fooi <= itemlevel ; fooi++)
	      {
 	         fputs( "</ul>\n" , fp5 );
	      }
 	      fputs( "\n" , fp5 );
 	      fputs( "\n" , fp5 );
	      itemlevel = 0;
  	      foundcode = 1;
           }
         ///////////////////////////////////////
     // end of html
     // end of html










          ///////////////////////////////////////
          ///////////////////////////////////////
          ///////////////////////////////////////
          ///////////////////////////////////////
          ///////////////////////////////////////
            if ( foundcode == 0 )
            if ( markup_output_format == 3 )
            if ( fetchline[0] == '-' )
            if ( fetchline[1] == ' ' )
            {
	     if ( itemlevel == 1)  
             {
 	        fputs( "\\iexitem{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen( fetchline ))) , fp5 );
  	        fputs( "}", fp5 );
  	        fputs( "\n", fp5 );
             }
             foundcode = 1;
           }
          ///////////////////////////////////////
          ///////////////////////////////////////
          ///////////////////////////////////////
          ///////////////////////////////////////
           if ( foundcode == 0 )
           if ( markup_output_format == 3 ) //fleche
           if ( fetchline[0] == '>' )
           if ( fetchline[1] == ' ' )
           {
	     if ( itemlevel == 0)  
             {
 	        fputs( "\\iexfleche{{\\bfseries " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen( fetchline ))) , fp5 );
  	        fputs( "}}\n", fp5 );
  	        fputs( "\\iexitembegin\n", fp5 );
                itemlevel = 1 ;
             }
             foundcode = 1;
          }
          ///////////////////////////////////////
          ///////////////////////////////////////
	  // item closer 
          ///////////////////////////////////////
          ///////////////////////////////////////
            if ( foundcode == 0 )
            if ( markup_output_format == 3 )
	    if ( itemlevel >= 1 )
            if ( fetchline[0] != '\\' )
            if ( fetchline[0] != '!' )
            {
	      for ( fooi = 1 ; fooi <= itemlevel ; fooi++)
	      {
 	         fputs( "\\iexitemend\n" , fp5 );
	      }
 	      fputs( "\n" , fp5 );
 	      fputs( "\n" , fp5 );
	      itemlevel = 0;
  	      foundcode = 1;
           }
         ///////////////////////////////////////
         // end of !exam
         ///////////////////////////////////////
         ///////////////////////////////////////
         ///////////////////////////////////////








            /////////////////////////////////////////////////////////////////
            if ( notercode == 1 ) 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == '=' ) 
            if ( fetchline[2] == ' ' ) 
            {
 	        fputs( "\%section249\n" , fp5 );
 	        fputs( "\\section{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 2+2, strlen( fetchline ))) , fp5 );
  	        fputs( "}\n", fp5 );
  	      foundcode = 1;
            }







            /////////////////////////////
            /////////////////////////////
            /// notercode
            /////////////////////////////
            /////////////////////////////
            if ( notercode == 1 ) 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )  // !section 
            if ( fetchline[1] == 's' ) 
            if ( fetchline[2] == 'e' ) 
            if ( fetchline[3] == 'c' ) 
            if ( fetchline[4] == 't' ) 
            if ( fetchline[5] == 'i' ) 
            if ( fetchline[6] == 'o' ) 
            if ( fetchline[7] == 'n' ) 
            if ( fetchline[8] == ' ' ) 
            {
	        //if ( beamercode == 1 ) 
                fputs( "\%mysectioncontent251\n" , fp5 );
 	        fputs( "\\section{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 8+2, strlen( fetchline ))) , fp5 );
  	        fputs( "}\n", fp5 );
  	        foundcode = 1;
            }


            /////////////////////////////
            /////////////////////////////
            if ( notercode == 1 ) 
            if ( foundcode == 0 )   // mysection 
            if ( fetchline[0] == '\\' ) 
            if ( fetchline[1] == 'm' ) 
            if ( fetchline[2] == 'y' ) 
            if ( fetchline[3] == 's' ) 
            if ( fetchline[4] == 'e' ) 
            if ( fetchline[5] == 'c' ) 
            if ( fetchline[6] == 't' ) 
            if ( fetchline[7] == 'i' ) 
            if ( fetchline[8] == 'o' ) 
            if ( fetchline[9] == 'n' ) 
            if ( fetchline[10] == '{' ) 
            {
 	        fputs( "\%section252\n" , fp5 );
  	        fputs( "\\section{", fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        foundcode = 1;
            }








            /////////////////////////////
            /////////////////////////////
            //////// notercode
            /////////////////////////////
            /////////////////////////////
            if ( notercode == 1 ) 
            if ( foundcode == 0 ) // !note bla   , activated with !noter 
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'n' ) 
            if ( fetchline[2] == 'o' ) 
            if ( fetchline[3] == 't' ) 
            if ( fetchline[4] == 'e' ) 
            if ( fetchline[5] == ' ' ) 
            {
  	      fputs( "- ", fp5 );
 	      fputs( strtrim( strcut( fetchline, 5+2, strlen( fetchline ))) , fp5 );
  	      fputs( "\\\\", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }

            /////////////////////////////
            //////// notercode  \mysection{  } 
            //////// notercode
            if ( notercode == 1 ) 
            if ( foundcode == 0 ) 
            if ( fetchline[0] == '\\' ) 
            if ( fetchline[1] == 'm' ) 
            if ( fetchline[2] == 'y' ) 
            if ( fetchline[3] == 's' ) 
            if ( fetchline[4] == 'e' ) 
            if ( fetchline[5] == 'c' ) 
            if ( fetchline[6] == 't' ) 
            if ( fetchline[7] == 'i' ) 
            if ( fetchline[8] == 'o' ) 
            if ( fetchline[9] == 'n' ) 
            if ( fetchline[10] == '{' ) 
            {
 	        fputs( "\%section253\n" , fp5 );
  	        fputs( "\\section{", fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        foundcode = 1;
            }


            /////////////////////////
            if ( notercode == 1 ) 
            if ( foundcode == 0 )
            {
  	      foundcode = 1;
            }
            //////// notercode
            //////// notercode
            /// anti note
            if ( notercode == 0 ) 
            if ( foundcode == 0 ) // !note bla
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'n' ) 
            if ( fetchline[2] == 'o' ) 
            if ( fetchline[3] == 't' ) 
            if ( fetchline[4] == 'e' ) 
            if ( fetchline[5] == ' ' ) 
            {
  	      foundcode = 1;
            }
     // end of noter
     // end of noter
     // end of noter
     // end of noter








    /*
            /////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////
            if ( foundcode == 0 )  // deprecated, but ok, that's too modern
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == '=' ) 
            if ( fetchline[2] == ' ' ) 
            {
              if ( markup_output_format == 2 )
              {
 	        fputs( "<h1>" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 2+2, strlen( fetchline ))) , fp5 );
  	        fputs( "</h1>\n", fp5 );
              }
              else
              {
 	        //fputs( "\%section254\n" , fp5 );
 	        fputs( "\\section{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 2+2, strlen( fetchline ))) , fp5 );
  	        fputs( "}\n", fp5 );
              }
  	      foundcode = 1;
            }

            /////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////
            if ( foundcode == 0 ) // deprecated, but ok, that's too modern
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == '=' ) 
            if ( fetchline[2] == '=' ) 
            if ( fetchline[3] == ' ' ) 
            {
              if ( markup_output_format == 2 )
              {
 	        fputs( "<h2>" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen( fetchline ))) , fp5 );
  	        fputs( "</h2>\n", fp5 );
              }
              else
              {
    	        fputs( "\\subsection{" , fp5 );
    	        fputs( strtrim( strcut( fetchline, 3+2, strlen( fetchline ))) , fp5 );
     	        fputs( "}\n", fp5 );
              }
  	      foundcode = 1;
            }


            /////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////
            if ( foundcode == 0 ) // deprecated, but ok, that's too modern
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == '=' ) 
            if ( fetchline[2] == '=' ) 
            if ( fetchline[3] == '=' ) 
            if ( fetchline[4] == ' ' ) 
            {
              if ( markup_output_format == 2 )
              {
 	        fputs( "<h3>" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 4+2, strlen( fetchline ))) , fp5 );
  	        fputs( "</h3>\n", fp5 );
              }
              else
              {
    	        fputs( "\\subsubsection{" , fp5 );
    	        fputs( strtrim( strcut( fetchline, 4+2, strlen( fetchline ))) , fp5 );
     	        fputs( "}\n", fp5 );
              }
  	      foundcode = 1;
            }
           */














            ///////////////////////////////
            ///////////// regular !subsection 
            ///////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 's' )
            if ( fetchline[2] == 'u' )
            if ( fetchline[3] == 'b' )
            if ( fetchline[4] == 's' )
            if ( fetchline[5] == 'e' ) 
            if ( fetchline[6] == 'c' )
            if ( fetchline[7] == 't' )
            if ( fetchline[8] == 'i' )
            if ( fetchline[9] == 'o' )
            if ( fetchline[10] == 'n' )
            if ( fetchline[11] == ' ' )
            {
 	      fputs( "\\subsection{" , fp5 );
 	      fputs( strtrim( strcut( fetchline, 11+2, strlen(fetchline))) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }















            /////////////////////////////////////// set language
            /////////////////////////////////////// set language
            /////////////////////////////////////// set language
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( strcmp( strrlf( fetchline ) , "!:set lang=none" ) == 0 )
            {
                markup_language = 0;
  	        foundcode = 1;
            }
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( strcmp( strrlf( fetchline ) , "!:set lang=en" ) == 0 )
            {
                markup_language = 1;
  	        foundcode = 1;
            }
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( strcmp( strrlf( fetchline ) , "!:set lang=fr" ) == 0 )
            {
                markup_language = 2;
  	        foundcode = 1;
            }

            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( strcmp( strrlf( fetchline ) , "!:set lang=de" ) == 0 )
            {
                markup_language = 3;
  	        foundcode = 1;
            }






            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( strcmp( strrlf( fetchline ) , "!set item=1" ) == 0 )
            {
                markup_item = 1;
  	        foundcode = 1;
            }
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( strcmp( strrlf( fetchline ) , "!set item=2" ) == 0 )
            {
                markup_item = 2;
  	        foundcode = 1;
            }













            ///////////// !figab{myfigure.png} regular fig 
            ///////////// the regular figab to place figures side by side
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == 'a' )
            if ( fetchline[5] == 'b' )
            if ( fetchline[6] == '{' )
            {
  	        fputs( "\\begin{center}\n", fp5 );
  	        fputs( "\\begin{figure}\n", fp5 );
  	        fputs( "\\centering\n", fp5 );
  	        fputs( "\\includegraphics[width=0.48\\linewidth]{" , fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        fputs( "~\n", fp5 );
  	        fputs( "\\includegraphics[width=0.48\\linewidth]{" , fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  2 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        fputs( "\\end{figure}\n", fp5 );
  	        fputs( "\\end{center}\n", fp5 );
  	        foundcode = 1;
            }



 
            //////////////////////////////
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'r' ) 
            if ( fetchline[2] == 'a' ) 
            if ( fetchline[3] == 'w' ) 
            if ( fetchline[4] == 'l' ) 
            if ( fetchline[5] == 'i' ) 
            if ( fetchline[6] == 'n' ) 
            if ( fetchline[7] == 'e' ) 
            if ( fetchline[8] == ' ' ) 
            {
 	      fputs( strtxt2tex(  strcut( fetchline , 8+2, strlen(fetchline ))) , fp5 );
 	      //fputs( strcut( fetchline, 8+2, strlen(fetchline)) , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }




            ///////////// !fig{myfigure.png} regular fig 
            ///////////// the regular fig to place figures 
            ///////////// !fig{pic.png}{Text info}{ref for cite}
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == '{' )
            {
	      if ( strcount( fetchline, '}' ) >= 2 )
	      {
 	        fputs( "\\begin{figure}\n" , fp5 );
 	        fputs( "\\centering\n" , fp5 );
 	        fputs( "\\includegraphics[width=1.0\\textwidth,keepaspectratio]{" , fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );

 	        fputs( "\\caption{" , fp5 );
	        fputs( strtxt2tex( strdelimit(  fetchline  ,  '{' ,'}' , 2 ) ) , fp5 );
  	        fputs( "}\n", fp5 );

                printf( "=> Previous label is %s.\n", strlabel );
                if ( strcmp( strlabel, strdelimit(  fetchline  ,  '{' ,'}' , 3 ) ) != 0 ) 
                {
  	           printf( "=> This is a new found label (%s).\n", strlabel );
                   strncpy( strlabel, strdelimit(  fetchline  ,  '{' ,'}' , 3 )  , PATH_MAX );
 	           fputs( "\\label{" , fp5 );
	           fputs( strlabel  , fp5 );
	           //fputs( strdelimit(  fetchline  ,  '{' ,'}' , 3 )  , fp5 );
  	           fputs( "}\n", fp5 );
                }
 	        fputs( "\\end{figure}\n" , fp5 );
	      }
	      else //normal
	      {
  	        fputs( "\\begin{center}\n", fp5 );
  	        fputs( "\\includegraphics[width=1.0\\textwidth]{" , fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        fputs( "\\end{center}\n", fp5 );
	      }
  	      foundcode = 1;
            }




            ///////////// !float{pic.png} for a quick float figure, force fig under text
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'l' )
            if ( fetchline[3] == 'o' )
            if ( fetchline[4] == 'a' )
            if ( fetchline[5] == 't' )
            if ( fetchline[6] == '{' )
            {
 	      fputs( "\\begin{figure}[H]\n" , fp5 );
  	      fputs( "\\begin{center}\n", fp5 );
  	      fputs( "\\includegraphics[width=1.0\\textwidth]{" , fp5 );
 	      fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	      fputs( "}\n", fp5 );
  	      fputs( "\\end{center}\n", fp5 );
  	      fputs( "\\end{figure}\n", fp5 );
  	      foundcode = 1;
            }






            ///////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////
            ///////////// Unterlagen
            ///////////// Try \begin{figure}[!htb]. In nearly all cases it helps.
            ///////////// !figh{pic.png}{Text info}{ref for cite}
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == 'h' )
            if ( fetchline[5] == '{' )
            {
	      if ( strcount( fetchline, '}' ) >= 2 )
	      {
 	        fputs( "\\begin{figure}[!htb]\n" , fp5 );
 	        fputs( "\\centering\n" , fp5 );
 	        fputs( "\\includegraphics[height=0.33\\textheight,keepaspectratio]{" , fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );

 	        fputs( "\\caption{" , fp5 );
	        fputs( strtxt2tex( strdelimit(  fetchline  ,  '{' ,'}' , 2 ) ) , fp5 );
  	        fputs( "}\n", fp5 );

 	        fputs( "\\label{" , fp5 );
	        fputs( strdelimit(  fetchline  ,  '{' ,'}' , 3 )  , fp5 );
  	        fputs( "}\n", fp5 );

 	        fputs( "\\end{figure}\n" , fp5 );
	      }
	      else //normal
	      {
  	        fputs( "\\begin{center}\n", fp5 );
  	        fputs( "\\includegraphics[height=1.0\\textheight]{" , fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        fputs( "\\end{center}\n", fp5 );
	      }
  	      foundcode = 1;
            }
            ///////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////







            ///////////// !unifig{myfigure.png} regular fig 
            ///////////// the regular fig to place figures 
     /*
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'u' )
            if ( fetchline[2] == 'n' )
            if ( fetchline[3] == 'i' )
            if ( fetchline[4] == 'f' )
            if ( fetchline[5] == 'i' )
            if ( fetchline[6] == 'g' )
            if ( fetchline[7] == '{' )
            {
	      if ( strcount( fetchline, '}' ) >= 2 )
	      {
 	        fputs( "\\unifigplus{" , fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}{", fp5 );
	        fputs( strtxt2tex( strdelimit(  fetchline  ,  '{' ,'}' , 2 ) ) , fp5 );
  	        fputs( "}", fp5 );
  	        fputs( "{", fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' , 3 ) , fp5 );
		fputs( "}", fp5 );
		fputs( "{", fp5 ); //size
 	        fputs( strdelimit( fetchline,  '{' ,'}' , 4 ) , fp5 );
		fputs( "}", fp5 ); //size
		fputs( "{", fp5 ); //bottom note
 	        fputs( strdelimit( fetchline,  '{' ,'}' , 5 ) , fp5 );
		fputs( "}", fp5 ); //bottom note
  	        fputs( "\n", fp5 );
	      }
	      else
	      {
  	        fputs( "\\begin{center}\n", fp5 );
  	        fputs( "\\includegraphics[width=1.0\\textwidth]{" , fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        fputs( "\\end{center}\n", fp5 );
	      }
  	      foundcode = 1;
            }
           */









            // !background{file.pdf}
            //fputs( "\\usepackage{wallpaper}\n", fp5 );
            //\CenterWallPaper{1}{background.pdf}
            if ( foundcode == 0 )
            if ( fetchline[0] ==  '!' ) 
            if ( fetchline[1] ==  'b' )
            if ( fetchline[2] ==  'a' )
            if ( fetchline[3] ==  'c' )
            if ( fetchline[4] ==  'k' ) 
            if ( fetchline[5] ==  'g' ) 
            if ( fetchline[6] ==  'r' )
            if ( fetchline[7] ==  'o' )
            if ( fetchline[8] ==  'u' )
            if ( fetchline[9] ==  'n' )
            if ( fetchline[10] == 'd' )
            if ( fetchline[11] == '{' )
            {
  	      fputs( "\\CenterWallPaper{1}{", fp5 );
 	      fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }


            // !ref 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'r' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'f' )
            if ( fetchline[4] == '{' )
            {
  	        fputs( "\\ref{" , fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        foundcode = 1;
            }
            // !cite 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'c' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 't' )
            if ( fetchline[4] == 'e' )
            if ( fetchline[5] == '{' )
            {
  	        fputs( "\\cite{" , fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        foundcode = 1;
            }



            ///////////// !img[0.5]{ } 
            ///////////// !img[0.5]{image.png}  with 0.5 is the reduction level (width)
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'i' )
            if ( fetchline[2] == 'm' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == '[' )
            {
  	        fputs( "\\begin{center}\n", fp5 );
  	        fputs( "\\includegraphics[scale,height=" , fp5 );
 	        fputs( strdelimit( fetchline,  '[' ,']' ,  1 ) , fp5 );
                fputs( "\\textheight]{" , fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        fputs( "\\end{center}\n", fp5 );
  	        //fputs( "\\includegraphics[scale=" , fp5 );
  	        //fputs( "]{" , fp5 );
 	        //fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        //fputs( "}\n", fp5 );
  	        foundcode = 1;
            }


               ////////////////////////////
               // !maths 
               ////////////////////////////
               if ( foundcode == 0 )
               if (  ( fetchline[ 0 ] == '!' ) 
               &&  ( fetchline[ 1 ] == 'm' ) 
               &&  ( fetchline[ 2 ] == 'a' ) 
               &&  ( fetchline[ 3 ] == 't' ) 
               &&  ( fetchline[ 4 ] == 'h' ) 
               &&  ( fetchline[ 5 ] == 's' ) 
               )
               {
                 fputs( "\\usepackage{amsmath}\n" , fp5);
                 fputs( "\\usepackage{graphicx}\n" , fp5);
                 fputs( "\\usepackage{epstopdf}\n" , fp5);
                 fputs( "\n" , fp5);
                 foundcode = 1;
               }


            ///////////// !img 
            ///////////// !img image.png
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'i' )
            if ( fetchline[2] == 'm' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == ' ' )
            {
  	        fputs( "\\includegraphics[scale]{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
  	        fputs( "}\n", fp5 );
  	      foundcode = 1;
            }





            // !equ equation with number and line feed, for maths.
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'e' )
            if ( fetchline[2] == 'q' )
            if ( fetchline[3] == 'u' )
            if ( fetchline[4] == ' ' )
            {
	       if ( itemlevel >= 1)  
               {
  	         fputs( "\\item[ ] ", fp5 );
  	         fputs( "\\begin{center}", fp5 );
 	         fputs(  strcut( fetchline, 4+2, strlen(fetchline)) , fp5 );
  	         fputs( "\\end{center}", fp5 );
  	         fputs( "\n", fp5 );
               }
               else
               {
  	         fputs( "\\begin{center}", fp5 );
 	         fputs(  strcut( fetchline, 4+2, strlen(fetchline)) , fp5 );
  	         fputs( "\\end{center}", fp5 );
  	         fputs( "\n", fp5 );
               }
  	      foundcode = 1;
            }

            // !box equation with number and line feed, for maths.
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'b' )
            if ( fetchline[2] == 'o' )
            if ( fetchline[3] == 'x' )
            if ( fetchline[4] == ' ' )
            {
	       if ( itemlevel >= 1)  
               {
  	         fputs( "\\item[ ] ", fp5 );
  	         fputs( "\\begin{center}", fp5 );
  	         fputs( "\\boxed{", fp5 );
 	         fputs(  strcut( fetchline, 4+2, strlen(fetchline)) , fp5 );
  	         fputs( "}", fp5 );
  	         fputs( "\\end{center}", fp5 );
  	         fputs( "\n", fp5 );
               }
               else
               {
  	         fputs( "\\begin{center}", fp5 );
  	         fputs( "\\boxed{", fp5 );
 	         fputs(  strcut( fetchline, 4+2, strlen(fetchline)) , fp5 );
  	         fputs( "}", fp5 );
  	         fputs( "\\end{center}", fp5 );
  	         fputs( "\n", fp5 );
               }
  	      foundcode = 1;
            }




            ////////////////////////////////////////////////////////////
            //\includegraphics[angle=270,origin=c,width=1.0\textwidth]{20190323_080841.jpg}
            ///////////// !figr fig.png Quick fig !! (workaround), quick workaround for right rotate
            ///////////// !figr myfigure.png  //this is a fast fig 
            // widely used!
            ////////////////////////////////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == 'r' )
            if ( fetchline[5] == ' ' )
            {
  	        fputs( "\\begin{center}\n", fp5 );
                fputs( "\\includegraphics[angle=270,origin=c,width=1.0\\textwidth]{", fp5 );
  	        /////fputs( "\\includegraphics[width=1.0\\textwidth]{" , fp5 );
 	        fputs( strcut( fetchline, 5+2, strlen(fetchline)) , fp5 );
  	        fputs( "}\n", fp5 );
  	        fputs( "\\end{center}\n", fp5 );
  	        foundcode = 1;
            }

            ///////////// !fig fig.png Quick fig !! (workaround), quick workaround
            ///////////// !fig myfigure.png  //this is a fast fig 
            // widely used!
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'f' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == ' ' )
            {
  	        fputs( "\\begin{center}\n", fp5 );
  	        fputs( "\\includegraphics[width=1.0\\textwidth]{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
  	        fputs( "}\n", fp5 );
  	        fputs( "\\end{center}\n", fp5 );
  	      foundcode = 1;
            }



            //![kittens!](http://placekitten.com/400/500 "Kitten from placekitten.com")
            // !()[] quick markdown workaround for figs
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == '[' )
            if ( strstr( fetchline , "]" ) != 0 ) 
            if ( strstr( fetchline , ")" ) != 0 ) 
            if ( strstr( fetchline , "(" ) != 0 ) 
            {
  	        fputs( "\\begin{center}\n", fp5 );
  	        fputs( "\\includegraphics[width=1.0\\textwidth]{" , fp5 );
 	        //fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
                fputs( strdelimit( fetchline,  '(' ,')' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        fputs( "\\end{center}\n", fp5 );
  	      foundcode = 1;
            }







            /////////////////////////////
            /////////////////////////////
            ///////////////////////////// for items, tabbed
            ///////////////////////////// !tab 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 't' )
            if ( fetchline[2] == 'a' )
            if ( fetchline[3] == 'b' )
            if ( fetchline[4] == ' ' )
            {
	      if ( numberinglevel == 1)  
	      {
       	        fputs( "\\item[ ] " , fp5 );
       	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
       	        fputs( "\n" , fp5 );
		numberinglevel = 1;
	      }
	      else if ( numberinglevel == 0)  
	      {
       	        fputs( "\\item[ ] " , fp5 );
       	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
       	        fputs( "\n" , fp5 );
		numberinglevel = 1;
	      }
 	      foundcode = 1;
            }





















            ////////////////////////
            //////// SIMPLE ITEMS
            /// !ol wash, it may remind you HTML !
            // Numbered list: (!ol)
            ////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'o' )  // for Ordered!
            if ( fetchline[2] == 'l' ) 
            if ( fetchline[3] == ' ' )
	    {
	        if ( itemlevel == 1) fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item[{\\bfseries " , fp5 );
                fooc = snprintf( fooccharo, 50 , "%d", mynumber );
 	        fputs( fooccharo , fp5 );
 	        fputs( ".}]{" , fp5 );
 	        fputs( strcut( fetchline, 3+2, strlen(fetchline)) , fp5 );
                fputs( "}" , fp5 );
 	        fputs( "\n" , fp5 );
                mynumber++;
                itemlevel = 2;
  	        foundcode = 1;
            }
            ////////////////////////
            //////// SIMPLE ITEMS
            /// !ul for bullet, it may remind you HTML !
            /// Bullet list: (!ul)
            ////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'u' ) // for bUllets
            if ( fetchline[2] == 'l' ) 
            if ( fetchline[3] == ' ' )
	    {
	        if ( itemlevel == 1) fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strcut( fetchline, 3+2, strlen(fetchline)) , fp5 );
 	        fputs( "\n" , fp5 );
                itemlevel = 2;
  	        foundcode = 1;
            }
            ////////////////////////
            //////// SIMPLE ITEMS
            /// !nu for none (null), no bullet, it may remind you HTML !
            /// null label list: (!nu)
            ////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'n' ) // for Null
            if ( fetchline[2] == 'u' ) 
            if ( fetchline[3] == ' ' )
	    {
	        if ( itemlevel == 1) fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item[]{" , fp5 );
 	        fputs( strcut( fetchline, 3+2, strlen(fetchline)) , fp5 );
 	        fputs( "}" , fp5 );
 	        fputs( "\n" , fp5 );
                itemlevel = 2;
  	        foundcode = 1;
            }










            /////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '>' )  //regular one 
            if ( fetchline[1] == ' ' )
            {
              mynumber = 1;
	      if ( itemlevel == 1)  
	      {
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	      }
	      else if ( itemlevel == 2)  
	      {
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 1;
	      }
	      else if ( itemlevel == 3)  
	      {
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 1;
	      }
	      else if ( itemlevel == 0)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 1;
	      }
  	      foundcode = 1;
             }


            /////////////////////////////
            /////////////////////////////
            /////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )  //regular one, !bull historically kept.
            if ( fetchline[1] == 'b' )  
            if ( fetchline[2] == 'u' )  
            if ( fetchline[3] == 'l' )  
            if ( fetchline[4] == 'l' )
            if ( fetchline[5] == ' ' )
            {
              mynumber = 1;
	      if ( itemlevel == 1)  
	      {
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 5+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	      }
	      else if ( itemlevel == 2)  
	      {
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 5+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 1;
	      }
	      else if ( itemlevel == 3)  
	      {
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 5+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 1;
	      }
	      else if ( itemlevel == 0)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( "{\\bfseries " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 5+2, strlen(fetchline))) , fp5 );
 	        fputs( "}" , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 1;
	      }
  	      foundcode = 1;
             }











            /// new!
            if ( foundcode == 0 )
            if ( fetchline[0] == '*' )
            if ( fetchline[1] == ' ' )
            {
	      if ( itemlevel == 1)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		itemlevel = 3;
	      }
	      else if ( itemlevel == 2)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		itemlevel = 3;
	      }
	      else if ( itemlevel == 3)  
	      {
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 3;
	      }
	      else if ( itemlevel == 4)  
	      {
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 3;
	      }
	      else if ( itemlevel == 0)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 3;
	      }
  	      foundcode = 1;
            }



            /// kinda for looking like tabulation for items 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 't' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == ' ' )
            {
	      if ( itemlevel == 1)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item[ ] " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
		itemlevel = 2;
	      }
	      else if ( itemlevel == 2)  
	      {
 	        fputs( "\\item[ ] " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 2;
	      }
	      else if ( itemlevel == 3)  
	      {
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\item[ ] " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 2;
	      }
	      else if ( itemlevel == 0)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item[ ] " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 2;
	      }
  	      foundcode = 1;
            }






         ///////////////////////////////////////
         ///////////////////////////////////////
         ///////////////////////////////////////
         ///////////////////////////////////////
         ///////////////////////////////////////
            /// old and ok
            /// the most useful part of unibeam 
            if ( foundcode == 0 )
            if ( fetchline[0] == '-' )
            if ( fetchline[1] == ' ' )
            {
	      if ( itemlevel == 1)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
                if ( markup_item == 2 ) fputs( " (3 Pkt.)" , fp5 );
 	        fputs( "\n" , fp5 );
		itemlevel = 2;
	      }
	      else if ( itemlevel == 2)  
	      {
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
                if ( markup_item == 2 ) fputs( " (3 Pkt.)" , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 2;
	      }
	      else if ( itemlevel == 3)  
	      {
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 2;
	      }
	      else if ( itemlevel == 0)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtrim( strcut( fetchline, 1+2, strlen(fetchline))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 2;
	      }
  	      foundcode = 1;
            }
	    // the most important: the item closer  (old item, but very stable)
            if ( foundcode == 0 )
	    if ( itemlevel >= 1 )
            if ( fetchline[0] != '\\' )
            if ( fetchline[0] != '!' )
            if ( fetchline[0] != '|' )
            {
	      for ( fooi = 1 ; fooi <= itemlevel ; fooi++)
	      {
	         fputs( "\\end{itemize}\n" , fp5 );
	      }
 	      fputs( "\n" , fp5 );
 	      fputs( "\n" , fp5 );
	      itemlevel = 0;
  	      foundcode = 1;
            }
         ///////////////////////////////////////
         ///////////////////////////////////////
         ///////////////////////////////////////
         ///////////////////////////////////////
         ///////////////////////////////////////


////////////////////////////////////
            /// old and ok
            /// the most useful part of unibeam 
            if ( foundcode == 0 )
            if ( fetchline[0] == '|' )
            if ( fetchline[1] == '-' )
            if ( fetchline[2] == ' ' )
            {
	      if ( itemlevel == 1)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs(  strtxt2tex(strtrim( strcut( fetchline, 2+2, strlen(fetchline)))) , fp5 );
                if ( markup_item == 2 ) fputs( " (3 Pkt.)" , fp5 );
 	        fputs( "\n" , fp5 );
		itemlevel = 2;
	      }
	      else if ( itemlevel == 2)  
	      {
 	        fputs( "\\item " , fp5 );
 	        fputs(  strtxt2tex(strtrim( strcut( fetchline, 2+2, strlen(fetchline)))) , fp5 );
                if ( markup_item == 2 ) fputs( " (3 Pkt.)" , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 2;
	      }
	      else if ( itemlevel == 3)  
	      {
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs(  strtxt2tex(strtrim( strcut( fetchline, 2+2, strlen(fetchline)))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 2;
	      }
	      else if ( itemlevel == 0)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item " , fp5 );
 	        fputs( strtxt2tex( strtrim( strcut( fetchline, 2+2, strlen(fetchline)))) , fp5 );
 	        fputs( "\n" , fp5 );
	        itemlevel = 2;
	      }
  	      foundcode = 1;
            }
	    // the most important: the item closer  (old item, but very stable)
            if ( foundcode == 0 )
	    if ( itemlevel >= 1 )
            if ( fetchline[0] != '\\' )
            if ( fetchline[0] != '!' )
            if ( fetchline[0] != '|' )
            {
	      for ( fooi = 1 ; fooi <= itemlevel ; fooi++)
	      {
	         fputs( "\\end{itemize}\n" , fp5 );
	      }
 	      fputs( "\n" , fp5 );
 	      fputs( "\n" , fp5 );
	      itemlevel = 0;
  	      foundcode = 1;
            }
         ///////////////////////////////////////
         ///////////////////////////////////////
         ///////////////////////////////////////
         ///////////////////////////////////////
         ///////////////////////////////////////



            ////////////////
            //// !latex here
            ////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
	    if ( strcmp( fetchcmdline, "!latex" ) == 0 )
            {
              fputs( "\\documentclass[11pt]{article}", fp5 );
              fputs( "\\usepackage{graphicx}", fp5 );
              fputs( "\\usepackage{epstopdf}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            } 




            //////////////////////////////////
            //// !tex
            ////////////////
            //// !tex here
            ////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
	    if ( strcmp( fetchcmdline, "!tex" ) == 0 )
            {
              fputs( "\\documentclass[11pt]{article}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            } 
            //// !begintex
           /*
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
	    if ( strcmp( fetchcmdline, "!begintex" ) == 0 )
            {
              fputs( "\\documentclass[11pt]{article}\n", fp5 );
              fputs( "\\begin{document}\n", fp5 );
  	      foundcode = 1;
            } 
           */







            /* removed
            /////////////////////////////
            // >> text, main
            // ![nu] text1 (with auto numbering)
            // ![nu] text2
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == '[' )
            if ( fetchline[2] == 'n' ) 
            if ( fetchline[3] == 'u' )
            if ( fetchline[4] == ']' )
            if ( fetchline[5] == ' ' )
	    {
	        if ( numberinglevel == 0) fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item[{\\bfseries " , fp5 );
                fooc = snprintf( fooccharo, 50 , "%d", mynumber );
 	        fputs( fooccharo , fp5 );
 	        fputs( ".}]{" , fp5 );
 	        fputs( strcut( fetchline, 5+2, strlen(fetchline)) , fp5 );
                fputs( "}" , fp5 );
 	        fputs( "\n" , fp5 );
                mynumber++;
		numberinglevel = 1;
  	        foundcode = 1;
            }
            */









            /*
            /////// FOR EXAMS
            ///////////////////////////// for questions with points
            /////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == '[' )
            if ( fetchline[2] == 'q' )
            if ( fetchline[3] == 't' )  // for question topics
            if ( fetchline[4] == ']' )
            if ( fetchline[5] == ' ' )
            {
              fputs( "\\iexflechelf{{\\bfseries ", fp5 );
 	      fputs( strtrim( strcut( fetchline, 5+2, strlen(fetchline))) , fp5 );
              fputs( "}}\\\\", fp5 );
 	      fputs( "\n" , fp5 );
  	      numberinglevel = 1;
  	      foundcode = 1;
            }
            */


   // for exams   > title,  !qu !qu or just !qu (depending on format)  



            ///////////////////////////// for questions with points
            /////////////////////////////
            /*
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == '[' )
            if ( fetchline[2] == 'q' ) // used for english type of questions (single question list, with pts)
            if ( fetchline[3] == ']' )
            if ( fetchline[4] == ' ' )
            {
	      if ( numberinglevel == 1)  
	      {
 	        fputs( "\\item[{\\bfseries {\\arabic{unibullcounter}}}.)]{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
                if ( markup_language == 1 )      fputs( " (3 Points)}" , fp5 );
                else if ( markup_language == 2 ) fputs( " (3 Pts.)}" , fp5 );
                else if ( markup_language == 3 ) fputs( " (3 Pkt.)}" , fp5 );
                else if ( markup_language == 0 ) fputs( "}" , fp5 );
 	        fputs( "\n" , fp5 );
                fputs( "\\addtocounter{unibullcounter}{1}\n" , fp5 );
		numberinglevel = 1;
	      }


	      else if ( numberinglevel == 0)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );

 	        fputs( "\\item[{\\bfseries {" , fp5 );
                fooc = snprintf( fooccharo, 50 , "%d", question_qucounter );
 	        fputs( fooccharo  , fp5 );
 	        fputs( "}}.)]{" , fp5 );
                question_qucounter++;

 	        //fputs( "\\item[{\\bfseries {\\arabic{unibullcounter}}}.)]{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
                if ( markup_language == 1 )      fputs( " (3 Points)}" , fp5 );
                else if ( markup_language == 2 ) fputs( " (3 Pts.)}" , fp5 );
                else if ( markup_language == 3 ) fputs( " (3 Pkt.)}" , fp5 );
                else if ( markup_language == 0 ) fputs( "}" , fp5 );
 	        fputs( "\n" , fp5 );
                fputs( "\\addtocounter{unibullcounter}{1}\n" , fp5 );
		//numberinglevel = 1;
 	        fputs( "\\end{itemize}\n" , fp5 );
 	        fputs( "\\vspace{4cm}\n" , fp5 );
	      }
  	      foundcode = 1;
            }
            */

            


            /// FOR EXERCISES
            ///////////////////////////// 
            /// free numbering question
            /// for exr !! (example: ...)
            /// sometimes useful
            ///////////////////////////// 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == '[' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == ']' )
            if ( fetchline[4] == ' ' )
            {
	      if ( numberinglevel == 1)  
	      {
 	        fputs( "\\item[{\\bfseries {\\arabic{unibullcounter}}}.)]{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
 	        fputs( "}\n" , fp5 );
                fputs( "\\addtocounter{unibullcounter}{1}\n" , fp5 );
		numberinglevel = 1;
	      }
	      else if ( numberinglevel == 0)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item[{\\bfseries {\\arabic{unibullcounter}}}.)]{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 4+2, strlen(fetchline))) , fp5 );
 	        fputs( "}\n" , fp5 );
 	        fputs( "\n" , fp5 );
                fputs( "\\addtocounter{unibullcounter}{1}\n" , fp5 );
		numberinglevel = 1;
	      }
  	      foundcode = 1;
            }









            ///////////////////////////// 
            /// sometimes useful, with !qu 
            /// Just the regular 
            /// Simple, and relatively easy
            ///////////////////////////// 
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'q' )
            if ( fetchline[2] == 'u' )
            if ( fetchline[3] == ' ' )
            {
	      if ( numberinglevel == 1)  
	      {
 	        fputs( "\\item[{\\bfseries {" , fp5 );
                fooc = snprintf( fooccharo, 50 , "%d", list_numbering );
 	        fputs( fooccharo  , fp5 );
 	        fputs( "}}.)]{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
 	        fputs( "}\n" , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 1;
                list_numbering++;
	      }
	      else if ( numberinglevel == 0)  
	      {
 	        fputs( "\\begin{itemize}\n" , fp5 );
 	        fputs( "\\item[{\\bfseries {" , fp5 );
                fooc = snprintf( fooccharo, 50 , "%d", list_numbering );
 	        fputs( fooccharo  , fp5 );
 	        fputs( "}}.)]{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 3+2, strlen(fetchline))) , fp5 );
 	        fputs( "}\n" , fp5 );
 	        fputs( "\n" , fp5 );
		numberinglevel = 1;
                list_numbering++;
	      }
  	      foundcode = 1;
            }












            // please keep it
            if ( foundcode == 0 )
            if ( ( fetchline[0] == '!' ) || ( fetchline[0] == '\\' )  )
            if ( fetchline[1] == 'm' ) 
            if ( fetchline[2] == 'y' ) 
            if ( fetchline[3] == 's' ) 
            if ( fetchline[4] == 'e' ) 
            if ( fetchline[5] == 'c' ) 
            if ( fetchline[6] == 't' ) 
            if ( fetchline[7] == 'i' ) 
            if ( fetchline[8] == 'o' ) 
            if ( fetchline[9] == 'n' ) 
            if ( fetchline[10] == '{' ) 
            {
 	        fputs( "\%section272\n" , fp5 );
  	        fputs( "\\section{", fp5 );
 	        fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	        fputs( "}\n", fp5 );
  	        foundcode = 1;
            }



            ////////////////////////////
            if ( foundcode == 0 )
            if ( beamercode == 1 )
            //if ( fetchcmdline[0] == '!' ) 
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 's' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'c' )
            if ( fetchline[4] == 't' )
            if ( fetchline[5] == 'i' )
            if ( fetchline[6] == 'o' )
            if ( fetchline[7] == 'n' )
            if ( fetchline[8] == ' ' )
            {  
 	      fputs( "\%section281\n" , fp5 );
 	      fputs( "\\section{" , fp5 );
 	      //fputs( strtrim( strcut( fetchcmdline, 8+2, strlen(fetchcmdline))) , fp5 );
 	      fputs( strtrim( strcut( fetchline, 8+2, strlen(fetchline))) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }



            /// new mysection
            // (why !, because it can be into the standard 250 chars)  
            if ( foundcode == 0 )
            //if ( fetchcmdline[0] == '!' ) 
	    //if ( strcmp( strcut(fetchcmdline, 2, 9) , "section " ) == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 's' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'c' )
            if ( fetchline[4] == 't' )
            if ( fetchline[5] == 'i' )
            if ( fetchline[6] == 'o' )
            if ( fetchline[7] == 'n' )
            if ( fetchline[8] == ' ' )
            {  
 	      fputs( "\%section260\n" , fp5 );
 	      fputs( "\\section{" , fp5 );
 	      //fputs( strtrim( strcut( fetchcmdline, 8+2, strlen(fetchcmdline))) , fp5 );
 	      fputs( strtrim( strcut( fetchline, 8+2, strlen(fetchline))) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }





            ///////////////////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' ) 
            if ( fetchcmdline[1] == '=' ) 
            if ( fetchcmdline[2] == ' ' ) 
            {
 	      fputs( "\%section263\n" , fp5 );
 	      fputs( "\\section{" , fp5 );
 	      fputs( strtrim( strcut( fetchcmdline, 2+2, strlen(fetchcmdline))) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }





            ///////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 's' ) 
            if ( fetchline[2] == 'e' ) 
            if ( fetchline[3] == 'c' ) 
            if ( fetchline[4] == ' ' ) 
            {
 	      fputs( "\%seconlysec12\n" , fp5 );
 	      fputs( "\\section{" , fp5 );
 	      fputs( strtrim( strcut( fetchline , 4+2, strlen(  fetchline ))) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }


            /////////////////////////////
            if ( foundcode == 0 )
	    if ( beamercode == 1 )
	    if ( contentcode == 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == '=' )
            if ( fetchline[2] == ' ' )
            {
              // this is for beamer here
 	      fputs( "\%section282\n" , fp5 );
 	      fputs( "\\section{" , fp5 );
 	      fputs( strtrim( strcut( fetchline, 2+2, strlen(fetchline))) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }


            ///////////////////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' ) 
            if ( fetchcmdline[1] == '=' ) 
            if ( fetchcmdline[2] == ' ' ) 
            {
              fputs( "\%mysectioncontent2\n" , fp5 );
 	      fputs( "\\section{" , fp5 );
 	      fputs( strtrim( strcut( fetchcmdline, 2+2, strlen(fetchcmdline))) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }



            ///////////////////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' ) 
	    if ( strcmp( strcut(fetchcmdline, 2, 9+3) , "!subsection " ) == 0 )
            {
 	      fputs( "\\subsection{" , fp5 );
 	      fputs( strtrim( strcut( fetchcmdline, 8+3+2, strlen(fetchcmdline))) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }

            ///////////////////////////////
            if ( foundcode == 0 )
            if ( ( fetchcmdline[0] == '!' ) || ( fetchcmdline[0] == '$' ))
	    if ( strcmp( strcut(fetchcmdline, 2, 5+3) , "subsec " ) == 0 )
            {
 	      fputs( "\\subsection{" , fp5 );
 	      fputs( strtrim( strcut( fetchcmdline, 4+3+2, strlen(fetchcmdline))) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }



	    /////////////////
            if ( foundcode == 0 )
            if ( ( fetchcmdline[0] == '!' ) || ( fetchcmdline[0] == '$' ))
	    if ( strcmp( strcut(fetchcmdline, 2, 9+3+3) , "!subsubsection " ) == 0 )
            {
 	      fputs( "\\subsubsection{" , fp5 );
 	      fputs( strtrim( strcut( fetchcmdline, 8+3+3+2, strlen(fetchcmdline))) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }


	    ////  function !chapter text / works always
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'c' )
            if ( fetchline[2] == 'h' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'p' )
            if ( fetchline[5] == 't' )
            if ( fetchline[6] == 'e' )
            if ( fetchline[7] == 'r' )
            if ( fetchline[8] == ' ' )
            {
 	      fputs( "\\chapter{" , fp5 );
 	      fputs( strtrim( strcut( fetchline, 8+2, strlen(fetchline))) , fp5 );
  	      fputs( "}\n", fp5 );
  	      foundcode = 1;
            }



           
	    /////////////////
            if ( foundcode == 0 ) // !- item into script
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == '-' )
            if ( fetchline[2] == ' ' )
            {
 	      fputs( "- " , fp5 );
 	      fputs( strcut( fetchline, 2+2, strlen(fetchline)) , fp5 );
  	      fputs( "\\\\", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }





            if ( foundcode == 0 ) // !it text
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'i' )
            if ( fetchline[2] == 't' )
            if ( fetchline[3] == ' ' )
            {
 	      fputs( "\\textit{" , fp5 );
 	      fputs( strcut( fetchline, 3+2, strlen(fetchline)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }


	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 ) // !italic thing
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'i' )
            if ( fetchline[2] == 't' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 'l' )
            if ( fetchline[5] == 'i' )
            if ( fetchline[6] == 'i' )
            if ( fetchline[7] == 'c' )
            if ( fetchline[8] == ' ' )
            {
 	      fputs( "\\textit{" , fp5 );
 	      fputs( strcut( fetchline, 8+2, strlen(fetchline)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }


	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 ) // !normal to do normal thing
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'n' )
            if ( fetchline[2] == 'o' )
            if ( fetchline[3] == 'r' )
            if ( fetchline[4] == 'm' )
            if ( fetchline[5] == 'a' )
            if ( fetchline[6] == 'l' )
            if ( fetchline[7] == ' ' )
            {
 	      fputs( strcut( fetchline, 7+2, strlen(fetchline)) , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }



	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 ) // !trim to do normal and trim line 
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 't' )
            if ( fetchline[2] == 'r' )
            if ( fetchline[3] == 'i' )
            if ( fetchline[4] == 'm' )
            if ( fetchline[5] == ' ' )
            {
 	      fputs(  strtrim( strcut( fetchline, 5+2, strlen(fetchline)) ) , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }

	    /////////////////
	    /////////////////
            if ( foundcode == 0 ) // !code to paste code, in dev
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'c' )
            if ( fetchline[2] == 'o' )
            if ( fetchline[3] == 'd' )
            if ( fetchline[4] == 'e' )
            if ( fetchline[5] == ' ' )
            {
 	      fputs(  strtrim( strcut( fetchline, 5+2, strlen(fetchline)) ) , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }

            if ( foundcode == 0 ) // !center 
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'c' )
            if ( fetchcmdline[2] == 'e' )
            if ( fetchcmdline[3] == 'n' )
            if ( fetchcmdline[4] == 't' )
            if ( fetchcmdline[5] == 'e' )
            if ( fetchcmdline[6] == 'r' )
            if ( fetchcmdline[7] == ' ' )
            {
 	      fputs( "\\begin{center}" , fp5 );
 	      fputs( strcut( fetchcmdline, 7+2, strlen(fetchcmdline)) , fp5 );
 	      fputs( "\\end{center}" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }



	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 ) // !bold, without line feed of tex !
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'b' )
            if ( fetchcmdline[2] == 'o' )
            if ( fetchcmdline[3] == 'l' )
            if ( fetchcmdline[4] == 'd' )
            if ( fetchcmdline[5] == ' ' )
            {
 	      fputs( "{\\bfseries " , fp5 );
 	      fputs( strcut( fetchcmdline, 5+2, strlen(fetchcmdline)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }

	    /////////////////
            if ( foundcode == 0 ) // !bd text
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'b' )
            if ( fetchcmdline[2] == 'd' )
            if ( fetchcmdline[3] == ' ' )
            {
 	      fputs( "{\\bfseries " , fp5 );
 	      fputs( strcut( fetchcmdline, 3+2, strlen(fetchcmdline)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }



	    /////////////////
            if ( foundcode == 0 ) // !boldline, without line feed of tex !
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'b' )
            if ( fetchcmdline[2] == 'o' )
            if ( fetchcmdline[3] == 'l' )
            if ( fetchcmdline[4] == 'd' )
            if ( fetchcmdline[5] == 'l' )
            if ( fetchcmdline[6] == 'i' )
            if ( fetchcmdline[7] == 'n' )
            if ( fetchcmdline[8] == 'e' )
            {
 	      fputs( "{\\bfseries " , fp5 );
 	      fputs( strcut( fetchcmdline, 8+2, strlen(fetchcmdline)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "\\\\", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }




	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 ) // !black
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'b' )
            if ( fetchcmdline[2] == 'l' )
            if ( fetchcmdline[3] == 'a' )
            if ( fetchcmdline[4] == 'c' )
            if ( fetchcmdline[5] == 'k' )
            if ( fetchcmdline[6] == ' ' )
            {
 	      fputs( "{\\color{black}" , fp5 );
 	      fputs( strcut( fetchcmdline, 6+2, strlen(fetchcmdline)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }

	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 ) // !pink 
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'p' )
            if ( fetchcmdline[2] == 'i' )
            if ( fetchcmdline[3] == 'n' )
            if ( fetchcmdline[4] == 'k' )
            if ( fetchcmdline[5] == ' ' )
            {
 	      fputs( "{\\color{mypink}" , fp5 );
 	      fputs( strcut( fetchcmdline, 5+2, strlen(fetchcmdline)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }


	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 ) // !blue
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'b' )
            if ( fetchcmdline[2] == 'l' )
            if ( fetchcmdline[3] == 'u' )
            if ( fetchcmdline[4] == 'e' )
            if ( fetchcmdline[5] == ' ' )
            {
 	      fputs( "{\\color{blue}" , fp5 );
 	      fputs( strcut( fetchcmdline, 5+2, strlen(fetchcmdline)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }




	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 ) // !red
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'r' )
            if ( fetchcmdline[2] == 'e' )
            if ( fetchcmdline[3] == 'd' )
            if ( fetchcmdline[4] == ' ' )
            {
 	      fputs( "{\\color{red}" , fp5 );
 	      fputs( strcut( fetchcmdline, 4+2, strlen(fetchcmdline)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }

	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 ) // !green
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'g' )
            if ( fetchcmdline[2] == 'r' )
            if ( fetchcmdline[3] == 'e' )
            if ( fetchcmdline[4] == 'e' )
            if ( fetchcmdline[5] == 'n' )
            if ( fetchcmdline[6] == ' ' )
            {
 	      fputs( "{\\color{green}" , fp5 );
 	      fputs( strcut( fetchcmdline, 6+2, strlen(fetchcmdline)) , fp5 );
  	      fputs( "}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }










	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 ) // !linefeed (small space) (better than medskip)
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'l' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'n' )
            if ( fetchline[4] == 'e' )
            if ( fetchline[5] == 'f' )
            if ( fetchline[6] == 'e' )
            if ( fetchline[7] == 'e' )
            if ( fetchline[8] == 'd' )
            {
 	      fputs( "\\vskip " , fp5 );
  	      fputs( "0.03in", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }








	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'b' )
            if ( fetchcmdline[2] == 'c' )
            if ( fetchcmdline[3] == 'o' )
            if ( fetchcmdline[4] == 'd' )
            if ( fetchcmdline[5] == 'e' )
            {
 	      fputs( "\\begin{verbatim}\n" , fp5 );
  	      foundcode = 1;
            }
	    /////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'e' )
            if ( fetchcmdline[2] == 'c' )
            if ( fetchcmdline[3] == 'o' )
            if ( fetchcmdline[4] == 'd' )
            if ( fetchcmdline[5] == 'e' )
            {
 	      fputs( "\\end{verbatim}\n" , fp5 );
  	      foundcode = 1;
            } 








	    /////////////////
	    /////////////////
	    /////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'c' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 't' )
            if ( fetchline[4] == 'e' )
            if ( fetchline[5] == ' ' )
            {
 	        fputs( "\\cite{" , fp5 );
 	        fputs( strtrim( strcut( fetchline, 5+2, strlen( fetchline ))) , fp5 );
  	        fputs( "}\n", fp5 );
  	        foundcode = 1;
            } 
	    /////////////////
	    /////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 't' )
            if ( fetchline[2] == 'o' )
            if ( fetchline[3] == 'r' )
            {
                 fputs( "\\bibliography{mybib}{}\n", fp5 );
                 fputs( "\\bibliographystyle{ieeetr}\n", fp5 );
  	         foundcode = 1;
            } 
            //////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'b' )
            if ( fetchline[2] == 'i' )
            if ( fetchline[3] == 'b' )
            if ( fetchline[4] == 'l' )
            if ( fetchline[5] == 'i' )
            if ( fetchline[6] == 'o' )
            if ( fetchline[7] == 'g' )
            if ( fetchline[8] == 'r' )
            if ( fetchline[9] == 'a' )
            if ( fetchline[10] == 'p' )
            if ( fetchline[11] == 'h' )
            if ( fetchline[12] == 'y' )
            {
                 fputs( "\\bibliography{mybib}{}\n", fp5 );
                 fputs( "\\bibliographystyle{ieeetr}\n", fp5 );
  	         foundcode = 1;
            } 




            ////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'r' ) 
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'v' ) 
            if ( fetchline[4] == ' ' ) 
            ///////// new! for review !rev my-review
            {
       	         fputs( "\\textcolor{blue}{" , fp5 );
        	 fputs( "[", fp5 );
       	         fputs( strcut( fetchline, 4+2, strlen(fetchline)) , fp5 );
        	 fputs( "]", fp5 );
        	 fputs( "}", fp5 );
        	 fputs( "\n", fp5 );
  	         foundcode = 1;
            }










	    /////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'b' )
	    if ( strcmp( fetchcmdline, "!bverbatim" ) == 0 )
            {
 	      fputs( "\\begin{verbatim}" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }
	    /////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
            if ( fetchcmdline[1] == 'e' )
	    if ( strcmp( fetchcmdline, "!everbatim" ) == 0 )
            {
 	      fputs( "\\end{verbatim}" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }


	    /////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
	    if ( strcmp( fetchline, "!smallskip" ) == 0 )
            {
 	      fputs( "\\smallskip" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }


	    /////////////////
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' )
	    if ( strcmp( fetchcmdline, "!bigskip" ) == 0 )
            {
 	      fputs( "\\bigskip" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }




            //// !maketitle
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' ) 
	    if ( strcmp( fetchcmdline, "!maketitle" ) == 0 ) //here begin
            {
 	      fputs( "\\maketitle" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }




            //// !clr
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' ) 
	    if ( strcmp( fetchcmdline, "!clr" ) == 0 ) //here begin
            {
 	      fputs( "\\clearpage" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }

       



          /*
            //// ! clr ! Bigskip from converting
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' ) 
	    if ( strcmp( fetchcmdline, "! clr" ) == 0 ) //here begin
            {
 	      fputs( "\\clearpage" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }
            //// ! clr ! Bigskip from converting
            if ( foundcode == 0 )
            if ( fetchcmdline[0] == '!' ) 
	    if ( strcmp( fetchcmdline, "! Bigskip" ) == 0 ) //here begin
            {
 	      fputs( "\\bigskip" , fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }
         */







            ///////////////////
            ///////////////////
            ///////////////////
            ///////////////////
            if ( foundcode == 0 ) // !gback{} //for beamer background
            if ( 
            ( fetchline[0] ==  '!' ) && ( fetchline[1] == 'g' ) && ( fetchline[2] == 'b' ) && 
              ( fetchline[3] == 'a' ) && ( fetchline[4] == 'c' ) && ( fetchline[5] == 'k' ) && ( fetchline[6] == '{' )

            )
            {
	      char fileinputsrc[PATH_MAX];
	      strncpy( fileinputsrc, "", PATH_MAX );
              strncat( fileinputsrc , strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX - strlen( fileinputsrc ) -1 );
              //#elif defined(_WIN32)
              //#elif defined(_WIN64)
	      if ( fileinputsrc[0] == '~' )
	      {
	         strncpy( fileinputsrc, "", PATH_MAX );
                 strncat( fileinputsrc , getenv( "HOME" ) , PATH_MAX - strlen( fileinputsrc ) -1 );
                 strncat( fileinputsrc , strdelimit( fetchline,  '~' ,'}' ,  1 ) , PATH_MAX - strlen( fileinputsrc ) -1 );
	      }
                
              if ( fileinputsrc[ strlen( fileinputsrc) -1 ] != '/' )
              {
                //printf( "graphicspath: PC must add / for tailing\n" );
                //strncat( fileinputsrc , "/" , PATH_MAX - strlen( fileinputsrc ) -1 );
              }
              /// fputs( "\\graphicspath{{", fp5 );
              //fputs/\usebackgroundtemplate{\includegraphics[width=\paperwidth,height=\paperheight]{~/figs/beamer.jpg}}
              fputs( "\\usebackgroundtemplate{\\includegraphics[width=\\paperwidth,height=\\paperheight]{", fp5 );
  	      fputs( fileinputsrc  , fp5 );
  	      fputs( "}}", fp5 );
  	      fputs( "\n", fp5 );

              strncpy( mygraphicspath, fileinputsrc , PATH_MAX );
  	      foundcode = 1;
            }









            ///////////////////////////
            ///////////////////////////
            ///////////////////////////
            if ( foundcode == 0 ) // !gpath{}
            if (
            (( fetchline[0] ==  '!' ) && ( fetchline[1] == 'g' ) && ( fetchline[2] == 'p' ) && ( fetchline[3] == 'a' ) && ( fetchline[4] == 't' ) && ( fetchline[5] == 'h' ) && ( fetchline[6] == '{' ))

            || ( ( fetchline[0] ==  '!' ) && ( fetchline[1] == 'g' ) && ( fetchline[2] == 'f' ) && ( fetchline[3] == 'x' ) && ( fetchline[4] == 'p' ) && ( fetchline[5] == 'a' )  && ( fetchline[6] == 't' ) && ( fetchline[7] == 'h' )  && ( fetchline[8] == '{' ))
            )
            {
	      char fileinputsrc[PATH_MAX];
	      strncpy( fileinputsrc, "", PATH_MAX );
              strncat( fileinputsrc , strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX - strlen( fileinputsrc ) -1 );
	      if ( fileinputsrc[0] == '~' )
	      {
	         strncpy( fileinputsrc, "", PATH_MAX );
                 strncat( fileinputsrc , getenv( "HOME" ) , PATH_MAX - strlen( fileinputsrc ) -1 );
                 strncat( fileinputsrc , strdelimit( fetchline,  '~' ,'}' ,  1 ) , PATH_MAX - strlen( fileinputsrc ) -1 );
	      }
                
              if ( fileinputsrc[ strlen( fileinputsrc) -1 ] != '/' )
              {
                printf( "graphicspath: PC must add / for tailing\n" );
                strncat( fileinputsrc , "/" , PATH_MAX - strlen( fileinputsrc ) -1 );
              }
              fputs( "\\graphicspath{{", fp5 );
  	      fputs( fileinputsrc  , fp5 );
  	      fputs( "}}", fp5 );
  	      fputs( "\n", fp5 );

              strncpy( mygraphicspath, fileinputsrc , PATH_MAX );
  	      foundcode = 1;
            }

            ///////////// !gpath{ath.png}
            if ( foundcode == 0 ) // security mode
            if ( fetchline[0] == '!' ) 
            if ( fetchline[1] == 'g' )
            if ( fetchline[2] == 'p' )
            if ( fetchline[3] == 'a' )
            if ( fetchline[4] == 't' )
            if ( fetchline[5] == 'h' )
            if ( fetchline[6] == '{' )
            {
              fputs( "%%%%%% !gpath-(2)\n", fp5 );
              fputs( "\\graphicspath{{", fp5 );
 	      fputs( strdelimit( fetchline,  '{' ,'}' ,  1 ) , fp5 );
  	      fputs( "}}", fp5 );
  	      fputs( "\n", fp5 );
  	      foundcode = 1;
            }








            ///////////////////////////
            ///////////////////////////
            ///////////////////////////
            ///////////////////////////
            if ( foundcode == 0 ) // !ipath{}
            if (( fetchline[0] ==  '!' ) && ( fetchline[1] == 'i' ) && ( fetchline[2] == 'p' ) && ( fetchline[3] == 'a' ) && ( fetchline[4] == 't' ) && ( fetchline[5] == 'h' ) && ( fetchline[6] == '{' ))

            { 
	      char fileinputsrc[PATH_MAX];
	      strncpy( fileinputsrc, "", PATH_MAX );
              strncat( fileinputsrc , strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX - strlen( fileinputsrc ) -1 );
	      if ( fileinputsrc[0] == '~' )
	      {
	         strncpy( fileinputsrc, "", PATH_MAX );
                 strncat( fileinputsrc , getenv( "HOME" ) , PATH_MAX - strlen( fileinputsrc ) -1 );
                 strncat( fileinputsrc , strdelimit( fetchline,  '~' ,'}' ,  1 ) , PATH_MAX - strlen( fileinputsrc ) -1 );
	      }
              if ( fileinputsrc[ strlen( fileinputsrc) -1 ] != '/' )
              {
                printf( "> path: PC must add / for tailing\n" );
                strncat( fileinputsrc , "/" , PATH_MAX - strlen( fileinputsrc ) -1 );
              }
              strncpy( myinputspath, fileinputsrc , PATH_MAX );
              printf( "> SET IPATH: %s\n", myinputspath );
  	      foundcode = 1;
            }
















        //////////////////////////////////////////////
	/// always working
        //////////////////////////////////////////////
            if ( foundcode == 0 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'm' )
            if ( fetchline[2] == 's' )
            if ( fetchline[3] == 'g' )
            if ( fetchline[4] == '{' )
            {
	      // !msg   
 	      strncpy( usertextfieldone, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
 	      strncpy( usertextfieldtwo, strdelimit( fetchline,  '{' ,'}' ,  2 ) , PATH_MAX );
	      if ( strcmp( usertextfieldone, "" ) != 0 )
	      if ( strcmp( usertextfieldtwo, "0" ) != 0 )
	        printf( "<USER MSG: %s>\n", usertextfieldone );
  	      foundcode = 1;
            }







        //////////////////////////////////////////////
            if ( foundcode == 0 )
	    if ( option_system_call == 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'i' )
            if ( fetchline[2] == 'e' )
            if ( fetchline[3] == 'x' )
            if ( fetchline[4] == '{' )
            {
	      // !iex for texiex     (it will convert an iex to tex, and create its tex) 
 	      strncpy( usertextfieldone, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
 	      strncpy( usertextfieldtwo, strdelimit( fetchline,  '{' ,'}' ,  2 ) , PATH_MAX );
	      if ( strcmp( usertextfieldone, "" ) != 0 )
	      if ( strcmp( usertextfieldtwo, "0" ) != 0 )
	      {
                 strncpy( wgetcmd, "",  PATH_MAX );
                 strncat( wgetcmd , " texiex  " , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  "  \""  , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  usertextfieldone  , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  "\""  , PATH_MAX - strlen( wgetcmd ) -1 );
   	         printf( " texiex : %s\n" , wgetcmd );
	         // secured or unsecured??
                 if ( option_system_call == 1 )
		    system( wgetcmd );
	      }
  	      foundcode = 1;
            }




        //////////////////////////////////////////////
        /*
            if ( foundcode == 0 )
	    if ( option_system_call == 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'x' )
            if ( fetchline[2] == 'm' )
            if ( fetchline[3] == 's' )
            if ( fetchline[4] == 'g' )
            if ( fetchline[5] == '{' )
            {
	      // !xmsg   
 	      strncpy( usertextfieldone, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
 	      strncpy( usertextfieldtwo, strdelimit( fetchline,  '{' ,'}' ,  2 ) , PATH_MAX );
	      if ( strcmp( usertextfieldone, "" ) != 0 )
	      if ( strcmp( usertextfieldtwo, "0" ) != 0 )
	      {
                 strncpy( wgetcmd, "",  PATH_MAX );
                 strncat( wgetcmd , " xmsg  " , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  "  \""  , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  usertextfieldone  , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  "\""  , PATH_MAX - strlen( wgetcmd ) -1 );
   	         printf( " xmsg : %s\n" , wgetcmd );
	         // secured or unsecured??
                 if ( option_system_call == 1 )
		    system( wgetcmd );
	      }
  	      foundcode = 1;
            }
         */





        //////////////////////////////////////////////
            if ( foundcode == 0 )
	    if ( option_system_call == 1 )
            if ( fetchline[0] == '!' )
            if ( fetchline[1] == 'w' )
            if ( fetchline[2] == 'g' )
            if ( fetchline[3] == 'e' )
            if ( fetchline[4] == 't' )
            if ( fetchline[5] == '{' )
            {
	      // !wget
 	      strncpy( usertextfieldone, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
 	      strncpy( usertextfieldtwo, strdelimit( fetchline,  '{' ,'}' ,  2 ) , PATH_MAX );
	      if ( strcmp( usertextfieldone, "" ) != 0 )
	      if ( strcmp( usertextfieldtwo, "" ) != 0 )
	      if ( fexist( usertextfieldtwo ) == 0 )
	      {
                 strncpy( wgetcmd, "",  PATH_MAX );
                 strncat( wgetcmd , " wget " , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  "  \""  , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  usertextfieldone  , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  "\""  , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd , " -O "  , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  "  \""  , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  usertextfieldtwo  , PATH_MAX - strlen( wgetcmd ) -1 );
                 strncat( wgetcmd ,  "\" "  , PATH_MAX - strlen( wgetcmd ) -1 );
   	         printf( " wgetcmd: %s\n" , wgetcmd );
	         // secured or unsecured??
                 if ( option_system_call == 1 )
   	         system( wgetcmd );
	      }
  	      foundcode = 1;
            }



        /*
        //////////////////////////////////////////////
            if ( foundcode == 0 )
	    if ( option_system_call == 1 )
            if ( ( ( fetchline[0] == '!' )
            && ( fetchline[1] == 'd' )
            && ( fetchline[2] == 'i' )
            && ( fetchline[3] == 'r' )
            && ( fetchline[4] == 'f' )
            && ( fetchline[5] == 'i' )
            && ( fetchline[6] == 'g' )
            && ( fetchline[7] == '{' ))
            ||
            ( ( fetchline[0] == '!' )   // figdir if you want to
            && ( fetchline[1] == 'f' )
            && ( fetchline[2] == 'i' )
            && ( fetchline[3] == 'g' )
            && ( fetchline[4] == 'd' )
            && ( fetchline[5] == 'i' )
            && ( fetchline[6] == 'r' )
            && ( fetchline[7] == '{' ))
            )
            {
   	      printf( "EXTCMD: dirfig\n" );
	      // !dirfig{directory/}{1}  to active or not
 	      strncpy( usertextfieldone, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
 	      strncpy( usertextfieldtwo, strdelimit( fetchline,  '{' ,'}' ,  2 ) , PATH_MAX );
	      if ( strcmp( usertextfieldone, "" ) != 0 )
	      if ( strcmp( usertextfieldtwo, "0" ) != 0 )
	      if ( fexist( usertextfieldone ) == 2 )
	      {
   	         printf( " dirfig found directory: %s \n", usertextfieldone );
                 strncpy( extcmd, "",  PATH_MAX );
                 strncat( extcmd , " dirfig " , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  "  \""  , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  usertextfieldone  , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  "\""  , PATH_MAX - strlen( extcmd ) -1 );
   	         printf( " ndirfig: %s\n" , extcmd );
	         // secured or unsecured??
                 if ( option_system_call == 1 ){
   	           printf( "CMD:%s\n", extcmd );
   	           system( extcmd ); }
	      }
  	      foundcode = 1;
            }
            */







        //////////////////////////////////////////////
            if ( foundcode == 0 ) // xfig
            if ( ( ( fetchline[0] == '!' )
            && ( fetchline[1] == 'x' )
            && ( fetchline[2] == 'f' )
            && ( fetchline[3] == 'i' )
            && ( fetchline[4] == 'g' )
            && ( fetchline[5] == '{' ))
            )
            {
   	      printf( "EXTCMD: xfig\n" );
 	      strncpy( usertextfieldone, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
 	      //strncpy( usertextfieldtwo, strdelimit( fetchline,  '{' ,'}' ,  2 ) , PATH_MAX );
	      if ( strcmp( usertextfieldone, "" ) != 0 )
	      if ( fexist( usertextfieldone ) == 1 )
	      {
                 strncpy( usertextfieleps , fbasenoext( usertextfieldone ) , PATH_MAX );
                 strncat( usertextfieleps , ".eps" , PATH_MAX - strlen( usertextfieleps ) -1 );
   	         printf( " xfig found file: %s \n", usertextfieldone );
                 strncpy( extcmd, "",  PATH_MAX );
                 strncat( extcmd , " fig2dev -L eps " , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  "  \""  , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  usertextfieldone  , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  "\""  , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  "  \""  , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  usertextfieleps  , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  "\""  , PATH_MAX - strlen( extcmd ) -1 );
   	         printf( " xfig: %s\n" , extcmd );
                 {
   	           printf( "CMD:%s\n", extcmd );
   	           system( extcmd ); 
                 }
                 if ( fexist( usertextfieleps ) == 1 )
                 {
         	        fputs( "\\begin{center}\n", fp5 );
         	        fputs( "\\includegraphics[width=1.0\\textwidth]{" , fp5 );
        	        fputs( usertextfieleps , fp5 );
         	        fputs( "}\n", fp5 );
         	        fputs( "\\end{center}\n", fp5 );
                 }
	      }
  	      foundcode = 1;
            }







  /*
        //////////////////////////////////////////////
        // pstoedit  -f fig file.eps  > file.fig
            if ( foundcode == 0 )  // eps2xfig from directory ~/pool/figs/ to local directory
            if ( ( ( fetchline[0] == '!' )
            && ( fetchline[1] == 'e' )
            && ( fetchline[2] == 'p' )
            && ( fetchline[3] == 's' )
            && ( fetchline[4] == '2' )
            && ( fetchline[5] == 'f' )
            && ( fetchline[6] == 'i' )
            && ( fetchline[7] == 'g' )
            && ( fetchline[8] == '{' ))
            )
            {
   	      printf( "EXTCMD: eps2fig script from pool to local dir \n" );
 	      strncpy( usertextfieldone, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
	      if ( strcmp( usertextfieldone, "" ) != 0 )
	      //if ( fexist( usertextfieldone ) == 1 )  // to be updated
	      {
                 strncpy( usertextfieleps , fbasenoext( usertextfieldone ) , PATH_MAX );
                 strncat( usertextfieleps , ".fig" , PATH_MAX - strlen( usertextfieleps ) -1 );  // target
   	         printf( " pstoedit found file: %s \n", usertextfieldone );
                 strncpy( extcmd, "",  PATH_MAX );
                 strncat( extcmd , " pstoedit -f fig  " , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  "  ~/pool/figs/\""  , PATH_MAX - strlen( extcmd ) -1 );  // to be updated
                 strncat( extcmd ,  usertextfieldone  , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  "\""  , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  "  \""  , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  usertextfieleps  , PATH_MAX - strlen( extcmd ) -1 );
                 strncat( extcmd ,  "\""  , PATH_MAX - strlen( extcmd ) -1 );
   	         printf( "CMD:%s\n", extcmd );
   	         system( extcmd ); 
	      }
  	      foundcode = 1;
            }
    */








        //////////////////////////////////////////////
        // fetch from ~/pool/figs  // !fetch 
        //////////////////////////////////////////////
            if ( foundcode == 0 )  
            if ( ( ( fetchline[0] == '!' )
            && ( fetchline[1] == 'f' )
            && ( fetchline[2] == 'e' )
            && ( fetchline[3] == 't' )
            && ( fetchline[4] == 'c' )
            && ( fetchline[5] == 'h' )
            && ( fetchline[6] == '{' ))
            )
            {
   	      printf( "Int cmd: fetch with ncp script for image from pool to local dir \n" );
 	      strncpy( usertextfieldone, strdelimit( fetchline,  '{' ,'}' ,  1 ) , PATH_MAX );
   	      printf( "  => file: %s\n", usertextfieldone ); 
	      if ( strcmp( usertextfieldone, "" ) != 0 )
	      {
                  strncpy( usertextfieleps , fbasename( usertextfieldone ) , PATH_MAX );
                  //strncpy( usertextfieleps , fbasenoext( usertextfieldone ) , PATH_MAX );
                  //strncat( usertextfieleps , ".fig" , PATH_MAX - strlen( usertextfieleps ) -1 );  // target
    	          char filencp[PATH_MAX];
    	          strncpy( filencp, "", PATH_MAX );
                  strncat( filencp , getenv( "HOME" ) , PATH_MAX - strlen( filencp ) -1 );
                  strncat( filencp , "/pool/figs/" , PATH_MAX - strlen( filencp ) -1 );
                  strncat( filencp ,  usertextfieldone  , PATH_MAX - strlen( filencp ) -1 );
                  printf( " [ncp] dest:%s <= src:%s \n", usertextfieleps, filencp );  
                  if ( fexist( filencp ) == 1 )
                  if ( fexist( usertextfieleps ) != 2 )
                  {
                    ncp( usertextfieleps, filencp ); 
                  }
                  else 
                  {
                    printf( " file not found: %s \n", filencp ); 
                  }
	      }
  	      foundcode = 1;
            }




            // !fin !fin 
	    if ( foundcode == 0 )
	    {
	        if ( beginitemize == 1 )
		{
		  fputs( "\\iexitemend\n" , fp5 ); 
		  fputs( "\n" , fp5 ); 
		}

 	        fputs( fetchline , fp5 );

                if ( markup_output_format == 2 )
 	            fputs( "<br>" , fp5 );

  	        fputs( "\n", fp5 );
	        beginitemize = 0;
            }

	 }
     }
     fclose( fp5 );
     fclose( fp6 );
   }
}










/*
///////////////////////////////////
int question_continue()
{
	    char c;
	    int foo;
	    printf("Do you want to continue (y/n)? ");
	    scanf(" %c",&c); 
	    c = toupper(c);
	    if   ( ( c == 'Y' ) ||    ( c == 'y' ) )
	    {
	       printf( "Yes\n");
	       foo = 1;
	    }
	    else if ( c == '1' ) 
	    {
	       printf( "Yes\n");
	       foo = 1;
	    }
	    else  
	    {
	      printf( "No\n");
	      foo = 0;
	    }
	    return foo; 
}
*/





void show_unimark_logo()
{
         printf("================= \n");
         printf("|||- UNIMARK -||| \n");
         printf("================= \n");
}







////////////////////////////////
/// check for enddoc 
////////////////////////////////
int cat_fdinfo_end( char *filein )
{
  char fetchlinetmp[4096];
  char fetchline[4096];
  char buf[4096];
  char ptr[4096];
  ssize_t nread;
  int i,j,k;
  i = 0 ; j = 0;
  int beginend = 0;
    FILE *fp6;
    fp6 = fopen( filein , "rb");
    while( !feof(fp6) ) 
    {
          fgets(fetchlinetmp, 4096, fp6); 
          strncpy( fetchline, "" , 4096 );
          for( i = 0 ; ( i <= strlen( fetchlinetmp ) ); i++ )
            if ( fetchlinetmp[ i ] != '\n' )
              fetchline[i]=fetchlinetmp[i];

         if ( !feof(fp6) )
         {
              if ( strcmp( fetchline , "!enddoc" ) == 0 )
                   beginend++;
              else if ( strcmp( fetchline , "\\end{document}" ) == 0 ) 
                   beginend++;
              else if ( strcmp( fetchline , "!EOF" ) == 0 )
                   beginend++;
	 }
     }
     fclose( fp6 );

  printf( "Info Begin+End: %d \n" , beginend );
  return beginend;
}

int cat_fdinfo_begin( char *filein )
{
  char fetchlinetmp[4096];
  char fetchline[4096];
  char buf[4096];
  char ptr[4096];
  ssize_t nread;
  int i,j,k;
  i = 0 ; j = 0;
  int beginend = 0;
    FILE *fp6;
    fp6 = fopen( filein , "rb");
    while( !feof(fp6) ) 
    {
          fgets(fetchlinetmp, 4096, fp6); 
          strncpy( fetchline, "" , 4096 );
          for( i = 0 ; ( i <= strlen( fetchlinetmp ) ); i++ )
            if ( fetchlinetmp[ i ] != '\n' )
              fetchline[i]=fetchlinetmp[i];

         if ( !feof(fp6) )
         {
              if ( strcmp( fetchline , "!begin" ) == 0 )
                   beginend++;
              else if ( strcmp( fetchline , "\\begin{document}" ) == 0 ) 
                   beginend++;
	 }
     }
     fclose( fp6 );

  printf( "Info Begin+End: %d \n" , beginend );
  return beginend;
}












//////////////////////////////
 void npdflatex( char *thefile ) 
 {
       char tfileout[PATH_MAX];
       strncpy( tfileout ,   fbasenoext( thefile ) , PATH_MAX );
       strncat( tfileout , ".tex" , PATH_MAX - strlen( tfileout ) -1 );
       nrunwith( " unibeam ", thefile );
       nrunwith( " pdflatex -shell-escape ", tfileout );
 }

//////////////////////////////
 void ntexmake( char *thefile , int ntexmakemode )
 {
       ///////////////////////////
       /*
       unibeam article.mrk article.tex 
       pdflatex article ;
       bibtex article 
       pdflatex article 
       pdflatex article 
       mupdf article.pdf   */
       //strncpy( theout, strtimestamp() , PATH_MAX );
       //char theout[PATH_MAX];
       ///////////////////////////
       char targetfile[PATH_MAX];
       char tfilein[PATH_MAX];
       char tfileout[PATH_MAX];
       char cmdi[PATH_MAX];
         strncpy( targetfile, fbasenoext( thefile ) , PATH_MAX );
         strncpy( tfileout,   fbasenoext( thefile ) , PATH_MAX );
         strncat( tfileout , ".pdf" , PATH_MAX - strlen( tfileout ) -1 );
         if ( ntexmakemode == 1 )       nrunwith( " unibeam ",  thefile );
         else if ( ntexmakemode == 2 )  nrunwith( " ./unibeam ",  thefile );
         nrunwith( " pdflatex ", targetfile );
         nrunwith( " bibtex ",   targetfile );
         nrunwith( " pdflatex ", targetfile );
         nrunwith( " pdflatex ", targetfile );
         nrunwith( " mupdf ",    tfileout );
 }











////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[])
{

    /// 
    strncpy( strlabel, "", PATH_MAX );
    strncpy( lastsubsection, "", PATH_MAX );
    strncpy( cursubsection , "", PATH_MAX );

    ////////////////////////////////////////////////////////
    if ( argc >= 2)
    if ( strcmp(argv[1], "--version" ) == 0)
    {
	  printf( "Unibeam version: %s\n", VERSIONNBR );
          return 0;
    }

    ////////////////////////////////////////////////////////
    if ( argc == 3)
    if ( strcmp( argv[1], "--filepath" ) == 0)
    {
          printf( "Filepath: %s\n", fbasewithoutfilename( argv[ 2 ] ) );
          return 0;
    }


    char targetfile[PATH_MAX];
    char cmdi[PATH_MAX];
    strncpy( slidemysection, "", PATH_MAX );
    strncpy( mygraphicspath, "" , PATH_MAX );
    strncpy( myinputspath , "" , PATH_MAX );
    FILE *fpout;
    int i ; 

    //////////////////////////////////////////
    if ( argc >= 2)
    if ( strcmp( argv[ 1 ] , "--debug"  ) == 0  ) 
    if ( strcmp( argv[ 2 ] , "hello"  ) == 0  ) 
    {
	  printf( "(Unibeam Command For Dev - Debug.)\n" );
	  printf( "Hello World\n" );
          return 0; 
    }


    ////////////////////////////////////////////////////////
    if ( argc == 4)
      if ( strcmp( argv[1] , "fetch" ) ==  0 ) 
      if ( strcmp( argv[2] , "logo" ) ==  0 ) 
      {
         system( " wget \"https://raw.githubusercontent.com/spartrekus/Linux-Logos-Imgs/master/Tux-Logos/220px-Tux.png\"   -O tux.png " );
         return 0;
      }


    ///////////////////////////////////////////
    ///////////////////////////////////////////
    ///////////////////////////////////////////
    //system( " unibeam article.mrk article.tex ; pdflatex article ; bibtex article ; pdflatex article ; pdflatex article ; mupdf article.pdf " );
    // you may visit for examples: https://github.com/spartrekus/Unibeam-Article-Example
    if ( argc >= 2)
    if ( strcmp( argv[ 1 ] , "--make"  ) == 0  ) 
    {
          for( i = 2 ; i < argc ; i++) 
          {
	    printf( "%d/%d: %s\n", i, argc-1 ,  argv[ i ] );
            if ( fexist( argv[ i ] ) == 1 ) ntexmake( argv[ i ] , 1 );
          }
          return 0;
    }

    //////////////////////////////////////////
    if ( argc >= 2)
    if ( strcmp( argv[ 1 ] , "--local-make"  ) == 0  ) 
    {
          for( i = 2 ; i < argc ; i++) 
          {
	    printf( "%d/%d: %s\n", i, argc-1 ,  argv[ i ] );
            if ( fexist( argv[ i ] ) == 1 ) ntexmake( argv[ i ] , 2 );
          }
          return 0;
    }



    if ( argc >= 2)
    if ( strcmp( argv[ 1 ] , "--pdflatex"  ) == 0  ) 
    {
          for( i = 2 ; i < argc ; i++) 
          {
	    printf( "%d/%d: %s\n", i, argc-1 ,  argv[ i ] );
            if ( fexist( argv[ i ] ) == 1 ) 
                npdflatex( argv[ i ] );
          }
          return 0;
    }


   ///////////////////////////////////////////
   ///////////////////////////////////////////
    if ( argc == 4)
      if ( strcmp( argv[ 1 ] , "--new"  ) == 0  )
      if ( strcmp( argv[ 2 ] , "beamer"  ) == 0  ) 
      if ( strcmp( argv[ 3 ] , ""  ) != 0  ) 
      {
          printf( "Create example beamer\n");
          fpout = fopen( argv[3] , "ab+");
            fputs( "#include{beamer.mrk}\n", fpout );
            fputs( "!gpath{figs}\n", fpout );
            fputs( "///!gback{~/include/img/beamer.jpg}\n", fpout );
            fputs( "!utf\n", fpout );
            fputs( "!begin\n", fpout );
            fputs( "!beamer\n", fpout );
            fputs( "!clr\n", fpout );
            fputs( "!= Section\n", fpout );
            fputs( "> Header\n", fpout );
            fputs( "- this is an example\n", fpout );
            fputs( "// !fig{pic.png}\n", fpout );
            fputs( "\n", fpout );
          fclose( fpout );
          return 0;
      } 






   ///////////////////////////////////////////
   ///////////////////////////////////////////
    if ( argc == 3)
      if ( strcmp( argv[ 1 ] , "--new"  ) == 0  )
      if ( strcmp( argv[ 2 ] , "slides"  ) == 0 ) // nconsole
      {
          printf( "Create example beamer\n");
          fpout = fopen( "example.bmr" , "ab+");
            fputs( "#include{beamer.mrk}\n", fpout );
            fputs( "!gpath{figs}\n", fpout );
            fputs( "///!gback{~/include/img/beamer.jpg}\n", fpout );
            fputs( "!utf\n", fpout );
            fputs( "!beamer\n", fpout );
            fputs( "!begin\n", fpout );
            fputs( "!section Section\n", fpout );
            fputs( "!clr\n", fpout );
            fputs( "> Header\n", fpout );
            fputs( "- this is an example\n", fpout );
            fputs( "//!fig{fig.png}\n", fpout );
            fputs( "//!textfig{fig.png}\n", fpout );
            fputs( "//!figab{fig.png}{fig.png}\n", fpout );
            fputs( "\n", fpout );
          fclose( fpout );
          return 0;
      } 







   ///////////////////////////////////////////
   ///////////////////////////////////////////
    if ( argc == 3)
      if ( strcmp( argv[ 1 ] , "--new"  ) == 0  )
      if ( strcmp( argv[ 2 ] , "exam"  ) == 0 ) 
      {
          printf( "Create example of exam (exam.mrk)\n");
          fpout = fopen( "exam.mrk" , "ab+");
             fputs( "\n", fpout );
             fputs( "\n", fpout );
             fputs( "#include{exam.mrk}\n", fpout );
             fputs( "!gfx\n", fpout );
             fputs( "!packages\n", fpout );
             fputs( "!utf\n", fpout );
             fputs( "!gpath{~/pool/figs/}\n", fpout );
             fputs( "!begin\n", fpout );
             fputs( "!exam+\n", fpout );
             fputs( "\n", fpout );
             fputs( "\n", fpout );
          fclose( fpout );
          return 0;
      } 











    ////////////////////////////////////////////////////////
    //// this is a test for developers
    ////////////////////////////////////////////////////////
    if ( argc == 2)
      if ( strcmp( argv[ 1 ] , "filename.tst"  ) == 0  ) 
      {
          strncpy( targetfile, fbasenoext( argv[ 1 ] ) , PATH_MAX );
          strncat( targetfile , ".tex" , PATH_MAX - strlen( targetfile ) -1 );
          printf( "checker <%s> \n", targetfile );
          return 0;
      } 
    
    ////////////////////////////////////////////////////////
    //// this is a test for developers
    ////////////////////////////////////////////////////////
    if ( argc == 3)
      if ( strcmp( argv[1] , "--fdinfo" ) ==  0 ) 
      {
         cat_fdinfo_end( argv[ 2 ] );
         return 0;
      }





    ////////////////////////////////////////////////////////
    /// rescue and debugging function
    ////////////////////////////////////////////////////////
    if ( argc == 3)
      if ( strcmp( argv[1] , "--cat" ) ==  0 ) 
      if ( fexist( argv[2] )  == 1 )
      {
         ncat_static( argv[ 2 ] );
         return 0;
      }




    char cwd[PATH_MAX];
    ////////////////////////////////////////////////////////
    if ( argc == 2)
    if ( ( strcmp( argv[1] , "--readme" ) ==  0 ) 
    || ( strcmp( argv[1] , "--help" ) ==  0 ) 
    || ( strcmp( argv[1] , "-help" ) ==  0 ) )
    {
        
	 show_unimark_logo();
         return 0;
    }








    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    printf("================= \n");
    printf("|||- UNIMARK -||| \n");
    printf("================= \n");
    printf("- An Universal Markup Language - \n");
    printf("- Cross-Platform, using plain C language - \n");
    printf("- Author: GITHUB Spartrekus - \n");
    printf("- LICENCE : GNU - \n");


    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    if ( argc == 3)
    {
     printf("- Please do not modify unimark too much, since \n" );
     printf("- it is intentionally made to be universal, largely portable, and minimalist.\n" );
     printf("- Choosen Style Output : TEX -\n" );
     printf("- (Currently there is only TeX available)-\n" );
     printf("- (Future: HTML to be available)-\n" );
    }





   ///////////////////////////////////////////
   ///////////////////////////////////////////
   ///////////////////////////////////////////
   // EXAMPLE OF Universal UNIMARK DOCUMENT
   ///////////////////////////////////////////
   ///////////////////////////////////////////
   ///////////////////////////////////////////
    if ( argc >= 2)
      if ( ( strcmp( argv[ 1 ] , "--create"  ) == 0  ) 
      || ( strcmp( argv[ 1 ] , "--new"  ) == 0  )
      || ( strcmp( argv[ 1 ] , "-new"  ) == 0  )
      || ( strcmp( argv[ 1 ] , "--newfile"  ) == 0  )
      || ( strcmp( argv[ 1 ] , "--example"  ) == 0  )
      )
      {
          strncpy( cmdi , "" , PATH_MAX );
          strncpy( cmdi , strtimestamp() , PATH_MAX );
          strncat( cmdi , "-" , PATH_MAX - strlen( cmdi ) -1 );
          strncat( cmdi , "example.mrk" , PATH_MAX - strlen( cmdi ) -1 );

          if ( argc >= 3 )
            strncpy( cmdi, argv[2], PATH_MAX );

          printf( "Create example\n");
          fpout = fopen( cmdi , "ab+");
            fputs( "!tex\n", fpout );
            fputs( "!gfx\n", fpout );
            fputs( "//!gpath{figs}\n", fpout );
            fputs( "//!ipath{~/pool/mrkdir/}\n", fpout );
            fputs( "//!gpath{~/pool/figs/}\n", fpout );
            fputs( "//\\usepackage{gensymb}\n", fpout );  // for the degree 
            fputs( "//\\pagestyle{headings}\n", fpout );
            fputs( "//\\usepackage[margin=0.5cm]{geometry}\n", fpout );
            fputs( "//!placeins\n", fpout );
            fputs( "!begin\n", fpout );
            fputs( "//!bold Example \n", fpout );
            fputs( "//\\begin{center}\n", fpout );
            fputs( "//\\includegraphics[width=1.0\\textwidth]{pic.png}\n", fpout );
            fputs( "//\\end{center}\n", fpout );
            fputs( "\n", fpout );
            fputs( "hello world\n", fpout );
            fputs( "\n", fpout );
            fputs( "\n", fpout );
          fclose( fpout );
          return 0;
      } 





    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
      if ( argc == 3)
      if ( strcmp( argv[1], "--auto" ) == 0 )
      if ( fexist( argv[2] ) == 1 )
      {
          strncpy( targetfile, fbasenoext( argv[ 2 ] ) , PATH_MAX );
          strncat( targetfile , ".tex" , PATH_MAX - strlen( targetfile ) -1 );
          printf( "Target: %s\n" , targetfile );

          int therebeginline = cat_fdinfo_begin( argv[ 2 ] );
          int therebeginend =  cat_fdinfo_end(   argv[ 2 ] );

          printf("  >SRC: %s => TRG: %s \n", argv[ 2 ] , targetfile  );

          if ( therebeginline == 0 ) 
               filenew_begin( targetfile );
          else
               filenew( targetfile );

          nfileunimark( targetfile , argv[2] );

          if ( markup_output_format == 1 ) 
          if ( therebeginend == 0 )  
          {
               printf( "File Append: End (TeX) \n" );
               fileappendend( targetfile );
          }
          printf( "-End of Unimark/Unibearm (OK).-\n" );
          printf( "===============================\n" );
          printf( "\n" );

          return 0;
      }



    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
      if ( argc == 4)
      if ( strcmp( argv[1] , "--pdf" ) == 0 )
      if ( fexist( argv[2] ) == 1 )
      {
          // 1: --pdf 
          // 2: source 
          // 3: target 
          printf( "Mode with --pdf, direct resulting tex: 1 => 2 (2 args).\n" );
          printf( "STEP:Catinfo \n" );
          int therebeginend = cat_fdinfo_end( argv[ 2 ] );
          printf( "STEP: info \n" );
          printf("  >SRC: %s => TRG: %s \n", argv[2] , argv[3] );
          printf( "STEP: filenew \n" );
          filenew( argv[3] );
          printf( "STEP: unimark \n" );
          nfileunimark( argv[3] , argv[2] );
          printf( "STEP: fileappend \n" );
          strncpy( targetfile,  argv[ 3 ]  , PATH_MAX );
          printf( "Target: %s\n" , targetfile );
          if ( markup_output_format == 1 ) 
            if ( therebeginend == 0 )  
            {
               printf( "File Append: End (TeX) \n" );
               fileappendend( targetfile );
            }
          printf( "-End of Unimark/Unibearm (OK).-\n" );
          printf( "===============================\n" );
          printf( "\n" );
          return 0;
      }





    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
      if ( argc == 3)
      if ( fexist( argv[1] ) == 1 )
      {
          // 1: source 
          // 2: target 
          printf( "Mode: 1 => 2 (2 args).\n" );
          printf( "STEP:Catinfo \n" );
          int therebeginend = cat_fdinfo_end( argv[ 1 ] );
          printf( "STEP: info \n" );
          printf("  >SRC: %s => TRG: %s \n", argv[1] , argv[2] );
          printf( "STEP: filenew \n" );
          filenew( argv[2] );
          printf( "STEP: unimark \n" );
          nfileunimark( argv[2] , argv[1] );
          printf( "STEP: fileappend \n" );
          strncpy( targetfile,  argv[ 2 ]  , PATH_MAX );
          printf( "Target: %s\n" , targetfile );

          if ( markup_output_format == 1 ) 
            if ( therebeginend == 0 )  
            {
               printf( "File Append: End (TeX) \n" );
               fileappendend( targetfile );
            }
          printf( "-End of Unimark/Unibearm (OK).-\n" );
          printf( "===============================\n" );
          printf( "\n" );
          return 0;
      }





    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    if ( argc == 2)
      if ( fexist( argv[1] ) == 1 )
      {
          // 1: source 
          // 2: target, created.
          printf( "Mode: 1 => create 1 (1 args).\n" );
          strncpy( targetfile, fbasenoext( argv[ 1 ] ) , PATH_MAX );
          strncat( targetfile , ".tex" , PATH_MAX - strlen( targetfile ) -1 );
          printf( "Target: %s\n" , targetfile );

          int therebeginend  = cat_fdinfo_end( argv[ 1 ] );

          printf("  >SRC: %s => TRG: %s \n", argv[1] , targetfile  );
          filenew( targetfile );
          nfileunimark( targetfile , argv[1] );

          /// append
          if ( markup_output_format == 1 ) 
          if ( therebeginend == 0 )  
          {
               printf( "File Append: End (TeX) \n" );
               fileappendend( targetfile );
          }

          printf( "-End of Unimark/Unibearm (OK).-\n" );
          printf( "===============================\n" );
          printf( "\n" );

          return 0;
      }







    if ( argc == 2)      printf( "Usage: unimark filein.mrk fileout.tex \n" );
    else if ( argc == 1) printf( "Usage: unimark filein.mrk fileout.tex \n" );

    return 0;

}




