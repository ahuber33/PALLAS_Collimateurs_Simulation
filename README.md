# PALLAS_CollSim for PALLAS_Coll Experiment [huber@lp2ib.in2p3.fr, huberarnaud@gmail.com]

## INSTRUCTIONS TO USE THE SIMULATION
- Download the VMWare [Geant4.11.2.1](https://heberge.lp2ib.in2p3.fr/G4VM/index.html)

```
git clone https://github.com/ahuber33/PALLAS_Collimateurs_Simulation
```

- Go to build Folder and use this command :
```
cmake -DGeant4_DIR=$G4COMP ../
make -j4
```  
then compile it with make

- The executable PALLAS_CollSim will be add to your bin folder

- If you want to have a visualization, launch this command : 
```
./PALLAS_CollSim [name of ROOT file ]
```  
It will generate x particle according to the vis.mac with QT and you will have a ROOT file with the name you gave located in the Resultats folder.

- If you want to have statistics without the visualization, use this command : 
```
./PALLAS_CollSim [name of ROOT file] [number of events generated] [name of macro]
```  
Personnaly, I used the vrml.mac but you can create another one. Just to remember that you need to write the name of your macro when you launch the simulation.

- In your macro, you need to specifiy some informations for the Messenger used :
```
######################################################
########## PART FOR MESSENGER INFORMATIONS ###########
######################################################
/geometry/SetCollimatorThickness 10 mm
/geometry/SetCollimatorInternalRadius 10 mm
/geometry/SetCollimatorExternalRadius 100 mm
/geometry/SetCollimatorSpectrometerDistance 200 mm

/display/SetStatusDisplayCelluleGeometry false
/display/SetStatusDisplayLIFGeometry false
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

- **/geometry/** corresponds to definition of specific volume & distance (here the Collimator) :
    - SetCollimatorThickness for the thickness of the collimator
    - SetCollimatorInternalRadius for the part of the collimator without matter
    - SetCollimatorExternalRadius for the external radius of the collimator
    - SetCollimatorSpectrometerDistance to define the distance between the end of the collimator and the begin of spectrometer
    - **IMPORTANT** You can find a **Geometry.cc** file where all the possible LogicalVolume are created and a **PALLAS_CollSimGeometryConstruction.cc** where these functions are call to construct the geometry.

- **/display/** manages if some part of the geometry are taken into account or no :
    - SetStatusDisplayCelluleGeometry for the "2 cells part"
    - SetStatusDisplayLIFGeometry for the LIF part
    - SetStatusDisplaySection1Geometry for the 2 first Quadrupole with ISOChamber
    - SetStatusDisplaySection2Geometry for Q3, Q4, ASMRemovalChamber, BreadboardRemovalChamber, ChassisRemovalChamber and ISOTubes
    - SetStatusDisplaySection3Geometry for ASMPoutre & YAGStation
    - SetStatusDisplaySection4Geometry for DipoleChamber, Dipole & 2 YAGs
    - SetStatusDisplaySection4DumpGeometry for shieldings, chassis & DiagsChamber
    - /run/reinitializeGeometry is mandatory to take into account the messegner informations

- **/field/** manages the magnetic field inside the spectrometer :
    - SetStatusMapBField allows to the user to choose the application of a B Map (if available) or a constant B value.
    - SetConstantBField defines the value of magnetic field if the SetStatusMapBField is set to false

- **/step/** manages the informations inside the SteppingAction :
    - SetTrackingStatus allows to user to activate/desactivate the tracking of particles. It can be useful for inputs verification.


- In your macro, you have also the possibility to use ParticleGun (for the generation of particle with inputs from text files) or GPS (useful for standard generation)
- If you want to use the ParticleGun, it is **mandatory** to use **/gun** messenger in your macro :
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
- **/gun/** manages the PrimaryGeneratorAction with GunParticle :
    - SetStatusGunParticle defines if the simulation will use GunParticle or GPS as particle generator. If this value is **false**, it is **mandatory** to add or uncomment the /gps informations
    - SetParticleName defines the particle 
    - SetEnergyReference defines the energy reference mandatory to have the energy of each particle according to the phasespace file.
    - SetPhaseSpaceFilename defines the name of the file that will be use to generate phase space of particles
    - SetMacroChargeFilename defines the name of the file that will be use to generate a given number of particle accordint to the macro charge.

- If you want to use GPS, you can use for example this kind of messengers :
```
/gps/number 1
/gps/particle e-
/gps/pos/type Point
/gps/pos/centre -0.152 2500.0 0.08 mm
/gps/energy 250. MeV
/gps/direction 0.0 1.0 0.0
```

- **IMPORTANT** : According to the PALLAS step file, the particle propagation is along the y-axis !!!

- Based on G4EmStandardPhysics_option3.

- DO NOT HESITATE TO REPORT BUGS OR ANY IDEAS THAT WILL IMPROVE THE SIMULATION !!!!


## ACCESS TO SIMULATION INFORMATIONS

- 4 Trees are created in the **BeginOfRunAction** :
    -   **Tree_Input** with the inputs information from particle generation :
        - x : X position red from input file
        - xoffset : Offset needed to be in the center of spectrometer tube according to step file. Currently, this value is fixed at -0.152 mm
        - xp : X momentum red from input file
        - y : Y position red from input file but coming from z information in simulation
        - yoffset : Offset needed in **z-Axis** to be in the center of spectrometer tube according to step file. Currently, this value is fixed at 0.08 mm
        - yp : X momentum red from input file but which correspond to zp in simulation
        - s : Gap between particle & reference particle in **z-Axis** according to input file but which corresponds to the position in **y-Axis** in simulation.
        - soffset : Corresponds to the **Y-Position**  of the spectrometer entrance. Currently, this value is fixed at 3114.5 mm
        - p : Z momentum red from input file but which correspond to yp in simulation
        - delta : Corresponds to the energy difference between a particle and the reference particle
        - energy : From delta information and Energy reference, the energy of a particle
        - Nevent : According to the macrocharge file, it corresponds to the number of particles generated for a given pahsespace configuration.

    - **Tree_Collimator** is the tree with the particles informations from particles/collimator interaction :
        - E_start : Energy of the particle at the entrance of collimator (interaction)
        - E_dep : Energy deposited by particles on the collimator
        - E_dep_e : Part of energy deposited on the collimator by electrons
        - E_dep_g : Part of energy deposited on the collimator by gammas

    - **Tree_FrontCollimator** with the informations of particles passing through the collimator:
        - particleID : ID of the particle 
            - e- =11
            - gamma = 22
            - proton = 2212
            - neutron = 2112
            - for more informations, look at PDG encoding GEANT4
        - parentID : Primary particle or secondary particle ?
        - E_exit : Energy of the particle
        - x_exit : X Position of the particle
        - y exit : Y Position of the particle
        - z_exit : Z Position of the particle
        - px_exit : X Momentum of the particle
        - py_exit : Y Momentum of the aprticle
        - pz_exit : Z Momentum of the particle

    - **Tree_BackCollimator** with the informations of particles passing which go back towards the rear of the collimator:
        - particleID : ID of the particle 
            - e- =11
            - gamma = 22
            - proton = 2212
            - neutron = 2112
            - for more informations, look at PDG encoding GEANT4
        - parentID : Primary particle or secondary particle ?
        - E_exit : Energy of the particle
        - x_exit : X Position of the particle
        - y exit : Y Position of the particle
        - z_exit : Z Position of the particle
        - px_exit : X Momentum of the particle
        - py_exit : Y Momentum of the aprticle
        - pz_exit : Z Momentum of the particle        
        
    - **Tree_YAG** with the informations of interacting particles with YAG :
        - x_exit : X Position of the particle
        - y_exit : Y Position of the particle
        - z_exit : Z Position of the particle
        - energy : Energy of the particle


- Each variables is initialized at the **BeginOfEventAction**
- You can find all the variable functions on the **PALLAS_ColSimEventAction.hh**
- Each Trees is filled (sometimes under conditions) at the **EndOfEventAction**
- If you want to access where the informations are extracted, go to **PALLAS_CollSimSteppingAction.cc**
- A ROOT file with [name of ROOT file] given at the launch is created and wrote at the **EndOfEventAction** 
