#include <unistd.h>
#include <iostream>

using namespace std;

int main(){
    
    auto *in = new char[2];
    in[0] = 0;
    char c;
    c = getchar();
    while ((c = getchar()) != EOF) {

        in[0] += 1;
        in[in[0]] = c;
        in = (char *) realloc(in, (in[0] + 2) * sizeof(char));

    }
    in[in[0]] = '\0';

    for (int i = 1; i < in[0]; i++) {

        in[i] = tolower(in[i]);

    }

    for (int i = 1; i < in[0] + 1; i++) {

        cout << in[i];
        
    }

    //printf("\n");
    //delete(in);
    fflush(stdout);
    close(STDOUT_FILENO);
    return 0;
}