/*
FILNAMN: 		send.cc
LABORATION:		
PROGRAMMERARE:	jened502, niker917
DATUM:			2013-11-14
BESKRIVNING:	
*/

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */



/*
 * 'open_port()' - Open serial port 1.
 * Returns the file descriptor on success or -1 on error.
 */

int open_port()
{
  int fd; /* File descriptor for the port */


  fd = open("/dev/tty.FireFly-AA63-SPP", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
   /*
    * Could not open the port.
    */
    perror("open_port: Unable to open /dev/ttyf1 - ");
  }
  else
    fcntl(fd, F_SETFL, 0);

  return fd;
}

void write(int fd)
{
	int n = write(fd,"ATZ\r", 4);
	if (n < 0)
	{
		fputs("write() of 4 bytes failed!\n", stderr);
	}
	return;
}

int main()
{
	int portnr;
	portnr = open_port();
	write(portnr);
    return 0;
}
