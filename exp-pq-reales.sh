#!/bin/bash
# k vecinos más cercanos
kvecinos=(1 2 3 5 10 20 50 70 100 250 500 750 1000 1500 3000 5000 7000 10000 15000) # k vecinos más
# Ruta al archivo de resultados
resultados_file="$HOME/RESULTADOS/resultKNN.txt"
ruta_dataset="$HOME/DATASETS/REALES/"
ruta_query="$HOME/DATASETS/REALES/QUERY/"

#dataset grandes
for kvecino in "${kvecinos[@]}"; do
    echo "Ejecutando pq con k: $kvecino"
    
    
     	#euclidean
	    ./getTime $ruta_dataset"licencias" $ruta_query"licencias_q.txt" $kvecino 1 0 1 0 >> "$resultados_file"
	    ./getTime $ruta_dataset"crash" $ruta_query"crash_q.txt" $kvecino 1 0 1 0 >> "$resultados_file"
	    ./getTime $ruta_dataset"stream" $ruta_query"stream_q.txt" $kvecino 1 0 1 0 >> "$resultados_file"
	    ./getTime $ruta_dataset"street" $ruta_query"street_q.txt" $kvecino 1 0 1 0 >> "$resultados_file"
 	    ./getTime $ruta_dataset"tiger" $ruta_query"tiger_q.txt" $kvecino 1 0 1 0 >> "$resultados_file"
	    #manhattan
	    ./getTime $ruta_dataset"licencias" $ruta_query"licencias_q.txt" $kvecino 1 0 2 0 >> "$resultados_file"
	    ./getTime $ruta_dataset"crash" $ruta_query"crash_q.txt" $kvecino 1 0 2 0 >> "$resultados_file"
	    ./getTime $ruta_dataset"stream" $ruta_query"stream_q.txt" $kvecino 1 0 2 0 >> "$resultados_file"
	    ./getTime $ruta_dataset"street" $ruta_query"street_q.txt" $kvecino 1 0 2 0 >> "$resultados_file"
            ./getTime $ruta_dataset"tiger" $ruta_query"tiger_q.txt" $kvecino 1 0 2 0 >> "$resultados_file"
done

