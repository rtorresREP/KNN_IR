#!/bin/bash



echo "Ejecutando exp-ir.sh..."  #65536 dataset fdo
./exp-ir.sh

echo "Ejecutando exp-ir-gauss.sh..." 
./exp-ir-gauss.sh

echo "Ejecutando exp-ir-reales.sh..."
./exp-ir-reales.sh

echo "Ejecutando exp mat grande 262144"
./experimentos-random1.sh

echo "Ejecutando exp-ir-reales-tiger.sh..."
./exp-ir-reales-tiger.sh



#ahora PQ
#echo "Ejecutando exp-pq.sh..."
#./exp-pq.sh

#echo "Ejecutando exp-pq-gauss.sh..."
#./exp-pq-gauss.sh

#echo "Ejecutando exp-reales.sh..."
#./exp-pq-reales.sh

#echo "Ejecutando dataset 256144"
#./experimentos-random1-pq.sh



echo "Scripts ejecutados."

