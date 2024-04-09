# PALLAS_CollSim for PALLAS_Coll Experiment [huber@cenbg.in2p3.fr, huberarnaud@gmail.com]

## INSTRUCTIONS TO USE THE SIMULATION
- Download the VMWare [Geant4.11.2.1](https://heberge.lp2ib.in2p3.fr/G4VM/index.html)

- Copy all the files from PALLAS_Coll_Simulation folder ici

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

- Based on G4EmStandardPhysics_option3.

- DO NOT HESITATE TO REPORT BUGS OR ANY IDEAS THAT WILL IMPROVE THE SIMULATION !!!!
  
  

## Commit #1 le 04/04/2024 [PALLAS_CollSim.0.0.0]
- Simulation issue de la version TP_Simulation.0.9.0 -> Commit initial avec changement des noms des fichiers et des variables
- Nettoyage complet de toutes les fonctions/variables non nécessaires à cette simulation
- Base de travail pour la simulation des collimateurs du projet PALLAS situés avant le spectromètre

## Commit #2 le 04/04/2024 [PALLAS_CollSim.0.1.0]
- Suppression des fichiers Materials.cc & hh
- Définition des matériaux se fait directement dans le fichier Geometry.cc avec l'aide de la base NIST
- Continuité du nettoyage du code
- Ajout d'un fichier .gitignore

## Commit #3 le 05/04/2024 [PALLAS_CollSim.0.1.1]
- Continuité nettoyage code
- Ajout de fonctions dans le fichier PALLAS_CollSimGeometry pour définir les couleurs ainsi que le World & Holder afin de gagner en visibilité et compression du code.
- Définition de volumes basiques d'intérêts (FrontOutput, BackOutput entourant le collimateur) permettant une étude préliminaire
- Creation de 3 Tree distincts pour pouvoir déterminer ce qui se passe dans le collimateur et ce qui en ressort en arrière et en avant
- Code en cours d'écriture pour accès aux informations dans le SteppingAction et fonctions en cours de développement

## Commit #4 le 09/04/2024 [PALLAS_CollSim.0.2.0]
- Correction bugs divers
- Ensemble des fonctions définis dans SteppingAction pour récupérer l'ensemble des informations nécessaires (pour le moment) avec prévisions également pour la suite.
- La création du string "GetProcessName" entraine un segmentation fault si le creatorprocess n'est pas crée. Il est donc indispensable de vérifier que celui-ci existe bien avant de faire appel à cette fonction (possible également en s'affranchissant juste du ParentID=0 qui n'a pas de processus créateur)
- 3 fonctions principales (UpdateCollimatorInformations, UpdateCollimatorFrontInformations & UpdateBackCollimatorInformations) permettant de remplir les 3 trees de stockage de données correspondant aux informations physique de ce qu'il se passe dans le collimateur, de ce qui ressort en face avant et en face arrière.
- Simulation simplifiée avec passage du cut physique à 1mm contre 1µm précédemment.