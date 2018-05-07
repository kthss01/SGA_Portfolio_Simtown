#pragma once

#include "GameNode.h"

//#define SUBWINOPEN 1

class MainGame : public GameNode
{
private:
	bool isDebug;

public:
	MainGame();
	~MainGame();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render() override;

	void LoadSimtownImg();
	void LoadTile();
	void LoadBuilding();
	void LoadHouses();
	void LoadCommunityBuildings();
	void LoadBusinesses();
	void LoadFunPlaces();
	void LoadUI();
};
