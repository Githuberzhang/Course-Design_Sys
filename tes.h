#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wincon.h>
#include <conio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>

#ifndef CORRU_H_INCLUDED
#define CORRU_H_INCLUDED

#define SCR_ROW 30             /*屏幕行数*/
#define SCR_COL 80             /*屏幕列数*/
#define M 5                    /*主菜单栏长度*/

/**
*媒体报道信息链结构
*/
typedef struct media_node
{
    char pvblg[9];              /*所属省份*/
    char casenum[13];           /*案件编号*/
    char reportdate[11];        /*报道日期*/
    char mediatype;             /*媒体类别‘1’、‘2’、‘3’、‘4’   分别代表四大媒体：报纸、广播、电视和互联网*/
    char medianame[21];         /*媒体名称*/
    char contentindex[51];      /*内容索引*/
    struct media_node *next;    /*指向下一结点的指针*/
}MEDIA_NODE;
/**
*案件信息链结构
*/
typedef struct case_node
{
    char casenum[13];           /*案件编号*/
    char casedate[11];          /*立案日期*/
    char pvblg[9];              /*所属省份*/
    char crimename[16];         /*被告人姓名*/
    char level;                 /*行政级别‘A’~’H’：分别代表副科级、正科级、副处级、正处级、副厅级、正厅级、副部级和正部级及以上*/
    char accusationname;        /*主要罪名‘1’~‘6’：分别代表受贿、贪污、挪用公款、渎职、滥用职权和其他*/
    float money;                /*涉案金额*/
    int punishment;             /*行事处罚1~99：有期徒刑  大于等于100：无期徒刑  小于0：死刑*/
    struct case_node *next;     /*指向下一结点的指针*/
    struct media_node *mnext;   /*指向媒体报道链的指针*/
}CASE_NODE;
/**
*省份信息链结构
*/
typedef struct province_node
{
    char pvblg[9];              /*所属省份*/
    char disciplineperson[16];  /*纪检负责人*/
    char tele[17];              /*联系电话*/
    struct province_node *next; /*指向下一结点的指针*/
    struct case_node *cnext;    /*指向案件信息链的指针*/
}PROVINCE_NODE;
/**
 *统计历年来各种罪名贪腐案件的发生数、涉案金额结构
 */
struct CRIMECASEMONEY
{
    char accuname;
    int sumcount;
    float summoney;
}crimecasemoney[6],crimecasemoneytmp;
/**
 *统计某年度内各种行政级别贪腐案件的发生数、刑事处罚结构
 */
struct LEVELCOUNTPUNISHMENT
{
    char classlevel;
    int year[5];
    int sumcount1;
}levelcountpunishment[8];
/**
 *统计某个时间段内各省份贪腐案件的发生数，涉案金额总数结构
 */
struct PROVINCECOUNTMONEY
{
    char proname[9];
    int sumcount2;
    float summoney1;
}provincecountmoneytmp;
/**
 *统计媒体报道数最多的10个贪腐案件，按报道数降序排序后输出案件编号、
 *被告行政级别、主要罪名、涉案金额、刑事处罚和媒体报道次数
 */
struct MEDIAMOST
{
    char casenum1[13];
    char classlevel1;
    char accuname1;
    float money1;
    int punish;
    int reportcount;
}mediamosttmp;
/**
 *统计不同媒体报道的次数
 */
struct STATUSMEDIATYPE
{
    char mediatype;
    int cnt;
}statusmediatype[4],statusmediatypetmp;
/**
 *屏幕窗口信息链结点结点结构
 */
typedef struct layer_node {
    char LayerNo;               /*弹出窗口层数*/
    SMALL_RECT rcArea;          /*弹出窗口区域坐标*/
    CHAR_INFO *pContent;        /*弹出窗口区域字符单元原信息存储缓冲区*/
    char *pScrAtt;              /*弹出窗口区域字符单元原属性值存储缓冲区*/
    struct layer_node *next;    /*指向下一结点的指针*/
} LAYER_NODE;

/**
 *标签束结构
 */
typedef struct labe1_bundle {
    char **ppLabel;             /*标签字符串数组首地址*/
    COORD *pLoc;                /*标签定位数组首地址*/
    int num;                    /*标签个数*/
} LABEL_BUNDLE;

/**
 *热区结构
 */
typedef struct hotarea_node {
    SMALL_RECT *pArea;          /*热区定位数组首地址*/
    char *pSort;                /*热区类别(按键、文本框、选项框)数组首地址*/
    char *pTag;                 /*热区序号数组首地址*/
    int num;                    /*热区个数*/
} HOT_AREA;

char *sysname = "贪腐案件信息管理系统";
char *proinfofile = "pro.txt";
char *mediainfofile = "media.txt";
char *caseinfofile = "case.txt";
char *probackupfile = "probackup.txt";
char *casebackupfile = "casebackup.txt";
char *mediabackupfile = "mediabackup.txt";

char *main_menu[] = {"文件(F)","数据维护(M)","数据查询(Q)","数据统计(S)","帮助(H)"};             /*系统主菜单名*/
char *sub_menu[] = {
                    "[S] 数据保存",          /*系统子菜单名*/
                    "[B] 数据备份",
                    "[R] 数据恢复",
                    "[X] 退出系统",
                    "[D]所属省份监管信息",
                    "[P]贪腐案件基本信息",
                    "[C]媒体报道基本信息",
                    "[D]所属省份监管信息",
                    "[P]贪腐案件基本信息",
                    "[C]媒体报道基本信息",
                    "[I]按罪名统计",
                    "[T]按行政级别及刑罚统计",
                    "[C]按所属省份统计",
                    "[U]按报道次数统计",
                    "[M]按报道类别统计",
                    "[T]联系开发者",
                    "[A]开发者信息"
                    };

LAYER_NODE *layhead = NULL;
PROVINCE_NODE *head = NULL;

int sub_menu_num[] = {4, 3, 3, 5, 2};   /*各主菜单项下子菜单的个数*/
int selmainmenu = 1;                    /*主菜单选项，1表示选中*/
int selsubmenu = 0;                     /*子菜单选项，0表示未选中*/
unsigned long ul;

CHAR_INFO *menubar = NULL;              /*存放菜单条屏幕区字符信息的缓冲区*/
CHAR_INFO *statebar = NULL;             /*存放状态条屏幕区字符信息的缓冲区*/

char *scratt = NULL;                    /*存放屏幕上字符单元属性值的缓冲区*/
char sys_state = '\0';                   /*用来保存系统状态的字符*/

HANDLE std_out;                         /*标准输出设备句柄*/
HANDLE std_in;                          /*标准输入设备句柄*/

/*自定义函数*/
int CreateList(PROVINCE_NODE **phead);/*从数据文件读取基础数据, 并存放到所创建的十字链表中*/
int ReCreateList(PROVINCE_NODE **phead,char *profile,char *casefile,char *mediafile);/*从数据文件读取基础数据, 并存放到所创建的十字链表中*/

PROVINCE_NODE *SeekProvinceNode(char *pvblg);/*按所属省份查找案件基本信息结点*/
CASE_NODE *SeekCaseNode(char *casenum);/*按案件编号查找案件基本信息结点*/
MEDIA_NODE *SeekMediaNode(char *casenum,char *pvblg);/*按案件编号和媒体报道日期查找媒体信息节点*/



int InsertMediaNode(MEDIA_NODE *pmedianode);/*在十字链表中插入一个媒体信息结点*/
int DeleteMediaNode(char *casenum,char *reportdate);/*在十字链表中删除一个媒体信息结点*/
int ModifyMediaNode(char *casenum,char *reportdate,MEDIA_NODE *pmedianode);/*对指定的媒体信息结点内容进行修改*/
int InsertCaseNode(char *pvblg,CASE_NODE *pcasenode);/*在十字链表中插入一个案件信息结点*/
int DeleteCaseNode(char *casenum,char *pvblg);/*在十字链表中删除一个案件信息结点*/
int ModifyCaseNode(CASE_NODE *pcasenode,char *casenum);/*对指定的案件信息结点内容进行修改*/
int InsertProvinceNode(PROVINCE_NODE **head,PROVINCE_NODE *pprovincenode);/*在十字链表中插入一个省份信息结点*/
int DeleteProvinceNode(char *pvblg);/*在十字链表中删除一个省份信息结点*/
int ModifyProvinceNode(PROVINCE_NODE *pprovincenode,char *pvblg);/*对指定的省份信息结点内容进行修改.*/

PROVINCE_NODE *QueryProvinceInfo1(char *pvblg);/*对指定的省份信息结点内容进行查询*/
PROVINCE_NODE *QueryProvinceInfo2(char *disciplineperson);/*对指定的省份信息结点内容进行查询*/
CASE_NODE *QueryCaseInfo1(char *casenum);/*对指定的案件信息结点内容进行查询.*/
CASE_NODE *QueryCaseInfo2(char *crimename,char level);/*对指定的案件信息结点内容进行查询.*/
MEDIA_NODE *QueryMediaInfo1(char *casenum,char *medianame);/*对指定的媒体信息结点内容进行查询.*/
MEDIA_NODE *QueryMediaInfo2(char *reportdate,char *medianame);/*对指定的媒体信息结点内容进行查询.*/
/*以主要罪名为条件分别统计历年来各种罪名贪腐案件的发生数、涉案金额，
按案件发生数降序排序后，输出统计结果*/
BOOL StatusCrimeName(void);
/*以行政级别为条件分别统计某年度内各种行政级别贪腐案件的发生数，
其中不同的刑事处罚案件分别是多少起*/
BOOL StatusLevelPunish(void);
/*以案件发生地所属省份为条件分别统计某个时间段内各省份贪腐案件的发生数，
涉案金额总数，按案件发生数降序排序后输出统计结果*/
BOOL StatusProvince(char *startdate,char *enddate);
BOOL StatusReportTime(void);/*统计媒体报道数最多的10个贪腐案件*/
BOOL StatusReportType(void);/*统计媒体报道种类*/


BOOL LoadData(void);
void RunSystem(void);/*运行系统, 在系统主界面下运行用户所选择的功能模块*/
void CloseSystem(void);/*关闭系统*/
void InInterface(void);/*初始化界面*/
void ClearScreen(void);/*清除屏幕信息*/
void ShowMenu(void);/*在屏幕上显示主菜单, 并设置热区, 在主菜单第一项上置选中标记*/
void ShowState(void);/*显示状态条*/
void TagMainMenu(int num);/*在指定主菜单项上置选中标志*/
void PopMenu(int num);/*弹出指定主菜单项对应的子菜单*/
void PopUp(SMALL_RECT *srpop, WORD att, LABEL_BUNDLE *labels, HOT_AREA *areas);/*在指定区域输出弹出窗口信息, 同时设置热区, 将弹出窗口位置信息入栈.*/
void PopOff(void);/*关闭顶层弹出窗口, 恢复覆盖区域原外观和字符单元原属性*/
void DrawBox(SMALL_RECT *pRc);/*在指定区域画边框*/
void TagSubMenu(int num);/*在指定子菜单项上做选中标记*/
void CalLocation(int num, SMALL_RECT *rc);/*计算弹出子菜单区域左上角和右下角的位置*/
int DealInput(HOT_AREA *pHotArea, int *piHot);/*在弹出窗口区域设置热区, 等待用户输入*/
void SetHotPoint(HOT_AREA *pHotArea, int iHot);/*设置热区*/

BOOL ExeFunction(int m, int s);/*执行由主菜单号和子菜单号确定的功能函数*/

/*弹出功能函数对应的界面*/

BOOL FileDataSave(void);/*数据保存*/
BOOL FileDataBackUp(void);/*数据备份*/
BOOL FileDataRestore_Interface(void);/*数据载入界面*/
BOOL FileDataRestore(char *profile,char *casefile,char *mediafile);/*数据载入*/
BOOL FileExit(void);/*退出*/

BOOL MaintainProInfo(void);/*省份信息维护*/
void InputProInfo(void);/*增加省份信息界面*/
void DelProInfo(void);/*删除省份信息界面*/
void ModifyProInfo(void);/*修改省份信息界面1*/
void InputModifyProInfo(void);/*修改省份信息界面2*/

BOOL MaintainCaseInfo(void);/*案件信息维护*/
void InputCaseInfo(void);/*增加案件信息界面*/
void DelCaseInfo(void);/*删除案件信息界面*/
void ModifyCaseInfo(void);/*修改案件信息界面1*/
void InputModifyCaseInfo(void);/*修改案件信息界面2*/

BOOL MaintainMediaInfo(void);/*媒体报道信息维护*/
void InputMediaInfo(void);/*增加媒体报道信息界面*/
void DelMediaInfo(void);/*删除媒体报道信息界面*/
void ModifyMediaInfo(void);/*修改媒体报道信息界面1*/
void InputModifyMediaInfo(void);/*修改媒体报道信息界面2*/

BOOL QueryProInfo(void);/*查询省份信息*/
void QueryProname(void);/*按省份名查询界面*/
PROVINCE_NODE *QueryProvinceInfo1(char *pvblg);/*按省份名查询*/
void QueryPerson(void);/*按纪检负责人查询界面*/
PROVINCE_NODE *QueryProvinceInfo2(char *disciplineperson);/*按纪检负责人查询*/

BOOL QueryCaseInfo(void);/*查询案件信息*/
void QueryCasenum(void);/*按案件编号查询界面*/
CASE_NODE *QueryCaseInfo1(char *casenum);/*按案件编号查询*/
void QueryCrimer(void);/*按犯罪嫌疑人查询界面*/
CASE_NODE *QueryCaseInfo2(char *crimename,char level);/*按犯罪嫌疑人查询*/

BOOL QueryMediaInfo(void);/*查询媒体信息*/
void QueryCaseMedia(void);/*按案件编号查询界面*/
MEDIA_NODE *QueryMediaInfo1(char *casenum,char *medianame);/*按案件编号查询*/
void QueryDateMedia(void);/*按报道日期查询界面*/
MEDIA_NODE *QueryMediaInfo2(char *reportdate,char *medianame);/*按报道日期查询*/

BOOL HelpTheme(void);/*帮助主题*/
BOOL HelpAbout(void);/*关于帮助*/

BOOL ShowModule(char **pString, int n);/* 显示功能函数对应的界面*/
#endif                  //CORRU_H_INCLUDED
