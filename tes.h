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

#define SCR_ROW 30             /*��Ļ����*/
#define SCR_COL 80             /*��Ļ����*/
#define M 5                    /*���˵�������*/

/**
*ý�屨����Ϣ���ṹ
*/
typedef struct media_node
{
    char pvblg[9];              /*����ʡ��*/
    char casenum[13];           /*�������*/
    char reportdate[11];        /*��������*/
    char mediatype;             /*ý�����1������2������3������4��   �ֱ�����Ĵ�ý�壺��ֽ���㲥�����Ӻͻ�����*/
    char medianame[21];         /*ý������*/
    char contentindex[51];      /*��������*/
    struct media_node *next;    /*ָ����һ����ָ��*/
}MEDIA_NODE;
/**
*������Ϣ���ṹ
*/
typedef struct case_node
{
    char casenum[13];           /*�������*/
    char casedate[11];          /*��������*/
    char pvblg[9];              /*����ʡ��*/
    char crimename[16];         /*����������*/
    char level;                 /*��������A��~��H�����ֱ�����Ƽ������Ƽ�������������������������������������������������������*/
    char accusationname;        /*��Ҫ������1��~��6�����ֱ�����ܻߡ�̰�ۡ�Ų�ù����ְ������ְȨ������*/
    float money;                /*�永���*/
    int punishment;             /*���´���1~99������ͽ��  ���ڵ���100������ͽ��  С��0������*/
    struct case_node *next;     /*ָ����һ����ָ��*/
    struct media_node *mnext;   /*ָ��ý�屨������ָ��*/
}CASE_NODE;
/**
*ʡ����Ϣ���ṹ
*/
typedef struct province_node
{
    char pvblg[9];              /*����ʡ��*/
    char disciplineperson[16];  /*�ͼ츺����*/
    char tele[17];              /*��ϵ�绰*/
    struct province_node *next; /*ָ����һ����ָ��*/
    struct case_node *cnext;    /*ָ�򰸼���Ϣ����ָ��*/
}PROVINCE_NODE;
/**
 *ͳ����������������̰�������ķ��������永���ṹ
 */
struct CRIMECASEMONEY
{
    char accuname;
    int sumcount;
    float summoney;
}crimecasemoney[6],crimecasemoneytmp;
/**
 *ͳ��ĳ����ڸ�����������̰�������ķ����������´����ṹ
 */
struct LEVELCOUNTPUNISHMENT
{
    char classlevel;
    int year[5];
    int sumcount1;
}levelcountpunishment[8];
/**
 *ͳ��ĳ��ʱ����ڸ�ʡ��̰�������ķ��������永��������ṹ
 */
struct PROVINCECOUNTMONEY
{
    char proname[9];
    int sumcount2;
    float summoney1;
}provincecountmoneytmp;
/**
 *ͳ��ý�屨��������10��̰����������������������������������š�
 *��������������Ҫ�������永�����´�����ý�屨������
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
 *ͳ�Ʋ�ͬý�屨���Ĵ���
 */
struct STATUSMEDIATYPE
{
    char mediatype;
    int cnt;
}statusmediatype[4],statusmediatypetmp;
/**
 *��Ļ������Ϣ�������ṹ
 */
typedef struct layer_node {
    char LayerNo;               /*�������ڲ���*/
    SMALL_RECT rcArea;          /*����������������*/
    CHAR_INFO *pContent;        /*�������������ַ���Ԫԭ��Ϣ�洢������*/
    char *pScrAtt;              /*�������������ַ���Ԫԭ����ֵ�洢������*/
    struct layer_node *next;    /*ָ����һ����ָ��*/
} LAYER_NODE;

/**
 *��ǩ���ṹ
 */
typedef struct labe1_bundle {
    char **ppLabel;             /*��ǩ�ַ��������׵�ַ*/
    COORD *pLoc;                /*��ǩ��λ�����׵�ַ*/
    int num;                    /*��ǩ����*/
} LABEL_BUNDLE;

/**
 *�����ṹ
 */
typedef struct hotarea_node {
    SMALL_RECT *pArea;          /*������λ�����׵�ַ*/
    char *pSort;                /*�������(�������ı���ѡ���)�����׵�ַ*/
    char *pTag;                 /*������������׵�ַ*/
    int num;                    /*��������*/
} HOT_AREA;

char *sysname = "̰��������Ϣ����ϵͳ";
char *proinfofile = "pro.txt";
char *mediainfofile = "media.txt";
char *caseinfofile = "case.txt";
char *probackupfile = "probackup.txt";
char *casebackupfile = "casebackup.txt";
char *mediabackupfile = "mediabackup.txt";

char *main_menu[] = {"�ļ�(F)","����ά��(M)","���ݲ�ѯ(Q)","����ͳ��(S)","����(H)"};             /*ϵͳ���˵���*/
char *sub_menu[] = {
                    "[S] ���ݱ���",          /*ϵͳ�Ӳ˵���*/
                    "[B] ���ݱ���",
                    "[R] ���ݻָ�",
                    "[X] �˳�ϵͳ",
                    "[D]����ʡ�ݼ����Ϣ",
                    "[P]̰������������Ϣ",
                    "[C]ý�屨��������Ϣ",
                    "[D]����ʡ�ݼ����Ϣ",
                    "[P]̰������������Ϣ",
                    "[C]ý�屨��������Ϣ",
                    "[I]������ͳ��",
                    "[T]�����������̷�ͳ��",
                    "[C]������ʡ��ͳ��",
                    "[U]����������ͳ��",
                    "[M]���������ͳ��",
                    "[T]��ϵ������",
                    "[A]��������Ϣ"
                    };

LAYER_NODE *layhead = NULL;
PROVINCE_NODE *head = NULL;

int sub_menu_num[] = {4, 3, 3, 5, 2};   /*�����˵������Ӳ˵��ĸ���*/
int selmainmenu = 1;                    /*���˵�ѡ�1��ʾѡ��*/
int selsubmenu = 0;                     /*�Ӳ˵�ѡ�0��ʾδѡ��*/
unsigned long ul;

CHAR_INFO *menubar = NULL;              /*��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
CHAR_INFO *statebar = NULL;             /*���״̬����Ļ���ַ���Ϣ�Ļ�����*/

char *scratt = NULL;                    /*�����Ļ���ַ���Ԫ����ֵ�Ļ�����*/
char sys_state = '\0';                   /*��������ϵͳ״̬���ַ�*/

HANDLE std_out;                         /*��׼����豸���*/
HANDLE std_in;                          /*��׼�����豸���*/

/*�Զ��庯��*/
int CreateList(PROVINCE_NODE **phead);/*�������ļ���ȡ��������, ����ŵ���������ʮ��������*/
int ReCreateList(PROVINCE_NODE **phead,char *profile,char *casefile,char *mediafile);/*�������ļ���ȡ��������, ����ŵ���������ʮ��������*/

PROVINCE_NODE *SeekProvinceNode(char *pvblg);/*������ʡ�ݲ��Ұ���������Ϣ���*/
CASE_NODE *SeekCaseNode(char *casenum);/*��������Ų��Ұ���������Ϣ���*/
MEDIA_NODE *SeekMediaNode(char *casenum,char *pvblg);/*��������ź�ý�屨�����ڲ���ý����Ϣ�ڵ�*/



int InsertMediaNode(MEDIA_NODE *pmedianode);/*��ʮ�������в���һ��ý����Ϣ���*/
int DeleteMediaNode(char *casenum,char *reportdate);/*��ʮ��������ɾ��һ��ý����Ϣ���*/
int ModifyMediaNode(char *casenum,char *reportdate,MEDIA_NODE *pmedianode);/*��ָ����ý����Ϣ������ݽ����޸�*/
int InsertCaseNode(char *pvblg,CASE_NODE *pcasenode);/*��ʮ�������в���һ��������Ϣ���*/
int DeleteCaseNode(char *casenum,char *pvblg);/*��ʮ��������ɾ��һ��������Ϣ���*/
int ModifyCaseNode(CASE_NODE *pcasenode,char *casenum);/*��ָ���İ�����Ϣ������ݽ����޸�*/
int InsertProvinceNode(PROVINCE_NODE **head,PROVINCE_NODE *pprovincenode);/*��ʮ�������в���һ��ʡ����Ϣ���*/
int DeleteProvinceNode(char *pvblg);/*��ʮ��������ɾ��һ��ʡ����Ϣ���*/
int ModifyProvinceNode(PROVINCE_NODE *pprovincenode,char *pvblg);/*��ָ����ʡ����Ϣ������ݽ����޸�.*/

PROVINCE_NODE *QueryProvinceInfo1(char *pvblg);/*��ָ����ʡ����Ϣ������ݽ��в�ѯ*/
PROVINCE_NODE *QueryProvinceInfo2(char *disciplineperson);/*��ָ����ʡ����Ϣ������ݽ��в�ѯ*/
CASE_NODE *QueryCaseInfo1(char *casenum);/*��ָ���İ�����Ϣ������ݽ��в�ѯ.*/
CASE_NODE *QueryCaseInfo2(char *crimename,char level);/*��ָ���İ�����Ϣ������ݽ��в�ѯ.*/
MEDIA_NODE *QueryMediaInfo1(char *casenum,char *medianame);/*��ָ����ý����Ϣ������ݽ��в�ѯ.*/
MEDIA_NODE *QueryMediaInfo2(char *reportdate,char *medianame);/*��ָ����ý����Ϣ������ݽ��в�ѯ.*/
/*����Ҫ����Ϊ�����ֱ�ͳ����������������̰�������ķ��������永��
������������������������ͳ�ƽ��*/
BOOL StatusCrimeName(void);
/*����������Ϊ�����ֱ�ͳ��ĳ����ڸ�����������̰�������ķ�������
���в�ͬ�����´��������ֱ��Ƕ�����*/
BOOL StatusLevelPunish(void);
/*�԰�������������ʡ��Ϊ�����ֱ�ͳ��ĳ��ʱ����ڸ�ʡ��̰�������ķ�������
�永���������������������������������ͳ�ƽ��*/
BOOL StatusProvince(char *startdate,char *enddate);
BOOL StatusReportTime(void);/*ͳ��ý�屨��������10��̰������*/
BOOL StatusReportType(void);/*ͳ��ý�屨������*/


BOOL LoadData(void);
void RunSystem(void);/*����ϵͳ, ��ϵͳ�������������û���ѡ��Ĺ���ģ��*/
void CloseSystem(void);/*�ر�ϵͳ*/
void InInterface(void);/*��ʼ������*/
void ClearScreen(void);/*�����Ļ��Ϣ*/
void ShowMenu(void);/*����Ļ����ʾ���˵�, ����������, �����˵���һ������ѡ�б��*/
void ShowState(void);/*��ʾ״̬��*/
void TagMainMenu(int num);/*��ָ�����˵�������ѡ�б�־*/
void PopMenu(int num);/*����ָ�����˵����Ӧ���Ӳ˵�*/
void PopUp(SMALL_RECT *srpop, WORD att, LABEL_BUNDLE *labels, HOT_AREA *areas);/*��ָ�������������������Ϣ, ͬʱ��������, ����������λ����Ϣ��ջ.*/
void PopOff(void);/*�رն��㵯������, �ָ���������ԭ��ۺ��ַ���Ԫԭ����*/
void DrawBox(SMALL_RECT *pRc);/*��ָ�����򻭱߿�*/
void TagSubMenu(int num);/*��ָ���Ӳ˵�������ѡ�б��*/
void CalLocation(int num, SMALL_RECT *rc);/*���㵯���Ӳ˵��������ϽǺ����½ǵ�λ��*/
int DealInput(HOT_AREA *pHotArea, int *piHot);/*�ڵ�������������������, �ȴ��û�����*/
void SetHotPoint(HOT_AREA *pHotArea, int iHot);/*��������*/

BOOL ExeFunction(int m, int s);/*ִ�������˵��ź��Ӳ˵���ȷ���Ĺ��ܺ���*/

/*�������ܺ�����Ӧ�Ľ���*/

BOOL FileDataSave(void);/*���ݱ���*/
BOOL FileDataBackUp(void);/*���ݱ���*/
BOOL FileDataRestore_Interface(void);/*�����������*/
BOOL FileDataRestore(char *profile,char *casefile,char *mediafile);/*��������*/
BOOL FileExit(void);/*�˳�*/

BOOL MaintainProInfo(void);/*ʡ����Ϣά��*/
void InputProInfo(void);/*����ʡ����Ϣ����*/
void DelProInfo(void);/*ɾ��ʡ����Ϣ����*/
void ModifyProInfo(void);/*�޸�ʡ����Ϣ����1*/
void InputModifyProInfo(void);/*�޸�ʡ����Ϣ����2*/

BOOL MaintainCaseInfo(void);/*������Ϣά��*/
void InputCaseInfo(void);/*���Ӱ�����Ϣ����*/
void DelCaseInfo(void);/*ɾ��������Ϣ����*/
void ModifyCaseInfo(void);/*�޸İ�����Ϣ����1*/
void InputModifyCaseInfo(void);/*�޸İ�����Ϣ����2*/

BOOL MaintainMediaInfo(void);/*ý�屨����Ϣά��*/
void InputMediaInfo(void);/*����ý�屨����Ϣ����*/
void DelMediaInfo(void);/*ɾ��ý�屨����Ϣ����*/
void ModifyMediaInfo(void);/*�޸�ý�屨����Ϣ����1*/
void InputModifyMediaInfo(void);/*�޸�ý�屨����Ϣ����2*/

BOOL QueryProInfo(void);/*��ѯʡ����Ϣ*/
void QueryProname(void);/*��ʡ������ѯ����*/
PROVINCE_NODE *QueryProvinceInfo1(char *pvblg);/*��ʡ������ѯ*/
void QueryPerson(void);/*���ͼ츺���˲�ѯ����*/
PROVINCE_NODE *QueryProvinceInfo2(char *disciplineperson);/*���ͼ츺���˲�ѯ*/

BOOL QueryCaseInfo(void);/*��ѯ������Ϣ*/
void QueryCasenum(void);/*��������Ų�ѯ����*/
CASE_NODE *QueryCaseInfo1(char *casenum);/*��������Ų�ѯ*/
void QueryCrimer(void);/*�����������˲�ѯ����*/
CASE_NODE *QueryCaseInfo2(char *crimename,char level);/*�����������˲�ѯ*/

BOOL QueryMediaInfo(void);/*��ѯý����Ϣ*/
void QueryCaseMedia(void);/*��������Ų�ѯ����*/
MEDIA_NODE *QueryMediaInfo1(char *casenum,char *medianame);/*��������Ų�ѯ*/
void QueryDateMedia(void);/*���������ڲ�ѯ����*/
MEDIA_NODE *QueryMediaInfo2(char *reportdate,char *medianame);/*���������ڲ�ѯ*/

BOOL HelpTheme(void);/*��������*/
BOOL HelpAbout(void);/*���ڰ���*/

BOOL ShowModule(char **pString, int n);/* ��ʾ���ܺ�����Ӧ�Ľ���*/
#endif                  //CORRU_H_INCLUDED
