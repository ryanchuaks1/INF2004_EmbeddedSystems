#include "common.h"

// SSI tags - tag length limited to 8 bytes by default
const char *ssi_tags[] = {"barcode"};

u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen)
{
  size_t printed;
  switch (iIndex)
  {
  case 0: // barcode
  {
    printed = snprintf(pcInsert, iInsertLen, "%s", "1234567890");
  }
  break;
  default:
    printed = 0;
    break;
  }

  return (u16_t)printed;
}

// Initialise the SSI handler
void ssi_init()
{
  http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
}
