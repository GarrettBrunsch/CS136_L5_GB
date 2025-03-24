**3/24/2025**
* #646, 694 should have been a separate function that returns a string with search results;  should have added matching records to one string and returned it; this way, it can be printed to a file or screen; e.g.,  result+=car[i].toString() -1
* 590 - should be two separate functions so printing to a file can be done independently from printing to the screen -1
* unnecessary variable – unnecessary computational and “human” expense, while(menuChoice!=QUIT) -1
```
hasErrors = true;
```
* 671 should have been global const or static member const so it can be changed easily -1
* 551 what do search criteria have to do with sort? -1
* 569 what if the error file is missing? -1
* 584 - should use peak function to check if an error file is empty -1
* 543  wasteful -1
```
//e.g.
enum SortMenu { CAR_ID = 1, MODEL, QUANTITY, PRICE,  BACK_TO_MAIN};
const string strSortMenu[] ={"N/A", "CAR ID", "MODEL", "QUANTITY", "PRICE",  "BACK_TO_MAIN"};
cout<<"Sorted by " << strSortMenu[PRICE]<<endl;
```
* 486 – wasteful -1
```
int *pList[MAX]={nullptr}; //initializes all pointers to nullptr; no loop needed
```
* sort and search as slow as it is, no need to make them even slower;  to expedite search and sort and for sort to work properly; all items/records should have been converted to all upper/lower case before storing in the array; -2
* 65-66 should not be members of the class; should be defined locally-1
* 102-104 should not be members of the class; should be defined globally-1
* #447 to avoid redundancy and the possible mistake of not pointing the pointer when new car added should be done in the constructors of the inventory class-1;
* #526 - has to be evaluated on every iteration of that loop; should create 2 separate functions one to print through the array and another to print through the array of pointers -1
* 247-250  should be at the top main.cpp
* file name(s) and/or file/folder structure/capitalization do not comply with posted instructions; folder- CSXXX_LN_FL (all caps, X – class catalog number, N- lab number, FL- your initials;  files:  all lower case, main() – main.cpp, input - “in_\*.txt”, output –“out_\*.txt”, error –“err_\*.txt”; do not include any other files or folders;  -2
* * #442 waste of time; at no point, an object should become invalid; in this context should validate a record before setting a car object -2
* should initialize all variables, members or not, to appropriate default values at the time of definition; improves readability, in case of member variables, helps to avoid initialization lists, helps with the exception handling and providing a strong exception guarantee; strings should be initialized to something other than “” n/a for example -2

* #484 mixed usage of the initialization list and setting up member variables in the body of a function makes future updates more complicated and reduces readability; either set all member variables in the initialization list or in the value of the function -1
* 441 – waste of time; constructor and destructor run to create and dismantle an object = computational expense; constructors and destructors are functions
```
car[i].setCar(…) //one function to run
```


