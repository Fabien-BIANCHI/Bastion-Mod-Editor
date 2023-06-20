#include "header.h"
#define BUFFER_SIZE 256

bool writeData(params* unitToMod,data_t data,settings_t settings) {

    /* File pointer to hold reference of input file */
    FILE* fPtr;
    FILE* fTemp;

    char buffer[BUFFER_SIZE];
    int  count;

    std::string s;
    std::string str;
    std::stringstream ss;

    /*  Open all required files */
    fopen_s(&fPtr, settings.new_path.c_str(), "r");
    fopen_s(&fTemp,"replace.tmp", "w");

    /* fopen() return NULL if unable to open file in given mode. */
    if (fPtr == NULL || fTemp == NULL)
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open file.\n");
        printf("Please check whether file exists and you have read/write privilege.\n");
        return false;
    }

    /*
    * Read line from source file and write to destination
    * file after replacing given line.
    */
    count = 1;
    bool hit = false;
    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL)
    {

        for (int i = 0; i < unitToMod->unitsToModify.size(); i++) {
            /* If current line is line to replace */
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->costLineOffset)) { //comand points
                s.clear(); str.clear(); ss.str(std::string());
                s = "                        (~/Resource_CommandPoints, ";
                ss << data.new_cp;
                str = ss.str();
                str.append("),\n");
                s.append(str);

                fputs(s.c_str(), fTemp);
                hit = true;
            }
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->fuelLineOffset)) {

                s.clear(); str.clear(); ss.str(std::string());
                s = "                    FuelCapacity     = ";
                ss << data.new_fuel;
                str = ss.str();
                str.append("\n");
                s.append(str);

                fputs(s.c_str(), fTemp);
                hit = true;
            }
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->fuelTimeLineOffset)) {

                s.clear(); str.clear(); ss.str(std::string());
                s = "                    FuelMoveDuration = ";
                ss << data.new_fuelTime;
                str = ss.str();
                str.append("\n");
                s.append(str);

                fputs(s.c_str(), fTemp);
                hit = true;
            }
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->maxSpeedLineOffset)) {

                s.clear(); str.clear(); ss.str(std::string());
                s = "               MaxSpeed         = ";
                ss << data.new_maxSpeed;
                str = ss.str();
                str.append("\n");
                s.append(str);

                fputs(s.c_str(), fTemp);
                hit = true;
            }
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->speedBonusLineOffset)) {

                s.clear(); str.clear(); ss.str(std::string());
                s = "                    SpeedBonusOnRoad = ";
                ss << data.new_speedBonus;
                str = ss.str();
                str.append("\n");
                s.append(str);
            }
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->opticalStrenghtLineOffset)) {

                s.clear(); str.clear(); ss.str(std::string());
                s = "                    OpticalStrength = ";
                ss << data.new_optical_strenght;
                str = ss.str();
                str.append("\n");
                s.append(str);

                fputs(s.c_str(), fTemp);
                hit = true;
                        s.append(str);

            }
        }
        if (!hit) {
            fputs(buffer, fTemp);
        }
       
        hit = false;
        count++;
    }

    /* Close all files to release resource */
    fclose(fPtr);
    fclose(fTemp);

    /* Delete original source file */
    remove(settings.new_path.c_str());

    /* Rename temporary file as original file */
    rename("replace.tmp", settings.new_path.c_str());

    return true; 
}