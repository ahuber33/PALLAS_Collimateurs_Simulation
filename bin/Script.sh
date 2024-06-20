#!/bin/bash

# Ensemble des valeurs pour variable1 et variable2
Material=(G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_BRASS G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_W G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb G4_Pb)
Thickness=(50 50 50 75 75 75 100 100 100 15 15 15 23 23 23 30 30 30 20 20 20 35 35 35 50 50 50)
Ecart=(0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6 0.2 0.4 0.6)
count=0

# Chemin vers le fichier macro
macro_file="vrml_base.mac"



# Boucles pour les deux ensembles de valeurs
for value1 in "${Thickness[@]}"; do

    while [[ $(pgrep -x PALLAS_CollSim | wc -l) -gt 6 ]]
    do
	sleep 1
    done

    value2=${Material[count]}
    value3=${Ecart[count]}
    count=$((count+1))
    cp vrml_base.mac base_tmp_$count.mac

    # Remplacer les valeurs dans le fichier macro
    sed -e "s/%Thickness/$value1/g" base_tmp_$count.mac > base_tmp2_$count.mac
    sed -e "s/%Material/$value2/g" base_tmp2_$count.mac > base_tmp3_$count.mac
    sed -e "s/%Ecart/$value3/g" base_tmp3_$count.mac > base_$count.mac
    rm base_tmp_$count.mac
    rm base_tmp2_$count.mac
    rm base_tmp3_$count.mac
    
    ./PALLAS_CollSim VerticalConfiguration_${value2}_epaisseur_${value1}_ecartement_${value3} 1 base_$count.mac &

    sleep 5
  done
done






# Boucles pour les deux ensembles de valeurs
#for value1 in "${thickness[@]}"; do
#    for value2 in "${distance[@]}"; do

#    while [[ $(pgrep -x PALLAS_CollSim | wc -l) -gt 6 ]]
#    do
#	sleep 1
#    done

#    count=$((count+1))
#    cp vrml_base.mac base_tmp_$count.mac

    # Remplacer les valeurs dans le fichier macro
#    sed -e "s/%thickness/$value1/g" base_tmp_$count.mac > base_tmp2_$count.mac
#    sed -e "s/%distance/$value2/g" base_tmp2_$count.mac > base_$count.mac
#    rm base_tmp_$count.mac
#    rm base_tmp2_$count.mac
    
#    ./PALLAS_CollSim HorizontalConfiguration_epaisseur_${value1}_ecartement_${value2} 110000 base_$count.mac &

#    sleep 1
#  done
#done


