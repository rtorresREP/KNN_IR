#!/bin/bash

# Definir los valores que deseas probar
valores=(2 3 5 10 20 50 70 100 250 500 750 1000 1500 3000 5000 7000 10000 15000 ) # k vecinos más cercanos

# Definir la lista de valores step
step=(1.1 1.2 1.3 1.414213562 2.0 2.5 3.0 3.5 4.0 4.5 5.0)

# Ruta al archivo de resultados
resultados_file="$HOME/RESULTADOS/resultKNN.txt"

# Iterar sobre los valores y ejecutar la instrucción
for valor in "${valores[@]}"; do
    echo "Ejecutando knnpq con k: $valor"

      ./getTime DH_10M_262144 DH_10M_262144_q.txt "$valor" 1 0 1 >> "$resultados_file"
      ./getTime DG_10M_262144 DG_10M_262144_q.txt "$valor" 1 0 1 >> "$resultados_file"    
done

for valor in "${valores[@]}"; do
    echo "Ejecutando knn ir's con k: $valor"
    for s in "${step[@]}"; do
    
      ./getTime DH_10M_262144 DH_10M_262144_q.txt "$valor" 2 "$s" 1 >> "$resultados_file"
      ./getTime DH_10M_262144 DH_10M_262144_q.txt "$valor" 2 "$s" 2 >> "$resultados_file"
      ./getTime DG_10M_262144 DG_10M_262144_q.txt "$valor" 2 "$s" 1 >> "$resultados_file"
      ./getTime DG_10M_262144 DG_10M_262144_q.txt "$valor" 2 "$s" 2 >> "$resultados_file"
      
      ./getTime DH_10M_262144 DH_10M_262144_q.txt "$valor" 3 "$s" 1 >> "$resultados_file"
      ./getTime DG_10M_262144 DG_10M_262144_q.txt "$valor" 3 "$s" 1 >> "$resultados_file"
      
      ./getTime DH_10M_262144 DH_10M_262144_q.txt "$valor" 3 "$s" 2 >> "$resultados_file"
      ./getTime DG_10M_262144 DG_10M_262144_q.txt "$valor" 3 "$s" 2 >> "$resultados_file"

    done
done
