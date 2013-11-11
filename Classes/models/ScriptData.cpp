#include "ScriptData.h"
#include <string>
#include <math.h>

namespace GAME_NAMESPACE
{
	const char* charsCopy( const char* source );
	//const wchar_t* charsCopyToWchar( const char* source );

	static const char* JsonName_lables = "labels";
	static const char* JsonName_hotspots = "hotspots";
	static const char* JsonName_tips = "tips";
	static const char* JsonName_mineFields = "mineFields";
	static const char* JsonName_extraDatas = "extraDatas";
	static const char* JsonName_config = "config";

	BeautyScriptData::BeautyScriptData(int level)
		:config(NULL)
		,labels(NULL)
		,hotspots(NULL)
		,tips(NULL)
		,extraDatas(NULL)
		,mineFields(NULL)
	{
		this->level = level;
	}

	BeautyScriptData::~BeautyScriptData()
	{
		if (labels)
		{
			for(int i = 0;i< labels->length;i++)
			{
				delete labels->list[i];
			}
			delete labels;
			labels = NULL;
		}
		if (hotspots)
		{
			for(int i = 0;i< hotspots->length;i++)
			{
				delete hotspots->list[i];
			}
			delete hotspots;
			hotspots = NULL;
		}
		if (tips)
		{
			for(int i = 0;i< tips->length;i++)
			{
				delete tips->list[i];
			}
			delete tips;
			tips = NULL;
		}
		if(mineFields)
		{
			for(int i = 0;i< mineFields->length;i++)
			{
				delete mineFields->list[i];
			}
			delete mineFields;
			mineFields = NULL;
		}
		if(extraDatas)
		{
			for(int i = 0;i< extraDatas->length;i++)
			{
				delete extraDatas->list[i];
			}
			delete extraDatas;
			extraDatas = NULL;
		}
	}

	BeautyScriptData* BeautyScriptData::createFormJsonData( int level, const char* data )
	{
		Json* c;
		Json* root = Json_create(data);
		BeautyScriptData* script = new BeautyScriptData( level );

		// label
		Json* labels = Json_getItem( root, JsonName_lables );
		if( labels != NULL )
		{
			script->labels = new GameArray<FrameLabel*>(Json_getSize(labels));
			c = labels->child;
			while ( c )
			{
				FrameLabel* data = new FrameLabel();
				data->name = charsCopy( Json_getString( c, "name" , NULL ) );
				data->startFrame = Json_getInt( c, "startFrame", 0);
				data->duration = Json_getInt( c, "duration", 0);
				script->labels->add(data);
				c = c->next;
			}
		}

		// hotspots
		Json* hotspots = Json_getItem( root, JsonName_hotspots );
		if( hotspots != NULL )
		{
			script->hotspots = new GameArray<HotSpot*>(Json_getSize(hotspots));
			c = hotspots->child;
			while ( c )
			{
				HotSpot* data = new HotSpot();
				//{frame:"0",x:"147.55",y:"277.5",size:"32",tx:"192.85",ty:"301",tsize:"32"},
				data->frame = Json_getInt( c, "frame" , 0 );
				data->x = Json_getFloat( c, "x", 0 );
				data->y = Json_getFloat( c, "y", 0 );
				data->size = Json_getFloat( c, "size", 0 );
				data->tx = Json_getFloat( c, "tx", 0 );
				data->ty = Json_getFloat( c, "ty", 0 );
				data->tsize= Json_getFloat( c, "tsize", 0 );
				script->hotspots->add(data);
				c = c->next;
			}
		}

		//extra data
		Json* extraDatas = Json_getItem( root, JsonName_extraDatas );
		if(extraDatas != NULL)
		{
			script->extraDatas = new GameArray<ExtraData*>(Json_getSize(extraDatas));
			c = extraDatas->child;
			while ( c )
			{
				ExtraData* data = new ExtraData();
				// { frame:"0",x:"147.55",y:"277.5",size:"32",tx:"192.85",ty:"301",tsize:"32" },
				data->frame = Json_getInt( c, "frame" , 0 );
				data->guide = ((Json_getInt(c, "guide", 0) == 1)?true:false);
				data->time = Json_getFloat( c, "time", 0);
				script->extraDatas->add(data);
				c = c->next;	
			}
		}

		// tips
		Json* tips = Json_getItem( root, JsonName_tips );
		if( tips != NULL )
		{
			script->tips = new GameArray<Tip*>(Json_getSize(tips));
			c = tips->child;
			while ( c )
			{
				Tip* data = new Tip();
				// {frame:"0",tip:"xxx",x:"130",y:"157.25",w:"155",h:"40"}
				data->frame = Json_getInt( c, "frame" , 0 );
				data->text = charsCopy( Json_getString( c, "tip" , NULL ) );
				data->x = Json_getFloat( c, "x", 0);
				data->y = Json_getFloat( c, "y", 0);
				data->w = Json_getFloat( c, "w", 0);
				data->h = Json_getFloat( c, "h", 0);
				script->tips->add(data);
				c = c->next;
			}
		}
		
		//mine field
		Json* json_mineField = Json_getItem( root, JsonName_mineFields );
		if( tips != NULL )
		{
			script->mineFields = new GameArray<MineField*>(Json_getSize(json_mineField));
			c = json_mineField->child;
			while ( c )
			{
				MineField* data = new MineField();
				Json* cc = c->child;
				// {frame:"0",tip:"xxx",x:"130",y:"157.25",w:"155",h:"40"}
				data->frame = Json_getInt( c, "frame" , 0 );
				data->x = Json_getFloat( c, "x", 0);
				data->y = Json_getFloat( c, "y", 0);
				data->w = Json_getFloat( c, "w", 0);
				data->h = Json_getFloat( c, "h", 0);
				data->r = Json_getFloat( c, "r", 0);
				data->v = Json_getFloat( c, "v", 0);
				data->calcVert();
				c = c->next;
				script->mineFields->add(data);
			}
		}
		Json_dispose(root);
		return script;
	}

	/*int BeautyScriptData::getPicID(int frame)
	{
	for(int i = 0; i<this->labels->length; i++)
	{
	FrameLabel *label = this->labels->list[i];
	if( label->startFrame <= frame && label->duration + label->startFrame>frame )
	{
	return frame - i;
	}
	}
	return 0;
	}*/

	float BeautyScriptData::getTime(int frame)
	{
		for( int i = 0; i < this->extraDatas->length; i++)
		{
			ExtraData* data = this->extraDatas->list[i];
			if( data->frame == frame )
			{
				return data->time;
			}
		}
		return 0;
	}

	bool BeautyScriptData::getGuide(int frame)
	{
		for( int i = 0; i < this->extraDatas->length; i++)
		{
			ExtraData* data = this->extraDatas->list[i];
			if( data->frame == frame )
			{
				return data->guide;
			}
		}
		return false;
	}

	Tip* BeautyScriptData::getTip(int frame)
	{
		for( int i = 0; i < this->tips->length; i++)
		{
			Tip* data = this->tips->list[i];
			if( data->frame == frame )
			{
				return data;
			}
		}
		return NULL;
	}

	void BeautyScriptData::getMineField( int frame,  GameArray<MineField*>* const outMineFields )
	{
		outMineFields->length = 0;
		for( int i = 0; i < this->mineFields->capacity; i++)
		{
			MineField* data = this->mineFields->list[i];
			if( data->frame == frame )
			{
				outMineFields->add(data);
			} else if(data->frame > frame)
			{
				break;
			}
		}
	}

	HotSpot* BeautyScriptData::getHotspot(int indexArr,int indexHotspot)
	{
		if(indexArr < this->labels->length && indexHotspot < this->labels->list[indexArr]->duration)
		{
			int index =  this->labels->list[indexArr]->startFrame + indexHotspot;
			if(index < this->hotspots->length)
			{
				return this->hotspots->list[index];
			}
		}
		else
		{
			//CCAssert(true,"hotspot array outside");
		}
		return NULL;
	}

	/*HotSpot* BeautyScriptData::getNextHotspot(int indexArr,int indexHotspot)
	{
	indexHotspot++;
	if(indexArr < this->labels->length)
	{
	if(indexHotspot < this->labels->list[indexArr]->duration)
	{
	int index =  this->labels->list[indexArr]->startFrame + indexHotspot;
	return this->hotspots->list[index];
	}
	else
	{
	indexArr++;
	if(indexArr < this->labels->length)
	{
	int index =  this->labels->list[indexArr]->startFrame;
	return this->hotspots->list[index];
	}
	}
	}
	return NULL;
	}*/

	FrameLabel* BeautyScriptData::getArrHotspot( int indexArr )
	{
		if(indexArr < this->labels->length)
		{
			return this->labels->list[indexArr];
		}
		return NULL;
	}

	FrameLabel* BeautyScriptData::getNextArrHotspot( int indexArr )
	{
		return getArrHotspot(++indexArr);
	}

	int BeautyScriptData::getTotalFrame()
	{
		// throw std::exception("The method or operation is not implemented.");
		return labels->list[labels->length - 1]->startFrame + labels->list[labels->length - 1]->duration - 1;
	}

	Config::Config()
		:tip(NULL)
	{
	}

	Config::~Config()
	{
		delete[] tip;
	}

	FrameLabel::FrameLabel()
		:name(NULL)
	{
	}

	FrameLabel::~FrameLabel()
	{
		delete[] name;
	}

	Tip::Tip()
		:text(NULL)
	{
	}

	Tip::~Tip()
	{
		delete[] text;
	}

	/////////////////////////////////////////////////////////////////////
	const char* charsCopy( const char* source )
	{
		if( source == NULL )
			return NULL;
		int length = strlen( source );
		char* dest = new char[ length + 1 ];
		strcpy( dest, source );
		return dest;
	}

	//const wchar_t* charsCopyToWchar( const char* source )
	//{
	//	if( source == NULL )
	//		return NULL;
	//	const size_t cSize = strlen(source)+1;
	//	wchar_t* wc = new wchar_t[cSize];
	//	//mbstowcs (wc, source, cSize);
	//	return wc;
	//}

	/*
	const char* Script::JSON_HOTSPOT = "hotspot";
	const char* Script::JSON_MINEFIELD = "minefield";
	const char* Script::JSON_CONFIG = "config";

	Script::Script()
	{
		imgnum = 0;
		arrHotspotsNum = 0;
		arrMinefieldsNum = 0;
		arrHotspots = NULL;
		arrMinefields = NULL;
		config = NULL;
		hardfactor = NULL;
	}

	Script::~Script()
	{
		for(int i=0;i<arrHotspotsNum;i++)delete arrHotspots[i];
		for(int i=0;i<arrMinefieldsNum;i++)delete arrMinefields[i];
		delete[] arrHotspots;
		delete[] arrMinefields;
		delete config;
		delete hardfactor;
	}

	Script* Script::createFormJson(const char* path)
	{
		char* fucktest = new char[20];
		unsigned long size;
		char* data = reinterpret_cast<char*>(FileUtils::getInstance()->getFileData(
			FileUtils::getInstance()->fullPathForFilename(path).c_str(), "r", &size));

		Json* root = Json_create(data);
		FREE(data);

		Script* script = new Script();

		// image num
		script->imgnum = Json_getInt(root,"imgnum",1);

		// hot spot 
		int index = 0;
		Json* arr_arr = Json_getItem(root,JSON_HOTSPOT);
		Json* arr = arr_arr->child;
		script->arrHotspotsNum = Json_getSize(arr_arr);
		script->arrHotspots = new ArrHotspot*[script->arrHotspotsNum];
		for(int i = 0; i < script->arrHotspotsNum; i++)
		{
			int size = Json_getSize(arr);
			ArrHotspot* arrHotspot = new ArrHotspot();
			arrHotspot->size = size;
			arrHotspot->hotspots = new Hotspot*[size];
			Json* jsonHotspot = arr->child;
			for(int j=0; j<size; j++)
			{
				Hotspot* hotspot = new Hotspot();
				hotspot->index = index;
				hotspot->x = Json_getFloat(jsonHotspot,"x",-1);
				hotspot->y = Json_getFloat(jsonHotspot,"y",-1);
				hotspot->tx = Json_getFloat(jsonHotspot,"tx",-1);
				hotspot->ty = Json_getFloat(jsonHotspot,"ty",-1);
				hotspot->scaleX = Json_getFloat(jsonHotspot,"scaleX",-1);
				hotspot->scaleY = Json_getFloat(jsonHotspot,"scaleY",-1);
				hotspot->time = Json_getFloat(jsonHotspot,"time",-1);
				//hotspot->guide = (strcmp(Json_getString(jsonHotspot,"guide",0),"true") == 1);
				hotspot->name = charsCopy(Json_getString(jsonHotspot,"name",NULL));
				hotspot->tip = charsCopy(Json_getString(jsonHotspot,"tip",NULL));
				arrHotspot->hotspots[j] = hotspot;
				jsonHotspot = jsonHotspot->next;

				if(j != size-1) index++;
				CCLOG("picture %d loading",index);
			}
			script->arrHotspots[i] = arrHotspot;
			arr = arr->next;
		}
		
		//CCLOG("%d",script->arrHotspotsNum);
		
		// mine field
		arr_arr = Json_getItem(root,JSON_MINEFIELD);
		arr = arr_arr->child;
		script->arrMinefieldsNum = Json_getSize(arr_arr);
		script->arrMinefields = new ArrMineField*[script->arrMinefieldsNum];
		for(int i=0; i<script->arrMinefieldsNum; i++)
		{
			int size = Json_getSize(arr);
			ArrMineField* arrMinefield = new ArrMineField();
			arrMinefield->size = size;
			arrMinefield->minefields = new Minefield*[size];
			Json* jsonMinefield = arr->child;
			for(int j=0 ;j<size; j++)
			{
				Minefield* minefield = new Minefield();
				minefield->x = Json_getFloat(jsonMinefield,"x",0);
				minefield->y = Json_getFloat(jsonMinefield,"y",0);
				minefield->scaleX = Json_getFloat(jsonMinefield,"scaleX",0);
				minefield->scaleY = Json_getFloat(jsonMinefield,"scaleY",0);
				minefield->score = Json_getInt(jsonMinefield,"score",0);
				minefield->rotation = Json_getFloat(jsonMinefield,"rotation",0);
				minefield->name = charsCopy(Json_getString(jsonMinefield,"name",NULL));
				arrMinefield->minefields[j] = minefield;
				jsonMinefield = jsonMinefield->next;
			}
			script->arrMinefields[i] = arrMinefield;
			arr = arr->next;
		}

		// config
		Json* jsonConfig = Json_getItem(root,JSON_CONFIG);
		Config* config = new Config();
		config->a = Json_getInt(jsonConfig,"a",0);
		config->b = Json_getInt(jsonConfig,"b",0);
		config->k = Json_getInt(jsonConfig,"k",0);
		config->sleep = Json_getInt(jsonConfig,"sleep",0);
		config->eyesNum = Json_getInt(jsonConfig,"eyesNum",0);
		config->c = Json_getFloat(jsonConfig,"c",0);
		config->tx = Json_getFloat(jsonConfig,"tx",0);
		config->ty = Json_getFloat(jsonConfig,"ty",0);
		config->time = Json_getFloat(jsonConfig,"time",0);
		config->tip = charsCopy(Json_getString(jsonConfig,"tip",NULL));
		script->config = config;

		Json_dispose(root);

		return script;
	}
	*/
	void MineField::calcVert()
	{
		vertx[0] = -w/2;
		vertx[1] = w/2;
		vertx[2] = w/2;
		vertx[3] = -w/2;
		verty[0] = -h/2;
		verty[1] = -h/2;
		verty[2] = h/2;
		verty[3] = h/2;
		float rv = r*3.14159265/180;
		float sinv = sinf(rv);
		float cosv = cosf(rv);
		for(int i=0;i<4;i++)
		{
			float vx = vertx[i];
			float vy = verty[i];
			vertx[i] = vx*cosv - vy*sinv + x;
			verty[i] = vy*cosv + vx*sinv + y;
		}
	}

}