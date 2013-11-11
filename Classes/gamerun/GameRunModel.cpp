#include "GameRunModel.h"
#include "../GameUtil.h"

GameRunModel::GameRunModel(void)
	:_modelDelegate(NULL)
	,_currentTimeMillis(0)
	,_limitTimeMillis(0)
	,_sleeping(0)
	,_isTouchedMineField(false)
	,_buyState(NO_BUY)
	,_loseState(NO_LOSE)
	,_untouchTime(0)
	,_showSleepTip(false)
	,_showTimeTip(false)
{
	// per frame max mine field number =  8
	_currMineFields = new GameArray<MineField*>(MAX_FIELD);
}

GameRunModel::~GameRunModel(void)
{
	delete _currMineFields;
}

void GameRunModel::setDelegate(GameRunModelDelegate *pDelegate)
{
	this->_modelDelegate = pDelegate;
}

void GameRunModel::levelBeginInit()
{
	//_state = STATE_RUN;
	_level = GameModel::getInstance()->getCurrBeauty();
	_sleeping = MAX_SLEEP;
	_nextLevel = _level;
	_index_arrHotspot = 0;
	_index_hotspot = 0;
	_isTouchedHotspot = false;
	_untouchTime = 0;
	_showTimeTip = false;
	_showSleepTip = false;
	// 
	GameModel::getInstance()->readScript(_level);
	resetHotspot();
	resetMineField();
	//
	_limitTimeMillis = BeautyStaticData::getInstance()->getLimitTime(_level) * 60;
	// 
	if(_modelDelegate)
		_modelDelegate->noticeInit(_level , GameModel::getInstance()->getCurrScript()->getTotalFrame());
	// tip show
	setState(STATE_BEGIN);
}

void GameRunModel::resetHotspot()
{
	// read hotspot
	_curHotspot = GameModel::getInstance()->getCurrScript()->getHotspot( _index_arrHotspot, _index_hotspot );
	if( _curHotspot == NULL ) return;

	// set point
	_x = _curHotspot->x;
	_y = GAME_HEIGHT - _curHotspot->y;
	_tx = _curHotspot->tx;
	_ty = GAME_HEIGHT - _curHotspot->ty;
	
	CCLOG("reset hotspot %d(%d,%d), touch begin %d,%d, touch end %d,%d", _curHotspot->frame, _index_arrHotspot, _index_hotspot,(int)_x, (int)_y, (int)_tx, (int)_ty);
}

void GameRunModel::resetMineField()
{
	// read mine field
	GameModel::getInstance()->getCurrScript()->getMineField( _curHotspot->frame , _currMineFields );
}

void GameRunModel::nextHotspot()
{
	BeautyScriptData* script =  GameModel::getInstance()->getCurrScript();
	_isTouchedHotspot = false;
	FrameLabel* arrHotspots = script->getArrHotspot( _index_arrHotspot );
	_index_hotspot++;
	if(_index_hotspot <= arrHotspots->duration)
	{
		if( _index_arrHotspot == script->labels->length - 1 
			&& _index_hotspot == arrHotspots->duration - 1 )
		{// is win !
			_curHotspot = NULL;
			setState(STATE_WIN);
		} 
		else if( _index_hotspot == arrHotspots->duration )
		{// pass level
			nextArr();
			_modelDelegate->noticePhasePass(this);
			resetHotspot();
			resetMineField();
			_modelDelegate->noticePass(this);
			CCLOG("notice Pass Level %d", _index_arrHotspot);
		}
		else 
		{
			resetHotspot();
			resetMineField();
			_modelDelegate->noticePass(this);
			CCLOG("notice Pass step %d", this->_curHotspot->frame);
		}
	}
}

void GameRunModel::nextArr()
{
	FrameLabel* arrHotspots = GameModel::getInstance()->getCurrScript()->getNextArrHotspot(_index_arrHotspot);
	if( arrHotspots == NULL )
	{
		setState(STATE_WIN);
		_index_hotspot--;
	}
	else
	{
		_index_hotspot = 0;
		_index_arrHotspot++;
	}
}

void GameRunModel::touchDownAndMove( const CCPoint&  point)
{
	_untouchTime = 0;
	switch (_state)
	{
	case STATE_BEGIN:
		break;
	case STATE_RUN:
		runningTouchMove(point);
		break;
	case STATE_WIN:
		break;
	case STATE_LOSE:
		break;
	default:
		break;
	}
}

void GameRunModel::runningTouchMove( const CCPoint&  point)
{
	if(!_isTouched)
		CCLOG("touch begin x=%f y=%f", point.x, point.y);
	//
	_isTouched = true;
	_touchX = point.x;
	_touchY = point.y;

	// mine field logic
	_isTouchedMineField = false;
	for( int i = 0; i < _currMineFields->length; i++ )
	{
		MineField* minefield = _currMineFields->list[i];
		if( MathUtil::pnpoly(4, minefield->vertx, minefield->verty, _touchX, 480 - _touchY) == 1 )
		{// is mine field touched 
			_isTouchedMineField = true;
			_reduceSleep = minefield->v == 0 ? 1 : minefield->v;
			// CCLOG( "reduce sleep %d", _reduceSleep);
			break;
		}
	}

	// 
	double dis;
	if(!_isTouchedHotspot)
	{// if not touch any hotspot , distance touch position to spot 1
		dis = MathUtil::lineSpace(_touchX, _touchY, _x, _y);
		if(dis < _curHotspot->size / 2 )//radiu
		{
			_isTouchedHotspot = true;
			_beginTouchTime = _currentTimeMillis;
			CCLOG("--------touched begin hotspot !!");
		}
	}
	else
	{// touched hotspot , distance touch position is to far from area
		int out;
		dis = MathUtil::pointToLine(_x, _y, _tx, _ty, _touchX, _touchY, out);
		if(dis >= _curHotspot->size / 2 )
		{
			_isTouchedHotspot = false;
			CCLOG("--------remove far hotspots line !!");
			goto end;
		}
		// is touched spot 2
		//dis = MathUtil::lineSpace(point.x,point.y,_tx,_ty);
		if( out == 2 && dis < _curHotspot->tsize / 2 )
		{// touched spot 2
			float time = GameModel::getInstance()->getCurrScript()->getTime(_curHotspot->frame);
			//if( time == 0 || time < _currentTimeMillis - _beginTouchTime )
			{//not  time out , refresh picture
				nextHotspot();
			}
			//else
			//{// time out , fail to step 1
			//	_isTouchedHotspot = false;
			//	CCLOG("--------take off to fast, be showly !!");
			//}
			CCLOG("--------moved end hotspot !!");
		}
	}
end:
	_modelDelegate->noticeTouchPoint(this);
}

void GameRunModel::touchUp()
{
	_isTouchedMineField = false;
	switch (_state)
	{
	case STATE_BEGIN:
		break;
	case STATE_RUN:
		runningTouchUp();
		break;
	case STATE_WIN:
		break;
	case STATE_LOSE:
		break;
	default:
		break;
	}
}

void GameRunModel::runningTouchUp()
{
	_isTouched = false;
	_isTouchedHotspot = false;
	_touchX = -1;
	_touchY = -1;
	_modelDelegate->noticeTouchPoint(this);
}

void GameRunModel::setState(GAMESTATE state)
{
	switch (state)
	{
	case STATE_BEGIN:
		_buyState = NO_BUY;
		_loseState = NO_LOSE;
		if(_modelDelegate)_modelDelegate->noticeBeginTip(_level , BeautyStaticData::getInstance()->getBeginTips(_level));
		break;
	case STATE_RUN:
		_buyState = NO_BUY;
		_loseState = NO_LOSE;
		if(_state == STATE_BEGIN)
		{
			_modelDelegate->noticePlay( this );
			_modelDelegate->noticeSleep(this->_sleeping);
		}
		else if(_state == STATE_BUY)
		{
			_modelDelegate->noticeCloseBuy();
		}
		break;
	case STATE_WIN:
		{
			// calc evil mask
			//GameModel::getInstance()->setUserBeautyData(_level,)
			bool newScore = saveScore();
			int gainHeart = GameModel::getInstance()->getUserBeautyDatas(_level)->gainHearts;
			int openBeauty = calcOpen( gainHeart );

			int evilMask = GameModel::getInstance()->getEvilMarkNum();
			if( gainHeart != MAX_HEART || !newScore )
			{
				evilMask = 0;
			}

			_modelDelegate->noticeWin( gainHeart, openBeauty, evilMask);
			GameModel::getInstance()->setCurrBeauty( _level + 1 );
			GameModel::getInstance()->saveGame();
			//CCLOG("GAME WIN !!!!!!!! ");
		}
		break;
	case STATE_LOSE:
		//CCLOG("GAME LOSE !!!!!!!! ");
		break;
	default:
		break;
	}
	_state = state;
}

int GameRunModel::calcOpen( int gainHeart )
{
	if( _level < REAL_BEAUTY_NUM - 1 )
	{
		if( GameModel::getInstance()->getUserBeautyDatas(_level + 1)->gainHearts >= 0 )
		{// next level already open
			return 0;
		}
		_nextLevel++;
		int passRequiement = BeautyStaticData::getInstance()->getRequiement(_level);
		if( gainHeart >= passRequiement )
		{// open next level
			GameModel::getInstance()->setUserBeautyData( _level + 1, 0 );
			return _level + 1;
		}
	}
	return 0;
}

static const int ScoreToHear[] = { 0, 10, 11, 30, 31, 60, 61, 80, 81, 100 };
bool GameRunModel::saveScore()
{
	/*
	heart 			1			2			3			4			5
	left sleep		0~10		11~30	31~60	61~80	81~100
	*/
	int levelHeart = GameModel::getInstance()->getUserBeautyDatas(_level)->gainHearts;
	int currHeart = 1;
	for(int i = 0; i < 10; i += 2)
	{
		if(_sleeping >= ScoreToHear[i] && _sleeping <= ScoreToHear[i+1])
		{
			currHeart = (i>>1) + 1;
			break;
		}
	}
	if( levelHeart < currHeart )
	{
		GameModel::getInstance()->setUserBeautyData( _level, currHeart );
		return true;
	}
	return false;
}

void GameRunModel::update(float delta)
{
	_currentTimeMillis += delta;

	switch (_state)
	{
	case STATE_BEGIN:
		break;
	case STATE_RUN:
		// Time logic
		_untouchTime += delta;
		_limitTimeMillis -= delta;
		if(_limitTimeMillis < 0)
		{
			_limitTimeMillis = 0;
			setState(STATE_LOSE);
			this->_modelDelegate->noticeLose(EMPTY_TIME);
		}
		this->_modelDelegate->noticeTime(floor(_limitTimeMillis));
		// Sleep logic
		if(_isTouchedMineField)
		{
			this->_sleeping -= 4*(_reduceSleep * delta);
			CCLOG( "reduce sleep %f ,delta %f", _sleeping, delta);
			if( this->_sleeping < 0 )
			{
				this->_sleeping = 0;
				setState(STATE_LOSE);
				this->_modelDelegate->noticeLose(EMPTY_SLEEP);
			}
			this->_modelDelegate->noticeSleep(_sleeping/MAX_SLEEP);
		}
		updateTip();
		break;
	case STATE_WIN:
		break;
	case STATE_LOSE:
		break;
	default:
		break;
	}
}

void GameRunModel::updateTip()
{
	if( _sleeping < 20  && !_showSleepTip )
	{
		_modelDelegate->noticeShowTip(SLEEP);
		_showSleepTip = true;
	}
	if(_limitTimeMillis < 20 && !_showTimeTip )
	{
		_modelDelegate->noticeShowTip(TIME);
		_showTimeTip = true;
	}
	if(_untouchTime > 15)
	{
		_modelDelegate->noticeShowTip(GUIDE);
		_untouchTime = 0;
	}
}

int GameRunModel::getCurLevel()
{
	return _level;
}

int GameRunModel::getCurPic()
{
	//return GameModel::getInstance()->getCurrScript()->getPicID(_curHotspot?_curHotspot->frame:0);
	return _curHotspot?_curHotspot->frame:( GameModel::getInstance()->getCurrScript()->getTotalFrame() );
}

CCPoint GameRunModel::getBeginPoint()
{
	return CCPoint(_x, _y);
}

CCPoint GameRunModel::getLastPoint()
{
	return CCPoint(_tx, _ty);
}

TOUCHMODE GameRunModel::getTouchInfo( CCPoint* outPoint )
{
	if(_isTouched)
	{
		outPoint->setPoint(_touchX,_touchY);
		if(_isTouchedHotspot)
		{
			return TOUCH_IN;
		}
		else
		{
			return TOUCH_OUT;
		}
	}
	return UNTOUCH;
}

const char* GameRunModel::getCurTip( CCRect* outRect )
{
	Tip* tip = GameModel::getInstance()->getCurrScript()->getTip(_curHotspot->frame);
	if ( tip )
	{
		outRect->setRect( tip->x, tip->y, tip->w, tip->h );
		return tip->text;
	}
	return NULL;
}

GAMESTATE GameRunModel::getState()
{
	return _state;
}

bool GameRunModel::getCurGuide()
{
	return GameModel::getInstance()->getCurrScript()->getGuide(_curHotspot->frame);
}

void GameRunModel::setPay(BUYSTATE buyState)
{
	_buyState = buyState;
	setState(STATE_BUY);
	_modelDelegate->noticeBuy(buyState);
}