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

## Commit #5 le 02/05/2024 [PALLAS_CollSim.0.2.1]
- Commit provisoire de sauvergarde pendant la mise en place des Messenger

## Commit #6 le 07/05/2024 [PALLAS_CollSim.0.2.2]
- Implémentation des éléments de géométrie de la ligne PALLAS via plusieurs fichiers GDML. Ces fichiers ont été définis à partir des fichiers géométrie .step, maillé et tesselated via FREECAD. Pour éviter d'avoir des fichiers trop gros et afin d'avoir juste une vision globale de la ligne, les fichiers GDML ont été simplifiés au maximum.
- TO DO : Terminer la mise en place des Messenger
- Nettoyer Le Materials !!!

## Commit #7 le 30/05/2024 [PALLAS_CollSim.0.3.0]
- Changement de l'architecture du code désormais basé sur l'utilisation de la classe G4Messenger plutôt que sur l'utilisation d'un fichier deconfig lu au début de la simulation
- Chacun des Messengers (PrimaryGeneractorAction et GeometryConstruction) est défini à l'initialization de chacune de ces classes et non pas via l'utilisation d'une nouvelle classe PALLAS_CollSimMessenger
- Le nombre d'évènements désirés défini dans la ligne de commande du lancement de la simulation est maintenant récupéré afin de pouvoir être utilisé dans la partie PrimaryGeneractorAction
- Le fichier Geometry ne contient plus de dépendance de elcture au fichier de config lors de son initialisation
- Effacement du fichier de config qui n'est plus utilisé
- Suite à l'importation de la classe G4GenericMessenger, les informations permettant de faire varier certains apramètres sont désormais indiqués directement dans la macro via les fonctions suivantes :
```
######################################################
########## PART FOR MESSENGER INFORMATIONS ###########
######################################################
/geometry/SetCollimatorThickness 10 mm
/geometry/SetCollimatorInternalRadius 10 mm
/geometry/SetCollimatorExternalRadius 100 mm
/geometry/SetCollimatorSpectrometerDistance 200 mm

/display/SetStatusDisplayCelluleGeometry false
/display//SetStatusDisplayLIFGeometry false
/display/SetStatusDisplaySection1Geometry false
/display/SetStatusDisplaySection2Geometry false
/display/SetStatusDisplaySection3Geometry false
/display/SetStatusDisplaySection4Geometry true
/display/SetStatusDisplaySection4DumpGeometry false
/run/reinitializeGeometry

/field/SetStatusMapBField false
/field/SetConstantBField 0.4 tesla

/step/SetTrackingStatus false
```  
- La partie /geometry permet de modifier des éléments de la géométrie
- La partie /display permet de gérer la prise en compte ou non de certains éléments de la géométrie
- La partie /field permet de gérer les informations liées au champ magnétique
- La partie /step permet d'activer ou non le suivi des particules en fonction de ce qui est désiré dans la simulation (vérification des inputs ou non)


- Une refonte du PrimaryGeneratorAction a également été faite afin de pouvoir générer des particules à partir d'un fichier de config (espace des phases 6d)
- Pour cela, la génération doit se faire via le GunParticle et non plus avec GPS 
- Ajout des fonctions permettant de lire le fichier d'espace de phase et de l'utiliser pour la génération des particules
- Chacunes des macroparticules générés selon ce fichier correspond à une certaine macrocharge qu'il convient de prendre en compte afin de pouvoir normaliser l'impact desdifférentes macroparticules
- Pour cela, une fonction a été ajoutée afin de lire le fichier de macrocharge, de le transformer en nombre équivalents d'électrons à générer.
- On peut ainsi obtenir pour chaque macroparticule généréé, son nombre de particules à générer.
- En résumé, chaque ligne du fichier correspond à une macroparticule auquel est associé une macrocharge. Chaque "Event" va alors avoir un certain nombre de particules générées en accorrd avec la macrocharge.
- Dans ce cas, le nombre d'évènements générés ne correspond alors plus au nombre total de particulees générées.
- Le nombre total de particules générées est calculé lors de la lecture des fichiers.
- Des fonctions supplémentaires utilisant l'ensemble de ces informations permet alors d'afficher sur le terminal l'avancement de la simulation ainsi que le temps estimé avant la fin de celle-ci.
- Le Messenger de la partie gun est donc celui-ci :
```
######################################################
##### PART TO SIMULATE PARTICLE WITH PARTICLEGUN #####
######################################################
/gun/SetStatusGunParticle false
/gun/SetParticleName geantino
/gun/SetEnergyReference 247 MeV
/gun/SetPhaseSpaceFilename PALLAS_phasespace_1411_preCollimateur.txt
/gun/SetMacroChargeFilename PALLAS_qm1411_preCollimateur.txt
```
- SetStatusGunParticle permet de définir si on utilise GunParticle (avec la lecture des ficheirs en input) ou si on utiliser GPS
- SetParticleName permet de définir le type des particules générées
- SetEnergyReference permet de définir l'énergie de référence du faisceau d'électrons
- SetPhaseSpaceFilename permet de définir le fichier utilisé pour l'espace de phase des particules
- SetMacroChargeFilename permet de définir le fichier utilisé pour les macrocharges


- Importation des fichiers GDML correspondant aux YAG en sortie de collimateur (ligne non déviée et déviée)

- Ajout d'un nouveau Tree "Input" permettant de visulaiser et de vérifier les inputs fournies par les fichiers de génération des particules.
- Création des fonctions permettant de remplir et d'accéder aux informations ainsi que celles permettant de remplir l'arbre.
- Ajout d'un nouveau Tree "YAG" prévu pour recueillir les informations issues de ce dernier.
- Création des fonctions permettant de remplir et d'accéder aux informations ainsi que celles permettant de remplir l'arbre.

- Mise à jour du README avec création de ce fichier History pour suivre l'évolution du code.
- Le fichier README gardant la vocation d'être la documentation du code.