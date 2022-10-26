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
#define	LINELEN		72
#define	UNIXEPOCH	2208988800	/* UNIX epoch, in UCT secs	*/

extern	int	errno;

int errexit(const char *format, ...);

void printReceive(char* transport, char* service) {
	pid_t pid = getpid();
	printf("[%s_%s] Child %d recieved the %s request\n", transport, service, (int)pid, service);
}

void printFinish(char* transport, char* service) {
	pid_t pid = getpid();
	printf("[%s_%s] Child %d finished the %s service\n", transport, service, (int)pid, service);
}
/*------------------------------------------------------------------------
 * TCPecho - do TCP ECHO on the given socket
 *------------------------------------------------------------------------
 */
int
TCPechod(int fd)
{
	char* transport = "tcp";
	char* service = "echo";
	char	buf[BUFSIZ];
	int	cc;
	printReceive(transport, service);
	while (cc = read(fd, buf, sizeof buf)) {
		if (cc < 0)
			errexit("echo read: %s\n", strerror(errno));
		if (write(fd, buf, cc) < 0);
			errexit("echo write: %s\n", strerror(errno));
	}
	printFinish(transport, service);
	return 0;
}


/*------------------------------------------------------------------------
 * TCPchargend - do TCP CHARGEN on the given socket
 *------------------------------------------------------------------------
 */
int
TCPchargend(int fd)
{
	char* transport = "tcp";
	char* service = "chargen";
	char c, buf[LINELEN+2];	/* print LINELEN chars + \r\n */

	c = ' ';
	buf[LINELEN] = '\r';
	buf[LINELEN+1] = '\n';
	printReceive(transport, service);
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
	printFinish(transport, service);
	return 0;
}

/*------------------------------------------------------------------------
 * TCPdaytimed - do TCP DAYTIME protocol
 *------------------------------------------------------------------------
 */
int
TCPdaytimed(int fd)
{
	char* transport = "tcp";
	char* service = "daytime";
	printReceive(transport, service);

	char	buf[LINELEN], *ctime();
	time_t	time(), now;

	(void) time(&now);
	sprintf(buf, "%s", ctime(&now));
	(void) write(fd, buf, strlen(buf));
	printFinish(transport, service);
	return 0;
}


/*------------------------------------------------------------------------
 * TCPtimed - do TCP TIME protocol
 *------------------------------------------------------------------------
 */
int
TCPtimed(int fd)
{
	char* transport = "tcp";
	char* service = "time";

	printReceive(transport, service);
	time_t	now;
	(void) time(&now);
	now = htonl((u_long)(now + UNIXEPOCH));
	(void) write(fd, (char *)&now, sizeof(now));
	printFinish(transport, service);
	return 0;
}

/*------------------------------------------------------------------------
 * UDP - do UDP ECHO on the given socket
 *------------------------------------------------------------------------
 */

int UDPechod(int fd) {
	char* transport = "udp";
	char* service = "echo";

	char buf[BUFSIZ];

	struct sockaddr_in fsin;
	socklen_t alen;
	int n;
	printReceive(transport, service);
	if ((n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*)&fsin, &alen)) < 0) {
         errexit("recvfrom: %s\n", strerror(errno));
	}
	buf[n] = '\0';
	(void) sendto(fd, (char *)&buf, sizeof(buf), 0, (struct sockaddr*)&fsin, alen);
	printFinish(transport, service);
	return 0;
}

/*------------------------------------------------------------------------
 * UDPchargend - do UDP CHARGEN on the given socket
 *------------------------------------------------------------------------
 */
int UDPchargend(int fd) {
	char* transport = "udp";
	char* service = "chargen";
	char c, buf[LINELEN+2];	/* print LINELEN chars + \r\n */

	c = ' ';
	buf[LINELEN] = '\r';
	buf[LINELEN+1] = '\n';
	int	i;

	for (i=0; i<LINELEN; ++i) {
		buf[i] = c++;
		if (c > '~')
			c = ' ';
	}

	char request[1];
	struct sockaddr_in fsin;
	socklen_t alen;
	printReceive(transport, service);
	if (recvfrom(fd, request, sizeof(request), 0, (struct sockaddr*)&fsin, &alen) < 0) {
         errexit("recvfrom: %s\n", strerror(errno));
	}
	(void) sendto(fd, (char *)&buf, sizeof(buf), 0, (struct sockaddr*)&fsin, alen);
	printFinish(transport, service);
	return 0;
}

/*------------------------------------------------------------------------
 * UDPdaytimed - do UDP DAYTIME protocol
 *------------------------------------------------------------------------
 */
int UDPdaytimed(int fd) {
	char* transport = "udp";
	char* service = "daytime";

	char buf[LINELEN], *ctime();
	time_t	time(), now;
	char request[1];

	struct sockaddr_in fsin;
	socklen_t alen;
	if (recvfrom(fd, request, sizeof(request), 0, (struct sockaddr*)&fsin, &alen) < 0) {
         errexit("recvfrom: %s\n", strerror(errno));
	}
	printReceive(transport, service);

	(void) time(&now);
	sprintf(buf, "%s", ctime(&now));
	(void) sendto(fd, (char *)&buf, sizeof(buf), 0, (struct sockaddr*)&fsin, sizeof(fsin));
	printFinish(transport, service);
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
	printReceive(transport, service);
	time_t now;
	(void) time(&now);
	now = htonl((u_long)(now + UNIXEPOCH));
	(void) sendto(fd, (char *)&now, sizeof(now), 0, (struct sockaddr*)&fsin, sizeof(fsin));
	printFinish(transport, service);
	return 0;
}
