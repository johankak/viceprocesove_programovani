#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

//globalni sdilena promenna:
int *monster;
int *bago;

//semafory:
sem_t *sem_monster;
sem_t *sem_bago;

int main() {

    // Inicializace semaforu
    sem_monster = sem_open("/sem_monster", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
    if (sem_monster == SEM_FAILED) {
        perror("sem_open sem_monster");
        exit(1);
    }
    
    sem_bago = sem_open("/sem_bago", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
    if (sem_bago == SEM_FAILED) {
        perror("sem_open sem_bago");
        sem_close(sem_monster);
        sem_unlink("/sem_monster");
        exit(1);
    }


    // Fork - vytvoreni dvou procesu
    pid_t pid = fork();

    
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    
    if (pid == 0) {
        // CHILD PROCES - zamkne bago, pak chce monster
        printf("Child: Zacinam, budu chtit bago a pak i monster\n");
        fflush(stdout);
        
        sem_wait(sem_bago);  // Zamkni bago
        printf("Child: Mam bago!\n");
        fflush(stdout);
        
        printf("Child: Chci monster...\n");
        fflush(stdout);
        sem_wait(sem_monster);  // Zkus ziskat monster (muze deadlock!)
        printf("Child: Mam oba zdroje (bago + monster)!\n");
        fflush(stdout);

        
        // Odemkni
        printf("Child: Uvolnuji prostredky\n");
        fflush(stdout);
        sem_post(sem_monster);
        sem_post(sem_bago);
        
        printf("Child: Koncim\n");
        fflush(stdout);
        exit(0);
    } else {
        // PARENT PROCES - zamkne monster, pak chce bago
        printf("Parent: Zacinam, budu chtit monster a pak i bago\n");
        fflush(stdout);
        
        sem_wait(sem_monster);  // Zamkni monster
        printf("Parent: Mam monster!\n");
        fflush(stdout);
        
        printf("Parent: Chci bago...\n");
        fflush(stdout);
        sem_wait(sem_bago);  // Zkus ziskat bago (muze deadlock!)
        printf("Parent: Mam oba zdroje (monster + bago)!\n");
        fflush(stdout);
        
        // Odemkni
        printf("Parent: Uvolnuji prostredky\n");
        fflush(stdout);
        sem_post(sem_bago);
        sem_post(sem_monster);
        
        printf("Parent: Cekam na child...\n");
        fflush(stdout);
        wait(NULL);  // Pockej na child proces
        
        printf("Parent: Koncim\n");
        fflush(stdout);
    }

    // Cleanup semaforu
    sem_close(sem_monster);
    sem_close(sem_bago);
    sem_unlink("/sem_monster");
    sem_unlink("/sem_bago");
    
    return 0;
}