/**
 *
 * Adapted from:
 *
 * File: example.cpp (https://sbml.org/software/libsbml/5.18.0/docs/formatted/cpp-api/libsbml-example.html)
 * Function : validateExampleSBML(SBMLDocument* sbmlDoc) -> renamed ValidateSBML(SBMLDocument* sbmlDoc)
 * Description: Validates and pretty-prints the warnings or errors if any.
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

#include "SbmlParser.hpp"

bool SbmlParser::ValidateSBML(SBMLDocument* _sbmlDoc)
{
    if (!_sbmlDoc || _sbmlDoc->getModel() == NULL)
    {
        std::cerr << "ValidateSBML: given a null SBML Document." << std::endl;
        return false;
    }

    std::string consistencyMessages;
    std::string validationMessages;
    bool noProblems = true;
    unsigned int numCheckFailures = 0;
    unsigned int numConsistencyErrors = 0;
    unsigned int numConsistencyWarnings = 0;
    unsigned int numValidationErrors = 0;
    unsigned int numValidationWarnings = 0;
    // LibSBML 3.3 is lenient when generating models from scratch using the
    // API for creating objects.  Once the whole model is done and before it
    // gets written out, it's important to check that the whole model is in
    // fact complete, consistent and valid.
    numCheckFailures = _sbmlDoc->checkInternalConsistency();
    if (numCheckFailures > 0)
    {
        noProblems = false;
        for (unsigned int i = 0; i < numCheckFailures; i++)
        {
            const SBMLError* sbmlErr = _sbmlDoc->getError(i);
            if (sbmlErr->isFatal() || sbmlErr->isError())
            {
                ++numConsistencyErrors;
            }
            else
            {
                ++numConsistencyWarnings;
            }
        }
        std::ostringstream oss;
        _sbmlDoc->printErrors(oss);
        consistencyMessages = oss.str();
    }
    // If the internal checks fail, it makes little sense to attempt
    // further validation, because the model may be too compromised to
    // be properly interpreted.
    if (numConsistencyErrors > 0)
    {
        consistencyMessages += "Further validation aborted.";
    }
    else
    {
        numCheckFailures = _sbmlDoc->checkConsistency();
        if (numCheckFailures > 0)
        {
            noProblems = false;
            for (unsigned int i = 0; i < numCheckFailures; i++)
            {
                const SBMLError* sbmlErr = _sbmlDoc->getError(i);
                if (sbmlErr->isFatal() || sbmlErr->isError())
                {
                    ++numValidationErrors;
                }
                else
                {
                    ++numValidationWarnings;
                }
            }
            std::ostringstream oss;
            _sbmlDoc->printErrors(oss);
            validationMessages = oss.str();
        }
    }
    if (noProblems)
        return true;
    else
    {
        if (numConsistencyErrors > 0)
        {
            std::cout << "ERROR: encountered " << numConsistencyErrors
                << " consistency error" << (numConsistencyErrors == 1 ? "" : "s")
                << " in model '" << _sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        if (numConsistencyWarnings > 0)
        {
            std::cout << "Notice: encountered " << numConsistencyWarnings
                << " consistency warning" << (numConsistencyWarnings == 1 ? "" : "s")
                << " in model '" << _sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        std::cout << std::endl << consistencyMessages;
        if (numValidationErrors > 0)
        {
            std::cout << "ERROR: encountered " << numValidationErrors
                << " validation error" << (numValidationErrors == 1 ? "" : "s")
                << " in model '" << _sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        if (numValidationWarnings > 0)
        {
            std::cout << "Notice: encountered " << numValidationWarnings
                << " validation warning" << (numValidationWarnings == 1 ? "" : "s")
                << " in model '" << _sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        std::cout << std::endl << validationMessages;
        return (numConsistencyErrors == 0 && numValidationErrors == 0);
    }
}

SBMLDocument* SbmlParser::OpenSBMLFile(const char* _filePath)
{
    std::cout << "Trying to read SBML file: " << _filePath << std::endl;
	SBMLDocument* document = readSBMLFromFile(_filePath);
    bool SBMLok = ValidateSBML(document);
    if (SBMLok)
    {
        std::cout << " The SBML validation process for file at: " << _filePath << " is a SUCCESS." << std::endl;
        return document;
    }
    else
    {
        std::cout << " The SBML validation process for file at: " << _filePath <<" has FAILED (see errors above)." << std::endl;
        return nullptr;
    }
}

void SbmlParser::PrettyPrintSBMLDocument(SBMLDocument* _sbmlDoc)
{
    Model* sbmlModel = _sbmlDoc->getModel();

    const std::string modelID = sbmlModel->getId();
    unsigned int level = sbmlModel->getLevel();
    unsigned int version = sbmlModel->getVersion();

    std::cout << "Loading model " << modelID << "." << std::endl;
    std::cout << "Compliant with SBML core level " << level << " version " << version << "." << std::endl;

    std::cout << "Species defined:" << std::endl;
    for (int i = 0; i < sbmlModel->getNumSpecies(); ++i)
    {
        Species* sp = sbmlModel->getSpecies(i);
        std::cout <<
            "\t" << sp->getId() << ": " << std::endl <<
            "\t Initial Amount: " << sp->getInitialAmount() << std::endl <<
            "\t Has Only Substance Unit: " << sp->getHasOnlySubstanceUnits() << std::endl <<
            "\t Compartment: " << sp->getCompartment() << std::endl <<
            "\t Constant: " << sp->getConstant() << std::endl <<
            "\t Boundary Condition: " << sp->getBoundaryCondition() << std::endl;
    }

    std::cout << "Reactions defined:" << std::endl;
    for (int i = 0; i < sbmlModel->getNumReactions(); ++i)
    {
        Reaction* r = sbmlModel->getReaction(i);
        int nbReactants = r->getNumReactants();
        int nbProducts = r->getNumProducts();

        std::cout << "\t" << r->getId() << ":" << std::endl;
        if (nbReactants > 0)
        {
            std::cout << "\t " << r->getReactant(0)->getSpecies();
        }
        else
        {
            std::cout << "\t None";
        }
        for (int j = 1; j < nbReactants; ++j)
        {
            std::cout << " + " << r->getReactant(j)->getSpecies();
        }

        if (nbProducts > 0)
        {
            std::cout << " --> " << r->getProduct(0)->getSpecies();
        }
        else
        {
            std::cout << " --> None";
        }

        for (int j = 1; j < nbProducts; ++j)
        {
            std::cout << " + " << r->getProduct(j)->getSpecies();
        }
        std::cout << std::endl;
        std::cout << "\t Kinetic Law: " << r->getKineticLaw()->getFormula() << std::endl;;
    }

}