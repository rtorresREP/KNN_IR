#!/bin/bash
# k vecinos más cercanos
kvecinos=(1 2 3 5 10 20 50 70 100 250 500 750 1000 1500 3000 5000 7000 10000 15000) # k vecinos más
step=(1.1 1.2 1.3 1.41 1.5 1.6 1.7 1.8 1.9 2)
#start=(3 4 5 6 7 8) #inc
start=(1 1.41 2 2.83 4)
# Ruta al archivo de resultados
resultados_file="$HOME/RESULTADOS/resultKNN.txt"
ruta_dataset="$HOME/DATASETS/65536/RANDOM-KNN/"


for kvecino in "${kvecinos[@]}"; do
    echo "Ejecutando IR	 con k: $kvecino"
    for s in "${step[@]}"; do
        for st in "${start[@]}"; do
        
           #euclidean
       
            ./getTime $ruta_dataset"test4" $ruta_dataset"test4_q.txt" $kvecino 3 $s 1 $st >> "$resultados_file"
            ./getTime $ruta_dataset"test5" $ruta_dataset"test5_q.txt" $kvecino 3 $s 1 $st >> "$resultados_file"
            ./getTime $ruta_dataset"test6" $ruta_dataset"test6_q.txt" $kvecino 3 $s 1 $st >> "$resultados_file"
            ./getTime $ruta_dataset"test7" $ruta_dataset"test7_q.txt" $kvecino 3 $s 1 $st >> "$resultados_file"
            
            #manhattan
            
            ./getTime $ruta_dataset"test4" $ruta_dataset"test4_q.txt" $kvecino 3 $s 2 $st >> "$resultados_file"
            ./getTime $ruta_dataset"test5" $ruta_dataset"test5_q.txt" $kvecino 3 $s 2 $st >> "$resultados_file"
            ./getTime $ruta_dataset"test6" $ruta_dataset"test6_q.txt" $kvecino 3 $s 2 $st >> "$resultados_file"
            ./getTime $ruta_dataset"test7" $ruta_dataset"test7_q.txt" $kvecino 3 $s 2 $st >> "$resultados_file"
          done
      done
 done
  
