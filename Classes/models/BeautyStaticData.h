#ifndef BEAUTYDATA_HH__HH
#define  BEAUTYDATA_HH__HH

#include <string>
#include "GameGlobal.h"

namespace GAME_NAMESPACE
{
	class BeautyStaticData
	{
	public:
		BeautyStaticData();
		~BeautyStaticData();

	private:
		const char** _names;
		const char** _requirementsText;
		const int* _requirements;
		const char** _beginTips;
		const int* _difficults;
		const int* _times;
		const int* _maxPic;

	public:
		static BeautyStaticData* getInstance();
		static void detroy();
		const char* getName(int index);
		const char* getRequiementText(int index);
		const char* getBeginTips(int index);
		int getDifficult(int index);
		int getLimitTime(int index);
		int getRequiement( int index );
		int getMaxPic( int index );
	};
}

#endif
