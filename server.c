/* rlsd.c - a remote ls server - with paranoia
 */

#include  <stdio.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <netdb.h>
#include  <time.h>
#include  <strings.h>
#include  <stdlib.h>
#include  <ctype.h>

#define   PORTNUM  15000   /* our remote ls server port */
#define   HOSTLEN  256
#define   oops(msg)      { perror(msg) ; exit(1) ; }

void sanitize(char *str);

int main(int ac, char *av[])
{
	struct  sockaddr_in   saddr;   /* build our address here */

	int	sock_id,sock_fd,n_read;       /* line id, file desc     */
	FILE	*sock_fpi, *sock_fpo;  /* streams for in and out */
	FILE	*pipe_fp;	       /* use popen to run ls    */
	char    dirname[BUFSIZ];       /* from client            */
	char    command[BUFSIZ];       /* for popen()            */
	int	dirlen, c;

      /** Step 1: ask kernel for a socket **/

	sock_id = socket( PF_INET, SOCK_STREAM, 0 );    /* get a socket */

	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(PORTNUM);        /* fill in socket port  */
	saddr.sin_family = AF_INET ;            /* fill in addr family  */
	bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr));

      /** Step 3: allow incoming calls with Qsize=1 on socket **/

	listen(sock_id, 1);


	       sock_fd = accept(sock_id, NULL, NULL); 

	       sock_fpi = fdopen(sock_fd,"r");

	       fgets(dirname, BUFSIZ-5, sock_fpi); //why BUFSIZ -5
		   //dirname1 = path1;
	       sanitize(dirname); //why clean

	       /* open writing direction as buffered stream */
	
		   int pipefd[2];
		   pipe(pipefd);
		   pid_t pid = fork();

		   if(pid == 0){
			close(pipefd[0]);
            dup2(pipefd[1],STDOUT_FILENO);
            close(pipefd[1]);
			char *argv[] = {dirname,NULL};
            execvp("ls", argv);
		   }
		   else{
			sleep(1);
			char buffer[10];
			close(pipefd[1]);

 while ((n_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) 
       { 
        write(sock_fd, buffer, n_read);
	   }
	   printf("done\n");
		   }
		   
       
}

void sanitize(char *str)
/*
 * it would be very bad if someone passed us an dirname like
 * "; rm *"  and we naively created a command  "ls ; rm *"
 *
 * so..we remove everything but slashes and alphanumerics
 * There are nicer solutions, see exercises
 */
{
	char *src, *dest;

	for ( src = dest = str ; *src ; src++ )
		if ( *src == '/' || isalnum(*src) )
			*dest++ = *src;
	*dest = '\0';
}
