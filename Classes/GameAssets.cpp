#include "GameAssets.h"

namespace GAME_NAMESPACE
{
	static GameAssetsManage* _instance = NULL;

	GameAssetsManage::GameAssetsManage()
	{

	}

	GameAssetsManage::~GameAssetsManage()
	{

	}

	GameAssetsManage* GameAssetsManage::getInstance()
	{
		if(_instance)return _instance;
		return _instance = new GameAssetsManage();
	}

	void GameAssetsManage::destoryInstance()
	{
		delete _instance;
		_instance = NULL;
	}
}