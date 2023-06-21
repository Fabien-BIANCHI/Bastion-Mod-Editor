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
bool isAlreadyPresent(std::vector<Weapon*> _vector, Weapon* test) {

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
std::vector<Weapon*> GUI::returnSelectedAmmo(std::vector<Weapon*> weapon_vector[], params* inputs) {

    std::vector<Weapon*> returnPtrs;

    for (int i = 0; i < weapon_vector->size(); i++) { //les sous categories
        if (weapon_vector->at(i) != nullptr) {
            if (weapon_vector->at(i)->isSelected) {
                returnPtrs.push_back(weapon_vector->at(i));
            }
        }
    }
    for (int j = 0; j < weapon_vector->size(); j++) { //la sous categorie "all"
        if (inputs->checkboxes_allUnits[j]) {
            if (weapon_vector->at(j) != nullptr) {
                if (!isAlreadyPresent(returnPtrs, weapon_vector->at(j))) {
                    returnPtrs.push_back(weapon_vector->at(j));
                }
            }
        }
    }
    return returnPtrs;
}
void GUI::navBarButtons(std::vector<Unit*> unit_vector[], std::vector<Weapon*> weapon_vector[], params* user_inputs) {
    if (ImGui::Button("search in Unit")) {
        user_inputs->search_unit_results = searchUnit(user_inputs->str1, unit_vector);

        user_inputs->show_unit_results = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("search in Ammunition")) {
        user_inputs->search_ammunition_results = searchUnit(user_inputs->str1, weapon_vector);
        user_inputs->show_ammunition_results = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("hide results")) {
        user_inputs->show_unit_results = false;
        user_inputs->show_ammunition_results = false;
    }
   
    ImGui::SameLine();

    if (ImGui::Button("Unselect all")) {
        for (int i = 0; i < unit_vector->size(); i++) {
            unit_vector->at(i)->isSelected = false;
        }
        for (int j = 0; j < weapon_vector->size(); j++) {
            weapon_vector->at(j)->isSelected = false;
        }
    }
   
    ImGui::SameLine();

    if (ImGui::Button("modify selected")) {
        user_inputs->unitsToModify.clear();
        user_inputs->ammunitionToModify.clear();

        user_inputs->unitsToModify = returnSelectedUnits(unit_vector, user_inputs);
        user_inputs->ammunitionToModify = returnSelectedAmmo(weapon_vector, user_inputs);
        if (user_inputs->unitsToModify.size()) {
            user_inputs->modify_units = true;
            user_inputs->old_e_value = -1;
        }
        else {
            user_inputs->modify_units = false;
        }
        if (user_inputs->ammunitionToModify.size()) {
            user_inputs->modify_ammo = true;
            user_inputs->old_e_value_ammo = -1;
        }
        else {
            user_inputs->modify_ammo = false;
        }
    }
   
}
//main window
void GUI::unitWindow(int unitcount, std::vector<Unit*> unit_vector[], std::vector<Weapon*> weapon_vector[], params* user_inputs, settings_t* settings, bool* x_button)
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
    
    ImGui::Begin("Selector",x_button);
    
    ImGui::InputTextWithHint("##unitNameBox","type here", user_inputs->str1, IM_ARRAYSIZE(user_inputs->str1));
 
    navBarButtons(unit_vector, weapon_vector, user_inputs);
   
    if (user_inputs->search_unit_results.size()) {
        if (user_inputs->show_unit_results) {
            showSearchResults(user_inputs->search_unit_results,user_inputs->unitsToModify);
        }
    }
    if (user_inputs->search_ammunition_results.size()) {
        if (user_inputs->show_ammunition_results) {
            showSearchResults(user_inputs->search_ammunition_results, user_inputs->ammunitionToModify);
        }
    }
    if (ImGui::CollapsingHeader(("Manual Search Units"))) {
        
        displayTreeNode(ack_type, unitcount, unit_vector, &counter,user_inputs);
        
        if (ImGui::TreeNode("allUnits")) {

            displayUnits("", unitcount, unit_vector, &counter,user_inputs);
            ImGui::TreePop();
        }
    }
    if (ImGui::CollapsingHeader(("Manual Search Ammo"))) {

    }
    if (user_inputs->modify_units) {
        unitSelectedWindow(user_inputs,settings);
    }
    if (user_inputs->modify_ammo) {
        ammoSelectedWindow(user_inputs, settings);
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
void GUI::ammoSelectedWindow(params* user_inputs,settings_t* settings) {

    ImGui::Begin("Selected Ammunitions");

    static int e = 0;
    static float physical_damage = 0.f;
    int size = user_inputs->ammunitionToModify.size();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "unitcount : %d",size );
    ImGui::Separator();
    ImGui::Checkbox("Show units", &user_inputs->show_units);
    ImGui::Text("select unit to show info about it");

    std::string id = "##"; //https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#q-about-the-id-stack-system

    if (user_inputs->show_ammo) {
        for (int i = 0; i < size; i++) {

            id.append(user_inputs->ammunitionToModify[i]->name);
            ImGui::RadioButton(id.c_str(), &e, i);
            ImGui::SameLine();
            ImGui::Text("%s", user_inputs->ammunitionToModify[i]->name.c_str());
        }

    }
    if ((e < size) && (e != user_inputs->old_e_value_ammo)) {
        //display current stats in the box
        physical_damage = user_inputs->ammunitionToModify[e]->physicalDamages;


        user_inputs->old_e_value_ammo = e;
    }
    ImGui::Separator();
    ImGui::Text("Modifications : ");
    ImGui::Separator();

    ImGui::InputFloat("PhysicalDamages", &physical_damage, (0.00F), (0.0F), "%.8f");
        

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

        ammo_data_t data = {};
        data.new_PhysicalDamages = physical_damage;

        writeData(user_inputs, nullptr, &data, *settings, false);
    }
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::Separator();
    ImGui::End();
}
//window opened after button "modify" is pressed
void GUI::unitSelectedWindow(params* user_inputs,settings_t* settings) {

    ImGui::Begin("Selected Units");

    static int cp = 0;
    static int speed = 0;
    static int e = 0; //holding info about which radio button is selected, starting at index 0
    static int fuel = 0;
    static float fuelTime = 0.f;
    static int maxspeed = 0;
    static float speedBonus = 0.f;
    static float opticalStrenght = 0.f;
    int size = user_inputs->unitsToModify.size();
    
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
    if ((e < size) && (e != user_inputs->old_e_value)) {
        cp = user_inputs->unitsToModify[e]->cost; //index du radio button
        fuel = user_inputs->unitsToModify[e]->fuel;
        fuelTime = user_inputs->unitsToModify[e]->fuelTime;
        maxspeed = user_inputs->unitsToModify[e]->maxSpeed;
        speedBonus = user_inputs->unitsToModify[e]->speedBonus;
        opticalStrenght = user_inputs->unitsToModify[e]->opticalStrenght;

        user_inputs->old_e_value = e;
    }
    
    ImGui::Separator();
    ImGui::Text("Modifications : ");
    ImGui::Separator();

    ImGui::InputInt("command points", &cp);


    if (ImGui::CollapsingHeader("Fuel attributes")) {
        ImGui::InputInt("fuel capacity", &fuel);
        ImGui::InputFloat("fuel time", &fuelTime, (0.00F), (0.0F), "%.8f");
    }

    if (ImGui::CollapsingHeader("Speed Attributes"))
    {
        ImGui::InputInt("MaxSpeed", &maxspeed);
        ImGui::InputFloat("SpeedBonusOnRoad", &speedBonus, (0.00F), (0.0F), "%.8f"); //no step value so no increment or decrement buttons. Shows 8 digits, sometimes what this displays is a bit off from the real value.
    }

    if (ImGui::CollapsingHeader("Optical")) {
        ImGui::InputFloat("optical strenght", &opticalStrenght, (0.00F), (0.0F), "%.8f");
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

        unit_data_t data = { };
        data.new_cp = cp;
        data.new_speed = speed;
        data.new_fuel = fuel;
        data.new_fuelTime = fuelTime;
        data.new_maxSpeed = maxspeed;
        data.new_speedBonus = speedBonus;
        data.new_optical_strenght = opticalStrenght;
        writeData(user_inputs, &data,nullptr,*settings,true);
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
void GUI::showSearchResults(std::vector<Weapon*> ammunition, std::vector<Weapon*> ammunitionToMod) {

    int size = ammunition.size();
 
    for (int i = 0; i < size; i++) {
        ImGui::Checkbox(("%s", ammunition[i]->name).c_str(), &ammunition[i]->isSelected);
        if (ammunition[i]->isSelected) {

            if (!isAlreadyPresent(ammunitionToMod, ammunition[i]))
                ammunitionToMod.push_back(ammunition[i]);
        }
    }
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "weaponcount : %d", size);
}