#!/bin/bash
# k vecinos más cercanos
kvecinos=(1 2 3 5 10 20 50 70 100 250 500 750 1000 1500 3000 5000 7000 10000 15000) # k vecinos más
# Ruta al archivo de resultados
resultados_file="$HOME/RESULTADOS/resultKNN.txt"
ruta_dataset="$HOME/DATASETS/65536/RANDOM-KNN/"

for kvecino in "${kvecinos[@]}"; do
    echo "Ejecutando pq con k: $kvecino"
 	     #euclidean
         
            ./getTime $ruta_dataset"test4" $ruta_dataset"test4_q.txt" $kvecino 1 0 1 0  >> "$resultados_file"
            ./getTime $ruta_dataset"test5" $ruta_dataset"test5_q.txt" $kvecino 1 0 1 0  >> "$resultados_file"
            ./getTime $ruta_dataset"test6" $ruta_dataset"test6_q.txt" $kvecino 1 0 1 0  >> "$resultados_file"
            ./getTime $ruta_dataset"test7" $ruta_dataset"test7_q.txt" $kvecino 1 0 1 0  >> "$resultados_file"
            
            #manhattan
            
            ./getTime $ruta_dataset"test4" $ruta_dataset"test4_q.txt" $kvecino 1 0 2 0 >> "$resultados_file"
            ./getTime $ruta_dataset"test5" $ruta_dataset"test5_q.txt" $kvecino 1 0 2 0 >> "$resultados_file"
            ./getTime $ruta_dataset"test6" $ruta_dataset"test6_q.txt" $kvecino 1 0 2 0 >> "$resultados_file"
            ./getTime $ruta_dataset"test7" $ruta_dataset"test7_q.txt" $kvecino 1 0 2 0 >> "$resultados_file"
            
           

done


