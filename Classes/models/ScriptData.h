#ifndef SCRIPT_HH__HH
#define  SCRIPT_HH__HH

#include "GameGlobal.h"
#include "GameUtil.h"

#include "ExtensionMacros.h"
#include "spine/Json.h"
#include "spine/extension.h"

USING_NS_CC_EXT;

namespace GAME_NAMESPACE
{
	class Config
	{
	public:
		int sleep;
		int eyeNum;
		const char* tip;
		Config();
		~Config();
	};
	
	class FrameLabel
	{
	public:
		const char* name;
		int startFrame;
		int duration;
		FrameLabel();
		~FrameLabel();
	};
	
	class HotSpot
	{
	public:
		int frame;
		float x,y,size,tx,ty,tsize;
	};
	
	class ExtraData
	{
	public:
		int frame;
		float time;
		bool guide;
	};
	
	class Tip
	{
	public:
		int frame;
		const char* text;
		float x,y,w,h;
		Tip();
		~Tip();
	};
	//
	class MineField
	{
	public:
		void calcVert();
		int frame;
		float x,y,w,h,r,v;
		//calc
		float vertx[4];
		float verty[4];
	};

	class BeautyScriptData
	{
	private:
		BeautyScriptData(int level);
	public:
		~BeautyScriptData();

	public:
		int level;
		Config* config;
		GameArray<FrameLabel*>* labels;
		GameArray<HotSpot*>* hotspots;
		GameArray<Tip*>* tips;
		GameArray<ExtraData*>* extraDatas;
		GameArray<MineField*>* mineFields;

	public:
		//int getPicID(int frame);
		float getTime(int frame);
		bool getGuide(int frame);
		HotSpot* getHotspot( int indexArr,int indexHotspot );
		//HotSpot* getNextHotspot( int indexArr,int indexHotspot );
		FrameLabel* getArrHotspot( int indexArr );
		FrameLabel* getNextArrHotspot( int indexArr );
		Tip* getTip(int frame);
		void getMineField( int frame, GameArray<MineField*>* const outMineFields );
		int getTotalFrame();

	public:
		// parameter data , please free yourself
		static BeautyScriptData* createFormJsonData( int level, const char* data );
	};

	/*class ArrHotspot;
	class ArrMineField;
	class Config;

	class Script
	{
	public:
		static const char* JSON_HOTSPOT;
		static const char* JSON_MINEFIELD;
		static const char* JSON_CONFIG;

	public:
		int level;
		int imgnum;
		int arrHotspotsNum;
		int arrMinefieldsNum;
		ArrHotspot** arrHotspots;
		ArrMineField** arrMinefields;
		Config* config;
		float* hardfactor;

	public:
		static Script* createFormJson(const char* path);

	private:
		Script();
	public:
		~Script();
	};

	class Hotspot
	{
	public:
		int index;
		float x,y,tx,ty,scaleX,scaleY,rotation,time;
		bool guide;
		const char* tip;
		const char* name;
		Hotspot();
		~Hotspot();
	};

	class ArrHotspot
	{
	public:
		int size;
		Hotspot** hotspots;
		ArrHotspot();
		~ArrHotspot();
	};

	class Minefield
	{
	public:
		float x,y,scaleX,scaleY,rotation,score;
		const char* name;
		Minefield();
		~Minefield();
	};

	class ArrMineField
	{
	public:
		int size;
		Minefield** minefields;
		ArrMineField();
		~ArrMineField();
	};

	class Config
	{
	public:
		int a,b,k,sleep,eyesNum;
		float c,tx,ty,time;
		const char* tip;
		Config();
		~Config();
	};*/
};

#endif
