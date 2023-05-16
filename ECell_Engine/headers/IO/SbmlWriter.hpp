/**
 *
 * Adapted from:
 *
 * File: example.cpp (https://sbml.org/software/libsbml/5.18.0/docs/formatted/cpp-api/libsbml-example.html)
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
#include "Maths/MathUtility.hpp"

namespace ECellEngine::IO
{
    class SbmlWriter
    {
    private:
        // The SBML Level and Version of the example SBML models.
        const static unsigned int Level = 3;
        const static unsigned int Version = 2;

        struct ReactionPointersCapsule
        {
            LIBSBML_CPP_NAMESPACE::Reaction* r;
            SpeciesReference* spr;
            KineticLaw* kl;
        };

        /// <summary>
        ///  Defines a parameter object according to the input parameters
        /// of the function. Since a no value is provided in this function,
        /// the parameter's SBML "constant" attribute is set to "false".
        /// </summary>
        /// <param name="_para_ptr">Pointer to the object.</param>
        /// <param name="_paraID">Id.</param>
        /// <param name="_unit">The name of a unit defined in SBML.</param>
        void DefineParameter(
            LIBSBML_CPP_NAMESPACE::Parameter* _para_ptr,
            const std::string& _paraID,
            const std::string& _unit);

        /// <summary>
        /// Defines a parameter object according to the input parameters
        /// of the function. Since a value is provided in this function,
        /// the parameter's SBML "constant" attribute is set to "true".
        /// </summary>
        /// <param name="_para_ptr">Pointer to the object.</param>
        /// <param name="_paraID">Id.</param>
        /// <param name="_unit">The name of a unit defined in SBML.</param>
        /// <param name="_value">Numerical value of the parameter.</param>
        void DefineParameter(
            LIBSBML_CPP_NAMESPACE::Parameter* _para_ptr,
            const std::string& _paraID,
            const std::string& _unit,
            const double _value);

        /// <summary>
        /// Defines an assignment rule to compute the value of a parameter
        /// such that _variableID=_formula.
        /// </summary>
        /// <param name="_ar">Pointer to the object</param>
        /// <param name="_variableID">ID of the parameter we are defining.
        /// It must be identical to a parameter ID in the listOfParameters
        /// of the SBMLDocument being build.</param>
        /// <param name="_formula">The string based mathematical formula.</param>
        void DefineParameterAssignementRule(
            AssignmentRule* _ar,
            const std::string& _variableID,
            const std::string& _formula);

        void DefineReaction(
            ReactionPointersCapsule* _rpc,
            const std::string& _rID,
            const std::vector<std::string>& _reactantIDs,
            const std::vector<int>& _reactantStoichs,
            const std::vector<std::string>& _productIDs,
            const std::vector<int>& _productStoichs,
            const std::string& _kParamID);

        void DefineSpecies(
            LIBSBML_CPP_NAMESPACE::Species* _sp_ptr,
            const std::string& _id,
            const std::string& _name,
            const std::string& _compartmentID,
            const std::string& _unit,
            const double _initialQuantity);

    public:
        /// <summary>
        /// Creates an SBML Document corresponding to the toy model in the paper of
        /// Gibson and Brick (2000,  DOI: 10.1021/jp993732q).
        /// </summary>
        /// <returns></returns>
        SBMLDocument* GibsonAndBruckToyModel();

        /// <summary>
        ///  Writes the given SBMLDocument to the given file.
        /// </summary>
        /// <param name="sbmlDoc">The SBMLDocument object to write.</param>
        /// <param name="filename">The name of the file to write.</param>
        /// <returns>True if it managed to write the document, false otherwise.</returns>
        bool WriteSBML(const SBMLDocument* sbmlDoc, const std::string& filename);
    };
}