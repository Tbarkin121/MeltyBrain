/*
 * The '__stack' definition is required by crt0, do not remove it.
 */
__stack = ORIGIN(RAM) + LENGTH(RAM);

_estack = __stack;  /* STM specific definition */

/*
 * Default stack sizes.
 * These are used by the startup in order to allocate stacks
 * for the different modes.
 */

__Main_Stack_Size = 16384 ;
__Main_Stack_Guard_Size = 0x40 ;

PROVIDE ( _Main_Stack_Size = __Main_Stack_Size ) ;

__Main_Stack_Limit = __stack  - __Main_Stack_Size ;


/* "PROVIDE" allows to easily override these values from an
 * object file or the command line. */
PROVIDE ( _Main_Stack_Limit = __Main_Stack_Limit ) ;

/*
 * There will be a link error if there is not this amount of
 * RAM free at the end.
 */
_Minimum_Stack_Size = 256 ;
