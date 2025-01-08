#!/bin/bash
#kvecinos=(1 2 3 5 10 20 50 70 100 250 500 750 1000 1500 3000 5000 7000 10000 15000 ) # k vecinos más cercanos

step=(1.1 1.2 1.3 1.41 2.0 2.5)
start=(0 1 2 3 4 5 6 7 8 9 10)
# Ruta al archivo de resultados
resultados_file="$HOME/RESULTADOS/resultKNN.txt"
ruta_dataset="$HOME/DATASETS/65536/GAUSS-KNN/"
ruta_query="$HOME/DATASETS/65536/QUERY1.txt"


for kvecino in $(seq 1 1 15000); do
    echo "Ejecutando knn con k: $kvecino"
    for s in "${step[@]}"; do
        for st in "${start[@]}"; do
           echo "Ejecutando pq"
           #euclidean
            ./getTime $ruta_dataset"test1-G" $ruta_query "$kvecino" 1 0 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test2-G" $ruta_query "$kvecino" 1 0 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test3-G" $ruta_query "$kvecino" 1 0 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test4-G" $ruta_query "$kvecino" 1 0 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test5-G" $ruta_query "$kvecino" 1 0 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test6-G" $ruta_query "$kvecino" 1 0 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test7-G" $ruta_query "$kvecino" 1 0 1 "$st" >> "$resultados_file"
            
            #manhattan
            ./getTime $ruta_dataset"test1-G" $ruta_query "$kvecino" 1 0 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test2-G" $ruta_query "$kvecino" 1 0 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test3-G" $ruta_query "$kvecino" 1 0 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test4-G" $ruta_query "$kvecino" 1 0 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test5-G" $ruta_query "$kvecino" 1 0 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test6-G" $ruta_query "$kvecino" 1 0 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test7-G" $ruta_query "$kvecino" 1 0 2 "$st" >> "$resultados_file"

            
        ##############################
        echo "Ejecutando IR MED"
            #euclidean     
            ./getTime $ruta_dataset"test1-G" $ruta_query "$kvecino" 3 $s 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test2-G" $ruta_query "$kvecino" 3 $s 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test3-G" $ruta_query "$kvecino" 3 $s 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test4-G" $ruta_query "$kvecino" 3 $s 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test5-G" $ruta_query "$kvecino" 3 $s 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test6-G" $ruta_query "$kvecino" 3 $s 1 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test7-G" $ruta_query "$kvecino" 3 $s 1 "$st" >> "$resultados_file"
                   
            #manhattan
            ./getTime $ruta_dataset"test1-G" $ruta_query "$kvecino" 3 $s 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test2-G" $ruta_query "$kvecino" 3 $s 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test3-G" $ruta_query "$kvecino" 3 $s 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test4-G" $ruta_query "$kvecino" 3 $s 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test5-G" $ruta_query "$kvecino" 3 $s 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test6-G" $ruta_query "$kvecino" 3 $s 2 "$st" >> "$resultados_file"
            ./getTime $ruta_dataset"test7-G" $ruta_query "$kvecino" 3 $s 2 "$st" >> "$resultados_file"

        done
    done
done
