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
//when calling, use the correct type according to the bool (last parametre)
bool writeData(params* modPtrs,unit_data_t* unit_data,ammo_data_t* ammo_data,settings_t settings,bool isUnit) {

    /* File pointer to hold reference of input file */
    FILE* fPtr;
    FILE* fTemp;

    char buffer[BUFFER_SIZE];
    int  count;

    if (isUnit) {
        if (unit_data) {
            settings.original_path = settings.ud_original_path;
            settings.new_path = settings.ud_new_path;
        }
        else {
            MessageBoxA(NULL, "isUnit is set to true but unit_data is set to null", NULL, NULL);
            exit(1);
        }
    }
    else {
        if (ammo_data) {
            settings.original_path = settings.am_original_path;
            settings.new_path = settings.am_new_path;
        }
        else {
            MessageBoxA(NULL, "isUnit is set to false but ammo_data is set to null", NULL, NULL);
            exit(1);
        }
    }

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
        if (isUnit) {
            for (int i = 0; i < modPtrs->unitsToModify.size(); i++) {
                /* If current line is line to replace */
                if(modPtrs->unitsToModify[i]->costLineOffset != 0)
                    if (count == (modPtrs->unitsToModify[i]->exportLineNumber + modPtrs->unitsToModify[i]->costLineOffset)) { //comand points
                    
                        fputs(customStr("                        (~/Resource_CommandPoints, ", &unit_data->new_cp, NULL).append("),\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->unitsToModify[i]->fuelLineOffset != 0)
                    if (count == (modPtrs->unitsToModify[i]->exportLineNumber + modPtrs->unitsToModify[i]->fuelLineOffset)) {

                        fputs(customStr("                    FuelCapacity     = ", &unit_data->new_fuel, NULL).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->unitsToModify[i]->fuelTimeLineOffset != 0)
                    if (count == (modPtrs->unitsToModify[i]->exportLineNumber + modPtrs->unitsToModify[i]->fuelTimeLineOffset)) {

                        fputs(customStr("                    FuelMoveDuration = ", NULL, &unit_data->new_fuelTime).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->unitsToModify[i]->maxSpeedLineOffset != 0)
                    if (count == (modPtrs->unitsToModify[i]->exportLineNumber + modPtrs->unitsToModify[i]->maxSpeedLineOffset)) {

                        fputs(customStr("               MaxSpeed         = ", &unit_data->new_maxSpeed, NULL).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->unitsToModify[i]->speedBonusLineOffset != 0)
                    if (count == (modPtrs->unitsToModify[i]->exportLineNumber + modPtrs->unitsToModify[i]->speedBonusLineOffset)) {

                        fputs(customStr("                    SpeedBonusOnRoad = ", NULL, &unit_data->new_speedBonus).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->unitsToModify[i]->opticalStrenghtLineOffset != 0)
                    if (count == (modPtrs->unitsToModify[i]->exportLineNumber + modPtrs->unitsToModify[i]->opticalStrenghtLineOffset)) {

                        fputs(customStr("                    OpticalStrength = ", NULL, &unit_data->new_optical_strenght).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->unitsToModify[i]->realRoadSpeedLineOffset != 0)
                    if (count == (modPtrs->unitsToModify[i]->exportLineNumber + modPtrs->unitsToModify[i]->realRoadSpeedLineOffset)) {

                        fputs(customStr("                RealRoadSpeed = ", &unit_data->new_realRoadSpeed, NULL).append("\n").c_str(), fTemp);
                        hit = true;
                    }
            }
            if (!hit) {
                fputs(buffer, fTemp);
            }
        }
        else //ammo
        {
         
            for (int j = 0; j < modPtrs->ammunitionToModify.size(); j++) {

                if(modPtrs->ammunitionToModify[j]->physicalDamagesLineOffset != 0)
                    if (count == (modPtrs->ammunitionToModify[j]->startLineNumber + modPtrs->ammunitionToModify[j]->physicalDamagesLineOffset)) {
                        fputs(customStr("    PhysicalDamages                   = ", NULL, &ammo_data->new_PhysicalDamages).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->ammunitionToModify[j]->max_range_offset != 0)
                    if (count == (modPtrs->ammunitionToModify[j]->startLineNumber + modPtrs->ammunitionToModify[j]->max_range_offset)) {
                        fputs(customStr("    PorteeMaximale                    = ((", &ammo_data->new_max_range, NULL).append(") * Metre)\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->ammunitionToModify[j]->min_range_offset != 0)
                    if (count == (modPtrs->ammunitionToModify[j]->startLineNumber + modPtrs->ammunitionToModify[j]->min_range_offset)) {
                        fputs(customStr("    PorteeMinimale                    = ((", &ammo_data->new_min_range, NULL).append(") * Metre)\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->ammunitionToModify[j]->suppressDamageOffset != 0)
                    if (count == (modPtrs->ammunitionToModify[j]->startLineNumber + modPtrs->ammunitionToModify[j]->suppressDamageOffset)) {
                        fputs(customStr("    SuppressDamages                   = ", NULL, &ammo_data->suppressDamage).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->ammunitionToModify[j]->radiusSuppressDamageOffset != 0)
                    if (count == (modPtrs->ammunitionToModify[j]->startLineNumber + modPtrs->ammunitionToModify[j]->radiusSuppressDamageOffset)) {
                        fputs(customStr("    RadiusSplashSuppressDamages       = ", &ammo_data->suppressDamageRadius, NULL).append(" * Metre\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->ammunitionToModify[j]->NoiseDissimulationMalusOffset != 0)
                    if (count == (modPtrs->ammunitionToModify[j]->startLineNumber + modPtrs->ammunitionToModify[j]->NoiseDissimulationMalusOffset)) {
                        fputs(customStr("    NoiseDissimulationMalus           = ", NULL, &ammo_data->new_NoiseDissimulationMalus).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->ammunitionToModify[j]->ShotsBeforeMaxNoiseOffset != 0)
                    if (count == (modPtrs->ammunitionToModify[j]->startLineNumber + modPtrs->ammunitionToModify[j]->ShotsBeforeMaxNoiseOffset)) {
                        fputs(customStr("    ShotsBeforeMaxNoise               = ", &ammo_data->new_ShotsBeforeMaxNoise, NULL).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->ammunitionToModify[j]->TempsDeViseeOffset != 0)
                    if (count == (modPtrs->ammunitionToModify[j]->startLineNumber + modPtrs->ammunitionToModify[j]->TempsDeViseeOffset)) {
                        fputs(customStr("    TempsDeVisee                      = ", NULL, &ammo_data->new_TempsDeVisee).append("\n").c_str(), fTemp);
                        hit = true;
                    }
                if (modPtrs->ammunitionToModify[j]->TempsEntreDeuxSalvesOffset != 0)
                    if (count == (modPtrs->ammunitionToModify[j]->startLineNumber + modPtrs->ammunitionToModify[j]->TempsEntreDeuxSalvesOffset)) {
                        fputs(customStr("    TempsEntreDeuxSalves              = ", NULL , &ammo_data->new_TempsEntreDeuxSalves).append("\n").c_str(), fTemp);
                        hit = true;
                    }
            }
            if (!hit) {
                fputs(buffer, fTemp);
            }
            
        }
        hit = false;
        count++;
    }

    /* Close all files to release resource */
    fclose(fPtr);
    fclose(fTemp);

    /* Delete original source file */
    if (remove(settings.new_path.c_str())) {
        MessageBox(NULL, "Fail deleting after writing", NULL, NULL);
    }

    /* Rename temporary file as original file */
    if (rename("replace.tmp", settings.new_path.c_str())) {
        MessageBox(NULL, "Fail renaming after writing", NULL, NULL);
    }

    return true; 
}