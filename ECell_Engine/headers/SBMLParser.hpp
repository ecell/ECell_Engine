#include <iostream>
#include <sbml/SBMLTypes.h>

class SBMLParser
{
public:
	/// <summary>
	/// Performs validation checks and explain what went wrong if necessary.
	/// </summary>
	/// <param name="sbmlDoc">The SBMLDocument object to check.</param>
	/// <returns>True is no problems were found, false otherwise.</returns>
	bool ValidateSBML(SBMLDocument* _document);

	SBMLDocument* OpenSBMLFile(const char* _filePath);

	void PrettyPrintSBMLDocument(SBMLDocument* _document); 
};