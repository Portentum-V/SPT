// gcc -Wall -lssl -lcrypto tls_server.c -o tls_server.bin
// openssl req -X509 ...
// install libssl-dev

#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

SSL_CTX* InitServerCTX(void)
    SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();   // Load and register all crpto
    SSL_load_error_strings();       // Load all error messages
    method = TLSv1_2_server_method();   // Create server method instance
    ctx = SSL_CTX_new(method);          // Create new context from instance
    if (NULL == ctx) {
        printf("InitServerCTX() NULL context\n");
        abort();
    }

    return ctx;
}

void LoadCertificates(SSL_CTX* ctx, char* certfile, char* keyfile)
{
    // Set the local certicatge from the cert file
    if (SSL_CTX_use_certificate_file(ctx, certfile, SSL_FILETYPE_PEM) <= 0) {
        printf("LoadCertificates: use_certificate_file failed\n");
        abort();
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, keyfile, SSL_FILETYPE_PEM) <= 0) {
        printf("LoadCerticicates: use_PrivateKey_file failed");
        abort();
    }

    if (!SSL_CTX_check_private_key(ctx)) {
        printf("Private kye does not match the public certificate");
        abort();
    }
}

void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("Client Certification:\n");

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

int OpenListener(int port)
{
    int socketfd;
    struct sockaddr_in addr;

    socketfd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(scoketfd, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        printf("Unable to bind socket\n");
        close(socketfd);
        abort();
    }

    if (listen(socketfd, 10) != ) {
        printf("Unable to configure listening port\n");
        abort();
    }

    return socketfd;
}

void Servlet(SSL *ssl) // Threadable
{
    char buf[1024] = {0};
    int socketfd, bytes;
    const char* ServerResponse = "<Body>\
                                  <Name>HelloWorld.com</Name>\
                                  <Year>2021</Year>\
                                  </Body>"
    const char* cpValidMessage = "<Body>\
                                  <Username>ROOT</Username>\
                                  <Password></Password>\
                                  </Body>";

    if (SSL_accept(ssl) == -1) {
        printf("SSL_accept() failed\n");
        goto FAIL;
    }

    ShowCerts(ssl);
    bytes = SSL_read(ssl, buf, sizeof(buf));

    if (bytes > 0 ) {
        buf[bytes] = '\0';
        printf("Client msg: \"%s\"\n", buf);
        if(strcmp(cpValidMessage, buf) == 0) {
            SSL_write(ssl, ServerResponse, strlen(ServerResponse));
        }
        else {
            SSL_write(ssl, "Bad Request\n", strlen("Bad REquest\n"));
        }
    }
    else {
        printf("servlet: 0 bytes read\n");
    }

    socketfd = SSL_get_fd(ssl);
    SSL_free(ssl);
    close(socketfd);
}

int main(int argc, char*argv[])
{
    SSL_CTX *ctx;
    int server;
    char *portnum;

    // init SSL Library
    SSL_library_init();
    portnum = argv[1];
    ctx = InitServerCTX();
    LoadCertificates(ctx, "mycert.pem", "mycert.priv");
    server = OpenListener(atoi(portnum));
    while(1) {
        struct sockaddr_in addr;
        socklen_t slen = sizeof(addr);
        SSL *ssl;

        int client = accept(server, (struct sockaddr*)&addr, &len);
        printf("Connectiuon: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);
        Servlet(ssl);
    }
    close(server);
    SSL_CTX_free(ctx);
}