#pragma once
class Ammo {
private:

public:
	std::string name, family, ProjectileType;
	int startLineNumber;
	float physicalDamages;
	int physicalDamagesLineOffset;
	int ammo_type_id; //refer to the string array in unitWindow()
	int max_range,max_range_offset;
	int min_range,min_range_offset;

	float suppressDamage;
	int suppressDamageOffset;

	int radiusSuppressDamage, radiusSuppressDamageOffset;
	int ShotsBeforeMaxNoise, ShotsBeforeMaxNoiseOffset;

	float NoiseDissimulationMalus;
	int NoiseDissimulationMalusOffset;

	float TempsDeVisee;
	int TempsDeViseeOffset;

	float TempsEntreDeuxSalves;
	int TempsEntreDeuxSalvesOffset;

	int numberOfWeapons;

	bool isSelected = false;

	Ammo() {}
	Ammo(std::string _name) : name(_name) {}
};

class Weapons {
private:

public:
	std::vector<Ammo*> ammos;
	Weapons() {}
};
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

	Weapons* guns;

	Unit() {}
	Unit(std::string _name) : name(_name) {}
	~Unit() {}
	
};
