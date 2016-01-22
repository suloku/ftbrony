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
 *
 *  @returns loop status
 */
static loop_status_t
loop(loop_status_t (*callback)(void))
{
  loop_status_t status = LOOP_CONTINUE;

#ifdef _3DS
  while(aptMainLoop())
  {
    status = callback();
    console_render();
    if(status != LOOP_CONTINUE)
      return status;
  }

  return LOOP_EXIT;
#else
  while(status == LOOP_CONTINUE)
    status = callback();
  return status;
#endif
}

/*! wait until the B button is pressed
 *
 *  @returns loop status
 */
static loop_status_t
wait_for_b(void)
{
#ifdef _3DS
  /* update button state */
  hidScanInput();

  /* check if B was pressed */
  if(hidKeysDown() & KEY_B)
    return LOOP_EXIT;

  /* B was not pressed */
  return LOOP_CONTINUE;
#else
  return LOOP_EXIT;
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
  loop_status_t status = LOOP_RESTART;

#ifdef _3DS
  /* initialize needed 3DS services */
  acInit();
  gfxInitDefault();
  gfxSet3D(false);
  sdmcWriteSafe(false);
#endif

 //Try to get baground from sd card (won't work as cia, would need absolute path)
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

  while(status == LOOP_RESTART)
  {
    /* initialize ftp subsystem */
    if(ftp_init() == 0)
    {
      /* ftp loop */
      status = loop(ftp_loop);

      /* done with ftp */
      ftp_exit();
    }
    else
      status = LOOP_EXIT;
  }

  console_print("Press B to exit\n");
  loop(wait_for_b);
  if (background != NULL) free (background);
#ifdef _3DS
  /* deinitialize 3DS services */
  gfxExit();
  acExit();
#endif

  return 0;
}
