#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(){

    //cout << "Child1 is open\n";
    
    auto *in = new char[2];
    in[0] = 0;
    char c;
    while ((c = getchar()) != EOF) {

        in[0] += 1;
        in[in[0]] = c;
        in = (char *) realloc(in, (in[0] + 2) * sizeof(char));

    }
    in[in[0]] = '\0';

    char *out = (char *) malloc((in[0] + 2) * sizeof(char));
    out[0] = in[0];

    for (int i = 1; i < in[0] + 1; i++) {

        out[i] = tolower(in[i]);

    }

    for (int i = 1; i < out[0]; i++) {

        cout << out[i];
        
    }

    printf("\n");
    fflush(stdout);
    close(STDOUT_FILENO);
    return 0;
}