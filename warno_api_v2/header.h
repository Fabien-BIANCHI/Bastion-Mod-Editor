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

#define NUMBER_OF_UNITS 828
#define DEBUGNAME_OFFSET 3
#define NATIONALITY_OFFSET 7
#define MOTHERCOUNTRY_OFFSET 8
#define ACKNOW_UNIT_TYPE_OFFSET 9
#define TYPE_UNIT_FORMATION_OFFSET 10
#define NUMBER_OF_ACKTYPE 28

//using this struct to pass data related to user choices
struct params
{
    int old_e_value = -1;

    char str1[128] = "";
   
    bool show_units = true;
    bool modify = false;
    bool show_results = false;

    //these 2 array are used to pass data from the unit selector window to the mod window
    bool checkboxes_allUnits[NUMBER_OF_UNITS];
    std::vector<Unit*> unitsToModify;
    std::vector<Unit*> search_results;

    char str2[128] = ""; //Toby
    enum { VALID, EMPTY_STRING, PATH_NOT_FOUND, FILES_MISSING } status = EMPTY_STRING; //Toby
};
//using this struct to store data to write 
struct data_t
{
    int new_cp, new_speed,new_fuel, new_maxSpeed;
    float new_fuelTime, new_speedBonus;
};
struct settings_t
{
    std::string original_path, new_path;
    FILE* hNew_file;
};
namespace GUI
{
    void displayUnits(std::string filter, int unitcount, Unit* _array[], int* counter, params* inputs);
    void displayTreeNode(std::string ack[], int unitcount, Unit* _array[], int* counter, params* inputs);
    std::vector<Unit*> returnSelectedUnits(Unit* _array[], params* inputs);
    void unitWindow(int unitcount, Unit* _array[], params* user_inputs, settings_t* settings,  bool* x_button);
    void unitSelectedWindow(params* user_inputs,settings_t* settings);
    void updateStatsView(params* user_inputs, int indexToSkip);
    void showSearchResults(std::vector<Unit*> units , std::vector<Unit*>  user_inputs);
    void navBarButtons(Unit* _array[], params* unitToMod);
    void directoryWindow(params* user_inputs, bool* x_button);
}
// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
int getDataFromFile(Unit* _array[], settings_t* settings);
void readLine(std::string line, int* line_counter,int* line_counter_all, int* units_counter, Unit* _array[]);
bool writeData(params* unitToMod, data_t data,settings_t settings);
std::vector<Unit*> searchUnit(std::string str, Unit* _array[]);
bool is_file_exist(const char* fileName);
bool isPathExist(const std::string& s);
bool checkFiles(std::string s);
bool checkDirectoryTxt();