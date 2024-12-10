# PALLAS_CollSim for PALLAS_Coll Experiment [huber@lp2ib.in2p3.fr, huberarnaud@gmail.com]

## INSTRUCTIONS TO USE THE SIMULATION
- Download the VMWare [Geant4.11.2.1](https://heberge.lp2ib.in2p3.fr/G4VM/index.html)

```
git clone https://github.com/ahuber33/PALLAS_Collimateurs_Simulation
```

- Go to build Folder and use this command :
```
 cmake -DGeant4_DIR=$G4COMP ../ -DOnnxRuntime_LIBRARY=../onnxruntime-linux-x64-1.17.1/lib/libonnxruntime.so.1.17.1
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
./PALLAS_CollSim [name of ROOT file] [number of events generated] [name of macro] [FileReader ON/OFF] [MultiThreading ON/OFF] [number of threads]

```  
For example, if you want to use the "PALLAS_phasespace_qm(1411)_preCollimateur.txt" file, you need to activate the FileReader [ON]. In this file, there is 106435 lines so 106435 events to generated with some macroparticles associated. According to the number of threads used if MT is ON, the simulation will create a ROOT file for each thread and at the end of the simulation. All ROOT files will be merged together with a name correspoding to the name given in [name of ROOT file]. The temporary ROOT files will be removed after the merge.

Note that it's not necessary to indicate a [number of threads] if the condition on MT is OFF. In opposite, you need to put a value if MT is ON.

Concerning the macro, personnaly I used the vrml.mac but you can create another one. Just to remember that you need to write the name of your macro when you launch the simulation.

- In your macro, you need to specifiy some informations for the Messenger used :
```
/control/alias VerticalCollimatorThickness 20
/control/alias HorizontalCollimatorThickness 60
/control/alias CollimatorSpectrometerDistance 50
/control/alias CollimatorVHDistance 10
/control/alias YParticleGenerationOffset 140 #Sum of precedent values

######################################################
########## PART FOR MESSENGER INFORMATIONS ###########
######################################################
/geometry/SetStatusRoundCollimator false
/geometry/SetCollimatorThickness 150 mm
/geometry/SetCollimatorInternalRadius 10 mm
/geometry/SetCollimatorExternalRadius 100 mm
/geometry/SetCollimatorDistanceBetweenPlates 0 mm

/geometry/SetVerticalCollimatorMaterial G4_W
/geometry/SetHorizontalCollimatorMaterial G4_Pb

/geometry/SetVerticalCollimatorThickness {VerticalCollimatorThickness} mm
/geometry/SetHorizontalCollimatorThickness {HorizontalCollimatorThickness} mm
/geometry/SetCollimatorSpectrometerDistance {CollimatorSpectrometerDistance} mm
/geometry/SetCollimatorVHDistance {CollimatorVHDistance} mm
/gun/SetYParticleGenerationOffset {YParticleGenerationOffset} mm
/geometry/SetOpenVerticalCollimator 0 mm
/geometry/SetOpenHorizontalCollimator 0 mm
/geometry/SetCollimatorLength 100 mm



/display/SetStatusDisplayCelluleGeometry false
/display/SetStatusDisplayLIFGeometry false
/display/SetStatusDisplaySection1Geometry false
/display/SetStatusDisplaySection2Geometry false
/display/SetStatusDisplaySection3Geometry false
/display/SetStatusDisplaySection4Geometry true
/display/SetStatusDisplaySection4DumpGeometry false
/run/reinitializeGeometry


/field/SetStatusMapBField false
/field/SetConstantDipoleBField 0. tesla #0.4 tesla
/field/SetQ1Gradient 0.0 #tesla/m
/field/SetQ2Gradient 0.0
/field/SetQ3Gradient 0.0
/field/SetQ4Gradient 0.0

/step/SetTrackingStatus true

```
- **alias** defines some values with YParticleGenerationOffset which depends on the 4 precedent values. Necessayre to be used in macro Script bash

- **/geometry/** corresponds to definition of specific volume & distance (here the Collimator) :
    - SetStatusRoundCollimator for the status of if we use Round collimator or not (USED FOR TESTS)
    - SetCollimatorThickness for the thickness of the collimator
    - SetCollimatorInternalRadius for the part of the collimator without matter
    - SetCollimatorExternalRadius for the external radius of the collimator
    - SetVerticalCollimatorMaterial to define the material of the vertical collimator
    - SetHorizontalCollimatorMaterial to define the material of the horizontal collimator
    - SetVerticalCollimatorThickness to define the thickness of the vertical collimator from the alias defined before
    - SetHorizontalCollimatorThickness to define the thickness of the horizontal collimator from the alias defined before
    - SetCollimatorSpectrometerDistance to define the distance between the end of the collimator and the begin of spectrometer from the alias defined before
    - SetCollimatorVHDistance define the distance between the end of the vertical collimator and the begin of the horizontal collimator from the alias defined before
    - SetYParticleGenerationOffset define the Offset parameter for the aprticle generation according to the different values of the 2 collimators. => DEFINED WITH GUN MESSENGER
    - SetOpenVerticalCollimator define the aperture of the vertical collimator
    - SetOpenHorizontalCollimator define the aperture of the horizontal collimator
    - SetCollimatorLength define the other dimensions of the collimator
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
    - SetConstantDipoleBField defines the value of magnetic field's Dipole if the SetStatusMapBField is set to false
    - SetQ1Gradient defines the gradient value of Q1
    - SetQ2Gradient defines the gradient value of Q2
    - SetQ3Gradient defines the gradient value of Q3
    - SetQ4Gradient defines the gradient value of Q4

- **/step/** manages the informations inside the SteppingAction :
    - SetTrackingStatus allows to user to activate/desactivate the tracking of particles. It can be useful for inputs verification.


- In your macro, you have also the possibility to use ParticleGun (for the generation of particle with inputs from text files) or GPS (useful for standard generation)
- If you want to use the ParticleGun, it is **mandatory** to use **/gun** messenger in your macro :
```
######################################################
##### PART TO SIMULATE PARTICLE WITH PARTICLEGUN #####
######################################################
/gun/SetStatusGunParticle true
/gun/SetParticleName e-
/gun/SetEnergyReference 247 MeV
/gun/SetStatusONNX true
```
- **/gun/** manages the PrimaryGeneratorAction with GunParticle :
    - SetStatusGunParticle defines if the simulation will use GunParticle or GPS as particle generator. If this value is **false**, it is **mandatory** to add or uncomment the /gps informations
    - SetParticleName defines the particle 
    - SetEnergyReference defines the energy reference mandatory to have the energy of each particle according to the phasespace file.
    - SetStatusONNX defines if the simulation will use this model to generate particles or not. If not, you will need to activate the FileReader parameter if you want to generate a bunch of particles similar at what happend in PALLAS configuration. If not, you will have a "normal" generation of particles according to the part wrote in PALLAS_CollSimPrimaryGeneratorAction.cc
    
- If you want to use the ONNX model, you need to specify the inputs parameter needed by the ML model :   
```
/laser/SetOffsetLaserFocus 558 # um
/laser/SetNormVecPotential 1.43
/laser/SetFracDopTargetChamber 0.0188 # %(/100)
/laser/SetPressure 58.6 #mbar
``` 
For more informations about theses parameters and the model, go check this article : https://link.aps.org/doi/10.1103/PhysRevAccelBeams.26.091302

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

- 6 Trees can be created in the **BeginOfRunAction** :
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

    - **Tree_HorizontalCollGlobal** with the informations of principal processus occuring in Horizontal collimator:
        - photonNculear
        - Radioactivation
        - Brem (commented to avoid having excessively large files)
        - hadElastic
        - neutronInelastic
        - nCapture
        - nFission
        - conv
        - phot
        - compt
        - annihil
        - Other

    - **Tree_VerticalCollGlobal** with the informations of principal processus occuring in Vertical collimator:
        - photonNculear
        - Radioactivation
        - Brem (commented to avoid having excessively large files)
        - hadElastic
        - neutronInelastic
        - nCapture
        - nFission
        - conv
        - phot
        - compt
        - annihil
        - Other


    - **Tree_HorizontalColl** with the informations concerning the energy deposited in Horizontal collimator:
        - particleID : ID of the particle 
            - e- =11
            - e+ = -11
            - gamma = 22
            - proton = 2212
            - neutron = 2112
            - for more informations, look at PDG encoding GEANT4
        - energy : Kinetic energy of the particle before the interaction
        - deposited_energy : Sum of all energy deposited 


    - **Tree_VerticalColl** with the informations concerning the energy deposited in Vertical collimator:
        - particleID : ID of the particle 
            - e- =11
            - e+ = -11
            - gamma = 22
            - proton = 2212
            - neutron = 2112
            - for more informations, look at PDG encoding GEANT4
        - energy : Kinetic energy of the particle before the interaction
        - deposited_energy : Sum of all energy deposited 

        
    - **Tree_BSYAG** with the informations of interacting particles with YAG when the magnet is OFF:
        - x_exit : X Position of the particle
        - y_exit : Y Position of the particle
        - z_exit : Z Position of the particle
        - parentID : ParentID of the particle
        - energy : Energy of the particle
        
    - **Tree_BSPECYAG** with the informations of interacting particles with YAG when the magnet is ON:
        - x_exit : X Position of the particle
        - y_exit : Y Position of the particle
        - z_exit : Z Position of the particle
        - parentID : ParentID of the particle
        - energy : Energy of the particle

- Each variables is initialized at the **BeginOfEventAction**
- You can find all the variable functions on the **PALLAS_ColSimEventAction.hh**
- Each Trees is filled (sometimes under conditions) at the **EndOfEventAction**
- If you want to access where the informations are extracted, go to **PALLAS_CollSimSteppingAction.cc**
- A ROOT file with [name of ROOT file] given at the launch is created and wrote at the **EndOfEventAction** 



## DRAW SIMULATION RESULTS

- After the simulation, you can obtain some graphic visualization of the results according to the Plot_2VD.cc file (Magnet OFF) or Plot_HV_devie.cc (Magnet ON).

- To run the program, run this command :
```
root '[name file].cc("[name of root file]")'
```

