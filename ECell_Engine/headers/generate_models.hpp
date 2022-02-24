/**
 *
 * Adapted from:
 *
 * File:        example.cpp (https://sbml.org/software/libsbml/5.18.0/docs/formatted/cpp-api/libsbml-example.html)
 * Description: Create an example model.
 * @author  Akiya Jouraku
 *
 * <!--------------------------------------------------------------------------
 * This sample program is distributed under a different license than the rest
 * of libSBML.  This program uses the open-source MIT license, as follows:
 *
 * Copyright (c) 2013-2018 by the California Institute of Technology
 * (California, USA), the European Bioinformatics Institute (EMBL-EBI, UK)
 * and the University of Heidelberg (Germany), with support from the National
 * Institutes of Health (USA) under grant R01GM070923.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Neither the name of the California Institute of Technology (Caltech), nor
 * of the European Bioinformatics Institute (EMBL-EBI), nor of the University
 * of Heidelberg, nor the names of any contributors, may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * -------------------------------------------------------------------------->
 */

#include <sbml/SBMLTypes.h>

// These variables are used in writeExampleSBML when writing an SBML
// document.  They are handed to libSBML functions in order to include
// the program information into comments within the SBML file.
const static std::string ProgramName = "createExampleModels";
const static std::string ProgramVersion = "1.0.0";

// The SBML Level and Version of the example SBML models.
const static unsigned int Level = 3;
const static unsigned int Version = 2;

// Function for creating the Example SBML documents.
SBMLDocument* createExampleEnzymaticReaction_();

Parameter* defineParameter(
    Parameter* _para_ptr,
    const std::string& _paraID,
    const std::string _unit,
    const double _value);

Species* defineSpecies(
	Species* _sp_ptr,
	const std::string& _id,
	const std::string& _name,
	const std::string& _compartmentID,
	const std::string& _unit,
	const double _initialQuantity);

Reaction* defineReaction(
    Reaction* _r_ptr,
    SpeciesReference* _spr_ptr,
    KineticLaw* _kl_ptr,
    const std::string& _rID,
    const std::vector<std::string>& _reactantIDs,
    const std::vector<std::string>& _productIDs,
    const std::string& _kParamID);

/// <summary>
/// Performs validation checks and explain what went wrong if necessary.
/// This function is based on validateSBML.cpp implemented by
/// Sarah Keating, Ben Bornstein, and Michael Hucka.
/// </summary>
/// <param name="sbmlDoc">The SBMLDocument object to check.</param>
/// <returns>True is no problems were found, false otherwise.</returns>
bool validateExampleSBML_(SBMLDocument* sbmlDoc);

/// <summary>
///  Writes the given SBMLDocument to the given file.
/// </summary>
/// <param name="sbmlDoc">The SBMLDocument object to write.</param>
/// <param name="filename">The name of the file to write.</param>
/// <returns>True if it managed to write the document, false otherwise.</returns>
bool writeExampleSBML_(const SBMLDocument* sbmlDoc, const std::string& filename);