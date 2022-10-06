#include <stdlib.h>
#include <stdio.h>


/*==================================================================================*/
/*==================================================================================*/
/*New Types definitions*/
/*=========================================*/
typedef unsigned char   u8;
typedef unsigned int    u16;
/*==================================================================================*/
/*==================================================================================*/
/*Define new data structure Cell that will enable each cell to have information
 *about surrounding cells*/
typedef struct struct_cell cell;
struct struct_cell
{
    u8   Status;   //Cell status name
    cell *RCell;   //pointer to right cell
    cell *LCell;   //pointer to left cell
    cell *UpCell;  //pointer to upper cell
    cell *LowCell; //pointer to lower cell
};


/*Configurations*/
/*==================================================================================*/
/*==================================================================================*/
/*Cell Status names*/
/*=========================================*/
/* NOT_DISCOVERED      : Cell is not yet discovered by algorithm
 * MINE                : this cell is a Mine
 * NOT_MINE            : this cell is not a Mine
 * VISITED             : this cell have been visited by algorithm
 * DISCOVERED_NOT_MINE : this cell wad discovered not to be a Mine by algorithm
 * BORDER              : this cell is map border cell
 * CURRENT_LOCATION    : algorithm is currently in this cell
 */
#define NOT_DISCOVERED           '0'
#define MINE                     '*'
#define NOT_MINE                 '-'
#define VISITED                  ' '
#define DISCOVERED_NOT_MINE      'D'
#define BORDER                   '='
#define CURRENT_LOCATION         '+'

/*Map elements Size (MAP_SIZE,MAP_SIZE)*/
#define MAP_SIZE         5

/*last available cell queue size*/
#define QUEUE_SIZE       10

/*Define true and false*/
#define TRUE             1
#define FALSE            0


/*declare array of u8 number that will hold status of each input map cells*/
/*========================================================================*/
u8 cells[MAP_SIZE+2][MAP_SIZE+2] = {{'=','=','=','=','=','=','='},
                                    {'=','-','-','-','-','*','='},
                                    {'=','-','-','-','-','*','='},
                                    {'=','-','-','-','*','-','='},
                                    {'=','-','-','-','*','-','='},
                                    {'=','-','-','-','*','-','='},
                                    {'=','=','=','=','=','=','='}};
/*10 x 10 Map */
/*===================================================================================
u8 cells[MAP_SIZE+2][MAP_SIZE+2] = {{'=','=','=','=','=','=','=','=','=','=','=','='},
                                    {'=','-','*','*','*','*','-','-','-','-','*','='},
                                    {'=','-','*','-','-','-','-','-','*','-','-','='},
                                    {'=','-','*','-','-','-','-','-','-','-','-','='},
                                    {'=','-','*','-','*','-','-','-','-','-','-','='},
                                    {'=','-','-','-','-','-','-','*','-','-','*','='},
                                    {'=','-','*','-','-','-','-','-','-','-','-','='},
                                    {'=','-','*','-','-','-','*','-','-','-','-','='},
                                    {'=','-','*','-','*','-','*','-','-','*','-','='},
                                    {'=','-','*','-','-','-','-','-','-','*','-','='},
                                    {'=','-','-','-','-','-','-','-','-','*','-','='},
                                    {'=','=','=','=','=','=','=','=','=','=','=','='}};
*/

/*==================================================================================*/
/*==================================================================================*/
/*Functions Prototypes*/
/*=========================================*/

/*this function is responsible for initializing a map with its initial status value for each cell
 *as well as establishing links between each cells(initialize each cell pointers)
 *Arguments : it takes a 2D array of cell type that represent a map map[row][col]
 *Return    : void */
void voidInitializeMap(cell map[][(MAP_SIZE+2)]);

/*this function creates an Input map from predefined u8 array cells[][]
 *Arguments : it takes a 2D array of cell type that represent a map map[row][col]
 *Return    : void */
void voidCreateInputMap(cell map[][(MAP_SIZE+2)]);

/*this function prints a map passed to it
 *Arguments : it takes a 2D array of cell type that represent a map map[row][col]
 *Return    : void */
void voidPrintMap(cell map[][(MAP_SIZE+2)]);

/*this function Searches the map and uses TakeAction(); and UpdateOutputMap(); functions to do that
 *Arguments : void
 *Return    : void */
void voidSearchMap(void);

/*this function contain the algorithm used for decision making based on surrounding cell status
 *it decide which cell should be visited next
 *Arguments : void
 *Return    : void */
void voidTakeAction(void);

/*this function responsible for changing output map cell's status based on discovered input map cells
 *it also saves last available cell coordinates in case the algorithm got stuck in a dead end rout
 *it could reposition itself to a cell that has available routs
 *ie cells that is surrounded by more than one NOT_MINE Cells
 *
 *Arguments : void
 *Return    : void */
void voidUpdateOutputMap(void);

/*this function reposition current cell position to last available cell so that
 *search algorithm can take new rout
 *Arguments : void
 *Return    : void */
void voidBackPropagate(void);


/*this function reposition current cell position to Right Cell
 *Arguments : void
 *Return    : void */
void voidGotoRightCell(void);

/*this function reposition current cell position to Left Cell
 *Arguments : void
 *Return    : void */
void voidGotoLeftCell(void);

/*this function reposition current cell position to Upper Cell
 *Arguments : void
 *Return    : void */
void voidGotoUppertCell(void);

/*this function reposition current cell position to Lower Cell
 *Arguments : void
 *Return    : void */
void voidGotoLowerCell(void);



/*==================================================================================*/
/*==================================================================================*/
/*Global variable*/
/*=========================================*/

/*Maps*/
/*=========================*/
/*I/P map of type cell*/
cell Inputmap[MAP_SIZE+2][MAP_SIZE+2];
/*O/P map of type cell*/
cell Outputmap[MAP_SIZE+2][MAP_SIZE+2];
/*=========================*/

/*Cell pointer that will be used to point to input map current position
 *and will be used to navigate through input map and collect information about cells*/
cell *CurrentInputCell  = NULL;
/*Cell pointer that will be used to point to output map current position
 *and will be used to navigate through output map*/
cell *CurrentOutputCell = NULL;


/* Note remember to chose queue size that match array size for bigger map you will need a bigger queue size
 * to match it other wise last available number will exceed queue size*/
/*Pointer to Last available input map cell*/
cell *LastAvailableInputCell[QUEUE_SIZE] = {NULL};
/*Pointer to Last available output map cell*/
cell *LastAvailableOutputCell[QUEUE_SIZE]= {NULL};




/*Dead end variable that is used to terminate Search*/
u8 DeadendCondition = FALSE;

/*Number of visited Cells variable*/
u16 visited_cells =0;

/*Number of available Cells variable*/
u16 NumberOfAvailableCells =0;

/*==================================================================================*/
/*==================================================================================*/

u8 main(void)
{
    /*Initialize Input map*/
    voidInitializeMap(Inputmap);
    /*Initialize Output map*/
    voidInitializeMap(Outputmap);
    /*Create Input map*/
    voidCreateInputMap(Inputmap);
    /*print Input map*/
    printf("Input Map :\n"
           "==============================\n");
    voidPrintMap(Inputmap);
    /*Search input map*/
    voidSearchMap();
    scanf("%d");
}

/*==================================================================================*/
/*==================================================================================*/
/*Function Implementations*/

/*this function is responsible for initializing a map with its initial status value for each cell
 *as well as establishing links between each cells(initialize each cell pointers)
 *Arguments : it takes a 2D array of cell type that represent a map map[row][col]
 *Return    : void */
void voidInitializeMap(cell map[][(MAP_SIZE+2)])
{
    /*2 loops to go through all map cells*/
     for(u8 i = 0 ; i<(MAP_SIZE+2) ; i++)
    {
        for(u8 j = 0 ; j<(MAP_SIZE+2) ; j++)
        {
            /*Border cell Check*/
            if( (0 == i) || (0 == j) || ((MAP_SIZE+1) ==i) || ((MAP_SIZE+1) ==j) )
            {
                /*if border cell fill it with BORDER status*/
                map[i][j].Status =  BORDER;
            }
            else
            {
                /*if not a border cell fill it with NOT_DISCOVERED status
                 * and create links with surrounding cell by initializing 4 cell pointers*/
                map[i][j].Status =  NOT_DISCOVERED;
                map[i][j].RCell   = &map[i][j+1];
                map[i][j].LCell   = &map[i][j-1];
                map[i][j].UpCell  = &map[i+1][j];
                map[i][j].LowCell = &map[i-1][j];
            }/*end of border cell Check*/

        }/*end of j for*/
    }/*end of i for*/

}/*end of voidInitializeMap()*/

/*this function creates an Input map from predefined u8 array cells[][]
 *Arguments : it takes a 2D array of cell type that represent a map map[row][col]
 *Return    : void */
void voidCreateInputMap(cell map[][(MAP_SIZE+2)])
{
    /*2 loops to go through all map cells from (1,1) cell to (MAP_SIZE,MAP_SIZE)*/
    for(u8 i=1;i<(MAP_SIZE+1);i++)
    {
        for(u8 j=1;j<(MAP_SIZE+1);j++)
        {
            /*fill input map cell status value with cells array value*/
            map[i][j].Status  = cells[i][j];

        }/*end of j for*/
    }/*end of i for*/

}/*end of voidCreateInputMap();*/

/*this function prints a map passed to it
 *Arguments : it takes a 2D array of cell type that represent a map map[row][col]
 *Return    : void */
void voidPrintMap(cell map[][(MAP_SIZE+2)])
{
    /*2 loops to go through all map cells from (0,0) cell to (MAP_SIZE+2,MAP_SIZE+2)
     *to print all cells including Borders */
    for(u8 i=0;i<(MAP_SIZE+2);i++)
    {
        for(u8 j=0;j<(MAP_SIZE+2);j++)
        {
            /*Print cell*/
            printf("%c ",map[i][j].Status);
        }/*end of j for*/

        /*Print new line after each row*/
        printf("\n");

    }/*end of i for*/
}

/*this function uses TakeAction(); and UpdateOutputMap(); functions to search map until the whole map is discovered
 *Arguments : void but it manipulate a global variable
 *Return    : void */
void voidSearchMap(void)
{

    /*loop first row in input map*/
    for(u8 i = 1; i < (MAP_SIZE) ; i++)
    {
        /*first row check*/
        if( NOT_MINE == Inputmap[1][i].Status )
        {
            /*Make the first available cell in the row the entry point for the search*/
            /*visit that cell and update it's status by initializing searching input and output pointers*/
            /*=============================================*/
            CurrentInputCell  = &Inputmap[1][i];
            CurrentOutputCell = &Outputmap[1][i];
            /*=============================================*/
            /*Increment number of visited cells*/
            visited_cells++;
            /*Break for loop*/
            break;

        }/*end of first row check*/

    }/*end of for loop*/

    /*Check CurrentInputCell value*/
    if( NULL == CurrentInputCell )
    {
        /* if CurrentInputCell value is null that means that the first row
         * in input map is full of mines then print the following message and terminate program*/
        printf("First row is full of mines please rearrange another map\n");
        /*terminate program*/
        exit(0);

    }/*end of CurrentInputCell value Check*/

    /*print map before start searching*/
    /*=============================================*/
    printf("Output Map before search :\n"
           "==============================\n");
    voidPrintMap(Outputmap);
    /*=============================================*/



    /*loop these steps until dead end is reached*/
    while(!DeadendCondition)
    {
        /*update output map status*/
        voidUpdateOutputMap();

        /*Print Current output map status*/
        printf("Step Number : %d\n"
               "==============================\n",visited_cells);
        voidPrintMap(Outputmap);

        /*position to next cell based on searching algorithm */
        voidTakeAction();

    }/*end of Searching loop*/

    /*=============================================*/
    /*Print final result of search*/
    /*=============================================*/

    /*print number of steps taken to search map*/
    /*=============================================*/
    printf("Map Searched in %d Steps\n"
           "==============================\n",visited_cells);

    /*Print original input map*/
    /*=============================================*/
    printf("Original Input Map\n"
           "==============================\n");
    voidPrintMap(Inputmap);

    /*Print final output map*/
    /*=============================================*/
    printf("Final Output Map\n"
           "==============================\n");
    voidPrintMap(Outputmap);

}/*end of voidSearchMap();*/

/*this function responsible for changing output map cell's status based on discovered input map cells
 *it also saves last available cell coordinates in case the algorithm got stuck in a dead end rout
 *it could reposition itself to a cell that has available routs
 *ie cells that is surrounded by more than one NOT_MINE Cells
 *
 *Arguments : void
 *Return    : void */
void voidUpdateOutputMap(void)
{
    /*available cell variable is used to store number of cells that is not a mine around current cell*/
    u8 AvailableCells =0;

    /*=====================================================================================*/
    /*update current cell status to CURRENT_POSITION*/
    CurrentOutputCell -> Status = CURRENT_LOCATION;

    /*=====================================================================================*/
    /*Update Surrounding Cells status*/
    /*Leave VISTED status unchanged in output map*/
    if( ( (CurrentOutputCell->RCell)  -> Status) != VISITED)
    {
        /*Copy Right cell status in input map to Right cell status in output map */
        (CurrentOutputCell->RCell)  -> Status = (CurrentInputCell->RCell)  -> Status;
    }
    if( ( (CurrentOutputCell->LCell)  -> Status) != VISITED)
    {
        /*Copy Left cell status in input map to Left cell status in output map */
        (CurrentOutputCell->LCell)  -> Status = (CurrentInputCell->LCell)  -> Status;
    }
    if( ( (CurrentOutputCell->UpCell)  -> Status) != VISITED)
    {
        /*Copy Upper cell status in input map to Upper cell status in output map */
        (CurrentOutputCell->UpCell)  -> Status = (CurrentInputCell->UpCell)  -> Status;
    }
    if( ( (CurrentOutputCell->LowCell)  -> Status) != VISITED)
    {
        /*Copy Lower cell status in input map to Lower cell status in output map */
        (CurrentOutputCell->LowCell)  -> Status = (CurrentInputCell->LowCell)  -> Status;
    }
    /*=====================================================================================*/
    /*Change every NOT_MINE status in Output map to DISCOVERED_NOT_MINE and increment number
     *of available routs from this Cell */
    if( ( (CurrentOutputCell->RCell)  -> Status) == NOT_MINE)
    {
        (CurrentOutputCell->RCell)  -> Status  = DISCOVERED_NOT_MINE;
        AvailableCells++;
    }
    if( ( (CurrentOutputCell->LCell)  -> Status) == NOT_MINE)
    {
        (CurrentOutputCell->LCell)  -> Status = DISCOVERED_NOT_MINE;
        AvailableCells++;
    }
    if( ( (CurrentOutputCell->UpCell)  -> Status) == NOT_MINE)
    {
        (CurrentOutputCell->UpCell)  -> Status  = DISCOVERED_NOT_MINE;
        AvailableCells++;
    }
    if( ( (CurrentOutputCell->LowCell)  -> Status) == NOT_MINE)
    {
        (CurrentOutputCell->LowCell)  -> Status  = DISCOVERED_NOT_MINE;
        AvailableCells++;
    }
    /*=====================================================================================*/
    /* Save Current Coordinates of both maps if this input cell has more than one cell that is
     * not a mine surrounding it*/
    if( AvailableCells > 1 )
    {
        /*Save Input map current cell position in available cells queue*/
        LastAvailableInputCell[NumberOfAvailableCells]  = CurrentInputCell;
        /*Save Output map current cell position in available cells queue*/
        LastAvailableOutputCell[NumberOfAvailableCells] = CurrentOutputCell;
        /*Increment number of available cells*/
        NumberOfAvailableCells++;
    }

}/*end of voidUpdateOutputMap()*/

/*this function contain the algorithm used for decision making based on surrounding cell status
 *it decide which cell should be visited next
 *Arguments : void
 *Return    : void */
void voidTakeAction(void)
{
    /*Algorithm sequence is
    * Go to right cell if available if not
    * Go to upper cell if available if not
    * Go to Lower cell if available if not
    * Go to Left cell if available if not
    * if all surrounding cells are not available (ie mine or border or already visited)
    * Go to last available cell (last cell that had more than one cell that were not a mine)
    * that is done by voidBackPropagate(); function*/

    /*======================================================================================*/
    /*======================================================================================*/
    /*Check surrounding cells availability*/
    /*Check if Right cell is available (DISCOVERED_NOT_MINE)*/
    if (DISCOVERED_NOT_MINE == ( (CurrentOutputCell->RCell)  -> Status) )
    {
        /*Goto Right cell*/
        voidGotoRightCell();
    }
    /*Check if Upper cell is available (DISCOVERED_NOT_MINE)*/
    else if (DISCOVERED_NOT_MINE == ( (CurrentOutputCell->UpCell)  -> Status) )
    {
        /*Goto Upper cell*/
        voidGotoUppertCell();
    }
    /*Check if Lower cell is available (DISCOVERED_NOT_MINE)*/
    else if (DISCOVERED_NOT_MINE == ( (CurrentOutputCell->LowCell)  -> Status) )
    {
        /*Goto Lower cell*/
        voidGotoLowerCell();
    }
    /*Check if Left cell is available (DISCOVERED_NOT_MINE)*/
    else if (DISCOVERED_NOT_MINE == ( (CurrentOutputCell->LCell)  -> Status) )
    {
        /*Goto Left cell*/
        voidGotoLeftCell();
    }
    /*in that case you are surrounded by mines and discovered cells */
    else
    {
        /*Terminate condition variable*/
        u8 TerminateCondition = TRUE;
        /*loop entire output map cells from (1,1) to (MAP_SIZE,MAP_SIZE)*/
        /*===================================================================*/
        for(u8 i = 1;i < (MAP_SIZE+1) ; i++)
        {
            for(u8 j = 1;j < (MAP_SIZE+1) ; j++)
            {
                if( DISCOVERED_NOT_MINE == Outputmap[i][j].Status)
                {
                    /* if you fond any available cell in output map make Terminate Condition false
                     * then beak j loop*/
                    TerminateCondition = FALSE;
                    break;
                }
            }/*end of j for loop*/

            /*if you found any available cell in output map beak i loop*/
            if( FALSE == TerminateCondition) {break;}

        }/*end of i for loop*/

        /*Termination condition check*/
        if ( (TRUE == TerminateCondition) || (0 == NumberOfAvailableCells ) )
        {
            /*if there are no available moves or there are no cells that has not
             *a mine in it (all map is either visited or a mine)
             *then terminate Search and Mark Current cell VISITED*/
            CurrentOutputCell-> Status = VISITED;
            /*make dead end condition true*/
            DeadendCondition = TRUE;

        }
        else
        {
            /*Notify Back propagation event*/
            printf("Back propagation happened\n");
            /*go to last available cell*/
            voidBackPropagate();
        }/*end of termination condition check*/

    }/*end of surrounding cells availability check */

    /*======================================================================================*/
    /*======================================================================================*/
}/*end of voidTakeAction();*/

/*this function reposition current cell position to last available cell so that search algorithm can take new rout
 *Arguments : void
 *Return    : void */
void voidBackPropagate(void)
{
    /*Mark current cell visited then reposition to last available cell*/
    /*========================================================*/
    /*Mark Current cell VISITED*/
    CurrentOutputCell-> Status = VISITED;
    /*reposition current position for both maps to last available position*/
    /*======================================================================================*/
    CurrentInputCell  = LastAvailableInputCell[NumberOfAvailableCells-1];
    CurrentOutputCell = LastAvailableOutputCell[NumberOfAvailableCells-1];
    /*Decrement number of available cells by one*/
    NumberOfAvailableCells--;
    /*======================================================================================*/

    /*increment number of visited cells*/
    /*========================================================*/
    visited_cells++;

}/*end of voidBackPropagate();*/


/*this function reposition current cell position to Right Cell
 *Arguments : void
 *Return    : void */
void voidGotoRightCell(void)
{

    /*Mark current cell visited then reposition to Right cell*/
    /*========================================================*/
    /*Mark Current cell VISITED*/
    CurrentOutputCell-> Status = VISITED;
    /*reposition current cell position to Right Cell*/
    /*========================================================*/
    /*Reposition Current Cell pointer in both maps to Right Cell*/
    CurrentInputCell  = CurrentInputCell->RCell;
    CurrentOutputCell = CurrentOutputCell->RCell;
    /*increment number of visited cells*/
    /*========================================================*/
    visited_cells++;

}

/*this function reposition current cell position to Left Cell
 *Arguments : void
 *Return    : void */
void voidGotoLeftCell(void)
{
    /*Mark current cell visited then reposition to Left cell*/
    /*========================================================*/
    /*Mark Current cell VISITED*/
    CurrentOutputCell-> Status = VISITED;
    /*reposition current cell position to Left Cell*/
    /*========================================================*/
    /*Reposition Current Cell pointer in both maps to Left Cell*/
    CurrentInputCell  = CurrentInputCell->LCell;
    CurrentOutputCell = CurrentOutputCell->LCell;
    /*increment number of visited cells*/
    /*========================================================*/
    visited_cells++;
}

/*this function reposition current cell position to Upper Cell
 *Arguments : void
 *Return    : void */
void voidGotoUppertCell(void)
{
    /*Mark current cell visited then reposition to Upper cell*/
    /*========================================================*/
    /*Mark Current cell VISITED*/
    CurrentOutputCell-> Status = VISITED;
    /*reposition current cell position to Upper Cell*/
    /*========================================================*/
    /*Reposition Current Cell pointer in both maps to Upper Cell*/
    CurrentInputCell  = CurrentInputCell->UpCell;
    CurrentOutputCell = CurrentOutputCell->UpCell;
    /*increment number of visited cells*/
    /*========================================================*/
    visited_cells++;
}

/*this function reposition current cell position to Lower Cell
 *Arguments : void
 *Return    : void */
void voidGotoLowerCell(void)
{
    /*Mark current cell visited then reposition to Lower cell*/
    /*========================================================*/
    /*Mark Current cell VISITED*/
    CurrentOutputCell-> Status = VISITED;
    /*reposition current cell position to Lower Cell*/
    /*========================================================*/
    /*Reposition Current Cell pointer in both maps to Lower Cell*/
    CurrentInputCell  = CurrentInputCell->LowCell;
    CurrentOutputCell = CurrentOutputCell->LowCell;
    /*increment number of visited cells*/
    /*========================================================*/
    visited_cells++;
}