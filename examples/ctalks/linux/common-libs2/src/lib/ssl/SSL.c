#include <stdlib.h>
#include <openssl/err.h>

#include "SSL.h"

void SSL_initLibs(){
	/* Global system initialization*/
	SSL_library_init();
    /*OpenSSL_add_all_algorithms();*/
	SSL_load_error_strings();
}


/* Note: This Function is not Thread Safe */
SSL_CTX *SSL_initializeContext(SSLContextMode mode, SSLClientAuthentication type, char *certfile,  char *keyfile) {
	SSL_CTX *context = NULL;

	/* Create our context*/
	if( mode == SSL_CLIENT ) context = SSL_CTX_new(  SSLv23_client_method() );
	else if( mode == SSL_SERVER ) context = SSL_CTX_new(  SSLv23_server_method() );
	else return NULL;

	if( context == NULL ){
		ERR_print_errors_fp(stderr);
		return NULL;
	}

	if( type == AUTHENTICATION_REQUIRE){
		/* Load our keys and certificates*/
		if ( SSL_CTX_use_certificate_file(context, certfile, SSL_FILETYPE_PEM) <= 0){
			ERR_print_errors_fp(stderr);
			SSL_CTX_free( context );
			return NULL;
		}

		if ( SSL_CTX_use_PrivateKey_file(context, keyfile, SSL_FILETYPE_PEM) <= 0 ){
			ERR_print_errors_fp(stderr);
			SSL_CTX_free( context );
			return NULL;
		}

		if ( !SSL_CTX_check_private_key(context) ){
			ERR_print_errors_fp(stderr);
			SSL_CTX_free( context );
			return NULL;
		}
	}
	return context;
}

