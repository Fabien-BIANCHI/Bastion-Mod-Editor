#include "header.h"
#include <fstream>
//check if a file exist using the path or name if in the same dir
bool is_file_exist(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}
/// <summary>
/// this function read data and stores it in a big array
/// </summary>
/// <param name="array"></param>
/// <returns></returns>
int getDataFromFile(std::vector<Unit*> unit_vector[], settings_t* settings) {

	char buffer[256];					//store the line read by fgets(), don't go under 200 some lines are long and it will cause issues for the line counters variables
	int line_counter_relative = 1;		//count line but is relative to the export line of the unit	
	int line_counter = 1;				//count all lines from the start to the end
	int units_counter = 0;				//count the unit
	
	
	/*
	1- checking if directories.txt is present (file containing the path to the mod)
	2- completing the path hardcoded (not user related so its ok) with the original file (UniteDescriptor.ndf) and a new one (UniteDescriptor2.txt)
	3- making a copy of the original file to the path of the new one (here the same dir)
	4- editing the new txt file (need txt to open)
	5- removing the original file
	6- renaming the new file with the name of the original file
	*/
	bool is_directoriesFile = is_file_exist("directories.txt");
	if (!is_directoriesFile) {
		MessageBoxA(NULL, "Erreur, file directories not found", NULL, NULL);
		exit(1);
	}
	
	FILE* direct_f;
	fopen_s(&direct_f, "directories.txt", "r");
	if (direct_f) {
		
		if (fgets(buffer, sizeof(buffer), direct_f)) {
			settings->original_path = (std::string)buffer;
			settings->new_path = (std::string)buffer;
			settings->original_path.append("\\GameData\\Generated\\Gameplay\\Gfx\\UniteDescriptor.ndf");
			settings->new_path.append("\\GameData\\Generated\\Gameplay\\Gfx\\UniteDescriptor2.txt"); 
		}
	}
	else {
		MessageBoxA(NULL, "Erreur, while reading directories in getDataFromFile()", NULL, NULL);
		exit(1);
	}
	fclose(direct_f);


	if (!CopyFileA(settings->original_path.c_str(), settings->new_path.c_str(), false)) {	//true to not overwrite
		MessageBoxA(NULL, "Error while copying original ndf file", NULL, NULL);
		exit(1);
	}

	bool isUnitDesc_exist = is_file_exist(settings->new_path.c_str());
	if (!isUnitDesc_exist) {
		MessageBoxA(NULL, "Erreur, file unitDescriptor not found", NULL, NULL);
		exit(1);
	}

	
	fopen_s(&settings->hNew_file, settings->new_path.c_str(), "r");
	if (!settings->hNew_file) {
		MessageBox(NULL, "Error reading data from Unitdesc", NULL, NULL);
		exit(1);
	}
	
	std::string line;
	//main loop, read every line
	while (fgets(buffer, sizeof(buffer), settings->hNew_file) != NULL) {

		line = (std::string)buffer;
		readLine(line, &line_counter_relative,&line_counter, &units_counter, unit_vector);

		line_counter_relative++;
		line_counter++;
	}
	fclose(settings->hNew_file);
	
	return units_counter;
}
//delete the space(s) at the beginning of the string
std::string deleteSpacePrefix(std::string original){

	int i = 0;
	int j = 0;
	while (original[i] == ' ') {
		i++;
	}
	std::string new_string = original.substr(i,100);
	return new_string;
}
//read every line and checking if the line contains data that we need to store in our unit instances
void readLine(std::string original_line, int* line_counter_relative,int* line_counter, int* units_counter, std::vector<Unit*> unit_vector[]) {

	//removing the indentation
	Unit* current_unit = nullptr;
	std::string buffer = deleteSpacePrefix(original_line);
	//every time export is found in a string, we reset the line_counter (relative offset) and we create a new instance of the unit class
	if (strstr(original_line.c_str(), "export")) {
		*line_counter_relative = 0;

		current_unit = new Unit();
		if (current_unit) {
			//saving the base line to which we will had offset later to find attribute for the unit
			current_unit->exportLineNumber = *line_counter;
			unit_vector->push_back(current_unit);
			*units_counter += 1;
		}
	}
	if (*line_counter > 4) {

		Unit* current_unit = unit_vector->at((*units_counter) - 1); //-1 because the unit_counter start at 1
		if (!current_unit) 
			return;
		
		switch (*line_counter_relative) 	//at the start of a descriptor for a unit, its always the same so we can use fixed size offset
		{
		case DEBUGNAME_OFFSET: 
			buffer = buffer.substr(21, 100);	//cutting the string at index 21
			current_unit->name = buffer;		//storing into the unit
			return;
		case NATIONALITY_OFFSET:
			buffer = buffer.substr(48, 100);
			current_unit->nationality = buffer;
			return;
		case MOTHERCOUNTRY_OFFSET:
			buffer = buffer.substr(35, 100);
			current_unit->mother_country = buffer;
			return;
		case ACKNOW_UNIT_TYPE_OFFSET:
			buffer = buffer.substr(53, 100);
			current_unit->acknow_unit_type = buffer;
			return;
		case TYPE_UNIT_FORMATION_OFFSET:
			buffer = buffer.substr(35, 100);
			current_unit->type_unit_formation = buffer;
			return;
		default:
			break;
		}
		//the commands points is not at a fixed offset in the export, so we have to find it by looking for a substring
		//we extract the cost by converting from string to int and we keep the line offset for future purpose
		if (strstr(buffer.c_str(), "~/Resource_CommandPoints")) {
			int cp;
			buffer = buffer.substr(27, 4);	//on coupe le debut 
			std::istringstream iss(buffer); //convert string to int
			iss >> cp;
			current_unit->cost = cp;
			current_unit->costLineOffset = *line_counter_relative;
		}

		else if (strstr(buffer.c_str(), "FuelCapacity     = ")) {
			buffer = buffer.substr(19, 4);
			std::istringstream iss(buffer);
			iss >> current_unit->fuel;
			current_unit->fuelLineOffset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "FuelMoveDuration = ")) {
			buffer = buffer.substr(19, 4);
			std::istringstream iss(buffer);
			iss >> current_unit->fuelTime;
			current_unit->fuelTimeLineOffset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "MaxSpeed         =")) {
			int maxspeed;
			buffer = buffer.substr(19, 4);
			std::istringstream iss(buffer);
			iss >> maxspeed;
			current_unit->maxSpeed = maxspeed;
			current_unit->maxSpeedLineOffset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "SpeedBonusOnRoad =")) {
			int speedBonus;
			buffer = buffer.substr(19, 18);	//on coupe le debut, et on prend au maximum 17 chiffres du float
			float speedBonus_f = std::stof(buffer);; //convert string to float
			current_unit->speedBonus = speedBonus_f;
			current_unit->speedBonusLineOffset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "OpticalStrength = ")) {
			buffer = buffer.substr(18, 4);
			std::istringstream iss(buffer);
			iss >> current_unit->opticalStrenght;
			current_unit->opticalStrenghtLineOffset = *line_counter_relative;
		}
		else if (strstr(buffer.c_str(), "RealRoadSpeed =")) {
			buffer = buffer.substr(16, 4);
			std::istringstream iss(buffer);
			iss >> current_unit->realRoadSpeed;
			current_unit->realRoadSpeedLineOffset = *line_counter_relative;
		}
	}
}

