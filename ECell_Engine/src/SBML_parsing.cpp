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

#include "SBML_parsing.hpp"

bool SBML_Parser::ValidateSBML(SBMLDocument* sbmlDoc)
{
    if (!sbmlDoc)
    {
        std::cerr << "validateExampleSBML: given a null SBML Document" << std::endl;
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
    numCheckFailures = sbmlDoc->checkInternalConsistency();
    if (numCheckFailures > 0)
    {
        noProblems = false;
        for (unsigned int i = 0; i < numCheckFailures; i++)
        {
            const SBMLError* sbmlErr = sbmlDoc->getError(i);
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
        sbmlDoc->printErrors(oss);
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
        numCheckFailures = sbmlDoc->checkConsistency();
        if (numCheckFailures > 0)
        {
            noProblems = false;
            for (unsigned int i = 0; i < numCheckFailures; i++)
            {
                const SBMLError* sbmlErr = sbmlDoc->getError(i);
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
            sbmlDoc->printErrors(oss);
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
                << " in model '" << sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        if (numConsistencyWarnings > 0)
        {
            std::cout << "Notice: encountered " << numConsistencyWarnings
                << " consistency warning" << (numConsistencyWarnings == 1 ? "" : "s")
                << " in model '" << sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        std::cout << std::endl << consistencyMessages;
        if (numValidationErrors > 0)
        {
            std::cout << "ERROR: encountered " << numValidationErrors
                << " validation error" << (numValidationErrors == 1 ? "" : "s")
                << " in model '" << sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        if (numValidationWarnings > 0)
        {
            std::cout << "Notice: encountered " << numValidationWarnings
                << " validation warning" << (numValidationWarnings == 1 ? "" : "s")
                << " in model '" << sbmlDoc->getModel()->getId() << "'." << std::endl;
        }
        std::cout << std::endl << validationMessages;
        return (numConsistencyErrors == 0 && numValidationErrors == 0);
    }
}

SBMLDocument* SBML_Parser::OpenSBMLFile(const char* _filePath)
{
	SBMLDocument* document = readSBMLFromFile(_filePath);


}

void Gillespie_SBML_Parser::ProcessSBMLDocument(SBMLDocument* _document)
{

}