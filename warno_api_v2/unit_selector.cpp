#include "header.h"
/// <summary>
/// print the text under each sub categories (<=>tree node)
/// using a different array for the "allUnits tree" and all the others tree b
/// </summary>
/// <param name="filter"></param>
/// <param name="unitcount"></param>
/// <param name="_array"></param>
/// <param name="counter"></param>
/// <param name="inputs"></param>
void GUI::displayUnits(std::string filter, int unitcount, std::vector<Unit*> unit_vector[], int* counter, params* inputs) {

    *counter = 0;
    for (int i = 0; i < unitcount; i++) {
        if (strcmp(filter.c_str(), "") == 0) { //all units
            ImGui::Checkbox(("%s", unit_vector->at(i)->name.c_str()), &inputs->checkboxes_allUnits[i]);
            (*counter) += 1;
        }
        else if (strcmp(filter.c_str(), unit_vector->at(i)->acknow_unit_type.c_str()) == 0) { //par filtre
            ImGui::Checkbox(("%s", unit_vector->at(i)->name.c_str()),&unit_vector->at(i)->isSelected);
            (*counter) += 1;
        }
    }
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "total count : %d", *counter);
}
void GUI::displayTreeNode(std::string ack[],int unitcount ,std::vector<Unit*> unit_vector[], int* counter, params* inputs) {

    for (int i = 0; i < NUMBER_OF_ACKTYPE; i++) {

        if (ImGui::TreeNode(ack[i].c_str())) { 
            ack[i].append("\n"); 
            ImGui::Separator();
            displayUnits(ack[i], unitcount, unit_vector, counter,inputs);
            ImGui::TreePop();
        }
    }
}
/// <summary>
/// return true if the unit is already in the vector
/// </summary>
/// <param name="_vector"></param>
/// <param name="test"></param>
/// <returns></returns>
bool isAlreadyPresent(std::vector<Unit*> _vector,Unit* test) {

    int size = _vector.size();
    for (int i = 0; i < size; i++) {
        if (_vector[i] == test) {
            return true;
        }
    }
    return false;
}
/// <summary>
/// return an vector with pointers toward the units to mod
/// </summary>
/// <param name="_array"></param>
/// <param name="inputs"></param>
/// <returns></returns>
std::vector<Unit*> GUI::returnSelectedUnits(std::vector<Unit*> unit_vector[], params* inputs) {

    std::vector<Unit*> returnPtrs;

    for (int i = 0; i < unit_vector->size(); i++) { //les sous categories
        if (unit_vector->at(i) != nullptr) {
            if (unit_vector->at(i)->isSelected) {
                returnPtrs.push_back(unit_vector->at(i));
            }
        }
    }
    for (int j = 0; j < unit_vector->size(); j++) { //la sous categorie "all"
        if (inputs->checkboxes_allUnits[j]) {
            if (unit_vector->at(j) != nullptr) {
                if (!isAlreadyPresent(returnPtrs, unit_vector->at(j))) {
                    returnPtrs.push_back(unit_vector->at(j));
                }
            }
        }
    }
    return returnPtrs;
}
void GUI::navBarButtons(std::vector<Unit*> unit_vector[], params* user_inputs) {
    if (ImGui::Button("search")) {
        user_inputs->search_results = searchUnit(user_inputs->str1, unit_vector);
        user_inputs->show_results = true;
    }
    ImGui::SameLine();
    if (user_inputs->show_results && (user_inputs->search_results.size() > 0)) {
        if (ImGui::Button("hide results")) {
            user_inputs->show_results = !user_inputs->show_results;
        }
    }
    else if (!user_inputs->show_results && (user_inputs->search_results.size() > 0)) {
        if (ImGui::Button("show results")) {
            user_inputs->show_results = !user_inputs->show_results;
        }
    }
    ImGui::SameLine();

    if (ImGui::Button("Unselect all units")) {
        for (int i = 0; i < unit_vector->size(); i++) {
            unit_vector->at(i)->isSelected = false;
        }
    }
   
    ImGui::SameLine();

    if (ImGui::Button("modify selected units")) {
        user_inputs->unitsToModify.clear();
        user_inputs->unitsToModify = returnSelectedUnits(unit_vector, user_inputs);
        if (user_inputs->unitsToModify.size()) {
            user_inputs->modify = true;
            user_inputs->old_e_value = -1;
        }
        else {
            user_inputs->modify = false;
        }
    }
   
}
//main window
void GUI::unitWindow(int unitcount, std::vector<Unit*> unit_vector[], params* user_inputs, settings_t* settings, bool* x_button)
{
    int counter;
    std::string ack_type[NUMBER_OF_ACKTYPE]; //string array containing all "AcknowUnitType"
    
#pragma region ack_type def 
    ack_type[0] = "SAM";
    ack_type[1] = "CanonAA";
    ack_type[2] = "GunArtillery";
    ack_type[3] = "ArtShell";
    ack_type[4] = "Reco";
    ack_type[5] = "TankDestroyer";
    ack_type[6] = "Command";
    ack_type[7] = "Tank";
    ack_type[8] = "Transport";
    ack_type[9] = "TankDestroyerMissile";
    ack_type[10] = "Inf";
    ack_type[11] = "Inf2";
    ack_type[12] = "Engineer";
    ack_type[13] = "Recon_INF";
    ack_type[14] = "MLRS";
    ack_type[15] = "Recon_Vehicle";
    ack_type[16] = "CommandVehicle";
    ack_type[17] = "Logistic";
    ack_type[18] = "Command_Infantry";
    ack_type[19] = "Inf_Elite";
    ack_type[20] = "Inf_Militia";
    ack_type[21] = "KaJaPa";
    ack_type[22] = "HeliAttack";
    ack_type[23] = "HeliTransport";
    ack_type[24] = "GroundAtk";
    ack_type[25] = "AirSup";
    ack_type[26] = "Air_CAS";
    ack_type[27] = "Multirole";
#pragma endregion
    
    ImGui::Begin("Unit selector",x_button);
    
    ImGui::InputTextWithHint("##unitNameBox","search unit here", user_inputs->str1, IM_ARRAYSIZE(user_inputs->str1));
 
    navBarButtons(unit_vector, user_inputs);
   
    if (user_inputs->search_results.size()) {
        if (user_inputs->show_results) {
            showSearchResults(user_inputs->search_results,user_inputs->unitsToModify);
        }
    }
    if (ImGui::CollapsingHeader(("Manual Search"))) {
        
        displayTreeNode(ack_type, unitcount, unit_vector, &counter,user_inputs);
        
        if (ImGui::TreeNode("allUnits")) {

            displayUnits("", unitcount, unit_vector, &counter,user_inputs);
            ImGui::TreePop();
        }
    }

    if (user_inputs->modify) {
        unitSelectedWindow(user_inputs,settings, ack_type);
    }
    ImGui::End();
}
void GUI::updateStatsView(params* user_inputs,int indexToSkip) {

    int size = user_inputs->unitsToModify.size();
    for (int i = 0; i < size; i++) {
        if (i != indexToSkip) {
            user_inputs->unitsToModify[i]->viewStats = false;
        }
    }
}
//window opened after button "modify" is pressed
void GUI::unitSelectedWindow(params* user_inputs,settings_t* settings, std::string* ack_type) {

    ImGui::Begin("Selected Units");

    static int cp = 0;
    static int speed = 0;
    static int e = 0; //holding info about which radio button is selected, starting at index 0
    static int fuel = 0;
    static float fuelTime = 0.f;
    static int maxspeed = 0;
    static float speedBonus = 0.f;
    static float opticalStrenght = 0.f;
    static int realRoadSpeed = 0;
    int size = user_inputs->unitsToModify.size();

    //les types d'unités dont on va afficher leur speedbonusonroad et pouvoir les modifier
#pragma region speed_bonus_exists
    std::vector<std::string> speed_bonus_exists = {
        "SAM",
        "CanonAA",
        "GunArtillery",
        "ArtShell",
        "Reco",
        "TankDestroyer",
        "Command",
        "Tank",
        "Transport",
        "TankDestroyerMissile",
        "MLRS",
        "Recon_Vehicle",
        "CommandVehicle",
        "Logistic",
        "KaJaPa",
    };
#pragma endregion


    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "unitcount : %d", size);
    ImGui::Separator();
    ImGui::Checkbox("Show units", &user_inputs->show_units);
    ImGui::Text("select unit to show info about it");
    ImGui::Separator();

    
    
    std::string id = "##"; //https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-about-the-id-stack-system
    
    if (user_inputs->show_units) {
        for (int i = 0; i < size; i++) {

            id.append(user_inputs->unitsToModify[i]->name);
            ImGui::RadioButton(id.c_str(), &e, i);
            ImGui::SameLine();
            ImGui::Text("%s", user_inputs->unitsToModify[i]->name.c_str());
        }

    }

    if (e >= size) e = 0;

    if ((e < size) && (e != user_inputs->old_e_value)) {
        cp = user_inputs->unitsToModify[e]->cost; //index du radio button
        fuel = user_inputs->unitsToModify[e]->fuel;
        fuelTime = user_inputs->unitsToModify[e]->fuelTime;
        maxspeed = user_inputs->unitsToModify[e]->maxSpeed;
        speedBonus = user_inputs->unitsToModify[e]->speedBonus;
        opticalStrenght = user_inputs->unitsToModify[e]->opticalStrenght;
        realRoadSpeed = user_inputs->unitsToModify[e]->realRoadSpeed;

        user_inputs->old_e_value = e;
    }
    
    //Check if the highlighted unit contains speedBonusOnRoad by seeing if its type is in the array speed_bonus_exists[].
    //If the ack type drawer of the highlighted unit is dropped down, then 
    // "user_inputs->unitsToModify[e]->acknow_unit_type" will have "\n" at the end.
    //That's why we compare with the strings in speed_bonus_exists, and also with those strings with "\n" appended.
    bool unit_has_speed_bonus = false;
    for (int i = 0; i < speed_bonus_exists.size(); i++) {
        if (
            (user_inputs->unitsToModify[e]->acknow_unit_type == speed_bonus_exists[i])
            ||
            (user_inputs->unitsToModify[e]->acknow_unit_type == speed_bonus_exists[i].append("\n"))
            )
        {
            unit_has_speed_bonus = true;
            break;
        }
    }

    ImGui::Separator();
    ImGui::Text("Modifications : ");
    ImGui::Separator();

    ImGui::InputInt("command points", &cp);

    if (ImGui::CollapsingHeader("Fuel attributes")) {
        ImGui::InputInt("fuel capacity", &fuel);
        ImGui::InputFloat("fuel time", &fuelTime);
    }

    if (ImGui::CollapsingHeader("Speed Attributes"))
    {
        if (unit_has_speed_bonus)
        {
            ImGui::Checkbox("Auto", &user_inputs->is_auto_speed_bonus);
            ImGui::SameLine();
            if (ImGui::Button("Help"))
                ImGui::OpenPopup("auto_help");
#pragma region auto_help def
            const char help[] = "With auto enabled, speedBonusOnRoad is calculated in a way that\nthe on road speed remains the same independent of \nthe value of maxSpeed AKA the off road speed.\n\nFurthermore, changing realRoadSpeed (UI variable only) will increase \nor decrease speedBonusOnRoad in a way that the on road speed will change \nproportionally to the change made to RealRoadSpeed";
#pragma endregion

            if (ImGui::BeginPopup("auto_help"))
            {
                ImGui::Text(help);
                ImGui::EndPopup();
            }
            ImGui::InputInt("RealRoadSpeed", &realRoadSpeed);
        }
        ImGui::InputInt("MaxSpeed", &maxspeed);
        if (unit_has_speed_bonus)
        {
            int flag;
            char description[30];
            if (user_inputs->is_auto_speed_bonus) {
                flag = ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly;
                ImGui::Text("auto:");
                ImGui::SameLine();
                
                //maintain the same on road speed even if the off road speed (maxspeed) changes.
                //Formula: (oldMaxSpeed + (oldMaxSpeed*oldSpeedBonus)) = (newMaxSpeed + (newMaxSpeed * newSpeedBonus))
                int oldMaxSpeed = user_inputs->unitsToModify[e]->maxSpeed;
                float oldSpeedBonus = user_inputs->unitsToModify[e]->speedBonus;
                speedBonus = ((oldMaxSpeed + oldMaxSpeed * oldSpeedBonus) - maxspeed) / maxspeed;

                //Change the on road speed proportionally to the change of realRoadSpeed
                //Formula: (maxSpeed + (maxSpeed * oldSpeedBonus)) * (newRealRoadSpeed/oldRealRoadSpeed) = (maxSpeed + (maxSpeed * newSpeedBonus))
                //-> newSpeedBonus = (newRealRoadSpeed/oldRealRoadSpeed) * (1+oldSpeedBonus) - 1
                float ratio = static_cast<float>(realRoadSpeed) / static_cast<float>(user_inputs->unitsToModify[e]->realRoadSpeed);
                speedBonus = ratio * (1 + speedBonus) - 1;
            }
            else
                flag = ImGuiInputTextFlags_::ImGuiInputTextFlags_None;
            ImGui::InputFloat("SpeedBonusOnRoad", &speedBonus, (0.00F), (0.0F), "%.8f", flag);
        }
      
    }

    if (ImGui::CollapsingHeader("Optical")) {
        ImGui::InputFloat("optical strenght", &opticalStrenght);
    }
        
    
    ImGui::Separator();
    //converting int to string
    std::stringstream ss;
    ss << size;
    std::string str = ss.str();
    //reusing id variable for different purpose
    id = "update the ";
    id.append(str);
    id.append(" selected units");

    int g = 2; //green
    ImGui::PushID(g);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(g / 7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(g / 7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(g / 7.0f, 0.8f, 0.8f));
    if (ImGui::Button(id.c_str())) {

        data_t data = { };
        data.new_cp = cp;
        data.new_speed = speed;
        data.new_fuel = fuel;
        data.new_fuelTime = fuelTime;
        data.new_maxSpeed = maxspeed;
        data.new_speedBonus = speedBonus;
        data.new_optical_strenght = opticalStrenght;
        data.new_realRoadSpeed = realRoadSpeed;
        writeData(user_inputs, data,*settings);
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::End();
}
//display the result and also add to the vector "unitToMod" if a unit is selected in the vector "units"
void GUI::showSearchResults(std::vector<Unit*> units, std::vector<Unit*> unitToMod) {

    int size = units.size();
    for (int i = 0; i < size; i++) {
        ImGui::Checkbox(("%s", units[i]->name).c_str(),&units[i]->isSelected);
        if (units[i]->isSelected) {

            if(!isAlreadyPresent(unitToMod,units[i]))
                unitToMod.push_back(units[i]);
        }
    }
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "unitcount : %d", size);

}
