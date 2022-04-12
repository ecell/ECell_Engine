#include <iostream>
#include <sbml/SBMLTypes.h>
using namespace std;
LIBSBML_CPP_NAMESPACE_USE
//
// Functions for creating the Example SBML documents.
//
SBMLDocument* createExampleEnzymaticReaction();            /* 7.1 */
SBMLDocument* createExampleInvolvingUnits();               /* 7.2 */
SBMLDocument* createExampleInvolvingFunctionDefinitions(); /* 7.8 */
//
// Helper functions for validating and writing the SBML documents created.
//
bool validateExampleSBML(SBMLDocument* sbmlDoc);
bool writeExampleSBML(const SBMLDocument* sbmlDoc, const string& filename);
//
// These variables are used in writeExampleSBML when writing an SBML
// document.  They are handed to libSBML functions in order to include
// the program information into comments within the SBML file.
//
const static string ProgramName = "createExampleModels";
const static string ProgramVersion = "1.0.0";
//
// The SBML Level and Version of the example SBML models.
//
const static unsigned int Level = 3;
const static unsigned int Version = 2;