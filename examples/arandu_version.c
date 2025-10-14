#include <arandu/arandu.h>
#include <stdio.h>

int
main (void)
{
  printf ("Testando a biblioteca Arandu.\n");
  printf ("Versão da biblioteca: %s\n", arandu_get_version ());

  printf ("Exemplo executado com sucesso!\n");

  return 0;
}
