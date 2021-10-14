#pragma once
#include "Field.h"
#include "Cell.h"
#include "Engine.h"
#include <vector>

extern const int OpponentGameFieldW;
extern const int OpponentGameFieldH;

class EnemyField : public Field
{
public:
	EnemyField(int x, int y) :Field(x, y), DeployingShipID(0) {}
	bool Click(POINT& coordinates) override;
	POINT Select(const size_t, const size_t);
	POINT RandomSelect();
	POINT MoveSelection(int Direction);

	int ShipExists(const POINT Coordinates);
	void CreateShips(const Engine::GAMESTATUS);
	void CloseNextShip();
	
	void SetShipMarkers();
	void Draw() override;

	bool CanFire() override;
	int ShootRecieve(const POINT ShootCoordinates) override;
	POINT ShootCreate() override;
	void ShootAnswer(const int status) override;
private:
	void SetShipDeployableStatus(Ship& ship);
	void DeployEnemyShips();
	void ClearField();
	void CleanShips();
	void Deselect();
public:
	Cell Cells[OpponentGameFieldW][OpponentGameFieldH];
	Ship Ships[10];
private:
	unsigned int DeployingShipID;
private:
	class Opponent
	{
	public:
		Opponent()
		{
			for (int i = 0; i < OpponentGameFieldW; i++)
				for (int j = 0; j < OpponentGameFieldH; j++)
				{
					this->Field[i][j] = -2;
				}

			AssignShootingPoints(FourDeckShootingPoints, 4);
			AssignShootingPoints(ThreeDeckShootingPoints, 3);
			AssignShootingPoints(TwoDeckShootingPoints, 2);
			AssignShootingPoints(OneDeckShootingPoints, 1);

			this->strategy = this->Strategy::Fourdeckship;
			this->Ships.push_back(4);
			this->Ships.push_back(3);
			this->Ships.push_back(3);
			this->Ships.push_back(2);
			this->Ships.push_back(2);
			this->Ships.push_back(2);
			this->Ships.push_back(1);
			this->Ships.push_back(1);
			this->Ships.push_back(1);
			this->Ships.push_back(1);
		}
		enum Strategy { Damage, OneDeckShip, Twodeckship, Threedeckship, Fourdeckship, Unknown} strategy;
		std::vector<int> Ships;
		std::vector<POINT> TargetShip;
		int Field[UserGameFieldW][UserGameFieldH];
		bool TargetShipKilled;
		POINT AimPoint;
	public:
		bool In_Range(const POINT Coordinates);
		void SetStrategy(const Strategy strategy = Strategy::Unknown);
		void SetDamageAroundShip(const std::vector<POINT> KilledShip);
		void SetDamageAroundDeck(const POINT DamagedDeck);
		void TurnShipAround(std::vector<POINT> &ship);
		POINT RandShootingPoint(const std::vector<POINT> vec);
		void AdjustShootingPoints(std::vector<POINT>& Strategy);
		int VectorPointExists(std::vector<POINT> vec, POINT point);
	public:
		std::vector<POINT> FourDeckShootingPoints;
		std::vector<POINT> ThreeDeckShootingPoints;
		std::vector<POINT> TwoDeckShootingPoints;
		std::vector<POINT> OneDeckShootingPoints;
	private:
		void AssignShootingPoints(std::vector<POINT>& vec, const size_t size);

	} opponent;
};
