#include "GalleryModel.h"
#include "GameModel_H.h"

GalleryModel::GalleryModel(void)
{
	this->_modelDelegate = NULL;
}

GalleryModel::~GalleryModel(void)
{

}

void GalleryModel::setDelegate(GalleryModelDelegate *pDelegate)
{
	this->_modelDelegate = pDelegate;
}

void GalleryModel::init()
{
	int data[REAL_BEAUTY_NUM];
	for(int i = 0 ;i<REAL_BEAUTY_NUM;i++)
	{
		data[i] = GameModel::getInstance()->getUserBeautyDatas(i)->gainHearts;
	}
	this->_modelDelegate->noticeView(data);
}
