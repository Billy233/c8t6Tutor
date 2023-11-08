
/***************************************************************************************
  * 鏈�绋嬪簭鐢辨睙鍗忕�戞妧鍒涘缓骞跺厤璐瑰紑婧愬叡浜�
  * 浣犲彲浠ヤ换鎰忔煡鐪嬨€佷娇鐢ㄥ拰淇�鏀癸紝骞跺簲鐢ㄥ埌鑷�宸辩殑椤圭洰涔嬩腑
  * 绋嬪簭鐗堟潈褰掓睙鍗忕�戞妧鎵€鏈夛紝浠讳綍浜烘垨缁勭粐涓嶅緱灏嗗叾鎹�涓哄繁鏈�
  * 
  * 绋嬪簭鍚嶇О锛�				0.96瀵窸LED鏄剧ず灞忛┍鍔ㄧ▼搴忥紙4閽堣剼I2C鎺ュ彛锛�
  * 绋嬪簭鍒涘缓鏃堕棿锛�			2023.10.24
  * 褰撳墠绋嬪簭鐗堟湰锛�			V0.0娴嬭瘯鐗�
  * 褰撳墠鐗堟湰鍙戝竷鏃堕棿锛�		2023.10.31
  * 
  * 姹熷崗绉戞妧瀹樻柟缃戠珯锛�		jiangxiekeji.com
  * 姹熷崗绉戞妧瀹樻柟娣樺疂搴楋細	jiangxiekeji.taobao.com
  * 绋嬪簭浠嬬粛鍙婃洿鏂板姩鎬侊細	jiangxiekeji.com/tutorial/oled.html
  * 
  * 濡傛灉浣犲彂鐜扮▼搴忎腑鐨勬紡娲炴垨鑰呯瑪璇�锛屽彲閫氳繃閭�浠跺悜鎴戜滑鍙嶉�堬細feedback@jiangxiekeji.com
  * 鍙戦€侀偖浠朵箣鍓嶏紝浣犲彲浠ュ厛鍒版洿鏂板姩鎬侀〉闈㈡煡鐪嬫渶鏂扮▼搴忥紝濡傛灉姝ら棶棰樺凡缁忎慨鏀癸紝鍒欐棤闇€鍐嶅彂閭�浠�
  ***************************************************************************************
  */

#include "stm32f10x.h"
#include "OLED.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>

/**
  * 鏁版嵁瀛樺偍鏍煎紡锛�
  * 绾靛悜8鐐癸紝楂樹綅鍦ㄤ笅锛屽厛浠庡乏鍒板彸锛屽啀浠庝笂鍒颁笅
  * 姣忎竴涓狟it瀵瑰簲涓€涓�鍍忕礌鐐�
  * 
  *      B0 B0                  B0 B0
  *      B1 B1                  B1 B1
  *      B2 B2                  B2 B2
  *      B3 B3  ------------->  B3 B3 --
  *      B4 B4                  B4 B4  |
  *      B5 B5                  B5 B5  |
  *      B6 B6                  B6 B6  |
  *      B7 B7                  B7 B7  |
  *                                    |
  *  -----------------------------------
  *  |   
  *  |   B0 B0                  B0 B0
  *  |   B1 B1                  B1 B1
  *  |   B2 B2                  B2 B2
  *  --> B3 B3  ------------->  B3 B3
  *      B4 B4                  B4 B4
  *      B5 B5                  B5 B5
  *      B6 B6                  B6 B6
  *      B7 B7                  B7 B7
  * 
  * 鍧愭爣杞村畾涔夛細
  * 宸︿笂瑙掍负(0, 0)鐐�
  * 妯�鍚戝悜鍙充负X杞达紝鍙栧€艰寖鍥达細0~127
  * 绾靛悜鍚戜笅涓篩杞达紝鍙栧€艰寖鍥达細0~63
  * 
  *       0             X杞�           127 
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  Y杞� |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  * 
  */


/*鍏ㄥ眬鍙橀噺*********************/

/**
  * OLED鏄惧瓨鏁扮粍
  * 鎵€鏈夌殑鏄剧ず鍑芥暟锛岄兘鍙�鏄�瀵规�ゆ樉瀛樻暟缁勮繘琛岃�诲啓
  * 闅忓悗璋冪敤OLED_Update鍑芥暟鎴朞LED_UpdateArea鍑芥暟
  * 鎵嶄細灏嗘樉瀛樻暟缁勭殑鏁版嵁鍙戦€佸埌OLED纭�浠讹紝杩涜�屾樉绀�
  */
uint8_t OLED_DisplayBuf[8][128];

/*********************鍏ㄥ眬鍙橀噺*/


/*寮曡剼閰嶇疆*********************/

/**
  * 鍑�    鏁帮細OLED鍐橲CL楂樹綆鐢靛钩
  * 鍙�    鏁帮細瑕佸啓鍏�SCL鐨勭數骞冲€硷紝鑼冨洿锛�0/1
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細褰撲笂灞傚嚱鏁伴渶瑕佸啓SCL鏃讹紝姝ゅ嚱鏁颁細琚�璋冪敤
  *           鐢ㄦ埛闇€瑕佹牴鎹�鍙傛暟浼犲叆鐨勫€硷紝灏哠CL缃�涓洪珮鐢靛钩鎴栬€呬綆鐢靛钩
  *           褰撳弬鏁颁紶鍏�0鏃讹紝缃甋CL涓轰綆鐢靛钩锛屽綋鍙傛暟浼犲叆1鏃讹紝缃甋CL涓洪珮鐢靛钩
  */
void OLED_W_SCL(uint8_t BitValue)
{
	/*鏍规嵁BitValue鐨勫€硷紝灏哖B8锛圫CL锛夌疆楂樼數骞虫垨鑰呬綆鐢靛钩*/
	GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)BitValue);
	
	/*濡傛灉鍗曠墖鏈洪€熷害杩囧揩锛屽彲鍦ㄦ�ゆ坊鍔犻€傞噺寤舵椂锛屼互閬垮厤瓒呭嚭I2C閫氫俊鐨勬渶澶ч€熷害*/
	//...
}

/**
  * 鍑�    鏁帮細OLED鍐橲DA楂樹綆鐢靛钩
  * 鍙�    鏁帮細瑕佸啓鍏�SDA鐨勭數骞冲€硷紝鑼冨洿锛�0/1
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細褰撲笂灞傚嚱鏁伴渶瑕佸啓SDA鏃讹紝姝ゅ嚱鏁颁細琚�璋冪敤
  *           鐢ㄦ埛闇€瑕佹牴鎹�鍙傛暟浼犲叆鐨勫€硷紝灏哠DA缃�涓洪珮鐢靛钩鎴栬€呬綆鐢靛钩
  *           褰撳弬鏁颁紶鍏�0鏃讹紝缃甋DA涓轰綆鐢靛钩锛屽綋鍙傛暟浼犲叆1鏃讹紝缃甋DA涓洪珮鐢靛钩
  */
void OLED_W_SDA(uint8_t BitValue)
{
	/*鏍规嵁BitValue鐨勫€硷紝灏哖B9锛圫DA锛夌疆楂樼數骞虫垨鑰呬綆鐢靛钩*/
	GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)BitValue);
	
	/*濡傛灉鍗曠墖鏈洪€熷害杩囧揩锛屽彲鍦ㄦ�ゆ坊鍔犻€傞噺寤舵椂锛屼互閬垮厤瓒呭嚭I2C閫氫俊鐨勬渶澶ч€熷害*/
	//...
}

/**
  * 鍑�    鏁帮細OLED寮曡剼鍒濆�嬪寲
  * 鍙�    鏁帮細鏃�
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細褰撲笂灞傚嚱鏁伴渶瑕佸垵濮嬪寲鏃讹紝姝ゅ嚱鏁颁細琚�璋冪敤
  *           鐢ㄦ埛闇€瑕佸皢SCL鍜孲DA寮曡剼鍒濆�嬪寲涓哄紑婕忔ā寮忥紝骞堕噴鏀惧紩鑴�
  */
void OLED_GPIO_Init(void)
{
	uint32_t i, j;
	
	/*鍦ㄥ垵濮嬪寲鍓嶏紝鍔犲叆閫傞噺寤舵椂锛屽緟OLED渚涚數绋冲畾*/
	for (i = 0; i < 1000; i++)
	{
		for (j = 0; j < 1000; j++);
	}
	
	/*灏哠CL鍜孲DA寮曡剼鍒濆�嬪寲涓哄紑婕忔ā寮�*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*閲婃斁SCL鍜孲DA*/
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/*********************寮曡剼閰嶇疆*/


/*閫氫俊鍗忚��*********************/

/**
  * 鍑�    鏁帮細I2C璧峰��
  * 鍙�    鏁帮細鏃�
  * 杩� 鍥� 鍊硷細鏃�
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);		//閲婃斁SDA锛岀‘淇漇DA涓洪珮鐢靛钩
	OLED_W_SCL(1);		//閲婃斁SCL锛岀‘淇漇CL涓洪珮鐢靛钩
	OLED_W_SDA(0);		//鍦⊿CL楂樼數骞虫湡闂达紝鎷変綆SDA锛屼骇鐢熻捣濮嬩俊鍙�
	OLED_W_SCL(0);		//璧峰�嬪悗鎶奡CL涔熸媺浣庯紝鍗充负浜嗗崰鐢ㄦ€荤嚎锛屼篃涓轰簡鏂逛究鎬荤嚎鏃跺簭鐨勬嫾鎺�
}

/**
  * 鍑�    鏁帮細I2C缁堟��
  * 鍙�    鏁帮細鏃�
  * 杩� 鍥� 鍊硷細鏃�
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);		//鎷変綆SDA锛岀‘淇漇DA涓轰綆鐢靛钩
	OLED_W_SCL(1);		//閲婃斁SCL锛屼娇SCL鍛堢幇楂樼數骞�
	OLED_W_SDA(1);		//鍦⊿CL楂樼數骞虫湡闂达紝閲婃斁SDA锛屼骇鐢熺粓姝�淇″彿
}

/**
  * 鍑�    鏁帮細I2C鍙戦€佷竴涓�瀛楄妭
  * 鍙�    鏁帮細Byte 瑕佸彂閫佺殑涓€涓�瀛楄妭鏁版嵁锛岃寖鍥达細0x00~0xFF
  * 杩� 鍥� 鍊硷細鏃�
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	
	/*寰�鐜�8娆★紝涓绘満渚濇�″彂閫佹暟鎹�鐨勬瘡涓€浣�*/
	for (i = 0; i < 8; i++)
	{
		/*浣跨敤鎺╃爜鐨勬柟寮忓彇鍑築yte鐨勬寚瀹氫竴浣嶆暟鎹�骞跺啓鍏ュ埌SDA绾�*/
		/*涓や釜!鐨勪綔鐢ㄦ槸锛岃�╂墍鏈夐潪闆剁殑鍊煎彉涓�1*/
		OLED_W_SDA(!!(Byte & (0x80 >> i)));
		OLED_W_SCL(1);	//閲婃斁SCL锛屼粠鏈哄湪SCL楂樼數骞虫湡闂磋�诲彇SDA
		OLED_W_SCL(0);	//鎷変綆SCL锛屼富鏈哄紑濮嬪彂閫佷笅涓€浣嶆暟鎹�
	}
	
	OLED_W_SCL(1);		//棰濆�栫殑涓€涓�鏃堕挓锛屼笉澶勭悊搴旂瓟淇″彿
	OLED_W_SCL(0);
}

/**
  * 鍑�    鏁帮細OLED鍐欏懡浠�
  * 鍙�    鏁帮細Command 瑕佸啓鍏ョ殑鍛戒护鍊硷紝鑼冨洿锛�0x00~0xFF
  * 杩� 鍥� 鍊硷細鏃�
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();				//I2C璧峰��
	OLED_I2C_SendByte(0x78);		//鍙戦€丱LED鐨処2C浠庢満鍦板潃
	OLED_I2C_SendByte(0x00);		//鎺у埗瀛楄妭锛岀粰0x00锛岃〃绀哄嵆灏嗗啓鍛戒护
	OLED_I2C_SendByte(Command);		//鍐欏叆鎸囧畾鐨勫懡浠�
	OLED_I2C_Stop();				//I2C缁堟��
}

/**
  * 鍑�    鏁帮細OLED鍐欐暟鎹�
  * 鍙�    鏁帮細Data 瑕佸啓鍏ユ暟鎹�鐨勮捣濮嬪湴鍧€
  * 鍙�    鏁帮細Count 瑕佸啓鍏ユ暟鎹�鐨勬暟閲�
  * 杩� 鍥� 鍊硷細鏃�
  */
void OLED_WriteData(uint8_t *Data, uint8_t Count)
{
	uint8_t i;
	
	OLED_I2C_Start();				//I2C璧峰��
	OLED_I2C_SendByte(0x78);		//鍙戦€丱LED鐨処2C浠庢満鍦板潃
	OLED_I2C_SendByte(0x40);		//鎺у埗瀛楄妭锛岀粰0x40锛岃〃绀哄嵆灏嗗啓鏁伴噺
	/*寰�鐜疌ount娆★紝杩涜�岃繛缁�鐨勬暟鎹�鍐欏叆*/
	for (i = 0; i < Count; i ++)
	{
		OLED_I2C_SendByte(Data[i]);	//渚濇�″彂閫丏ata鐨勬瘡涓€涓�鏁版嵁
	}
	OLED_I2C_Stop();				//I2C缁堟��
}

/*********************閫氫俊鍗忚��*/


/*纭�浠堕厤缃�*********************/

/**
  * 鍑�    鏁帮細OLED鍒濆�嬪寲
  * 鍙�    鏁帮細鏃�
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細浣跨敤鍓嶏紝闇€瑕佽皟鐢ㄦ�ゅ垵濮嬪寲鍑芥暟
  */
void OLED_Init(void)
{
	OLED_GPIO_Init();			//鍏堣皟鐢ㄥ簳灞傜殑绔�鍙ｅ垵濮嬪寲
	
	/*鍐欏叆涓€绯诲垪鐨勫懡浠わ紝瀵筄LED杩涜�屽垵濮嬪寲閰嶇疆*/
	OLED_WriteCommand(0xAE);	//璁剧疆鏄剧ず寮€鍚�/鍏抽棴锛�0xAE鍏抽棴锛�0xAF寮€鍚�
	
	OLED_WriteCommand(0xD5);	//璁剧疆鏄剧ず鏃堕挓鍒嗛�戞瘮/鎸�鑽″櫒棰戠巼
	OLED_WriteCommand(0x80);	//0x00~0xFF
	
	OLED_WriteCommand(0xA8);	//璁剧疆澶氳矾澶嶇敤鐜�
	OLED_WriteCommand(0x3F);	//0x0E~0x3F
	
	OLED_WriteCommand(0xD3);	//璁剧疆鏄剧ず鍋忕Щ
	OLED_WriteCommand(0x00);	//0x00~0x7F
	
	OLED_WriteCommand(0x40);	//璁剧疆鏄剧ず寮€濮嬭�岋紝0x40~0x7F
	
	OLED_WriteCommand(0xA1);	//璁剧疆宸﹀彸鏂瑰悜锛�0xA1姝ｅ父锛�0xA0宸﹀彸鍙嶇疆
	
	OLED_WriteCommand(0xC8);	//璁剧疆涓婁笅鏂瑰悜锛�0xC8姝ｅ父锛�0xC0涓婁笅鍙嶇疆

	OLED_WriteCommand(0xDA);	//璁剧疆COM寮曡剼纭�浠堕厤缃�
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//璁剧疆瀵规瘮搴�
	OLED_WriteCommand(0xCF);	//0x00~0xFF

	OLED_WriteCommand(0xD9);	//璁剧疆棰勫厖鐢靛懆鏈�
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//璁剧疆VCOMH鍙栨秷閫夋嫨绾у埆
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//璁剧疆鏁翠釜鏄剧ず鎵撳紑/鍏抽棴

	OLED_WriteCommand(0xA6);	//璁剧疆姝ｅ父/鍙嶈壊鏄剧ず锛�0xA6姝ｅ父锛�0xA7鍙嶈壊

	OLED_WriteCommand(0x8D);	//璁剧疆鍏呯數娉�
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//寮€鍚�鏄剧ず
}

/**
  * 鍑�    鏁帮細OLED璁剧疆鏄剧ず鍏夋爣浣嶇疆
  * 鍙�    鏁帮細Page 鎸囧畾鍏夋爣鎵€鍦ㄧ殑椤碉紝鑼冨洿锛�0~7
  * 鍙�    鏁帮細X 鎸囧畾鍏夋爣鎵€鍦ㄧ殑X杞村潗鏍囷紝鑼冨洿锛�0~127
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細OLED榛樿�ょ殑Y杞达紝鍙�鑳�8涓狟it涓轰竴缁勫啓鍏ワ紝鍗�1椤电瓑浜�8涓猋杞村潗鏍�
  */
void OLED_SetCursor(uint8_t Page, uint8_t X)
{
	/*濡傛灉浣跨敤姝ょ▼搴忛┍鍔�1.3瀵哥殑OLED鏄剧ず灞忥紝鍒欓渶瑕佽В闄ゆ�ゆ敞閲�*/
	/*鍥犱负1.3瀵哥殑OLED椹卞姩鑺�鐗囷紙SH1106锛夋湁132鍒�*/
	/*灞忓箷鐨勮捣濮嬪垪鎺ュ湪浜嗙��2鍒楋紝鑰屼笉鏄�绗�0鍒�*/
	/*鎵€浠ラ渶瑕佸皢X鍔�2锛屾墠鑳芥�ｅ父鏄剧ず*/
//	X += 2;
	
	/*閫氳繃鎸囦护璁剧疆椤靛湴鍧€鍜屽垪鍦板潃*/
	OLED_WriteCommand(0xB0 | Page);					//璁剧疆椤典綅缃�
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//璁剧疆X浣嶇疆楂�4浣�
	OLED_WriteCommand(0x00 | (X & 0x0F));			//璁剧疆X浣嶇疆浣�4浣�
}

/*********************纭�浠堕厤缃�*/


/*宸ュ叿鍑芥暟*********************/

/*宸ュ叿鍑芥暟浠呬緵鍐呴儴閮ㄥ垎鍑芥暟浣跨敤*/

/**
  * 鍑�    鏁帮細娆℃柟鍑芥暟
  * 鍙�    鏁帮細X 搴曟暟
  * 鍙�    鏁帮細Y 鎸囨暟
  * 杩� 鍥� 鍊硷細绛変簬X鐨刌娆℃柟
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//缁撴灉榛樿�や负1
	while (Y --)			//绱�涔榊娆�
	{
		Result *= X;		//姣忔�℃妸X绱�涔樺埌缁撴灉涓�
	}
	return Result;
}

/**
  * 鍑�    鏁帮細鍒ゆ柇鎸囧畾鐐规槸鍚﹀湪鎸囧畾澶氳竟褰㈠唴閮�
  * 鍙�    鏁帮細nvert 澶氳竟褰㈢殑椤剁偣鏁�
  * 鍙�    鏁帮細vertx verty 鍖呭惈澶氳竟褰㈤《鐐圭殑x鍜寉鍧愭爣鐨勬暟缁�
  * 鍙�    鏁帮細testx testy 娴嬭瘯鐐圭殑X鍜寉鍧愭爣
  * 杩� 鍥� 鍊硷細鎸囧畾鐐规槸鍚﹀湪鎸囧畾澶氳竟褰㈠唴閮�锛�1锛氬湪鍐呴儴锛�0锛氫笉鍦ㄥ唴閮�
  */
uint8_t OLED_pnpoly(uint8_t nvert, int16_t *vertx, int16_t *verty, int16_t testx, int16_t testy)
{
	int16_t i, j, c = 0;
	
	/*姝ょ畻娉曠敱W. Randolph Franklin鎻愬嚭*/
	/*鍙傝€冮摼鎺ワ細https://wrfranklin.org/Research/Short_Notes/pnpoly.html*/
	for (i = 0, j = nvert - 1; i < nvert; j = i++)
	{
		if (((verty[i] > testy) != (verty[j] > testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
		{
			c = !c;
		}
	}
	return c;
}

/**
  * 鍑�    鏁帮細鍒ゆ柇鎸囧畾鐐规槸鍚﹀湪鎸囧畾瑙掑害鍐呴儴
  * 鍙�    鏁帮細X Y 鎸囧畾鐐圭殑鍧愭爣
  * 鍙�    鏁帮細StartAngle EndAngle 璧峰�嬭�掑害鍜岀粓姝㈣�掑害锛岃寖鍥达細-180~180
  *           姘村钩鍚戝彸涓�0搴︼紝姘村钩鍚戝乏涓�180搴︽垨-180搴︼紝涓嬫柟涓烘�ｆ暟锛屼笂鏂逛负璐熸暟锛岄『鏃堕拡鏃嬭浆
  * 杩� 鍥� 鍊硷細鎸囧畾鐐规槸鍚﹀湪鎸囧畾瑙掑害鍐呴儴锛�1锛氬湪鍐呴儴锛�0锛氫笉鍦ㄥ唴閮�
  */
uint8_t OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle)
{
	int16_t PointAngle;
	PointAngle = atan2(Y, X) / 3.14 * 180;	//璁＄畻鎸囧畾鐐圭殑寮у害锛屽苟杞�鎹�涓鸿�掑害琛ㄧず
	if (StartAngle < EndAngle)	//璧峰�嬭�掑害灏忎簬缁堟�㈣�掑害鐨勬儏鍐�
	{
		/*濡傛灉鎸囧畾瑙掑害鍦ㄨ捣濮嬬粓姝㈣�掑害涔嬮棿锛屽垯鍒ゅ畾鎸囧畾鐐瑰湪鎸囧畾瑙掑害*/
		if (PointAngle > StartAngle && PointAngle < EndAngle)
		{
			return 1;
		}
	}
	else			//璧峰�嬭�掑害澶т簬浜庣粓姝㈣�掑害鐨勬儏鍐�
	{
		/*濡傛灉鎸囧畾瑙掑害澶т簬璧峰�嬭�掑害鎴栬€呭皬浜庣粓姝㈣�掑害锛屽垯鍒ゅ畾鎸囧畾鐐瑰湪鎸囧畾瑙掑害*/
		if (PointAngle > StartAngle || PointAngle < EndAngle)
		{
			return 1;
		}
	}
	return 0;		//涓嶆弧瓒充互涓婃潯浠讹紝鍒欏垽鏂�鍒ゅ畾鎸囧畾鐐逛笉鍦ㄦ寚瀹氳�掑害
}

/*********************宸ュ叿鍑芥暟*/


/*鍔熻兘鍑芥暟*********************/

/**
  * 鍑�    鏁帮細灏哋LED鏄惧瓨鏁扮粍鏇存柊鍒癘LED灞忓箷
  * 鍙�    鏁帮細鏃�
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細鎵€鏈夌殑鏄剧ず鍑芥暟锛岄兘鍙�鏄�瀵筄LED鏄惧瓨鏁扮粍杩涜�岃�诲啓
  *           闅忓悗璋冪敤OLED_Update鍑芥暟鎴朞LED_UpdateArea鍑芥暟
  *           鎵嶄細灏嗘樉瀛樻暟缁勭殑鏁版嵁鍙戦€佸埌OLED纭�浠讹紝杩涜�屾樉绀�
  *           鏁呰皟鐢ㄦ樉绀哄嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_Update(void)
{
	uint8_t j;
	/*閬嶅巻姣忎竴椤�*/
	for (j = 0; j < 8; j ++)
	{
		/*璁剧疆鍏夋爣浣嶇疆涓烘瘡涓€椤电殑绗�涓€鍒�*/
		OLED_SetCursor(j, 0);
		/*杩炵画鍐欏叆128涓�鏁版嵁锛屽皢鏄惧瓨鏁扮粍鐨勬暟鎹�鍐欏叆鍒癘LED纭�浠�*/
		OLED_WriteData(OLED_DisplayBuf[j], 128);
	}
}

/**
  * 鍑�    鏁帮細灏哋LED鏄惧瓨鏁扮粍閮ㄥ垎鏇存柊鍒癘LED灞忓箷
  * 鍙�    鏁帮細X 鎸囧畾鍖哄煙宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鍖哄煙宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Width 鎸囧畾鍖哄煙鐨勫�藉害锛岃寖鍥达細0~128
  * 鍙�    鏁帮細Height 鎸囧畾鍖哄煙鐨勯珮搴︼紝鑼冨洿锛�0~64
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細姝ゅ嚱鏁颁細鑷冲皯鏇存柊鍙傛暟鎸囧畾鐨勫尯鍩�
  *           濡傛灉鏇存柊鍖哄煙Y杞村彧鍖呭惈閮ㄥ垎椤碉紝鍒欏悓涓€椤电殑鍓╀綑閮ㄥ垎浼氳窡闅忎竴璧锋洿鏂�
  * 璇�    鏄庯細鎵€鏈夌殑鏄剧ず鍑芥暟锛岄兘鍙�鏄�瀵筄LED鏄惧瓨鏁扮粍杩涜�岃�诲啓
  *           闅忓悗璋冪敤OLED_Update鍑芥暟鎴朞LED_UpdateArea鍑芥暟
  *           鎵嶄細灏嗘樉瀛樻暟缁勭殑鏁版嵁鍙戦€佸埌OLED纭�浠讹紝杩涜�屾樉绀�
  *           鏁呰皟鐢ㄦ樉绀哄嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t j;
	
	/*鍙傛暟妫€鏌ワ紝淇濊瘉鎸囧畾鍖哄煙涓嶄細瓒呭嚭灞忓箷鑼冨洿*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}
	
	/*閬嶅巻鎸囧畾鍖哄煙娑夊強鐨勭浉鍏抽〉*/
	/*(Y + Height - 1) / 8 + 1鐨勭洰鐨勬槸(Y + Height) / 8骞跺悜涓婂彇鏁�*/
	for (j = Y / 8; j < (Y + Height - 1) / 8 + 1; j ++)
	{
		/*璁剧疆鍏夋爣浣嶇疆涓虹浉鍏抽〉鐨勬寚瀹氬垪*/
		OLED_SetCursor(j, X);
		/*杩炵画鍐欏叆Width涓�鏁版嵁锛屽皢鏄惧瓨鏁扮粍鐨勬暟鎹�鍐欏叆鍒癘LED纭�浠�*/
		OLED_WriteData(&OLED_DisplayBuf[j][X], Width);
	}
}

/**
  * 鍑�    鏁帮細灏哋LED鏄惧瓨鏁扮粍鍏ㄩ儴娓呴浂
  * 鍙�    鏁帮細鏃�
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_Clear(void)
{
	uint8_t i, j;
	for (j = 0; j < 8; j ++)				//閬嶅巻8椤�
	{
		for (i = 0; i < 128; i ++)			//閬嶅巻128鍒�
		{
			OLED_DisplayBuf[j][i] = 0x00;	//灏嗘樉瀛樻暟缁勬暟鎹�鍏ㄩ儴娓呴浂
		}
	}
}

/**
  * 鍑�    鏁帮細灏哋LED鏄惧瓨鏁扮粍閮ㄥ垎娓呴浂
  * 鍙�    鏁帮細X 鎸囧畾鍖哄煙宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鍖哄煙宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Width 鎸囧畾鍖哄煙鐨勫�藉害锛岃寖鍥达細0~128
  * 鍙�    鏁帮細Height 鎸囧畾鍖哄煙鐨勯珮搴︼紝鑼冨洿锛�0~64
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_ClearArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t i, j;
	
	/*鍙傛暟妫€鏌ワ紝淇濊瘉鎸囧畾鍖哄煙涓嶄細瓒呭嚭灞忓箷鑼冨洿*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}
	
	for (j = Y; j < Y + Height; j ++)		//閬嶅巻鎸囧畾椤�
	{
		for (i = X; i < X + Width; i ++)	//閬嶅巻鎸囧畾鍒�
		{
			OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8));	//灏嗘樉瀛樻暟缁勬寚瀹氭暟鎹�娓呴浂
		}
	}
}

/**
  * 鍑�    鏁帮細灏哋LED鏄惧瓨鏁扮粍鍏ㄩ儴鍙栧弽
  * 鍙�    鏁帮細鏃�
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_Reverse(void)
{
	uint8_t i, j;
	for (j = 0; j < 8; j ++)				//閬嶅巻8椤�
	{
		for (i = 0; i < 128; i ++)			//閬嶅巻128鍒�
		{
			OLED_DisplayBuf[j][i] ^= 0xFF;	//灏嗘樉瀛樻暟缁勬暟鎹�鍏ㄩ儴鍙栧弽
		}
	}
}
	
/**
  * 鍑�    鏁帮細灏哋LED鏄惧瓨鏁扮粍閮ㄥ垎鍙栧弽
  * 鍙�    鏁帮細X 鎸囧畾鍖哄煙宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鍖哄煙宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Width 鎸囧畾鍖哄煙鐨勫�藉害锛岃寖鍥达細0~128
  * 鍙�    鏁帮細Height 鎸囧畾鍖哄煙鐨勯珮搴︼紝鑼冨洿锛�0~64
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_ReverseArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t i, j;
	
	/*鍙傛暟妫€鏌ワ紝淇濊瘉鎸囧畾鍖哄煙涓嶄細瓒呭嚭灞忓箷鑼冨洿*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}
	
	for (j = Y; j < Y + Height; j ++)		//閬嶅巻鎸囧畾椤�
	{
		for (i = X; i < X + Width; i ++)	//閬嶅巻鎸囧畾鍒�
		{
			OLED_DisplayBuf[j / 8][i] ^= 0x01 << (j % 8);	//灏嗘樉瀛樻暟缁勬寚瀹氭暟鎹�鍙栧弽
		}
	}
}

/**
  * 鍑�    鏁帮細OLED鏄剧ず涓€涓�瀛楃��
  * 鍙�    鏁帮細X 鎸囧畾瀛楃�﹀乏涓婅�掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾瀛楃�﹀乏涓婅�掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Char 鎸囧畾瑕佹樉绀虹殑瀛楃�︼紝鑼冨洿锛欰SCII鐮佸彲瑙佸瓧绗�
  * 鍙�    鏁帮細FontSize 鎸囧畾瀛椾綋澶у皬
  *           鑼冨洿锛歄LED_8X16		瀹�8鍍忕礌锛岄珮16鍍忕礌
  *                 OLED_6X8		瀹�6鍍忕礌锛岄珮8鍍忕礌
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize)
{
	if (FontSize == OLED_8X16)		//瀛椾綋涓哄��8鍍忕礌锛岄珮16鍍忕礌
	{
		/*灏咥SCII瀛楁ā搴揙LED_F8x16鐨勬寚瀹氭暟鎹�浠�8*16鐨勫浘鍍忔牸寮忔樉绀�*/
		OLED_ShowImage(X, Y, 8, 16, OLED_F8x16[Char - ' ']);
	}
	else if(FontSize == OLED_6X8)	//瀛椾綋涓哄��6鍍忕礌锛岄珮8鍍忕礌
	{
		/*灏咥SCII瀛楁ā搴揙LED_F6x8鐨勬寚瀹氭暟鎹�浠�6*8鐨勫浘鍍忔牸寮忔樉绀�*/
		OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
	}
}

/**
  * 鍑�    鏁帮細OLED鏄剧ず瀛楃�︿覆
  * 鍙�    鏁帮細X 鎸囧畾瀛楃�︿覆宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾瀛楃�︿覆宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細String 鎸囧畾瑕佹樉绀虹殑瀛楃�︿覆锛岃寖鍥达細ASCII鐮佸彲瑙佸瓧绗︾粍鎴愮殑瀛楃�︿覆
  * 鍙�    鏁帮細FontSize 鎸囧畾瀛椾綋澶у皬
  *           鑼冨洿锛歄LED_8X16		瀹�8鍍忕礌锛岄珮16鍍忕礌
  *                 OLED_6X8		瀹�6鍍忕礌锛岄珮8鍍忕礌
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_ShowString(uint8_t X, uint8_t Y, char *String, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)		//閬嶅巻瀛楃�︿覆鐨勬瘡涓�瀛楃��
	{
		/*璋冪敤OLED_ShowChar鍑芥暟锛屼緷娆℃樉绀烘瘡涓�瀛楃��*/
		OLED_ShowChar(X + i * FontSize, Y, String[i], FontSize);
	}
}

/**
  * 鍑�    鏁帮細OLED鏄剧ず鏁板瓧锛堝崄杩涘埗锛屾�ｆ暣鏁帮級
  * 鍙�    鏁帮細X 鎸囧畾鏁板瓧宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鏁板瓧宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Number 鎸囧畾瑕佹樉绀虹殑鏁板瓧锛岃寖鍥达細0~4294967295
  * 鍙�    鏁帮細Length 鎸囧畾鏁板瓧鐨勯暱搴︼紝鑼冨洿锛�0~10
  * 鍙�    鏁帮細FontSize 鎸囧畾瀛椾綋澶у皬
  *           鑼冨洿锛歄LED_8X16		瀹�8鍍忕礌锛岄珮16鍍忕礌
  *                 OLED_6X8		瀹�6鍍忕礌锛岄珮8鍍忕礌
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_ShowNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; i < Length; i++)		//閬嶅巻鏁板瓧鐨勬瘡涓€浣�							
	{
		/*璋冪敤OLED_ShowChar鍑芥暟锛屼緷娆℃樉绀烘瘡涓�鏁板瓧*/
		/*Number / OLED_Pow(10, Length - i - 1) % 10 鍙�浠ュ崄杩涘埗鎻愬彇鏁板瓧鐨勬瘡涓€浣�*/
		/*+ '0' 鍙�灏嗘暟瀛楄浆鎹�涓哄瓧绗︽牸寮�*/
		OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
	}
}

/**
  * 鍑�    鏁帮細OLED鏄剧ず鏈夌�﹀彿鏁板瓧锛堝崄杩涘埗锛屾暣鏁帮級
  * 鍙�    鏁帮細X 鎸囧畾鏁板瓧宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鏁板瓧宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Number 鎸囧畾瑕佹樉绀虹殑鏁板瓧锛岃寖鍥达細-2147483648~2147483647
  * 鍙�    鏁帮細Length 鎸囧畾鏁板瓧鐨勯暱搴︼紝鑼冨洿锛�0~10
  * 鍙�    鏁帮細FontSize 鎸囧畾瀛椾綋澶у皬
  *           鑼冨洿锛歄LED_8X16		瀹�8鍍忕礌锛岄珮16鍍忕礌
  *                 OLED_6X8		瀹�6鍍忕礌锛岄珮8鍍忕礌
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	uint32_t Number1;
	
	if (Number >= 0)						//鏁板瓧澶т簬绛変簬0
	{
		OLED_ShowChar(X, Y, '+', FontSize);	//鏄剧ず+鍙�
		Number1 = Number;					//Number1鐩存帴绛変簬Number
	}
	else									//鏁板瓧灏忎簬0
	{
		OLED_ShowChar(X, Y, '-', FontSize);	//鏄剧ず-鍙�
		Number1 = -Number;					//Number1绛変簬Number鍙栬礋
	}
	
	for (i = 0; i < Length; i++)			//閬嶅巻鏁板瓧鐨勬瘡涓€浣�								
	{
		/*璋冪敤OLED_ShowChar鍑芥暟锛屼緷娆℃樉绀烘瘡涓�鏁板瓧*/
		/*Number1 / OLED_Pow(10, Length - i - 1) % 10 鍙�浠ュ崄杩涘埗鎻愬彇鏁板瓧鐨勬瘡涓€浣�*/
		/*+ '0' 鍙�灏嗘暟瀛楄浆鎹�涓哄瓧绗︽牸寮�*/
		OLED_ShowChar(X + (i + 1) * FontSize, Y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
	}
}

/**
  * 鍑�    鏁帮細OLED鏄剧ず鍗佸叚杩涘埗鏁板瓧锛堝崄鍏�杩涘埗锛屾�ｆ暣鏁帮級
  * 鍙�    鏁帮細X 鎸囧畾鏁板瓧宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鏁板瓧宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Number 鎸囧畾瑕佹樉绀虹殑鏁板瓧锛岃寖鍥达細0x00000000~0xFFFFFFFF
  * 鍙�    鏁帮細Length 鎸囧畾鏁板瓧鐨勯暱搴︼紝鑼冨洿锛�0~8
  * 鍙�    鏁帮細FontSize 鎸囧畾瀛椾綋澶у皬
  *           鑼冨洿锛歄LED_8X16		瀹�8鍍忕礌锛岄珮16鍍忕礌
  *                 OLED_6X8		瀹�6鍍忕礌锛岄珮8鍍忕礌
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_ShowHexNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)		//閬嶅巻鏁板瓧鐨勬瘡涓€浣�
	{
		/*浠ュ崄鍏�杩涘埗鎻愬彇鏁板瓧鐨勬瘡涓€浣�*/
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		
		if (SingleNumber < 10)			//鍗曚釜鏁板瓧灏忎簬10
		{
			/*璋冪敤OLED_ShowChar鍑芥暟锛屾樉绀烘�ゆ暟瀛�*/
			/*+ '0' 鍙�灏嗘暟瀛楄浆鎹�涓哄瓧绗︽牸寮�*/
			OLED_ShowChar(X + i * FontSize, Y, SingleNumber + '0', FontSize);
		}
		else							//鍗曚釜鏁板瓧澶т簬10
		{
			/*璋冪敤OLED_ShowChar鍑芥暟锛屾樉绀烘�ゆ暟瀛�*/
			/*+ 'A' 鍙�灏嗘暟瀛楄浆鎹�涓轰粠A寮€濮嬬殑鍗佸叚杩涘埗瀛楃��*/
			OLED_ShowChar(X + i * FontSize, Y, SingleNumber - 10 + 'A', FontSize);
		}
	}
}

/**
  * 鍑�    鏁帮細OLED鏄剧ず浜岃繘鍒舵暟瀛楋紙浜岃繘鍒讹紝姝ｆ暣鏁帮級
  * 鍙�    鏁帮細X 鎸囧畾鏁板瓧宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鏁板瓧宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Number 鎸囧畾瑕佹樉绀虹殑鏁板瓧锛岃寖鍥达細0x00000000~0xFFFFFFFF
  * 鍙�    鏁帮細Length 鎸囧畾鏁板瓧鐨勯暱搴︼紝鑼冨洿锛�0~16
  * 鍙�    鏁帮細FontSize 鎸囧畾瀛椾綋澶у皬
  *           鑼冨洿锛歄LED_8X16		瀹�8鍍忕礌锛岄珮16鍍忕礌
  *                 OLED_6X8		瀹�6鍍忕礌锛岄珮8鍍忕礌
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_ShowBinNum(uint8_t X, uint8_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
	uint8_t i;
	for (i = 0; i < Length; i++)		//閬嶅巻鏁板瓧鐨勬瘡涓€浣�	
	{
		/*璋冪敤OLED_ShowChar鍑芥暟锛屼緷娆℃樉绀烘瘡涓�鏁板瓧*/
		/*Number / OLED_Pow(2, Length - i - 1) % 2 鍙�浠ヤ簩杩涘埗鎻愬彇鏁板瓧鐨勬瘡涓€浣�*/
		/*+ '0' 鍙�灏嗘暟瀛楄浆鎹�涓哄瓧绗︽牸寮�*/
		OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', FontSize);
	}
}

/**
  * 鍑�    鏁帮細OLED鏄剧ず娴�鐐规暟瀛楋紙鍗佽繘鍒讹紝灏忔暟锛�
  * 鍙�    鏁帮細X 鎸囧畾鏁板瓧宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鏁板瓧宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Number 鎸囧畾瑕佹樉绀虹殑鏁板瓧锛岃寖鍥达細-4294967295.0~4294967295.0
  * 鍙�    鏁帮細IntLength 鎸囧畾鏁板瓧鐨勬暣鏁颁綅闀垮害锛岃寖鍥达細0~10
  * 鍙�    鏁帮細FraLength 鎸囧畾鏁板瓧鐨勫皬鏁颁綅闀垮害锛岃寖鍥达細0~9锛岃繃闀跨殑灏忔暟浼氭湁绮惧害涓㈠け
  * 鍙�    鏁帮細FontSize 鎸囧畾瀛椾綋澶у皬
  *           鑼冨洿锛歄LED_8X16		瀹�8鍍忕礌锛岄珮16鍍忕礌
  *                 OLED_6X8		瀹�6鍍忕礌锛岄珮8鍍忕礌
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_ShowFloatNum(uint8_t X, uint8_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
{
	uint32_t Temp;
	
	if (Number >= 0)						//鏁板瓧澶т簬绛変簬0
	{
		OLED_ShowChar(X, Y, '+', FontSize);	//鏄剧ず+鍙�
	}
	else									//鏁板瓧灏忎簬0
	{
		OLED_ShowChar(X, Y, '-', FontSize);	//鏄剧ず-鍙�
		Number = -Number;					//Number鍙栬礋
	}
	
	/*鏄剧ず鏁存暟閮ㄥ垎*/
	OLED_ShowNum(X + FontSize, Y, Number, IntLength, FontSize);
	
	/*鏄剧ず灏忔暟鐐�*/
	OLED_ShowChar(X + (IntLength + 1) * FontSize, Y, '.', FontSize);
	
	/*灏哊umber鐨勬暣鏁伴儴鍒嗗噺鎺夛紝闃叉��涔嬪悗灏嗗皬鏁伴儴鍒嗕箻鍒版暣鏁版椂鍥犳暟杩囧ぇ閫犳垚閿欒��*/
	Number -= (uint32_t)Number;
	
	/*灏嗗皬鏁伴儴鍒嗕箻鍒版暣鏁伴儴鍒嗭紝骞舵樉绀�*/
	Temp = OLED_Pow(10, FraLength);
	OLED_ShowNum(X + (IntLength + 2) * FontSize, Y, ((uint32_t)(Number * Temp)) % Temp, FraLength, FontSize);
}

/**
  * 鍑�    鏁帮細OLED鏄剧ず姹夊瓧涓�
  * 鍙�    鏁帮細X 鎸囧畾姹夊瓧涓插乏涓婅�掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾姹夊瓧涓插乏涓婅�掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Chinese 鎸囧畾瑕佹樉绀虹殑姹夊瓧涓诧紝鑼冨洿锛氬繀椤诲叏閮ㄤ负姹夊瓧鎴栬€呭叏瑙掑瓧绗︼紝涓嶈�佸姞鍏ヤ换浣曞崐瑙掑瓧绗�
  *           鏄剧ず鐨勬眽瀛楅渶瑕佸湪OLED_Data.c閲岀殑OLED_CF16x16鏁扮粍瀹氫箟
  *           鏈�鎵惧埌鎸囧畾姹夊瓧鏃讹紝浼氭樉绀洪粯璁ゅ浘褰�锛堜竴涓�鏂规�嗭紝鍐呴儴涓€涓�闂�鍙凤級
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_ShowChinese(uint8_t X, uint8_t Y, char *Chinese)
{
	uint8_t pChinese = 0;
	uint8_t pIndex;
	uint8_t i;
	char SingleChinese[OLED_CHN_CHAR_WIDTH + 1] = {0};
	
	for (i = 0; Chinese[i] != '\0'; i ++)		//閬嶅巻姹夊瓧涓�
	{
		SingleChinese[pChinese] = Chinese[i];	//鎻愬彇姹夊瓧涓叉暟鎹�鍒板崟涓�姹夊瓧鏁扮粍
		pChinese ++;							//璁℃�¤嚜澧�
		
		/*褰撴彁鍙栨�℃暟鍒拌揪OLED_CHN_CHAR_WIDTH鏃讹紝鍗充唬琛ㄦ彁鍙栧埌浜嗕竴涓�瀹屾暣鐨勬眽瀛�*/
		if (pChinese >= OLED_CHN_CHAR_WIDTH)
		{
			pChinese = 0;		//璁℃�″綊闆�
			
			/*閬嶅巻鏁翠釜姹夊瓧瀛楁ā搴擄紝瀵绘壘鍖归厤鐨勬眽瀛�*/
			/*濡傛灉鎵惧埌鏈€鍚庝竴涓�姹夊瓧锛堝畾涔変负绌哄瓧绗︿覆锛夛紝鍒欒〃绀烘眽瀛楁湭鍦ㄥ瓧妯″簱瀹氫箟锛屽仠姝㈠�绘壘*/
			for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex ++)
			{
				/*鎵惧埌鍖归厤鐨勬眽瀛�*/
				if (strcmp(OLED_CF16x16[pIndex].Index, SingleChinese) == 0)
				{
					break;		//璺冲嚭寰�鐜�锛屾�ゆ椂pIndex鐨勫€间负鎸囧畾姹夊瓧鐨勭储寮�
				}
			}
			
			/*灏嗘眽瀛楀瓧妯″簱OLED_CF16x16鐨勬寚瀹氭暟鎹�浠�16*16鐨勫浘鍍忔牸寮忔樉绀�*/
			OLED_ShowImage(X + ((i + 1) / OLED_CHN_CHAR_WIDTH - 1) * 16, Y, 16, 16, OLED_CF16x16[pIndex].Data);
		}
	}
}

/**
  * 鍑�    鏁帮細OLED鏄剧ず鍥惧儚
  * 鍙�    鏁帮細X 鎸囧畾鍥惧儚宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鍥惧儚宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Width 鎸囧畾鍥惧儚鐨勫�藉害锛岃寖鍥达細0~128
  * 鍙�    鏁帮細Height 鎸囧畾鍥惧儚鐨勯珮搴︼紝鑼冨洿锛�0~64
  * 鍙�    鏁帮細Image 鎸囧畾瑕佹樉绀虹殑鍥惧儚
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_ShowImage(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
	uint8_t i, j;
	
	/*鍙傛暟妫€鏌ワ紝淇濊瘉鎸囧畾鍥惧儚涓嶄細瓒呭嚭灞忓箷鑼冨洿*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	
	/*灏嗗浘鍍忔墍鍦ㄥ尯鍩熸竻绌�*/
	OLED_ClearArea(X, Y, Width, Height);
	
	/*閬嶅巻鎸囧畾鍥惧儚娑夊強鐨勭浉鍏抽〉*/
	/*(Height - 1) / 8 + 1鐨勭洰鐨勬槸Height / 8骞跺悜涓婂彇鏁�*/
	for (j = 0; j < (Height - 1) / 8 + 1; j ++)
	{
		/*閬嶅巻鎸囧畾鍥惧儚娑夊強鐨勭浉鍏冲垪*/
		for (i = 0; i < Width; i ++)
		{
			/*瓒呭嚭杈圭晫锛屽垯璺宠繃鏄剧ず*/
			if (X + i > 127) {break;}
			if (Y / 8 + j > 7) {return;}
			
			/*鏄剧ず鍥惧儚鍦ㄥ綋鍓嶉〉鐨勫唴瀹�*/
			OLED_DisplayBuf[Y / 8 + j][X + i] |= Image[j * Width + i] << (Y % 8);
			
			/*瓒呭嚭杈圭晫锛屽垯璺宠繃鏄剧ず*/
			/*浣跨敤continue鐨勭洰鐨勬槸锛屼笅涓€椤佃秴鍑鸿竟鐣屾椂锛屼笂涓€椤电殑鍚庣画鍐呭�硅繕闇€瑕佺户缁�鏄剧ず*/
			if (Y / 8 + j + 1 > 7) {continue;}
			
			/*鏄剧ず鍥惧儚鍦ㄤ笅涓€椤电殑鍐呭��*/
			OLED_DisplayBuf[Y / 8 + j + 1][X + i] |= Image[j * Width + i] >> (8 - Y % 8);
		}
	}
}

/**
  * 鍑�    鏁帮細OLED浣跨敤printf鍑芥暟鎵撳嵃鏍煎紡鍖栧瓧绗︿覆
  * 鍙�    鏁帮細X 鎸囧畾鏍煎紡鍖栧瓧绗︿覆宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鏍煎紡鍖栧瓧绗︿覆宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細FontSize 鎸囧畾瀛椾綋澶у皬
  *           鑼冨洿锛歄LED_8X16		瀹�8鍍忕礌锛岄珮16鍍忕礌
  *                 OLED_6X8		瀹�6鍍忕礌锛岄珮8鍍忕礌
  * 鍙�    鏁帮細format 鎸囧畾瑕佹樉绀虹殑鏍煎紡鍖栧瓧绗︿覆锛岃寖鍥达細ASCII鐮佸彲瑙佸瓧绗︾粍鎴愮殑瀛楃�︿覆
  * 鍙�    鏁帮細... 鏍煎紡鍖栧瓧绗︿覆鍙傛暟鍒楄〃
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...)
{
	char String[30];						//瀹氫箟瀛楃�︽暟缁�
	va_list arg;							//瀹氫箟鍙�鍙樺弬鏁板垪琛ㄦ暟鎹�绫诲瀷鐨勫彉閲廰rg
	va_start(arg, format);					//浠巉ormat寮€濮嬶紝鎺ユ敹鍙傛暟鍒楄〃鍒癮rg鍙橀噺
	vsprintf(String, format, arg);			//浣跨敤vsprintf鎵撳嵃鏍煎紡鍖栧瓧绗︿覆鍜屽弬鏁板垪琛ㄥ埌瀛楃�︽暟缁勪腑
	va_end(arg);							//缁撴潫鍙橀噺arg
	OLED_ShowString(X, Y, String, FontSize);//OLED鏄剧ず瀛楃�︽暟缁勶紙瀛楃�︿覆锛�
}

/**
  * 鍑�    鏁帮細OLED鍦ㄦ寚瀹氫綅缃�鐢讳竴涓�鐐�
  * 鍙�    鏁帮細X 鎸囧畾鐐圭殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鐐圭殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_DrawPoint(uint8_t X, uint8_t Y)
{
	/*鍙傛暟妫€鏌ワ紝淇濊瘉鎸囧畾浣嶇疆涓嶄細瓒呭嚭灞忓箷鑼冨洿*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	
	/*灏嗘樉瀛樻暟缁勬寚瀹氫綅缃�鐨勪竴涓狟it鏁版嵁缃�1*/
	OLED_DisplayBuf[Y / 8][X] |= 0x01 << (Y % 8);
}

/**
  * 鍑�    鏁帮細OLED鑾峰彇鎸囧畾浣嶇疆鐐圭殑鍊�
  * 鍙�    鏁帮細X 鎸囧畾鐐圭殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鐐圭殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 杩� 鍥� 鍊硷細鎸囧畾浣嶇疆鐐规槸鍚﹀�勪簬鐐逛寒鐘舵€侊紝1锛氱偣浜�锛�0锛氱唲鐏�
  */
uint8_t OLED_GetPoint(uint8_t X, uint8_t Y)
{
	/*鍙傛暟妫€鏌ワ紝淇濊瘉鎸囧畾浣嶇疆涓嶄細瓒呭嚭灞忓箷鑼冨洿*/
	if (X > 127) {return 0;}
	if (Y > 63) {return 0;}
	
	/*鍒ゆ柇鎸囧畾浣嶇疆鐨勬暟鎹�*/
	if (OLED_DisplayBuf[Y / 8][X] & 0x01 << (Y % 8))
	{
		return 1;	//涓�1锛岃繑鍥�1
	}
	
	return 0;		//鍚﹀垯锛岃繑鍥�0
}

/**
  * 鍑�    鏁帮細OLED鐢荤嚎
  * 鍙�    鏁帮細X0 鎸囧畾涓€涓�绔�鐐圭殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y0 鎸囧畾涓€涓�绔�鐐圭殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細X1 鎸囧畾鍙︿竴涓�绔�鐐圭殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y1 鎸囧畾鍙︿竴涓�绔�鐐圭殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_DrawLine(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1)
{
	int16_t x, y, dx, dy, d, incrE, incrNE, temp;
	int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
	uint8_t yflag = 0, xyflag = 0;
	
	if (y0 == y1)		//妯�绾垮崟鐙�澶勭悊
	{
		/*0鍙风偣X鍧愭爣澶т簬1鍙风偣X鍧愭爣锛屽垯浜ゆ崲涓ょ偣X鍧愭爣*/
		if (x0 > x1) {temp = x0; x0 = x1; x1 = temp;}
		
		/*閬嶅巻X鍧愭爣*/
		for (x = x0; x <= x1; x ++)
		{
			OLED_DrawPoint(x, y0);	//渚濇�＄敾鐐�
		}
	}
	else if (x0 == x1)	//绔栫嚎鍗曠嫭澶勭悊
	{
		/*0鍙风偣Y鍧愭爣澶т簬1鍙风偣Y鍧愭爣锛屽垯浜ゆ崲涓ょ偣Y鍧愭爣*/
		if (y0 > y1) {temp = y0; y0 = y1; y1 = temp;}
		
		/*閬嶅巻Y鍧愭爣*/
		for (y = y0; y <= y1; y ++)
		{
			OLED_DrawPoint(x0, y);	//渚濇�＄敾鐐�
		}
	}
	else				//鏂滅嚎
	{
		/*浣跨敤Bresenham绠楁硶鐢荤洿绾匡紝鍙�浠ラ伩鍏嶈€楁椂鐨勬诞鐐硅繍绠楋紝鏁堢巼鏇撮珮*/
		/*鍙傝€冩枃妗ｏ細https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
		/*鍙傝€冩暀绋嬶細https://www.bilibili.com/video/BV1364y1d7Lo*/
		
		if (x0 > x1)	//0鍙风偣X鍧愭爣澶т簬1鍙风偣X鍧愭爣
		{
			/*浜ゆ崲涓ょ偣鍧愭爣*/
			/*浜ゆ崲鍚庝笉褰卞搷鐢荤嚎锛屼絾鏄�鐢荤嚎鏂瑰悜鐢辩��涓€銆佷簩銆佷笁銆佸洓璞￠檺鍙樹负绗�涓€銆佸洓璞￠檺*/
			temp = x0; x0 = x1; x1 = temp;
			temp = y0; y0 = y1; y1 = temp;
		}
		
		if (y0 > y1)	//0鍙风偣Y鍧愭爣澶т簬1鍙风偣Y鍧愭爣
		{
			/*灏哬鍧愭爣鍙栬礋*/
			/*鍙栬礋鍚庡奖鍝嶇敾绾匡紝浣嗘槸鐢荤嚎鏂瑰悜鐢辩��涓€銆佸洓璞￠檺鍙樹负绗�涓€璞￠檺*/
			y0 = -y0;
			y1 = -y1;
			
			/*缃�鏍囧織浣峺flag锛岃�颁綇褰撳墠鍙樻崲锛屽湪鍚庣画瀹為檯鐢荤嚎鏃讹紝鍐嶅皢鍧愭爣鎹㈠洖鏉�*/
			yflag = 1;
		}
		
		if (y1 - y0 > x1 - x0)	//鐢荤嚎鏂滅巼澶т簬1
		{
			/*灏哫鍧愭爣涓嶻鍧愭爣浜掓崲*/
			/*浜掓崲鍚庡奖鍝嶇敾绾匡紝浣嗘槸鐢荤嚎鏂瑰悜鐢辩��涓€璞￠檺0~90搴﹁寖鍥村彉涓虹��涓€璞￠檺0~45搴﹁寖鍥�*/
			temp = x0; x0 = y0; y0 = temp;
			temp = x1; x1 = y1; y1 = temp;
			
			/*缃�鏍囧織浣峹yflag锛岃�颁綇褰撳墠鍙樻崲锛屽湪鍚庣画瀹為檯鐢荤嚎鏃讹紝鍐嶅皢鍧愭爣鎹㈠洖鏉�*/
			xyflag = 1;
		}
		
		/*浠ヤ笅涓築resenham绠楁硶鐢荤洿绾�*/
		/*绠楁硶瑕佹眰锛岀敾绾挎柟鍚戝繀椤讳负绗�涓€璞￠檺0~45搴﹁寖鍥�*/
		dx = x1 - x0;
		dy = y1 - y0;
		incrE = 2 * dy;
		incrNE = 2 * (dy - dx);
		d = 2 * dy - dx;
		x = x0;
		y = y0;
		
		/*鐢昏捣濮嬬偣锛屽悓鏃跺垽鏂�鏍囧織浣嶏紝灏嗗潗鏍囨崲鍥炴潵*/
		if (yflag && xyflag){OLED_DrawPoint(y, -x);}
		else if (yflag)		{OLED_DrawPoint(x, -y);}
		else if (xyflag)	{OLED_DrawPoint(y, x);}
		else				{OLED_DrawPoint(x, y);}
		
		while (x < x1)		//閬嶅巻X杞寸殑姣忎釜鐐�
		{
			x ++;
			if (d < 0)		//涓嬩竴涓�鐐瑰湪褰撳墠鐐逛笢鏂�
			{
				d += incrE;
			}
			else			//涓嬩竴涓�鐐瑰湪褰撳墠鐐逛笢鍖楁柟
			{
				y ++;
				d += incrNE;
			}
			
			/*鐢绘瘡涓€涓�鐐癸紝鍚屾椂鍒ゆ柇鏍囧織浣嶏紝灏嗗潗鏍囨崲鍥炴潵*/
			if (yflag && xyflag){OLED_DrawPoint(y, -x);}
			else if (yflag)		{OLED_DrawPoint(x, -y);}
			else if (xyflag)	{OLED_DrawPoint(y, x);}
			else				{OLED_DrawPoint(x, y);}
		}	
	}
}

/**
  * 鍑�    鏁帮細OLED鐭╁舰
  * 鍙�    鏁帮細X 鎸囧畾鐭╁舰宸︿笂瑙掔殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鐭╁舰宸︿笂瑙掔殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Width 鎸囧畾鐭╁舰鐨勫�藉害锛岃寖鍥达細0~128
  * 鍙�    鏁帮細Height 鎸囧畾鐭╁舰鐨勯珮搴︼紝鑼冨洿锛�0~64
  * 鍙�    鏁帮細IsFilled 鎸囧畾鐭╁舰鏄�鍚﹀～鍏�
  *           鑼冨洿锛歄LED_UNFILLED		涓嶅～鍏�
  *                 OLED_FILLED			濉�鍏�
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled)
{
	uint8_t i, j;
	if (!IsFilled)		//鎸囧畾鐭╁舰涓嶅～鍏�
	{
		/*閬嶅巻涓婁笅X鍧愭爣锛岀敾鐭╁舰涓婁笅涓ゆ潯绾�*/
		for (i = X; i < X + Width; i ++)
		{
			OLED_DrawPoint(i, Y);
			OLED_DrawPoint(i, Y + Height - 1);
		}
		/*閬嶅巻宸﹀彸Y鍧愭爣锛岀敾鐭╁舰宸﹀彸涓ゆ潯绾�*/
		for (i = Y; i < Y + Height; i ++)
		{
			OLED_DrawPoint(X, i);
			OLED_DrawPoint(X + Width - 1, i);
		}
	}
	else				//鎸囧畾鐭╁舰濉�鍏�
	{
		/*閬嶅巻X鍧愭爣*/
		for (i = X; i < X + Width; i ++)
		{
			/*閬嶅巻Y鍧愭爣*/
			for (j = Y; j < Y + Height; j ++)
			{
				/*鍦ㄦ寚瀹氬尯鍩熺敾鐐癸紝濉�鍏呮弧鐭╁舰*/
				OLED_DrawPoint(i, j);
			}
		}
	}
}

/**
  * 鍑�    鏁帮細OLED涓夎�掑舰
  * 鍙�    鏁帮細X0 鎸囧畾绗�涓€涓�绔�鐐圭殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y0 鎸囧畾绗�涓€涓�绔�鐐圭殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細X1 鎸囧畾绗�浜屼釜绔�鐐圭殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y1 鎸囧畾绗�浜屼釜绔�鐐圭殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細X2 鎸囧畾绗�涓変釜绔�鐐圭殑妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y2 鎸囧畾绗�涓変釜绔�鐐圭殑绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細IsFilled 鎸囧畾涓夎�掑舰鏄�鍚﹀～鍏�
  *           鑼冨洿锛歄LED_UNFILLED		涓嶅～鍏�
  *                 OLED_FILLED			濉�鍏�
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_DrawTriangle(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2, uint8_t IsFilled)
{
	uint8_t minx = X0, miny = Y0, maxx = X0, maxy = Y0;
	uint8_t i, j;
	int16_t vx[] = {X0, X1, X2};
	int16_t vy[] = {Y0, Y1, Y2};
	
	if (!IsFilled)			//鎸囧畾涓夎�掑舰涓嶅～鍏�
	{
		/*璋冪敤鐢荤嚎鍑芥暟锛屽皢涓変釜鐐圭敤鐩寸嚎杩炴帴*/
		OLED_DrawLine(X0, Y0, X1, Y1);
		OLED_DrawLine(X0, Y0, X2, Y2);
		OLED_DrawLine(X1, Y1, X2, Y2);
	}
	else					//鎸囧畾涓夎�掑舰濉�鍏�
	{
		/*鎵惧埌涓変釜鐐规渶灏忕殑X銆乊鍧愭爣*/
		if (X1 < minx) {minx = X1;}
		if (X2 < minx) {minx = X2;}
		if (Y1 < miny) {miny = Y1;}
		if (Y2 < miny) {miny = Y2;}
		
		/*鎵惧埌涓変釜鐐规渶澶х殑X銆乊鍧愭爣*/
		if (X1 > maxx) {maxx = X1;}
		if (X2 > maxx) {maxx = X2;}
		if (Y1 > maxy) {maxy = Y1;}
		if (Y2 > maxy) {maxy = Y2;}
		
		/*鏈€灏忔渶澶у潗鏍囦箣闂寸殑鐭╁舰涓哄彲鑳介渶瑕佸～鍏呯殑鍖哄煙*/
		/*閬嶅巻姝ゅ尯鍩熶腑鎵€鏈夌殑鐐�*/
		/*閬嶅巻X鍧愭爣*/		
		for (i = minx; i <= maxx; i ++)
		{
			/*閬嶅巻Y鍧愭爣*/	
			for (j = miny; j <= maxy; j ++)
			{
				/*璋冪敤OLED_pnpoly锛屽垽鏂�鎸囧畾鐐规槸鍚﹀湪鎸囧畾涓夎�掑舰涔嬩腑*/
				/*濡傛灉鍦�锛屽垯鐢荤偣锛屽�傛灉涓嶅湪锛屽垯涓嶅仛澶勭悊*/
				if (OLED_pnpoly(3, vx, vy, i, j)) {OLED_DrawPoint(i, j);}
			}
		}
	}
}

/**
  * 鍑�    鏁帮細OLED鐢诲渾
  * 鍙�    鏁帮細X 鎸囧畾鍦嗙殑鍦嗗績妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鍦嗙殑鍦嗗績绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細Radius 鎸囧畾鍦嗙殑鍗婂緞锛岃寖鍥达細0~255
  * 鍙�    鏁帮細IsFilled 鎸囧畾鍦嗘槸鍚﹀～鍏�
  *           鑼冨洿锛歄LED_UNFILLED		涓嶅～鍏�
  *                 OLED_FILLED			濉�鍏�
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_DrawCircle(uint8_t X, uint8_t Y, uint8_t Radius, uint8_t IsFilled)
{
	int16_t x, y, d, j;
	
	/*浣跨敤Bresenham绠楁硶鐢诲渾锛屽彲浠ラ伩鍏嶈€楁椂鐨勬诞鐐硅繍绠楋紝鏁堢巼鏇撮珮*/
	/*鍙傝€冩枃妗ｏ細https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
	/*鍙傝€冩暀绋嬶細https://www.bilibili.com/video/BV1VM4y1u7wJ*/
	
	d = 1 - Radius;
	x = 0;
	y = Radius;
	
	/*鐢绘瘡涓�鍏�鍒嗕箣涓€鍦嗗姬鐨勮捣濮嬬偣*/
	OLED_DrawPoint(X + x, Y + y);
	OLED_DrawPoint(X - x, Y - y);
	OLED_DrawPoint(X + y, Y + x);
	OLED_DrawPoint(X - y, Y - x);
	
	if (IsFilled)		//鎸囧畾鍦嗗～鍏�
	{
		/*閬嶅巻璧峰�嬬偣Y鍧愭爣*/
		for (j = -y; j < y; j ++)
		{
			/*鍦ㄦ寚瀹氬尯鍩熺敾鐐癸紝濉�鍏呴儴鍒嗗渾*/
			OLED_DrawPoint(X, Y + j);
		}
	}
	
	while (x < y)		//閬嶅巻X杞寸殑姣忎釜鐐�
	{
		x ++;
		if (d < 0)		//涓嬩竴涓�鐐瑰湪褰撳墠鐐逛笢鏂�
		{
			d += 2 * x + 1;
		}
		else			//涓嬩竴涓�鐐瑰湪褰撳墠鐐逛笢鍗楁柟
		{
			y --;
			d += 2 * (x - y) + 1;
		}
		
		/*鐢绘瘡涓�鍏�鍒嗕箣涓€鍦嗗姬鐨勭偣*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X + y, Y + x);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - y, Y - x);
		OLED_DrawPoint(X + x, Y - y);
		OLED_DrawPoint(X + y, Y - x);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X - y, Y + x);
		
		if (IsFilled)	//鎸囧畾鍦嗗～鍏�
		{
			/*閬嶅巻涓�闂撮儴鍒�*/
			for (j = -y; j < y; j ++)
			{
				/*鍦ㄦ寚瀹氬尯鍩熺敾鐐癸紝濉�鍏呴儴鍒嗗渾*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
			
			/*閬嶅巻涓や晶閮ㄥ垎*/
			for (j = -x; j < x; j ++)
			{
				/*鍦ㄦ寚瀹氬尯鍩熺敾鐐癸紝濉�鍏呴儴鍒嗗渾*/
				OLED_DrawPoint(X - y, Y + j);
				OLED_DrawPoint(X + y, Y + j);
			}
		}
	}
}

/**
  * 鍑�    鏁帮細OLED鐢绘き鍦�
  * 鍙�    鏁帮細X 鎸囧畾妞�鍦嗙殑鍦嗗績妯�鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾妞�鍦嗙殑鍦嗗績绾靛潗鏍囷紝鑼冨洿锛�0~63
  * 鍙�    鏁帮細A 鎸囧畾妞�鍦嗙殑妯�鍚戝崐杞撮暱搴︼紝鑼冨洿锛�0~255
  * 鍙�    鏁帮細B 鎸囧畾妞�鍦嗙殑绾靛悜鍗婅酱闀垮害锛岃寖鍥达細0~255
  * 鍙�    鏁帮細IsFilled 鎸囧畾妞�鍦嗘槸鍚﹀～鍏�
  *           鑼冨洿锛歄LED_UNFILLED		涓嶅～鍏�
  *                 OLED_FILLED			濉�鍏�
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_DrawEllipse(uint8_t X, uint8_t Y, uint8_t A, uint8_t B, uint8_t IsFilled)
{
	int16_t x, y, j;
	int16_t a = A, b = B;
	float d1, d2;
	
	/*浣跨敤Bresenham绠楁硶鐢绘き鍦嗭紝鍙�浠ラ伩鍏嶉儴鍒嗚€楁椂鐨勬诞鐐硅繍绠楋紝鏁堢巼鏇撮珮*/
	/*鍙傝€冮摼鎺ワ細https://blog.csdn.net/myf_666/article/details/128167392*/
	
	x = 0;
	y = b;
	d1 = b * b + a * a * (-b + 0.5);
	
	if (IsFilled)	//鎸囧畾妞�鍦嗗～鍏�
	{
		/*閬嶅巻璧峰�嬬偣Y鍧愭爣*/
		for (j = -y; j < y; j ++)
		{
			/*鍦ㄦ寚瀹氬尯鍩熺敾鐐癸紝濉�鍏呴儴鍒嗘き鍦�*/
			OLED_DrawPoint(X, Y + j);
			OLED_DrawPoint(X, Y + j);
		}
	}
	
	/*鐢绘き鍦嗗姬鐨勮捣濮嬬偣*/
	OLED_DrawPoint(X + x, Y + y);
	OLED_DrawPoint(X - x, Y - y);
	OLED_DrawPoint(X - x, Y + y);
	OLED_DrawPoint(X + x, Y - y);
	
	/*鐢绘き鍦嗕腑闂撮儴鍒�*/
	while (b * b * (x + 1) < a * a * (y - 0.5))
	{
		if (d1 <= 0)		//涓嬩竴涓�鐐瑰湪褰撳墠鐐逛笢鏂�
		{
			d1 += b * b * (2 * x + 3);
		}
		else				//涓嬩竴涓�鐐瑰湪褰撳墠鐐逛笢鍗楁柟
		{
			d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
			y --;
		}
		x ++;
		
		if (IsFilled)	//鎸囧畾妞�鍦嗗～鍏�
		{
			/*閬嶅巻涓�闂撮儴鍒�*/
			for (j = -y; j < y; j ++)
			{
				/*鍦ㄦ寚瀹氬尯鍩熺敾鐐癸紝濉�鍏呴儴鍒嗘き鍦�*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
		}
		
		/*鐢绘き鍦嗕腑闂撮儴鍒嗗渾寮�*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X + x, Y - y);
	}
	
	/*鐢绘き鍦嗕袱渚ч儴鍒�*/
	d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
	
	while (y > 0)
	{
		if (d2 <= 0)		//涓嬩竴涓�鐐瑰湪褰撳墠鐐逛笢鏂�
		{
			d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
			x ++;
			
		}
		else				//涓嬩竴涓�鐐瑰湪褰撳墠鐐逛笢鍗楁柟
		{
			d2 += a * a * (-2 * y + 3);
		}
		y --;
		
		if (IsFilled)	//鎸囧畾妞�鍦嗗～鍏�
		{
			/*閬嶅巻涓や晶閮ㄥ垎*/
			for (j = -y; j < y; j ++)
			{
				/*鍦ㄦ寚瀹氬尯鍩熺敾鐐癸紝濉�鍏呴儴鍒嗘き鍦�*/
				OLED_DrawPoint(X + x, Y + j);
				OLED_DrawPoint(X - x, Y + j);
			}
		}
		
		/*鐢绘き鍦嗕袱渚ч儴鍒嗗渾寮�*/
		OLED_DrawPoint(X + x, Y + y);
		OLED_DrawPoint(X - x, Y - y);
		OLED_DrawPoint(X - x, Y + y);
		OLED_DrawPoint(X + x, Y - y);
	}
}

/**
  * 鍑�    鏁帮細OLED鐢诲渾寮�
  * 鍙�    鏁帮細X 鎸囧畾鍦嗗姬鐨勫渾蹇冩í鍧愭爣锛岃寖鍥达細0~127
  * 鍙�    鏁帮細Y 鎸囧畾鍦嗗姬鐨勫渾蹇冪旱鍧愭爣锛岃寖鍥达細0~63
  * 鍙�    鏁帮細Radius 鎸囧畾鍦嗗姬鐨勫崐寰勶紝鑼冨洿锛�0~255
  * 鍙�    鏁帮細StartAngle 鎸囧畾鍦嗗姬鐨勮捣濮嬭�掑害锛岃寖鍥达細-180~180
  *           姘村钩鍚戝彸涓�0搴︼紝姘村钩鍚戝乏涓�180搴︽垨-180搴︼紝涓嬫柟涓烘�ｆ暟锛屼笂鏂逛负璐熸暟锛岄『鏃堕拡鏃嬭浆
  * 鍙�    鏁帮細EndAngle 鎸囧畾鍦嗗姬鐨勭粓姝㈣�掑害锛岃寖鍥达細-180~180
  *           姘村钩鍚戝彸涓�0搴︼紝姘村钩鍚戝乏涓�180搴︽垨-180搴︼紝涓嬫柟涓烘�ｆ暟锛屼笂鏂逛负璐熸暟锛岄『鏃堕拡鏃嬭浆
  * 鍙�    鏁帮細IsFilled 鎸囧畾鍦嗗姬鏄�鍚﹀～鍏咃紝濉�鍏呭悗涓烘墖褰�
  *           鑼冨洿锛歄LED_UNFILLED		涓嶅～鍏�
  *                 OLED_FILLED			濉�鍏�
  * 杩� 鍥� 鍊硷細鏃�
  * 璇�    鏄庯細璋冪敤姝ゅ嚱鏁板悗锛岃�佹兂鐪熸�ｅ湴鍛堢幇鍦ㄥ睆骞曚笂锛岃繕闇€璋冪敤鏇存柊鍑芥暟
  */
void OLED_DrawArc(uint8_t X, uint8_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled)
{
	int16_t x, y, d, j;
	
	/*姝ゅ嚱鏁板€熺敤Bresenham绠楁硶鐢诲渾鐨勬柟娉�*/
	
	d = 1 - Radius;
	x = 0;
	y = Radius;
	
	/*鍦ㄧ敾鍦嗙殑姣忎釜鐐规椂锛屽垽鏂�鎸囧畾鐐规槸鍚﹀湪鎸囧畾瑙掑害鍐咃紝鍦�锛屽垯鐢荤偣锛屼笉鍦�锛屽垯涓嶅仛澶勭悊*/
	if (OLED_IsInAngle(x, y, StartAngle, EndAngle))	{OLED_DrawPoint(X + x, Y + y);}
	if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
	if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
	if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}
	
	if (IsFilled)	//鎸囧畾鍦嗗姬濉�鍏�
	{
		/*閬嶅巻璧峰�嬬偣Y鍧愭爣*/
		for (j = -y; j < y; j ++)
		{
			/*鍦ㄥ～鍏呭渾鐨勬瘡涓�鐐规椂锛屽垽鏂�鎸囧畾鐐规槸鍚﹀湪鎸囧畾瑙掑害鍐咃紝鍦�锛屽垯鐢荤偣锛屼笉鍦�锛屽垯涓嶅仛澶勭悊*/
			if (OLED_IsInAngle(0, j, StartAngle, EndAngle)) {OLED_DrawPoint(X, Y + j);}
		}
	}
	
	while (x < y)		//閬嶅巻X杞寸殑姣忎釜鐐�
	{
		x ++;
		if (d < 0)		//涓嬩竴涓�鐐瑰湪褰撳墠鐐逛笢鏂�
		{
			d += 2 * x + 1;
		}
		else			//涓嬩竴涓�鐐瑰湪褰撳墠鐐逛笢鍗楁柟
		{
			y --;
			d += 2 * (x - y) + 1;
		}
		
		/*鍦ㄧ敾鍦嗙殑姣忎釜鐐规椂锛屽垽鏂�鎸囧畾鐐规槸鍚﹀湪鎸囧畾瑙掑害鍐咃紝鍦�锛屽垯鐢荤偣锛屼笉鍦�锛屽垯涓嶅仛澶勭悊*/
		if (OLED_IsInAngle(x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + y);}
		if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + x);}
		if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y - y);}
		if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y - x);}
		if (OLED_IsInAngle(x, -y, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y - y);}
		if (OLED_IsInAngle(y, -x, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y - x);}
		if (OLED_IsInAngle(-x, y, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + y);}
		if (OLED_IsInAngle(-y, x, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + x);}
		
		if (IsFilled)	//鎸囧畾鍦嗗姬濉�鍏�
		{
			/*閬嶅巻涓�闂撮儴鍒�*/
			for (j = -y; j < y; j ++)
			{
				/*鍦ㄥ～鍏呭渾鐨勬瘡涓�鐐规椂锛屽垽鏂�鎸囧畾鐐规槸鍚﹀湪鎸囧畾瑙掑害鍐咃紝鍦�锛屽垯鐢荤偣锛屼笉鍦�锛屽垯涓嶅仛澶勭悊*/
				if (OLED_IsInAngle(x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + x, Y + j);}
				if (OLED_IsInAngle(-x, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - x, Y + j);}
			}
			
			/*閬嶅巻涓や晶閮ㄥ垎*/
			for (j = -x; j < x; j ++)
			{
				/*鍦ㄥ～鍏呭渾鐨勬瘡涓�鐐规椂锛屽垽鏂�鎸囧畾鐐规槸鍚﹀湪鎸囧畾瑙掑害鍐咃紝鍦�锛屽垯鐢荤偣锛屼笉鍦�锛屽垯涓嶅仛澶勭悊*/
				if (OLED_IsInAngle(-y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X - y, Y + j);}
				if (OLED_IsInAngle(y, j, StartAngle, EndAngle)) {OLED_DrawPoint(X + y, Y + j);}
			}
		}
	}
}

/*********************鍔熻兘鍑芥暟*/


/*****************姹熷崗绉戞妧|鐗堟潈鎵€鏈�****************/
/*****************jiangxiekeji.com*****************/
