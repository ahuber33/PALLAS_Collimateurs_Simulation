#include "PALLAS_CollSimMessenger.hh"

//----------------------------------------------------------------------
// constructor of the messenger: define the commands

PALLAS_CollSimMessenger::PALLAS_CollSimMessenger(PALLAS_CollSimGeometry *det, PALLAS_CollSimPrimaryGeneratorAction *gen)
{
  cout << "Constructor PALLAS_CollSimMessenger" << endl;

  //runac = mgr;
  det_ptr = det;
  gen_ptr = gen;

  // define the commands
  DefineInputCommands();
}

// destructor: delete all allocated command objects
PALLAS_CollSimMessenger::~PALLAS_CollSimMessenger()
{
  cout << "Destructor PALLAS_CollSimMessenger" << endl;

  delete input_dir;
  // delete input_cmd_open;
  // delete input_cmd_open_SRIM;
  // delete input_cmd_B;
  // delete input_cmd_th;
  // delete input_cmd_open_Field;
}

//----------------------------------------------------------------------
// functions defining the commands

// - for input file
void PALLAS_CollSimMessenger::DefineInputCommands()
{
  // commands directory
  input_dir = new G4UIdirectory("/input/");
  input_dir->SetGuidance("Input file commands directory");

  // commands

  // // Open CRADLE++ file event
  // input_cmd_open = new G4UIcmdWithAString("/input/Open_CRADLE", this);
  // input_cmd_open->SetGuidance("Open the input events file.");
  // input_cmd_open->SetParameterName("filename", false);
  // input_cmd_open->AvailableForStates(G4State_PreInit, G4State_Idle);

  // // Open SRIM file event
  // input_cmd_open_SRIM = new G4UIcmdWithAString("/input/Open_SRIM", this);
  // input_cmd_open_SRIM->SetGuidance("Open the input implantation file.");
  // input_cmd_open_SRIM->SetParameterName("filename", false);
  // input_cmd_open_SRIM->AvailableForStates(G4State_PreInit, G4State_Idle);

  // // Open Magnetic Field file
  // input_cmd_open_Field = new G4UIcmdWithAString("/input/Open_B", this);
  // input_cmd_open_Field->SetGuidance("Open the input magnetic field file.");
  // input_cmd_open_Field->SetParameterName("filename", false);
  // input_cmd_open_Field->AvailableForStates(G4State_PreInit, G4State_Idle);

  // // Set Output root filename
  // input_cmd_filename = new G4UIcmdWithAString("/output", this);
  // input_cmd_filename->SetGuidance("Set root filename");
  // input_cmd_filename->SetParameterName("filename", false);
  // input_cmd_filename->AvailableForStates(G4State_PreInit, G4State_Idle);

  // // Set B field Value
  // input_cmd_B = new G4UIcmdWithAString("/Magnetic_Field", this);
  // input_cmd_B->SetGuidance("Set magnetic field value");
  // input_cmd_B->SetParameterName("B", false);
  // input_cmd_B->AvailableForStates(G4State_PreInit, G4State_Idle);

  // // Set Plastic Sintillator threshoold for histograms
  // input_cmd_th = new G4UIcmdWithAString("/Threshoold_PlasticScintillator", this);
  // input_cmd_th->SetGuidance("Set Threshoold PlasticScintillator");
  // input_cmd_th->SetParameterName("threshoold", false);
  // input_cmd_th->AvailableForStates(G4State_PreInit, G4State_Idle);

  // // Set Ion Beam Size
  // input_cmd_beam_size = new G4UIcmdWithAString("/Beam_Size", this);
  // input_cmd_beam_size->SetGuidance("Set Beam Size");
  // input_cmd_beam_size->SetParameterName("beam_size", false);
  // input_cmd_beam_size->AvailableForStates(G4State_PreInit, G4State_Idle);

  // // Set Ion Beam position
  // input_cmd_beam_position = new G4UIcmdWithAString("/Beam_Position", this);
  // input_cmd_beam_position->SetGuidance("Set Beam Position");
  // input_cmd_beam_position->SetParameterName("beam_position", false);
  // input_cmd_beam_position->AvailableForStates(G4State_PreInit, G4State_Idle);

  // // Set Ion Beam position
  // input_cmd_dl_thickness = new G4UIcmdWithAString("/SiDeadLayer_Thickness", this);
  // input_cmd_dl_thickness->SetGuidance("Set Silicon Dead Layer Thickness");
  // input_cmd_dl_thickness->SetParameterName("dl_th", false);
  // input_cmd_dl_thickness->AvailableForStates(G4State_PreInit, G4State_Idle);

  // // Set Catcher position z
  // input_cmd_catcher_z = new G4UIcmdWithAString("/Catcher_Position_z", this);
  // input_cmd_catcher_z->SetGuidance("Set Catcher position");
  // input_cmd_catcher_z->SetParameterName("catcher_z", false);
  // input_cmd_catcher_z->AvailableForStates(G4State_PreInit, G4State_Idle);

  // // Set Catcher position theta
  // input_cmd_catcher_theta = new G4UIcmdWithAString("/Catcher_Position", this);
  // input_cmd_catcher_theta->SetGuidance("Set Catcher position");
  // input_cmd_catcher_theta->SetParameterName("catcher_z", false);
  // input_cmd_catcher_theta->AvailableForStates(G4State_PreInit, G4State_Idle);

  // Set Catcher Thickness
  input_cmd_collimator_internal_radius = new G4UIcmdWithAString("/Collimator_InternalRadius", this);
  input_cmd_collimator_internal_radius->SetGuidance("Set Collimator internal radius");
  input_cmd_collimator_internal_radius->SetParameterName("collimator_internal_radius", false);
  input_cmd_collimator_internal_radius->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//----------------------------------------------------------------------
// function processing the commands
void PALLAS_CollSimMessenger::SetNewValue(G4UIcommand *cmd, G4String args)
{

  // input file commands

  // if (cmd == input_cmd_open)
  // {
  //   runac->OpenInput(args);
  // }
  // if (cmd == input_cmd_open_SRIM)
  // {
  //   runac->OpenInputSRIM(args);
  // }
  // if (cmd == input_cmd_open_Field)
  // {
  //   det_ptr->OpenInputB(args);
  // }
  // if (cmd == input_cmd_B)
  // {
  //   G4double value;
  //   G4String unit;
  //   std::istringstream iss(args);
  //   iss >> value >> unit;
  //   det_ptr->SetBFieldValue(value * G4UnitDefinition::GetValueOf(unit));
  // }
  // if (cmd == input_cmd_th)
  // {
  //   G4double value;
  //   G4String unit;
  //   std::istringstream iss(args);
  //   iss >> value >> unit;
  //   runac->SetThreshoold(value * G4UnitDefinition::GetValueOf(unit));
  // }
  // if (cmd == input_cmd_beam_size)
  // {
  //   G4double value, value1;
  //   G4String unit, unit1;
  //   std::istringstream iss(args);
  //   iss >> value >> unit >> value1 >> unit1;
  //   gen_ptr->SetBeamSize(value * G4UnitDefinition::GetValueOf(unit), value1 * G4UnitDefinition::GetValueOf(unit1));
  // }
  // if (cmd == input_cmd_beam_position)
  // {
  //   G4double value, value1;
  //   G4String unit, unit1;
  //   std::istringstream iss(args);
  //   iss >> value >> unit >> value1 >> unit1;
  //   gen_ptr->SetBeamPosition(value * G4UnitDefinition::GetValueOf(unit), value1 * G4UnitDefinition::GetValueOf(unit1));
  // }
  // if (cmd == input_cmd_catcher_z)
  // {
  //   G4double value;
  //   G4String unit;
  //   std::istringstream iss(args);
  //   iss >> value >> unit;
    
  //   det_ptr->SetCatcherPosition_z(value * G4UnitDefinition::GetValueOf(unit));
  //   gen_ptr->SetCatcherPosition_z(value * G4UnitDefinition::GetValueOf(unit));
  // }
  // if (cmd == input_cmd_catcher_theta)
  // {
  //   G4double value1;
  //   G4String value, unit1;
  //   std::istringstream iss(args);
  //   iss >> value >> value1 >> unit1;
  //   det_ptr->SetCatcherPosition_theta(value, value1 * G4UnitDefinition::GetValueOf(unit1));
  // }
  // if (cmd == input_cmd_filename)
  // {
  //   runac->SetOutputFilename(args);
  // }
  // if (cmd == input_cmd_dl_thickness)
  // {
  //   G4double value;
  //   G4String unit;
  //   std::istringstream iss(args);
  //   iss >> value >> unit;
  //   det_ptr->SetSiDeadLayer_Thickness(value * G4UnitDefinition::GetValueOf(unit));
  // }
  if (cmd == input_cmd_collimator_internal_radius)
  {
     G4cout << "\n\n MESSENGER ICI \n\n" << G4endl;
    G4double value;
    G4String unit;
    std::istringstream iss(args);
    iss >> value >> unit;
    det_ptr->SetCollimatorInternalRadius(value * G4UnitDefinition::GetValueOf(unit));
  }
}
