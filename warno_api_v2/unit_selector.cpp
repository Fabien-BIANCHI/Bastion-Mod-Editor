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
void GUI::displayUnits(std::string filter, int unitcount, Unit* _array[],int* counter,params* inputs) {

    *counter = 0;
    for (int i = 0; i < unitcount; i++) {
        if (strcmp(filter.c_str(), "") == 0) { //all units
            ImGui::Checkbox(("%s", _array[i]->name.c_str()), &inputs->checkboxes_allUnits[i]);
            (*counter) += 1;
        }
        else if (strcmp(filter.c_str(), _array[i]->acknow_unit_type.c_str()) == 0) { //par filtre
            ImGui::Checkbox(("%s", _array[i]->name.c_str()),&_array[i]->isSelected);
            (*counter) += 1;
        }
    }
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "total count : %d", *counter);
}
void GUI::displayTreeNode(std::string ack[],int unitcount , Unit* _array[], int* counter,params* inputs) {

    for (int i = 0; i < NUMBER_OF_ACKTYPE; i++) {

        if (ImGui::TreeNode(ack[i].c_str())) { 
            ack[i].append("\n"); 
            ImGui::Separator();
            displayUnits(ack[i], unitcount, _array, counter,inputs);
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
std::vector<Unit*> GUI::returnSelectedUnits(Unit* _array[],params* inputs) {

    std::vector<Unit*> returnPtrs;

    for (int i = 0; i < NUMBER_OF_UNITS; i++) { //les sous categories
        if (_array[i] != nullptr) {
            if (_array[i]->isSelected) {
                returnPtrs.push_back(_array[i]);
            }
        }
    }
    for (int j = 0; j < NUMBER_OF_UNITS; j++) { //la sous categorie "all"
        if (inputs->checkboxes_allUnits[j]) {
            if (_array[j] != nullptr) {
                if (!isAlreadyPresent(returnPtrs, _array[j])) {
                    returnPtrs.push_back(_array[j]);
                }
            }
        }
    }
    return returnPtrs;
}
void GUI::navBarButtons(Unit* _array[], params* user_inputs) {
    if (ImGui::Button("search")) {
        user_inputs->search_results = searchUnit(user_inputs->str1, _array);
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
        for (int i = 0; i < NUMBER_OF_UNITS; i++) {
            _array[i]->isSelected = false;
        }
    }
   
    ImGui::SameLine();

    if (ImGui::Button("modify selected units")) {
        user_inputs->unitsToModify.clear();
        user_inputs->unitsToModify = returnSelectedUnits(_array, user_inputs);
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
void GUI::unitWindow(int unitcount, Unit* _array[], params* user_inputs,settings_t* settings, bool* x_button)
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
 
    navBarButtons(_array, user_inputs);
   
    if (user_inputs->search_results.size()) {
        if (user_inputs->show_results) {
            showSearchResults(user_inputs->search_results,user_inputs->unitsToModify);
        }
    }
    if (ImGui::CollapsingHeader(("Manual Search"))) {
        
        displayTreeNode(ack_type, unitcount, _array, &counter,user_inputs);
        
        if (ImGui::TreeNode("allUnits")) {

            displayUnits("", unitcount, _array, &counter,user_inputs);
            ImGui::TreePop();
        }
    }

    if (user_inputs->modify) {
        unitSelectedWindow(user_inputs,settings);
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
void GUI::unitSelectedWindow(params* user_inputs,settings_t* settings) {

    ImGui::Begin("Selected Units");

    static int cp = 0;
    static int speed = 0;
    static int e = 0; //holding info about which radio button is selected, starting at index 0
    static int fuel = 0;
    static float fuelTime = 0.f;
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
        opticalStrenght = user_inputs->unitsToModify[e]->opticalStrenght;

        user_inputs->old_e_value = e;
    }
    
    

    ImGui::Separator();
    ImGui::Text("Modifications : ");
    ImGui::Separator();

 

    ImGui::InputInt("command points", &cp);
    if (ImGui::CollapsingHeader("Fuel attributes")) {
        ImGui::InputInt("fuel capacity", &fuel);
        ImGui::InputFloat("fuel time", &fuelTime);
    }
    if(ImGui::CollapsingHeader("Optical"))
        ImGui::InputFloat("optical strenght", &opticalStrenght);
    
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
        data.new_optical_strenght = opticalStrenght;
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
