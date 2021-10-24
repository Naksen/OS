#include <unistd.h>
#include <iostream>

using namespace std;

int main(){
    
    auto *in = new char[2];
    in[0] = 0;
    char c;
    while ((c = getchar()) != EOF) {

        in[0] += 1;
        in[in[0]] = c;
        in = (char *) realloc(in, (in[0] + 2) * sizeof(char));

    }
    in[in[0]] = '\0';

    char *out = (char *) malloc(2 * sizeof(char));
    out[0] = 0;

    for (int i = 1; i < in[0] + 1; i++) {

        if (in[i] == ' ' && in[i + 1] == ' ') {

            i++;
            continue;

        }

        out[0]++;
        out[out[0]] = in[i];
        out = (char *) realloc(out, (out[0] + 2) * sizeof(out));

    }

    out[0]++;
    out[out[0]] = '\0';

    for (int i = 1; i < out[0]; i++) {

        cout << out[i];
        
    }

    printf("\n");
    fflush(stdout);
    //delete(in);
    //delete(out);
    close(STDOUT_FILENO);
    return 0;
}