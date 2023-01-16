#include "interface.h"


int main( void )
{
  initialize();

  while (1)
  {
    light(button());
  }
  return 0;
}
