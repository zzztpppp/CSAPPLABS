#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen, serverlen;
    char clientname[MAXLINE], clientport[MAXLINE], servername[MAXLINE], serverport[MAXLINE];
    struct sockaddr_storage client_addr, server_addr;
    char *request_for, *response_for;

    /* Check command line arguements */
    if (argv != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    listenfd = Open_listenfd(argv[1]);

    /* Start sequentail proxy server main routine */
    while (1) {
        clientlen = sizeof(client_addr);
        connfd = Accept(listenfd, (SA *)&client_addr, &clientlen);
        Getnameinfo((SA *) &client_addr, clientlen, clientname, MAXLINE, 
                    clientport, MAXLINE, 0);
        printf("Conected from (%s %s)\n", clientname, clientport);

        /* Read and process request from the client*/
        if (process_request(connfd, &request_for, &servername, &serverport) == -1) {
            /* Ignore malformed request */
            Close(connfd);
            continue;
        }

        /* If the request is valid, forward it to the server */
        connfd = Open_clientfd(servername, serverport);
        if (process_response(connfd, &response_for) == -1){
            /* Ignore malformed response */
            Close(connfd);
            continue;
        }
        Rio_writen(connfd, &response_for, strlen(response_for));
    }
}


/*
 * process_request - Process the comming request 
 * from the socket connection connfd.
 */
static int process_request(int connfd, char *request_for, char *servername, char *portname){
    char buf[MAXLINE], method[MAXLINE], url[MAXLINE], version[MAXLINE];
    rio_t rp; 

    /* Read request lines and headers */
    Rio_readinitb(&rp, connfd);
    if (!Rio_readlineb(&rp, buf, MAXLINE)) {
        return -1;
    }
    printf("%s \n", buf);

    sscanf(buf, "%s %s %s", method, url, version);

    if (!strcasecmp(method, "GET")) {
        clienterror(connfd, method, "501", "Not implemented", 
                    "This method is not implemented");
        return -1;
    }
    
    char *ptr;
    /* Parse out host and port */
    if (!strstr(url, "http:")) {
        clienterror(connfd, url, "400", "Bad request", 
                    "Request cannot be understood");
        return -1;
    }

    /* Ignore '://' in the request body, and parse out host and port */
    ptr += 3;
    strcpy(servername, ptr);
    if (!(ptr = index(servername, ':')) {
        strcpy(portname, "8080");
    }
    else {
        strcpy(portname, ptr);
    }

    /* URI */
    if ()


}



/*
 * clienterror - Emit error message to socket client 
 *     at fd.
 */
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}