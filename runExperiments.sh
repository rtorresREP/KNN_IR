#!/bin/bash

kvecinos=(1 2 3 5 10 20 50 70 100 250 500 750 1000 1500 3000 5000 7000 10000 15000 ) # k vecinos más cercanos
# Definir la lista de valores step
step=(1.1 1.2 1.3 1.41 2.0 2.5)

start=(0 1 2 3 4 5 6 7 8 9 10)
# Ruta al archivo de resultados
resultados_file="$HOME/RESULTADOS/resultKNN.txt"

# Iterar sobre los valores y ejecutar la instrucción

for kvecino in "${kvecinos[@]}"; do
    echo "Ejecutando knn ir's con k: $kvecino"
    for s in "${step[@]}"; do
        for st in "${start[@]}"; do
        
            ./getTime DH_05_1024 DH_05_1024_q.txt "$kvecino" 1 0 1 "$st" >> "$resultados_file"
            ./getTime DH_80_1024 DH_80_1024_q.txt "$kvecino" 1 0 1 "$st" >> "$resultados_file"
            ./getTime DH_50_1024 DH_50_1024_q.txt "$kvecino" 1 0 1 "$st" >> "$resultados_file"
            
            ./getTime DH_1M_262144 DH_1M_262144_q.txt "$kvecino" 1 0 1 "$st" >> "$resultados_file"
      	    ./getTime DH_10M_262144 DH_10M_262144_q.txt "$kvecino" 1 0 1 "$st" >> "$resultados_file"
            ./getTime DH_100M_262144 DH_100M_262144_q.txt "$kvecino" 1 0 1 "$st">> "$resultados_file"
            
      
            ./getTime DH_05_1024 DH_05_1024_q.txt "$kvecino" 1 0 2 "$st" >> "$resultados_file"
            ./getTime DH_80_1024 DH_80_1024_q.txt "$kvecino" 1 0 2 "$st" >> "$resultados_file"
            ./getTime DH_50_1024 DH_50_1024_q.txt "$kvecino" 1 0 2 "$st" >> "$resultados_file"
            
            ./getTime DH_1M_262144 DH_1M_262144_q.txt "$kvecino" 1 0 2 "$st" >> "$resultados_file"
      	    ./getTime DH_10M_262144 DH_10M_262144_q.txt "$kvecino" 1 0 2 "$st" >> "$resultados_file"
            ./getTime DH_100M_262144 DH_100M_262144_q.txt "$kvecino" 1 0 2 "$st">> "$resultados_file"
        
        ##############################
        ##############################
        ##############################
        
            ./getTime DH_05_1024 DH_05_1024_q.txt "$kvecino" 3 "$s" 1 "$st" >> "$resultados_file"
            ./getTime DH_80_1024 DH_80_1024_q.txt "$kvecino" 3 "$s" 1 "$st" >> "$resultados_file"
            ./getTime DH_50_1024 DH_50_1024_q.txt "$kvecino" 3 "$s" 1 "$st" >> "$resultados_file"
            
            ./getTime DH_1M_262144 DH_1M_262144_q.txt "$kvecino" 3 "$s" 1 "$st" >> "$resultados_file"
      	    ./getTime DH_10M_262144 DH_10M_262144_q.txt "$kvecino" 3 "$s" 1 "$st" >> "$resultados_file"
            ./getTime DH_100M_262144 DH_100M_262144_q.txt "$kvecino" 3 "$s" 1 "$st">> "$resultados_file"
            
      
            ./getTime DH_05_1024 DH_05_1024_q.txt "$kvecino" 3 "$s" 2 "$st" >> "$resultados_file"
            ./getTime DH_80_1024 DH_80_1024_q.txt "$kvecino" 3 "$s" 2 "$st" >> "$resultados_file"
            ./getTime DH_50_1024 DH_50_1024_q.txt "$kvecino" 3 "$s" 2 "$st" >> "$resultados_file"
            
            ./getTime DH_1M_262144 DH_1M_262144_q.txt "$kvecino" 3 "$s" 2 "$st" >> "$resultados_file"
      	    ./getTime DH_10M_262144 DH_10M_262144_q.txt "$kvecino" 3 "$s" 2 "$st" >> "$resultados_file"
            ./getTime DH_100M_262144 DH_100M_262144_q.txt "$kvecino" 3 "$s" 2 "$st">> "$resultados_file"
            
            
        done
    done
done
