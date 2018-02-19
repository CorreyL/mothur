/*
 *  phylodiversitycommand.cpp
 *  Mothur
 *
 *  Created by westcott on 4/30/10.
 *  Copyright 2010 Schloss Lab. All rights reserved.
 *
 */

#include "phylodiversitycommand.h"
#include "treereader.h"

//**********************************************************************************************************************
vector<string> PhyloDiversityCommand::setParameters(){	
	try {

		CommandParameter ptree("tree", "InputTypes", "", "", "none", "none", "none","phylodiv",false,true,true); parameters.push_back(ptree);
        CommandParameter pname("name", "InputTypes", "", "", "NameCount", "none", "none","",false,false,true); parameters.push_back(pname);
        CommandParameter pcount("count", "InputTypes", "", "", "NameCount-CountGroup", "none", "none","",false,false,true); parameters.push_back(pcount);
		CommandParameter pgroup("group", "InputTypes", "", "", "CountGroup", "none", "none","",false,false,true); parameters.push_back(pgroup);
		CommandParameter pgroups("groups", "String", "", "", "", "", "","",false,false); parameters.push_back(pgroups);
		CommandParameter piters("iters", "Number", "", "1000", "", "", "","",false,false); parameters.push_back(piters);
		CommandParameter pfreq("freq", "Number", "", "100", "", "", "","",false,false); parameters.push_back(pfreq);
		CommandParameter pprocessors("processors", "Number", "", "1", "", "", "","",false,false,true); parameters.push_back(pprocessors);
		CommandParameter prarefy("rarefy", "Boolean", "", "F", "", "", "","rarefy",false,false); parameters.push_back(prarefy);
        CommandParameter psubsample("sampledepth", "Number", "", "0", "", "", "","",false,false); parameters.push_back(psubsample);
		CommandParameter psummary("summary", "Boolean", "", "T", "", "", "","summary",false,false); parameters.push_back(psummary);
		CommandParameter pcollect("collect", "Boolean", "", "F", "", "", "","",false,false); parameters.push_back(pcollect);
		CommandParameter pscale("scale", "Boolean", "", "F", "", "", "","",false,false); parameters.push_back(pscale);
		CommandParameter pseed("seed", "Number", "", "0", "", "", "","",false,false); parameters.push_back(pseed);
        CommandParameter pinputdir("inputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(pinputdir);
		CommandParameter poutputdir("outputdir", "String", "", "", "", "", "","",false,false); parameters.push_back(poutputdir);
		
		vector<string> myArray;
		for (int i = 0; i < parameters.size(); i++) {	myArray.push_back(parameters[i].name);		}
		return myArray;
	}
	catch(exception& e) {
		m->errorOut(e, "PhyloDiversityCommand", "setParameters");
		exit(1);
	}
}
//**********************************************************************************************************************
string PhyloDiversityCommand::getHelpString(){	
	try {
		string helpString = "";
		helpString += "The phylo.diversity command parameters are tree, group, name, count, groups, iters, freq, processors, scale, rarefy, collect and summary.  tree and group are required, unless you have valid current files.\n";
		helpString += "The groups parameter allows you to specify which of the groups in your groupfile you would like analyzed. The group names are separated by dashes. By default all groups are used.\n";
		helpString += "The iters parameter allows you to specify the number of randomizations to preform, by default iters=1000, if you set rarefy to true.\n";
		helpString += "The freq parameter is used indicate when to output your data, by default it is set to 100. But you can set it to a percentage of the number of sequence. For example freq=0.10, means 10%. \n";
        helpString += "The sampledepth parameter allows you to enter the number of sequences you want to sample.\n";
		helpString += "The scale parameter is used indicate that you want your output scaled to the number of sequences sampled, default = false. \n";
		helpString += "The rarefy parameter allows you to create a rarefaction curve. The default is false.\n";
		helpString += "The collect parameter allows you to create a collectors curve. The default is false.\n";
		helpString += "The summary parameter allows you to create a .summary file. The default is true.\n";
		helpString += "The processors parameter allows you to specify the number of processors to use. The default is 1.\n";
		helpString += "The phylo.diversity command should be in the following format: phylo.diversity(groups=yourGroups, rarefy=yourRarefy, iters=yourIters).\n";
		helpString += "Example phylo.diversity(groups=A-B-C, rarefy=T, iters=500).\n";
		helpString += "The phylo.diversity command output two files: .phylo.diversity and if rarefy=T, .rarefaction.\n";
		helpString += "Note: No spaces between parameter labels (i.e. groups), '=' and parameters (i.e.yourGroups).\n";
		return helpString;
	}
	catch(exception& e) {
		m->errorOut(e, "PhyloDiversityCommand", "getHelpString");
		exit(1);
	}
}
//**********************************************************************************************************************
string PhyloDiversityCommand::getOutputPattern(string type) {
    try {
        string pattern = "";
        
        if (type == "phylodiv") {  pattern = "[filename],[tag],phylodiv"; } 
        else if (type == "rarefy") {  pattern = "[filename],[tag],phylodiv.rarefaction"; } 
        else if (type == "summary") {  pattern = "[filename],[tag],phylodiv.summary"; }
        else { m->mothurOut("[ERROR]: No definition for type " + type + " output pattern.\n"); m->setControl_pressed(true);  }
        
        return pattern;
    }
    catch(exception& e) {
        m->errorOut(e, "PhyloDiversityCommand", "getOutputPattern");
        exit(1);
    }
}

//**********************************************************************************************************************
PhyloDiversityCommand::PhyloDiversityCommand(){	
	try {
		abort = true; calledHelp = true; 
		setParameters();
		vector<string> tempOutNames;
		outputTypes["phylodiv"] = tempOutNames;
		outputTypes["rarefy"] = tempOutNames;
		outputTypes["summary"] = tempOutNames;
	}
	catch(exception& e) {
		m->errorOut(e, "PhyloDiversityCommand", "PhyloDiversityCommand");
		exit(1);
	}
}
//**********************************************************************************************************************
PhyloDiversityCommand::PhyloDiversityCommand(string option)  {
	try {
		abort = false; calledHelp = false;   
		
		//allow user to run help
		if(option == "help") { help(); abort = true; calledHelp = true; }
		else if(option == "citation") { citation(); abort = true; calledHelp = true;}
		
		else {
			vector<string> myArray = setParameters();;
			
			OptionParser parser(option);
			map<string,string> parameters = parser.getParameters();
			map<string,string>::iterator it;
			
			ValidParameters validParameter;
		
			//check to make sure all parameters are valid for command
			for (map<string,string>::iterator it = parameters.begin(); it != parameters.end(); it++) { 
				if (validParameter.isValidParameter(it->first, myArray, it->second) != true) {  abort = true;  }
			}
			
			//initialize outputTypes
			vector<string> tempOutNames;
			outputTypes["phylodiv"] = tempOutNames;
			outputTypes["rarefy"] = tempOutNames;
			outputTypes["summary"] = tempOutNames;
			
			//if the user changes the input directory command factory will send this info to us in the output parameter 
			string inputDir = validParameter.valid(parameters, "inputdir");		
			if (inputDir == "not found"){	inputDir = "";		}
			else {
				string path;
				it = parameters.find("tree");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["tree"] = inputDir + it->second;		}
				}
				
				it = parameters.find("group");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["group"] = inputDir + it->second;		}
				}
				
				it = parameters.find("name");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["name"] = inputDir + it->second;		}
				}
                
                it = parameters.find("count");
				//user has given a template file
				if(it != parameters.end()){ 
					path = util.hasPath(it->second);
					//if the user has not given a path then, add inputdir. else leave path alone.
					if (path == "") {	parameters["count"] = inputDir + it->second;		}
				}
			}
			
			//check for required parameters
			treefile = validParameter.validFile(parameters, "tree");
			if (treefile == "not open") { treefile = ""; abort = true; }
			else if (treefile == "not found") { 				
				//if there is a current design file, use it
				treefile = current->getTreeFile(); 
				if (treefile != "") { m->mothurOut("Using " + treefile + " as input file for the tree parameter."); m->mothurOutEndLine(); }
				else { 	m->mothurOut("You have no current tree file and the tree parameter is required."); m->mothurOutEndLine(); abort = true; }								
			}else { current->setTreeFile(treefile); }	
			
			//check for required parameters
			groupfile = validParameter.validFile(parameters, "group");
			if (groupfile == "not open") { groupfile = ""; abort = true; }
			else if (groupfile == "not found") { groupfile = ""; }
			else { current->setGroupFile(groupfile); }
			
			namefile = validParameter.validFile(parameters, "name");
			if (namefile == "not open") { namefile = ""; abort = true; }
			else if (namefile == "not found") { namefile = ""; }
			else { current->setNameFile(namefile); }
			
            countfile = validParameter.validFile(parameters, "count");
			if (countfile == "not open") { countfile = ""; abort = true; }
			else if (countfile == "not found") { countfile = "";  }	
			else { current->setCountFile(countfile); }
            
            if ((namefile != "") && (countfile != "")) {
                m->mothurOut("[ERROR]: you may only use one of the following: name or count."); m->mothurOutEndLine(); abort = true;
            }
			
            if ((groupfile != "") && (countfile != "")) {
                m->mothurOut("[ERROR]: you may only use one of the following: group or count."); m->mothurOutEndLine(); abort=true;
            }

			outputDir = validParameter.valid(parameters, "outputdir");		if (outputDir == "not found"){	outputDir = util.hasPath(treefile);	}
			
			string temp;
			temp = validParameter.valid(parameters, "freq");			if (temp == "not found") { temp = "100"; }
			util.mothurConvert(temp, freq);
			
			temp = validParameter.valid(parameters, "rarefy");			if (temp == "not found") { temp = "F"; }
			rarefy = util.isTrue(temp);
            
            temp = validParameter.valid(parameters, "sampledepth");		if (temp == "not found") { temp = "0"; }
            if (util.isNumeric1(temp)) {
                util.mothurConvert(temp, subsampleSize);
                if (subsampleSize == 0) { subsample = false; }
                else { subsample = true; }
            }else {
                subsample = false;
                m->mothurOut("[ERROR]: sampledepth must be numeric, aborting.\n"); m->mothurOutEndLine(); abort=true;
            }
            if (subsample) { rarefy = true;  }
            
            temp = validParameter.valid(parameters, "iters");			if (temp == "not found") { temp = "1000"; }
            util.mothurConvert(temp, iters);
            if (!rarefy) { iters = 1;  }
			
			temp = validParameter.valid(parameters, "summary");			if (temp == "not found") { temp = "T"; }
			summary = util.isTrue(temp);
			
			temp = validParameter.valid(parameters, "scale");			if (temp == "not found") { temp = "F"; }
			scale = util.isTrue(temp);
			
			temp = validParameter.valid(parameters, "collect");			if (temp == "not found") { temp = "F"; }
			collect = util.isTrue(temp);
			
			temp = validParameter.valid(parameters, "processors");	if (temp == "not found"){	temp = current->getProcessors();	}
			processors = current->setProcessors(temp);
            
			groups = validParameter.valid(parameters, "groups");			
			if (groups == "not found") { groups = "";  }
			else { 
				util.splitAtDash(groups, Groups);
                if (Groups.size() != 0) { if (Groups[0]== "all") { Groups.clear(); } }
			}
			
			if ((!collect) && (!rarefy) && (!summary)) { m->mothurOut("No outputs selected. You must set either collect, rarefy or summary to true, summary=T by default."); m->mothurOutEndLine(); abort=true; }
			
			if (countfile=="") {
                if (namefile == "") {
                    vector<string> files; files.push_back(treefile);
                    if (!current->getMothurCalling())  {  parser.getNameFile(files);  }
                } 
            }
		}
		
	}
	catch(exception& e) {
		m->errorOut(e, "PhyloDiversityCommand", "PhyloDiversityCommand");
		exit(1);
	}			
}
//**********************************************************************************************************************

int PhyloDiversityCommand::execute(){
	try {
		
		if (abort) { if (calledHelp) { return 0; }  return 2;	}
		
        long start = time(NULL);
        
		current->setTreeFile(treefile);
        TreeReader* reader;
        if (countfile == "") { reader = new TreeReader(treefile, groupfile, namefile); }
        else { reader = new TreeReader(treefile, countfile); }
        vector<Tree*> trees = reader->getTrees();
        ct = trees[0]->getCountTable();
        delete reader;

		vector<string> tGroups = ct->getNamesOfGroups();
        if (Groups.size() == 0) { Groups = tGroups; }
        else {
            //check that groups are valid
            for (int i = 0; i < Groups.size(); i++) {
                if (!util.inUsersGroups(Groups[i], tGroups)) {
                    m->mothurOut(Groups[i] + " is not a valid group, and will be disregarded."); m->mothurOutEndLine();
                    // erase the invalid group from userGroups
                    Groups.erase(Groups.begin()+i);
                    i--;
                }
            }
        }
		//incase the user had some mismatches between the tree and group files we don't want group xxx to be analyzed
		for (int i = 0; i < Groups.size(); i++) {  if (Groups[i] == "xxx") { Groups.erase(Groups.begin()+i);  break; }  }
		 
		vector<string> outputNames;
		
		//for each of the users trees
		for(int i = 0; i < trees.size(); i++) {
		
			if (m->getControl_pressed()) { delete ct; for (int j = 0; j < trees.size(); j++) { delete trees[j]; } for (int j = 0; j < outputNames.size(); j++) {	util.mothurRemove(outputNames[j]); 	} return 0; }
			
			ofstream outSum, outRare, outCollect;
            map<string, string> variables; 
            variables["[filename]"] = outputDir + util.getRootName(util.getSimpleName(treefile));
            variables["[tag]"] = toString(i+1);
			string outSumFile = getOutputFileName("summary",variables);
			string outRareFile = getOutputFileName("rarefy",variables);
			string outCollectFile = getOutputFileName("phylodiv",variables);
			
			if (summary)	{ util.openOutputFile(outSumFile, outSum); outputNames.push_back(outSumFile);		outputTypes["summary"].push_back(outSumFile);			}
			if (rarefy)		{ util.openOutputFile(outRareFile, outRare); outputNames.push_back(outRareFile);	outputTypes["rarefy"].push_back(outRareFile);			}
			if (collect)	{ util.openOutputFile(outCollectFile, outCollect); outputNames.push_back(outCollectFile);	 outputTypes["phylodiv"].push_back(outCollectFile);  }
			
			int numLeafNodes = trees[i]->getNumLeaves();
            
			//create a vector containing indexes of leaf nodes, randomize it, select nodes to send to calculator
			vector<int> randomLeaf;
			for (int j = 0; j < numLeafNodes; j++) {  
				if (util.inUsersGroups(trees[i]->tree[j].getGroup(), Groups) ) { //is this a node from the group the user selected.
					randomLeaf.push_back(j); 
				}
			}
            
			numLeafNodes = randomLeaf.size();  //reset the number of leaf nodes you are using 
			
			//each group, each sampling, if no rarefy iters = 1;
			map<string, vector<float> > diversity;
			
			//each group, each sampling, if no rarefy iters = 1;
			map<string, vector<float> > sumDiversity;
			
			//find largest group total 
			int largestGroup = 0;
			for (int j = 0; j < Groups.size(); j++) {
                int numSeqsThisGroup = ct->getGroupCount(Groups[j]);
                if (numSeqsThisGroup > largestGroup) { largestGroup = numSeqsThisGroup; }
				
                //initialize diversity
                diversity[Groups[j]].resize(numSeqsThisGroup+1, 0.0);		//numSampled
																											//groupA		0.0			0.0
                //initialize sumDiversity
                sumDiversity[Groups[j]].resize(numSeqsThisGroup+1, 0.0);
			}

			//convert freq percentage to number
            if (subsample) {  largestGroup = subsampleSize;  }
			int increment = 100;
			if (freq < 1.0) {  increment = largestGroup * freq;  
			}else { increment = freq;  }
			
			//initialize sampling spots
			set<int> numSampledList;
			for(int k = 1; k <= largestGroup; k++){  if((k == 1) || (k % increment == 0)){  numSampledList.insert(k); }   }
			if(largestGroup % increment != 0){	numSampledList.insert(largestGroup);   }
			
			//add other groups ending points
            if (!subsample) {
                for (int j = 0; j < Groups.size(); j++) {
                    if (numSampledList.count(diversity[Groups[j]].size()-1) == 0) {  numSampledList.insert(diversity[Groups[j]].size()-1); }
                }
            }
			
            if (rarefy) {
                vector<int> procIters;
                int numItersPerProcessor = iters / processors;
                
                //divide iters between processes
                for (int h = 0; h < processors; h++) {
                    if(h == processors - 1){ numItersPerProcessor = iters - h * numItersPerProcessor; }
                    procIters.push_back(numItersPerProcessor);
                }
                
                createProcesses(procIters, trees[i], diversity, sumDiversity, iters, increment, randomLeaf, numSampledList, outCollect, outSum);
                
            }else{ //no need to paralellize if you dont want to rarefy
                driver(trees[i], diversity, sumDiversity, iters, increment, randomLeaf, numSampledList, outCollect, outSum, true);	
            }
				
			if (rarefy) {	printData(numSampledList, sumDiversity, outRare, iters);	}
		}
		
	
		if (m->getControl_pressed()) { for (int i = 0; i < outputNames.size(); i++) {	util.mothurRemove(outputNames[i]); 	} return 0; }

        m->mothurOut("It took " + toString(time(NULL) - start) + " secs to run phylo.diversity.\n");

        
		m->mothurOut("\nOutput File Names: \n"); 
		for (int i = 0; i < outputNames.size(); i++) {	m->mothurOut(outputNames[i] +"\n"); 	} m->mothurOutEndLine();

		
		return 0;
	}
	catch(exception& e) {
		m->errorOut(e, "PhyloDiversityCommand", "execute");
		exit(1);
	}
}
//**********************************************************************************************************************
void PhyloDiversityCommand::printSumData(map< string, vector<float> >& div, ofstream& out, int numIters){
    try {
        
        out << "Groups\tnumSampled\tphyloDiversity" << endl;
        
        out.setf(ios::fixed, ios::floatfield); out.setf(ios::showpoint);
        
        int numSampled = 0;
        for (int j = 0; j < Groups.size(); j++) {
            if (subsample) { numSampled = subsampleSize; }
            else {  numSampled = (div[Groups[j]].size()-1);  }
            
            out << Groups[j] << '\t' << numSampled << '\t';
            
            float score;
            if (scale)	{  score = (div[Groups[j]][numSampled] / (float)numIters) / (float)numSampled;	}
            else		{	score = div[Groups[j]][numSampled] / (float)numIters;	}
            
            out << setprecision(4) << score << endl;
        }
        
        out.close();
        
    }
    catch(exception& e) {
        m->errorOut(e, "PhyloDiversityCommand", "printSumData");
        exit(1);
    }
}
//**********************************************************************************************************************

void PhyloDiversityCommand::printData(set<int>& num, map< string, vector<float> >& div, ofstream& out, int numIters){
    try {
        
        out << "numSampled";
        for (int i = 0; i < Groups.size(); i++) { out << '\t' << Groups[i];  }
        out << endl;
        
        out.setf(ios::fixed, ios::floatfield); out.setf(ios::showpoint);
        
        for (set<int>::iterator it = num.begin(); it != num.end(); it++) {
            int numSampled = *it;
            
            out << numSampled;
            
            for (int j = 0; j < Groups.size(); j++) {
                if (numSampled < div[Groups[j]].size()) {
                    float score;
                    if (scale)	{  score = (div[Groups[j]][numSampled] / (float)numIters) / (float)numSampled;	}
                    else		{	score = div[Groups[j]][numSampled] / (float)numIters;	}
                    
                    out << '\t' << setprecision(4) << score ;
                }else { out << "\tNA" ; }
            }
            out << endl;
        }
        
        out.close();
        
    }
    catch(exception& e) {
        m->errorOut(e, "PhyloDiversityCommand", "printData");
        exit(1);
    }
}

//**********************************************************************************************************************
int PhyloDiversityCommand::createProcesses(vector<int>& procIters, Tree* t, map< string, vector<float> >& div, map<string, vector<float> >& sumDiv, int numIters, int increment, vector<int>& randomLeaf, set<int>& numSampledList, ofstream& outCollect, ofstream& outSum){
	try {
        int process = 1;
		
		vector<int> processIDS;
		map< string, vector<float> >::iterator itSum;
        vector<string> Treenames = t->getTreeNames();

		#if defined (__APPLE__) || (__MACH__) || (linux) || (__linux) || (__linux__) || (__unix__) || (__unix)
				
		//loop through and create all the processes you want
		while (process != processors) {
			pid_t pid = fork();
			
			if (pid > 0) {
				processIDS.push_back(pid);  //create map from line number to pid so you can append files in correct order later
				process++;
			}else if (pid == 0){
				driver(t, div, sumDiv, procIters[process], increment, randomLeaf, numSampledList, outCollect, outSum, false);
				
				string outTemp = outputDir + toString(process) + ".sumDiv.temp";
				ofstream out;
				util.openOutputFile(outTemp, out);
				
				//output the sumDIversity
				for (itSum = sumDiv.begin(); itSum != sumDiv.end(); itSum++) {
					out << itSum->first << '\t' << (itSum->second).size() << '\t';
					for (int k = 0; k < (itSum->second).size(); k++) { 
						out << (itSum->second)[k] << '\t';
					}
					out << endl;
				}
				
				out.close();
				
				exit(0);
			}
		}
		
        
		driver(t, div, sumDiv, procIters[0], increment, randomLeaf, numSampledList, outCollect, outSum, true);
		
		//force parent to wait until all the processes are done
		for (int i=0;i<(processors-1);i++) { 
			int temp = processIDS[i];
			wait(&temp);
		}
		
		//get data created by processes
		for (int i=0;i<(processors-1);i++) { 
			
			//input the sumDIversity
			string inTemp = outputDir + toString(processIDS[i]) + ".sumDiv.temp";
			ifstream in;
			util.openInputFile(inTemp, in);
				
			//output the sumDIversity
			for (int j = 0; j < sumDiv.size(); j++) { 
				string group = "";
				int size = 0;
				
				in >> group >> size; util.gobble(in);
				
				for (int k = 0; k < size; k++) { 
					float tempVal;
					in >> tempVal;
					
					sumDiv[group][k] += tempVal;
				}
				util.gobble(in);
			}
				
			in.close();
			util.mothurRemove(inTemp);
		}
#else
        
        //fill in functions
        vector<phylodivData*> pDataArray;
		DWORD   dwThreadIdArray[processors-1];
		HANDLE  hThreadArray[processors-1];
        vector<CountTable*> cts;
        vector<Tree*> trees;
        map<string, int> rootForGroup = getRootForGroups(t);
		
		//Create processor worker threads.
		for( int i=1; i<processors; i++ ){
            CountTable* copyCount = new CountTable();
            copyCount->copy(ct);
            Tree* copyTree = new Tree(copyCount, Treenames);
            copyTree->getCopy(t);
            
            cts.push_back(copyCount);
            trees.push_back(copyTree);
            
            map<string, vector<float> > copydiv = div;
            map<string, vector<float> > copysumDiv = sumDiv;
            vector<int> copyrandomLeaf = randomLeaf;
            set<int> copynumSampledList = numSampledList;
            map<string, int> copyRootForGrouping = rootForGroup;
            
            phylodivData* temp = new phylodivData(m, procIters[i], copydiv, copysumDiv, copyTree, copyCount, increment, copyrandomLeaf, copynumSampledList, copyRootForGrouping, subsample, subsampleSize, Groups);
			pDataArray.push_back(temp);
			processIDS.push_back(i);
            
			hThreadArray[i-1] = CreateThread(NULL, 0, MyPhyloDivThreadFunction, pDataArray[i-1], 0, &dwThreadIdArray[i-1]);
		}
		
		driver(t, div, sumDiv, procIters[0], increment, randomLeaf, numSampledList, outCollect, outSum, true);
		
		//Wait until all threads have terminated.
		WaitForMultipleObjects(processors-1, hThreadArray, TRUE, INFINITE);
		
		//Close all thread handles and free memory allocations.
		for(int i=0; i < pDataArray.size(); i++){
            for (itSum = pDataArray[i]->sumDiv.begin(); itSum != pDataArray[i]->sumDiv.end(); itSum++) {
                for (int k = 0; k < (itSum->second).size(); k++) {
                    sumDiv[itSum->first][k] += (itSum->second)[k];
                }
            }
			delete cts[i];
            delete trees[i];
			CloseHandle(hThreadArray[i]);
			delete pDataArray[i];
		}
		
#endif

	return 0;		
	
	}
	catch(exception& e) {
		m->errorOut(e, "PhyloDiversityCommand", "createProcesses");
		exit(1);
	}
}
//**********************************************************************************************************************
int PhyloDiversityCommand::driver(Tree* t, map< string, vector<float> >& div, map<string, vector<float> >& sumDiv, int numIters, int increment, vector<int>& randomLeaf, set<int>& numSampledList, ofstream& outCollect, ofstream& outSum, bool doSumCollect){
	try {
		int numLeafNodes = randomLeaf.size();
        
        map<string, int> rootForGroup = getRootForGroups(t); //maps groupName to root node in tree. "root" for group may not be the trees root and we don't want to include the extra branches.
        
        
		for (int l = 0; l < numIters; l++) {
            util.mothurRandomShuffle(randomLeaf);
            
            //initialize counts
            map<string, int> counts;
            vector< map<string, bool> > countedBranch;
            for (int i = 0; i < t->getNumNodes(); i++) {
                map<string, bool> temp;
                for (int j = 0; j < Groups.size(); j++) { temp[Groups[j]] = false; }
                countedBranch.push_back(temp);
            }
            
            for (int j = 0; j < Groups.size(); j++) {  counts[Groups[j]] = 0;   }
            
            map<string, int> metCount; bool allDone = false;
            for (int j = 0; j < Groups.size(); j++) {  counts[Groups[j]] = false;   }
            for(int k = 0; k < numLeafNodes; k++){
                
                if (m->getControl_pressed()) { return 0; }
                
                //calc branch length of randomLeaf k
                vector<float> br = calcBranchLength(t, randomLeaf[k], countedBranch, rootForGroup);
                
                //for each group in the groups update the total branch length accounting for the names file
                vector<string> groups = t->tree[randomLeaf[k]].getGroup();
                
                for (int j = 0; j < groups.size(); j++) {
                    
                    if (util.inUsersGroups(groups[j], Groups)) {
                        int numSeqsInGroupJ = 0;
                        map<string, int>::iterator it;
                        it = t->tree[randomLeaf[k]].pcount.find(groups[j]);
                        if (it != t->tree[randomLeaf[k]].pcount.end()) { //this leaf node contains seqs from group j
                            numSeqsInGroupJ = it->second;
                        }
                        
                        if (numSeqsInGroupJ != 0) {	div[groups[j]][(counts[groups[j]]+1)] = div[groups[j]][counts[groups[j]]] + br[j];  }
                        
                        for (int s = (counts[groups[j]]+2); s <= (counts[groups[j]]+numSeqsInGroupJ); s++) {
                            div[groups[j]][s] = div[groups[j]][s-1];  //update counts, but don't add in redundant branch lengths
                        }
                        counts[groups[j]] += numSeqsInGroupJ;
                        if (subsample) {
                            if (counts[groups[j]] >= subsampleSize) { metCount[groups[j]] = true; }
                            bool allTrue = true;
                            for (int h = 0; h < Groups.size(); h++) {
                                if (!metCount[Groups[h]]) { allTrue = false; }
                            }
                            if (allTrue) { allDone = true; }
                        }
                        if (allDone) { j+=groups.size(); k+=numLeafNodes; }
                    }
                }
            }
            
            //if you subsample then rarefy=t
            if (rarefy) {
                //add this diversity to the sum
                for (int j = 0; j < Groups.size(); j++) {
                    for (int g = 0; g < div[Groups[j]].size(); g++) {
                        sumDiv[Groups[j]][g] += div[Groups[j]][g];
                    }
                }
            }
            
            if ((collect) && (l == 0) && doSumCollect) {  printData(numSampledList, div, outCollect, 1);  }
            if ((summary) && (l == 0) && doSumCollect) {  printSumData(div, outSum, 1);  }
        }
        
        return 0;

	}
	catch(exception& e) {
		m->errorOut(e, "PhyloDiversityCommand", "driver");
		exit(1);
	}
}

//**********************************************************************************************************************
//**********************************************************************************************************************
//need a vector of floats one branch length for every group the node represents.
vector<float> PhyloDiversityCommand::calcBranchLength(Tree* t, int leaf, vector< map<string, bool> >& counted, map<string, int> roots){
	try {
        
		//calc the branch length
		//while you aren't at root
		vector<float> sums; 
		int index = leaf;
		
		vector<string> groups = t->tree[leaf].getGroup();
		sums.resize(groups.size(), 0.0);
		
        
        //you are a leaf
		if(t->tree[index].getBranchLength() != -1){	
			for (int k = 0; k < groups.size(); k++) { 
                sums[k] += abs(t->tree[index].getBranchLength());	
			}
		}
        
        
        index = t->tree[index].getParent();	
        
		//while you aren't at root
		while(t->tree[index].getParent() != -1){
            
			if (m->getControl_pressed()) {  return sums; }
			
			for (int k = 0; k < groups.size(); k++) {
                
                if (index >= roots[groups[k]]) { counted[index][groups[k]] = true; } //if you are at this groups "root", then say we are done
                
                if (!counted[index][groups[k]]){ //if counted[index][groups[k] is true this groups has already added all br from here to root, so quit early
                    if (t->tree[index].getBranchLength() != -1) {
                        sums[k] += abs(t->tree[index].getBranchLength());
                    }
                    counted[index][groups[k]] = true;
                }
            }
            index = t->tree[index].getParent();	
        }
        
		return sums;
        
	}
	catch(exception& e) {
		m->errorOut(e, "PhyloDiversityCommand", "calcBranchLength");
		exit(1);
	}
}
//**********************************************************************************************************************
map<string, int> PhyloDiversityCommand::getRootForGroups(Tree* t){
    try {
        map<string, int> roots; //maps group to root for group, may not be root of tree
        map<string, bool> done;
        
        //initialize root for all groups to -1
        for (int k = 0; k < (t->getCountTable())->getNamesOfGroups().size(); k++) { done[(t->getCountTable())->getNamesOfGroups()[k]] = false; }
        
        for (int i = 0; i < t->getNumLeaves(); i++) {
            
            vector<string> groups = t->tree[i].getGroup();
            
            int index = t->tree[i].getParent();
            
            for (int j = 0; j < groups.size(); j++) {
                
                if (done[groups[j]] == false) { //we haven't found the root for this group yet, initialize it
                    done[groups[j]] = true;
                    roots[groups[j]] = i; //set root to self to start
                }
                
                //while you aren't at root
                while(t->tree[index].getParent() != -1){
                    
                    if (m->getControl_pressed()) {  return roots; }
                    
                    //do both your chidren have have descendants from the users groups?
                    int lc = t->tree[index].getLChild();
                    int rc = t->tree[index].getRChild();
                    
                    int LpcountSize = 0;
                    map<string, int>:: iterator itGroup = t->tree[lc].pcount.find(groups[j]);
                    if (itGroup != t->tree[lc].pcount.end()) { LpcountSize++;  }
                    
                    int RpcountSize = 0;
                    itGroup = t->tree[rc].pcount.find(groups[j]);
                    if (itGroup != t->tree[rc].pcount.end()) { RpcountSize++;  }
                    
                    if ((LpcountSize != 0) && (RpcountSize != 0)) { //possible root
                        if (index > roots[groups[j]]) {  roots[groups[j]] = index; }
                    }else { ;}
                    
                    index = t->tree[index].getParent();
                }
            }
        }

        return roots;
        
    }
    catch(exception& e) {
        m->errorOut(e, "PhyloDiversityCommand", "getRootForGroups");
        exit(1);
    }
}
//**********************************************************************************************************************



