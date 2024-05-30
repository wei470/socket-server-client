/* rls.c - a client for a remote directory listing service
 *         usage: rls hostname directory
 */
#include       <stdio.h>
#include       <sys/types.h>
#include       <sys/socket.h>
#include       <netinet/in.h>
#include       <netdb.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<string.h>

#define        oops(msg)       { perror(msg); exit(1); }
#define		PORTNUM		15000

int main(int ac, char *av[])
{
	struct sockaddr_in  servadd;        /* the number to call */

	int    sock_id, sock_fd;            /* the socket and fd  */
	char   buffer[BUFSIZ];              /* to receive message */
	int    n_read;                      /* for message length */

	sock_id = socket( AF_INET, SOCK_STREAM, 0 );    /* get a line   */

    servadd.sin_addr.s_addr = inet_addr("127.0.0.1");
	servadd.sin_port = htons(PORTNUM);      /* fill in port number  */
	servadd.sin_family = AF_INET ;          /* fill in socket type  */

    connect(sock_id,(struct sockaddr *)&servadd, sizeof(servadd));
	write(sock_id, "~\n", 9);


	while( (n_read = read(sock_id, buffer, BUFSIZ)) > 0 )
		if ( write(1, buffer, n_read) == -1 )
			oops("write");
	close( sock_id );    
}
