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

	Unit() {}
	Unit(std::string _name) : name(_name) {}
	~Unit() {}
	/*
* dead code
*
//unit_id can be 0 for LOG, 1 for INF, 2 for ART, 3 for TNK, 4 for REC, 5 for DCA, 6 for HEL, 7 for AIR
Unit(int unit_id) {
	switch (unit_id)
	{
	case 0:
		isLOG = true;
		pLOG = new LOG();
		break;
	case 1:
		isINF = true;
		pINF = new INF();
		break;
	case 2:
		isART = true;
		pART = new ART();
		break;
	case 3:
		isTNK = true;
		pTNK = new TNK();
		break;
	case 4:
		isREC = true;
		pREC = new REC();
		break;
	case 5:
		isDCA = true;
		pDCA = new DCA();
		break;
	case 6:
		isHEL = true;
		pHEL = new HEL();
		break;
	case 7:
		isAIR = true;
		pAIR = new AIR();
		break;
	default:
		break;
	}
}
*/
};
class Weapon {
private:

public:

};
