/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Jared Kirschner 2015
------------------------------------------------------------------------------*/

// Note: this is a workaround because even though this symbol is defined in _syscalls.c
// and _syscalls.c is part of the makefile,
// when using <system_error> we get a linker error for an undefined hidden symbol
void *__dso_handle = 0;
