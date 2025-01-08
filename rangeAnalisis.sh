#!/bin/bash

# Definir los valores de radio que deseas probar
valores=(3 5 8 10 15 20 25 30 35 40 45 50 55 60 70 75 90 100 150 250 300 400 500 600 700) # RADIO


# Ruta al archivo de resultados
resultados_file="$HOME/RESULTADOS/resultRANGE.txt"
#<PATH K2TREE> <X_1> <Y_1> <type range 1=compactRange, 2= RANGE > <radio> 

# Iterar sobre los valores y ejecutar la instrucción
for radio in "${valores[@]}"; do
    echo "Ejecutando algoritmos con radio: $radio"
      ./rangeAnalisis DH_05_1024 100 150 1 "$radio" >> "$resultados_file"
      ./rangeAnalisis DH_50_1024 100 150 1 "$radio"  >> "$resultados_file"
      ./rangeAnalisis DH_80_1024 100 150 1 "$radio" >> "$resultados_file"

      ./rangeAnalisis DH_05_1024 100 150 2 "$radio" >> "$resultados_file"
      ./rangeAnalisis DH_50_1024 100 150 2 "$radio"  >> "$resultados_file"
      ./rangeAnalisis DH_80_1024 100 150 2 "$radio" >> "$resultados_file"

done


