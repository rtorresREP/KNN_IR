#!/bin/bash
# Definición de variables
kvecinos=(1 2 3 5 10 20 50 70 100 250 500 750 1000 1500 3000 5000 7000 10000 15000) # k vecinos más
step=(1.1 1.2 1.3 1.41 1.5 1.6 1.7 1.8 1.9 2)
#start=(3 4 5 6 7 8) #inc
start=(1 1.41 2 2.83 4)
resultados_file="$HOME/RESULTADOS/resultKNN.txt"
ruta_dataset="$HOME/DATASETS/262144/"

# Iteración sobre diferentes valores de k vecinos
for kvecino in "${kvecinos[@]}"; do
    echo "Ejecutando knn con k: $kvecino"
    for s in "${step[@]}"; do
        for st in "${start[@]}"; do
            # Euclidean
            for dataset in "DH_1M_262144" "DH_10M_262144" "DH_100M_262144"; do
                ./getTime "${ruta_dataset}${dataset}" "${ruta_dataset}${dataset}_q.txt" "$kvecino" 3 "$s" 1 "$st" >> "$resultados_file"
            done
               for dataset in "DG_1M_262144" "DG_10M_262144" "DG_100M_262144"; do
                ./getTime "${ruta_dataset}${dataset}" "${ruta_dataset}${dataset}_q.txt" "$kvecino" 3 "$s" 1 "$st" >> "$resultados_file"
            done

            # Manhattan
            for dataset in "DH_1M_262144" "DH_10M_262144" "DH_100M_262144"; do
                ./getTime "${ruta_dataset}${dataset}" "${ruta_dataset}${dataset}_q.txt" "$kvecino" 3 "$s" 2 "$st" >> "$resultados_file"
            done
            for dataset in "DG_1M_262144" "DG_10M_262144" "DG_100M_262144"; do
                ./getTime "${ruta_dataset}${dataset}" "${ruta_dataset}${dataset}_q.txt" "$kvecino" 3 "$s" 2 "$st" >> "$resultados_file"
            done

        done
    done
done

