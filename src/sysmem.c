#include "sysmem.h"

// This actually allocates the physical bytes in the ATtiny85's SRAM!
union SharedSystemRam sys_ram;
