# PALLAS_CollSim for PALLAS_Coll Experiment [huber@cenbg.in2p3.fr, huberarnaud@gmail.com]

## INSTRUCTIONS TO USE THE SIMULATION
- Download the VMWare [Geant4.11.0.2](https://heberge.lp2ib.in2p3.fr/G4VM/index.html)

- Copy all the files from PALLAS_Coll_Simulation folder [ici](https://github.com/ahuber33/PALLAS_Coll_Simulation)

- Create a build folder in the PALLAS_Coll_Simulation folder and go inside.

- Use this command : 
```
cmake -DGeant4_DIR=$G4COMP ../
make -j4
```

- The executable PALLAS_CollSim will be add to your bin folder

- If you want to have a visualization, launch this command : 
```
./PALLAS_CollSim [name of ROOT file ]
```  
It will generate 1 particle according to the vis.mac with QT and you will have a ROOT file with the name you gave in response located in the Resultats folder.

- If you want to have statistics without the visualization, use this command : 
```
./PALLAS_CollSim [name of ROOT file] [number of events generated] [name of macro]
```  
Personnaly, I used the vrml.mac but you can create another one. Just to remember that you need to write the name of your macro when you launch the simulation.

- An PALLAS_CollSim.cfg file is located in bin directory. All the dimensions necessary are in this file to avoid recompilation when you want to change some parameters. If you add some other dimensions, don't forget to add the variables in Geometry.cc.

- Based on G4EmStandardPhysics_option3 with additional Optical part.

- DO NOT HESITATE TO REPORT BUGS OR ANY IDEAS THAT WILL IMPROVE THE SIMULATION !!!!
  
  

## Commit #1 le 04/04/2024 [PALLAS_CollSim.0.0.0]
- Simulation issue de la version TP_Simulation.0.9.0 -> Commit initial avec changement des noms des fichiers et des variables
- Nettoyage complet de toutes les fonctions/variables non nécessaires à cette simulation
- Base de travail pour la simulation des collimateurs du projet PALLAS situés avant le spectromètre

