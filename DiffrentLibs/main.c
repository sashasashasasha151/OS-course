#include <stdio.h>
#include <dlfcn.h>

void printt(char *);

int add(int, int);

int mul(int, int);


int main() {
    printt("Hello world");
    printf("%d\n", add(1, 2));
    printf("%d\n", mul(3, 4));

    void *din_lib = dlopen("./libdins.so", RTLD_LAZY);
    if (!din_lib) {
        fprintf(stderr, "Lib opennig error");
        return 0;
    }

    int (*div)(int, int);
    div = dlsym(din_lib, "div");

    printf("%d\n", div(10, 2));

    dlclose(din_lib);

    return 0;
}