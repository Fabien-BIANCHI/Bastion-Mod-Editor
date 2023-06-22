#include "header.h"
#define BUFFER_SIZE 256
//int or float depending on the type, set the un-used param as NULL
std::string customStr(std::string text, int* intVal,float* floatVal) {

    if (intVal && floatVal) 
        MessageBoxA(NULL,"Error1 writing data",NULL,NULL);
    if(!intVal && !floatVal)
        MessageBoxA(NULL, "Error2 writing data", NULL, NULL);

    std::string s;
    std::string str;
    std::stringstream ss;

    if (intVal) {
        s = text;
        ss << *intVal;
        str = ss.str();
        s.append(str);
        return s;
    }
    else {
        s = text;
        ss << *floatVal;
        str = ss.str();
        s.append(str);
        return s;
    }
}

bool writeData(params* unitToMod,data_t data,settings_t settings) {

    /* File pointer to hold reference of input file */
    FILE* fPtr;
    FILE* fTemp;

    char buffer[BUFFER_SIZE];
    int  count;

   

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
                
                fputs(customStr("                        (~/Resource_CommandPoints, ", &data.new_cp,NULL).append("),\n").c_str(), fTemp);
                hit = true;
            }
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->fuelLineOffset)) {

          
                fputs(customStr("                    FuelCapacity     = ", &data.new_fuel,NULL).append("\n").c_str(), fTemp);
                hit = true;
            }
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->fuelTimeLineOffset)) {


                fputs(customStr("                    FuelMoveDuration = ", NULL, &data.new_fuelTime).append("\n").c_str(), fTemp);
                hit = true;
            }
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->maxSpeedLineOffset)) {

                fputs(customStr("               MaxSpeed         = ", &data.new_maxSpeed,NULL).append("\n").c_str(), fTemp);
                hit = true;
            }
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->speedBonusLineOffset)) {

                fputs(customStr("                    SpeedBonusOnRoad = ", NULL,&data.new_speedBonus).append("\n").c_str(), fTemp);
                hit = true;
            }
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->opticalStrenghtLineOffset)) {

                fputs(customStr("                    OpticalStrength = ", NULL,&data.new_optical_strenght).append("\n").c_str(), fTemp);
                hit = true;
            }
            if (count == (unitToMod->unitsToModify[i]->exportLineNumber + unitToMod->unitsToModify[i]->realRoadSpeedLineOffset)) {

                fputs(customStr("                RealRoadSpeed = ", &data.new_realRoadSpeed, NULL).append("\n").c_str(), fTemp);
                hit = true;
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