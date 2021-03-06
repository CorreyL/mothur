/*
 *  mergefilecommand.cpp
 *  Mothur
 *
 *  Created by Pat Schloss on 6/14/09.
 *  Copyright 2009 Patrick D. Schloss. All rights reserved.
 *
 */

#include "mergefilecommand.h"

//**********************************************************************************************************************
vector<string> MergeFileCommand::setParameters(){	
	try {
		CommandParameter pinput("input", "String", "", "", "", "", "","",false,true,true); parameters.push_back(pinput);
		CommandParameter poutput("output", "String", "", "", "", "", "","",false,true,true); parameters.push_back(poutput);
		CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "MergeFileCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string MergeFileCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The merge.file command takes a list of files separated by dashes and merges them into one file."; 
		helpString += "The merge.file command parameters are input and output."; 
		helpString += "Example merge.file(input=small.fasta-large.fasta, output=all.fasta).";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "MergeFileCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
MergeFileCommand::MergeFileCommand(){	
	try {
		abort = true; calledHelp = true; 
		setParameters();
		vector<string> tempOutNames;
		outputTypes["merge"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "MergeFileCommand", "MergeFileCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

MergeFileCommand::MergeFileCommand(string option)  {
	try {
		abort = false; calledHelp = false;   
		
		if(option == "help") {
			help();
			abort = true; calledHelp = true;
		}else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		else {
			vector<string> myArray = setParameters();
			
			OptionParser parser(option);
			map<string,string> parameters = parser.getParameters();
			
			ValidParameters validParameter;
			
			//check to make sure all parameters are valid for command
			for (map<string,string>::iterator it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["merge"] = tempOutNames;
			
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.valid(parameters, "inputdir");		
			if (inputDir == "not found"){	inputDir = "";		}
			
			string fileList = validParameter.valid(parameters, "input");
			if(fileList == "not found") { m->mothurOut("you must enter two or more file names"); m->mothurOutEndLine();  abort=true;  }
			else{ 	util.splitAtDash(fileList, fileNames);	}
			
			//if the user changes the output directory command factory will send this info to us in the output parameter 
			string outputDir = validParameter.valid(parameters, "outputdir");		if (outputDir == "not found")	{	outputDir = "";		}
			
			
			numInputFiles = fileNames.size();
			ifstream testFile;
			if(numInputFiles == 0){
				m->mothurOut("you must enter two or more file names and you entered " + toString(fileNames.size()) +  " file names"); m->mothurOutEndLine();
				abort=true;  
			}
			else{
				for(int i=0;i<numInputFiles;i++){
					if (inputDir != "") {
						string path = util.hasPath(fileNames[i]);
						//if the user has not given a path then, add inputdir. else leave path alone.
						if (path == "") {	fileNames[i] = inputDir + fileNames[i];		}
					}
					
                    if (util.checkLocations(fileNames[i], current->getLocations())) { }
                    else { fileNames.erase(fileNames.begin()+i); i--; } //erase from file list
				}
			}   
			
			outputFileName = validParameter.valid(parameters, "output");			
			if (outputFileName == "not found") { m->mothurOut("you must enter an output file name"); m->mothurOutEndLine();  abort=true;  }
			else if (outputDir != "") { outputFileName = outputDir + util.getSimpleName(outputFileName);  }
		}
			
	}
	catch(exception& e) {
		m->errorOut(e, "MergeFileCommand", "MergeFileCommand");
		exit(1);
	}
}
//**********************************************************************************************************************

int MergeFileCommand::execute(){
	try {
		if (abort) { if (calledHelp) { return 0; }  return 2;	}
		
		util.mothurRemove(outputFileName);
		for(int i=0;i<numInputFiles;i++){  util.appendFiles(fileNames[i], outputFileName);  }
		
		if (m->getControl_pressed()) {  util.mothurRemove(outputFileName); return 0;  }
		
		m->mothurOut("\nOutput File Names: \n"); 
		m->mothurOut(outputFileName); m->mothurOutEndLine();	outputNames.push_back(outputFileName); outputTypes["merge"].push_back(outputFileName);
		m->mothurOutEndLine();

		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "MergeFileCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************
