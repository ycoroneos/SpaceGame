#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED
//***********************************************


#define debug 1


//***********************************************

#if debug==1
# define DEBUG_PRINT(x) printf x
# define PUTS(x) puts x
#else
# define DEBUG_PRINT(x) do {} while (0)
# define PUTS(x) do {} while (0)
#endif

#endif // MACROS_H_INCLUDED
