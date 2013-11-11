#include "SelectMenuModel.h"

SelectMenuModel::SelectMenuModel(void)
	:_delegate(NULL)
{
}

SelectMenuModel::~SelectMenuModel(void)
{
}

void SelectMenuModel::setDelegate(SelectMenuModelDelegate *pDelegate)
{
	this->_delegate = pDelegate;
}

BeautyData SelectMenuModel::getBeautyData( int index )
{
	BeautyData data;
	data.name = BeautyStaticData::getInstance()->getName(index);
	data.difficult = BeautyStaticData::getInstance()->getDifficult(index);
	data.time = BeautyStaticData::getInstance()->getLimitTime(index);
	return data;
}

int SelectMenuModel::getUserGainHeartNum( int index )
{
	return GameModel::getInstance()->getUserBeautyDatas(index)->gainHearts;
}

void SelectMenuModel::onEnter()
{
	_delegate->noticeView(this);
	_delegate->noticeMoveLevel(GameModel::getInstance()->getCurrBeauty());
}

void SelectMenuModel::gotoBeauty( int index )
{
	if(getUserGainHeartNum(index) >= 0)
	{
		GameModel::getInstance()->readScript(index);
		_delegate->noticeGotoBeauty();
	}
	else
	{
		_delegate->noticeShowLock( index - 1, BeautyStaticData::getInstance()->getRequiement(index));
	}
}

void SelectMenuModel::gotoBoss()
{
	_delegate->noticeGotoBoss(GameModel::getInstance()->getEvilMarkNum());
	//throw std::exception("The method or operation is not implemented.");
}

void SelectMenuModel::gotoUnlockBeauty( int beautyTouched )
{
	GameModel::getInstance()->unlockBeauty(beautyTouched);
	_delegate->noticeUnlock(beautyTouched);
	//throw std::exception("The method or operation is not implemented.");
}

