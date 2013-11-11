#ifndef GAME_GLOBAL_HH__HH
#define  GAME_GLOBAL_HH__HH

#define GAME_NAMESPACE pplasto
#define GAME_WIDTH 320
#define GAME_HEIGHT 480
#define MAX_BEAUTY_NUM 10
#define REAL_BEAUTY_NUM 7
#define MAX_STAR 5
#define MAX_HEART 5
#define MAX_FIELD 8
#define MAX_SLEEP 100

#define MUSIC_GAME "music/sound_mu_game.mp3"
#define MUSIC_MENU "music/sound_mu_menu.mp3"
#define EFFECT_CLICK "music/sound_ef_click.mp3"
#define EFFECT_FAILED "music/sound_ef_level_failed.mp3"
#define EFFECT_UNLOCK "music/sound_ef_level_unlock.mp3"
#define EFFECT_WIN "music/sound_ef_level_win.mp3"
#define EFFECT_STAGE_PASS "music/sound_ef_stage_cpt.mp3"

extern bool soundEnable;
extern bool effectEnable;
extern void toggleSound();
extern void toggleEffect();
extern void playBackgroundMusic(const char* name);
extern void playEffect(const char* name);

#endif
