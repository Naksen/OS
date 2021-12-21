#include <stdio.h>
#include <dlfcn.h>

int main(){
    void* cur_lib = NULL;
    float (*Derivative)(float A, float deltaX);
    float (*E)(int x);
    
    //Загрузка библиотек:
    int key;
    int lib_num;
    printf("Library: [1/2]?\n");
    scanf("%d",&lib_num);

    //Открываем нужную библиотеку:
    if (lib_num == 1) {

        cur_lib = dlopen("/home/naksan/cprog/5_lab_OS/libd1.so",RTLD_LAZY); 

    } else if (lib_num == 2) {

        cur_lib = dlopen("/home/naksan/cprog/5_lab_OS/libd2.so",RTLD_LAZY);

    } else {

        printf("You don't write start key\n");

    }

    //Нахождения адрессов функций:
    Derivative = dlsym(cur_lib, "Derivative");
    E = dlsym(cur_lib, "E");

    while (scanf("%d",&key) > 0 ) {

        if (key == 1) {

            float A,deltaX;
            scanf("%f%f",&A,&deltaX);
            printf("%s(%f, %f) = %f\n","Derivative", A, deltaX,(*Derivative)(A, deltaX));

        } else if (key == 2) {

            int x;
            scanf("%d",&x);
            printf("%s(%d) = %f\n","E",x,(*E)(x));

        } else if (key == 0) {

            dlclose(cur_lib);

            if (lib_num == 1) {

                cur_lib = dlopen("./libd2.so",RTLD_LAZY);
                lib_num = 2;

            } else if (lib_num == 2) {

                cur_lib = dlopen("./libd1.so",RTLD_LAZY);
                lib_num = 1;

            }

            Derivative = dlsym(cur_lib,"Derivative");
            E = dlsym(cur_lib, "E");
            
        }

    }

    dlclose(cur_lib);
}