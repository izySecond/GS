// Automatic generated file.
// This files stores STM fonts. The large font tables are stored separately
// in flash, but not in the RO segment of the programm.
//
// filename : fontsFLASH.c 
//

#include "fonts.h"
#include <stdbool.h>
#include "fontsFLASH.h"

#define BASE_ADR ((const uint8_t *) 0x8100000)

bool checkVersionFlashFonts(void){
   long version = * ((long *) BASE_ADR);
   return version == (long) 1595446945;
}

sFONT Font24FLASH = {
    BASE_ADR + 0x8 , // adr. of font table in FLASH
    17 , // Width 
    24 , // Height 
};

sFONT Font20FLASH = {
    BASE_ADR + 0x1AC0 , // adr. of font table in FLASH
    14 , // Width 
    20 , // Height 
};

sFONT Font16FLASH = {
    BASE_ADR + 0x2998 , // adr. of font table in FLASH
    11 , // Width 
    16 , // Height 
};

sFONT Font12FLASH = {
    BASE_ADR + 0x3578 , // adr. of font table in FLASH
    7 , // Width 
    12 , // Height 
};

sFONT Font8FLASH = {
    BASE_ADR + 0x39EC , // adr. of font table in FLASH
    5 , // Width 
    8 , // Height 
};

sFONT CourierNew48FLASH = {
    BASE_ADR + 0x3CE4 , // adr. of font table in FLASH
    29 , // Width 
    47 , // Height 
};

sFONT CourierNewBold40FLASH = {
    BASE_ADR + 0x82A8 , // adr. of font table in FLASH
    30 , // Width 
    42 , // Height 
};

sFONT CourierNewBold60FLASH = {
    BASE_ADR + 0xC100 , // adr. of font table in FLASH
    45 , // Width 
    62 , // Height 
};

sFONT Consolas40FLASH = {
    BASE_ADR + 0x14B0C , // adr. of font table in FLASH
    23 , // Width 
    41 , // Height 
};

sFONT Consolas60FLASH = {
    BASE_ADR + 0x178B1 , // adr. of font table in FLASH
    34 , // Width 
    61 , // Height 
};

sFONT Consolas80FLASH = {
    BASE_ADR + 0x1E9E0 , // adr. of font table in FLASH
    45 , // Width 
    81 , // Height 
};

sFONT Consolas90FLASH = {
    BASE_ADR + 0x29E3A , // adr. of font table in FLASH
    50 , // Width 
    91 , // Height 
};

sFONT ConsolasBold40FLASH = {
    BASE_ADR + 0x38A9D , // adr. of font table in FLASH
    23 , // Width 
    41 , // Height 
};

sFONT ConsolasBold60FLASH = {
    BASE_ADR + 0x3B842 , // adr. of font table in FLASH
    34 , // Width 
    61 , // Height 
};

sFONT ConsolasBold80FLASH = {
    BASE_ADR + 0x42971 , // adr. of font table in FLASH
    45 , // Width 
    81 , // Height 
};

sFONT ConsolasBold90FLASH = {
    BASE_ADR + 0x4DDCB , // adr. of font table in FLASH
    51 , // Width 
    91 , // Height 
};

sFONT FiraMono40FLASH = {
    BASE_ADR + 0x5CA2E , // adr. of font table in FLASH
    25 , // Width 
    43 , // Height 
};

sFONT FiraMono60FLASH = {
    BASE_ADR + 0x60A02 , // adr. of font table in FLASH
    37 , // Width 
    64 , // Height 
};

sFONT FiraMono80FLASH = {
    BASE_ADR + 0x680C2 , // adr. of font table in FLASH
    49 , // Width 
    85 , // Height 
};

sFONT FiraMono90FLASH = {
    BASE_ADR + 0x75D8F , // adr. of font table in FLASH
    55 , // Width 
    97 , // Height 
};

sFONT FiraMonoMedium40FLASH = {
    BASE_ADR + 0x85988 , // adr. of font table in FLASH
    25 , // Width 
    44 , // Height 
};

sFONT FiraMonoMedium60FLASH = {
    BASE_ADR + 0x89AD8 , // adr. of font table in FLASH
    37 , // Width 
    65 , // Height 
};

sFONT FiraMonoMedium80FLASH = {
    BASE_ADR + 0x91373 , // adr. of font table in FLASH
    49 , // Width 
    87 , // Height 
};

sFONT FiraMonoMedium90FLASH = {
    BASE_ADR + 0x9F572 , // adr. of font table in FLASH
    55 , // Width 
    97 , // Height 
};

// EOF
