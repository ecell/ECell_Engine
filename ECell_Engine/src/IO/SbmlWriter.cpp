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
#include "IO/SbmlWriter.hpp"
using namespace std;
using namespace ECellEngine::Maths;

void ECellEngine::IO::SbmlWriter::DefineParameter(
    Parameter* _para_ptr,
    const std::string& _paraID,
    const std::string& _unit)
{
    _para_ptr->setId(_paraID);
    _para_ptr->setUnits(_unit);
    _para_ptr->setConstant(false);
}

void ECellEngine::IO::SbmlWriter::DefineParameter(
    Parameter* _para_ptr,
    const std::string& _paraID,
    const std::string& _unit,
    const double _value)
{
    _para_ptr->setId(_paraID);
    _para_ptr->setValue(_value);
    _para_ptr->setUnits(_unit);
    _para_ptr->setConstant(true);
}

void ECellEngine::IO::SbmlWriter::DefineParameterAssignementRule(
    AssignmentRule* _ar,
    const std::string& _variableID,
    const std::string& _formula)
{
    _ar->setVariable(_variableID);
    ASTNode* math = SBML_parseL3Formula(_formula.c_str());
    _ar->setMath(math);
}

void ECellEngine::IO::SbmlWriter::DefineReaction(
    ReactionPointersCapsule* _rpc,
    const std::string& _rID,
    const std::vector<std::string>& _reactantIDs,
    const std::vector<int>& _reactantStoichs,
    const std::vector<std::string>& _productIDs,
    const std::vector<int>& _productStoichs,
    const std::string& _kParamID)
{
    _rpc->r->setId(_rID);
    _rpc->r->setReversible(false);

    // Creates the Reactants.
    // The object will be created within the reaction in the SBML <listOfReactants>.
    int cnt = 0;
    for (auto it = _reactantIDs.begin(); it != _reactantIDs.end(); ++it)
    {
        _rpc->spr = _rpc->r->createReactant();
        _rpc->spr->setSpecies(*it);
        _rpc->spr->setStoichiometry(_reactantStoichs[cnt]);
        _rpc->spr->setConstant(false);
        cnt++;
    }

    // Creates the products.
    cnt = 0;
    for (auto it = _productIDs.begin(); it != _productIDs.end(); ++it)
    {
        _rpc->spr = _rpc->r->createProduct();
        _rpc->spr->setSpecies(*it);
        _rpc->spr->setStoichiometry(_productStoichs[cnt]);
        _rpc->spr->setConstant(false);
        cnt++;
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
    cnt = 0;
    for (auto it = _reactantIDs.begin(); it != _reactantIDs.end(); ++it)
    {
        /*mathXMLString += "    <ci> ";
        mathXMLString += *it;
        mathXMLString += " </ci>";*/
        mathXMLString += str_nCR(*it, _reactantStoichs[cnt]);
        cnt++;
    }

    mathXMLString +=
        "  </apply>"
        "</math>";

    astKl_Formula = readMathMLFromString(mathXMLString.c_str());
    _rpc->kl->setMath(astKl_Formula);
    delete astKl_Formula;
}

void ECellEngine::IO::SbmlWriter::DefineSpecies(
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
}


//===============================================================================
// Functions for creating the Example SBML documents.
//===============================================================================
SBMLDocument* ECellEngine::IO::SbmlWriter::GibsonAndBruckToyModel()
{
    const unsigned int level = Level;
    const unsigned int version = Version;

    // Creates an SBMLDocument object 
    SBMLDocument* sbmlDoc = new SBMLDocument(level, version);

    // Creates a Model object inside the SBMLDocument object. 
    Model* model = sbmlDoc->createModel();
    model->setId("GibsonAndBruckToyModel");
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
    // (UnitDefinition1) Creates an UnitDefinition object ("per_second")
    //---------------------------------------------------------------------------
    unitdef = model->createUnitDefinition();
    unitdef->setId("per_second");
    //  Creates an Unit inside the UnitDefinition object 
    unit = unitdef->createUnit();
    unit->setKind(UNIT_KIND_SECOND);
    unit->setMultiplier(1);
    unit->setScale(0);
    unit->setExponent(-1);

    //---------------------------------------------------------------------------  
    // (UnitDefinition1) Creates an UnitDefinition object ("per_item_per_second")
    //---------------------------------------------------------------------------
    unitdef = model->createUnitDefinition();
    unitdef->setId("per_item_per_second");
    //  Creates an Unit inside the UnitDefinition object 
    unit = unitdef->createUnit();
    unit->setKind(UNIT_KIND_ITEM);
    unit->setMultiplier(1);
    unit->setScale(0);
    unit->setExponent(-1);

    //  Creates an Unit inside the UnitDefinition object 
    unit = unitdef->createUnit();
    unit->setKind(UNIT_KIND_SECOND);
    unit->setMultiplier(1);
    unit->setScale(0);
    unit->setExponent(-1);

    //---------------------------------------------------------------------------  
    // (UnitDefinition1) Creates an UnitDefinition object ("per_itemSQ_per_second")
    //---------------------------------------------------------------------------
    unitdef = model->createUnitDefinition();
    unitdef->setId("per_itemSQ_per_second");
    //  Creates an Unit inside the UnitDefinition object 
    unit = unitdef->createUnit();
    unit->setKind(UNIT_KIND_ITEM);
    unit->setMultiplier(1);
    unit->setScale(0);
    unit->setExponent(-2);

    //  Creates an Unit inside the UnitDefinition object 
    unit = unitdef->createUnit();
    unit->setKind(UNIT_KIND_SECOND);
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
    DefineSpecies(sp, "A", "A", compName, "item", 6);

    sp = model->createSpecies();
    DefineSpecies(sp, "B", "B", compName, "item", 14);

    sp = model->createSpecies();
    DefineSpecies(sp, "C", "C", compName, "item", 8);

    sp = model->createSpecies();
    DefineSpecies(sp, "D", "D", compName, "item", 12);

    sp = model->createSpecies();
    DefineSpecies(sp, "E", "E", compName, "item", 9);

    sp = model->createSpecies();
    DefineSpecies(sp, "F", "F", compName, "item", 3);

    sp = model->createSpecies();
    DefineSpecies(sp, "G", "G", compName, "item", 3);
    
    //---------------------------------------------------------------------------
    // Creates Global Parameters objects inside the Model object.
    //---------------------------------------------------------------------------
    // Temporary pointers
    Parameter* para = model->createParameter();
    DefineParameter(para, "k1", "per_item_per_second");

    para = model->createParameter();
    DefineParameter(para, "k2", "per_item_per_second");

    para = model->createParameter();
    DefineParameter(para, "k3", "per_item_per_second", 1);

    para = model->createParameter();
    DefineParameter(para, "k4", "per_second", 1);

    para = model->createParameter();
    DefineParameter(para, "k5", "per_item_per_second", 1);

    //---------------------------------------------------------------------------
    // Creates an assignment rule for a parameter inside the Model object.
    //---------------------------------------------------------------------------
    AssignmentRule* ar = model->createAssignmentRule();
    DefineParameterAssignementRule(ar, "k1", "1per_item_per_second+(2dimensionless*k2-1per_item_per_second/2dimensionless)");

    ar = model->createAssignmentRule();
    DefineParameterAssignementRule(ar, "k2", "2dimensionless*k3");

    //---------------------------------------------------------------------------
    // Creates Reaction objects inside the Model object.
    //---------------------------------------------------------------------------
    // Temporary pointers.
    ReactionPointersCapsule rpc;

    // Creates the reaction
    rpc.r = model->createReaction();
    DefineReaction(&rpc, "R1", {"A", "B"}, { 1, 1 }, {"C"}, { 1 }, "k1");

    rpc.r = model->createReaction();
    DefineReaction(&rpc, "R2", { "B", "C" }, { 1, 1 }, { "D" }, { 1 }, "k2");

    rpc.r = model->createReaction();
    DefineReaction(&rpc, "R3", { "D", "E" }, { 1, 1 }, { "E","F" }, { 1, 1 }, "k3");

    rpc.r = model->createReaction();
    DefineReaction(&rpc, "R4", { "F" }, { 1 }, { "D", "G" }, { 1, 1 }, "k4");

    rpc.r = model->createReaction();
    DefineReaction(&rpc, "R5", { "E", "G" }, { 1, 1 }, { "A" }, { 1 }, "k5");
    
    return sbmlDoc;
}

bool ECellEngine::IO::SbmlWriter::WriteSBML(const SBMLDocument* sbmlDoc, const string& filename)
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