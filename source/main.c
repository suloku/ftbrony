#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#ifdef _3DS
#include <3ds.h>
#endif
#include "console.h"
#include "ftp.h"

u8 * background;

/*! looping mechanism
 *
 *  @param[in] callback function to call during each iteration
 */
static void
loop(int (*callback)(void))
{
#ifdef _3DS
  while(aptMainLoop())
  {
    if(callback() == 0)
      console_render();
    else
      return;
  }
#else
  for(;;)
    callback();
#endif
}

/*! wait until the B button is pressed
 *
 *  @returns -1 if B was pressed
 */
static int
wait_for_b(void)
{
#ifdef _3DS
  /* update button state */
  hidScanInput();

  /* check if B was pressed */
  if(hidKeysDown() & KEY_B)
    return -1;

  /* B was not pressed */
  return 0;
#else
  return -1;
#endif
}

/*! entry point
 *
 *  @param[in] argc unused
 *  @param[in] argv unused
 *
 *  returns exit status
 */
int
main(int  argc,
     char *argv[])
{
#ifdef _3DS
  /* initialize needed 3DS services */
  gfxInitDefault();
  gfxSet3D(false);
#endif

  //Try to get baground from sd card
  FILE * pFile;
  long lSize;
  size_t result;
  pFile = fopen ( "background.bin" , "rb" );
  if (pFile!=NULL){
	  fseek (pFile , 0 , SEEK_END);
	  lSize = ftell (pFile);
	  rewind (pFile);
	  background = (char*) malloc (sizeof(char)*lSize);
	  if (background != NULL) {
		  result = fread (background,1,lSize,pFile);
		  if (result != lSize) {
			if (background != NULL){
				free(background);
			}
		  }
	  }
  }
  fclose (pFile);

  /* initialize console subsystem */
  console_init();
  console_set_status("\n" GREEN STATUS_STRING RESET);

  /* initialize ftp subsystem */
  if(ftp_init() == 0)
  {
    /* ftp loop */
    loop(ftp_loop);

    /* done with ftp */
    ftp_exit();
  }

  console_print("Press B to exit\n");
  loop(wait_for_b);

#ifdef _3DS
  /* deinitialize 3DS services */
  gfxExit();
#endif
  free (background);

  return 0;
}
