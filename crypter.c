#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define KEY 0x5A  

// encryptr file
void encryptFile(const char *inputFile, const char *outputFile) {
    FILE *inFile = fopen(inputFile, "rb");
    FILE *outFile = fopen(outputFile, "wb");

    if (!inFile || !outFile) {
        perror("fail to open file");
        exit(EXIT_FAILURE);
    }

    int ch;
    while ((ch = fgetc(inFile)) != EOF) {
        fputc(ch ^ KEY, outFile);  // xor encode
    }

    fclose(inFile);
    fclose(outFile);

    printf("file is encrypted :%s\n", outputFile);
}

// decryptr file
void decryptFile(const char *inputFile, const char *outputFile) {
    FILE *inFile = fopen(inputFile, "rb");
    FILE *outFile = fopen(outputFile, "wb");

    if (!inFile || !outFile) {
        perror("fail to open file");
        exit(EXIT_FAILURE);
    }

    int ch;
    while ((ch = fgetc(inFile)) != EOF) {
        fputc(ch ^ KEY, outFile);  // xor decode
    }

    fclose(inFile);
    fclose(outFile);

    printf("file is decrypted :%s\n", outputFile);
}

int main() {
    const char *plainTextFile = "plain.txt";       // source
    const char *encryptedFile = "encrypted.txt";  // encrypt file
    const char *decryptedFile = "decrypted.txt";  // decrypt file

    encryptFile(plainTextFile, encryptedFile);    
    decryptFile(encryptedFile, decryptedFile);

    return 0;
}
