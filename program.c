#include <stdio.h>
#include <stdlib.h>
#include <printf.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#define stop 0
#define start 1
#define rozmiar 16
#define status 727271
#define glowny 727272

bool dziala(pid_t p) 
{
    if (!kill(p, 0)) 
	{
		return true;
	}
    else return false;
}

struct piecbajtow 
{
    char *bufor;
    bool got; 
    unsigned long long licz;
};

void FBB1(struct piecbajtow *fbb)
{
    fbb->got = false;
    fbb->bufor = calloc(5, sizeof(char));
    fbb->licz = 0;
}

void FBB2(struct piecbajtow *fbb, char l)
{
    fbb->bufor[fbb->licz % 5] = l;
    fbb->licz++;
    if (fbb->licz > 4) fbb->got = true;
}

bool sprawdz(struct piecbajtow *fbb) 
{
    if (!fbb->got) 
	{
		return false;
	}
    for (int i = 1; i < 5; i++) 
	{
        if (fbb->bufor[0] != fbb->bufor[i]) return false;
    }
    return true;
}

const char *bin(char l) 
{
    static char k[9];
    k[0] = '\0';
    for (int m = 128; m > 0; m >>= 1) 
	{
        strcat(k, ((l & m) == m) ? "1" : "0");
    }
    return k;
}

void showFBB(struct piecbajtow *fbb) {
    if (!fbb->got) return;
    for (int i = 0; i < 5; i++) {
        char l = fbb->bufor[i];
        printf("[%llu] %s\n", (unsigned long long) fbb->licz - 5 + i, bin(l));
    }
    printf("\n\n\n;
}

int main() {
    srand((unsigned int) time(NULL));
    pid_t pid = fork();
    if (!pid) 
	{
		pid_t ppid = getppid();
        int id1 = shmget(glowny, rozmiar, IPC_CREAT | 0644), id2 = shmget(status, sizeof(bool), IPC_CREAT | 0644);
        if (id1 == -1 || id2 == -1) 
		{
            printf("Blad\n\n");
            _exit(1);
        }
        int *buf1 = (int *) shmat(id1, NULL, 0);
        bool *buf2 = (bool *) shmat(id2, NULL, 0);
		buf2[0] = start;
        if (buf1 == NULL || buf2 == NULL) 
		{
            printf("Blad\n\n");
            _exit(1);
        }
        for (int i = 0; buf2[0] && dziala(ppid) == start; i++) 
		{
            buf1[i % rozmiar] = (char) rand() % 256;
        }
        _exit(0);
    }
    int id3 = shmget(glowny, rozmiar, IPC_CREAT | 0644), id4 = shmget(status, sizeof(bool), IPC_CREAT | 0644);
    if (id3 == -1 || id4 == -1) 
		printf("Blad\n\n");
    int *buf3 = (int *) shmat(id3, NULL, 0);
    bool *buf4 = (bool *) shmat(id4, NULL, 0);
    if (buf3 == NULL || buf4 == NULL) 
		printf("Blad\n\n");
    buf4[0] = start;
    struct piecbajtow *piecbajtow = malloc(sizeof(piecbajtow));
    FBB1(piecbajtow);
    for (int i = 0; true; i++) 
	{
        FBB2(piecbajtow, buf3[i % rozmiar]);
        if (sprawdz(piecbajtow)) 
		{
            showFBB(piecbajtow);
            printf("Znalezione\n");
            buf4[0] = stop;
            break;
        }
    }
    wait(NULL);
    return 0;