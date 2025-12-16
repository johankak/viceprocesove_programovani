#!/bin/bash

# Script pro opakovane spousteni deadlock programu

counter=1

echo "Spoustim deadlock v nekonecne smycce..."
echo "Pro ukonceni stiskni Ctrl+C"
echo "================================"
echo ""

while true; do
    echo ">>> Pokus #$counter <<<"
    
    # Spust deadlock s timeoutem 5 sekund
    ./deadlock
    
    
    echo ""
    ((counter++))
done
