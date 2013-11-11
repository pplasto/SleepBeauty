#include "GameModel.h"

namespace GAME_NAMESPACE
{
	static GameModel* _instance = NULL;

	GameModel::GameModel()
		:_currScript(NULL)
		,_currBeauty(-1)
	{
		_userBeautyData[0].gainHearts = 0;

		for(int i = 1; i < REAL_BEAUTY_NUM; i++)
		{
			_userBeautyData[i].gainHearts = -1;
		}
		loadSave();
	}

	GameModel::~GameModel()
	{
		if(_currScript) delete _currScript;
	}

	GameModel* GameModel::getInstance()
	{
		if(_instance)return _instance;
		return _instance = new GameModel();
	}

	void GameModel::destoryInstance()
	{
		delete _instance;
		_instance = NULL;
	}

	void GameModel::readScript(int level)
	{
		_currBeauty = level;
		if(_currScript)
		{
			if( _currScript->level == level ) 
			{ // if current script level is the same with setting level , return
				return;
			} else {
				delete _currScript;
				_currScript = NULL;
			}
		}
		char path[64];
		sprintf(path,"level/beauty_%d.js",level);
		//sprintf(path,"level/beauty_.js");
		unsigned long size;
		char* data = reinterpret_cast<char*>(CCFileUtils::sharedFileUtils()->getFileData(
			CCFileUtils::sharedFileUtils()->fullPathForFilename(path).c_str(), "r", &size));
		_currScript = BeautyScriptData::createFormJsonData( level, data );
		free(data);
	}

	BeautyScriptData* GameModel::getCurrScript()
	{
		return _currScript;
	}

	void GameModel::setUserBeautyData(int index, int heartNum)
	{
		_userBeautyData[index].gainHearts = heartNum;
	}

	UserBeautyData* GameModel::getUserBeautyDatas( int index )
	{
		return &_userBeautyData[index];
	}

	int GameModel::getCurrBeauty()
	{
		return _currBeauty;
	}

	int GameModel::getEvilMarkNum()
	{
		int n = 0;
		for( int i = 0 ; i < REAL_BEAUTY_NUM; i++ )
		{
			if (_userBeautyData[i].gainHearts == 5)
			{
				n++;
			}
		}
		return n;
	}

	void GameModel::loadSave()
	{
		//CCData *data = CCUserDefault::sharedUserDefault()->setIntegerForKey();
		//string path = CCFileUtils::sharedFileUtils()->getWritablePath() + string("gamesave");

		//this->_currBeauty = -1;
		//if( true )
		//{
		//	unsigned char* bytes = data->getBytes();
		//	for(int i = 0; i<REAL_BEAUTY_NUM; i++)
		//	{
		//		int hearts =  (bytes[i] == 255)?-1:bytes[i];
		//		this->_userBeautyData[i].gainHearts = hearts;
		//		if( hearts == -1 && _currBeauty < 0 )
		//		{
		//			this->_currBeauty = i - 1;
		//		}
		//	}
		

		std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + std::string("gamesave");
		FILE* file = fopen(path.c_str(), "r");

		if (file) 
		{
			char buf[REAL_BEAUTY_NUM];
			//fseek(file, 0, SEEK_END);
			int len = REAL_BEAUTY_NUM;//ftell(file);
			//rewind(file);
			int rLen = fread(buf, sizeof(char), sizeof(char)*len, file);
			fclose(file);

			for(int i = 0; i<REAL_BEAUTY_NUM; i++)
			{
				int hearts =  (int)buf[i] - 2;
				this->_userBeautyData[i].gainHearts = hearts;
				if( hearts == -1 && _currBeauty < 0 )
				{
					this->_currBeauty = i - 1;
				}
			}
		}
	}

	void GameModel::saveGame()
	{
		std::string path = CCFileUtils::sharedFileUtils()->getWritablePath() + std::string("gamesave");
		FILE* file = fopen(path.c_str(), "w");
		if (file) 
		{
			char pContent[REAL_BEAUTY_NUM+1];
			for( int i = 0; i<REAL_BEAUTY_NUM; i++ )
			{
				int valueHearts = _userBeautyData[i].gainHearts;
				char value = valueHearts + 2;
				pContent[i] = value;
			}
			pContent[REAL_BEAUTY_NUM] = 0;
			fputs(pContent, file);
			fclose(file);  
		}  
		else  
			CCLOG("save file error.");  

		//return;
		/*unsigned char data[REAL_BEAUTY_NUM];
		for( int i = 0; i<REAL_BEAUTY_NUM; i++ )
		{
		int valueHearts = _userBeautyData[i].gainHearts;
		unsigned char value = valueHearts<0?255:valueHearts;
		data[i] = value;
		}
		UserDefault::getInstance()->setDataForKey("save0", Data::create(data,REAL_BEAUTY_NUM));*/
	}

	void GameModel::setCurrBeauty( int index )
	{
		if( index<REAL_BEAUTY_NUM && index >= 0 )
			_currBeauty = index;
	}

	void GameModel::unlockBeauty( int beautyTouched )
	{
		// TODO 
		_userBeautyData[beautyTouched].gainHearts = 0;
		//throw std::exception("The method or operation is not implemented.");
	}

}