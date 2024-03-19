#include <stdio.h>
#include <unistd.h>

int main() {
    int id;
    if((id = fork()) < 0) {
        printf("Kunne ikke forke\n");
        return 1;
    }

    if (id == 0) {
        printf("Child id = %d\n", id);
    } else {
        printf("Parrent id = %d\n", id);
    }

    printf("id = %d\n", id);
    getchar();
    
    return 0;
}