#ifndef Gallery_MODEL_HH
#define  Gallery_MODEL_HH

#include "cocos2d.h"

USING_NS_CC;

class GalleryModelDelegate;

class GalleryModel
{
private:
	GalleryModelDelegate *_modelDelegate;

public:
	GalleryModel();
	~GalleryModel();

	void setDelegate(GalleryModelDelegate *pDelegate);

	void init();
};

class GalleryModelDelegate
{
public:
	virtual void noticeView(int* data) = 0;
	virtual void noticeShowBeautyPicture(int index) = 0;
};

#endif
