#pragma once

#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <cctype>
#include <regex>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include "class.h"

#define DEBUGNAME_OFFSET 3
#define NATIONALITY_OFFSET 7
#define MOTHERCOUNTRY_OFFSET 8
#define ACKNOW_UNIT_TYPE_OFFSET 9
#define TYPE_UNIT_FORMATION_OFFSET 10
#define NUMBER_OF_ACKTYPE 28

//using this struct to pass data related to user choices
struct params
{
    int old_e_value = -1;       //this is the radio button value, storing so we can check the previous frame value
    int old_e_value_ammo = -1;

    char str1[128]       = "";        
    char user_path[128]  = "";

    bool show_units                  = true;     //show the  units in the "selected units" window
    bool show_ammo                   = true;
    bool modify_units                = false;        //set to true when the user press "modify selected units"
    bool modify_ammo                 = false;
    bool show_unit_results           = false;  //set to true to see the result of the search bar
    bool show_ammunition_results     = false;
    bool opticalList                 = false;   

    
    bool *checkboxes_allUnits;          //this array is used to know if a unit is selected in the 'all' tree node
    std::vector<Unit*> unitsToModify;   //this vector store the units that are going to be modify
    std::vector<Weapon*> ammunitionToModify;

    std::vector<Unit*> search_unit_results;  //this vector store the results of the search bar as pointers
    std::vector<Weapon*> search_ammunition_results;  //this vector store the results of the search bar as pointers


    enum { VALID, EMPTY_STRING, PATH_NOT_FOUND, FILES_MISSING } status = EMPTY_STRING; 
};
//using this struct to store data to pass to the write function
typedef struct unit_data
{
    int new_cp, new_speed,new_fuel, new_maxSpeed;
    float new_fuelTime,new_optical_strenght, new_speedBonus;
}unit_data_t;
typedef struct ammo_data 
{
    float new_PhysicalDamages;
}ammo_data_t;
//used to store paths
typedef struct settings
{
    std::string original_path, new_path;
    std::string ud_original_path, ud_new_path;
    std::string am_original_path, am_new_path;
    FILE* hNew_file;
}settings_t;
namespace GUI
{
    void displayUnits(std::string filter, int unitcount, std::vector<Unit*> unit_vector[], int* counter, params* inputs);
    void displayTreeNode(std::string ack[], int unitcount, std::vector<Unit*> unit_vector[], int* counter, params* inputs);
    std::vector<Unit*> returnSelectedUnits(std::vector<Unit*> unit_vector[], params* inputs);
    std::vector<Weapon*> returnSelectedAmmo(std::vector<Weapon*> unit_vector[], params* inputs);
    void unitWindow(int unitcount, std::vector<Unit*> unit_vector[], std::vector<Weapon*> weapon_vector[], params* user_inputs, settings_t* settings, bool* x_button);
    void unitSelectedWindow(params* user_inputs,settings_t* settings);
    void ammoSelectedWindow(params* user_inputs, settings_t* settings);
    void updateStatsView(params* user_inputs, int indexToSkip);
    void showSearchResults(std::vector<Unit*> units, std::vector<Unit*>  user_inputs);
    void showSearchResults(std::vector<Weapon*> ammunition, std::vector<Weapon*> ammunitionToMod);
    void navBarButtons(std::vector<Unit*> unit_vector[], std::vector<Weapon*> weapon_vector[], params* user_inputs);
    void directoryWindow(params* user_inputs, bool* x_button);
}
// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int getDataFromFile(std::vector<Unit*> unit_vector[],std::vector<Weapon*> weapons_vector[], settings_t* settings);
void readLine(std::string line, int* line_counter,int* line_counter_all, int* units_counter, std::vector<Unit*> unit_vector[]);
void readLine(std::string original_line, int* line_counter_relative, int* line_counter, int* weapon_counter, std::vector<Weapon*> weapon_vector[]);
bool writeData(params* modPtrs, unit_data_t* unit_data, ammo_data_t* ammo_data, settings_t settings, bool isUnit);
std::vector<Unit*> searchUnit(std::string str, std::vector<Unit*> unit_vector[]);
std::vector<Weapon*> searchUnit(std::string str, std::vector<Weapon*> weapon_vector[]);
bool is_file_exist(const char* fileName);
bool isPathExist(const std::string& s);
bool checkFiles(std::string s);
bool checkDirectoryTxt();