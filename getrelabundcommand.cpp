/*
 *  getrelabundcommand.cpp
 *  Mothur
 *
 *  Created by westcott on 6/21/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "getrelabundcommand.h"

//**********************************************************************************************************************
vector<string> GetRelAbundCommand::getValidParameters(){	
	try {
		string Array[] =  {"groups","label","scale","outputdir","inputdir"};
		vector<string> myArray (Array, Array+(sizeof(Array)/sizeof(string)));
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "GetRelAbundCommand", "getValidParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
GetRelAbundCommand::GetRelAbundCommand(){	
	try {
		abort = true; calledHelp = true; 
		vector<string> tempOutNames;
		outputTypes["relabund"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "GetRelAbundCommand", "GetRelAbundCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
vector<string> GetRelAbundCommand::getRequiredParameters(){	
	try {
		vector<string> myArray;
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "GetRelAbundCommand", "getRequiredParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
vector<string> GetRelAbundCommand::getRequiredFiles(){	
	try {
		string Array[] =  {"shared"};
		vector<string> myArray (Array, Array+(sizeof(Array)/sizeof(string)));
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "GetRelAbundCommand", "getRequiredFiles");
		exit(1);
	}
}
//**********************************************************************************************************************
GetRelAbundCommand::GetRelAbundCommand(string option) {
	try {
		globaldata = GlobalData::getInstance();
		abort = false; calledHelp = false;   
		allLines = 1;
		labels.clear();
		
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		
		else {
			//valid paramters for this command
			string AlignArray[] =  {"groups","label","scale","outputdir","inputdir"};
			vector<string> myArray (AlignArray, AlignArray+(sizeof(AlignArray)/sizeof(string)));
			
			OptionParser parser(option);
			map<string,string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			
			//check to make sure all parameters are valid for command
			for (map<string,string>::iterator it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["relabund"] = tempOutNames;
		
			//if the user changes the output directory command factory will send this info to us in the output parameter 
			outputDir = validParameter.validFile(parameters, "outputdir", false);		if (outputDir == "not found"){	
				outputDir = "";	
				outputDir += m->hasPath(globaldata->inputFileName); //if user entered a file with a path then preserve it	
			}
			
			//make sure the user has already run the read.otu command
			if ((globaldata->getSharedFile() == "")) {
				 m->mothurOut("You must read a list and a group, or a shared file before you can use the get.relabund command."); m->mothurOutEndLine(); abort = true; 
			}

			//check for optional parameter and set defaults
			// ...at some point should added some additional type checking...
			label = validParameter.validFile(parameters, "label", false);			
			if (label == "not found") { label = ""; }
			else { 
				if(label != "all") {  m->splitAtDash(label, labels);  allLines = 0;  }
				else { allLines = 1;  }
			}
			
			//if the user has not specified any labels use the ones from read.otu
			if (label == "") {  
				allLines = globaldata->allLines; 
				labels = globaldata->labels; 
			}
			
			groups = validParameter.validFile(parameters, "groups", false);			
			if (groups == "not found") { groups = ""; pickedGroups = false; }
			else { 
				pickedGroups = true;
				m->splitAtDash(groups, Groups);
				globaldata->Groups = Groups;
			}
			
			scale = validParameter.validFile(parameters, "scale", false);				if (scale == "not found") { scale = "totalgroup"; }
			
			if ((scale != "totalgroup") && (scale != "totalotu") && (scale != "averagegroup") && (scale != "averageotu")) {
				m->mothurOut(scale + " is not a valid scaling option for the get.relabund command. Choices are totalgroup, totalotu, averagegroup, averageotu."); m->mothurOutEndLine(); abort = true; 
			}
		}

	}
	catch(exception& e) {
		m->errorOut(e, "GetRelAbundCommand", "GetRelAbundCommand");
		exit(1);
	}
}

//**********************************************************************************************************************

void GetRelAbundCommand::help(){
	try {
		m->mothurOut("The get.relabund command can only be executed after a successful read.otu command of a list and group or shared file.\n");
		m->mothurOut("The get.relabund command parameters are groups, scale and label.  No parameters are required.\n");
		m->mothurOut("The groups parameter allows you to specify which of the groups in your groupfile you would like included. The group names are separated by dashes.\n");
		m->mothurOut("The label parameter allows you to select what distance levels you would like, and are also separated by dashes.\n");
		m->mothurOut("The scale parameter allows you to select what scale you would like to use. Choices are totalgroup, totalotu, averagegroup, averageotu, default is totalgroup.\n");
		m->mothurOut("The get.relabund command should be in the following format: get.relabund(groups=yourGroups, label=yourLabels).\n");
		m->mothurOut("Example get.relabund(groups=A-B-C, scale=averagegroup).\n");
		m->mothurOut("The default value for groups is all the groups in your groupfile, and all labels in your inputfile will be used.\n");
		m->mothurOut("The get.relabund command outputs a .relabund file.\n");
		m->mothurOut("Note: No spaces between parameter labels (i.e. groups), '=' and parameters (i.e.yourGroups).\n\n");

	}
	catch(exception& e) {
		m->errorOut(e, "GetRelAbundCommand", "help");
		exit(1);
	}
}

//**********************************************************************************************************************

GetRelAbundCommand::~GetRelAbundCommand(){
}

//**********************************************************************************************************************

int GetRelAbundCommand::execute(){
	try {
	
		if (abort == true) { if (calledHelp) { return 0; }  return 2;	}
		
		string outputFileName = outputDir + m->getRootName(m->getSimpleName(globaldata->inputFileName)) + "relabund";
		ofstream out;
		m->openOutputFile(outputFileName, out);
		out.setf(ios::fixed, ios::floatfield); out.setf(ios::showpoint);
		
		read = new ReadOTUFile(globaldata->inputFileName);	
		read->read(&*globaldata); 
		input = globaldata->ginput;
		lookup = input->getSharedRAbundVectors();
		string lastLabel = lookup[0]->getLabel();
		
		//if the users enters label "0.06" and there is no "0.06" in their file use the next lowest label.
		set<string> processedLabels;
		set<string> userLabels = labels;

		//as long as you are not at the end of the file or done wih the lines you want
		while((lookup[0] != NULL) && ((allLines == 1) || (userLabels.size() != 0))) {
			
			if (m->control_pressed) {  outputTypes.clear();  for (int i = 0; i < lookup.size(); i++) {  delete lookup[i];  } globaldata->Groups.clear(); delete read;  out.close(); remove(outputFileName.c_str()); return 0; }
	
			if(allLines == 1 || labels.count(lookup[0]->getLabel()) == 1){			

				m->mothurOut(lookup[0]->getLabel()); m->mothurOutEndLine();
				getRelAbundance(lookup, out);
				
				processedLabels.insert(lookup[0]->getLabel());
				userLabels.erase(lookup[0]->getLabel());
			}
			
			if ((m->anyLabelsToProcess(lookup[0]->getLabel(), userLabels, "") == true) && (processedLabels.count(lastLabel) != 1)) {
				string saveLabel = lookup[0]->getLabel();
			
				for (int i = 0; i < lookup.size(); i++) {  delete lookup[i];  }  
				lookup = input->getSharedRAbundVectors(lastLabel);
				m->mothurOut(lookup[0]->getLabel()); m->mothurOutEndLine();
				
				getRelAbundance(lookup, out);
				
				processedLabels.insert(lookup[0]->getLabel());
				userLabels.erase(lookup[0]->getLabel());
				
				//restore real lastlabel to save below
				lookup[0]->setLabel(saveLabel);
			}
			
			lastLabel = lookup[0]->getLabel();
			//prevent memory leak
			for (int i = 0; i < lookup.size(); i++) {  delete lookup[i]; lookup[i] = NULL; }
			
			if (m->control_pressed) {  outputTypes.clear();  globaldata->Groups.clear(); delete read;  out.close(); remove(outputFileName.c_str()); return 0; }

			//get next line to process
			lookup = input->getSharedRAbundVectors();				
		}
		
		if (m->control_pressed) { outputTypes.clear(); globaldata->Groups.clear(); delete read;  out.close(); remove(outputFileName.c_str());  return 0; }

		//output error messages about any remaining user labels
		set<string>::iterator it;
		bool needToRun = false;
		for (it = userLabels.begin(); it != userLabels.end(); it++) {  
			m->mothurOut("Your file does not include the label " + *it); 
			if (processedLabels.count(lastLabel) != 1) {
				m->mothurOut(". I will use " + lastLabel + "."); m->mothurOutEndLine();
				needToRun = true;
			}else {
				m->mothurOut(". Please refer to " + lastLabel + "."); m->mothurOutEndLine();
			}
		}
	
		//run last label if you need to
		if (needToRun == true)  {
			for (int i = 0; i < lookup.size(); i++) { if (lookup[i] != NULL) { delete lookup[i]; } }  
			lookup = input->getSharedRAbundVectors(lastLabel);
			
			m->mothurOut(lookup[0]->getLabel()); m->mothurOutEndLine();
			
			getRelAbundance(lookup, out);
			
			for (int i = 0; i < lookup.size(); i++) {  delete lookup[i];  }
		}
	
		//reset groups parameter
		globaldata->Groups.clear();  
		delete input; globaldata->ginput = NULL;
		delete read;
		out.close();
		
		if (m->control_pressed) { outputTypes.clear(); remove(outputFileName.c_str()); return 0;}
		
		m->mothurOutEndLine();
		m->mothurOut("Output File Names: "); m->mothurOutEndLine();
		m->mothurOut(outputFileName); m->mothurOutEndLine(); outputNames.push_back(outputFileName); outputTypes["relabund"].push_back(outputFileName);
		m->mothurOutEndLine();
		
		//set relabund file as new current relabundfile
		string current = "";
		itTypes = outputTypes.find("relabund");
		if (itTypes != outputTypes.end()) {
			if ((itTypes->second).size() != 0) { current = (itTypes->second)[0]; m->setRelAbundFile(current); }
		}
		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "GetRelAbundCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************

int GetRelAbundCommand::getRelAbundance(vector<SharedRAbundVector*>& thisLookUp, ofstream& out){
	try {
		
		 for (int i = 0; i < thisLookUp.size(); i++) {
			out << thisLookUp[i]->getLabel() << '\t' << thisLookUp[i]->getGroup() << '\t' << thisLookUp[i]->getNumBins() << '\t';
			
			for (int j = 0; j < thisLookUp[i]->getNumBins(); j++) {
			
				if (m->control_pressed) { return 0; }
			
				int abund = thisLookUp[i]->getAbundance(j);
				
				float relabund = 0.0;
				
				if (scale == "totalgroup") { 
					relabund = abund / (float) thisLookUp[i]->getNumSeqs();
				}else if (scale == "totalotu") {
					//calc the total in this otu
					int totalOtu = 0;
					for (int l = 0; l < thisLookUp.size(); l++) {  totalOtu += thisLookUp[l]->getAbundance(j); }
					relabund = abund / (float) totalOtu;
				}else if (scale == "averagegroup") {
					relabund = abund / (float) (thisLookUp[i]->getNumSeqs() / (float) thisLookUp[i]->getNumBins());
				}else if (scale == "averageotu") {
					//calc the total in this otu
					int totalOtu = 0;
					for (int l = 0; l < thisLookUp.size(); l++) {  totalOtu += thisLookUp[l]->getAbundance(j); }
					float averageOtu = totalOtu / (float) thisLookUp.size();
					
					relabund = abund / (float) averageOtu;
				}else{ m->mothurOut(scale + " is not a valid scaling option."); m->mothurOutEndLine(); m->control_pressed = true; return 0; }
				
				out << relabund << '\t';
			}
			out << endl;
		 }
	
		 return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "GetRelAbundCommand", "getRelAbundance");
		exit(1);
	}
}
//**********************************************************************************************************************


