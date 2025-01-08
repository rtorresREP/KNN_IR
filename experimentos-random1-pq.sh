#!/bin/bash
# Definición de variables
kvecinos=(1 2 3 5 10 20 50 70 100 250 500 750 1000 1500 3000 5000 7000 10000 15000) # k vecinos más
resultados_file="$HOME/RESULTADOS/resultKNN.txt"
ruta_dataset="$HOME/DATASETS/262144/"

# Iteración sobre diferentes valores de k vecinos
for kvecino in "${kvecinos[@]}"; do
    echo "Ejecutando knn con k: $kvecino"
    
            echo "Ejecutando IR MED con start: "$st" y step: "$s
            # Euclidean
            for dataset in "DH_1M_262144" "DH_10M_262144" "DH_100M_262144"; do
                ./getTime "${ruta_dataset}${dataset}" "${ruta_dataset}${dataset}_q.txt" "$kvecino" 1 0 1 0 >> "$resultados_file"
            done
            
            for dataset in "DG_1M_262144" "DG_10M_262144" "DG_100M_262144"; do
                ./getTime "${ruta_dataset}${dataset}" "${ruta_dataset}${dataset}_q.txt" "$kvecino" 1 0 1 0 >> "$resultados_file"
            done

            # Manhattan
            for dataset in "DH_1M_262144" "DH_10M_262144" "DH_100M_262144"; do
                ./getTime "${ruta_dataset}${dataset}" "${ruta_dataset}${dataset}_q.txt" "$kvecino" 1 0 2 0 >> "$resultados_file"
            done
            
            for dataset in "DG_1M_262144" "DG_10M_262144" "DG_100M_262144"; do
                ./getTime "${ruta_dataset}${dataset}" "${ruta_dataset}${dataset}_q.txt" "$kvecino" 1 0 2 0 >> "$resultados_file"
            done

done

