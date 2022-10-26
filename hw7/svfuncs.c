#include "svfuncs.h"

/* sv_funcs.c - TCPechod, TCPchargend, TCPdaytimed, TCPtimed */

#include <sys/types.h>
#include <sys/errno.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h> // htonl(),
#include <unistd.h> // getpid()
#include <time.h> // time()

#define	BUFSIZ	4096		/* max read buffer size	*/

extern	int	errno;

int errexit(const char *format, ...);

void printReceive(char* transport, char* service) {
	pid_t pid = getpid();
	printf("[%s_%s] Child %d recieved the %s request", transport, service, (int)pid, service);
}

void printFinish(char* transport, char* service) {
	pid_t pid = getpid();
	printf("[%s_%s] Child %d finished the %s service", transport, service, (int)pid, service);
}
/*------------------------------------------------------------------------
 * TCPecho - do TCP ECHO on the given socket
 *------------------------------------------------------------------------
 */
int
TCPechod(int fd)
{
	char	buf[BUFSIZ];
	int	cc;

	while (cc = read(fd, buf, sizeof buf)) {
		if (cc < 0)
			errexit("echo read: %s\n", strerror(errno));
		if (write(fd, buf, cc) < 0);
			errexit("echo write: %s\n", strerror(errno));
	}
	return 0;
}

#define	LINELEN		72

/*------------------------------------------------------------------------
 * TCPchargend - do TCP CHARGEN on the given socket
 *------------------------------------------------------------------------
 */
int
TCPchargend(int fd)
{
	char	c, buf[LINELEN+2];	/* print LINELEN chars + \r\n */

	c = ' ';
	buf[LINELEN] = '\r';
	buf[LINELEN+1] = '\n';
	while (1) {
		int	i;

		for (i=0; i<LINELEN; ++i) {
			buf[i] = c++;
			if (c > '~')
				c = ' ';
		}
		if (write(fd, buf, LINELEN+2) < 0)
			break;
	}
	return 0;
}

/*------------------------------------------------------------------------
 * TCPdaytimed - do TCP DAYTIME protocol
 *------------------------------------------------------------------------
 */
int
TCPdaytimed(int fd)
{
	char	buf[LINELEN], *ctime();
	time_t	time(), now;

	(void) time(&now);
	sprintf(buf, "%s", ctime(&now));
	(void) write(fd, buf, strlen(buf));
	return 0;
}

#define	UNIXEPOCH	2208988800	/* UNIX epoch, in UCT secs	*/

/*------------------------------------------------------------------------
 * TCPtimed - do TCP TIME protocol
 *------------------------------------------------------------------------
 */
int
TCPtimed(int fd)
{
	time_t	now;
	(void) time(&now);
	now = htonl((u_long)(now + UNIXEPOCH));
	(void) write(fd, (char *)&now, sizeof(now));
	return 0;
}

/*------------------------------------------------------------------------
 * UDP - do UDP ECHO on the given socket
 *------------------------------------------------------------------------
 */

int UDPechod(int fd) {
	return 0;
}

/*------------------------------------------------------------------------
 * UDPchargend - do UDP CHARGEN on the given socket
 *------------------------------------------------------------------------
 */
int UDPchargend(int fd) {
	return 0;
}

/*------------------------------------------------------------------------
 * UDPdaytimed - do UDP DAYTIME protocol
 *------------------------------------------------------------------------
 */
int UDPdaytimed(int fd) {
	return 0;
}
/*------------------------------------------------------------------------
 * UDPtimed - do UDP TIME protocol
 *------------------------------------------------------------------------
 */
int UDPtimed(int fd) {
	char* transport = "udp";
	char* service = "time";

	char buf[1];
	struct sockaddr_in fsin;
	socklen_t alen;
	if (recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&fsin, &alen) < 0) {
         errexit("recvfrom: %s\n", strerror(errno));
      }
	time_t now;
	(void) time(&now);
	now = htonl((u_long)(now + UNIXEPOCH));
	(void) sendto(fd, (char *)&now, sizeof(now), 0, (struct sockaddr*)&fsin, sizeof(fsin));
	return 0;
}
