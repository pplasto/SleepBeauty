#ifndef GAME_MODEL_HH__HH
#define  GAME_MODEL_HH__HH

#include "GameGlobal.h"
#include "ScriptData.h"
#include "cocos2d.h"

USING_NS_CC;

namespace GAME_NAMESPACE
{
	class UserBeautyData
	{
	public:
		int gainHearts;
		// int buyGuide;
		// int buyTime;
		// int buySleep;
	};

	class GameModel
	{
	// Single Instance
	private:
		GameModel();
		~GameModel();
	public:
		static GameModel* getInstance();
		static void destoryInstance();

	private:
		int _currBeauty;
		BeautyScriptData* _currScript;

	private:// save
		UserBeautyData _userBeautyData[REAL_BEAUTY_NUM];
		
	public:
		void readScript( int level );
		BeautyScriptData* getCurrScript();
		UserBeautyData* getUserBeautyDatas(int index);
		void setUserBeautyData(int index, int heartNum);
		int getCurrBeauty();
		void setCurrBeauty(int index);
		int getEvilMarkNum();
		void saveGame();
		void loadSave();
		void unlockBeauty( int beautyTouched );
	};
}

#endif
