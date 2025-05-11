#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <mpi.h>

char encrypt_char(char c){
	if (c == 'z'){return 'c';}
	if (c == 'Z'){return 'C';}
	if (c == 'y'){return 'b';}
	if (c == 'Y'){return 'B';}
	if (c == 'x'){return 'a';}
	if (c == 'X'){return 'A';}
	if ((c >= 'a' && c < 'z') || (c >= 'A' && c < 'Z')) {return c+3;};
	return c;
}

char decrypt_char(char c){
	if (c == 'c'){return 'z';};
	if (c == 'C'){return 'Z';};
	if (c == 'b'){return 'y';};
	if (c == 'B'){return 'Y';};
	if (c == 'a'){return 'x';};
	if (c == 'A'){return 'X';};
	if ((c > 'a' && c < 'z') || (c > 'A' && c < 'Z')) {return c-3;};
	return c;
}

int main(int argc, char *argv[]) {
    int rank, size, encrypt;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if(rank == 0) {//the master process will not work on the array just distribute it
        int console;
        char string[1000];
        printf("Input mode, Enter 1 for console, 0 for file: ");
        fflush(stdout);
        scanf("%d", &console);
        printf("Enter 1 for encrypt, 0 for decrypt: ");
        fflush(stdout);
        scanf("%d", &encrypt);
        if(console){
            printf("Enter the string: ");
            fflush(stdout);
            scanf("%s", string);
        }else{
            FILE *file;
            char name[100];
            printf("Enter the file name: ");
            fflush(stdout);
            scanf("%s", name);
            file = fopen(name, "r");
            if (file == NULL) {
                printf("Failed to open file '%s'.\n", name);
                return 1;
            }
            fgets(string, sizeof(string), file);
            fclose(file);
printf("The Input string is: %s\n", string);
        }
        int length = strlen(string)+1;
        int chunk = length / (size - 1);
        int reminder = length % (size - 1);
        int start = 0;
        //distribute work
        for (int i = 1; i < size; i++) {
            MPI_Send(&encrypt, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        for (int i = 1; i < size; i++) {
            int cnt;
            if (i < reminder) { cnt = chunk + 1; }
            else { cnt = chunk; }
            MPI_Send(&cnt, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&string[start], cnt, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            start += cnt;
        }

        //receive array
        start = 0;
        for (int i = 1; i < size; i++) {
            int cnt;
            if (i < reminder) { cnt = chunk + 1; }
            else { cnt = chunk; }
            MPI_Recv(&string[start], cnt, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            start += cnt;
        }
        printf("Output: %s\n", string);
    }else{
        int cnt;
        MPI_Recv(&encrypt, 1, MPI_INT, 0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&cnt, 1, MPI_INT, 0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        char myPart[cnt];
        MPI_Recv(myPart, cnt, MPI_CHAR, 0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(encrypt){
            for(int i=0 ; i<cnt; i++){
                myPart[i] = encrypt_char(myPart[i]);
            }
        }else{
            for(int i=0 ; i<cnt; i++){
                myPart[i] = decrypt_char(myPart[i]);
            }
        }
        MPI_Send(myPart, cnt, MPI_CHAR,0,0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}