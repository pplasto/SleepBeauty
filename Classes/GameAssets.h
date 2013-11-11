#ifndef GAME_ASSETS_HH__HH
#define  GAME_ASSETS_HH__HH

#include "GameGlobal.h"
#include "cocos2d.h"

USING_NS_CC;

namespace GAME_NAMESPACE
{
	class GameAssetsManage
	{
	public:


	private:
		GameAssetsManage();
		~GameAssetsManage();
	public:
		static GameAssetsManage* getInstance();
		static void destoryInstance();
	};
}

#endif
