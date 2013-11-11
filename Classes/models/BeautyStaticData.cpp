#include "BeautyStaticData.h"

namespace GAME_NAMESPACE
{
	static const char* names[] = {"可爱小妹","醉酒空空","迷情莉亚","抱枕小泽","柔情学妹","私人小秘","空中小姐"};
	static const int difficults[] = {1,1,2,2,3,3,4};
	static const int times[] = {8,5,3,2,2,2,2};
	static const int passRequirement[] = {0, 2, 2, 2, 3, 3, 4};
	static const int maxPic[] = {15, 15, 14, 15, 14, 13, 15};
	static const char* requirementsText[] = {
		"无."
		,"第一关2心或直接解锁2元."
		,"第二关2心或直接解锁2元."
		,"第三关2心或直接解锁2元."
		,"第四关3心或直接解锁2元."
		,"第五关3心或直接解锁3元."
		,"第六关4心或直接解锁4元."
	};
	// 字数奇数 + 3个点
	static const char* beginTips[] = {
		 "根据提示解开梦中情人身上的层层束缚，找到并解除使她处于沉睡的封印。..."
		,"每个女孩身上都有敏感的部位，一旦触屏会让女孩熟睡度下降，小心哦。..."
		,"有些敏感区会处于脱衣的路线上，在脱这样的区域时要注意避开或者调整节奏。..."
		,"有一些场景中的摆设挡住了女孩，需要移开才可以进行下一步。..."
		,"学妹的胸部很敏感哦，要轻轻触碰。..."
		,"小秘的丝袜比较紧，下拉的时候要注意方向哦。..."
		,"空姐的胸部会比较丰满，扣子不好解开，要有耐心哦。..."
	};

	static BeautyStaticData* _instance;

	const char* BeautyStaticData::getName( int index )
	{
		return _names[index];
	}

	const char* BeautyStaticData::getRequiementText( int index )
	{
		return _requirementsText[index];
	}

	int BeautyStaticData::getDifficult( int index )
	{
		return _difficults[index];
	}

	int BeautyStaticData::getLimitTime( int index )
	{
		return _times[index];
	}

	const char* BeautyStaticData::getBeginTips( int index )
	{
		return _beginTips[index];
	}

	int BeautyStaticData::getRequiement( int index )
	{
		return _requirements[index];
	}

	BeautyStaticData::BeautyStaticData()
	{
		this->_names = names;
		this->_difficults = difficults;
		this->_times = times;
		this->_requirementsText = requirementsText;
		this->_beginTips = beginTips;
		this->_requirements = passRequirement;
		_maxPic = maxPic;
	}

	BeautyStaticData* BeautyStaticData::getInstance()
	{
		if(!_instance){
			_instance = new BeautyStaticData();
		}
		return _instance;
	}

	void BeautyStaticData::detroy()
	{
		if(_instance)
		{
			delete _instance;
			_instance = NULL;
		}
	}

	BeautyStaticData::~BeautyStaticData()
	{

	}

	int BeautyStaticData::getMaxPic( int index )
	{
		return _maxPic[index];
	}

}