#pragma once

class Unit {
private:

public:
	int exportLineNumber;//first line of the unit in the file 

	bool isSelected = false;
	bool viewStats = false;

	std::string name,nationality,mother_country,acknow_unit_type,type_unit_formation;
	int cost,costLineOffset;
	int fuel, fuelTime, fuelLineOffset, fuelTimeLineOffset;
	int maxSpeed, maxSpeedLineOffset;
	float speedBonus;
	int speedBonusLineOffset;
	int opticalStrenghtLineOffset;
	float opticalStrenght;
	int realRoadSpeed;
	int realRoadSpeedLineOffset;

	Unit() {}
	Unit(std::string _name) : name(_name) {}
	~Unit() {}
	
};
class Weapon {
private:

public:
	std::string name,family, ProjectileType;
	int startLineNumber;
	float physicalDamages;
	int physicalDamagesLineOffset;
	int ammo_type_id; //refer to the string array in unitWindow()

	bool isSelected = false;
};
