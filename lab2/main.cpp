#include <iostream>
#include <unistd.h>
#include <cctype>
#include <algorithm>
#include <sys/wait.h>

using namespace std;

int main() {
    int fd[2];
    int fd_1[2];
    int fd_2[2];
    pipe(fd); // pipe: Child_1 -> Child_2
    pipe(fd_1); // pipe: Parent -> Child_1
    pipe(fd_2); // pipe: Child_2 -> Parent
    int pid_1 = 0;
    int pid_2 = 0;
    //reading

    auto *in = new char[2];
    in[0] = 0;
    char c;
    while ((c = getchar()) != EOF) {

        in[0] += 1;
        in[in[0]] = c;
        in = (char *) realloc(in, (in[0] + 2) * sizeof(char));

    }
    in[in[0]] = '\0';
    
    if ((pid_1 = fork()) > 0) { // Parent 1
        
        //cout << "||Parent 1||" << "\n";
        write(fd_1[1], in, (in[0] + 2) * sizeof(char));
        close(fd_1[1]);
        close(fd_1[0]);
        close(fd[1]);

        if ((pid_2 = fork()) > 0) { // Parent 2

            //cout << "||Parent 2||" << "\n";

            close(fd_2[1]);
            close(fd[1]);
            close(fd[0]);

            char ch;
            while(read(fd_2[0], &ch, sizeof(char)) > 0){
                putchar(ch);
            }

        } else if (pid_2 == 0) { //Child_2
            
            //cout << "||Child 2 ||" << "\n";

            if (dup2(fd_2[1], fileno(stdout)) == -1 ) {
            
                perror("dup2");
                exit(1);

            }
            close(fd_2[1]);

            if (dup2(fd[0], fileno(stdin)) == -1) {

                perror("dup2");
                exit(1);

            }
            close(fd[0]);

            //cout << "child2 is opening...\n"; 
            execl("child2", "child2", NULL);
            perror("execl");


        } else { 
            
            cout << "fork error" << endl;
            exit(-1);
        
        }

    } else if (pid_1 == 0) { //Child_1
        
        //cout << "||Child 1 ||" << "\n";
        if (dup2(fd[1], fileno(stdout)) == -1 ) {
            
            perror("dup2");
            exit(1);

        }
        close(fd[1]);
        if (dup2(fd_1[0], fileno(stdin)) == -1) {

            perror("dup2");
            exit(1);

        }
        close(fd_1[0]);
        close(fd_1[1]);
        //cout << "child1 is opening...\n"; 
        execl("child1", "child1", NULL);
        perror("execl");

    } else {

        cout << "Fork error" << endl;
        exit(-1);
    
    }
    return 0;
}
