#include "dry.h"
#include <stdlib.h>	/* malloc */
#include "al_uart_hal.h"





/* Global Variables: */
struct tms      time_info;
Rec_Pointer     Ptr_Glob,
                Next_Ptr_Glob;
int             Int_Glob;
Boolean         Bool_Glob;
char            Ch_1_Glob,
                Ch_2_Glob;
int             Arr_1_Glob [50];
int             Arr_2_Glob [50] [50];

#ifndef REG
        Boolean Reg = false;
#define REG
        /* REG becomes defined as empty */
        /* i.e. no register variables   */
#else
        Boolean Reg = true;
#undef REG
#define REG register
#endif

Boolean		Done;

long long       Begin_Time,
                End_Time,
                User_Time;
long           Microseconds,
                Dhrystones_Per_Second;

float           Microseconds_float,
                Dhrystones_Per_Second_float;
/* end of variables for time measurement */

/* prototype declarations */
/* in this file */
void Proc_1(REG Rec_Pointer Ptr_Val_Par);
void Proc_2(One_Fifty *Int_Par_Ref);
void Proc_3(Rec_Pointer *Ptr_Ref_Par);
void Proc_4(void);
void Proc_5(void);



int main()
/*****/

  /* main program, corresponds to procedures        */
  /* Main and Proc_0 in the Ada version             */
{

    One_Fifty       Int_1_Loc;
    One_Fifty       Int_2_Loc;
    One_Fifty       Int_3_Loc;
    char            Ch_Index;
    Enumeration     Enum_Loc;
    Str_30          Str_1_Loc;
    Str_30          Str_2_Loc;
    int             Run_Index;
    long long       Number_Of_Runs = 1000000;
  printf ("Trying %d runs through Dhrystone:\r\n", Number_Of_Runs);

  uint64_t hz = AlSys_GetTimerFreq();
  /* Initializations */

  Rec_Type tmp_var1, tmp_var2; 
  Next_Ptr_Glob = &tmp_var1;
	Ptr_Glob = &tmp_var2;

  Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
  Ptr_Glob->Discr                       = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp      = 40;
  strcpy (Ptr_Glob->variant.var_1.Str_Comp, 
          "DHRYSTONE PROGRAM, SOME STRING");
  strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  Arr_2_Glob [8][7] = 10;
        /* Was missing in published program. Without this statement,    */
        /* Arr_2_Glob [8][7] would have an undefined value.             */
        /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
        /* overflow may occur for this array element.                   */

  printf ("\r\n");
  printf ("Dhrystone Benchmark, Version %s\r\n", Version);

  printf ("Program compiled without 'register' attribute\r\n");
  printf ("Using %s, HZ=%d\r\n", CLOCK_TYPE, HZ);
  printf ("\r\n");

  Done = false;
  while (!Done) {
    Arr_2_Glob [8][7] = 10;


    /***************/
    /* Start timer */
    /***************/

    Start_Timer();
    //Begin_Time = AlSys_GetTimerTickCount();
    printf ("the Begin_Time is %ld :\r\n", Begin_Time);
for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index){
		Proc_5();
		Proc_4();
 		/* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
		Int_1_Loc = 2;
		Int_2_Loc = 3;
		strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
		Enum_Loc = Ident_2;
		Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
      		/* Bool_Glob == 1 */
    		while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */ {
      			Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
        		/* Int_3_Loc == 7 */
      			Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
        		/* Int_3_Loc == 7 */
      			Int_1_Loc += 1;
    		} /* while */
      		/* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
    		Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
      		/* Int_Glob == 5 */
    		Proc_1 (Ptr_Glob);
    		for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index){
 		/* loop body executed twice */
      			if (Enum_Loc == Func_1 (Ch_Index, 'C')){
          		/* then, not executed */
        			Proc_6 (Ident_1, &Enum_Loc);
        			strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
        			Int_2_Loc = Run_Index;
        			Int_Glob = Run_Index;
        		}
    		}
	/* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
      Int_2_Loc = Int_2_Loc * Int_1_Loc;
      Int_1_Loc = Int_2_Loc / Int_3_Loc;
      Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
	/* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
      Proc_2 (&Int_1_Loc);
	/* Int_1_Loc == 5 */

    } /* loop "for Run_Index" */

    /**************/
    /* Stop timer */
    /**************/

    Stop_Timer();
    //End_Time = AlSys_GetTimerTickCount();

    if(End_Time < Begin_Time) {
      User_Time = ((-1) - Begin_Time) + End_Time;
    } else {
      User_Time = End_Time - Begin_Time;
    }

    if (User_Time < Too_Small_Time)
    {
      printf ("Measured time too small to obtain meaningful results\r\n");
      printf ("Please increase number of runs\r\n");
      Number_Of_Runs = Number_Of_Runs * 10;
      printf ("\r\n");
      printf ("%ld\r\n",User_Time);
      printf ("%ld\r\n",Too_Small_Time);
    } else Done = true;
  }

  printf ("Final values of the variables used in the benchmark:\r\n");
  printf ("\r\n");
  printf ("Int_Glob:            %d\r\n", Int_Glob);
  printf ("        should be:   %d\r\n", 5);
  printf ("Bool_Glob:           %d\r\n", Bool_Glob);
  printf ("        should be:   %d\r\n", 1);
  printf ("Ch_1_Glob:           %c\r\n", Ch_1_Glob);
  printf ("        should be:   %c\r\n", 'A');
  printf ("Ch_2_Glob:           %c\r\n", Ch_2_Glob);
  printf ("        should be:   %c\r\n", 'B');
  printf ("Arr_1_Glob[8]:       %d\r\n", Arr_1_Glob[8]);
  printf ("        should be:   %d\r\n", 7);
  printf ("Arr_2_Glob[8][7]:    %d\r\n", Arr_2_Glob[8][7]);
  printf ("        should be:   Number_Of_Runs + 10\r\n");
  printf ("Ptr_Glob->\r\n");
  printf ("  Ptr_Comp:          %p\r\n", Ptr_Glob->Ptr_Comp);
  printf ("        should be:   (implementation-dependent)\r\n");
  printf ("  Discr:             %d\r\n", Ptr_Glob->Discr);
  printf ("        should be:   %d\r\n", 0);
  printf ("  Enum_Comp:         %d\r\n", Ptr_Glob->variant.var_1.Enum_Comp);
  printf ("        should be:   %d\r\n", 2);
  printf ("  Int_Comp:          %d\r\n", Ptr_Glob->variant.var_1.Int_Comp);
  printf ("        should be:   %d\r\n", 17);
  printf ("  Str_Comp:          %s\r\n", Ptr_Glob->variant.var_1.Str_Comp);
  printf ("        should be:   DHRYSTONE PROGRAM, SOME STRING\r\n");
  printf ("Next_Ptr_Glob->\r\n");
  printf ("  Ptr_Comp:          %p\r\n", Next_Ptr_Glob->Ptr_Comp);
  printf ("        should be:   (implementation-dependent), same as above\r\n");
  printf ("  Discr:             %d\r\n", Next_Ptr_Glob->Discr);
  printf ("        should be:   %d\r\n", 0);
  printf ("  Enum_Comp:         %d\r\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
  printf ("        should be:   %d\r\n", 1);
  printf ("  Int_Comp:          %d\r\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
  printf ("        should be:   %d\r\n", 18);
  printf ("  Str_Comp:          %s\r\n",
                                Next_Ptr_Glob->variant.var_1.Str_Comp);
  printf ("        should be:   DHRYSTONE PROGRAM, SOME STRING\r\n");
  printf ("Int_1_Loc:           %d\r\n", Int_1_Loc);
  printf ("        should be:   %d\r\n", 5);
  printf ("Int_2_Loc:           %d\r\n", Int_2_Loc);
  printf ("        should be:   %d\r\n", 13);
  printf ("Int_3_Loc:           %d\r\n", Int_3_Loc);
  printf ("        should be:   %d\r\n", 7);
  printf ("Enum_Loc:            %d\r\n", Enum_Loc);
  printf ("        should be:   %d\r\n", 1);
  printf ("Str_1_Loc:           %s\r\n", Str_1_Loc);
  printf ("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\r\n");
  printf ("Str_2_Loc:           %s\r\n", Str_2_Loc);
  printf ("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\r\n");
  printf ("\r\n");

  printf ("the User_Time is %f\r\n", User_Time);
  printf ("the HZ is %f\r\n", HZ);
  printf ("the Number_Of_Runs is %f\r\n", Number_Of_Runs);

    Microseconds = (long) User_Time * Mic_secs_Per_Second 
                        / ((long) Number_Of_Runs);
    Dhrystones_Per_Second = (long) Number_Of_Runs
                        / (long) User_Time;

    Microseconds_float = (float) User_Time * Mic_secs_Per_Second 
                        /((float) Number_Of_Runs);
    Dhrystones_Per_Second_float = (float) Number_Of_Runs
                        / (float) User_Time;


    printf ("the End_Time is %ld\r\n", End_Time);
    printf ("the User_Time is %ld\r\n", User_Time);
    printf ("the HZ is %ld\r\n", HZ);
    printf ("the Number_Of_Runs is %ld\r\n", Number_Of_Runs);
    
    printf ("Microseconds for one run through Dhrystone: ");
    printf ("%ld \r\n", Microseconds);
    printf ("Dhrystones per Second:                      ");
    printf ("%ld \r\n", Dhrystones_Per_Second);

    printf ("Microseconds for one run through Dhrystone: ");
    printf ("%10.1f \r\n", Microseconds_float);
    printf ("Dhrystones per Second:                      ");
    printf ("%10.0f \r\n", Dhrystones_Per_Second_float);
    
    printf ("\r\n");
  
}


void Proc_1 (Ptr_Val_Par)
/******************/

REG Rec_Pointer Ptr_Val_Par;
    /* executed once */
{
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;  
                                        /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */
  
  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob); 
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp 
        = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp 
                        == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
    /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp, 
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7 (Next_Record->variant.var_1.Int_Comp, 10, 
           &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */


void Proc_2 (Int_Par_Ref)
/******************/
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */

One_Fifty   *Int_Par_Ref;
{
  One_Fifty  Int_Loc;  
  Enumeration   Enum_Loc;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
    if (Ch_1_Glob == 'A')
      /* then, executed */
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    } /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


void Proc_3 (Ptr_Ref_Par)
/******************/
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */

Rec_Pointer *Ptr_Ref_Par;

{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


void Proc_4 (void) /* without parameters */
/*******/
    /* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */


void Proc_5 (void) /* without parameters */
/*******/
    /* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */
