// gcc -Wall -lssl -lcrypto tls_client.c -o tls_client.bin
// Install libssl-dev
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// Create raw socket
int OpenConnection(const char* hostname, int port)
{
    int socketfd;
    struct hostent *host;
    struct sockaddr_in addr;
    if ((host = gethostbyname(hostname)) == NULL) {
        perror(hostname);
        abort(); // Some SSL function?
    }

    socketfd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);

    if (connect(scoketfd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        close(socketfd);
        perror(hostname);
        abort();
    }

    return socketfd;
}

SSL_CTX* InitCTX(void)
{
    SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    method = TLSv1_2_client_method();
    ctx = SSL_CTX_new(method);
    if (NULL == ctx) {
        printf("InitCTX() NULL context\n");
        abort();
    }

    return ctx;
}

void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("Server Certification:\n");

        line = X509_NAME_oneline(X509_get_subject_name(cert)), 0, 0);
        printf("SubjectL %s\n", line);

        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);

        free(line);
        X509_free(cert);
    }

    else {
        printf("No cert from SSL_get_peer_certificate()\n");
    }
}

int main(int argc, char* argv[]) {
    SSL_CTX *ctx;   // SSL Context
    SSL *ssl;       // SSL Object
    int socketfd;     // Socket info
    char buf[1024];  // Response
    char acClientRequest[1024] = {0}; // Request
    int bytes;
    char *hostname, *portnum;

    if (argc != 3) {
        printf("Usage: %s <hostname> <port number>\n", argv[0]);
        exit(0);
    }

    // init SSL Library
    SSL_library_init();
    hostname = argv[1];
    portnum = argv[2];

    ctx = InitCTX();
    socketfd = OpenConnection(hostname, atoi(portnum));
    ssl = SSL_new(ctx);         // Create a new SSL connection State
    SSL_set_fd(ssl, socketfd);  // Attach the sockfd

    if (SSL_connectt(ssl) == -1) {
        prtinf("Failure to connect via SSL_connect()\n");
        goto FAIL;
    }
    
    char acUsername[16] = {0};
    char acPassword[16] = {0};
    const char *cpRequestMessage = "<Body>\
                                    <Username>%s</Username>\
                                    <Password>%s</Password>\
                                    </Body>";
    printf("Username: ");
    scanf("%s", acUsername);
    printf("Password: ");
    scanf("%s", acPassword);

    sprintf(acClientRequest, cpRequestMessage, acUsername, acPassword);
    
    printf("\n\nConnected with %s encryption\n", SSL_get_cipher(ssl));
    ShowCerts(ssl);
    SSL_write(ssl, acClientRequest, strlen(acClientRequest));
    bytes = SSL_read(ssl, buf, sizeof(buf));
    buf[bytes] = 0;
    printf("Received: %s", buf);
    SSL_free(ssl);

FAIL:
    close(socketfd);
    SSL_CTX_free(ctx);
    return 0;
}