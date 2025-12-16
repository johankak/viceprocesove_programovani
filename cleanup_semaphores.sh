#!/bin/bash

# Script pro uvolneni POSIX semaforu

echo "Uvolnuji semafory..."

# POSIX semafory jsou ulozeny v /dev/shm/ s prefixem "sem."
# Pro semafory /sem_monster a /sem_bago jsou soubory:
# /dev/shm/sem.sem_monster a /dev/shm/sem.sem_bago

rm -f /dev/shm/sem.sem_monster 2>/dev/null && echo "    sem_monster smazan"
rm -f /dev/shm/sem.sem_bago 2>/dev/null && echo "    sem_bago smazan"

# Zobraz zbyvajici semafory
echo "Zbyvajici semafory v adresari /dev/shm/:"
ls -la /dev/shm/sem.* 2>/dev/null || echo "    zadne semafory nenalezeny"

echo "Hotovo!"
