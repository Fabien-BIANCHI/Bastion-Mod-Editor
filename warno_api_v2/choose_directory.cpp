//Window to choose the mod directory. 
//It should check if the necessary files exist, like UniteDescriptor.ndf .

#include "header.h"
#include <sys/types.h>
#include <sys/stat.h>

//Vérifier si le chemin existe
bool isPathExist(const std::string& s)
{
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
}

//Vérifie si il y a bien les fichiers ndf nécéssaires avec le bon chemin
bool checkFiles(std::string s)
{
    if (!isPathExist(s + "/GameData/Generated/Gameplay/Gfx/UniteDescriptor.ndf")) {}
    else if (!isPathExist(s + "/GameData/Generated/Gameplay/Gfx/Ammunition.ndf")) {}
    else return true;

    return false;

}

bool checkDirectoryTxt() {
    if (!isPathExist("directories.txt"))
    {
        return false;
    }
    FILE* file;
    fopen_s(&file, "directories.txt", "r");
    if (!file) { return false; }
    char buffer[256];
    std::string line;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line = (std::string)buffer;
    }
    if (isPathExist(line))
    {
        if (checkFiles(line) == true)
        {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    remove("directories.txt");
    return false;
}

void GUI::directoryWindow(params* user_inputs, bool* x_button) {
    if (checkDirectoryTxt())
    {
        user_inputs->status = params::VALID;
        return;
    }
   
    ImGui::Begin("Directory Window",x_button);
    ImGui::Text("Enter the path of your mod folder");
    ImGui::InputText("<-path", user_inputs->str2, IM_ARRAYSIZE(user_inputs->str2));

    if (ImGui::Button("Use Directory"))
    {
        const char* path = user_inputs->str2;
        std::string path_str = path;

        if (isPathExist(path_str)) //Le chemin existe. Est-ce que le répertoire contient bien les fichiers NDF?
        {
            if (checkFiles(path_str) == true) //Oui il les contient
            {
                user_inputs->status = params::VALID;
                FILE* directory_path;
                fopen_s(&directory_path, "directories.txt", "w");
                if (!directory_path) { return; }
                fputs(path, directory_path);
                fclose(directory_path);
            }
            else //Non il  ne les contient pas
            {
                user_inputs->status = params::FILES_MISSING;
            }
        }
        else if (path[0] == '\0') //Le champs est vide
            user_inputs->status = params::EMPTY_STRING;
        else if (!isPathExist(path))
        {
            user_inputs->status = params::PATH_NOT_FOUND; //Le chemin n'existe pas
            printf("ERROR: Directory not found\n");
        }

    }
    int status = user_inputs->status;
    if (status == params::EMPTY_STRING)
        ImGui::Text("Enter a directory path");
    else if (status == params::PATH_NOT_FOUND)
        ImGui::Text("ERROR: Directory not found");
    else if (status == params::FILES_MISSING)
        ImGui::Text("ERROR: Missing files in Mod Folder");


    
    ImGui::End();
}
