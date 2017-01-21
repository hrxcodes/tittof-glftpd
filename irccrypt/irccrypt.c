#include "base64.h"
#include <string.h>
#include <sodium.h>
#define PASSWORD "Correct Horse Battery Staple"
#define MESSAGE ((const unsigned char *) "We make files transfer!")
#define MESSAGE_LEN 23
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES + MESSAGE_LEN)
#define PREFIX_LEN 4

unsigned char salt[crypto_pwhash_SALTBYTES];
unsigned char nonce[crypto_secretbox_NONCEBYTES];
unsigned char key[crypto_secretbox_KEYBYTES];
unsigned char ciphertext[CIPHERTEXT_LEN];
unsigned char decrypted[MESSAGE_LEN];

int main () {

    /* let's derive the key from PASSWORD */
    randombytes_buf(salt, crypto_pwhash_SALTBYTES);
    if (crypto_pwhash
            (key, crypto_secretbox_KEYBYTES, PASSWORD, strlen(PASSWORD), salt,
             crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE,
             crypto_pwhash_ALG_DEFAULT) != 0) {
            /* out of memory */
            exit(2);
    }

    /* generate nonce and encrypt the message */
    randombytes_buf(nonce, crypto_secretbox_NONCEBYTES);
    crypto_secretbox_easy(ciphertext, MESSAGE, MESSAGE_LEN, nonce, key);
    /* put nonce and ciphertext together */
    unsigned char nonce_ciphertext[CIPHERTEXT_LEN + crypto_secretbox_NONCEBYTES];
    memcpy(nonce_ciphertext, nonce, crypto_secretbox_NONCEBYTES);
    memcpy(nonce_ciphertext+crypto_secretbox_NONCEBYTES, ciphertext, CIPHERTEXT_LEN);
    /* and base64 encode it */
    unsigned char base64_nonce_ciphertext[Base64encode_len(CIPHERTEXT_LEN + crypto_secretbox_NONCEBYTES)];
    Base64encode(base64_nonce_ciphertext, nonce_ciphertext, CIPHERTEXT_LEN + crypto_secretbox_NONCEBYTES);

    /*
     * mind the 512 byte limit!
     * :nickname!ident@userhostname.invalid PRIVMSG #ircchannel :+XS XXXXXX\r\n
     */

    /* now send base64_nonce_ciphertext to the other side */

    /* and base64 decode it */
    Base64decode(nonce_ciphertext, base64_nonce_ciphertext);
    /* split it into nonce and ciphertext again */
    memcpy(nonce, nonce_ciphertext, crypto_secretbox_NONCEBYTES);
    memcpy(ciphertext, nonce_ciphertext + crypto_secretbox_NONCEBYTES, MESSAGE_LEN);

    /* decrypt MESSAGE */
    if (crypto_secretbox_open_easy(decrypted, ciphertext, CIPHERTEXT_LEN, nonce, key) != 0) {
            /* cannot decrypt */
        exit(1);
    }

    printf("cleartext:%s\n:user!ident@hostname.invalid PRIVMSG #ircchannel :+XZ %s\ndecrypted:%s\n",MESSAGE, base64_nonce_ciphertext, decrypted);
}
