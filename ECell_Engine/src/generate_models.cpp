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

#include <iostream>
#include "generate_models.hpp"
using namespace std;
LIBSBML_CPP_NAMESPACE_USE


//===============================================================================
//
// Main routine
//
//  Creates SBML models represented in "Example models expressed in XML using
//  SBML" in Section 7 of the SBML Level 3 Version 2 specification(*). 
//
//   (*) The specification document is available at the following URL:
//       http://sbml.org/Documents/Specifications
//
//===============================================================================
//
//int main(int argc, char* argv[])
//{
//    SBMLDocument* sbmlDoc = 0;
//    bool SBMLok = false;
//    try
//    {
//        //-------------------------------------------------
//        // 7.1 A Simple example application of SBML
//        //-------------------------------------------------
//        sbmlDoc = createExampleEnzymaticReaction();
//        SBMLok = validateExampleSBML(sbmlDoc);
//        if (SBMLok) writeExampleSBML(sbmlDoc, "enzymaticreaction.xml");
//        delete sbmlDoc;
//        if (!SBMLok) return 1;
//    }
//    catch (std::bad_alloc& e)
//    {
//        cerr << e.what() << ": Unable to allocate memory." << endl;
//        return 1;
//    }
//    catch (...)
//    {
//        cerr << "Unexpected exceptional condition encountered." << endl;
//        return 1;
//    }
//    // A 0 return status is the standard Unix/Linux way to say "all ok".
//    return 0;
//}

Parameter* defineParameter(
    Parameter* _para_ptr,
    const std::string& _paraID,
    const std::string _unit,
    const double _value)
{
    _para_ptr->setId(_paraID);
    _para_ptr->setValue(_value);
    _para_ptr->setUnits(_unit);

    return _para_ptr;
}

Species* defineSpecies(
    Species* _sp_ptr,
    const std::string& _id,
    const std::string& _name,
    const std::string& _compartmentID,
    const std::string& _unit,
    const double _initialQuantity)
{
    //Required L3V2
    _sp_ptr->setId(_id);
    _sp_ptr->setCompartment(_compartmentID);
    _sp_ptr->setHasOnlySubstanceUnits(true);
    _sp_ptr->setBoundaryCondition(false);
    _sp_ptr->setConstant(false);

    //Optional L3V2
    _sp_ptr->setName(_name);
    // Sets the "compartment" attribute of the Species object to identify the 
    // compartment in which the Species object is located.
    
    // Sets the "initialAmount" attribute of the Species object.
    //
    //  In SBML, the units of a Species object's initial quantity are
    //  determined by two attributes, "substanceUnits" and
    //  "hasOnlySubstanceUnits", and the "spatialDimensions" attribute
    //  of the Compartment object ("cytosol") in which the species
    //  object is located.  Here, we are setting the values for
    //  "substanceUnits" to _unit and "hasOnlySubstanceUnits" to true. 
    //  So, the species defined do not take the unit of the compartement in
    //  which it is located.
    
    _sp_ptr->setSubstanceUnits(_unit);
    _sp_ptr->setInitialAmount(_initialQuantity);

    return _sp_ptr;
}

void defineReaction(
    ReactionPointersCapsule* _rpc,
    const std::string& _rID,
    const std::vector<std::string>& _reactantIDs,
    const std::vector<std::string>& _productIDs,
    const std::string& _kParamID)
{
    _rpc->r->setId(_rID);
    _rpc->r->setReversible(false);

    // (Reactant1) Creates the Reactants.
    // The object will be created within the reaction in the SBML <listOfReactants>.
    for (auto it = _reactantIDs.begin(); it != _reactantIDs.end(); ++it)
    {
        _rpc->spr = _rpc->r->createReactant();
        _rpc->spr->setSpecies(*it);
        _rpc->spr->setConstant(false);
    }

    // (Reactant1) Creates the Reactants.
    for (auto it = _productIDs.begin(); it != _productIDs.end(); ++it)
    {
        _rpc->spr = _rpc->r->createProduct();
        _rpc->spr->setSpecies(*it);
        _rpc->spr->setConstant(false);
    }

    //---------------------------------------------------------------------------
    // Creates a KineticLaw object inside the Reaction object ("R1"). 
    //---------------------------------------------------------------------------
    _rpc->kl = _rpc->r->createKineticLaw();

    ASTNode* astKl_Formula = new ASTNode(AST_NAME);
    string mathXMLString =
        "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">"
        "  <apply>"
        "    <times/>"
        "    <ci> ";
    mathXMLString += _kParamID;
    mathXMLString += " </ci>";

    for (auto it = _reactantIDs.begin(); it != _reactantIDs.end(); ++it)
    {
        mathXMLString += "    <ci> ";
        mathXMLString += *it;
        mathXMLString += " </ci>";
    }

    mathXMLString +=
        "  </apply>"
        "</math>";

    astKl_Formula = readMathMLFromString(mathXMLString.c_str());
    _rpc->kl->setMath(astKl_Formula);
    delete astKl_Formula;
}


//===============================================================================
// Functions for creating the Example SBML documents.
//===============================================================================
SBMLDocument* createExampleEnzymaticReaction_()
{
    const unsigned int level = Level;
    const unsigned int version = Version;

    // Creates an SBMLDocument object 
    SBMLDocument* sbmlDoc = new SBMLDocument(level, version);

    // Creates a Model object inside the SBMLDocument object. 
    Model* model = sbmlDoc->createModel();
    model->setId("Gillespie_Gibson_Example");
    model->setSubstanceUnits("item");
    model->setTimeUnits("second");
    model->setExtentUnits("item");

    // Creates UnitDefinition objects inside the Model object.
    // Temporary pointers (reused more than once below).
    UnitDefinition* unitdef;
    Unit* unit;

    //---------------------------------------------------------------------------  
    // (UnitDefinition1) Creates an UnitDefinition object ("count")
    //---------------------------------------------------------------------------
    //unitdef = model->createUnitDefinition();
    //unitdef->setId("count");
    ////  Creates an Unit inside the UnitDefinition object 
    //unit = unitdef->createUnit();
    //unit->setKind(UNIT_KIND_ITEM);
    //unit->setMultiplier(1);
    //unit->setScale(0);
    //unit->setExponent(1);

    //---------------------------------------------------------------------------  
    // (UnitDefinition1) Creates an UnitDefinition object ("count_per_second")
    //---------------------------------------------------------------------------
    unitdef = model->createUnitDefinition();
    unitdef->setId("item_per_second");
    //  Creates an Unit inside the UnitDefinition object 
    unit = unitdef->createUnit();
    unit->setId("item");
    unit->setKind(UNIT_KIND_ITEM);
    unit->setMultiplier(1);
    unit->setScale(0);
    unit->setExponent(1);

    //  Creates an Unit inside the UnitDefinition object 
    unit = unitdef->createUnit();
    unit->setKind(UNIT_KIND_SECOND);
    unit->setId("per_second");
    unit->setMultiplier(1);
    unit->setScale(0);
    unit->setExponent(-1);

    //---------------------------------------------------------------------------
    // Creates a Compartment object inside the Model object.
    //---------------------------------------------------------------------------
    Compartment* comp;
    const string compName = "Simulation_Space";
    comp = model->createCompartment();
    comp->setId(compName);
    comp->setConstant(true);
    comp->setUnits("dimensionless");
    //comp->setSpatialDimensions(3.f);

    // Sets the "size" attribute of the Compartment object.
    // We are not setting the units on the compartment size explicitly, so
    // the units of this Compartment object will be the default SBML units of
    // volume, which are liters.
    comp->setSize(1);

    //---------------------------------------------------------------------------
    // Creates Species objects inside the Model object. 
    //---------------------------------------------------------------------------
    // Temporary pointer (reused more than once below).
    Species* sp;

    sp = model->createSpecies();
    sp = defineSpecies(sp, "A", "A", compName, "item", 6);

    sp = model->createSpecies();
    sp = defineSpecies(sp, "B", "B", compName, "item", 14);

    sp = model->createSpecies();
    sp = defineSpecies(sp, "C", "C", compName, "item", 8);

    sp = model->createSpecies();
    sp = defineSpecies(sp, "D", "D", compName, "item", 12);

    sp = model->createSpecies();
    sp = defineSpecies(sp, "E", "E", compName, "item", 9);

    sp = model->createSpecies();
    sp = defineSpecies(sp, "F", "F", compName, "item", 3);

    sp = model->createSpecies();
    sp = defineSpecies(sp, "G", "G", compName, "item", 3);
    

    //---------------------------------------------------------------------------
    // Creates Reaction objects inside the Model object.
    //---------------------------------------------------------------------------
    // Temporary pointers.
    ReactionPointersCapsule rpc;
    Parameter* para;

    // Creates the reaction
    rpc.r = model->createReaction();
    defineReaction(&rpc, "R1", {"A", "B"}, {"C"}, "k1");
    // Creates local Parameter object inside the KineticLaw object associated with the
    // last created reaction.
    para = rpc.kl->createParameter();
    defineParameter(para, "k1", "item_per_second", 1);

    rpc.r = model->createReaction();
    defineReaction(&rpc, "R2", { "B", "C" }, { "D" }, "k2");
    para = rpc.kl->createParameter();
    defineParameter(para, "k2", "item_per_second", 1);

    rpc.r = model->createReaction();
    defineReaction(&rpc, "R3", { "D", "E" }, { "D","F" }, "k3");
    para = rpc.kl->createParameter();
    defineParameter(para, "k3", "item_per_second", 1);

    rpc.r = model->createReaction();
    defineReaction(&rpc, "R4", { "F" }, { "D", "G" }, "k4");
    para = rpc.kl->createParameter();
    defineParameter(para, "k4", "item_per_second", 1);

    rpc.r = model->createReaction();
    defineReaction(&rpc, "R5", { "E", "G" }, { "A" }, "k5");
    para = rpc.kl->createParameter();
    defineParameter(para, "k5", "item_per_second", 1);
    
    return sbmlDoc;
}

//===============================================================================
// Helper functions for writing/validating the given SBML documents.
//===============================================================================
bool validateExampleSBML_(SBMLDocument* sbmlDoc)
{
    if (!sbmlDoc)
    {
        cerr << "validateExampleSBML: given a null SBML Document" << endl;
        return false;
    }

    string consistencyMessages;
    string validationMessages;
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
        ostringstream oss;
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
            ostringstream oss;
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
            cout << "ERROR: encountered " << numConsistencyErrors
                << " consistency error" << (numConsistencyErrors == 1 ? "" : "s")
                << " in model '" << sbmlDoc->getModel()->getId() << "'." << endl;
        }
        if (numConsistencyWarnings > 0)
        {
            cout << "Notice: encountered " << numConsistencyWarnings
                << " consistency warning" << (numConsistencyWarnings == 1 ? "" : "s")
                << " in model '" << sbmlDoc->getModel()->getId() << "'." << endl;
        }
        cout << endl << consistencyMessages;
        if (numValidationErrors > 0)
        {
            cout << "ERROR: encountered " << numValidationErrors
                << " validation error" << (numValidationErrors == 1 ? "" : "s")
                << " in model '" << sbmlDoc->getModel()->getId() << "'." << endl;
        }
        if (numValidationWarnings > 0)
        {
            cout << "Notice: encountered " << numValidationWarnings
                << " validation warning" << (numValidationWarnings == 1 ? "" : "s")
                << " in model '" << sbmlDoc->getModel()->getId() << "'." << endl;
        }
        cout << endl << validationMessages;
        return (numConsistencyErrors == 0 && numValidationErrors == 0);
    }
}

bool writeExampleSBML_(const SBMLDocument* sbmlDoc, const string& filename)
{
    SBMLWriter sbmlWriter;
    bool result = sbmlWriter.writeSBML(sbmlDoc, filename);
    if (result)
    {
        cout << "Wrote file \"" << filename << "\"" << endl;
        return true;
    }
    else
    {
        cerr << "Failed to write \"" << filename << "\"" << endl;
        return false;
    }
}