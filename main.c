#include "tes.h"
int main()
{
    COORD size = {SCR_COL, SCR_ROW};                /*���ڻ�������С*/

    std_out = GetStdHandle(STD_OUTPUT_HANDLE);      /* ��ȡ��׼����豸���*/
    std_in = GetStdHandle(STD_INPUT_HANDLE);        /* ��ȡ��׼�����豸���*/

    SetConsoleTitle(sysname);                       /*���ô��ڱ���*/
    SetConsoleScreenBufferSize(std_out, size);      /*���ô��ڻ�������С80*25*/

    LoadData();
    InInterface();
    RunSystem();
    CloseSystem();

    return 0;
}

/**
 * ��������: LoadData
 * ��������: ������������������ݴ������ļ����뵽�ڴ滺������ʮ��������.
 * �������: ��
 * �������: ��
 * �� �� ֵ: BOOL����, ���ܺ����г��˺���ExitSys�ķ���ֵ����ΪFALSE��,
 *           ���������ķ���ֵ����ΪTRUE.
 *
 * ����˵��:
 */
BOOL LoadData()
{
    int Re;
    Re = CreateList(&head);
    sys_state |= Re;
    sys_state &= ~(4 + 8 + 16 - Re);
    if (sys_state < (4 | 8 | 16))
    {  /*���ݼ�����ʾ��Ϣ*/
        printf("\nϵͳ�������ݲ�����!\n");
        printf("\n�����������...\n");
        getch();
    }

    return TRUE;
}

/**
 * ��������: CreateList
 * ��������: �������ļ���ȡ��������, ����ŵ���������ʮ��������.
 * �������: ��
 * �������: phead ����ͷָ��ĵ�ַ, ����������������ʮ����.
 * �� �� ֵ: int����ֵ, ��ʾ�����������.
 *           0  ����, ������
 *           4  �Ѽ���ʡ����Ϣ���ݣ��ް�����Ϣ��ý����Ϣ����
 *           12 �Ѽ��ذ�����Ϣ���ݣ���ý����Ϣ����
 *           28 ����������ݶ��Ѽ���
 */
int CreateList(PROVINCE_NODE **phead)
{
    int flag = 0;
    int re = 0;
    PROVINCE_NODE *prohead = NULL, *pPROVINCENODE,struct1;
    CASE_NODE *pCASENODE,struct2;
    MEDIA_NODE *pMEDIANODE,struct3;
    FILE *pf;
    if((pf = fopen(proinfofile,"r")) == NULL)
    {
        printf("ʡ����Ϣ�ļ���ʧ�ܣ�\n");
        return re;
    }
    printf("ʡ����Ϣ�ļ��򿪳ɹ���\n");
    while(fscanf(pf,"%s %s %s\n",struct1.pvblg,struct1.disciplineperson,struct1.tele)==3)
    {
        pPROVINCENODE = (PROVINCE_NODE *)malloc(sizeof(PROVINCE_NODE));
        *pPROVINCENODE = struct1;
        pPROVINCENODE->cnext = NULL;
        pPROVINCENODE->next = prohead;                 /*��������ȳ�������*/
        prohead = pPROVINCENODE;                       /*��������ȳ�������*/
    }
    fclose(pf);
    if(prohead == NULL)
    {
        printf("ʡ����Ϣ����ʧ�ܣ�\n");
        return re;
    }
    printf("ʡ����Ϣ���سɹ���\n");
    *phead = prohead;                                  /*������ʮ�ֽ��������ͷ*/

    fclose(pf);
    re += 4;

    if((pf = fopen(caseinfofile,"r")) == NULL)
    {
        printf("������Ϣ��ʧ�ܣ�\n");
        return re;
    }
    printf("������Ϣ�򿪳ɹ���\n");
    re += 8;

    /*�������ļ���ȡ������Ϣ���ݣ�����������Ӧ�ڵ�İ�����Ϣ֧����*/
    while(fscanf(pf,"%s %s %s %s %c %c %f %d\n",struct2.casenum,
                 struct2.casedate,struct2.pvblg,struct2.crimename,
                 &struct2.level,&struct2.accusationname,
                 &struct2.money,&struct2.punishment)==8)
    {
        pCASENODE = (CASE_NODE *)malloc(sizeof(CASE_NODE));
        *pCASENODE = struct2;
        pCASENODE->mnext = NULL;
        pPROVINCENODE = prohead;

        while((pPROVINCENODE != NULL) && (strcmp(pPROVINCENODE->pvblg,pCASENODE->pvblg)) != 0)
        {
            pPROVINCENODE = pPROVINCENODE->next;
        }

        if(pPROVINCENODE != NULL)
        {
            pCASENODE->next = pPROVINCENODE->cnext;         /*��������ȳ�������*/
            pPROVINCENODE->cnext = pCASENODE;              /*��������ȳ�������*/
        }
        else
        {
            free(pCASENODE);
        }
    }
    fclose(pf);
    if((pf = fopen(mediainfofile,"r")) == NULL)
    {
        printf("ý�屨����Ϣ��ʧ��!\n");
        return re;
    }
    printf("ý�屨����Ϣ�򿪳ɹ���\n");
    re += 16;

    while(fscanf(pf,"%s %s %s %c %s %s\n",struct3.pvblg,
                 struct3.casenum,struct3.reportdate,
                 &struct3.mediatype,struct3.medianame,struct3.contentindex)==6)
    {
        pMEDIANODE = (MEDIA_NODE *)malloc(sizeof(MEDIA_NODE));
        *pMEDIANODE = struct3;
        flag = 0;
        for(pPROVINCENODE = prohead;pPROVINCENODE != NULL && flag==0;pPROVINCENODE = pPROVINCENODE->next)
        {
            for(pCASENODE = pPROVINCENODE->cnext;pCASENODE != NULL && flag==0;pCASENODE = pCASENODE->next)
            {
                if(strcmp(pCASENODE->casenum,pMEDIANODE->casenum) == 0)
                {
                    flag = 1;
                    break;
                }
            }
            if(flag)
            {
                break;
            }
        }

        if(flag)
        {
            pMEDIANODE->next = pCASENODE->mnext;
            pCASENODE->mnext = pMEDIANODE;
        }
        else
        {
            free(pMEDIANODE);
        }
    }
    fclose(pf);
    return re;
}

/**
 * ��������: RunSystem
 * ��������: ����ϵͳ, ��ϵͳ�������������û���ѡ��Ĺ���ģ��.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void RunSystem(void)
{
    INPUT_RECORD input;
    DWORD dword;
    int num= 0;/*�Ӳ˵�λ��*/
    int i;
    int tmp;/*�Ӳ˵�λ��*/
    char vkc,asc;
    INPUT_RECORD inRec;

    while(1)
    {
        ReadConsoleInput(std_in,&input,1,&dword);
        vkc = input.Event.KeyEvent.wVirtualKeyCode;
        asc = input.Event.KeyEvent.uChar.AsciiChar;
        if(input.EventType == KEY_EVENT && input.Event.KeyEvent.bKeyDown == TRUE)
        {
            /*ϵͳ��ݼ��Ĵ���*/
            if (vkc == 67) /*�������C��*/
            {
                if (layhead->LayerNo != 0) /*�����ǰ���Ӳ˵�����*/
                {
                    PopOff();            /*�رյ������Ӳ˵�*/
                    selsubmenu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
                }
                ExeFunction(5, 1);  /*���а������⹦�ܺ���*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
            { /*������������Alt��*/
                switch (vkc)  /*�ж���ϼ�Ctrl+��ĸ*/
                {
                    case 70:  /*Ctrl+F*/
                        PopMenu(1);
                        break;
                    case 77: /*Ctrl+M*/
                        PopMenu(2);
                        break;
                    case 81: /*Ctrl+Q*/
                        PopMenu(3);
                        break;
                    case 83: /*Ctrl+S*/
                        PopMenu(4);
                        break;
                    case 72: /*Ctrl+H*/
                        PopMenu(5);
                        break;
                }
            }
            else if(asc == 0)/*�Է�������ĸ���Ĵ���*/
            {
                if(layhead->LayerNo == 0)/*���δ�����Ӳ˵�*/

                {
                    switch(vkc)
                    {
                        case 37:
                            selmainmenu--;
                            if(selmainmenu == 0)
                            {
                                selmainmenu = 5;
                            }
                            TagMainMenu(selmainmenu);
                            break;
                        case 39:
                            selmainmenu++;
                            if(selmainmenu == 6)
                            {
                                selmainmenu = 1;
                            }
                            TagMainMenu(selmainmenu);
                            break;
                        case 40:
                            PopMenu(selmainmenu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else /*�ѵ����Ӳ˵�ʱ*/
                {
                    for(num=0,i=1; i<selmainmenu; i++)
                    {
                        /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                        num += sub_menu_num[i-1];
                    }
                    switch(vkc)
                    {
                        case 37:
                            selmainmenu--;
                            if(selmainmenu < 1)
                            {
                                selmainmenu = 5;
                            }
                            TagMainMenu(selmainmenu);
                            PopOff();
                            PopMenu(selmainmenu);
                            TagSubMenu(1);
                            break;
                        case 38:
                            tmp = selsubmenu - 1;
                            if (tmp < 1)
                            {
                                tmp = sub_menu_num[selmainmenu-1];
                            }
                            if (strlen(sub_menu[num+tmp-1]) == 0)
                            {
                                tmp--;
                            }
                            TagSubMenu(tmp);
                            break;
                        case 39:
                            selmainmenu++;
                            if(selmainmenu > 5)
                            {
                                selmainmenu = 1;
                            }
                            TagMainMenu(selmainmenu);
                            PopOff();
                            PopMenu(selmainmenu);
                            TagSubMenu(1);
                            break;
                        case 40:
                            tmp = selsubmenu + 1;
                            if (tmp > sub_menu_num[selmainmenu-1])
                            {
                                tmp = 1;
                            }
                            if (strlen(sub_menu[num+tmp-1]) == 0)
                            {
                                tmp++;
                            }
                            TagSubMenu(tmp);
                            break;
                    }
                }
            }
            else if ((asc-vkc == 0) || (asc-vkc == 32))
            {  /*������ͨ��*/
                if (layhead->LayerNo == 0)  /*���δ�����Ӳ˵�*/
                {
                    switch (vkc)
                    {
                        case 70: /*f��F*/
                            PopMenu(1);
                            break;
                        case 77: /*m��M*/
                            PopMenu(2);
                            break;
                        case 81: /*q��Q*/
                            PopMenu(3);
                            break;
                        case 83: /*s��S*/
                            PopMenu(4);
                            break;
                        case 72: /*h��H*/
                            PopMenu(5);
                            break;
                        case 13: /*�س�*/
                            PopMenu(selmainmenu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else /*�ѵ����Ӳ˵�ʱ�ļ������봦��*/
                {
                    if (vkc == 27) /*�������ESC��*/
                    {
                        PopOff();
                        selsubmenu = 0;
                    }
                    else if(vkc == 13) /*������»س���*/
                    {
                        tmp = selsubmenu;
                        PopOff();
                        selsubmenu = 0;
                        ExeFunction(selmainmenu, tmp);

                    }
                    else /*������ͨ���Ĵ���*/
                    {
                        /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
                        for (num=0,i=1; i<selmainmenu; i++)
                        {
                            num += sub_menu_num[i-1];
                        }

                        /*�����뵱ǰ�Ӳ˵���ÿһ��Ĵ����ַ����бȽ�*/
                        for (i=num; i<num+sub_menu_num[selmainmenu-1]; i++)
                        {
                            if (strlen(sub_menu[i])>0 && vkc==sub_menu[i][1])
                            { /*���ƥ��ɹ�*/
                                PopOff();
                                selsubmenu = 0;
                                ExeFunction(selmainmenu, i-num+1);
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * ��������: CloseSystem
 * ��������: �ر�ϵͳ
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void CloseSystem(void)
{
    PROVINCE_NODE *hd1, *hd2;
    CASE_NODE *casenode1,*casenode2;
    MEDIA_NODE *medianode1,*medianode2;

    for(hd1 = head; hd1 != NULL; hd1 = hd1->next)
    {
        hd2 = hd1->next;
        for(casenode1 = hd1->cnext; casenode1 != NULL; casenode1 = casenode1->next)
        {
            casenode2 = casenode1->next;
            for(medianode1 = casenode1->mnext; medianode1 != NULL; medianode1 = medianode1->next)
            {
                medianode2 = medianode1->next;
                free(medianode1);
                medianode1 = medianode2;
            }
            casenode1 = casenode2;
        }
        hd1 = hd2;
    }

    /*����*/
    ClearScreen();

    /*�ͷŴ�Ų˵�����״̬������Ϣ��̬�洢��*/
    free(menubar);
    free(statebar);

    /*�رձ�׼���������豸���*/
    CloseHandle(std_out);
    CloseHandle(std_in);

    /*�����ڱ�������Ϊ���н���*/
    SetConsoleTitle("���н���");
}

/**
 * ��������: InInterface
 * ��������: ��ʼ������.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void InInterface(void)
{
    WORD attri = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE;
    SetConsoleTextAttribute(std_out,attri);

    ClearScreen();
    scratt = (char *)calloc(SCR_ROW * SCR_COL, sizeof(char));
    layhead = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    layhead->LayerNo = 0;
    layhead->rcArea.Left = 0;
    layhead->rcArea.Top = 0;
    layhead->rcArea.Bottom = SCR_ROW - 1;
    layhead->rcArea.Right = SCR_COL - 1;
    layhead->pContent = NULL;
    layhead->pScrAtt = scratt;
    layhead->next = NULL;

    ShowMenu();
    /*ShowState();*/
}

/**
 * ��������: ClearScreen
 * ��������: �����Ļ��Ϣ.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void ClearScreen(void)
{
    CONSOLE_SCREEN_BUFFER_INFO scrbuf;
    COORD home = {0, 0};
    unsigned long size;

    GetConsoleScreenBufferInfo(std_out, &scrbuf );/*ȡ��Ļ��������Ϣ*/
    size =  scrbuf.dwSize.X * scrbuf.dwSize.Y; /*������Ļ�������ַ���Ԫ��*/

    /*����Ļ���������е�Ԫ���ַ���������Ϊ��ǰ��Ļ�������ַ�����*/
    FillConsoleOutputAttribute(std_out, scrbuf.wAttributes, size, home,&ul);

    /*����Ļ���������е�Ԫ���Ϊ�ո��ַ�*/
    FillConsoleOutputCharacter(std_out, ' ', size, home, &ul);
}

/**
 * ��������: ShowMenu
 * ��������: ����Ļ����ʾ���˵�, ����������, �����˵���һ������ѡ�б��.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void ShowMenu(void)
{
    int i;
    CONSOLE_SCREEN_BUFFER_INFO scrbuf;
    CONSOLE_CURSOR_INFO curinfo;
    COORD size,startpos = {0,0},endpos = {0,4};
    GetConsoleScreenBufferInfo(std_out,&scrbuf);        /*�Ȼ����Ļ����������Ϣ*/
    size.X = scrbuf.dwSize.X;                           /*���˵�����size*/
    size.Y = 1;
    SetConsoleCursorPosition(std_out,startpos);         /*���ù���λ��*/

    for(i=0;i<M;i++)                                    /*������˵�����ѡ����Ϣ*/
    {
        printf("  %s  ",main_menu[i]);
    }
    GetConsoleCursorInfo(std_out,&curinfo);             /*����ƶ��󣬻�ȡ���λ����Ϣ*/
    curinfo.bVisible = FALSE;
    SetConsoleCursorInfo(std_out,&curinfo);             /*���ع��*/
    /*����˵�����Ļ���ַ���Ϣ������*/
    menubar = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    /*�������ݵ��˵�����Ļ���ַ���Ϣ������*/
    SMALL_RECT srmenu = {0,0,size.X,0};
    ReadConsoleOutput(std_out,menubar,size,startpos,&srmenu);

    for(i=0;i<M;i++)
    {
        /*���ò˵�����ɫ��Ϣ*/
        (menubar+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
        /*char ch = (char)((menubar+i)->Char.AsciiChar);
        (menubar+i)->Attributes |= FOREGROUND_RED;*/
    }

    /*���޸ĺ�Ĳ˵����ַ���Ϣд����Ļ��һ��*/
    WriteConsoleOutput(std_out,menubar,size,startpos,&srmenu);
    SetConsoleCursorPosition(std_out,endpos);           /*����������ڵ�һ�е�����*/
    TagMainMenu(1);
}

/**
 * ��������: ShowState
 * ��������: ��ʾ״̬��.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��: ״̬���ַ�����Ϊ�׵׺���, ��ʼ״̬��״̬��Ϣ.
 */
void ShowState(void)
{
    int i;
    CONSOLE_SCREEN_BUFFER_INFO scrbuf;
    COORD size;
    COORD pos = {0, 0};
    GetConsoleScreenBufferInfo(std_out,&scrbuf);
    size.X = scrbuf.dwSize.X;
    size.Y = 1;
    SMALL_RECT srmenu ={0, scrbuf.dwSize.Y-1, size.X-1, scrbuf.dwSize.Y-1};
    if (statebar == NULL)
    {
        statebar = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
        ReadConsoleOutput(std_out, statebar, size, pos, &srmenu);
    }
    for (i=0; i<size.X; i++)
        (statebar+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;/*�׵׺���*/
    WriteConsoleOutput(std_out, statebar, size, pos, &srmenu);
}

/**
 * ��������: TagMainMenu
 * ��������: ��ָ�����˵�������ѡ�б�־.
 * �������: num ѡ�е����˵����
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void TagMainMenu(int num)
{
    CONSOLE_SCREEN_BUFFER_INFO scrbuf;
    COORD size;
    COORD pos = {0,0};
    int i, posa = 2, posb;

    if(num == 0)
    {
        posa = 0,posb = 0;
    }
    else
    {
        for(i=1;i<num;i++)
        {
            posa += 4+strlen(main_menu[i-1]);
        }
        posb = posa+strlen(main_menu[num-1]);
    }
    GetConsoleScreenBufferInfo(std_out,&scrbuf);
    size.X = scrbuf.dwSize.X;                         /*��ȡ��Ļ�Ŀ��*/
    size.Y = 1;

    /*ȥ��ѡ�в˵���֮ǰ�Ĳ˵���ı��*/
    for(i=0;i<posa;i++)
    {
        (menubar+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    }

    /*ȥ��ѡ�в˵���Ĳ˵���ı��*/
    for(i=posb;i<scrbuf.dwSize.X;i++)
    {
        (menubar+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    }

    /*��ѡ�еĲ˵�������ǣ��ڵװ���*/
    for(i=posa;i<posb;i++)
    {
        (menubar+i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }

    /*����ǺõĲ˵�����Ϣд�����ڵ�һ��*/
    SMALL_RECT srmenu = {0,0,size.X-1,0};
    WriteConsoleOutput(std_out,menubar,size,pos,&srmenu);
}

/**
 * ��������: PopMenu
 * ��������: ����ָ�����˵����Ӧ���Ӳ˵�.
 * �������: num ָ�������˵����
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void PopMenu(int num)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    int i, j, loc = 0;

    if (num != selmainmenu)       /*���ָ�����˵�������ѡ�в˵�*/
    {
        if (layhead->LayerNo != 0) /*�����ǰ�����Ӳ˵�����*/
        {
            PopOff();
            selsubmenu = 0;
        }
    }
    else if (layhead->LayerNo != 0) /*���ѵ������Ӳ˵����򷵻�*/
    {
        return;
    }

    selmainmenu = num;
    TagMainMenu(selmainmenu);
    CalLocation(selmainmenu,&srpop);/*���㵯���Ӳ˵�������λ��, �����srpop��*/

    /*�����Ӳ˵���һ�����ڲ˵������е��±�*/
    for(i=1; i<selmainmenu; i++)
    {
        loc += sub_menu_num[i-1];
    }
    /*���Ӳ˵�������ǩ�ṹ*/
    labels.ppLabel = sub_menu + loc; /*��ǩ����һ����ǩ�ַ����ĵ�ַ*/
    labels.num = sub_menu_num[selmainmenu-1];/*��ǩ���б�ǩ�ַ����ĸ���*/
    COORD alabelloc[labels.num];/*�����ǩ������С�ı�ǩλ������*/

    for(i=0; i<labels.num; i++)
    {
        alabelloc[i].X = srpop.Left + 2;
        alabelloc[i].Y = srpop.Top + i + 1;
    }

    labels.pLoc = alabelloc;
    /*��������*/
    areas.num = labels.num;/*�����ĸ��������ڱ�ǩ�ĸ��������Ӳ˵�������*/
    SMALL_RECT arealoc[areas.num];/*������������������λ��*/
    char aSort[areas.num];/*��������������������Ӧ���*/
    char aTag[areas.num]; /*����������ÿ�������ı��*/

    for(i=0; i<areas.num; i++)
    {
        arealoc[i].Left = srpop.Left + 2;
        arealoc[i].Top = srpop.Top + i + 1;
        arealoc[i].Right = srpop.Right - 2;
        arealoc[i].Bottom = srpop.Top;
        aSort[i] = 0;/*�������Ϊ0(��ť��)*/
        aTag[i] = i + 1;
    }

    areas.pArea = arealoc;
    areas.pSort = aSort;
    areas.pTag = aTag;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    PopUp(&srpop, att, &labels, &areas);
    DrawBox(&srpop);
    pos.X = srpop.Left + 2;

    for(pos.Y=srpop.Top+1; pos.Y<srpop.Bottom; pos.Y++)
    {
        if(strlen(sub_menu[loc+pos.Y-srpop.Top-1]) == 0)
        {
            FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-3,pos,&ul);
            for (j=srpop.Left+2; j<srpop.Right-1; j++)
            {   /*ȡ���������ַ���Ԫ����������*/
                scratt[pos.Y*SCR_COL+j] &= 3; /*��λ��Ľ�������˵���λ*/
            }
        }

    }
}

/**
 * ��������: PopUp
 * ��������: ��ָ�������������������Ϣ, ͬʱ��������, ����������λ����Ϣ��ջ.
 * �������: srpop ��������λ�����ݴ�ŵĵ�ַ
 *           att �������������ַ�����
 *           labels ���������б�ǩ����Ϣ��ŵĵ�ַ
             areas ����������������Ϣ��ŵĵ�ַ
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void PopUp(SMALL_RECT *srpop, WORD att, LABEL_BUNDLE *labels, HOT_AREA *areas)
{
    int i,j,row;
    char *pch;
    LAYER_NODE *nextlayer;
    COORD pos = {0,0};
    COORD size;

    /*���õ������ڴ�С*/
    size.X = srpop->Right - srpop->Left + 1;
    size.Y = srpop->Bottom - srpop->Top + 1;

    /*�����ŵ���������Ϣ�Ķ�̬������*/
    nextlayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextlayer->next = layhead;
    nextlayer->LayerNo = layhead->LayerNo + 1;
    nextlayer->pContent = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    nextlayer->pScrAtt = (char *)malloc(size.X * size.Y * sizeof(char));
    nextlayer->rcArea = *srpop;
    pch = nextlayer->pScrAtt;

    /*���������ڸ����������Ϣ��������*/
    ReadConsoleOutput(std_out,nextlayer->pContent,size,pos,srpop);

    for(i=srpop->Top; i<=srpop->Bottom; i++)
    {
        for(j=srpop->Left; j<=srpop->Right; j++)
        {
            *pch = scratt[i*SCR_COL+j];
            pch++;
        }
    }

    /*����������Ϣ��ջ*/
    layhead = nextlayer;
    pos.X = srpop->Left;
    pos.Y = srpop->Top;
    for(i=srpop->Top; i<=srpop->Bottom; i++)
    {
        FillConsoleOutputAttribute(std_out, att, size.X, pos, &ul);
        pos.Y++;
    }
    /*����ǩ���еı�ǩ�ַ������趨��λ�����*/
    for (i=0; i<labels->num; i++)
    {
        pch = labels->ppLabel[i];
        if (strlen(pch) != 0)
        {
            WriteConsoleOutputCharacter(std_out, pch, strlen(pch),labels->pLoc[i], &ul);
        }
    }
    /*���õ������������ַ���Ԫ��������*/
    for (i=srpop->Top; i<=srpop->Bottom; i++)
    {   /*�˶���ѭ�������ַ���Ԫ�Ĳ��*/
        for (j=srpop->Left; j<=srpop->Right; j++)
        {
            scratt[i*SCR_COL+j] = layhead->LayerNo;
        }
    }

    for (i=0; i<areas->num; i++)
    {   /*�˶���ѭ�����������������ַ���Ԫ���������ͺ��������*/
        row = areas->pArea[i].Top;
        for (j=areas->pArea[i].Left; j<=areas->pArea[i].Right; j++)
        {
            scratt[row*SCR_COL+j] |= (areas->pSort[i] << 6) | (areas->pTag[i] << 2);
        }
    }
}

/**
 * ��������: PopOff
 * ��������: �رն��㵯������, �ָ���������ԭ��ۺ��ַ���Ԫԭ����.
 * �������: ��
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void PopOff(void)
{
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pch;
    int i, j;

    if ((layhead->next==NULL) || (layhead->pContent==NULL))
    {   /*ջ�״�ŵ���������Ļ��Ϣ�����ùر�*/
        return;
    }
    nextLayer = layhead->next;
    /*�ָ�������������ԭ���*/
    size.X = layhead->rcArea.Right - layhead->rcArea.Left + 1;
    size.Y = layhead->rcArea.Bottom - layhead->rcArea.Top + 1;
    WriteConsoleOutput(std_out, layhead->pContent, size, pos, &(layhead->rcArea));
    /*�ָ��ַ���Ԫԭ����*/
    pch = layhead->pScrAtt;
    for (i=layhead->rcArea.Top; i<=layhead->rcArea.Bottom; i++)
    {
        for (j=layhead->rcArea.Left; j<=layhead->rcArea.Right; j++)
        {
            scratt[i*SCR_COL+j] = *pch;
            pch++;
        }
    }
    free(layhead->pContent);    /*�ͷŶ�̬�洢��*/
    free(layhead->pScrAtt);
    free(layhead);
    layhead = nextLayer;
    selsubmenu = 0;
}

/**
 * ��������: DrawBox
 * ��������: ��ָ�����򻭱߿�.
 * �������: pRc �������λ����Ϣ�ĵ�ַ
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|'};  /*�����õ��ַ�*/
    COORD pos = {pRc->Left, pRc->Top};  /*��λ����������Ͻ�*/

    WriteConsoleOutputCharacter(std_out, &chBox[0], 1, pos, &ul);/*���߿����Ͻ�*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*��ѭ�����ϱ߿����*/
        WriteConsoleOutputCharacter(std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(std_out, &chBox[0], 1, pos, &ul);/*���߿����Ͻ�*/
    for (pos.Y = pRc->Top+1; pos.Y < pRc->Bottom; pos.Y++)
    {   /*��ѭ�����߿�����ߺ��ұ���*/
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(std_out, &chBox[2], 1, pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(std_out, &chBox[2], 1, pos, &ul);
    }
    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(std_out, &chBox[0], 1, pos, &ul);/*���߿����½�*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*���±߿����*/
        WriteConsoleOutputCharacter(std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(std_out, &chBox[0], 1, pos, &ul);/*���߿����½�*/
}

/**
 * ��������: TagSubMenu
 * ��������: ��ָ���Ӳ˵�������ѡ�б��.
 * �������: num ѡ�е��Ӳ˵����
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void TagSubMenu(int num)
{
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    int width;

    CalLocation(selmainmenu, &srpop);  /*���㵯���Ӳ˵�������λ��, �����srpop��*/
    if ((num<1) || (num == selsubmenu) || (num>srpop.Bottom-srpop.Top-1))
    {   /*����Ӳ˵����Խ�磬������Ӳ˵��ѱ�ѡ�У��򷵻�*/
        return;
    }

    pos.X = srpop.Left + 2;
    width = srpop.Right - srpop.Left - 3;
    if (selsubmenu != 0) /*����ȡ��ԭѡ���Ӳ˵����ϵı��*/
    {
        pos.Y = srpop.Top + selsubmenu;
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
        FillConsoleOutputAttribute(std_out, att, width, pos, &ul);
        pos.X += 1;
    }
    /*��ָ���Ӳ˵�������ѡ�б��*/
    pos.X = srpop.Left + 2;
    pos.Y = srpop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
    FillConsoleOutputAttribute(std_out, att, width, pos, &ul);
    selsubmenu = num;  /*�޸�ѡ���Ӳ˵����*/
    return;
}

/**
 * ��������: CalLocation
 * ��������: ���㵯���Ӳ˵��������ϽǺ����½ǵ�λ��.
 * �������: num ѡ�е����˵����
 * �������: rc �������λ����Ϣ�ĵ�ַ
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void CalLocation(int num, SMALL_RECT *rc)
{
    int i, len, loc = 0;

    rc->Top = 1; /*������ϱ߶��ڵ�2�У��к�Ϊ1*/
    rc->Left = 1;
    for (i=1; i<num; i++)
    {   /*����������߽�λ��, ͬʱ�����һ���Ӳ˵������Ӳ˵��ַ��������е�λ��*/
        rc->Left += strlen(main_menu[i-1]) + 4;
        loc += sub_menu_num[i-1];
    }
    rc->Right = strlen(sub_menu[loc]);/*��ʱ��ŵ�һ���Ӳ˵����ַ�������*/

    for (i=1; i<sub_menu_num[num-1]; i++)
    {   /*������Ӳ˵��ַ��������䳤�ȴ����rc->Right*/
        len = strlen(sub_menu[loc+i]);
        if (rc->Right < len)
        {
            rc->Right = len;
        }
    }

    rc->Right += rc->Left + 3;  /*����������ұ߽�*/
    rc->Bottom = rc->Top + sub_menu_num[num-1] + 1;/*���������±ߵ��к�*/
    if (rc->Right >= SCR_COL)  /*�ұ߽�Խ��Ĵ���*/
    {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
}

/**
 * ��������: DealInput
 * ��������: �ڵ�������������������, �ȴ��û�����.
 * �������: pHotArea ����������ŵĴ�ŵ�ַ, ��ָ�򽹵�������ŵ�ָ��
 *           piHot ����굥�������س���ո�ʱ���ص�ǰ�������
 * �������:
 * �� �� ֵ:
 *
 * ����˵��:
 */
int DealInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    DWORD res;
    int num, arrow, re = 0;
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/

    SetHotPoint(pHotArea, *piHot);
    while (1)
    {
        ReadConsoleInput(std_in, &inRec, 1, &res);
        if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
        {
            vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
            asc = inRec.Event.KeyEvent.uChar.AsciiChar;
            if (asc == 0)
            {
                arrow = 0;
                switch (vkc)
                {  /*�����(���ϡ��ҡ���)�Ĵ���*/
                    case 37: arrow = 1; break;
                    case 38: arrow = 2; break;
                    case 39: arrow = 3; break;
                    case 40: arrow = 4; break;
                }
                if (arrow > 0)
                {
                    num = *piHot;
                    while (TRUE)
                    {
                        if (arrow < 3)
                        {
                            num--;
                        }
                        else
                        {
                            num++;
                        }
                        if ((num < 1) || (num > pHotArea->num) ||
                            ((arrow % 2) && (pHotArea->pArea[num-1].Top
                            == pHotArea->pArea[*piHot-1].Top)) || ((!(arrow % 2))
                            && (pHotArea->pArea[num-1].Top
                            != pHotArea->pArea[*piHot-1].Top)))
                        {
                            break;
                        }
                    }
                    if (num > 0 && num <= pHotArea->num)
                    {
                        *piHot = num;
                        SetHotPoint(pHotArea, *piHot);
                    }
                }
            }
            else if (vkc == 27)
            {  /*ESC��*/
                re = 27;
                break;
            }
            else if (vkc == 13)
            {  /*�س�����ʾ���µ�ǰ��ť*/
                re = 13;
                break;
            }
        }
    }
    return re;
}

/**
 * ��������: SetHotPoint
 * ��������: ��������
 * �������: pHotArea ѡ�е�����
 * �������: iHot ����������ŵĴ�ŵ�ַ, ��ָ�򽹵�������ŵ�ָ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = {0, 0};
    WORD att1, att2;
    int i, width;

    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    for (i=0; i<pHotArea->num; i++)
    {  /*����ť��������Ϊ�׵׺���*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        if (pHotArea->pSort[i] == 0)
        {  /*�����ǰ�ť��*/
            FillConsoleOutputAttribute(std_out, att2, width, pos, &ul);
        }
    }

    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 1;
    if (pHotArea->pSort[iHot-1] == 0)
    {  /*�����������ǰ�ť��*/
        FillConsoleOutputAttribute(std_out, att1, width, pos, &ul);
    }
    else if (pHotArea->pSort[iHot-1] == 1)
    {  /*�������������ı�����*/
        SetConsoleCursorPosition(std_out, pos);
        GetConsoleCursorInfo(std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(std_out, &lpCur);
    }
}

/**
 * ��������: MaintainProInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
BOOL MaintainProInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    char *pCh[] = {"¼��ʡ����Ϣ", "ɾ��ʡ����Ϣ","�޸�ʡ����Ϣ","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 10;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+3, srpop.Top+6},
                    {srpop.Left+7, srpop.Top+8}};
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{srpop.Left + 3, srpop.Top + 2,
                           srpop.Left + 14, srpop.Top + 2},
                          {srpop.Left + 3, srpop.Top + 4,
                           srpop.Left + 14, srpop.Top + 4},
                          {srpop.Left + 3, srpop.Top + 6,
                           srpop.Left + 14, srpop.Top + 6},
                          {srpop.Left + 7, srpop.Top + 8,
                           srpop.Left + 10, srpop.Top + 8}};/*�ĸ���ť������λ��*/
    char aSort[] = {0,0,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);

    pos.X = srpop.Left + 1;
    pos.Y = srpop.Top + 1;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    pos.Y = srpop.Top + 3;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    pos.Y = srpop.Top + 5;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    pos.Y = srpop.Top + 7;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);



    if (DealInput(&areas, &iHot) == 13 && (iHot == 1 || iHot == 2 || iHot == 3))
    {
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }

    PopOff();
    if (bRet)
    {
        if(iHot==1)
        {
            InputProInfo();
        }
        else if(iHot == 2)
        {
            DelProInfo();
        }
        else if(iHot == 3)
        {
            ModifyProInfo();
        }

    }
    return bRet;
}

/**
 * ��������: InputProInfo
 * ��������: ¼��ʡ���ŵĶԻ��򵯳�
 * �������:
 * �������: ��
 * �� �� ֵ: �ṹָ��
 *
 * ����˵��:
 */
void InputProInfo(void)
{
    PROVINCE_NODE* pnewnode = (PROVINCE_NODE *)malloc(sizeof(PROVINCE_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"ʡ������",
                   "        ",
                   "�ͼ츺����",
                   "               ",
                   "��ϵ�绰",
                   "                ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 10;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 8;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+14, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+14, srpop.Top+4},
                    {srpop.Left+3, srpop.Top+6},
                    {srpop.Left+14, srpop.Top+6},
                    {srpop.Left+9, srpop.Top+8},
                    {srpop.Left+18, srpop.Top+8}};
    labels.pLoc = aLoc;

    areas.num = 5;
    SMALL_RECT aArea[] = {{srpop.Left + 14, srpop.Top + 2,
                           srpop.Left + 22, srpop.Top + 2},
                          {srpop.Left + 14, srpop.Top + 4,
                           srpop.Left + 22, srpop.Top + 4},
                          {srpop.Left + 14, srpop.Top + 6,
                           srpop.Left + 26, srpop.Top + 6},
                          {srpop.Left + 9, srpop.Top + 8,
                           srpop.Left + 12, srpop.Top + 8},
                          {srpop.Left + 18, srpop.Top + 8,
                           srpop.Left + 21, srpop.Top + 8}};/*���������λ��*/
    char aSort[] = {1,1,1,0,0};
    char aTag[] = {1,2,3,4,5};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+7;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char proname[9];
    char person[16];
    char phone[17];

    char *pc1 = proname;
    char *pc2 = person;
    char *pc3 = phone;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[3],&res);
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[5],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1 || iHot==2 || iHot==3))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pc1)=c;
                }
                if(iHot==2)
                {
                    *(--pc2)=c;
                }
                if(iHot==3)
                {
                    *(--pc3)=c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3|| iHot == 4 || iHot == 5))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                if(iHot==4)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    *pc3 = '\0';
                    if(SeekProvinceNode(proname))
                    {
                        char *plabel_name[] = {"ʡ����Ϣ�Ѵ���","ȷ��"};
                        ShowModule(plabel_name,2);
                        return;
                    }
                    if(strlen(proname)!=0 && strlen(person)!=0 && strlen(phone)!=0)
                    {
                        strcpy(pnewnode->pvblg,proname);
                        strcpy(pnewnode->disciplineperson,person);
                        strcpy(pnewnode->tele,phone);
                        pnewnode->cnext = NULL;
                        pnewnode->next = NULL;
                        PopOff();
                        int re;
                        re = InsertProvinceNode(&head,pnewnode);
                        if(re)
                        {
                            char *plabel_name[] = {"ʡ����Ϣ¼��ɹ���","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(pc1)==0 || strlen(pc2)==0 || strlen(pc3)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }

                }
                else if(iHot==5)
                {
                    PopOff();
                    return;
                }
                else if(iHot==1 || iHot==2 || iHot==3)
                {
                    PopOff();
                    char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                    ShowModule(plabel_name, 2);
                    PopOff();
                    return;
                }

            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 5;
                            }
                            SetHotPoint(&areas,iHot);

                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>5)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==4 || iHot==5)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+7) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc1++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
            else if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+14) && (iHot==2))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
            else if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+15) && (iHot==3))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc3++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

/**
 * ��������: InsertProvinceNode
 * ��������: ��ʮ�������в���һ��ʡ����Ϣ���.
 * �������: head ����ͷָ��
 *           pprovince_node ָ����Ҫ�������ָ��
 * �������: ��
 * �� �� ֵ: int����, 1��ʾ����ɹ�, 0��ʾ����ʧ��
 *
 * ����˵��:
 */
int InsertProvinceNode(PROVINCE_NODE **head,PROVINCE_NODE *pprovincenode)
{
    pprovincenode->next = *head;
    *head = pprovincenode;
    return 1;
}

/**
 * ��������: DelProInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void DelProInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"�������ɾ��ʡ������",
                   "        ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+8, srpop.Top+4},
                    {srpop.Left+7, srpop.Top+6},
                    {srpop.Left+15, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{srpop.Left + 8, srpop.Top + 4,
                           srpop.Left + 17, srpop.Top + 4},
                          {srpop.Left + 7, srpop.Top + 6,
                           srpop.Left + 10, srpop.Top + 6},
                          {srpop.Left + 15, srpop.Top + 6,
                           srpop.Left + 18, srpop.Top + 6}};/*���������λ��*/
    char aSort[] = {1,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char proname[8];
    char *pro = proname;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[1],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1))
            {
                //pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                *(--pro) = c;
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==2)
                {
                    *(pro) = '\0';
                    if(strlen(proname)!=0)
                    {
                        PopOff();
                        if(SeekProvinceNode(proname))
                        {
                            ;
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴�ʡ��","ȷ��"};
                            ShowModule(plabel_name,2);
                            return;
                        }
                        int re = DeleteProvinceNode(proname);
                        if(re)
                        {
                            PopOff();
                            char *plabel_name[] = {"ɾ���ɹ���","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            PopOff();
                            char *plabel_name[] = {"�޴�ʡ�ݣ�","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(proname) == 0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return ;
                    }

                }
                else if(iHot==3)
                {
                    PopOff();
                    return ;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 3;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>3)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==2 || iHot==3)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+7) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pro++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * ��������: DeleteProvinceNode
 * ��������: ��ʮ��������ɾ��һ��ʡ����Ϣ���.
 * �������: head ����ͷָ��
 *           pvblg ����ʡ��
 * �������: ��
 * �� �� ֵ: int����, 1��ʾɾ���ɹ�, 0��ʾɾ��ʧ��
 *
 * ����˵��:
 */
int DeleteProvinceNode(char *pvblg)
{
    PROVINCE_NODE *p1 = head,*p2;
    p2 = SeekProvinceNode(pvblg);
    if(p2 == head)
    {
        head = head->next;
        free(p2);
        return 1;
    }
    else
    {
        for(;p1 != NULL;p1 = p1->next)
        {
            if(p1->next->pvblg == p2->pvblg)
            {
                p1->next = p2->next;
                free(p2);
                return 1;
            }
        }
    }
    return 0;

}

/**
 * ��������: ModifyProInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ:
 *
 * ����˵��:
 */
void ModifyProInfo(void)
{

    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"��������޸�ʡ������",
                   "          ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+8, srpop.Top+4},
                    {srpop.Left+7, srpop.Top+6},
                    {srpop.Left+15, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{srpop.Left + 8, srpop.Top + 4,
                           srpop.Left + 17, srpop.Top + 4},
                          {srpop.Left + 7, srpop.Top + 6,
                           srpop.Left + 10, srpop.Top + 6},
                          {srpop.Left + 15, srpop.Top + 6,
                           srpop.Left + 18, srpop.Top + 6}};/*���������λ��*/
    char aSort[] = {1,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char proname[8];
    char *pro = proname;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[1],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1))
            {
                //pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                *(--pro) = c;
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                if(iHot==2)
                {
                    *(pro) = '\0';
                    if(SeekProvinceNode(proname))
                    {
                        ;
                    }
                    else
                    {
                        char *plabel_name[] = {"�޴�ʡ��","ȷ��"};
                        ShowModule(plabel_name,2);
                        return;
                    }
                    if(strlen(proname)!=0)
                    {
                        PopOff();
                        InputModifyProInfo();
                        return;
                    }
                    else if(strlen(proname)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }
                }
                else if(iHot==3)
                {
                    PopOff();
                    return;
                }
            }

            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 3;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>3)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==2 || iHot==3)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+7) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pro++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * ��������: InputModifyProInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void InputModifyProInfo(void)
{
    PROVINCE_NODE *pnewnode = (PROVINCE_NODE *)malloc(sizeof(PROVINCE_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"ʡ������",
                   "        ",
                   "�ͼ츺����",
                   "               ",
                   "��ϵ�绰",
                   "                ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 10;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 8;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+14, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+14, srpop.Top+4},
                    {srpop.Left+3, srpop.Top+6},
                    {srpop.Left+14, srpop.Top+6},
                    {srpop.Left+9, srpop.Top+8},
                    {srpop.Left+18, srpop.Top+8}};
    labels.pLoc = aLoc;

    areas.num = 5;
    SMALL_RECT aArea[] = {{srpop.Left + 14, srpop.Top + 2,
                           srpop.Left + 22, srpop.Top + 2},
                          {srpop.Left + 14, srpop.Top + 4,
                           srpop.Left + 22, srpop.Top + 4},
                          {srpop.Left + 14, srpop.Top + 6,
                           srpop.Left + 26, srpop.Top + 6},
                          {srpop.Left + 9, srpop.Top + 8,
                           srpop.Left + 12, srpop.Top + 8},
                          {srpop.Left + 18, srpop.Top + 8,
                           srpop.Left + 21, srpop.Top + 8}};/*���������λ��*/
    char aSort[] = {1,1,1,0,0};
    char aTag[] = {1,2,3,4,5};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+7;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char proname[8];
    char person[15];
    char phone[16];

    char *pc1 = proname;
    char *pc2 = person;
    char *pc3 = phone;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[3],&res);
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[5],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1 || iHot==2 || iHot==3))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pc1)=c;
                }
                if(iHot==2)
                {
                    *(--pc2)=c;
                }
                if(iHot==3)
                {
                    *(--pc3)=c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3|| iHot == 4 || iHot == 5))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==4)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    *pc3 = '\0';
                    if(strlen(proname)!=0 && strlen(person)!=0 && strlen(phone)!=0)
                    {
                        strcpy(pnewnode->pvblg,proname);
                        strcpy(pnewnode->disciplineperson,person);
                        strcpy(pnewnode->tele,phone);
                        pnewnode->cnext = NULL;
                        pnewnode->next = NULL;
                        PopOff();
                        int re = ModifyProvinceNode(pnewnode,proname);
                        if(re)
                        {
                            char *plabel_name[] = {"�޸ĳɹ���","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴�ʡ�ݣ�","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(pc1)==0 || strlen(pc2)==0 || strlen(pc3)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }

                }
                else if(iHot==5)
                {
                    PopOff();
                    return;
                }
                else if(iHot==1 || iHot==2 || iHot==3)
                {
                    PopOff();
                    char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                    ShowModule(plabel_name, 2);
                    PopOff();
                    return;
                }

            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 5;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>5)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==4 || iHot==5)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+7) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc1++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
            else if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+14) && (iHot==2))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
            else if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+15) && (iHot==3))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc3++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

 /**
 * ��������: ModifyProvinceNode
 * ��������: ��ָ����ʡ����Ϣ������ݽ����޸�.
 * �������: head ����ͷָ��
 *           pvblg ����ʡ��
 *           pprovincenode ָ�����޸����ݽ���ָ��
 * �������: ��
 * �� �� ֵ: int����, 1��ʾ�޸ĳɹ�, 0��ʾ�޸�ʧ��
 *
 * ����˵��:
 */
int ModifyProvinceNode(PROVINCE_NODE *pprovincenode,char *pvblg)
{
    PROVINCE_NODE *provincenode = SeekProvinceNode(pvblg);
    PROVINCE_NODE *ptmp;
    if(provincenode != NULL)
    {
        ptmp = provincenode->next;
        *provincenode = *pprovincenode;
        provincenode->next = ptmp;
        return 1;
    }
    return 0;
}

/**
 * ��������: MaintainCaseInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
BOOL MaintainCaseInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    char *pCh[] = {"¼��̰��������Ϣ", "ɾ��̰��������Ϣ","�޸�̰��������Ϣ","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 10;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+3, srpop.Top+6},
                    {srpop.Left+9, srpop.Top+8}};
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{srpop.Left + 3, srpop.Top + 2,
                           srpop.Left + 18, srpop.Top + 2},
                          {srpop.Left + 3, srpop.Top + 4,
                           srpop.Left + 18, srpop.Top + 4},
                          {srpop.Left + 3, srpop.Top + 6,
                           srpop.Left + 18, srpop.Top + 6},
                          {srpop.Left + 9, srpop.Top + 8,
                           srpop.Left + 12, srpop.Top + 8}};/*�ĸ���ť������λ��*/
    char aSort[] = {0,0,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);

    pos.X = srpop.Left + 1;
    pos.Y = srpop.Top + 1;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    pos.Y = srpop.Top + 3;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    pos.Y = srpop.Top + 5;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    pos.Y = srpop.Top + 7;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);

    if (DealInput(&areas, &iHot) == 13 && (iHot == 1 || iHot == 2 || iHot == 3))
    {
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }

    PopOff();
    if (bRet)
    {
        if(iHot==1)
        {
            InputCaseInfo();
        }
        else if(iHot == 2)
        {
            DelCaseInfo();
        }
        else if(iHot == 3)
        {
            ModifyCaseInfo();
        }

    }
    return bRet;
}

/**
 * ��������: InputCaseInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void InputCaseInfo(void)
{
    CASE_NODE *pnewnode = (CASE_NODE *)malloc(sizeof(CASE_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"�������",  "               ",
                   "��������",  "               ",
                   "����ʡ��",  "               ",
                   "����������","               ",
                   "�永���",  "               ",
                   "��������",  "               ",
                   "('A'-'H'���ֱ�����Ƽ������Ƽ�����������",
                   "��������������������������������������������)",

                   "��Ҫ����",  "               ",
                   "('1'-'6'���ֱ�����ܻߡ�̰�ۡ�",
                   "Ų�ù����ְ������ְȨ������)",

                   "���´���",  "               ",
                   "(1~99������ͽ��  ���ڵ���100������ͽ��  С��0������)",

                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 42;
    pos.Y = 25;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 23;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3,  srpop.Top+2},
                    {srpop.Left+14, srpop.Top+2},

                    {srpop.Left+3,  srpop.Top+4},
                    {srpop.Left+14, srpop.Top+4},

                    {srpop.Left+3,  srpop.Top+6},
                    {srpop.Left+14, srpop.Top+6},

                    {srpop.Left+3,  srpop.Top+8},
                    {srpop.Left+14, srpop.Top+8},

                    {srpop.Left+3, srpop.Top+10},
                    {srpop.Left+14, srpop.Top+10},

                    {srpop.Left+3, srpop.Top+12},
                    {srpop.Left+14, srpop.Top+12},
                    {srpop.Left+3, srpop.Top+13},
                    {srpop.Left+3, srpop.Top+14},

                    {srpop.Left+3, srpop.Top+16},
                    {srpop.Left+14, srpop.Top+16},
                    {srpop.Left+3, srpop.Top+17},
                    {srpop.Left+3,srpop.Top+18},

                    {srpop.Left+3, srpop.Top+20},
                    {srpop.Left+14,srpop.Top+20},
                    {srpop.Left+3,srpop.Top+21},

                    {srpop.Left+15, srpop.Top+23},
                    {srpop.Left+23,srpop.Top+23},

                    };
    labels.pLoc = aLoc;

    areas.num = 10;
    SMALL_RECT aArea[] = {{srpop.Left + 14, srpop.Top + 2,
                           srpop.Left + 29, srpop.Top + 2},

                          {srpop.Left + 14, srpop.Top + 4,
                           srpop.Left + 29, srpop.Top + 4},

                          {srpop.Left + 14, srpop.Top + 6,
                           srpop.Left + 29, srpop.Top + 6},

                          {srpop.Left + 14, srpop.Top + 8,
                           srpop.Left + 29, srpop.Top + 8},

                          {srpop.Left + 14, srpop.Top + 10,
                           srpop.Left + 29, srpop.Top + 10},

                          {srpop.Left + 14, srpop.Top + 12,
                           srpop.Left + 29, srpop.Top + 12},

                          {srpop.Left + 14, srpop.Top + 16,
                           srpop.Left + 29, srpop.Top + 16},

                          {srpop.Left + 14, srpop.Top + 20,
                           srpop.Left + 29, srpop.Top + 20},

                          {srpop.Left + 15, srpop.Top + 23,
                           srpop.Left + 18, srpop.Top + 23},
                          {srpop.Left + 23, srpop.Top + 23,
                           srpop.Left + 26, srpop.Top + 23},
                           };/*ʮ��������λ��*/
    char aSort[] = {1,1,1,1,1,1,1,1,0,0};
    char aTag[] = {1,2,3,4,5,6,7,8,9,10};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+22;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char casenum[13];           /*�������*/
    char casedate[11];          /*��������*/
    char pvblg[9];              /*����ʡ��*/
    char crimename[14];         /*����������*/
    char charmoney[15];         /*�永���*/
    char level;                 /*��������*/
    char accusationname;        /*��Ҫ����*/
    char punishment[4];         /*���´���*/

    char *pc1 = casenum;
    char *pc2 = casedate;
    char *pc3 = pvblg;
    char *pc4 = crimename;
    char *pc5 = charmoney;
    char *pc6 = punishment;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[3],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[5],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[7],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[9],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[11],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[15],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[19],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
	    //DealInput(&areas,&iHot);
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown)
            && (iHot==1 || iHot==2 || iHot==3 || iHot==4 || iHot==5 || iHot==6 || iHot==7 || iHot==8))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pc1) = c;
                }
                else if(iHot==2)
                {
                    *(--pc2) = c;
                }
                else if(iHot==3)
                {
                    *(--pc3) = c;
                }
                else if(iHot==4)
                {
                    *(--pc4) = c;
                }
                else if(iHot==5)
                {
                    *(--pc5) = c;
                }
                else if(iHot==6)
                {
                    level = c;
                }
                else if(iHot==7)
                {
                    accusationname = c;
                }
                else if(iHot==8)
                {
                    *(--pc6) = c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot==1 || iHot==2 || iHot==3 || iHot==4 || iHot==5 ||
                      iHot==6 || iHot==7 || iHot==8 || iHot==9 || iHot==10))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==9)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    *pc3 = '\0';
                    *pc4 = '\0';
                    *pc5 = '\0';
                    *pc6 = '\0';
                    if(strlen(casedate)!=0 && strlen(casenum)!=0 && strlen(pvblg)!=0
                        && strlen(crimename)!=0 && strlen(charmoney)!=0 && strlen(punishment)!=0)
                    {
                        float power = 1.0,value;
                        int i=0;
                        for(value=0.0;charmoney[i]>='0'&&charmoney[i]<='9';i++)//����С����ǰ������
                        {
                            value=value*10.0+(charmoney[i]-'0');
                        }

                        if(charmoney[i]=='.')
                        {
                            i++;
                        }
                        for(power=1.0;charmoney[i]>='0'&&charmoney[i]<='9';i++)//����С����������
                        {
                            value=value*10.0+(charmoney[i]-'0');
                            power*=10.0;
                        }
                        int punish = atoi(punishment);
                        strcpy(pnewnode->casedate,casedate);
                        strcpy(pnewnode->casenum,casenum);
                        strcpy(pnewnode->pvblg,pvblg);
                        strcpy(pnewnode->crimename,crimename);
                        pnewnode->accusationname = accusationname;
                        pnewnode->level = level;
                        pnewnode->money = value/power;
                        pnewnode->punishment = punish;
                        pnewnode->mnext = NULL;
                        pnewnode->next = NULL;
                        PopOff();

                        int re = InsertCaseNode(pvblg,pnewnode);
                        if(re)
                        {
                            char *plabel_name[] = {"������Ϣ¼��ɹ���","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"������Ϣ¼��ʧ�ܣ�","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }

                        PopOff();
                        return;
                    }
                    else if(strlen(casedate)==0 || strlen(casenum)==0 ||
                            strlen(pvblg)==0 || strlen(crimename)==0 || strlen(charmoney)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }
                }
                else if(iHot==10)
                {
                    PopOff();
                    return;
                }
            }

            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 10;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>10)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==9 || iHot==10)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) && (iHot<9) && (pos.X<=aArea[0].Left+15))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    if(iHot == 1)
                    {
                        *(pc1++) = ch;
                    }
                    else if(iHot == 2)
                    {
                        *(pc2++) = ch;
                    }
                    else if(iHot == 3)
                    {
                        *(pc3++) = ch;
                    }
                    else if(iHot == 4)
                    {
                        *(pc4++) = ch;
                    }
                    else if(iHot == 5)
                    {
                        *(pc5++) = ch;
                    }
                    else if(iHot == 6)
                    {
                        level = ch;
                    }
                    else if(iHot == 7)
                    {
                        accusationname = ch;
                    }
                    else if(iHot == 8)
                    {
                        *(pc6++) = ch;
                    }
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

/**
 * ��������: InsertCaseNode
 * ��������: ��ʮ�������в���һ��������Ϣ���.
 * �������: head ����ͷָ��
 *           pvblg ����ʡ��
 *           pcasenode ָ����Ҫ�������ָ��
 * �������: ��
 * �� �� ֵ: int����, 1��ʾ����ɹ�, 0��ʾ����ʧ��
 *
 * ����˵��:
 */
int InsertCaseNode(char *pvblg,CASE_NODE *pcasenode)
{
    PROVINCE_NODE *provincenode = SeekProvinceNode(pvblg);
    if(provincenode)
    {
        pcasenode->next = provincenode->cnext;
        provincenode->cnext = pcasenode;
        return 1;
    }
    return 0;
}

 /**
 * ��������: DelCaseInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void DelCaseInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"������ʡ������",
                   "        ",
                   "�����밸�����",
                   "            ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 6;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+18, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+18, srpop.Top+4},
                    {srpop.Left+9, srpop.Top+6},
                    {srpop.Left+18, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{srpop.Left + 18, srpop.Top + 2,
                           srpop.Left + 26, srpop.Top + 2},
                          {srpop.Left + 18, srpop.Top + 4,
                           srpop.Left + 30, srpop.Top + 4},
                          {srpop.Left + 9, srpop.Top + 6,
                           srpop.Left + 12, srpop.Top + 6},
                          {srpop.Left + 18, srpop.Top + 6,
                           srpop.Left + 21, srpop.Top + 6}};/*�ĸ�������λ��*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char proname[9];
    char casenum[13];

    char *pc1 = proname;
    char *pc2 = casenum;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,8,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1 || iHot==2))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pc1)=c;
                }
                if(iHot==2)
                {
                    *(--pc2)=c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3|| iHot == 4))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==3)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    if(strlen(proname)!=0 && strlen(casenum)!=0)
                    {
                        PopOff();
                        if(SeekProvinceNode(proname))
                        {
                            ;
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴�ʡ��","ȷ��"};
                            ShowModule(plabel_name,2);
                            return;
                        }
                        int re = DeleteCaseNode(casenum,proname);
                        if(re)
                        {
                            char *plabel_name[] = {"������Ϣɾ���ɹ���","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴˰�����Ϣ��","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }

                        PopOff();
                        return;
                    }
                    else if(strlen(proname)==0 || strlen(casenum)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }

                }
                else if(iHot==4)
                {
                    PopOff();
                    return;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 4;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>4)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==3 || iHot==4)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+7) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc1++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
			}
            else if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+9) && (iHot==2))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

/**
 * ��������: DeleteCaseNode
 * ��������: ��ʮ��������ɾ��һ��������Ϣ���.
 * �������: head ����ͷָ��
 *           casenum �������
             pvblg ����ʡ��
 * �������: ��
 * �� �� ֵ: int����, 1��ʾɾ���ɹ�, 0��ʾɾ��ʧ��
 *
 * ����˵��:
 */
int DeleteCaseNode(char *casenum,char *pvblg)
{
    int flag = 0;
    PROVINCE_NODE *provincenode = SeekProvinceNode(pvblg);
    CASE_NODE *p1 = NULL,*p2 = provincenode->cnext;
    for(;p2 != NULL;p2 = p2->next)
    {
        if(strcmp(p2->casenum,casenum) == 0)
        {
            flag = 1;
            break;
        }
        p1 = p2;
    }
    if(flag)
    {
        if(p1 == NULL)                          /*ɾ���İ�����Ϣ�ڵ��ڵ�һ��*/
            provincenode->cnext = p2->next;
        else
            p1->next = p2->next;
        free(p2);
        return 1;
    }
    return 0;
}

 /**
 * ��������: ModifyCaseInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void ModifyCaseInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"������ʡ������",
                   "        ",
                   "�����밸�����",
                   "            ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 6;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+18, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+18, srpop.Top+4},
                    {srpop.Left+9, srpop.Top+6},
                    {srpop.Left+18, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 4;
    //aAreaҪ�͵��µ�FillConsoleOutputAttribute��ƥ��
    SMALL_RECT aArea[] = {{srpop.Left + 18, srpop.Top + 2,
                           srpop.Left + 26, srpop.Top + 2},
                          {srpop.Left + 18, srpop.Top + 4,
                           srpop.Left + 30, srpop.Top + 4},
                          {srpop.Left + 9, srpop.Top + 6,
                           srpop.Left + 12, srpop.Top + 6},
                          {srpop.Left + 18, srpop.Top + 6,
                           srpop.Left + 21, srpop.Top + 6}};/*�ĸ�������λ��*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char proname[9];
    char casenum[13];

    char *pc1 = proname;
    char *pc2 = casenum;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,8,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
	    //DealInput(&areas,&iHot);
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1 || iHot==2))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pc1)=c;
                }
                if(iHot==2)
                {
                    *(--pc2)=c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3|| iHot == 4))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==3)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    if(strlen(proname)!=0 && strlen(casenum)!=0)
                    {
                        PopOff();
                        if(!SeekProvinceNode(proname))
                        {
                            char *plabel_name[] = {"�޴�ʡ��","ȷ��"};
                            ShowModule(plabel_name,2);
                            return;
                        }
                        InputModifyCaseInfo();//ת���޸ĺ���
                        return;
                    }
                    else if(strlen(proname)==0 || strlen(casenum)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }

                }
                else if(iHot==4)
                {
                    PopOff();
                    return;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 4;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>4)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==3 || iHot==4)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+7) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc1++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
            else if((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+11) && (iHot==2))
            {
                ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

/**
 * ��������: InputModifyCaseInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
void InputModifyCaseInfo(void)
{
    CASE_NODE *pnewnode = (CASE_NODE *)malloc(sizeof(CASE_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"�������",  "               ",
                   "��������",  "               ",
                   "����ʡ��",  "               ",
                   "����������","               ",
                   "�永���",  "               ",
                   "��������",  "               ",
                   "('A'-'H'���ֱ�����Ƽ������Ƽ�����������",
                   "��������������������������������������������)",

                   "��Ҫ����",  "               ",
                   "('1'-'6'���ֱ�����ܻߡ�̰�ۡ�",
                   "Ų�ù����ְ������ְȨ������)",

                   "���´���",  "               ",
                   "(1~99������ͽ��  ���ڵ���100������ͽ��  С��0������)",

                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 42;
    pos.Y = 25;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 23;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3,  srpop.Top+2},
                    {srpop.Left+14, srpop.Top+2},

                    {srpop.Left+3,  srpop.Top+4},
                    {srpop.Left+14, srpop.Top+4},

                    {srpop.Left+3,  srpop.Top+6},
                    {srpop.Left+14, srpop.Top+6},

                    {srpop.Left+3,  srpop.Top+8},
                    {srpop.Left+14, srpop.Top+8},

                    {srpop.Left+3, srpop.Top+10},
                    {srpop.Left+14, srpop.Top+10},

                    {srpop.Left+3, srpop.Top+12},
                    {srpop.Left+14, srpop.Top+12},
                    {srpop.Left+3, srpop.Top+13},
                    {srpop.Left+3, srpop.Top+14},

                    {srpop.Left+3, srpop.Top+16},
                    {srpop.Left+14, srpop.Top+16},
                    {srpop.Left+3, srpop.Top+17},
                    {srpop.Left+3,srpop.Top+18},

                    {srpop.Left+3, srpop.Top+20},
                    {srpop.Left+14,srpop.Top+20},
                    {srpop.Left+3,srpop.Top+21},

                    {srpop.Left+15, srpop.Top+23},
                    {srpop.Left+23,srpop.Top+23},

                    };
    labels.pLoc = aLoc;

    areas.num = 10;
    SMALL_RECT aArea[] = {{srpop.Left + 14, srpop.Top + 2,
                           srpop.Left + 29, srpop.Top + 2},

                          {srpop.Left + 14, srpop.Top + 4,
                           srpop.Left + 29, srpop.Top + 4},

                          {srpop.Left + 14, srpop.Top + 6,
                           srpop.Left + 29, srpop.Top + 6},

                          {srpop.Left + 14, srpop.Top + 8,
                           srpop.Left + 29, srpop.Top + 8},

                          {srpop.Left + 14, srpop.Top + 10,
                           srpop.Left + 29, srpop.Top + 10},

                          {srpop.Left + 14, srpop.Top + 12,
                           srpop.Left + 29, srpop.Top + 12},

                          {srpop.Left + 14, srpop.Top + 16,
                           srpop.Left + 29, srpop.Top + 16},

                          {srpop.Left + 14, srpop.Top + 20,
                           srpop.Left + 29, srpop.Top + 20},

                          {srpop.Left + 15, srpop.Top + 23,
                           srpop.Left + 18, srpop.Top + 23},
                          {srpop.Left + 23, srpop.Top + 23,
                           srpop.Left + 26, srpop.Top + 23},
                           };/*ʮ��������λ��*/
    char aSort[] = {1,1,1,1,1,1,1,1,0,0};
    char aTag[] = {1,2,3,4,5,6,7,8,9,10};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+22;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char casenum[13];           /*�������*/
    char casedate[11];          /*��������*/
    char pvblg[9];              /*����ʡ��*/
    char crimename[14];         /*����������*/
    char charmoney[15];         /*�永���*/
    char level;                 /*��������*/
    char accusationname;        /*��Ҫ����*/
    char punishment[4];         /*���´���*/

    char *pc1 = casenum;
    char *pc2 = casedate;
    char *pc3 = pvblg;
    char *pc4 = crimename;
    char *pc5 = charmoney;
    char *pc6 = punishment;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[3],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[5],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[7],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[9],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[11],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[15],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[19],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
	    //DealInput(&areas,&iHot);
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown)
            && (iHot==1 || iHot==2 || iHot==3 || iHot==4 || iHot==5 || iHot==6 || iHot==7 || iHot==8))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pc1) = c;
                }
                else if(iHot==2)
                {
                    *(--pc2) = c;
                }
                else if(iHot==3)
                {
                    *(--pc3) = c;
                }
                else if(iHot==4)
                {
                    *(--pc4) = c;
                }
                else if(iHot==5)
                {
                    *(--pc5) = c;
                }
                else if(iHot==6)
                {
                    level = c;
                }
                else if(iHot==7)
                {
                    accusationname = c;
                }
                else if(iHot==8)
                {
                    *(--pc6) = c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot==1 || iHot==2 || iHot==3 || iHot==4 || iHot==5 || iHot==6 || iHot==7 || iHot==8 || iHot==9 || iHot==10))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==9)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    *pc3 = '\0';
                    *pc4 = '\0';
                    *pc5 = '\0';
                    *pc6 = '\0';
                    if(strlen(casedate)!=0 && strlen(casenum)!=0 && strlen(pvblg)!=0
                        && strlen(crimename)!=0 && strlen(charmoney)!=0 && strlen(punishment)!=0)
                    {
                        float power = 1.0,value;
                        int i=0;
                        for(value=0.0;charmoney[i]>='0'&&charmoney[i]<='9';i++)//����С����ǰ������
                        {
                            value=value*10.0+(charmoney[i]-'0');
                        }

                        if(charmoney[i]=='.')
                        {
                            i++;
                        }
                        for(power=1.0;charmoney[i]>='0'&&charmoney[i]<='9';i++)//����С����������
                        {
                            value=value*10.0+(charmoney[i]-'0');
                            power*=10.0;
                        }
                        int punish = atoi(punishment);
                        strcpy(pnewnode->casedate,casedate);
                        strcpy(pnewnode->casenum,casenum);
                        strcpy(pnewnode->pvblg,pvblg);
                        strcpy(pnewnode->crimename,crimename);
                        pnewnode->accusationname = accusationname;
                        pnewnode->level = level;
                        pnewnode->money = value/power;
                        pnewnode->punishment = punish;
                        pnewnode->mnext = NULL;
                        pnewnode->next = NULL;
                        PopOff();
                        int re = ModifyCaseNode(pnewnode,casenum);
                        if(re)
                        {
                            char *plabel_name[] = {"������Ϣ�޸ĳɹ���","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"������Ϣ�޸�ʧ�ܣ�","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }

                        PopOff();
                        return;
                    }
                    else if(strlen(casedate)==0 || strlen(casenum)==0 ||
                            strlen(pvblg)==0 || strlen(crimename)==0 || strlen(charmoney)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }
                }
                else if(iHot==10)
                {
                    PopOff();
                    return;
                }
            }

            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 10;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>10)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==9 || iHot==10)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) && (iHot<9) && (pos.X<=aArea[0].Left+15))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    if(iHot == 1)
                    {
                        *(pc1++) = ch;
                    }
                    else if(iHot == 2)
                    {
                        *(pc2++) = ch;
                    }
                    else if(iHot == 3)
                    {
                        *(pc3++) = ch;
                    }
                    else if(iHot == 4)
                    {
                        *(pc4++) = ch;
                    }
                    else if(iHot == 5)
                    {
                        *(pc5++) = ch;
                    }
                    else if(iHot == 6)
                    {
                        level = ch;
                    }
                    else if(iHot == 7)
                    {
                        accusationname = ch;
                    }
                    else if(iHot == 8)
                    {
                        *(pc6++) = ch;
                    }
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

/**
 * ��������: ModifyCaseNode
 * ��������: ��ָ���İ�����Ϣ������ݽ����޸�.
 * �������: head ����ͷָ��
 *           casenum �������
 *           pcasenode ָ�����޸����ݽ���ָ��
 * �������: ��
 * �� �� ֵ: int����, 1��ʾ�޸ĳɹ�, 0��ʾ�޸�ʧ��
 *
 * ����˵��:
 */
int ModifyCaseNode(CASE_NODE *pcasenode,char *casenum)
{
    CASE_NODE *casenode = SeekCaseNode(casenum),*ptmp;
    if(casenode)
    {
        ptmp = casenode->next;
        *casenode = *pcasenode;
        casenode->next = ptmp;
        return 1;
    }
    return 0;
}

/**
 * ��������: MaintainMediaInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
BOOL MaintainMediaInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    char *pCh[] = {"¼��ý�屨����Ϣ", "ɾ��ý�屨����Ϣ","�޸�ý�屨����Ϣ","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 10;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+3, srpop.Top+6},
                    {srpop.Left+9, srpop.Top+8}};
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{srpop.Left + 3, srpop.Top + 2,
                           srpop.Left + 18, srpop.Top + 2},
                          {srpop.Left + 3, srpop.Top + 4,
                           srpop.Left + 18, srpop.Top + 4},
                          {srpop.Left + 3, srpop.Top + 6,
                           srpop.Left + 18, srpop.Top + 6},
                          {srpop.Left + 9, srpop.Top + 8,
                           srpop.Left + 12, srpop.Top + 8}};/*�ĸ���ť������λ��*/
    char aSort[] = {0,0,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);

    pos.X = srpop.Left + 1;
    pos.Y = srpop.Top + 1;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    pos.Y = srpop.Top + 3;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    pos.Y = srpop.Top + 5;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    pos.Y = srpop.Top + 7;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);

    if (DealInput(&areas, &iHot) == 13)
    {
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    PopOff();
    if(bRet)
    {
        if(iHot==1)
        {
            InputMediaInfo();
        }
        else if(iHot==2)
        {
            DelMediaInfo();
        }
        else if(iHot==3)
        {
            ModifyMediaInfo();
        }
    }
    return bRet;
}

/**
 * ��������: InputMediaInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void InputMediaInfo(void)
{
    MEDIA_NODE *pnewnode = (MEDIA_NODE *)malloc(sizeof(MEDIA_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"����ʡ��","        ",
                   "�������","            ",
                   "��������","          ",
                   "ý������","                    ",
                   "��������","                                                   ",
                   "ý����� '1' '2' '3' '4'�ֱ����ֽ���㲥�����Ӻͻ�����"," ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 57;
    pos.Y = 16;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 14;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3,  srpop.Top+2},
                    {srpop.Left+12, srpop.Top+2},
                    {srpop.Left+3,  srpop.Top+4},
                    {srpop.Left+12, srpop.Top+4},
                    {srpop.Left+3,  srpop.Top+6},
                    {srpop.Left+12, srpop.Top+6},
                    {srpop.Left+3,  srpop.Top+8},
                    {srpop.Left+12, srpop.Top+8},
                    {srpop.Left+3, srpop.Top+10},
                    {srpop.Left+12, srpop.Top+10},
                    {srpop.Left+3, srpop.Top+12},
                    {srpop.Left+60, srpop.Top+12},
                    {srpop.Left+24, srpop.Top+14},
                    {srpop.Left+31,srpop.Top+14},
                    };
    labels.pLoc = aLoc;

    areas.num = 8;
    SMALL_RECT aArea[] = {{srpop.Left + 12, srpop.Top + 2,
                           srpop.Left + 20, srpop.Top + 2},
                          {srpop.Left + 12, srpop.Top + 4,
                           srpop.Left + 24, srpop.Top + 4},
                          {srpop.Left + 12, srpop.Top + 6,
                           srpop.Left + 22, srpop.Top + 6},
                          {srpop.Left + 12, srpop.Top + 8,
                           srpop.Left + 32, srpop.Top + 8},
                          {srpop.Left + 12, srpop.Top + 10,
                           srpop.Left + 62, srpop.Top + 10},

                          {srpop.Left + 60, srpop.Top + 12,
                           srpop.Left + 61, srpop.Top + 12},

                          {srpop.Left + 24, srpop.Top + 14,
                           srpop.Left + 27, srpop.Top + 14},
                          {srpop.Left + 31, srpop.Top + 14,
                           srpop.Left + 34, srpop.Top + 14},
                           };/*�˸�������λ��*/
    char aSort[] = {1,1,1,1,1,1,0,0};
    char aTag[] = {1,2,3,4,5,6,7,8};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+13;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char pvblg[9];              /*����ʡ��*/
    char casenum[13];           /*�������*/
    char reportdate[11];        /*��������*/
    char medianame[21];         /*ý������*/
    char contentindex[51];      /*��������*/
    char mediatype;             /*ý�����*/

    char *pc1 = pvblg;
    char *pc2 = casenum;
    char *pc3 = reportdate;
    char *pc4 = medianame;
    char *pc5 = contentindex;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,8,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[3],&res);
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[5],&res);
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[7],&res);
	FillConsoleOutputAttribute(std_out,att2,50,aLoc[9],&res);
	FillConsoleOutputAttribute(std_out,att2,1,aLoc[11],&res);

	SetHotPoint(&areas,iHot);
	while(1)
	{
	    //DealInput(&areas,&iHot);
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown)
            && (iHot==1 || iHot==2 || iHot==3 || iHot==4 || iHot==5 || iHot==6))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pc1) = c;
                }
                else if(iHot==2)
                {
                    *(--pc2) = c;
                }
                else if(iHot==3)
                {
                    *(--pc3) = c;
                }
                else if(iHot==4)
                {
                    *(--pc4) = c;
                }
                else if(iHot==5)
                {
                    *(--pc5) = c;
                }
                else if(iHot==6)
                {
                    mediatype = c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot==1 || iHot==2 || iHot==3 || iHot==4 || iHot==5 || iHot==6 || iHot==7 || iHot==8))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==7)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    *pc3 = '\0';
                    *pc4 = '\0';
                    *pc5 = '\0';
                    if(strlen(reportdate)!=0 && strlen(casenum)!=0 && strlen(pvblg)!=0)
                    {
                        strcpy(pnewnode->reportdate,reportdate);
                        strcpy(pnewnode->casenum,casenum);
                        strcpy(pnewnode->pvblg,pvblg);
                        strcpy(pnewnode->medianame,medianame);
                        strcpy(pnewnode->contentindex,contentindex);

                        pnewnode->mediatype = mediatype;
                        pnewnode->next = NULL;
                        PopOff();
                        int re = InsertMediaNode(pnewnode);
                        if(re)
                        {
                            char *plabel_name[] = {"ý����Ϣ¼��ɹ���","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"ý����Ϣ¼��ʧ�ܣ�","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }

                        PopOff();
                        return;
                    }
                    else if(strlen(reportdate)==0 || strlen(casenum)==0 ||
                            strlen(pvblg)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }
                }
                else if(iHot==8)
                {
                    PopOff();
                    return;
                }
            }

            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 8;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>8)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==7 || iHot==8)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }

            if ((keyRec.Event.KeyEvent.bKeyDown) && (iHot<7) && (pos.X<=aArea[0].Left+49))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    if(iHot == 1)
                    {
                        *(pc1++) = ch;
                    }
                    else if(iHot == 2)
                    {
                        *(pc2++) = ch;
                    }
                    else if(iHot == 3)
                    {
                        *(pc3++) = ch;
                    }
                    else if(iHot == 4)
                    {
                        *(pc4++) = ch;
                    }
                    else if(iHot == 5)
                    {
                        *(pc5++) = ch;
                    }
                    else if(iHot == 6)
                    {
                        mediatype = ch;
                    }
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }

}

/**
 * ��������: InsertMediaNode
 * ��������: ��ʮ�������в���һ��ý����Ϣ���.
 * �������: head ����ͷָ��
 *           pmedianode ָ����Ҫ�������ָ��
 * �������: ��
 * �� �� ֵ: int����, 1��ʾ����ɹ�, 0��ʾ����ʧ��
 *
 * ����˵��:
 */
int InsertMediaNode(MEDIA_NODE *pmedianode)
{
    CASE_NODE *casenode;
    casenode = SeekCaseNode(pmedianode->casenum);
    if(casenode != NULL)
    {
        pmedianode->next = casenode->mnext;
        casenode->mnext = pmedianode;
        return 1;
    }
    return 0;
}

/**
 * ��������: DelMediaInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void DelMediaInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"������ʡ������",
                   "        ",
                   "�����밸�����",
                   "            ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 6;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+18, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+18, srpop.Top+4},
                    {srpop.Left+9, srpop.Top+6},
                    {srpop.Left+18, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{srpop.Left + 18, srpop.Top + 2,
                           srpop.Left + 26, srpop.Top + 2},
                          {srpop.Left + 18, srpop.Top + 4,
                           srpop.Left + 30, srpop.Top + 4},
                          {srpop.Left + 9, srpop.Top + 6,
                           srpop.Left + 12, srpop.Top + 6},
                          {srpop.Left + 18, srpop.Top + 6,
                           srpop.Left + 21, srpop.Top + 6}};/*�ĸ�������λ��*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char proname[9];
    char casenum[13];
    char *pc1 = proname;
    char *pc2 = casenum;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,8,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1 || iHot==2))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pc1)=c;
                }
                if(iHot==2)
                {
                    *(--pc2)=c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3|| iHot == 4))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==3)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    if(strlen(proname)!=0 && strlen(casenum)!=0)
                    {
                        PopOff();
                        if(SeekProvinceNode(proname))
                        {
                            ;
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴�ʡ��","ȷ��"};
                            ShowModule(plabel_name,2);
                            return;
                        }

                        int re = DeleteMediaNode(casenum,proname);
                        if(re)
                        {
                            char *plabel_name[] = {"ý����Ϣɾ���ɹ���","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"ý����Ϣɾ��ʧ�ܣ�","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(proname)==0 || strlen(casenum)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }

                }
                else if(iHot==4)
                {
                    PopOff();
                    return;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 4;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>4)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==3 || iHot==4)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+7) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc1++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
            else if((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+11) && (iHot==2))
            {
                ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

/**
 * ��������: DeleteMediaNode
 * ��������: ��ʮ��������ɾ��һ��ý����Ϣ���.
 * �������: head ����ͷָ��
 *           casenum �������
             reportdate ��������
 * �������: ��
 * �� �� ֵ: int����, 1��ʾɾ���ɹ�, 0��ʾɾ��ʧ��
 *
 * ����˵��:
 */
int DeleteMediaNode(char *casenum,char *pvblg)
{
    int flag = 0;
    PROVINCE_NODE *hd;
    CASE_NODE *casenode;
    MEDIA_NODE *p1 = NULL,*p2;
    for(hd = head;hd != NULL;hd = hd->next)
    {
        casenode = hd->cnext;
        while(casenode != NULL)
        {
            if(strcmp(casenode->casenum,casenum) == 0)
            {
                flag = 1;
                break;
            }
            casenode = casenode->next;
        }
        if(flag)
        {
            break;
        }
    }
    for(p2 = casenode->mnext;p2 != NULL && strcmp(p2->pvblg,pvblg) != 0;p2 = p2->next)
    {
        p1 = p2;
    }
    if(p2 != NULL)
    {
        if(p1 == NULL)              /*Ҫɾ����ý����Ϣ�ڵ��ڵ�һ��*/
        {
            casenode->mnext = p2->next;
        }
        else
        {
            p1->next = p2->next;
        }
        free(p2);
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * ��������: ModifyMediaInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void ModifyMediaInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"������ʡ������",
                   "        ",
                   "�����밸�����",
                   "            ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 6;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+18, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+18, srpop.Top+4},
                    {srpop.Left+9, srpop.Top+6},
                    {srpop.Left+18, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{srpop.Left + 18, srpop.Top + 2,
                           srpop.Left + 26, srpop.Top + 2},
                          {srpop.Left + 18, srpop.Top + 4,
                           srpop.Left + 30, srpop.Top + 4},
                          {srpop.Left + 9, srpop.Top + 6,
                           srpop.Left + 12, srpop.Top + 6},
                          {srpop.Left + 18, srpop.Top + 6,
                           srpop.Left + 21, srpop.Top + 6}};/*�ĸ�������λ��*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char proname[9];
    char casenum[13];

    char *pc1 = proname;
    char *pc2 = casenum;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,8,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
	    //DealInput(&areas,&iHot);
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1 || iHot==2))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pc1)=c;
                }
                if(iHot==2)
                {
                    *(--pc2)=c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3|| iHot == 4))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==3)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    if(strlen(proname)!=0 && strlen(casenum)!=0)
                    {
                        PopOff();
                        if(SeekProvinceNode(proname))
                        {
                            InputModifyMediaInfo();
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴�ʡ��","ȷ��"};
                            ShowModule(plabel_name,2);
                        }
                        return;
                    }
                    else if(strlen(proname)==0 || strlen(casenum)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }

                }
                else if(iHot==4)
                {
                    PopOff();
                    return;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 4;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>4)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==3 || iHot==4)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+7) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc1++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
            else if((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+11) && (iHot==2))
            {
                ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

 /**
 * ��������: InputModifyMediaInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void InputModifyMediaInfo(void)
{
    MEDIA_NODE *pnewnode = (MEDIA_NODE *)malloc(sizeof(MEDIA_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"����ʡ��","        ",
                   "�������","            ",
                   "��������","          ",
                   "ý������","                    ",
                   "��������","                                                   ",
                   "ý����� '1' '2' '3' '4'�ֱ����ֽ���㲥�����Ӻͻ�����"," ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 57;
    pos.Y = 16;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 14;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3,  srpop.Top+2},
                    {srpop.Left+12, srpop.Top+2},
                    {srpop.Left+3,  srpop.Top+4},
                    {srpop.Left+12, srpop.Top+4},
                    {srpop.Left+3,  srpop.Top+6},
                    {srpop.Left+12, srpop.Top+6},
                    {srpop.Left+3,  srpop.Top+8},
                    {srpop.Left+12, srpop.Top+8},
                    {srpop.Left+3, srpop.Top+10},
                    {srpop.Left+12, srpop.Top+10},
                    {srpop.Left+3, srpop.Top+12},
                    {srpop.Left+60, srpop.Top+12},
                    {srpop.Left+24, srpop.Top+14},
                    {srpop.Left+31,srpop.Top+14},
                    };
    labels.pLoc = aLoc;

    areas.num = 8;
    SMALL_RECT aArea[] = {{srpop.Left + 12, srpop.Top + 2,
                           srpop.Left + 20, srpop.Top + 2},
                          {srpop.Left + 12, srpop.Top + 4,
                           srpop.Left + 24, srpop.Top + 4},
                          {srpop.Left + 12, srpop.Top + 6,
                           srpop.Left + 22, srpop.Top + 6},
                          {srpop.Left + 12, srpop.Top + 8,
                           srpop.Left + 32, srpop.Top + 8},
                          {srpop.Left + 12, srpop.Top + 10,
                           srpop.Left + 62, srpop.Top + 10},

                          {srpop.Left + 60, srpop.Top + 12,
                           srpop.Left + 61, srpop.Top + 12},

                          {srpop.Left + 24, srpop.Top + 14,
                           srpop.Left + 27, srpop.Top + 14},
                          {srpop.Left + 31, srpop.Top + 14,
                           srpop.Left + 34, srpop.Top + 14},
                           };/*�˸�������λ��*/
    char aSort[] = {1,1,1,1,1,1,0,0};
    char aTag[] = {1,2,3,4,5,6,7,8};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+13;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char pvblg[9];              /*����ʡ��*/
    char casenum[13];           /*�������*/
    char reportdate[11];        /*��������*/
    char medianame[21];         /*ý������*/
    char contentindex[51];      /*��������*/
    char mediatype;             /*ý�����*/

    char *pc1 = pvblg;
    char *pc2 = casenum;
    char *pc3 = reportdate;
    char *pc4 = medianame;
    char *pc5 = contentindex;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,8,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[3],&res);
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[5],&res);
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[7],&res);
	FillConsoleOutputAttribute(std_out,att2,50,aLoc[9],&res);
	FillConsoleOutputAttribute(std_out,att2,1,aLoc[11],&res);

	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown)
            && (iHot==1 || iHot==2 || iHot==3 || iHot==4 || iHot==5 || iHot==6))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pc1) = c;
                }
                else if(iHot==2)
                {
                    *(--pc2) = c;
                }
                else if(iHot==3)
                {
                    *(--pc3) = c;
                }
                else if(iHot==4)
                {
                    *(--pc4) = c;
                }
                else if(iHot==5)
                {
                    *(--pc5) = c;
                }
                else if(iHot==6)
                {
                    mediatype = c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot==1 || iHot==2 || iHot==3 || iHot==4 || iHot==5 || iHot==6 || iHot==7 || iHot==8))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==7)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    *pc3 = '\0';
                    *pc4 = '\0';
                    *pc5 = '\0';
                    if(strlen(reportdate)!=0 && strlen(casenum)!=0 && strlen(pvblg)!=0)
                    {
                        strcpy(pnewnode->reportdate,reportdate);
                        strcpy(pnewnode->casenum,casenum);
                        strcpy(pnewnode->pvblg,pvblg);
                        strcpy(pnewnode->medianame,medianame);
                        strcpy(pnewnode->contentindex,contentindex);
                        pnewnode->mediatype = mediatype;
                        pnewnode->next = NULL;
                        PopOff();
                        int re = ModifyMediaNode(casenum,pvblg,pnewnode);
                        if(re)
                        {
                            char *plabel_name[] = {"ý����Ϣ�޸ĳɹ���","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"ý����Ϣ�޸�ʧ�ܣ�","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(reportdate)==0 || strlen(casenum)==0 ||
                            strlen(pvblg)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }
                }
                else if(iHot==8)
                {
                    PopOff();
                    return;
                }
            }

            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 8;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>8)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==7 || iHot==8)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) && (iHot<7) && (pos.X<=aArea[0].Left+49))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    if(iHot == 1)
                    {
                        *(pc1++) = ch;
                    }
                    else if(iHot == 2)
                    {
                        *(pc2++) = ch;
                    }
                    else if(iHot == 3)
                    {
                        *(pc3++) = ch;
                    }
                    else if(iHot == 4)
                    {
                        *(pc4++) = ch;
                    }
                    else if(iHot == 5)
                    {
                        *(pc5++) = ch;
                    }
                    else if(iHot == 6)
                    {
                        mediatype = ch;
                    }
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

/**
 * ��������: ModifyMediaNode
 * ��������: ��ָ����ý����Ϣ������ݽ����޸�.
 * �������: casenum �������
 *           reportdate ��������
 *           pmedianode ָ�����޸����ݽ���ָ��
 * �������: ��
 * �� �� ֵ: int����, 1��ʾ�޸ĳɹ�, 0��ʾ�޸�ʧ��
 *
 * ����˵��:
 */
int ModifyMediaNode(char *casenum,char *pvblg,MEDIA_NODE *pmedianode)
{
    MEDIA_NODE *medianode = SeekMediaNode(casenum,pvblg);
    MEDIA_NODE *ptmp;
    if(medianode)
    {
        ptmp = medianode->next;
        *medianode = *pmedianode;
        medianode->next = ptmp;
        return 1;
    }
    return 0;
}

/**
 * ��������: QueryProInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
BOOL QueryProInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos,pos1,pos2;
    WORD att;
    char *pCh[] = {"��ʡ�����Ʋ�ѯ", "���ͼ츺���˲�ѯ","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+4, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+9, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{srpop.Left + 4, srpop.Top + 2,
                           srpop.Left + 17, srpop.Top + 2},
                          {srpop.Left + 3, srpop.Top + 4,
                           srpop.Left + 18, srpop.Top + 4},
                          {srpop.Left + 9, srpop.Top + 6,
                           srpop.Left + 12, srpop.Top + 6},};/*������ť������λ��*/
    char aSort[] = {0,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);

    pos.X = srpop.Left + 1;
    pos.Y = srpop.Top + 5;
    pos1.X = srpop.Left + 1;
    pos1.Y = srpop.Top + 1;
    pos2.X = srpop.Left + 1;
    pos2.Y = srpop.Top + 3;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos1,&ul);
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos2, &ul);

    if(DealInput(&areas, &iHot) == 13)
    {
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    PopOff();
    if(bRet)
    {
        if(iHot==1)
        {
            QueryProname();
        }
        else if(iHot==2)
        {
            QueryPerson();
        }
    }
    return bRet;
}

/**
 * ��������: QueryProname
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void QueryProname(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"���������ѯʡ������",
                   "        ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+8, srpop.Top+4},
                    {srpop.Left+7, srpop.Top+6},
                    {srpop.Left+15, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{srpop.Left + 8, srpop.Top + 4,
                           srpop.Left + 17, srpop.Top + 4},
                          {srpop.Left + 7, srpop.Top + 6,
                           srpop.Left + 10, srpop.Top + 6},
                          {srpop.Left + 15, srpop.Top + 6,
                           srpop.Left + 18, srpop.Top + 6}};/*���������λ��*/
    char aSort[] = {1,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char proname[9];
    char *pro = proname;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[1],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1))
            {
                //pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                *(--pro) = c;
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==2)
                {
                    if(strlen(proname)!=0)
                    {
                        *pro = '\0';
                        PopOff();
                        PROVINCE_NODE *tmp = QueryProvinceInfo1(proname);
                        if(tmp!=NULL)
                        {
                            char *plabel_name[] = {"ʡ�����ƣ�",tmp->pvblg,"�ͼ츺���ˣ�",tmp->disciplineperson,"��ϵ�绰��",tmp->tele,"ȷ��"};
                            ShowModule(plabel_name, 7);
                            return;
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴�ʡ�ݣ�","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(proname) == 0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return ;
                    }

                }
                else if(iHot==3)
                {
                    PopOff();
                    return ;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 3;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>3)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==2 || iHot==3)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+7) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pro++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * ��������: QueryProvinceInfo1
 * ��������: ��ָ����ʡ����Ϣ������ݽ��в�ѯ.
 * �������: head ����ͷָ��
 *           pvblg ����ʡ��
 * �������: ��
 * �� �� ֵ: PROVINCE_NODE ����, ���ز�ѯ�����ɵ�����ͷָ��
 *
 * ����˵��: ��ʡ������Ϊ���������Ҳ���ʾ��������������ʡ�ݼ����Ϣ
 */
PROVINCE_NODE *QueryProvinceInfo1(char *pvblg)
{
    PROVINCE_NODE *hd = head;
    for(;hd != NULL;hd = hd->next)
    {
        if(strcmp(hd->pvblg,pvblg) == 0)
            return hd;
    }
    return NULL;
}

/**
 * ��������: QueryPerson
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void QueryPerson(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"������ͼ츺��������",
                   "               ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+5, srpop.Top+4},
                    {srpop.Left+7, srpop.Top+6},
                    {srpop.Left+15, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{srpop.Left + 5, srpop.Top + 4,
                           srpop.Left + 20, srpop.Top + 4},
                          {srpop.Left + 7, srpop.Top + 6,
                           srpop.Left + 10, srpop.Top + 6},
                          {srpop.Left + 15, srpop.Top + 6,
                           srpop.Left + 18, srpop.Top + 6}};/*���������λ��*/
    char aSort[] = {1,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char person[15];
    char *per = person;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[1],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1))
            {
                //pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                *(--per) = c;
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==2)
                {
                    *per = '\0';
                    if(strlen(person)!=0)
                    {
                        PopOff();
                        PROVINCE_NODE *tmp = QueryProvinceInfo2(person);
                        if(tmp!=NULL)
                        {
                            for(;tmp!=NULL;tmp=tmp->next)
                            {
                                char *plabel_name[] = {"ʡ�����ƣ�",tmp->pvblg,"�ͼ츺���ˣ�",tmp->disciplineperson,"��ϵ�绰��",tmp->tele,"ȷ��"};
                                ShowModule(plabel_name, 7);
                            }
                            return;
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴�ʡ�ݣ�","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(person) == 0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return ;
                    }

                }
                else if(iHot==3)
                {
                    PopOff();
                    return ;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 3;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>3)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==2 || iHot==3)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+14) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(per++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * ��������: QueryProvinceInfo2
 * ��������: ��ָ����ʡ����Ϣ������ݽ��в�ѯ.
 * �������: head ����ͷָ��
 *           disciplineperson �ͼ츺����
 * �������: ��
 * �� �� ֵ: PROVINCE_NODE ����, ���ز�ѯ�����ɵ�����ͷָ��
 *
 * ����˵��: �Լͼ츺��������Ϊ���������Ҳ���ʾ��������������ʡ�ݼ����Ϣ
 */
PROVINCE_NODE *QueryProvinceInfo2(char *disciplineperson)
{
    PROVINCE_NODE *hd,*newprovincenodehd = NULL,*ptmp;
    for(hd = head;hd != NULL;hd = hd->next)
    {
        if(strcmp(hd->disciplineperson,disciplineperson) == 0)
        {
             ptmp = (PROVINCE_NODE *)malloc(sizeof(PROVINCE_NODE));
            *ptmp = *hd;
            ptmp->next = newprovincenodehd;
            newprovincenodehd = ptmp;
        }
    }
    return newprovincenodehd;
}

/**
 * ��������: QueryCaseInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
BOOL QueryCaseInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos,pos1,pos2;
    WORD att;
    char *pCh[] = {"������������Ʋ�ѯ", "���������������������ѯ","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+6, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+13, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{srpop.Left + 6, srpop.Top + 2,
                           srpop.Left + 23, srpop.Top + 2},
                          {srpop.Left + 3, srpop.Top + 4,
                           srpop.Left + 26, srpop.Top + 4},
                          {srpop.Left + 13, srpop.Top + 6,
                           srpop.Left + 16, srpop.Top + 6},};/*������ť������λ��*/
    char aSort[] = {0,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);

    pos.X = srpop.Left + 1;
    pos.Y = srpop.Top + 5;
    pos1.X = srpop.Left + 1;
    pos1.Y = srpop.Top + 1;
    pos2.X = srpop.Left + 1;
    pos2.Y = srpop.Top + 3;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos1,&ul);
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos2, &ul);

    if (DealInput(&areas, &iHot) == 13)
    {
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    PopOff();
    if(bRet)
    {
        if(iHot==1)
        {
            QueryCasenum();
        }
        else if(iHot==2)
        {
            QueryCrimer();
        }
    }

    return bRet;
}

/**
 * ��������: QueryCasenum
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void QueryCasenum(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"���������ѯ�İ������",
                   "            ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 4;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+8, srpop.Top+4},
                    {srpop.Left+8, srpop.Top+6},
                    {srpop.Left+16, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{srpop.Left + 8, srpop.Top + 4,
                           srpop.Left + 20, srpop.Top + 4},
                          {srpop.Left + 8, srpop.Top + 6,
                           srpop.Left + 11, srpop.Top + 6},
                          {srpop.Left + 16, srpop.Top + 6,
                           srpop.Left + 19, srpop.Top + 6}};/*���������λ��*/
    char aSort[] = {1,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char casenum[13];
    char *pcase = casenum;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[1],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1))
            {
                //pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                *(--pcase) = c;
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==2)
                {
                    if(strlen(casenum)!=0)
                    {
                        PopOff();
                        CASE_NODE *tmp = QueryCaseInfo1(casenum);
                        if(tmp!=NULL)
                        {
                            for(;tmp!=NULL;tmp = tmp->next)
                            {
                                char punchar[4];
                                itoa(tmp->punishment,punchar,10);

                                /*��float������ת�����ַ���*/
                                int num = (int)(tmp->money);
                                int i;
                                float flo = tmp->money - num;
                                char ary[100];
                                itoa(num,ary,10);
                                char *p1 = ary;
                                while(*p1)
                                {
                                    p1++;
                                }
                                *p1++ = '.';
                                for(i=0; i<6 ;i++)
                                {
                                    flo *= 10;
                                    num = (int)flo;
                                    *p1++ = num + '0';
                                    flo = flo - num;
                                }
                                *p1 = '\0';

                                char a[2],b[2];
                                a[0] = tmp->level;
                                a[1] = '\0';
                                b[0] = tmp->accusationname;
                                b[1] = '\0';


                                    char *plabel_name[] = {"������ţ�",tmp->casenum,"�������ڣ�",
                                                        tmp->casedate,"����ʡ�ݣ�",tmp->pvblg,"������������",
                                                        tmp->crimename,"��������",a,"��Ҫ������",b,
                                                        "�永��",ary,"���´�����",punchar,"ȷ��"};
                                    ShowModule(plabel_name,17);
                            }

                            return;
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴˰�����Ϣ��","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(casenum) == 0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }

                }
                else if(iHot==3)
                {
                    PopOff();
                    return ;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 3;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>3)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==2 || iHot==3)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+10) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pcase++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * ��������: QueryCaseInfo1
 * ��������: ��ָ���İ�����Ϣ������ݽ��в�ѯ.
 * �������: head ����ͷָ��
 *           casenum �������
 * �������: ��
 * �� �� ֵ: CASE_NODE ����, ���ز�ѯ�����ɵ�����ͷָ��
 *
 * ����˵��: �԰������Ϊ�������Ҳ���ʾ����������̰�������Ļ�����Ϣ
 */
CASE_NODE *QueryCaseInfo1(char *casenum)
{
    PROVINCE_NODE *hd = head;
    CASE_NODE *casenode,*newcasenodehd = NULL,*ptmp;
    for(;hd !=NULL;hd = hd->next)
    {
        for(casenode = hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            if(strcmp(casenode->casenum,casenum) == 0)
            {
                ptmp = (CASE_NODE *)malloc(sizeof(CASE_NODE));
                *ptmp = *casenode;
                ptmp->next = newcasenodehd;
                newcasenodehd = ptmp;
            }
        }
    }
    return newcasenodehd;
}

/**
 * ��������: QueryCrimer
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: ��
 *
 * ����˵��:
 */
void QueryCrimer(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"�����뱻������",
                   "               ",
                   "�����뼶�𣨡�A��~��H�����ֱ�����Ƽ������Ƽ�����������",
                   "�������������������������������������������ϣ�",
                   " ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[2]) + 6;
    pos.Y = 13;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 7;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+3, srpop.Top+6},
                    {srpop.Left+3, srpop.Top+7},
                    {srpop.Left+3, srpop.Top+9},
                    {srpop.Left+21, srpop.Top+11},
                    {srpop.Left+33, srpop.Top+11}};
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{srpop.Left + 3, srpop.Top + 4,
                           srpop.Left + 18, srpop.Top + 4},
                          {srpop.Left + 3, srpop.Top + 9,
                           srpop.Left + 18, srpop.Top + 9},
                          {srpop.Left + 21, srpop.Top + 11,
                           srpop.Left + 24, srpop.Top + 11},
                          {srpop.Left + 33, srpop.Top + 11,
                           srpop.Left + 36, srpop.Top + 11}};/*�ĸ�������λ��*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+10;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char crimer[16];
    char level;
    char *pcrimer = crimer;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[4],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1 || iHot==2))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pcrimer) = c;
                }
                else if(iHot==2)
                {
                    level = c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3 || iHot == 4))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==3)
                {
                    *pcrimer = '\0';
                    if(strlen(crimer)!=0)
                    {
                        PopOff();
                        CASE_NODE *tmp = QueryCaseInfo2(crimer,level);
                        if(tmp!=NULL)
                        {
                            for(;tmp!=NULL;tmp=tmp->next)
                            {
                                char punchar[4];
                                itoa(tmp->punishment,punchar,10);

                                /*��float������ת�����ַ���*/
                                int num = (int)(tmp->money);
                                int i;
                                float flo = tmp->money - num;
                                char ary[100];
                                itoa(num,ary,10);
                                char *p1 = ary;
                                while(*p1)
                                {
                                    p1++;
                                }
                                *p1++ = '.';
                                for(i=0; i<6 ;i++)
                                {
                                    flo *= 10;
                                    num = (int)flo;
                                    *p1++ = num + '0';
                                    flo = flo - num;
                                }
                                *p1 = '\0';

                                char a[2],b[2];
                                a[0] = tmp->level;
                                a[1] = '\0';
                                b[0] = tmp->accusationname;
                                b[1] = '\0';

                                    char *plabel_name[] = {"������ţ�",tmp->casenum,"�������ڣ�",
                                                        tmp->casedate,"����ʡ�ݣ�",tmp->pvblg,"������������",
                                                        tmp->crimename,"��������",a,"��Ҫ������",b,
                                                        "�永��",ary,"���´�����",punchar,"ȷ��"};
                                    ShowModule(plabel_name,17);
                            }
                            return;
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴˰�����Ϣ��","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(crimer) == 0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return;
                    }
                }
                else if(iHot==4)
                {
                    PopOff();
                    return ;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 4;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>4)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==3 || iHot==4)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+13) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pcrimer++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
            else if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+13) && (iHot==2))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    level = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * ��������: QueryCaseInfo2
 * ��������: ��ָ���İ�����Ϣ������ݽ��в�ѯ.
 * �������: head ����ͷָ��
 *           crimename ����������
 * �������: ��
 * �� �� ֵ: CASE_NODE ����, ���ز�ѯ�����ɵ�����ͷָ��
 *
 * ����˵��: �Ա�����������������Ϊ�������Ҳ���ʾ����������̰�������Ļ�����Ϣ
 */
CASE_NODE *QueryCaseInfo2(char *crimename,char level)
{
    PROVINCE_NODE *hd = head;
    CASE_NODE *casenode,*newcasenodehd = NULL,*ptmp;
    for(;hd !=NULL;hd = hd->next)
    {
        for(casenode = hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            if(strcmp(casenode->crimename,crimename) == 0 && (casenode->level==level))
            {
                ptmp = (CASE_NODE *)malloc(sizeof(CASE_NODE));
                *ptmp = *casenode;
                ptmp->next = newcasenodehd;
                newcasenodehd = ptmp;
            }
        }
    }
    return newcasenodehd;
}

/**
 * ��������: QueryMediaInfo
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
BOOL QueryMediaInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos,pos1,pos2;
    WORD att;
    char *pCh[] = {"�����������ý�����Ʋ�ѯ", "�����������벿��ý�����Ʋ�ѯ","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 3;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+5, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+15, srpop.Top+6}};
    labels.pLoc = aLoc;

    areas.num = 3;
    SMALL_RECT aArea[] = {{srpop.Left + 5, srpop.Top + 2,
                           srpop.Left + 28, srpop.Top + 2},
                          {srpop.Left + 3, srpop.Top + 4,
                           srpop.Left + 30, srpop.Top + 4},
                          {srpop.Left + 15, srpop.Top + 6,
                           srpop.Left + 18, srpop.Top + 6},};/*������ť������λ��*/
    char aSort[] = {0,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);

    pos.X = srpop.Left + 1;
    pos.Y = srpop.Top + 5;
    pos1.X = srpop.Left + 1;
    pos1.Y = srpop.Top + 1;
    pos2.X = srpop.Left + 1;
    pos2.Y = srpop.Top + 3;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos1,&ul);
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos2, &ul);

    if (DealInput(&areas, &iHot) == 13)
    {
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    PopOff();

    if(bRet)
    {
        if(iHot==1)
        {
            QueryCaseMedia();
        }
        else if(iHot==2)
        {
            QueryDateMedia();
        }
    }

    return bRet;
}

/**
 * ��������: QueryCaseMedia
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
void QueryCaseMedia(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"���������ѯ�İ������",
                   "            ",
                   "������ý������",
                   "                    ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 12;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 6;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+4, srpop.Top+4},
                    {srpop.Left+3, srpop.Top+6},
                    {srpop.Left+4, srpop.Top+8},
                    {srpop.Left+8, srpop.Top+10},
                    {srpop.Left+16, srpop.Top+10}};
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{srpop.Left + 4, srpop.Top + 4,
                           srpop.Left + 16, srpop.Top + 4},
                          {srpop.Left + 4, srpop.Top + 8,
                           srpop.Left + 24, srpop.Top + 8},
                          {srpop.Left + 8, srpop.Top + 10,
                           srpop.Left + 11,srpop.Top + 10},
                          {srpop.Left + 16, srpop.Top + 10,
                           srpop.Left + 19, srpop.Top + 10}};/*�ĸ�������λ��*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+9;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char casenum[13];
    char medianame[21];
    char *pc1 = casenum;
    char *pc2 = medianame;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1 || iHot==2))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(pc1--) = c;
                }
                else if(iHot==2)
                {
                    *(pc2--) = c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3 || iHot == 4))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==3)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    if(strlen(casenum)!=0 && strlen(medianame)!=0)
                    {
                        PopOff();
                        MEDIA_NODE *tmp = QueryMediaInfo1(casenum,medianame);
                        if(tmp!=NULL)
                        {
                            for(;tmp!=NULL;tmp=tmp->next)
                            {
                                char a[2];
                                a[0] = tmp->mediatype;
                                a[1] = '\0';
                                char *plabel_name[] = {"����ʡ�ݣ�",tmp->pvblg,"������ţ�",tmp->casenum,
                                                        "�������ڣ�",tmp->reportdate,"ý�����ƣ�",tmp->medianame,
                                                        "ý�����",a,"����������",tmp->contentindex,"ȷ��"};
                                ShowModule(plabel_name,13);
                            }
                            return;
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴�ý����Ϣ��","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        return;
                    }
                    else if(strlen(casenum)==0 || strlen(medianame)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return ;
                    }

                }
                else if(iHot==4)
                {
                    PopOff();
                    return ;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 4;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>4)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==3 || iHot==4)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+9) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc1++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
            else if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+19) && (iHot==2))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * ��������: QueryMediaInfo1
 * ��������: ��ָ����ý����Ϣ������ݽ��в�ѯ.
 * �������: head ����ͷָ��
 *           casenum �������
 *           medianame ý������
 * �������: ��
 * �� �� ֵ: MEDIA_NODE ����, ���ز�ѯ�����ɵ�����ͷָ��
 *
 * ����˵��: �԰�����ź�ý������Ϊ�������Ҳ���ʾ����������ý�屨��������Ϣ
 */
MEDIA_NODE *QueryMediaInfo1(char *casenum,char *medianame)
{
    PROVINCE_NODE *hd;
    CASE_NODE *casenode;
    MEDIA_NODE *medianode,*newmedianodehd = NULL,*ptmp;
    for(hd = head;hd != NULL;hd = hd->next)
    {
        for(casenode= hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            for(medianode = casenode->mnext;medianode != NULL;medianode = medianode->next)
            {
                if(strcmp(medianode->medianame,medianame) == 0 && strcmp(medianode->casenum,casenum) == 0)
                {
                    ptmp = (MEDIA_NODE *)malloc(sizeof(MEDIA_NODE));
                    *ptmp = *medianode;
                    ptmp->next = newmedianodehd;
                    newmedianodehd = ptmp;
                }
            }
        }
    }
    return newmedianodehd;
}

/**
 * ��������: QueryDateMedia
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
void QueryDateMedia(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"�����뱨������",
                   "          ",
                   "�����벿��ý������",
                   "                    ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[2]) + 8;
    pos.Y = 12;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 6;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+3, srpop.Top+4},
                    {srpop.Left+3, srpop.Top+6},
                    {srpop.Left+3, srpop.Top+8},
                    {srpop.Left+6, srpop.Top+10},
                    {srpop.Left+14, srpop.Top+10}};
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{srpop.Left + 3, srpop.Top + 4,
                           srpop.Left + 13, srpop.Top + 4},
                          {srpop.Left + 3, srpop.Top + 8,
                           srpop.Left + 23, srpop.Top + 8},
                          {srpop.Left + 6, srpop.Top + 10,
                           srpop.Left + 9,srpop.Top + 10},
                          {srpop.Left + 14, srpop.Top + 10,
                           srpop.Left + 17, srpop.Top + 10}};/*�ĸ�������λ��*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+9;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char reportdate[10];
    char medianame[20];
    char *pc1 = reportdate;
    char *pc2 = medianame;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1 || iHot==2))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(pc1--) = c;
                }
                else if(iHot==2)
                {
                    *(pc2--) = c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3 || iHot == 4))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==3)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    if(strlen(reportdate)!=0 && strlen(medianame)!=0)
                    {
                        PopOff();
                        MEDIA_NODE *tmp = QueryMediaInfo2(reportdate,medianame);
                        if(tmp!=NULL)
                        {
                            for(;tmp!=NULL;tmp=tmp->next)
                            {
                                char a[2];
                                a[0] = tmp->mediatype;
                                a[1] = '\0';
                                char *plabel_name[] = {"����ʡ�ݣ�",tmp->pvblg,"������ţ�",tmp->casenum,
                                                        "�������ڣ�",tmp->reportdate,"ý�����ƣ�",tmp->medianame,
                                                        "ý�����",a,"����������",tmp->contentindex,"ȷ��"};
                                ShowModule(plabel_name,13);
                            }
                        }
                        else
                        {
                            char *plabel_name[] = {"�޴�ý����Ϣ��","ȷ��"};
                            ShowModule(plabel_name, 2);
                        }
                        return;
                    }
                    else if(strlen(reportdate)==0 || strlen(medianame)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return ;
                    }

                }
                else if(iHot==4)
                {
                    PopOff();
                    return;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 4;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>4)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==3 || iHot==4)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+7) && (iHot==1))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc1++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
            else if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+19) && (iHot==2))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * ��������: QueryMediaInfo2
 * ��������: ��ָ����ý����Ϣ������ݽ��в�ѯ.
 * �������: head ����ͷָ��
 *           reportdate ��������
 *           medianame ý������
 * �������: ��
 * �� �� ֵ: MEDIA_NODE ����, ���ز�ѯ�����ɵ�����ͷָ��
 *
 * ����˵��: �Ա������ں�ý�����Ƶ�һ����Ϊ�������Ҳ���ʾ����������ý�屨��������Ϣ
 */
MEDIA_NODE *QueryMediaInfo2(char *reportdate,char *medianame)
{
    PROVINCE_NODE *hd;
    CASE_NODE *casenode;
    MEDIA_NODE *medianode,*newmedianodehd = NULL,*ptmp;
    for(hd = head;hd != NULL;hd = hd->next)
    {
        for(casenode= hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            for(medianode = casenode->mnext;medianode != NULL;medianode = medianode->next)
            {
                if(strcmp(medianode->reportdate,reportdate) == 0 && strstr(medianode->medianame,medianame) != NULL)
                {
                    ptmp = (MEDIA_NODE *)malloc(sizeof(MEDIA_NODE));
                    *ptmp = *medianode;
                    ptmp->next = newmedianodehd;
                    newmedianodehd = ptmp;
                }
            }
        }
    }
    return newmedianodehd;
}

/**
 * ��������: HelpTheme
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
 BOOL HelpTheme(void)
{
    char *plabel_name[] = {"    ��ӭ�뱾ϵͳ��������ϵ","",
                "�������䣺zmhustcsee@gmail.com","",
                "          U201511086@hust.edu.cn","",
                "          2856817781@qq.com","",
                "GitHub��  Githuberzhang","",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 11);

    return TRUE;
}

/**
 * ��������: HelpAbout
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
BOOL HelpAbout(void)
{
    char *plabel_name[] = {"   �����ߣ�����","","",
                           "   ���пƼ���ѧ","",
                           "�������ѧ�뼼��ѧԺ","",
                           " ׿Խ����ʦ��1501",
                           "ȷ��"
                          };

    ShowModule(plabel_name,9);
    return TRUE;
}

/**
 * ��������: ShowModule
 * ��������: ��ʾ���ܺ�����Ӧ�Ľ���
 * �������: pString ������ʾ���ݵ�ָ������ĵ�ַ
 *           n ��ʾ���ݵ�����
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
BOOL ShowModule(char **pString, int n)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    int iHot = 1;
    int i, maxlen, str_len;

    for (i=0,maxlen=0; i<n; i++) {
        str_len = strlen(pString[i]);
        if (maxlen < str_len) {
            maxlen = str_len;
        }
    }

    pos.X = maxlen + 9;
    pos.Y = n + 5;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = n;
    labels.ppLabel = pString;
    COORD aLoc[n];

    for (i=0; i<n; i++) {
        aLoc[i].X = srpop.Left + 5;
        aLoc[i].Y = srpop.Top + 2 + i;

    }
    str_len = strlen(pString[n-1]);
    aLoc[n-1].X = srpop.Left + 4 + (maxlen-str_len)/2;
    aLoc[n-1].Y = aLoc[n-1].Y + 2;

    labels.pLoc = aLoc;

    areas.num = 1;
    SMALL_RECT aArea[] = {{aLoc[n-1].X, aLoc[n-1].Y,
                           aLoc[n-1].X + 3, aLoc[n-1].Y}};

    char aSort[] = {0};
    char aTag[] = {1};

    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);

    pos.X = srpop.Left + 1;
    pos.Y = srpop.Top + 2 + n;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);

    DealInput(&areas, &iHot);
    PopOff();

    return TRUE;
}

/**
 * ��������: SeekProvinceNode
 * ��������: ������ʡ�ݲ��Ұ���������Ϣ���
 * �������:
 *           pvblg ����ʡ��
 * �������: ��
 * �� �� ֵ: ����ʱ���ؽ��ĵ�ַ, ���򷵻�NULL
 *
 * ����˵��:
 */
PROVINCE_NODE *SeekProvinceNode(char *pvblg)
{
    PROVINCE_NODE *hd = head;
    int find = 0;
    for(;hd != NULL;hd = hd->next)
    {
        if(strcmp(hd->pvblg,pvblg) == 0)
        {
            find = 1;
            break;
        }
    }
    if(find) return hd;
    else return NULL;
}

/**
 * ��������: SeekCaseNode
 * ��������: ��������Ų��Ұ���������Ϣ���
 * �������:
 *           casenum �������
 * �������: ��
 * �� �� ֵ: ����ʱ���ؽ��ĵ�ַ, ���򷵻�NULL
 *
 * ����˵��:
 */
CASE_NODE *SeekCaseNode(char *casenum)
{
    PROVINCE_NODE *hd = head;
    CASE_NODE *pcasenode;
    int find = 0;
    for(;hd != NULL;hd = hd->next)
    {
        pcasenode = hd->cnext;
        for(;pcasenode != NULL;pcasenode = pcasenode->next)
        {
            if(strcmp(pcasenode->casenum,casenum) == 0)
            {
                find = 1;
                break;
            }
        }
        if(find)
            break;
    }
    if(find)
        return pcasenode;
    else return NULL;
}

/**
 * ��������: SeekMediaNode
 * ��������: ��������ź�ý�屨�����ڲ���ý����Ϣ�ڵ�
 * �������:
 *           casenum �������
 *           reportdate ��������
 * �������: ��
 * �� �� ֵ: ����ʱ���ؽ��ĵ�ַ, ���򷵻�NULL
 *
 * ����˵��:
 */
MEDIA_NODE *SeekMediaNode(char *casenum,char *pvblg)
{
    int flag = 0;
    MEDIA_NODE *pmedianode;
    CASE_NODE *casenode = SeekCaseNode(casenum);
    if(casenode != NULL)
    {
        for(pmedianode = casenode->mnext;pmedianode != NULL;pmedianode = pmedianode->next)
        {
            if(strcmp(pmedianode->pvblg,pvblg) == 0)
            {
                flag = 1;
                break;
            }
        }
    }
    if(flag)
    {
        return pmedianode;
    }
    else
    {
        return NULL;
    }
}

/**
 * ��������: FileDataSave
 * ��������: ���������������
 * �������: head����ͷָ��
 * �������:
 * ����ֵ: BOOL���ͣ���ΪTURE
 *
 * ����˵��:
 */
BOOL FileDataSave(void)
{
    PROVINCE_NODE *hd;
    CASE_NODE *casenode;
    MEDIA_NODE *medianode;
    FILE *fsave;

    fsave = fopen(proinfofile,"w+");
    if(!fsave)
    {
        char *plabel_name[] = {"���ݱ���ʧ��",
                               "ȷ��"
                          };
        ShowModule(plabel_name, 2);
        return TRUE;
    }

    for(hd = head;hd!=NULL;hd = hd->next)
    {
        fprintf(fsave,"%s %s %s\n",hd->pvblg,hd->disciplineperson,hd->tele);
    }
    fclose(fsave);

    fsave = fopen(caseinfofile,"w+");
    if(!fsave)
    {
        char *plabel_name[] = {"���ݱ���ʧ��",
                           "ȷ��"};

        ShowModule(plabel_name, 2);
        return TRUE;
    }
    for(hd = head;hd != NULL;hd = hd->next)
    {
        for(casenode = hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            fprintf(fsave,"%s %s %s %s %c %c %f %d\n",casenode->casenum,
                    casenode->casedate,casenode->pvblg,casenode->crimename,
                    casenode->level,casenode->accusationname,
                    casenode->money,casenode->punishment);
        }
    }
    fclose(fsave);

    fsave = fopen(mediainfofile,"w+");
    if(!fsave)
    {
        char *plabel_name[] = {"���ݱ���ʧ��",
                           "ȷ��"
                          };
        ShowModule(plabel_name, 2);
        return TRUE;
    }
    for(hd = head;hd != NULL;hd = hd->next)
    {
        for(casenode = hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            for(medianode = casenode->mnext;medianode != NULL;medianode = medianode->next)
            {
                fprintf(fsave,"%s %s %s %c %s %s\n",medianode->pvblg,medianode->casenum,
                        medianode->reportdate,medianode->mediatype,medianode->medianame,
                        medianode->contentindex);
            }
        }
    }
    fclose(fsave);
    char *plabel_name[] = {"���ݱ���ɹ�",
                           "ȷ��"
                          };

    ShowModule(plabel_name, 2);

    return TRUE;
}

/**
 * ��������: FileDataBackup
 * ��������: ��������������ݵ�ָ���ļ���ȥ
 * �������: head ����ͷָ��
 *           filename �����ļ���
 * �������:
 * ����ֵ: BOOL���ͣ���ΪTURE
 *
 * ����˵��:
 */
BOOL FileDataBackUp(void)
{
    PROVINCE_NODE *hd;
    CASE_NODE *casenode;
    MEDIA_NODE *medianode;
    FILE *fpro,*fcase,*fmedia;

    fpro = fopen(probackupfile,"w+");
    fcase = fopen(casebackupfile,"w+");
    fmedia = fopen(mediabackupfile,"w+");
    if(!fpro)
    {
        char *plabel_name[] = {"���ݱ���ʧ��",
                           "ȷ��"
                          };
        ShowModule(plabel_name, 2);
        return TRUE;
    }
    if(!fcase)
    {
        char *plabel_name[] = {"���ݱ���ʧ��",
                           "ȷ��"
                          };
        ShowModule(plabel_name, 2);
        return TRUE;
    }
    if(!fmedia)
    {
        char *plabel_name[] = {"���ݱ���ʧ��",
                           "ȷ��"
                          };
        ShowModule(plabel_name, 2);
        return TRUE;
    }

    for(hd = head;hd != NULL;hd = hd->next)
    {
        fprintf(fpro,"%s %s %s\n",hd->pvblg,hd->disciplineperson,hd->tele);
    }

    for(hd = head;hd != NULL;hd = hd->next)
    {
        for(casenode = hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            fprintf(fcase,"%s %s %s %s %c %c %f %d\n",casenode->casenum,
                    casenode->casedate,casenode->pvblg,casenode->crimename,
                    casenode->level,casenode->accusationname,
                    casenode->money,casenode->punishment);
        }
    }

    for(hd = head;hd != NULL;hd = hd->next)
    {
        for(casenode = hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            for(medianode = casenode->mnext;medianode != NULL;medianode = medianode->next)
            {
                fprintf(fmedia,"%s %s %s %c %s %s\n",medianode->pvblg,medianode->casenum,
                        medianode->reportdate,medianode->mediatype,medianode->medianame,
                        medianode->contentindex);
            }
        }
    }

    char *plabel_name[] = {"ʡ����Ϣ�����ѱ�����",
                            probackupfile,
                           "������Ϣ�����ѱ�����",
                           casebackupfile,
                           "ý�屨�������ѱ�����",
                           mediabackupfile,
                           "ȷ��"
                          };

    ShowModule(plabel_name, 7);
    fclose(fpro);
    fclose(fcase);
    fclose(fmedia);
    return TRUE;

}

/**
 * ��������: FileDataRestore_Interface
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
BOOL FileDataRestore_Interface(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"�������ʡ����Ϣ�ļ�����",
                   "                    ",
                   "������İ�����Ϣ�ļ�����",
                   "                    ",
                   "�������ý�屨���ļ�����",
                   "                    ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 16;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 8;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+5, srpop.Top+4},
                    {srpop.Left+3, srpop.Top+6},
                    {srpop.Left+5, srpop.Top+8},
                    {srpop.Left+3, srpop.Top+10},
                    {srpop.Left+5, srpop.Top+12},
                    {srpop.Left+8, srpop.Top+14},
                    {srpop.Left+17,srpop.Top+14}};
    labels.pLoc = aLoc;

    areas.num = 5;
    SMALL_RECT aArea[] = {{srpop.Left + 5, srpop.Top + 4,
                           srpop.Left + 25, srpop.Top + 4},
                          {srpop.Left + 5, srpop.Top + 8,
                           srpop.Left + 25, srpop.Top + 8},
                          {srpop.Left + 5, srpop.Top + 12,
                           srpop.Left + 25, srpop.Top + 12},
                          {srpop.Left + 8, srpop.Top + 14,
                           srpop.Left + 11, srpop.Top + 14},
                          {srpop.Left + 17, srpop.Top + 14,
                           srpop.Left + 20, srpop.Top + 14}};/*���������λ��*/
    char aSort[] = {1,1,1,0,0};
    char aTag[] = {1,2,3,4,5};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+13;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char profile[20];
    char casefile[20];
    char mediafile[20];
    char *pf1 = profile;
    char *pf2 = casefile;
    char *pf3 = mediafile;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[3],&res);
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[5],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return TRUE;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) &&
                    (iHot==1 || iHot==2 || iHot==3))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(--pf1) = c;
                }
                else if(iHot==2)
                {
                    *(--pf2) = c;
                }
                else if(iHot==3)
                {
                    *(--pf3) = c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3 || iHot==4 || iHot==5))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                *pf1 = '\0';
                *pf2 = '\0';
                *pf3 = '\0';
                if(iHot==4)
                {
                    if(strlen(profile)!=0 && strlen(casefile)!=0 && strlen(mediafile)!=0)
                    {
                        FileDataRestore(profile,casefile,mediafile);
                        PopOff();
                        return TRUE;
                    }
                    else if(strlen(profile)==0 || strlen(casefile)==0 || strlen(mediafile)==0)
                    {
                        char *plabel_name[] = {"���ݲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        return TRUE;
                    }

                }
                else if(iHot==5)
                {
                    PopOff();
                    return TRUE;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 5;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>5)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==4 || iHot==5)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+20) && (iHot==1 || iHot==2 || iHot==3))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    if(iHot==1)
                    {
                        *(pf1++) = ch;
                    }
                    else if(iHot==2)
                    {
                        *(pf2++) = ch;
                    }
                    else if(iHot==3)
                    {
                        *(pf3++) = ch;
                    }
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * ��������: FileDataRestore
 * ��������: �ָ������������
 * �������: profile,casefile,mediafile �����ļ���
 * �������:
 * ����ֵ: BOOL���ͣ���ΪTURE
 *
 * ����˵��:
 */
BOOL FileDataRestore(char *profile,char *casefile,char *mediafile)
{

    int re = ReCreateList(&head,profile,casefile,mediafile);
    if(re == 28)
    {
        char *plabel_name[] = {"��������ɹ�","ȷ��"};
        ShowModule(plabel_name,2);
        PopOff();
        return TRUE;
    }
    else
    {
        char *plabel_name[] = {"��������ʧ��","ȷ��"};
        ShowModule(plabel_name,2);
        PopOff();
        return TRUE;
    }
}

/**
 * ��������: ReCreateList
 * ��������: �ؽ�����
 * �������: phead ����ͷָ���ַ
 *           profile,casefile,mediafile �����ļ���
 * �������:
 * ����ֵ: int��
 *
 * ����˵��:
 */
int ReCreateList(PROVINCE_NODE **phead,char *profile,char *casefile,char *mediafile)
{
    int flag = 0;
    int re = 0;
    PROVINCE_NODE *prohead = NULL, *pPROVINCENODE,struct1;
    CASE_NODE *pCASENODE,struct2;
    MEDIA_NODE *pMEDIANODE,struct3;
    FILE *pf;
    if((pf = fopen(profile,"r")) == NULL)
    {
        return re;
    }
    while(fscanf(pf,"%s %s %s\n",struct1.pvblg,struct1.disciplineperson,struct1.tele)==3)
    {
        pPROVINCENODE = (PROVINCE_NODE *)malloc(sizeof(PROVINCE_NODE));
        *pPROVINCENODE = struct1;
        pPROVINCENODE->cnext = NULL;
        pPROVINCENODE->next = prohead;                 /*��������ȳ�������*/
        prohead = pPROVINCENODE;                       /*��������ȳ�������*/
    }
    fclose(pf);
    if(prohead == NULL)
    {
        return re;
    }
    *phead = prohead;                                  /*������ʮ�ֽ��������ͷ*/

    fclose(pf);
    re += 4;

    if((pf = fopen(casefile,"r")) == NULL)
    {
        return re;
    }
    re += 8;

    while(fscanf(pf,"%s %s %s %s %c %c %f %d\n",struct2.casenum,
                 struct2.casedate,struct2.pvblg,struct2.crimename,
                 &struct2.level,&struct2.accusationname,
                 &struct2.money,&struct2.punishment)==8)
    {
        pCASENODE = (CASE_NODE *)malloc(sizeof(CASE_NODE));
        *pCASENODE = struct2;
        pCASENODE->mnext = NULL;
        pPROVINCENODE = prohead;

        while((pPROVINCENODE != NULL) && (strcmp(pPROVINCENODE->pvblg,pCASENODE->pvblg)) != 0)
        {
            pPROVINCENODE = pPROVINCENODE->next;
        }

        if(pPROVINCENODE != NULL)
        {
            pCASENODE->next = pPROVINCENODE->cnext;         /*��������ȳ�������*/
            pPROVINCENODE->cnext = pCASENODE;              /*��������ȳ�������*/
        }
        else
        {
            free(pCASENODE);
        }

    }
    fclose(pf);
    if((pf = fopen(mediafile,"r")) == NULL)
    {
        return re;
    }
    re += 16;

    while(fscanf(pf,"%s %s %s %c %s %s\n",struct3.pvblg,
                 struct3.casenum,struct3.reportdate,
                 &struct3.mediatype,struct3.medianame,struct3.contentindex)==6)
    {
        pMEDIANODE = (MEDIA_NODE *)malloc(sizeof(MEDIA_NODE));
        *pMEDIANODE = struct3;
        flag = 0;
        for(pPROVINCENODE = prohead;pPROVINCENODE != NULL && flag==0;pPROVINCENODE = pPROVINCENODE->next)
        {
            for(pCASENODE = pPROVINCENODE->cnext;pCASENODE != NULL && flag==0;pCASENODE = pCASENODE->next)
            {
                if(strcmp(pCASENODE->casenum,pMEDIANODE->casenum) == 0)
                {
                    flag = 1;
                    break;
                }
            }
            if(flag)
            {
                break;
            }
        }

        if(flag)
        {
            pMEDIANODE->next = pCASENODE->mnext;
            pCASENODE->mnext = pMEDIANODE;
        }
        else
        {
            free(pMEDIANODE);
        }
    }
    fclose(pf);
    return re;
}

/**
 * ��������: FileExit
 * ��������: �������ܺ�����Ӧ�Ľ����˳�ϵͳ
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
BOOL FileExit(void)
{

    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    char *pCh[] = {"ȷ���˳�ϵͳ��", "ȷ��    ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 7;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+5, srpop.Top+5}};
    labels.pLoc = aLoc;

    areas.num = 2;
    SMALL_RECT aArea[] = {{srpop.Left + 5, srpop.Top + 5,
                           srpop.Left + 8, srpop.Top + 5},
                          {srpop.Left + 13, srpop.Top + 5,
                           srpop.Left + 16, srpop.Top + 5}};/* "ȷ��" �� "ȡ��" ��ť������λ��*/
    char aSort[] = {0, 0};
    char aTag[] = {1, 2};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);

    pos.X = srpop.Left + 1;
    pos.Y = srpop.Top + 4;
    FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-1, pos, &ul);

    if (DealInput(&areas, &iHot) == 13 && iHot == 1)
    {
        bRet = TRUE;
        PopOff();
        exit(-1);
    }

    PopOff();
    return bRet;
}

/**
 * ��������: StatusCrimeName
 * ��������: ����Ҫ����Ϊ�����ֱ�ͳ����������������̰�������ķ�������
 *           �永��������������������������ͳ�ƽ����
 * �������: head ����ͷָ��
 * �������: ��
 * �� �� ֵ: void
 * ����˵��:
 */
BOOL StatusCrimeName(void)
{
    int i,j;
    PROVINCE_NODE *hd;
    CASE_NODE *casenode;

    for(i=0; i<6; i++)
    {
        crimecasemoney[i].sumcount = 0;
        crimecasemoney[i].summoney = 0.0;
    }

    for(hd = head;hd != NULL;hd = hd->next)
    {
        for(casenode = hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            if(casenode->accusationname == '1')
            {
                crimecasemoney[0].sumcount++;
                crimecasemoney[0].accuname = '1';
                crimecasemoney[0].summoney += casenode->money;
            }
            else if(casenode->accusationname == '2')
            {
                crimecasemoney[1].sumcount++;
                crimecasemoney[1].accuname = '2';
                crimecasemoney[1].summoney += casenode->money;
            }
            else if(casenode->accusationname == '3')
            {
                crimecasemoney[2].sumcount++;
                crimecasemoney[2].accuname = '3';
                crimecasemoney[2].summoney += casenode->money;
            }
            else if(casenode->accusationname == '4')
            {
                crimecasemoney[3].sumcount++;
                crimecasemoney[3].accuname = '4';
                crimecasemoney[3].summoney += casenode->money;
            }
            else if(casenode->accusationname == '5')
            {
                crimecasemoney[4].sumcount++;
                crimecasemoney[4].accuname = '5';
                crimecasemoney[4].summoney += casenode->money;
            }
            else if(casenode->accusationname == '6')
            {
                crimecasemoney[5].sumcount++;
                crimecasemoney[5].accuname = '6';
                crimecasemoney[5].summoney += casenode->money;
            }
        }
    }


    for(i=0;i<6;i++)
    {
        for(j=0;j<6-i-1;j++)
        {
            if(crimecasemoney[j].sumcount < crimecasemoney[j+1].sumcount)
            {
                crimecasemoneytmp = crimecasemoney[j];
                crimecasemoney[j] = crimecasemoney[j+1];
                crimecasemoney[j+1] = crimecasemoneytmp;
            }
        }
    }

    for(i=0;i<6;i++)
    {
        if((crimecasemoney[i].accuname!='1') &&
            (crimecasemoney[i].accuname!='2') &&
            (crimecasemoney[i].accuname!='3') &&
            (crimecasemoney[i].accuname!='4')&&
            (crimecasemoney[i].accuname!='5') && (crimecasemoney[i].accuname!='6'))
        {
            crimecasemoney[i].accuname = '0';
        }
    }
    LABEL_BUNDLE labels;
	HOT_AREA areas;
	BOOL bRet = TRUE;
	SMALL_RECT srpop;
	COORD pos;
	WORD att;

	char *s1 = "����̰������ͳ��";
	char *s2 = "��Ҫ����        ������    �永�ܽ��(��Ԫ)";

	char s3[100];
	char s4[100];
	char s5[100];
    char s6[100];
	char s7[100];
    char s8[100];

    char *p[6] = {s3,s4,s5,s6,s7,s8};
	for (i=0; i<6; i++)
	{
		switch(crimecasemoney[i].accuname)
		{
			case '1':
				sprintf(p[i],"�ܻ�                %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
				break;

			case '2':
				sprintf(p[i],"̰��                %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
				break;

			case '3':
				sprintf(p[i],"Ų�ù���            %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
				break;

			case '4':
				sprintf(p[i],"��ְ                %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
                break;

			case '5':
				sprintf(p[i],"����ְȨ            %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
				break;

			case '6':
				sprintf(p[i],"����                %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
				break;
            default:
                sprintf(p[i],"------------------------------------");
                break;
		}
	}

	char *pCh[] = {s1,s2,p[0],p[1],p[2],p[3],p[4],p[5],"ȷ��"};
	int iHot = 1;

	pos.X = strlen(pCh[0]) + 30;
	pos.Y = 15;
	srpop.Left = (SCR_COL - pos.X) / 2;
	srpop.Right = srpop.Left + pos.X - 1;
	srpop.Top = (SCR_ROW - pos.Y) / 2;
	srpop.Bottom = srpop.Top + pos.Y - 1;

	att = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;  /*�׵׺���*/
	labels.num = 9;
	labels.ppLabel = pCh;
	COORD aLoc[] = {{srpop.Left+15, srpop.Top+2},//����̰������ͳ��

					{srpop.Left+3, srpop.Top+4},//��Ҫ����

					{srpop.Left+3, srpop.Top+6},
					{srpop.Left+3, srpop.Top+7},
					{srpop.Left+3, srpop.Top+8},
					{srpop.Left+3, srpop.Top+9},
					{srpop.Left+3, srpop.Top+10},
					{srpop.Left+3, srpop.Top+11},

					{srpop.Left+21, srpop.Top+13}};//ȷ��
	labels.pLoc = aLoc;

	areas.num = 1;
	SMALL_RECT aArea[] = {{srpop.Left + 21, srpop.Top + 13,
						   srpop.Left + 24, srpop.Top + 13}};/*��ť������λ��*/
	char aSort[] = {0};
	char aTag[] = {1};
	areas.pArea = aArea;
	areas.pSort = aSort;
	areas.pTag = aTag;
	PopUp(&srpop, att, &labels, &areas);

	pos.X = srpop.Left + 3;
	pos.Y = srpop.Top + 5;
	FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-5, pos, &ul);

	if (DealInput(&areas, &iHot) == 13 && (iHot == 1))
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	PopOff();
	return bRet;
}

/**
 * ��������: StatusLevelPunish
 * ��������: ����������Ϊ�����ֱ�ͳ��ĳ����ڸ�����������̰�������ķ�������
 *           �������´���Ϊ���̡�����ͽ�̡�����ͽ��1-5�ꡢ
 *           ����ͽ��6-10������ͽ��11�����ϵİ����ֱ��Ƕ�����
 * �������: head ����ͷָ��
 * �������: ��
 * �� �� ֵ: void
 * ����˵��:
 */
BOOL StatusLevelPunish(void)
{
    PROVINCE_NODE *hd;
    CASE_NODE *casenode;
    /*��ʼ���ṹ����*/
    int i,j;
    for(i=0; i<8; i++)
    {
        levelcountpunishment[i].sumcount1 = 0;
        for(j=0; j<5; j++)
        {
            levelcountpunishment[i].year[j] = 0;
        }
    }

    for(hd = head;hd != NULL;hd = hd->next)
    {
        for(casenode = hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            if(casenode->level == 'A')
            {
                levelcountpunishment[0].classlevel = 'A';
                levelcountpunishment[0].sumcount1++;
                if(casenode->punishment >=1 && casenode->punishment <=5)
                    levelcountpunishment[0].year[0]++;
                else if(casenode->punishment >=6 && casenode->punishment <= 10)
                    levelcountpunishment[0].year[1]++;
                else if(casenode->punishment >=11 && casenode->punishment <= 99)
                    levelcountpunishment[0].year[2]++;
                else if(casenode->punishment >=100)
                    levelcountpunishment[0].year[3]++;
                else if(casenode->punishment < 0)
                    levelcountpunishment[0].year[4]++;
            }
            else if(casenode->level == 'B')
            {
                levelcountpunishment[1].classlevel = 'B';
                levelcountpunishment[1].sumcount1++;
                if(casenode->punishment >=1 && casenode->punishment <= 5)
                    levelcountpunishment[1].year[0]++;
                else if(casenode->punishment >=6 && casenode->punishment <= 10)
                    levelcountpunishment[1].year[1]++;
                else if(casenode->punishment >=11 && casenode->punishment <= 99)
                    levelcountpunishment[1].year[2]++;
                else if(casenode->punishment >=100)
                    levelcountpunishment[1].year[3]++;
                else if(casenode->punishment < 0)
                    levelcountpunishment[1].year[4]++;
            }
            else if(casenode->level == 'C')
            {
                levelcountpunishment[2].classlevel = 'C';
                levelcountpunishment[2].sumcount1++;
                if(casenode->punishment >=1 && casenode->punishment <= 5)
                    levelcountpunishment[2].year[0]++;
                else if(casenode->punishment >=6 && casenode->punishment <= 10)
                    levelcountpunishment[2].year[1]++;
                else if(casenode->punishment >=11 && casenode->punishment <= 99)
                    levelcountpunishment[2].year[2]++;
                else if(casenode->punishment >=100)
                    levelcountpunishment[2].year[3]++;
                else if(casenode->punishment < 0)
                    levelcountpunishment[2].year[4]++;
            }
            else if(casenode->level == 'D')
            {
                levelcountpunishment[3].classlevel = 'D';
                levelcountpunishment[3].sumcount1++;
                if(casenode->punishment >=1 && casenode->punishment <= 5)
                    levelcountpunishment[3].year[0]++;
                else if(casenode->punishment >=6 && casenode->punishment <= 10)
                    levelcountpunishment[3].year[1]++;
                else if(casenode->punishment >=11 && casenode->punishment <= 99)
                    levelcountpunishment[3].year[2]++;
                else if(casenode->punishment >=100)
                    levelcountpunishment[3].year[3]++;
                else if(casenode->punishment < 0)
                    levelcountpunishment[3].year[4]++;
            }
            else if(casenode->level == 'E')
            {
                levelcountpunishment[4].classlevel = 'E';
                levelcountpunishment[4].sumcount1++;
                if(casenode->punishment >=1 && casenode->punishment <= 5)
                    levelcountpunishment[4].year[0]++;
                else if(casenode->punishment >=6 && casenode->punishment <= 10)
                    levelcountpunishment[4].year[1]++;
                else if(casenode->punishment >=11 && casenode->punishment <= 99)
                    levelcountpunishment[4].year[2]++;
                else if(casenode->punishment >=100)
                    levelcountpunishment[4].year[3]++;
                else if(casenode->punishment < 0)
                    levelcountpunishment[4].year[4]++;
            }
            else if(casenode->level == 'F')
            {
                levelcountpunishment[5].classlevel = 'F';
                levelcountpunishment[5].sumcount1++;
                if(casenode->punishment >=1 && casenode->punishment <= 5)
                    levelcountpunishment[5].year[0]++;
                else if(casenode->punishment >=6 && casenode->punishment <= 10)
                    levelcountpunishment[5].year[1]++;
                else if(casenode->punishment >=11 && casenode->punishment <= 99)
                    levelcountpunishment[5].year[2]++;
                else if(casenode->punishment >=100)
                    levelcountpunishment[5].year[3]++;
                else if(casenode->punishment < 0)
                    levelcountpunishment[5].year[4]++;
            }
            else if(casenode->level == 'G')
            {
                levelcountpunishment[6].classlevel = 'G';
                levelcountpunishment[6].sumcount1++;
                if(casenode->punishment >=1 && casenode->punishment <= 5)
                    levelcountpunishment[6].year[0]++;
                else if(casenode->punishment >=6 && casenode->punishment <= 10)
                    levelcountpunishment[6].year[1]++;
                else if(casenode->punishment >=11 && casenode->punishment <= 99)
                    levelcountpunishment[6].year[2]++;
                else if(casenode->punishment >=100)
                    levelcountpunishment[6].year[3]++;
                else if(casenode->punishment < 0)
                    levelcountpunishment[6].year[4]++;
            }
            else if(casenode->level == 'H')
            {
                levelcountpunishment[7].classlevel = 'H';
                levelcountpunishment[7].sumcount1++;
                if(casenode->punishment >=1 && casenode->punishment <= 5)
                    levelcountpunishment[7].year[0]++;
                else if(casenode->punishment >=6 && casenode->punishment <= 10)
                    levelcountpunishment[7].year[1]++;
                else if(casenode->punishment >=11 && casenode->punishment <= 99)
                    levelcountpunishment[7].year[2]++;
                else if(casenode->punishment >=100)
                    levelcountpunishment[7].year[3]++;
                else if(casenode->punishment < 0)
                    levelcountpunishment[7].year[4]++;
            }
        }
    }

    LABEL_BUNDLE labels;
	HOT_AREA areas;
	BOOL bRet = TRUE;
	SMALL_RECT srpop;
	COORD pos;
	WORD att;

	char *s1 = "2016�����������̰�������������´���ͳ��";
	char *s2 = "��������       1-5��   6-10��   11������   ����ͽ��   ����";

	char s3[100];
	char s4[100];
	char s5[100];
    char s6[100];
	char s7[100];
    char s8[100];
	char s9[100];
	char s10[100];

	char *p[8] = {s3,s4,s5,s6,s7,s8,s9,s10};
	for (i=0; i<8; i++)
	{
		switch(levelcountpunishment[i].classlevel)
		{
			case 'A':
				sprintf(p[i],"���Ƽ�             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;

			case 'B':
				sprintf(p[i],"���Ƽ�             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;

			case 'C':
				sprintf(p[i],"������             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;

			case 'D':
				sprintf(p[i],"������             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;
			case 'E':
				sprintf(p[i],"������             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;

			case 'F':
				sprintf(p[i],"������             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;

			case 'G':
				sprintf(p[i],"������             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;
			case 'H':
				sprintf(p[i],"������������       %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;
            default:
                sprintf(p[i],"----------------------------------------------------------");
                break;
		}
	}

	char *pCh[] = {s1,s2,p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],"ȷ��"};
	int iHot = 1;

	pos.X = strlen(pCh[0]) + 24;
	pos.Y = 17;
	srpop.Left = (SCR_COL - pos.X) / 2;
	srpop.Right = srpop.Left + pos.X - 1;
	srpop.Top = (SCR_ROW - pos.Y) / 2;
	srpop.Bottom = srpop.Top + pos.Y - 1;

	att = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;  /*�׵׺���*/
	labels.num = 11;
	labels.ppLabel = pCh;
	COORD aLoc[] = {{srpop.Left+12, srpop.Top+2},//����̰������ͳ��

					{srpop.Left+3, srpop.Top+4},

					{srpop.Left+3, srpop.Top+6},
					{srpop.Left+3, srpop.Top+7},
					{srpop.Left+3, srpop.Top+8},
					{srpop.Left+3, srpop.Top+9},
					{srpop.Left+3, srpop.Top+10},
					{srpop.Left+3, srpop.Top+11},
					{srpop.Left+3, srpop.Top+12},
					{srpop.Left+3, srpop.Top+13},

					{srpop.Left+30, srpop.Top+15}};//ȷ��
	labels.pLoc = aLoc;

	areas.num = 1;
	SMALL_RECT aArea[] = {{srpop.Left + 30, srpop.Top + 15,
						   srpop.Left + 33, srpop.Top + 15}};/*��ť������λ��*/
	char aSort[] = {0};
	char aTag[] = {1};
	areas.pArea = aArea;
	areas.pSort = aSort;
	areas.pTag = aTag;
	PopUp(&srpop, att, &labels, &areas);

	pos.X = srpop.Left + 3;
	pos.Y = srpop.Top + 5;
	FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-5, pos, &ul);

	if (DealInput(&areas, &iHot) == 13 && (iHot == 1))
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	PopOff();
	return bRet;

}

/**
 * ��������: StatusProUI
 * ��������: ��ʾ��ѯ���ڵĽ���
 *
 * �������:
 * �������: ��
 * �� �� ֵ: ��ΪTRUE
 * ����˵��:
 */
BOOL StatusProUI(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"��������ʼ����",
                   "          ",
                   "�������ֹ����",
                   "          ",
                   "ȷ��","ȡ��"};
    int iHot = 1;

    pos.X = strlen(pCh[2]) + 6;
    pos.Y = 12;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
    labels.num = 6;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+5, srpop.Top+4},
                    {srpop.Left+3, srpop.Top+6},
                    {srpop.Left+5, srpop.Top+8},
                    {srpop.Left+5, srpop.Top+10},
                    {srpop.Left+12, srpop.Top+10}};
    labels.pLoc = aLoc;

    areas.num = 4;
    SMALL_RECT aArea[] = {{srpop.Left + 5, srpop.Top + 4,
                           srpop.Left + 15, srpop.Top + 4},
                          {srpop.Left + 5, srpop.Top + 8,
                           srpop.Left + 15, srpop.Top + 8},
                          {srpop.Left + 5, srpop.Top + 10,
                           srpop.Left + 8,srpop.Top + 10},
                          {srpop.Left + 12, srpop.Top + 10,
                           srpop.Left + 15, srpop.Top + 10}};/*�ĸ�������λ��*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*����¼��ʡ����Ϣ�ĶԻ���*/
    /**< ��'-'������� */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+9;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<���´���Ի�������� */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char startdate[10];
    char enddate[10];
    char *pc1 = startdate;
    char *pc2 = enddate;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // ���ù��λ��
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //��Ϣ�������򱳾�ɫ
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* ��ESC���˳�ѭ��*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return TRUE;
            }

			/*backspace���Ĵ���*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1 || iHot==2))
            {
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                if(iHot==1)
                {
                    *(pc1--) = c;
                }
                else if(iHot==2)
                {
                    *(pc2--) = c;
                }
            }

            /*enter ���Ĵ���*/
            else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) &&
                     (keyRec.Event.KeyEvent.bKeyDown) &&
                     (iHot == 1 || iHot == 2 || iHot == 3 || iHot == 4))
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                if(iHot==3)
                {
                    *pc1 = '\0';
                    *pc2 = '\0';
                    if(strlen(startdate)!=0 && strlen(enddate)!=0)
                    {
                        PopOff();
                        StatusProvince(startdate,enddate);
                        return TRUE;
                    }
                    else if(strlen(startdate)==0 || strlen(enddate)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"���ڲ���Ϊ�գ�¼��ʧ�ܣ�","ȷ��"};
                        ShowModule(plabel_name, 2);
                        PopOff();
                        return TRUE;
                    }

                }
                else if(iHot==4)
                {
                    PopOff();
                    return TRUE;
                }
            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*�������¼�*/
                {
                    switch (vkc)
                    {
                        case 37:
                        case 38:
                            iHot--;
                            if(iHot<1)
                            {
                                iHot = 4;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                        case 39:
                        case 40:
                            iHot++;
                            if(iHot>4)
                            {
                                iHot = 1;
                            }
                            SetHotPoint(&areas,iHot);
                            break;
                    }
                    if(iHot==3 || iHot==4)
                    {
                        GetConsoleCursorInfo(std_out,&lpCur);
                        lpCur.bVisible = FALSE;
                        SetConsoleCursorInfo(std_out,&lpCur);
                    }
                }
            }
            if ((keyRec.Event.KeyEvent.bKeyDown) &&
                 (pos.X<=aArea[0].Left+20) && (iHot==1 || iHot==2))
			{
				ch = keyRec.Event.KeyEvent.uChar.AsciiChar;
                if(ch>=0x20 && ch<=0x7e)
                {
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // ��ȡ���λ��
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //��ʾ������ַ�
                    if(iHot==1)
                    {
                        *(pc1++) = ch;
                    }
                    else if(iHot==2)
                    {
                        *(pc2++) = ch;
                    }
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * ��������: StatusProvince
 * ��������: �԰�������������ʡ��Ϊ�����ֱ�ͳ��ĳ��ʱ����ڸ�ʡ��̰�������ķ�������
 *            �永���������������������������������ͳ�ƽ����
 * �������: ��ʼ�������ֹ����
 * �������: ��
 * �� �� ֵ: void
 * ����˵��:
 */
BOOL StatusProvince(char *startdate,char *enddate)
{
    PROVINCE_NODE *hd;
    CASE_NODE *casenode;
    int i = 0,j = 0,cnt = 0;
    for(hd = head;hd != NULL;hd = hd->next)
    {
       cnt++;
    }
    int num1 = atoi(startdate);
    int num2 = atoi(enddate);
    struct PROVINCECOUNTMONEY provincecountmoney[cnt];
    for(i=0; i<cnt; i++)
    {
        provincecountmoney[i].sumcount2 = 0;
        provincecountmoney[i].summoney1 = 0.0;
    }
    i = 0;
    for(hd = head;hd != NULL;hd = hd->next)
    {
        strcpy(provincecountmoney[i].proname,hd->pvblg);
        for(casenode = hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            int num3 = atoi(casenode->casedate);
            if(num1 <= num3 && num2 >= num3)
            {
                provincecountmoney[i].sumcount2 ++;
                provincecountmoney[i].summoney1 += casenode->money;
            }
        }
        i++;
    }
    for(i=0;i<cnt;i++)
    {
        for(j=0;j<cnt-i-1;j++)
        {
            if(provincecountmoney[j].sumcount2 < provincecountmoney[j+1].sumcount2)
            {
                provincecountmoneytmp = provincecountmoney[j];
                provincecountmoney[j] = provincecountmoney[j+1];
                provincecountmoney[j+1] = provincecountmoneytmp;
            }
        }
    }

    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
	int iHot = 1;
    int labelnum = cnt+3;
    char str[labelnum][100];
    char *p[labelnum];

    for(i=0; i<labelnum; i++)
    {
        p[i] = str[i];
    }
    sprintf(p[0],"%s--%s��ʡ��̰������ͳ��",startdate,enddate);
    p[1] = "ʡ��          ������      �永�ܽ��";
    p[labelnum-1] = "ȷ��";
    for (i=2; i<cnt+2; i++)
    {
        sprintf(p[i],"%-16s%-12d%.3f",
                provincecountmoney[i-2].proname,provincecountmoney[i-2].sumcount2,
                provincecountmoney[i-2].summoney1);
    }
    char *pCh[labelnum];
    for(i=0; i<labelnum; i++)
    {
        pCh[i] = p[i];
    }

	pos.X = strlen(pCh[0]) + 8;
	pos.Y = 9+cnt;
	srpop.Left = (SCR_COL - pos.X) / 2;
	srpop.Right = srpop.Left + pos.X - 1;
	srpop.Top = (SCR_ROW - pos.Y) / 2;
	srpop.Bottom = srpop.Top + pos.Y - 1;

	att = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;  //�׵׺���
	labels.num = labelnum;
    labels.ppLabel = pCh;

    COORD aLoc[labelnum];
    aLoc[0].X = srpop.Left+4;
    aLoc[0].Y = srpop.Top+2;
    aLoc[1].X = srpop.Left+3;
    aLoc[1].Y = srpop.Top+4;
    aLoc[labelnum-1].X = srpop.Left+18;
    aLoc[labelnum-1].Y = srpop.Top+7+cnt;

    for(i=2; i<cnt+2; i++)
    {
        aLoc[i].X = srpop.Left+3;
        aLoc[i].Y = srpop.Top+4+i;
    }
	labels.pLoc = aLoc;

	areas.num = 1;
	SMALL_RECT aArea[] = {{srpop.Left + 18, srpop.Top+7+cnt,
						    srpop.Left + 21, srpop.Top+7+cnt}};//��ť������λ��
	char aSort[] = {0};
	char aTag[] = {1};
	areas.pArea = aArea;
	areas.pSort = aSort;
	areas.pTag = aTag;
	PopUp(&srpop, att, &labels, &areas);

	pos.X = srpop.Left + 3;
	pos.Y = srpop.Top + 5;
	FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-5, pos, &ul);

	if (DealInput(&areas, &iHot) == 13 && (iHot == 1))
	{
		PopOff();
	}

	return TRUE;
}

 /**
 * ��������: StatusReportTime
 * ��������: ͳ��ý�屨��������10��̰����������������������������������š�
 *           ��������������Ҫ�������永�����´�����ý�屨��������
 * �������: head ����ͷָ��
 * �������: ��
 * �� �� ֵ: void
 * ����˵��:
 */
BOOL StatusReportTime(void)
{
    PROVINCE_NODE *hd;
    CASE_NODE *casenode;
    MEDIA_NODE *medianode;
    int i,j = 0;
    int sum = 0;
    for(hd = head;hd != NULL;hd = hd->next)
    {
        for(casenode = hd->cnext;casenode != NULL;casenode = casenode->next)
        {
            sum++;                                             /*�õ��ܰ�����*/
        }
    }
    struct MEDIAMOST mediamost[10];
    struct MEDIAMOST realcase[sum];
    /*��ʼ��ͳ�ƽڵ�*/
    for(i=0;i<sum;i++)
    {
        realcase[i].reportcount = 0;
        realcase[i].money1 = 0.0;
    }
    i=0;

    for(hd=head; hd!=NULL; hd=hd->next)
    {
        for(casenode=hd->cnext; casenode!=NULL; casenode=casenode->next)
        {
            for(medianode=casenode->mnext; medianode!=NULL; medianode=medianode->next)
            {
                realcase[i].reportcount++;
            }
            strcpy(realcase[i].casenum1,casenode->casenum);
            realcase[i].classlevel1 = casenode->level;
            realcase[i].accuname1 = casenode->accusationname;
            realcase[i].money1 = casenode->money;
            realcase[i].punish = casenode->punishment;
            i++;
        }
    }

    for(i=0;i<sum;i++)
    {
        for(j=0;j<sum-i-1;j++)
        {
            if(realcase[j].reportcount < realcase[j+1].reportcount)
            {
                mediamosttmp = realcase[j];
                realcase[j] = realcase[j+1];
                realcase[j+1] = mediamosttmp;
            }
        }
    }
    if(sum<=10)
    {
        for(i=0; i<sum; i++)
        {
            mediamost[i] = realcase[i];
        }
        for(i=sum; i<10; i++)
        {
            mediamost[i].classlevel1 = 'P';/*��һ��'A'~'H'�޹ص���ĸ*/
        }
    }
    else if(sum>10)
    {
        for(i=0; i<10 ;i++)
        {
            mediamost[i] = realcase[i];
        }
    }


    LABEL_BUNDLE labels;
	HOT_AREA areas;
	BOOL bRet = TRUE;
	SMALL_RECT srpop;
	COORD pos;
	WORD att;

    char str[10][100];
	char *p[10];
	for(i=0; i<10; i++)
    {
        p[i] = str[i];
    }

    char *line = "--------------------------------------------------------------------";

	for(i=0; i<10; i++)
	{
		switch(mediamost[i].classlevel1)
		{
			case 'A':
				switch(mediamost[i].accuname1)
				{

					case '1':
						sprintf(p[i],"%s       ���Ƽ�      �ܻ�     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       ���Ƽ�      ̰��     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       ���Ƽ�  Ų�ù���     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       ���Ƽ�      ��ְ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       ���Ƽ�  ����ְȨ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       ���Ƽ�      ����     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'B':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       ���Ƽ�      �ܻ�     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       ���Ƽ�      ̰��     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       ���Ƽ�  Ų�ù���     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       ���Ƽ�      ��ְ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       ���Ƽ�  ����ְȨ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       ���Ƽ�      ����     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'C':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       ������      �ܻ�     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       ������      ̰��     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       ������  Ų�ù���     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       ������      ��ְ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       ������    ����ְȨ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       ������      ����     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'D':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       ������      �ܻ�     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       ������      ̰��     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       ������  Ų�ù���     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       ������      ��ְ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       ������  ����ְȨ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       ������      ����     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'E':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       ������      �ܻ�     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       ������      ̰��     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       ������  Ų�ù���     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       ������      ��ְ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       ������  ����ְȨ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       ������      ����     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);

						break;
				}
				break;
			case 'F':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       ������      �ܻ�     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       ������      ̰��     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       ������  Ų�ù���     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       ������      ��ְ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       ������  ����ְȨ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       ������      ����     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'G':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       ������      �ܻ�     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       ������      ̰��     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       ������  Ų�ù���     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       ������      ��ְ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       ������  ����ְȨ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       ������      ����     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'H':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       ������      �ܻ�     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       ������      ̰��     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       ������  Ų�ù���     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       ������      ��ְ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       ������  ����ְȨ     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       ������      ����     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
            default :
                sprintf(p[i],"%s",line);
                break;
        }
	}


	char *pCh[] = {"ý�屨������ǰʮ��̰��������ϸ��Ϣ"," �������       ��������   ��Ҫ����  �永���   ���´���   ��������",
                    p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],"ȷ��"};
	int iHot = 1;

	pos.X = strlen(pCh[0]) + 40;
	pos.Y = 19;
	srpop.Left = (SCR_COL - pos.X) / 2;
	srpop.Right = srpop.Left + pos.X - 1;
	srpop.Top = (SCR_ROW - pos.Y) / 2;
	srpop.Bottom = srpop.Top + pos.Y - 1;

	att = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;  //�׵׺���
	labels.num = 13;
	labels.ppLabel = pCh;
	COORD aLoc[] = {{srpop.Left+20, srpop.Top+2},//����̰������ͳ��

					{srpop.Left+3, srpop.Top+4},

					{srpop.Left+3, srpop.Top+6},
					{srpop.Left+3, srpop.Top+7},
					{srpop.Left+3, srpop.Top+8},
					{srpop.Left+3, srpop.Top+9},
					{srpop.Left+3, srpop.Top+10},
					{srpop.Left+3, srpop.Top+11},
					{srpop.Left+3, srpop.Top+12},
					{srpop.Left+3, srpop.Top+13},
					{srpop.Left+3, srpop.Top+14},
					{srpop.Left+3, srpop.Top+15},

					{srpop.Left+34, srpop.Top+17}};//ȷ��
	labels.pLoc = aLoc;

	areas.num = 1;
	SMALL_RECT aArea[] = {{srpop.Left + 34, srpop.Top + 17,
						    srpop.Left + 37, srpop.Top + 17}};//��ť������λ��
	char aSort[] = {0};
	char aTag[] = {1};
	areas.pArea = aArea;
	areas.pSort = aSort;
	areas.pTag = aTag;
	PopUp(&srpop, att, &labels, &areas);

	pos.X = srpop.Left + 3;
	pos.Y = srpop.Top + 5;
	FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-5, pos, &ul);

	if (DealInput(&areas, &iHot) == 13 && (iHot == 1))
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	PopOff();
	return bRet;
}

/**
 * ��������: StatusReportType
 * ��������: �������ܺ�����Ӧ�Ľ���
 * �������:
 * �������: ��
 * �� �� ֵ: BOOL����, ��ΪTRUE
 *
 * ����˵��:
 */
BOOL StatusReportType(void)
{
    PROVINCE_NODE *hd = head;
    CASE_NODE *casenode;
    MEDIA_NODE *medianode;

    int i,j;
    for(i=0; i<4; i++)
    {
        statusmediatype[i].cnt = 0;
    }
    for(;hd!=NULL;hd = hd->next)
    {
        for(casenode=hd->cnext;casenode!=NULL;casenode=casenode->next)
        {
            for(medianode=casenode->mnext;medianode!=NULL;medianode=medianode->next)
            {
                switch(medianode->mediatype)
                {
                    case '1':
                        statusmediatype[0].mediatype = '1';
                        statusmediatype[0].cnt++;
                        break;
                    case '2':
                        statusmediatype[1].mediatype = '2';
                        statusmediatype[1].cnt++;
                        break;
                    case '3':
                        statusmediatype[2].mediatype = '3';
                        statusmediatype[2].cnt++;
                        break;
                    case '4':
                        statusmediatype[3].mediatype = '4';
                        statusmediatype[3].cnt++;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    for(i=0; i<4; i++)
    {
        for(j=0; j<3-i; j++)
        {
            if(statusmediatype[j].cnt < statusmediatype[j+1].cnt)
            {
                statusmediatypetmp = statusmediatype[j];
                statusmediatype[j] = statusmediatype[j+1];
                statusmediatype[j+1] = statusmediatypetmp;
            }
        }
    }

    for(i=0;i<4;i++)
    {
        if(statusmediatype[i].mediatype!='1' && statusmediatype[i].mediatype!='2'
           && statusmediatype[i].mediatype!='3' && statusmediatype[i].mediatype!='4')
        {
            statusmediatype[i].mediatype = '9';
        }
    }

    LABEL_BUNDLE labels;
	HOT_AREA areas;
	BOOL bRet = TRUE;
	SMALL_RECT srpop;
	COORD pos;
	WORD att;

	char *s1 = "̰����������ͳ��";
	char *s2 = "ý�����       �ܱ�������";

	char s3[100];
	char s4[100];
	char s5[100];
    char s6[100];

	char *p[4] = {s3,s4,s5,s6};
    for(i=0; i<4; i++)
    {
        switch(statusmediatype[i].mediatype)
        {
            case '1':
                sprintf(p[i],"��ֽ              %d",statusmediatype[i].cnt);
                break;
            case '2':
                sprintf(p[i],"�㲥              %d",statusmediatype[i].cnt);
                break;
            case '3':
                sprintf(p[i],"����              %d",statusmediatype[i].cnt);
                break;
            case '4':
                sprintf(p[i],"������            %d",statusmediatype[i].cnt);
                break;
            default:
                sprintf(p[i],"--------------------");
        }
    }

	char *pCh[] = {s1,s2,p[0],p[1],p[2],p[3],"ȷ��"};
	int iHot = 1;

	pos.X = strlen(pCh[0]) + 14;
	pos.Y = 13;
	srpop.Left = (SCR_COL - pos.X) / 2;
	srpop.Right = srpop.Left + pos.X - 1;
	srpop.Top = (SCR_ROW - pos.Y) / 2;
	srpop.Bottom = srpop.Top + pos.Y - 1;

	att = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;  /*�׵׺���*/
	labels.num = 7;
	labels.ppLabel = pCh;
	COORD aLoc[] = {{srpop.Left+7, srpop.Top+2},//����̰������ͳ��

					{srpop.Left+3, srpop.Top+4},

					{srpop.Left+5, srpop.Top+6},
					{srpop.Left+5, srpop.Top+7},
					{srpop.Left+5, srpop.Top+8},
					{srpop.Left+5, srpop.Top+9},

					{srpop.Left+13, srpop.Top+11}};//ȷ��
	labels.pLoc = aLoc;

	areas.num = 1;
	SMALL_RECT aArea[] = {{srpop.Left + 13, srpop.Top + 11,
						   srpop.Left + 16, srpop.Top + 11}};/*��ť������λ��*/
	char aSort[] = {0};
	char aTag[] = {1};
	areas.pArea = aArea;
	areas.pSort = aSort;
	areas.pTag = aTag;
	PopUp(&srpop, att, &labels, &areas);

	pos.X = srpop.Left + 3;
	pos.Y = srpop.Top + 5;
	FillConsoleOutputCharacter(std_out, '-', srpop.Right-srpop.Left-5, pos, &ul);

	if (DealInput(&areas, &iHot) == 13 && (iHot == 1))
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	PopOff();
	return bRet;
}

/**
 * ��������: ExeFunction
 * ��������: ִ�������˵��ź��Ӳ˵���ȷ���Ĺ��ܺ���.
 * �������: m ���˵����
 *           s �Ӳ˵����
 * �������: ��
 * �� �� ֵ: BOOL����, TRUE �� FALSE
 *
 * ����˵��: ����ִ�к���ExitSysʱ, �ſ��ܷ���FALSE, ������������Ƿ���TRUE
 */
BOOL ExeFunction(int m, int s)
{
    BOOL bo;
    int i,loc;
    BOOL(*pfun[sub_menu_num[0]+sub_menu_num[1]+sub_menu_num[2]+sub_menu_num[3]+sub_menu_num[4]])(void);
    pfun[0] = FileDataSave;
    pfun[1] = FileDataBackUp;
    pfun[2] = FileDataRestore_Interface;
    pfun[3] = FileExit;
    pfun[4] = MaintainProInfo;
    pfun[5] = MaintainCaseInfo;
    pfun[6] = MaintainMediaInfo;
    pfun[7] = QueryProInfo;
    pfun[8] = QueryCaseInfo;
    pfun[9] = QueryMediaInfo;
    pfun[10] = StatusCrimeName;
    pfun[11] = StatusLevelPunish;
    pfun[12] = StatusProUI;
    pfun[13] = StatusReportTime;
    pfun[14] = StatusReportType;
    pfun[15] = HelpTheme;
    pfun[16] = HelpAbout;

    for (i=1,loc=0; i<m; i++)  /*�������˵��ź��Ӳ˵��ż����Ӧ�±�*/
    {
        loc += sub_menu_num[i-1];
    }
    loc += s - 1;

    if (pfun[loc] != NULL)
    {
        bo = (*pfun[loc])();  /*�ú���ָ�������ָ��Ĺ��ܺ���*/
    }
    return bo;
}
