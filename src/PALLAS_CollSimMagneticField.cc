//// PALLAS_CollSimMagneticfield.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimMagneticField.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PALLAS_CollSimMagneticField::PALLAS_CollSimMagneticField()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PALLAS_CollSimMagneticField::~PALLAS_CollSimMagneticField()
{
}

G4double fitFunction(G4double *x, G4double *par)
{
  double x0 = par[0];
  double amp_e = par[1];
  double x0_e = par[2];
  double sigma_e = par[3];
  double amp_g = par[4];
  double x0_g = par[5];
  double sigma_g = par[6];

  double stepF = 0;
  double gausF = 0;

  if (x[0] < x0)
  {
    stepF = amp_e * (TMath::Erf((x[0] - x0_e) / sigma_e));
    gausF = 0;
  }

  else
  {
    stepF = 0;
    gausF = amp_g * TMath::Exp(-0.5 * TMath::Power((x[0] - x0_g) / sigma_g, 2));
  }

  double combined = stepF + gausF;

  return combined;
}

G4double symmetrizedFunctionY(double *x, double *par)
{
  double x0 = 0; // Point de symétrie
  double f_x = 0;
  double f_symmetric = 0;

  if (x[0] < x0)
  {
    f_x = 0;
    f_symmetric = fitFunction(new double[1]{2 * x0 - x[0]}, par);
  }

  else
  {
    f_x = fitFunction(x, par);
    f_symmetric = 0;
  }

  return (f_x + f_symmetric);
}

G4double symmetrizedFunctionS(double *x, double *par)
{
  double x0 = 3.4495; // Point de symétrie
  double f_x = 0;
  double f_symmetric = 0;

  if (x[0] < x0)
  {
    f_x = 0;
    f_symmetric = fitFunction(new double[1]{2 * x0 - x[0]}, par);
  }

  else
  {
    f_x = fitFunction(x, par);
    f_symmetric = 0;
  }

  return (f_x + f_symmetric);
}

void PALLAS_CollSimMagneticField::GetFieldValue(const G4double point[4], double *bField) const
{

  double x = point[0];
  double y = point[1];
  double z = point[2];

  if (StatusMapBField == true)
  {
    TF1 *fitFuncY = new TF1("fitFuncY", symmetrizedFunctionY, -0.15, 0.15, 7);
    fitFuncY->FixParameter(0, -1.05579 + 1.15);
    fitFuncY->FixParameter(1, -0.985);
    fitFuncY->FixParameter(2, -1.03649 + 1.15);
    fitFuncY->FixParameter(3, 0.0307999);
    fitFuncY->FixParameter(4, 721.501);
    fitFuncY->FixParameter(5, -1.58778 + 1.15);
    fitFuncY->FixParameter(6, 0.141887);

    TF1 *fitFuncS = new TF1("fitFuncS", symmetrizedFunctionS, 3.0995, 3.7995, 7);
    fitFuncS->FixParameter(0, 0.169992 + 3.4495);
    fitFuncS->FixParameter(1, -0.806796);
    fitFuncS->FixParameter(2, 0.193481 + 3.4495);
    fitFuncS->FixParameter(3, 0.0405178);
    fitFuncS->FixParameter(4, 1.9817);
    fitFuncS->FixParameter(5, 0.0119007 + 3.4495);
    fitFuncS->FixParameter(6, 0.0946281);

    bField[0] = -fitFuncY->Eval(z / 1000) * fitFuncS->Eval(y / 1000) * CLHEP::tesla;
    bField[1] = 0.;
    bField[2] = 0.;
  }

  else
  {
    if (y < 3599 && y > 3270)
    {
      bField[0] = -ConstantBField;
      bField[1] = 0.;
      bField[2] = 0.;
    }
    else
    {
      bField[0] = 0.;
      bField[1] = 0.;
      bField[2] = 0.;
    }
  }

  // G4cout << "\n\n\nBField value = " << ConstantBField << G4endl;
  // G4cout << "y = " << y << G4endl;
  // G4cout << "z = " << z << G4endl;
  // G4cout << "B= " << bField[0] << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PALLAS_CollSimMagneticField::SetField(G4double fieldValue)
{
  ConstantBField = fieldValue;
  // G4cout << "SETFIELD FONCTION : " << ConstantBField << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PALLAS_CollSimMagneticField::SetMapBFieldStatus(G4bool status)
{
  StatusMapBField = status;
  // G4cout << "SETSTATUS MAP BFIELD : " << StatusMapBField << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
