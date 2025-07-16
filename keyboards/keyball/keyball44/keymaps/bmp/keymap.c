#include QMK_KEYBOARD_H
#include "quantum.h"
#include "keymap_japanese.h"

// レイヤー定義
#define _MAIN 0
#define _CUSTOM 1
#define _NUMBER 2
#define _SYMBOL 3
#define _BLUETOOTH 4
#define _MOUSE 5
#define _SCROLL 6

// 長いキーをマクロに
#define NOSPACE _______
#define LC_LG2 LCTL_T(KC_LNG2)
#define RC_LG1 RCTL_T(KC_LNG1)

#define LS_SPC LSFT_T(KC_SPC)
#define LT2_ENT LT(2, KC_ENT)

#define LA_LEFT LALT(KC_LEFT)
#define LA_RGHT LALT(KC_RGHT)
#define SFT_ALT LSFT(LALT(KC_NO))

#define LT2_LG1 LT(2, KC_LNG1)
#define LT3_ENT LT(3, KC_ENT)

// BLP短縮名(susumu)
// #define KBC_RST  KBC_RST
#define KBC_SAV  KBC_SAVE
#define CPI_U1H  CPI_I100
#define CPI_D1H  CPI_D100
#define CPI_U1K  CPI_I1K
// #define CPI_D1K  CPI_D1K
#define SCR_TO  SCRL_TO
#define SCR_MO  SCRL_MO
#define SCR_DN  SCRL_DVI
#define SCR_UP  SCRL_DVD
#define SN_VRT  SSNP_VRT
#define SN_HOR  SSNP_HOR
#define SN_FRE  SSNP_FRE
// #define AML_TO   AML_TO
#define AML_U50  AML_I50
// #define AML_D50  AML_D50

#define SCR_REV  SCRL_REV

// デフォルト値設定
#ifndef KEYBALL_CPI_DEFAULT
#    define KEYBALL_CPI_DEFAULT 2000
#endif
#ifndef KEYBALL_SCROLL_DIV_DEFAULT
#    define KEYBALL_SCROLL_DIV_DEFAULT 8 
#endif

// キーコード定義
enum custom_keycodes {
	AMT_P1 = KEYBALL_SAFE_RANGE, // オートマウスレイヤに入る閾値を+1  他のキーコードと衝突しないように
	AMT_M1, // オートマウスレイヤに入る閾値を-1
	SFT_ALT2, // Shift キーと Alt キーの同時押し(susumu)
	RUN_TTM, // テラタームマクロ実行(susumu)
	// SCRL_REV, // スクロール方向反転
};

// 変数定義
bool isScrollInvert = false;
bool isJisMode = true;
bool isRecording = false;
int pairingId = -1;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	// 0
	[_MAIN] = LAYOUT_universal(
		KC_ESC , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,        KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
		MO(1)  , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,        KC_H   , KC_J   , KC_K   , KC_L   , KC_P   , MO(1)  ,
		MO(2)  , KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   ,        KC_N   , KC_M   , KC_BTN1, KC_BTN2, MO(6)  , MO(2)  ,
		         KC_LGUI, KC_LALT, SFT_ALT, LC_LG2 , LS_SPC ,        LT3_ENT, LT2_LG1, NOSPACE, NOSPACE, MO(4)
	),
	// 1
	[_CUSTOM] = LAYOUT_universal(
		XXXXXXX, XXXXXXX, KC_F2  , KC_PGUP, KC_ENT , KC_TAB ,        KC_TAB , KC_F7  , KC_UP  , KC_ENT , XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, KC_HOME, KC_DEL , KC_END , XXXXXXX,        KC_BSPC, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, KC_PSCR, KC_PGDN, XXXXXXX, XXXXXXX,        JP_MINS, JP_EQL , JP_COMM, JP_DOT , JP_QUES, JP_UNDS, 
		         _______, _______, _______, _______, _______,        _______, _______, NOSPACE, NOSPACE, XXXXXXX
	),
	// 2
	[_NUMBER] = LAYOUT_universal(
		XXXXXXX, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,        KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , XXXXXXX, 
		XXXXXXX, KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 ,        KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , XXXXXXX, 
		XXXXXXX, KC_F11 , KC_F12 , XXXXXXX, XXXXXXX, XXXXXXX,        JP_PLUS, JP_ASTR, JP_LABK, JP_RABK, JP_SLSH, XXXXXXX, 
		         _______, _______, _______, _______, _______,        _______, _______, NOSPACE, NOSPACE, XXXXXXX 
	),
	// 3
	[_SYMBOL] = LAYOUT_universal(
		XXXXXXX, JP_QUOT, JP_DQUO, JP_EXLM, XXXXXXX, JP_TILD,        JP_BSLS, XXXXXXX, JP_LBRC, JP_RBRC, JP_PERC, XXXXXXX, 
		XXXXXXX, JP_AT  , JP_SCLN, JP_DLR , XXXXXXX, JP_GRV ,        JP_HASH, XXXXXXX, JP_LPRN, JP_RPRN, JP_PIPE, XXXXXXX, 
		XXXXXXX, XXXXXXX, XXXXXXX, JP_COLN, JP_CIRC, XXXXXXX,        JP_AMPR, XXXXXXX, JP_LCBR, JP_RCBR, XXXXXXX, XXXXXXX, 
		         _______, _______, _______, _______, _______,        _______, _______, NOSPACE, NOSPACE, XXXXXXX
	),
	// 4
	[_BLUETOOTH] = LAYOUT_universal(
		XXXXXXX, XXXXXXX, XXXXXXX, ADV_ID1, ADV_ID0, AD_WO_L,        AD_WO_L, ADV_ID0, ADV_ID1, XXXXXXX, XXXXXXX, TO(6)  , 
		XXXXXXX, XXXXXXX, XXXXXXX, RUN_TTM, BATT_LV, SEL_USB,        SEL_USB, BATT_LV, RUN_TTM, XXXXXXX, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, ENT_SLP, SEL_BLE,        SEL_BLE, ENT_SLP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		         _______, _______, _______, _______, _______,        _______, _______, NOSPACE, NOSPACE, XXXXXXX
	),
	// 5
	[_MOUSE] = LAYOUT_universal(
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
	),
	// 6
	[_SCROLL] = LAYOUT_universal(
		KC_ESC , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,        KBC_RST, CPI_U1H, CPI_U1K, SCR_UP , SN_VRT , TO(0)  , 
		XXXXXXX, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,        KBC_SAV, CPI_D1H, CPI_D1K, SCR_DN , SN_FRE , XXXXXXX, 
		XXXXXXX, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   ,        XXXXXXX, KC_BTN3, LA_LEFT, LA_RGHT, XXXXXXX, XXXXXXX, 
		         XXXXXXX, XXXXXXX, KC_LALT, KC_LCTL, KC_SPC ,        _______, _______, NOSPACE, NOSPACE, XXXXXXX
	),
};
// clang-format on

// レイヤー6でスクロールモード
layer_state_t layer_state_set_user(layer_state_t state) {
	// 現在のレイヤー状態から最も高いレイヤーを取得
	switch (get_highest_layer(state)) {
		case _SCROLL:
		// _SCROLL レイヤーが最も高い場合はスクロールモードを有効化
		keyball_set_scroll_mode(true);
		break;
		default:
		// 通常モードの場合はスクロールモードを無効化
		keyball_set_scroll_mode(false);
		break;
	}
	return state; // 更新されたレイヤー状態を返す
}

// OLED設定
#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"
#include "bmp.h"
#include "apidef.h"
#include "keyboard.h"

static const char *format_4d(int8_t d) {
	static char buf[5] = {0}; // max width (4) + NUL (1)
	char        lead   = ' ';
	if (d < 0) {
		d    = -d;
		lead = '-';
	}
	buf[3] = (d % 10) + '0';
	d /= 10;
	if (d == 0) {
		buf[2] = lead;
		lead   = ' ';
	} else {
		buf[2] = (d % 10) + '0';
		d /= 10;
	}
	if (d == 0) {
		buf[1] = lead;
		lead   = ' ';
	} else {
		buf[1] = (d % 10) + '0';
		d /= 10;
	}
	buf[0] = lead;
	return buf;
}

static const char *format_3d(int8_t d) {
	static char buf[4] = {0}; // max width (3) + NUL (1)
	char        lead   = ' ';
	if (d < 0) {
		d    = -d;
		lead = '-';
	}
	buf[2] = (d % 10) + '0';
	d /= 10;
	if (d == 0) {
		buf[1] = lead;
		lead   = ' ';
	} else {
		buf[1] = (d % 10) + '0';
		d /= 10;
	}
	buf[0] = lead;
	return buf;
}

// サブ側OLEDにもキー入力情報を送るためのメソッド
bool should_process_keypress(void) { return true; }

// サブ側OLEDの表示制御
void oledkit_render_logo_user(void){
	// 1行目 ----------------------------------------------------------
	switch(pairingId){
		case 0:
		oled_write_P(PSTR("BT0:Slave   "), false);
		break;
		
		default:
		oled_write_P(PSTR("USB:        "), false);
		break;
	}
	oled_write_P(PSTR("         "), false);
	
	// 2行目 ----------------------------------------------------------
	oled_write_P(PSTR("Layer:"), false);
	switch (get_highest_layer(layer_state | default_layer_state)) {
		case _CUSTOM:
		oled_write_P(PSTR("Custom"), false);
		break;
		case _MAIN:
		oled_write_P(PSTR("Main  "), false);
		break;
		case _NUMBER:
		oled_write_P(PSTR("Number"), false);
		break;
		case _SYMBOL:
		oled_write_P(PSTR("Symbol  "), false);
		break;
		case _BLUETOOTH:
		oled_write_P(PSTR("Blueto"), false);
		break;
		case _MOUSE:
		oled_write_P(PSTR("Mouse "), false);
		break;
		case _SCROLL:
		oled_write_P(PSTR("Scroll"), false);
		break;
		default:
		oled_write_P(PSTR("Undef "), false);
		break;
	}
	
	// 3行目 ----------------------------------------------------------
	// 4行目 ----------------------------------------------------------
}

// マウスキー判定となるキーを追加するためのメソッド
bool is_mouse_record_user(uint16_t keycode, keyrecord_t* record) {
	switch(keycode) {
		case KC_TAB:
		return true;
		default:
		return false;
	}
	return false;
}

// メイン側

// 1秒ごとに更新するためのタイマー
static uint32_t batt_update_timer = 0;

// 初期
bool isInit = true;
void oledkit_render_info_user(void) {
	if (isInit) {
		keyball_set_scroll_mode(false);
		isInit = false;
	}
	
	// 1行目: Bluetoothの接続先表示
	switch (pairingId) {
		case 0: oled_write_P(PSTR("BT0:Slave   "), false); break;
		case 1: oled_write_P(PSTR("BT1:Macbook "), false); break;
		default: oled_write_P(PSTR("USB:        "), false); break;
	}
	
	if (isScrollInvert) {
		oled_write_P(PSTR(" SCRL:Rev"), false);
	} else {
		oled_write_P(PSTR(" SCRL:Nml"), false);
	}
	
	// 2行目: レイヤー情報
	oled_write_P(PSTR("Layer:"), false);
	switch (get_highest_layer(layer_state | default_layer_state)) {
		case _CUSTOM: oled_write_P(PSTR("Custom"), false); break;
		case _MAIN: oled_write_P(PSTR("Main  "), false); break;
		case _NUMBER: oled_write_P(PSTR("Number"), false); break;
		case _BLUETOOTH: oled_write_P(PSTR("Blueto"), false); break;
		case _SYMBOL: oled_write_P(PSTR("Symbol"), false); break;
		case _MOUSE: oled_write_P(PSTR("Mouse "), false); break;
		case _SCROLL: oled_write_P(PSTR("Scroll"), false); break;
		default: oled_write_P(PSTR("Undef "), false); break;
	}
	
	oled_write_P(PSTR(" :"), false);
	oled_write_P(isJisMode ? PSTR("JIS") : PSTR(" US"), false);
	oled_write_P(isRecording ? PSTR(" REC") : PSTR("    "), false);
	
	// 3行目: オートマウス情報 CPI情報
	oled_write_P(PSTR("TM:"), false);
	oled_write(format_3d(keyball_get_total_move()), false);
	oled_write_P(PSTR("  "), false);
	oled_write_P(PSTR("CP:"), false);
	oled_write(format_4d(keyball_get_cpi()) + 1, false);
	oled_write_P(PSTR("00"), false);
	oled_write_P(PSTR("     "), false);
	
	// ★ 4行目: バッテリー電圧表示 ★ 1行21文字
	// 現在の時間を取得
	uint32_t now = timer_read32();
	oled_write_P(PSTR("BT:"), false);
	// 1秒（1000ms）経過したらバッテリー電圧を更新
	if (now - batt_update_timer >= 1000) {
		batt_update_timer = now; // タイマーをリセット
		
		char batt_str[17];  // 文字分のバッファを確保
		snprintf(batt_str, sizeof(batt_str), " %4dmV", BMPAPI->app.get_vcc_mv(0));
		
		if (bmp_config->mode == SPLIT_MASTER) {
			snprintf(batt_str + 7, sizeof(batt_str) - 7, " %4dmV", BMPAPI->app.get_vcc_mv(1));
		}
		
		oled_write(batt_str, false); // バッテリー情報を表示
	}
	oled_write_P(PSTR(" "), false);
}
#endif

// マクロの記録を開始する時に起動されます。
void dynamic_macro_record_start_user(int8_t direction){
	isRecording = true;
}
// マクロの記録を停止した時に起動されます。
void dynamic_macro_record_end_user(int8_t direction){
	isRecording = false;
}


// トラックボールのセンサ値取得時に呼ばれるイベント
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
	if(!isScrollInvert){
		mouse_report.v = mouse_report.v * -1;
	}
	
	return mouse_report;
}

// カスタムキー　自作
bool my_custom_process_record(uint16_t keycode, keyrecord_t *record) {
	
	// Shift キーと Alt キーの同時押し キーコードの挙動を設定
	switch (keycode) {
		case SFT_ALT2:
		if (record->event.pressed) {
			// キーが押されたとき
			register_code(KC_LSFT);  // 左Shiftを押す
			register_code(KC_LALT);  // 左Altを押す
		} else {
			// キーが離されたとき
			unregister_code(KC_LSFT);  // 左Shiftを離す
			unregister_code(KC_LALT);  // 左Altを離す
		}
		return false; // 他の処理を止める
	}
	
	// テラタームマクロを実行
	switch (keycode) {
		case RUN_TTM:
		if (record->event.pressed) {
			// Windowsでrun_macro.batを実行
			SEND_STRING("cmd /c start \"\" \"C:\\Projects\\TeraTermMacro\\connect_com8.ttl\"\n");
		}
		return false;
	}
	
	// スクロール方向反転
	// switch (keycode) {
	// 	case SCRL_REV:
	// 	if (record->event.pressed) {
	// 		// add keyball_toggle_scroll_reverse
	// 		keyball_toggle_scroll_reverse();
	// 		break;
	// 	}
	// 	return false;
	// }
	
	return true; // 他のキーコードを通常通り処理
}

// カスタムキー定義
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	
	return my_custom_process_record(keycode, record);
	
	switch (keycode) {
		
		case AD_WO_L:
		return true;
		
		case ADV_ID0:
		pairingId = 0;
		return true;
		
		case ADV_ID1:
		pairingId = 1;
		return true;
		
		case ADV_ID2:
		pairingId = 2;
		return true;
		
		case ADV_ID3:
		pairingId = 3;
		return true;
		
		case ADV_ID4:
		pairingId = 4;
		return true;
		
		case ADV_ID5:
		pairingId = 5;
		return true;
		
		case ADV_ID6:
		pairingId = 6;
		return true;
		
		case ADV_ID7:
		pairingId = 7;
		return true;
		
		case SEL_BLE:
		return true;
		
		case SEL_USB:
		pairingId = -1;
		return true;
		
		default:
		return true;
	}
	return true;
}

/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/