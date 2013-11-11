#ifndef _GAME_UTIL_H
#define _GAME_UTIL_H

#include <string>

class MathUtil
{
public:
	static double lineSpace(double x1, double y1,double x2, double y2);
	// static double lineSpace( Point point1, Point point2 );
	// static double pointToLine( Point point, Point linePoint1, Point linePoint2 );
	static double pointToLine(double x1, double y1, double x2, double y2, double x0, double y0, int& out);
	static int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy);
};

template<class T>
class GameArray
{
public:
	int capacity;
	int length;
	T *list;
	
public:
	GameArray( int capacity );
	~GameArray();
	//T& operator [](int index);
	//int getSize();
	int add(T);
};

template<class T>
GameArray<T>::~GameArray()
{
	if(list) delete []list;
}

template<class T>
GameArray<T>::GameArray( int capacity ):length(0),list(NULL)
{
	this->capacity = capacity;
	if( capacity > 0 )
	{
		list = new T[capacity];
	}
}

template<class T>
int GameArray<T>::add(T t)
{
	if( length + 1 > capacity ) 
	{
			int a = 0;
			int b = 1/ a;
	}
	list[length++] = t;
	return length;
}

//inline void WStrToUTF8(std::string& dest, const std::wstring& src){
//	dest.clear();
//	for (size_t i = 0; i < src.size(); i++){
//		wchar_t w = src[i];
//		if (w <= 0x7f)
//			dest.push_back((char)w);
//		else if (w <= 0x7ff){
//			dest.push_back(0xc0 | ((w >> 6)& 0x1f));
//			dest.push_back(0x80| (w & 0x3f));
//		}
//		else if (w <= 0xffff){
//			dest.push_back(0xe0 | ((w >> 12)& 0x0f));
//			dest.push_back(0x80| ((w >> 6) & 0x3f));
//			dest.push_back(0x80| (w & 0x3f));
//		}
//		else if (sizeof(wchar_t) > 2 && w <= 0x10ffff){
//			dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation
//			dest.push_back(0x80| ((w >> 12) & 0x3f));
//			dest.push_back(0x80| ((w >> 6) & 0x3f));
//			dest.push_back(0x80| (w & 0x3f));
//		}
//		else
//			dest.push_back('?');
//	}
//}

////! simple warpper
//inline std::string WStrToUTF8(const std::wstring& str){
//	std::string result;
//	WStrToUTF8(result, str);
//	return result;
//}

#endif