#include "tes.h"
int main()
{
    COORD size = {SCR_COL, SCR_ROW};                /*窗口缓冲区大小*/

    std_out = GetStdHandle(STD_OUTPUT_HANDLE);      /* 获取标准输出设备句柄*/
    std_in = GetStdHandle(STD_INPUT_HANDLE);        /* 获取标准输入设备句柄*/

    SetConsoleTitle(sysname);                       /*设置窗口标题*/
    SetConsoleScreenBufferSize(std_out, size);      /*设置窗口缓冲区大小80*25*/

    LoadData();
    InInterface();
    RunSystem();
    CloseSystem();

    return 0;
}

/**
 * 函数名称: LoadData
 * 函数功能: 将代码表和三类基础数据从数据文件载入到内存缓冲区和十字链表中.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: BOOL类型, 功能函数中除了函数ExitSys的返回值可以为FALSE外,
 *           其他函数的返回值必须为TRUE.
 *
 * 调用说明:
 */
BOOL LoadData()
{
    int Re;
    Re = CreateList(&head);
    sys_state |= Re;
    sys_state &= ~(4 + 8 + 16 - Re);
    if (sys_state < (4 | 8 | 16))
    {  /*数据加载提示信息*/
        printf("\n系统基础数据不完整!\n");
        printf("\n按任意键继续...\n");
        getch();
    }

    return TRUE;
}

/**
 * 函数名称: CreateList
 * 函数功能: 从数据文件读取基础数据, 并存放到所创建的十字链表中.
 * 输入参数: 无
 * 输出参数: phead 主链头指针的地址, 用来返回所创建的十字链.
 * 返 回 值: int型数值, 表示链表创建的情况.
 *           0  空链, 无数据
 *           4  已加载省份信息数据，无案件信息和媒体信息数据
 *           12 已加载案件信息数据，无媒体信息数据
 *           28 三类基础数据都已加载
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
        printf("省份信息文件打开失败！\n");
        return re;
    }
    printf("省份信息文件打开成功！\n");
    while(fscanf(pf,"%s %s %s\n",struct1.pvblg,struct1.disciplineperson,struct1.tele)==3)
    {
        pPROVINCENODE = (PROVINCE_NODE *)malloc(sizeof(PROVINCE_NODE));
        *pPROVINCENODE = struct1;
        pPROVINCENODE->cnext = NULL;
        pPROVINCENODE->next = prohead;                 /*创建后进先出的链表*/
        prohead = pPROVINCENODE;                       /*创建后进先出的链表*/
    }
    fclose(pf);
    if(prohead == NULL)
    {
        printf("省份信息加载失败！\n");
        return re;
    }
    printf("省份信息加载成功！\n");
    *phead = prohead;                                  /*三方向十字交叉链表表头*/

    fclose(pf);
    re += 4;

    if((pf = fopen(caseinfofile,"r")) == NULL)
    {
        printf("案件信息打开失败！\n");
        return re;
    }
    printf("案件信息打开成功！\n");
    re += 8;

    /*从数据文件读取案件信息数据，存入主链对应节点的案件信息支链中*/
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
            pCASENODE->next = pPROVINCENODE->cnext;         /*创建后进先出的链表*/
            pPROVINCENODE->cnext = pCASENODE;              /*创建后进先出的链表*/
        }
        else
        {
            free(pCASENODE);
        }
    }
    fclose(pf);
    if((pf = fopen(mediainfofile,"r")) == NULL)
    {
        printf("媒体报道信息打开失败!\n");
        return re;
    }
    printf("媒体报道信息打开成功！\n");
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
 * 函数名称: RunSystem
 * 函数功能: 运行系统, 在系统主界面下运行用户所选择的功能模块.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void RunSystem(void)
{
    INPUT_RECORD input;
    DWORD dword;
    int num= 0;/*子菜单位置*/
    int i;
    int tmp;/*子菜单位置*/
    char vkc,asc;
    INPUT_RECORD inRec;

    while(1)
    {
        ReadConsoleInput(std_in,&input,1,&dword);
        vkc = input.Event.KeyEvent.wVirtualKeyCode;
        asc = input.Event.KeyEvent.uChar.AsciiChar;
        if(input.EventType == KEY_EVENT && input.Event.KeyEvent.bKeyDown == TRUE)
        {
            /*系统快捷键的处理*/
            if (vkc == 67) /*如果按下C键*/
            {
                if (layhead->LayerNo != 0) /*如果当前有子菜单弹出*/
                {
                    PopOff();            /*关闭弹出的子菜单*/
                    selsubmenu = 0; /*将选中子菜单项的项号置为0*/
                }
                ExeFunction(5, 1);  /*运行帮助主题功能函数*/
            }
            else if (inRec.Event.KeyEvent.dwControlKeyState
                     & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
            { /*如果按下左或右Alt键*/
                switch (vkc)  /*判断组合键Ctrl+字母*/
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
            else if(asc == 0)/*对非数字字母键的处理*/
            {
                if(layhead->LayerNo == 0)/*如果未弹出子菜单*/

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
                else /*已弹出子菜单时*/
                {
                    for(num=0,i=1; i<selmainmenu; i++)
                    {
                        /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
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
            {  /*按下普通键*/
                if (layhead->LayerNo == 0)  /*如果未弹出子菜单*/
                {
                    switch (vkc)
                    {
                        case 70: /*f或F*/
                            PopMenu(1);
                            break;
                        case 77: /*m或M*/
                            PopMenu(2);
                            break;
                        case 81: /*q或Q*/
                            PopMenu(3);
                            break;
                        case 83: /*s或S*/
                            PopMenu(4);
                            break;
                        case 72: /*h或H*/
                            PopMenu(5);
                            break;
                        case 13: /*回车*/
                            PopMenu(selmainmenu);
                            TagSubMenu(1);
                            break;
                    }
                }
                else /*已弹出子菜单时的键盘输入处理*/
                {
                    if (vkc == 27) /*如果按下ESC键*/
                    {
                        PopOff();
                        selsubmenu = 0;
                    }
                    else if(vkc == 13) /*如果按下回车键*/
                    {
                        tmp = selsubmenu;
                        PopOff();
                        selsubmenu = 0;
                        ExeFunction(selmainmenu, tmp);

                    }
                    else /*其他普通键的处理*/
                    {
                        /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
                        for (num=0,i=1; i<selmainmenu; i++)
                        {
                            num += sub_menu_num[i-1];
                        }

                        /*依次与当前子菜单中每一项的代表字符进行比较*/
                        for (i=num; i<num+sub_menu_num[selmainmenu-1]; i++)
                        {
                            if (strlen(sub_menu[i])>0 && vkc==sub_menu[i][1])
                            { /*如果匹配成功*/
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
 * 函数名称: CloseSystem
 * 函数功能: 关闭系统
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
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

    /*清屏*/
    ClearScreen();

    /*释放存放菜单条、状态条的信息动态存储区*/
    free(menubar);
    free(statebar);

    /*关闭标准输入和输出设备句柄*/
    CloseHandle(std_out);
    CloseHandle(std_in);

    /*将窗口标题栏置为运行结束*/
    SetConsoleTitle("运行结束");
}

/**
 * 函数名称: InInterface
 * 函数功能: 初始化界面.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
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
 * 函数名称: ClearScreen
 * 函数功能: 清除屏幕信息.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void ClearScreen(void)
{
    CONSOLE_SCREEN_BUFFER_INFO scrbuf;
    COORD home = {0, 0};
    unsigned long size;

    GetConsoleScreenBufferInfo(std_out, &scrbuf );/*取屏幕缓冲区信息*/
    size =  scrbuf.dwSize.X * scrbuf.dwSize.Y; /*计算屏幕缓冲区字符单元数*/

    /*将屏幕缓冲区所有单元的字符属性设置为当前屏幕缓冲区字符属性*/
    FillConsoleOutputAttribute(std_out, scrbuf.wAttributes, size, home,&ul);

    /*将屏幕缓冲区所有单元填充为空格字符*/
    FillConsoleOutputCharacter(std_out, ' ', size, home, &ul);
}

/**
 * 函数名称: ShowMenu
 * 函数功能: 在屏幕上显示主菜单, 并设置热区, 在主菜单第一项上置选中标记.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void ShowMenu(void)
{
    int i;
    CONSOLE_SCREEN_BUFFER_INFO scrbuf;
    CONSOLE_CURSOR_INFO curinfo;
    COORD size,startpos = {0,0},endpos = {0,4};
    GetConsoleScreenBufferInfo(std_out,&scrbuf);        /*先获得屏幕缓冲区的信息*/
    size.X = scrbuf.dwSize.X;                           /*主菜单栏的size*/
    size.Y = 1;
    SetConsoleCursorPosition(std_out,startpos);         /*设置光标初位置*/

    for(i=0;i<M;i++)                                    /*输出主菜单栏的选项信息*/
    {
        printf("  %s  ",main_menu[i]);
    }
    GetConsoleCursorInfo(std_out,&curinfo);             /*光标移动后，获取光标位置信息*/
    curinfo.bVisible = FALSE;
    SetConsoleCursorInfo(std_out,&curinfo);             /*隐藏光标*/
    /*申请菜单栏屏幕区字符信息缓冲区*/
    menubar = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    /*读入数据到菜单栏屏幕区字符信息缓冲区*/
    SMALL_RECT srmenu = {0,0,size.X,0};
    ReadConsoleOutput(std_out,menubar,size,startpos,&srmenu);

    for(i=0;i<M;i++)
    {
        /*设置菜单栏颜色信息*/
        (menubar+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
        /*char ch = (char)((menubar+i)->Char.AsciiChar);
        (menubar+i)->Attributes |= FOREGROUND_RED;*/
    }

    /*将修改后的菜单栏字符信息写入屏幕第一行*/
    WriteConsoleOutput(std_out,menubar,size,startpos,&srmenu);
    SetConsoleCursorPosition(std_out,endpos);           /*最后将鼠标放置在第一行第五列*/
    TagMainMenu(1);
}

/**
 * 函数名称: ShowState
 * 函数功能: 显示状态条.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明: 状态条字符属性为白底黑字, 初始状态无状态信息.
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
        (statebar+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;/*白底黑字*/
    WriteConsoleOutput(std_out, statebar, size, pos, &srmenu);
}

/**
 * 函数名称: TagMainMenu
 * 函数功能: 在指定主菜单项上置选中标志.
 * 输入参数: num 选中的主菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
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
    size.X = scrbuf.dwSize.X;                         /*获取屏幕的宽度*/
    size.Y = 1;

    /*去除选中菜单项之前的菜单项的标记*/
    for(i=0;i<posa;i++)
    {
        (menubar+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    }

    /*去除选中菜单后的菜单项的标记*/
    for(i=posb;i<scrbuf.dwSize.X;i++)
    {
        (menubar+i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
    }

    /*在选中的菜单上做标记，黑底白字*/
    for(i=posa;i<posb;i++)
    {
        (menubar+i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }

    /*将标记好的菜单条信息写到窗口第一行*/
    SMALL_RECT srmenu = {0,0,size.X-1,0};
    WriteConsoleOutput(std_out,menubar,size,pos,&srmenu);
}

/**
 * 函数名称: PopMenu
 * 函数功能: 弹出指定主菜单项对应的子菜单.
 * 输入参数: num 指定的主菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void PopMenu(int num)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    int i, j, loc = 0;

    if (num != selmainmenu)       /*如果指定主菜单不是已选中菜单*/
    {
        if (layhead->LayerNo != 0) /*如果此前已有子菜单弹出*/
        {
            PopOff();
            selsubmenu = 0;
        }
    }
    else if (layhead->LayerNo != 0) /*若已弹出该子菜单，则返回*/
    {
        return;
    }

    selmainmenu = num;
    TagMainMenu(selmainmenu);
    CalLocation(selmainmenu,&srpop);/*计算弹出子菜单的区域位置, 存放在srpop中*/

    /*计算子菜单第一项在在菜单数组中的下标*/
    for(i=1; i<selmainmenu; i++)
    {
        loc += sub_menu_num[i-1];
    }
    /*将子菜单项存入标签结构*/
    labels.ppLabel = sub_menu + loc; /*标签束第一个标签字符串的地址*/
    labels.num = sub_menu_num[selmainmenu-1];/*标签束中标签字符串的个数*/
    COORD alabelloc[labels.num];/*定义标签个数大小的标签位置数组*/

    for(i=0; i<labels.num; i++)
    {
        alabelloc[i].X = srpop.Left + 2;
        alabelloc[i].Y = srpop.Top + i + 1;
    }

    labels.pLoc = alabelloc;
    /*设置热区*/
    areas.num = labels.num;/*热区的个数，等于标签的个数，即子菜单的项数*/
    SMALL_RECT arealoc[areas.num];/*定义数组存放所有热区位置*/
    char aSort[areas.num];/*定义数组存放所有热区对应类别*/
    char aTag[areas.num]; /*定义数组存放每个热区的编号*/

    for(i=0; i<areas.num; i++)
    {
        arealoc[i].Left = srpop.Left + 2;
        arealoc[i].Top = srpop.Top + i + 1;
        arealoc[i].Right = srpop.Right - 2;
        arealoc[i].Bottom = srpop.Top;
        aSort[i] = 0;/*热区类别都为0(按钮型)*/
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
            {   /*取消该区域字符单元的热区属性*/
                scratt[pos.Y*SCR_COL+j] &= 3; /*按位与的结果保留了低两位*/
            }
        }

    }
}

/**
 * 函数名称: PopUp
 * 函数功能: 在指定区域输出弹出窗口信息, 同时设置热区, 将弹出窗口位置信息入栈.
 * 输入参数: srpop 弹出窗口位置数据存放的地址
 *           att 弹出窗口区域字符属性
 *           labels 弹出窗口中标签束信息存放的地址
             areas 弹出窗口中热区信息存放的地址
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void PopUp(SMALL_RECT *srpop, WORD att, LABEL_BUNDLE *labels, HOT_AREA *areas)
{
    int i,j,row;
    char *pch;
    LAYER_NODE *nextlayer;
    COORD pos = {0,0};
    COORD size;

    /*设置弹出窗口大小*/
    size.X = srpop->Right - srpop->Left + 1;
    size.Y = srpop->Bottom - srpop->Top + 1;

    /*申请存放弹出窗口信息的动态缓冲区*/
    nextlayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
    nextlayer->next = layhead;
    nextlayer->LayerNo = layhead->LayerNo + 1;
    nextlayer->pContent = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
    nextlayer->pScrAtt = (char *)malloc(size.X * size.Y * sizeof(char));
    nextlayer->rcArea = *srpop;
    pch = nextlayer->pScrAtt;

    /*将弹出窗口覆盖区域的信息保存起来*/
    ReadConsoleOutput(std_out,nextlayer->pContent,size,pos,srpop);

    for(i=srpop->Top; i<=srpop->Bottom; i++)
    {
        for(j=srpop->Left; j<=srpop->Right; j++)
        {
            *pch = scratt[i*SCR_COL+j];
            pch++;
        }
    }

    /*弹出窗口信息入栈*/
    layhead = nextlayer;
    pos.X = srpop->Left;
    pos.Y = srpop->Top;
    for(i=srpop->Top; i<=srpop->Bottom; i++)
    {
        FillConsoleOutputAttribute(std_out, att, size.X, pos, &ul);
        pos.Y++;
    }
    /*将标签束中的标签字符串在设定的位置输出*/
    for (i=0; i<labels->num; i++)
    {
        pch = labels->ppLabel[i];
        if (strlen(pch) != 0)
        {
            WriteConsoleOutputCharacter(std_out, pch, strlen(pch),labels->pLoc[i], &ul);
        }
    }
    /*设置弹出窗口区域字符单元的新属性*/
    for (i=srpop->Top; i<=srpop->Bottom; i++)
    {   /*此二重循环设置字符单元的层号*/
        for (j=srpop->Left; j<=srpop->Right; j++)
        {
            scratt[i*SCR_COL+j] = layhead->LayerNo;
        }
    }

    for (i=0; i<areas->num; i++)
    {   /*此二重循环设置所有热区中字符单元的热区类型和热区编号*/
        row = areas->pArea[i].Top;
        for (j=areas->pArea[i].Left; j<=areas->pArea[i].Right; j++)
        {
            scratt[row*SCR_COL+j] |= (areas->pSort[i] << 6) | (areas->pTag[i] << 2);
        }
    }
}

/**
 * 函数名称: PopOff
 * 函数功能: 关闭顶层弹出窗口, 恢复覆盖区域原外观和字符单元原属性.
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void PopOff(void)
{
    LAYER_NODE *nextLayer;
    COORD size;
    COORD pos = {0, 0};
    char *pch;
    int i, j;

    if ((layhead->next==NULL) || (layhead->pContent==NULL))
    {   /*栈底存放的主界面屏幕信息，不用关闭*/
        return;
    }
    nextLayer = layhead->next;
    /*恢复弹出窗口区域原外观*/
    size.X = layhead->rcArea.Right - layhead->rcArea.Left + 1;
    size.Y = layhead->rcArea.Bottom - layhead->rcArea.Top + 1;
    WriteConsoleOutput(std_out, layhead->pContent, size, pos, &(layhead->rcArea));
    /*恢复字符单元原属性*/
    pch = layhead->pScrAtt;
    for (i=layhead->rcArea.Top; i<=layhead->rcArea.Bottom; i++)
    {
        for (j=layhead->rcArea.Left; j<=layhead->rcArea.Right; j++)
        {
            scratt[i*SCR_COL+j] = *pch;
            pch++;
        }
    }
    free(layhead->pContent);    /*释放动态存储区*/
    free(layhead->pScrAtt);
    free(layhead);
    layhead = nextLayer;
    selsubmenu = 0;
}

/**
 * 函数名称: DrawBox
 * 函数功能: 在指定区域画边框.
 * 输入参数: pRc 存放区域位置信息的地址
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void DrawBox(SMALL_RECT *pRc)
{
    char chBox[] = {'+','-','|'};  /*画框用的字符*/
    COORD pos = {pRc->Left, pRc->Top};  /*定位在区域的左上角*/

    WriteConsoleOutputCharacter(std_out, &chBox[0], 1, pos, &ul);/*画边框左上角*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*此循环画上边框横线*/
        WriteConsoleOutputCharacter(std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(std_out, &chBox[0], 1, pos, &ul);/*画边框右上角*/
    for (pos.Y = pRc->Top+1; pos.Y < pRc->Bottom; pos.Y++)
    {   /*此循环画边框左边线和右边线*/
        pos.X = pRc->Left;
        WriteConsoleOutputCharacter(std_out, &chBox[2], 1, pos, &ul);
        pos.X = pRc->Right;
        WriteConsoleOutputCharacter(std_out, &chBox[2], 1, pos, &ul);
    }
    pos.X = pRc->Left;
    pos.Y = pRc->Bottom;
    WriteConsoleOutputCharacter(std_out, &chBox[0], 1, pos, &ul);/*画边框左下角*/
    for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
    {   /*画下边框横线*/
        WriteConsoleOutputCharacter(std_out, &chBox[1], 1, pos, &ul);
    }
    pos.X = pRc->Right;
    WriteConsoleOutputCharacter(std_out, &chBox[0], 1, pos, &ul);/*画边框右下角*/
}

/**
 * 函数名称: TagSubMenu
 * 函数功能: 在指定子菜单项上做选中标记.
 * 输入参数: num 选中的子菜单项号
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void TagSubMenu(int num)
{
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    int width;

    CalLocation(selmainmenu, &srpop);  /*计算弹出子菜单的区域位置, 存放在srpop中*/
    if ((num<1) || (num == selsubmenu) || (num>srpop.Bottom-srpop.Top-1))
    {   /*如果子菜单项号越界，或该项子菜单已被选中，则返回*/
        return;
    }

    pos.X = srpop.Left + 2;
    width = srpop.Right - srpop.Left - 3;
    if (selsubmenu != 0) /*首先取消原选中子菜单项上的标记*/
    {
        pos.Y = srpop.Top + selsubmenu;
        att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
        FillConsoleOutputAttribute(std_out, att, width, pos, &ul);
        pos.X += 1;
    }
    /*在指定子菜单项上做选中标记*/
    pos.X = srpop.Left + 2;
    pos.Y = srpop.Top + num;
    att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
    FillConsoleOutputAttribute(std_out, att, width, pos, &ul);
    selsubmenu = num;  /*修改选中子菜单项号*/
    return;
}

/**
 * 函数名称: CalLocation
 * 函数功能: 计算弹出子菜单区域左上角和右下角的位置.
 * 输入参数: num 选中的主菜单项号
 * 输出参数: rc 存放区域位置信息的地址
 * 返 回 值: 无
 *
 * 调用说明:
 */
void CalLocation(int num, SMALL_RECT *rc)
{
    int i, len, loc = 0;

    rc->Top = 1; /*区域的上边定在第2行，行号为1*/
    rc->Left = 1;
    for (i=1; i<num; i++)
    {   /*计算区域左边界位置, 同时计算第一个子菜单项在子菜单字符串数组中的位置*/
        rc->Left += strlen(main_menu[i-1]) + 4;
        loc += sub_menu_num[i-1];
    }
    rc->Right = strlen(sub_menu[loc]);/*暂时存放第一个子菜单项字符串长度*/

    for (i=1; i<sub_menu_num[num-1]; i++)
    {   /*查找最长子菜单字符串，将其长度存放在rc->Right*/
        len = strlen(sub_menu[loc+i]);
        if (rc->Right < len)
        {
            rc->Right = len;
        }
    }

    rc->Right += rc->Left + 3;  /*计算区域的右边界*/
    rc->Bottom = rc->Top + sub_menu_num[num-1] + 1;/*计算区域下边的行号*/
    if (rc->Right >= SCR_COL)  /*右边界越界的处理*/
    {
        len = rc->Right - SCR_COL + 1;
        rc->Left -= len;
        rc->Right = SCR_COL - 1;
    }
}

/**
 * 函数名称: DealInput
 * 函数功能: 在弹出窗口区域设置热区, 等待用户输入.
 * 输入参数: pHotArea 焦点热区编号的存放地址, 即指向焦点热区编号的指针
 *           piHot 用鼠标单击、按回车或空格时返回当前热区编号
 * 输出参数:
 * 返 回 值:
 *
 * 调用说明:
 */
int DealInput(HOT_AREA *pHotArea, int *piHot)
{
    INPUT_RECORD inRec;
    DWORD res;
    int num, arrow, re = 0;
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/

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
                {  /*方向键(左、上、右、下)的处理*/
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
            {  /*ESC键*/
                re = 27;
                break;
            }
            else if (vkc == 13)
            {  /*回车键表示按下当前按钮*/
                re = 13;
                break;
            }
        }
    }
    return re;
}

/**
 * 函数名称: SetHotPoint
 * 函数功能: 设置热区
 * 输入参数: pHotArea 选中的热区
 * 输出参数: iHot 焦点热区编号的存放地址, 即指向焦点热区编号的指针
 * 返 回 值: 无
 *
 * 调用说明:
 */
void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
    CONSOLE_CURSOR_INFO lpCur;
    COORD pos = {0, 0};
    WORD att1, att2;
    int i, width;

    att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
    att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    for (i=0; i<pHotArea->num; i++)
    {  /*将按钮类热区置为白底黑字*/
        pos.X = pHotArea->pArea[i].Left;
        pos.Y = pHotArea->pArea[i].Top;
        width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
        if (pHotArea->pSort[i] == 0)
        {  /*热区是按钮类*/
            FillConsoleOutputAttribute(std_out, att2, width, pos, &ul);
        }
    }

    pos.X = pHotArea->pArea[iHot-1].Left;
    pos.Y = pHotArea->pArea[iHot-1].Top;
    width = pHotArea->pArea[iHot-1].Right - pHotArea->pArea[iHot-1].Left + 1;
    if (pHotArea->pSort[iHot-1] == 0)
    {  /*被激活热区是按钮类*/
        FillConsoleOutputAttribute(std_out, att1, width, pos, &ul);
    }
    else if (pHotArea->pSort[iHot-1] == 1)
    {  /*被激活热区是文本框类*/
        SetConsoleCursorPosition(std_out, pos);
        GetConsoleCursorInfo(std_out, &lpCur);
        lpCur.bVisible = TRUE;
        SetConsoleCursorInfo(std_out, &lpCur);
    }
}

/**
 * 函数名称: MaintainProInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
BOOL MaintainProInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    char *pCh[] = {"录入省份信息", "删除省份信息","修改省份信息","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 10;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 10, srpop.Top + 8}};/*四个按钮的热区位置*/
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
 * 函数名称: InputProInfo
 * 函数功能: 录入省份信的对话框弹出
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 结构指针
 *
 * 调用说明:
 */
void InputProInfo(void)
{
    PROVINCE_NODE* pnewnode = (PROVINCE_NODE *)malloc(sizeof(PROVINCE_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"省份名称",
                   "        ",
                   "纪检负责人",
                   "               ",
                   "联系电话",
                   "                ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 10;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 21, srpop.Top + 8}};/*五个热区的位置*/
    char aSort[] = {1,1,1,0,0};
    char aTag[] = {1,2,3,4,5};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+7;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
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
    char vkc, asc;                                          /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[3],&res);
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[5],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                        char *plabel_name[] = {"省份信息已存在","确认"};
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
                            char *plabel_name[] = {"省份信息录入成功！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(pc1)==0 || strlen(pc2)==0 || strlen(pc3)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                    char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
                    ShowModule(plabel_name, 2);
                    PopOff();
                    return;
                }

            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pc3++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

/**
 * 函数名称: InsertProvinceNode
 * 函数功能: 在十字链表中插入一个省份信息结点.
 * 输入参数: head 主链头指针
 *           pprovince_node 指向所要插入结点的指针
 * 输出参数: 无
 * 返 回 值: int类型, 1表示插入成功, 0表示插入失败
 *
 * 调用说明:
 */
int InsertProvinceNode(PROVINCE_NODE **head,PROVINCE_NODE *pprovincenode)
{
    pprovincenode->next = *head;
    *head = pprovincenode;
    return 1;
}

/**
 * 函数名称: DelProInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void DelProInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入待删除省份名称",
                   "        ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 18, srpop.Top + 6}};/*五个热区的位置*/
    char aSort[] = {1,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char proname[8];
    char *pro = proname;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[1],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1))
            {
                //pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                *(--pro) = c;
            }

            /*enter 键的处理*/
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
                            char *plabel_name[] = {"无此省份","确认"};
                            ShowModule(plabel_name,2);
                            return;
                        }
                        int re = DeleteProvinceNode(proname);
                        if(re)
                        {
                            PopOff();
                            char *plabel_name[] = {"删除成功！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            PopOff();
                            char *plabel_name[] = {"无此省份！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(proname) == 0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pro++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * 函数名称: DeleteProvinceNode
 * 函数功能: 在十字链表中删除一个省份信息结点.
 * 输入参数: head 主链头指针
 *           pvblg 所属省份
 * 输出参数: 无
 * 返 回 值: int类型, 1表示删除成功, 0表示删除失败
 *
 * 调用说明:
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
 * 函数名称: ModifyProInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值:
 *
 * 调用说明:
 */
void ModifyProInfo(void)
{

    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入待修改省份名称",
                   "          ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 18, srpop.Top + 6}};/*五个热区的位置*/
    char aSort[] = {1,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char proname[8];
    char *pro = proname;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[1],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1))
            {
                //pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                *(--pro) = c;
            }

            /*enter 键的处理*/
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
                        char *plabel_name[] = {"无此省份","确认"};
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
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pro++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * 函数名称: InputModifyProInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void InputModifyProInfo(void)
{
    PROVINCE_NODE *pnewnode = (PROVINCE_NODE *)malloc(sizeof(PROVINCE_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"省份名称",
                   "        ",
                   "纪检负责人",
                   "               ",
                   "联系电话",
                   "                ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 10;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 21, srpop.Top + 8}};/*五个热区的位置*/
    char aSort[] = {1,1,1,0,0};
    char aTag[] = {1,2,3,4,5};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+7;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
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
    char vkc, asc;                                          /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[3],&res);
	FillConsoleOutputAttribute(std_out,att2,16,aLoc[5],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                            char *plabel_name[] = {"修改成功！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"无此省份！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(pc1)==0 || strlen(pc2)==0 || strlen(pc3)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                    char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
                    ShowModule(plabel_name, 2);
                    PopOff();
                    return;
                }

            }
            vkc = keyRec.Event.KeyEvent.wVirtualKeyCode;
            asc = keyRec.Event.KeyEvent.uChar.AsciiChar;
            if(keyRec.EventType == KEY_EVENT && keyRec.Event.KeyEvent.bKeyDown == TRUE)
            {
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pc3++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

 /**
 * 函数名称: ModifyProvinceNode
 * 函数功能: 对指定的省份信息结点内容进行修改.
 * 输入参数: head 主链头指针
 *           pvblg 所属省份
 *           pprovincenode 指向存放修改内容结点的指针
 * 输出参数: 无
 * 返 回 值: int类型, 1表示修改成功, 0表示修改失败
 *
 * 调用说明:
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
 * 函数名称: MaintainCaseInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
BOOL MaintainCaseInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    char *pCh[] = {"录入贪腐案件信息", "删除贪腐案件信息","修改贪腐案件信息","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 10;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 12, srpop.Top + 8}};/*四个按钮的热区位置*/
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
 * 函数名称: InputCaseInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void InputCaseInfo(void)
{
    CASE_NODE *pnewnode = (CASE_NODE *)malloc(sizeof(CASE_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"案件编号",  "               ",
                   "立案日期",  "               ",
                   "所属省份",  "               ",
                   "被告人姓名","               ",
                   "涉案金额",  "               ",
                   "行政级别",  "               ",
                   "('A'-'H'：分别代表副科级、正科级、副处级、",
                   "正处级、副厅级、正厅级、副部级和正部级及以上)",

                   "主要罪名",  "               ",
                   "('1'-'6'：分别代表受贿、贪污、",
                   "挪用公款、渎职、滥用职权和其他)",

                   "刑事处罚",  "               ",
                   "(1~99：有期徒刑  大于等于100：无期徒刑  小于0：死刑)",

                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 42;
    pos.Y = 25;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           };/*十个热区的位置*/
    char aSort[] = {1,1,1,1,1,1,1,1,0,0};
    char aTag[] = {1,2,3,4,5,6,7,8,9,10};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+22;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char casenum[13];           /*案件编号*/
    char casedate[11];          /*立案日期*/
    char pvblg[9];              /*所属省份*/
    char crimename[14];         /*被告人姓名*/
    char charmoney[15];         /*涉案金额*/
    char level;                 /*行政级别*/
    char accusationname;        /*主要罪名*/
    char punishment[4];         /*行事处罚*/

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
    char vkc, asc;                                          /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
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

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                        for(value=0.0;charmoney[i]>='0'&&charmoney[i]<='9';i++)//计算小数点前的数字
                        {
                            value=value*10.0+(charmoney[i]-'0');
                        }

                        if(charmoney[i]=='.')
                        {
                            i++;
                        }
                        for(power=1.0;charmoney[i]>='0'&&charmoney[i]<='9';i++)//计算小数点后的数字
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
                            char *plabel_name[] = {"案件信息录入成功！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"案件信息录入失败！","确认"};
                            ShowModule(plabel_name, 2);
                        }

                        PopOff();
                        return;
                    }
                    else if(strlen(casedate)==0 || strlen(casenum)==0 ||
                            strlen(pvblg)==0 || strlen(crimename)==0 || strlen(charmoney)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
 * 函数名称: InsertCaseNode
 * 函数功能: 在十字链表中插入一个案件信息结点.
 * 输入参数: head 主链头指针
 *           pvblg 所属省份
 *           pcasenode 指向所要插入结点的指针
 * 输出参数: 无
 * 返 回 值: int类型, 1表示插入成功, 0表示插入失败
 *
 * 调用说明:
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
 * 函数名称: DelCaseInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void DelCaseInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入省份名称",
                   "        ",
                   "请输入案件编号",
                   "            ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 21, srpop.Top + 6}};/*四个热区的位置*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
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
    char vkc, asc;                                          /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,8,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                            char *plabel_name[] = {"无此省份","确认"};
                            ShowModule(plabel_name,2);
                            return;
                        }
                        int re = DeleteCaseNode(casenum,proname);
                        if(re)
                        {
                            char *plabel_name[] = {"案件信息删除成功！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"无此案件信息！","确认"};
                            ShowModule(plabel_name, 2);
                        }

                        PopOff();
                        return;
                    }
                    else if(strlen(proname)==0 || strlen(casenum)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

/**
 * 函数名称: DeleteCaseNode
 * 函数功能: 在十字链表中删除一个案件信息结点.
 * 输入参数: head 主链头指针
 *           casenum 案件编号
             pvblg 所属省份
 * 输出参数: 无
 * 返 回 值: int类型, 1表示删除成功, 0表示删除失败
 *
 * 调用说明:
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
        if(p1 == NULL)                          /*删除的案件信息节点在第一个*/
            provincenode->cnext = p2->next;
        else
            p1->next = p2->next;
        free(p2);
        return 1;
    }
    return 0;
}

 /**
 * 函数名称: ModifyCaseInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void ModifyCaseInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入省份名称",
                   "        ",
                   "请输入案件编号",
                   "            ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
    //aArea要和底下的FillConsoleOutputAttribute相匹配
    SMALL_RECT aArea[] = {{srpop.Left + 18, srpop.Top + 2,
                           srpop.Left + 26, srpop.Top + 2},
                          {srpop.Left + 18, srpop.Top + 4,
                           srpop.Left + 30, srpop.Top + 4},
                          {srpop.Left + 9, srpop.Top + 6,
                           srpop.Left + 12, srpop.Top + 6},
                          {srpop.Left + 18, srpop.Top + 6,
                           srpop.Left + 21, srpop.Top + 6}};/*四个热区的位置*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
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
    char vkc, asc;                                          /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,8,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
	    //DealInput(&areas,&iHot);
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                            char *plabel_name[] = {"无此省份","确认"};
                            ShowModule(plabel_name,2);
                            return;
                        }
                        InputModifyCaseInfo();//转到修改函数
                        return;
                    }
                    else if(strlen(proname)==0 || strlen(casenum)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

/**
 * 函数名称: InputModifyCaseInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
void InputModifyCaseInfo(void)
{
    CASE_NODE *pnewnode = (CASE_NODE *)malloc(sizeof(CASE_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"案件编号",  "               ",
                   "立案日期",  "               ",
                   "所属省份",  "               ",
                   "被告人姓名","               ",
                   "涉案金额",  "               ",
                   "行政级别",  "               ",
                   "('A'-'H'：分别代表副科级、正科级、副处级、",
                   "正处级、副厅级、正厅级、副部级和正部级及以上)",

                   "主要罪名",  "               ",
                   "('1'-'6'：分别代表受贿、贪污、",
                   "挪用公款、渎职、滥用职权和其他)",

                   "刑事处罚",  "               ",
                   "(1~99：有期徒刑  大于等于100：无期徒刑  小于0：死刑)",

                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 42;
    pos.Y = 25;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           };/*十个热区的位置*/
    char aSort[] = {1,1,1,1,1,1,1,1,0,0};
    char aTag[] = {1,2,3,4,5,6,7,8,9,10};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+22;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char casenum[13];           /*案件编号*/
    char casedate[11];          /*立案日期*/
    char pvblg[9];              /*所属省份*/
    char crimename[14];         /*被告人姓名*/
    char charmoney[15];         /*涉案金额*/
    char level;                 /*行政级别*/
    char accusationname;        /*主要罪名*/
    char punishment[4];         /*行事处罚*/

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
    char vkc, asc;                                          /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
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

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                        for(value=0.0;charmoney[i]>='0'&&charmoney[i]<='9';i++)//计算小数点前的数字
                        {
                            value=value*10.0+(charmoney[i]-'0');
                        }

                        if(charmoney[i]=='.')
                        {
                            i++;
                        }
                        for(power=1.0;charmoney[i]>='0'&&charmoney[i]<='9';i++)//计算小数点后的数字
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
                            char *plabel_name[] = {"案件信息修改成功！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"案件信息修改失败！","确认"};
                            ShowModule(plabel_name, 2);
                        }

                        PopOff();
                        return;
                    }
                    else if(strlen(casedate)==0 || strlen(casenum)==0 ||
                            strlen(pvblg)==0 || strlen(crimename)==0 || strlen(charmoney)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
 * 函数名称: ModifyCaseNode
 * 函数功能: 对指定的案件信息结点内容进行修改.
 * 输入参数: head 主链头指针
 *           casenum 案件编号
 *           pcasenode 指向存放修改内容结点的指针
 * 输出参数: 无
 * 返 回 值: int类型, 1表示修改成功, 0表示修改失败
 *
 * 调用说明:
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
 * 函数名称: MaintainMediaInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
BOOL MaintainMediaInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    char *pCh[] = {"录入媒体报道信息", "删除媒体报道信息","修改媒体报道信息","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 10;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 12, srpop.Top + 8}};/*四个按钮的热区位置*/
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
 * 函数名称: InputMediaInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void InputMediaInfo(void)
{
    MEDIA_NODE *pnewnode = (MEDIA_NODE *)malloc(sizeof(MEDIA_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"所属省份","        ",
                   "案件编号","            ",
                   "报道日期","          ",
                   "媒体名称","                    ",
                   "内容索引","                                                   ",
                   "媒体类别 '1' '2' '3' '4'分别代表报纸、广播、电视和互联网"," ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 57;
    pos.Y = 16;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           };/*八个热区的位置*/
    char aSort[] = {1,1,1,1,1,1,0,0};
    char aTag[] = {1,2,3,4,5,6,7,8};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+13;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char pvblg[9];              /*所属省份*/
    char casenum[13];           /*案件编号*/
    char reportdate[11];        /*报道日期*/
    char medianame[21];         /*媒体名称*/
    char contentindex[51];      /*内容索引*/
    char mediatype;             /*媒体类别*/

    char *pc1 = pvblg;
    char *pc2 = casenum;
    char *pc3 = reportdate;
    char *pc4 = medianame;
    char *pc5 = contentindex;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
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

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                            char *plabel_name[] = {"媒体信息录入成功！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"媒体信息录入失败！","确认"};
                            ShowModule(plabel_name, 2);
                        }

                        PopOff();
                        return;
                    }
                    else if(strlen(reportdate)==0 || strlen(casenum)==0 ||
                            strlen(pvblg)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
 * 函数名称: InsertMediaNode
 * 函数功能: 在十字链表中插入一个媒体信息结点.
 * 输入参数: head 主链头指针
 *           pmedianode 指向所要插入结点的指针
 * 输出参数: 无
 * 返 回 值: int类型, 1表示插入成功, 0表示插入失败
 *
 * 调用说明:
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
 * 函数名称: DelMediaInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void DelMediaInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入省份名称",
                   "        ",
                   "请输入案件编号",
                   "            ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 21, srpop.Top + 6}};/*四个热区的位置*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
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
    char vkc, asc;                                          /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,8,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                            char *plabel_name[] = {"无此省份","确认"};
                            ShowModule(plabel_name,2);
                            return;
                        }

                        int re = DeleteMediaNode(casenum,proname);
                        if(re)
                        {
                            char *plabel_name[] = {"媒体信息删除成功！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"媒体信息删除失败！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(proname)==0 || strlen(casenum)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

/**
 * 函数名称: DeleteMediaNode
 * 函数功能: 在十字链表中删除一个媒体信息结点.
 * 输入参数: head 主链头指针
 *           casenum 案件编号
             reportdate 报道日期
 * 输出参数: 无
 * 返 回 值: int类型, 1表示删除成功, 0表示删除失败
 *
 * 调用说明:
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
        if(p1 == NULL)              /*要删除的媒体信息节点在第一个*/
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
 * 函数名称: ModifyMediaInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void ModifyMediaInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入省份名称",
                   "        ",
                   "请输入案件编号",
                   "            ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 25;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 21, srpop.Top + 6}};/*四个热区的位置*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
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
    char vkc, asc;                                          /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,8,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
	    //DealInput(&areas,&iHot);
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                            char *plabel_name[] = {"无此省份","确认"};
                            ShowModule(plabel_name,2);
                        }
                        return;
                    }
                    else if(strlen(proname)==0 || strlen(casenum)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }
        }
    }
}

 /**
 * 函数名称: InputModifyMediaInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void InputModifyMediaInfo(void)
{
    MEDIA_NODE *pnewnode = (MEDIA_NODE *)malloc(sizeof(MEDIA_NODE));
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"所属省份","        ",
                   "案件编号","            ",
                   "报道日期","          ",
                   "媒体名称","                    ",
                   "内容索引","                                                   ",
                   "媒体类别 '1' '2' '3' '4'分别代表报纸、广播、电视和互联网"," ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 57;
    pos.Y = 16;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           };/*八个热区的位置*/
    char aSort[] = {1,1,1,1,1,1,0,0};
    char aTag[] = {1,2,3,4,5,6,7,8};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+13;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';

    char pvblg[9];              /*所属省份*/
    char casenum[13];           /*案件编号*/
    char reportdate[11];        /*报道日期*/
    char medianame[21];         /*媒体名称*/
    char contentindex[51];      /*内容索引*/
    char mediatype;             /*媒体类别*/

    char *pc1 = pvblg;
    char *pc2 = casenum;
    char *pc3 = reportdate;
    char *pc4 = medianame;
    char *pc5 = contentindex;

    pos.X = aArea[0].Left;
    pos.Y = aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;                                          /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
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

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                            char *plabel_name[] = {"媒体信息修改成功！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        else
                        {
                            char *plabel_name[] = {"媒体信息修改失败！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(reportdate)==0 || strlen(casenum)==0 ||
                            strlen(pvblg)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
 * 函数名称: ModifyMediaNode
 * 函数功能: 对指定的媒体信息结点内容进行修改.
 * 输入参数: casenum 案件编号
 *           reportdate 报道日期
 *           pmedianode 指向存放修改内容结点的指针
 * 输出参数: 无
 * 返 回 值: int类型, 1表示修改成功, 0表示修改失败
 *
 * 调用说明:
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
 * 函数名称: QueryProInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
BOOL QueryProInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos,pos1,pos2;
    WORD att;
    char *pCh[] = {"按省份名称查询", "按纪检负责人查询","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 12, srpop.Top + 6},};/*三个按钮的热区位置*/
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
 * 函数名称: QueryProname
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void QueryProname(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入待查询省份名称",
                   "        ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 18, srpop.Top + 6}};/*五个热区的位置*/
    char aSort[] = {1,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char proname[9];
    char *pro = proname;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[1],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1))
            {
                //pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                *(--pro) = c;
            }

            /*enter 键的处理*/
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
                            char *plabel_name[] = {"省份名称：",tmp->pvblg,"纪检负责人：",tmp->disciplineperson,"联系电话：",tmp->tele,"确认"};
                            ShowModule(plabel_name, 7);
                            return;
                        }
                        else
                        {
                            char *plabel_name[] = {"无此省份！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(proname) == 0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pro++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * 函数名称: QueryProvinceInfo1
 * 函数功能: 对指定的省份信息结点内容进行查询.
 * 输入参数: head 主链头指针
 *           pvblg 所属省份
 * 输出参数: 无
 * 返 回 值: PROVINCE_NODE 类型, 返回查询结果组成的新链头指针
 *
 * 调用说明: 以省份名称为条件来查找并显示满足条件的所属省份监管信息
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
 * 函数名称: QueryPerson
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void QueryPerson(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入纪检负责人名称",
                   "               ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 18, srpop.Top + 6}};/*五个热区的位置*/
    char aSort[] = {1,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char person[15];
    char *per = person;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[1],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1))
            {
                //pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                *(--per) = c;
            }

            /*enter 键的处理*/
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
                                char *plabel_name[] = {"省份名称：",tmp->pvblg,"纪检负责人：",tmp->disciplineperson,"联系电话：",tmp->tele,"确认"};
                                ShowModule(plabel_name, 7);
                            }
                            return;
                        }
                        else
                        {
                            char *plabel_name[] = {"无此省份！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(person) == 0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(per++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * 函数名称: QueryProvinceInfo2
 * 函数功能: 对指定的省份信息结点内容进行查询.
 * 输入参数: head 主链头指针
 *           disciplineperson 纪检负责人
 * 输出参数: 无
 * 返 回 值: PROVINCE_NODE 类型, 返回查询结果组成的新链头指针
 *
 * 调用说明: 以纪检负责人姓名为条件来查找并显示满足条件的所属省份监管信息
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
 * 函数名称: QueryCaseInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
BOOL QueryCaseInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos,pos1,pos2;
    WORD att;
    char *pCh[] = {"按案件编号名称查询", "按被告姓名与行政级别查询","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 16, srpop.Top + 6},};/*三个按钮的热区位置*/
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
 * 函数名称: QueryCasenum
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void QueryCasenum(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入待查询的案件编号",
                   "            ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 19, srpop.Top + 6}};/*五个热区的位置*/
    char aSort[] = {1,0,0};
    char aTag[] = {1,2,3};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+5;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char casenum[13];
    char *pcase = casenum;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[1],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
			else if ((keyRec.Event.KeyEvent.wVirtualKeyCode == VK_BACK)
               && (pos.X>aArea[0].Left) &&(keyRec.Event.KeyEvent.bKeyDown) && (iHot==1))
            {
                //pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
                pos.X--;
                WriteConsoleOutputCharacter(std_out,&c,1,pos,&ul);
                SetConsoleCursorPosition(std_out,pos);
                *(--pcase) = c;
            }

            /*enter 键的处理*/
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

                                /*将float型数据转换成字符串*/
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


                                    char *plabel_name[] = {"案件编号：",tmp->casenum,"立案日期：",
                                                        tmp->casedate,"所属省份：",tmp->pvblg,"被告人姓名：",
                                                        tmp->crimename,"行政级别：",a,"主要罪名：",b,
                                                        "涉案金额：",ary,"行事处罚：",punchar,"确定"};
                                    ShowModule(plabel_name,17);
                            }

                            return;
                        }
                        else
                        {
                            char *plabel_name[] = {"无此案件信息！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(casenum) == 0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pcase++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * 函数名称: QueryCaseInfo1
 * 函数功能: 对指定的案件信息结点内容进行查询.
 * 输入参数: head 主链头指针
 *           casenum 案件编号
 * 输出参数: 无
 * 返 回 值: CASE_NODE 类型, 返回查询结果组成的新链头指针
 *
 * 调用说明: 以案件编号为条件查找并显示满足条件的贪腐案件的基本信息
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
 * 函数名称: QueryCrimer
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 无
 *
 * 调用说明:
 */
void QueryCrimer(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入被告姓名",
                   "               ",
                   "请输入级别（‘A’~’H’：分别代表副科级、正科级、副处级、",
                   "正处级、副厅级、正厅级、副部级和正部级及以上）",
                   " ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[2]) + 6;
    pos.Y = 13;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 36, srpop.Top + 11}};/*四个热区的位置*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+10;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
    INPUT_RECORD keyRec;
    DWORD res;
    char ch,c=' ';
    char crimer[16];
    char level;
    char *pcrimer = crimer;

    pos.X=aArea[0].Left, pos.Y=aArea[0].Top;
    CONSOLE_CURSOR_INFO lpCur;
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,15,aLoc[4],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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

                                /*将float型数据转换成字符串*/
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

                                    char *plabel_name[] = {"案件编号：",tmp->casenum,"立案日期：",
                                                        tmp->casedate,"所属省份：",tmp->pvblg,"被告人姓名：",
                                                        tmp->crimename,"行政级别：",a,"主要罪名：",b,
                                                        "涉案金额：",ary,"行事处罚：",punchar,"确定"};
                                    ShowModule(plabel_name,17);
                            }
                            return;
                        }
                        else
                        {
                            char *plabel_name[] = {"无此案件信息！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        PopOff();
                        return;
                    }
                    else if(strlen(crimer) == 0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    level = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * 函数名称: QueryCaseInfo2
 * 函数功能: 对指定的案件信息结点内容进行查询.
 * 输入参数: head 主链头指针
 *           crimename 被告人姓名
 * 输出参数: 无
 * 返 回 值: CASE_NODE 类型, 返回查询结果组成的新链头指针
 *
 * 调用说明: 以被告姓名和行政级别为条件查找并显示满足条件的贪腐案件的基本信息
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
 * 函数名称: QueryMediaInfo
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
BOOL QueryMediaInfo(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos,pos1,pos2;
    WORD att;
    char *pCh[] = {"按案件编号与媒体名称查询", "按报道日期与部分媒体名称查询","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[1]) + 6;
    pos.Y = 8;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 18, srpop.Top + 6},};/*三个按钮的热区位置*/
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
 * 函数名称: QueryCaseMedia
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
void QueryCaseMedia(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入待查询的案件编号",
                   "            ",
                   "请输入媒体名称",
                   "                    ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 12;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 19, srpop.Top + 10}};/*四个热区的位置*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+9;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
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
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,12,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                                char *plabel_name[] = {"所属省份：",tmp->pvblg,"案件编号：",tmp->casenum,
                                                        "报道日期：",tmp->reportdate,"媒体名称：",tmp->medianame,
                                                        "媒体类别：",a,"内容索引：",tmp->contentindex,"确认"};
                                ShowModule(plabel_name,13);
                            }
                            return;
                        }
                        else
                        {
                            char *plabel_name[] = {"无此媒体信息！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        return;
                    }
                    else if(strlen(casenum)==0 || strlen(medianame)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * 函数名称: QueryMediaInfo1
 * 函数功能: 对指定的媒体信息结点内容进行查询.
 * 输入参数: head 主链头指针
 *           casenum 案件编号
 *           medianame 媒体名称
 * 输出参数: 无
 * 返 回 值: MEDIA_NODE 类型, 返回查询结果组成的新链头指针
 *
 * 调用说明: 以案件编号和媒体名称为条件查找并显示满足条件的媒体报道基本信息
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
 * 函数名称: QueryDateMedia
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
void QueryDateMedia(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入报道日期",
                   "          ",
                   "请输入部分媒体名称",
                   "                    ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[2]) + 8;
    pos.Y = 12;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 17, srpop.Top + 10}};/*四个热区的位置*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+9;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
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
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                                char *plabel_name[] = {"所属省份：",tmp->pvblg,"案件编号：",tmp->casenum,
                                                        "报道日期：",tmp->reportdate,"媒体名称：",tmp->medianame,
                                                        "媒体类别：",a,"内容索引：",tmp->contentindex,"确认"};
                                ShowModule(plabel_name,13);
                            }
                        }
                        else
                        {
                            char *plabel_name[] = {"无此媒体信息！","确认"};
                            ShowModule(plabel_name, 2);
                        }
                        return;
                    }
                    else if(strlen(reportdate)==0 || strlen(medianame)==0)
                    {
                        PopOff();
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
                    *(pc2++) = ch;
                    pos.X++;
                    SetConsoleCursorPosition(std_out,pos);
                }
            }

        }
    }
}

/**
 * 函数名称: QueryMediaInfo2
 * 函数功能: 对指定的媒体信息结点内容进行查询.
 * 输入参数: head 主链头指针
 *           reportdate 报道日期
 *           medianame 媒体名称
 * 输出参数: 无
 * 返 回 值: MEDIA_NODE 类型, 返回查询结果组成的新链头指针
 *
 * 调用说明: 以报道日期和媒体名称的一部分为条件查找并显示满足条件的媒体报道基本信息
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
 * 函数名称: HelpTheme
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
 BOOL HelpTheme(void)
{
    char *plabel_name[] = {"    欢迎与本系统开发者联系","",
                "作者邮箱：zmhustcsee@gmail.com","",
                "          U201511086@hust.edu.cn","",
                "          2856817781@qq.com","",
                "GitHub：  Githuberzhang","",
                           "确认"
                          };

    ShowModule(plabel_name, 11);

    return TRUE;
}

/**
 * 函数名称: HelpAbout
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
BOOL HelpAbout(void)
{
    char *plabel_name[] = {"   开发者：张铭","","",
                           "   华中科技大学","",
                           "计算机科学与技术学院","",
                           " 卓越工程师班1501",
                           "确认"
                          };

    ShowModule(plabel_name,9);
    return TRUE;
}

/**
 * 函数名称: ShowModule
 * 函数功能: 显示功能函数对应的界面
 * 输入参数: pString 用于显示数据的指针数组的地址
 *           n 显示数据的条数
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
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

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
 * 函数名称: SeekProvinceNode
 * 函数功能: 按所属省份查找案件基本信息结点
 * 输入参数:
 *           pvblg 所属省份
 * 输出参数: 无
 * 返 回 值: 查中时返回结点的地址, 否则返回NULL
 *
 * 调用说明:
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
 * 函数名称: SeekCaseNode
 * 函数功能: 按案件编号查找案件基本信息结点
 * 输入参数:
 *           casenum 案件编号
 * 输出参数: 无
 * 返 回 值: 查中时返回结点的地址, 否则返回NULL
 *
 * 调用说明:
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
 * 函数名称: SeekMediaNode
 * 函数功能: 按案件编号和媒体报道日期查找媒体信息节点
 * 输入参数:
 *           casenum 案件编号
 *           reportdate 报道日期
 * 输出参数: 无
 * 返 回 值: 查中时返回结点的地址, 否则返回NULL
 *
 * 调用说明:
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
 * 函数名称: FileDataSave
 * 函数功能: 保存三类基础数据
 * 输入参数: head主链头指针
 * 输出参数:
 * 返回值: BOOL类型，总为TURE
 *
 * 调用说明:
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
        char *plabel_name[] = {"数据保存失败",
                               "确认"
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
        char *plabel_name[] = {"数据保存失败",
                           "确认"};

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
        char *plabel_name[] = {"数据保存失败",
                           "确认"
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
    char *plabel_name[] = {"数据保存成功",
                           "确认"
                          };

    ShowModule(plabel_name, 2);

    return TRUE;
}

/**
 * 函数名称: FileDataBackup
 * 函数功能: 备份三类基础数据到指定文件中去
 * 输入参数: head 主链头指针
 *           filename 备份文件名
 * 输出参数:
 * 返回值: BOOL类型，总为TURE
 *
 * 调用说明:
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
        char *plabel_name[] = {"数据备份失败",
                           "确认"
                          };
        ShowModule(plabel_name, 2);
        return TRUE;
    }
    if(!fcase)
    {
        char *plabel_name[] = {"数据备份失败",
                           "确认"
                          };
        ShowModule(plabel_name, 2);
        return TRUE;
    }
    if(!fmedia)
    {
        char *plabel_name[] = {"数据备份失败",
                           "确认"
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

    char *plabel_name[] = {"省份信息数据已备份至",
                            probackupfile,
                           "案件信息数据已备份至",
                           casebackupfile,
                           "媒体报道数据已备份至",
                           mediabackupfile,
                           "确认"
                          };

    ShowModule(plabel_name, 7);
    fclose(fpro);
    fclose(fcase);
    fclose(fmedia);
    return TRUE;

}

/**
 * 函数名称: FileDataRestore_Interface
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
BOOL FileDataRestore_Interface(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"待导入的省份信息文件名称",
                   "                    ",
                   "待导入的案件信息文件名称",
                   "                    ",
                   "待导入的媒体报道文件名称",
                   "                    ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 16;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 20, srpop.Top + 14}};/*五个热区的位置*/
    char aSort[] = {1,1,1,0,0};
    char aTag[] = {1,2,3,4,5};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+13;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
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
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[3],&res);
	FillConsoleOutputAttribute(std_out,att2,20,aLoc[5],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return TRUE;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                        char *plabel_name[] = {"数据不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
 * 函数名称: FileDataRestore
 * 函数功能: 恢复三类基础数据
 * 输入参数: profile,casefile,mediafile 三类文件名
 * 输出参数:
 * 返回值: BOOL类型，总为TURE
 *
 * 调用说明:
 */
BOOL FileDataRestore(char *profile,char *casefile,char *mediafile)
{

    int re = ReCreateList(&head,profile,casefile,mediafile);
    if(re == 28)
    {
        char *plabel_name[] = {"数据载入成功","确认"};
        ShowModule(plabel_name,2);
        PopOff();
        return TRUE;
    }
    else
    {
        char *plabel_name[] = {"数据载入失败","确认"};
        ShowModule(plabel_name,2);
        PopOff();
        return TRUE;
    }
}

/**
 * 函数名称: ReCreateList
 * 函数功能: 重建链表
 * 输入参数: phead 主链头指针地址
 *           profile,casefile,mediafile 三类文件名
 * 输出参数:
 * 返回值: int型
 *
 * 调用说明:
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
        pPROVINCENODE->next = prohead;                 /*创建后进先出的链表*/
        prohead = pPROVINCENODE;                       /*创建后进先出的链表*/
    }
    fclose(pf);
    if(prohead == NULL)
    {
        return re;
    }
    *phead = prohead;                                  /*三方向十字交叉链表表头*/

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
            pCASENODE->next = pPROVINCENODE->cnext;         /*创建后进先出的链表*/
            pPROVINCENODE->cnext = pCASENODE;              /*创建后进先出的链表*/
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
 * 函数名称: FileExit
 * 函数功能: 弹出功能函数对应的界面退出系统
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
 */
BOOL FileExit(void)
{

    LABEL_BUNDLE labels;
    HOT_AREA areas;
    BOOL bRet = TRUE;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;
    char *pCh[] = {"确认退出系统吗？", "确定    取消"};
    int iHot = 1;

    pos.X = strlen(pCh[0]) + 6;
    pos.Y = 7;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
    labels.num = 2;
    labels.ppLabel = pCh;
    COORD aLoc[] = {{srpop.Left+3, srpop.Top+2},
                    {srpop.Left+5, srpop.Top+5}};
    labels.pLoc = aLoc;

    areas.num = 2;
    SMALL_RECT aArea[] = {{srpop.Left + 5, srpop.Top + 5,
                           srpop.Left + 8, srpop.Top + 5},
                          {srpop.Left + 13, srpop.Top + 5,
                           srpop.Left + 16, srpop.Top + 5}};/* "确定" 和 "取消" 按钮的热区位置*/
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
 * 函数名称: StatusCrimeName
 * 函数功能: 以主要罪名为条件分别统计历年来各种罪名贪腐案件的发生数、
 *           涉案金额，按案件发生数降序排序后，输出统计结果。
 * 输入参数: head 主链头指针
 * 输出参数: 无
 * 返 回 值: void
 * 调用说明:
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

	char *s1 = "历年贪腐案件统计";
	char *s2 = "主要罪名        案件数    涉案总金额(万元)";

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
				sprintf(p[i],"受贿                %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
				break;

			case '2':
				sprintf(p[i],"贪污                %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
				break;

			case '3':
				sprintf(p[i],"挪用公款            %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
				break;

			case '4':
				sprintf(p[i],"渎职                %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
                break;

			case '5':
				sprintf(p[i],"滥用职权            %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
				break;

			case '6':
				sprintf(p[i],"其他                %d           %-9.3f",crimecasemoney[i].sumcount,crimecasemoney[i].summoney);
				break;
            default:
                sprintf(p[i],"------------------------------------");
                break;
		}
	}

	char *pCh[] = {s1,s2,p[0],p[1],p[2],p[3],p[4],p[5],"确认"};
	int iHot = 1;

	pos.X = strlen(pCh[0]) + 30;
	pos.Y = 15;
	srpop.Left = (SCR_COL - pos.X) / 2;
	srpop.Right = srpop.Left + pos.X - 1;
	srpop.Top = (SCR_ROW - pos.Y) / 2;
	srpop.Bottom = srpop.Top + pos.Y - 1;

	att = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;  /*白底黑字*/
	labels.num = 9;
	labels.ppLabel = pCh;
	COORD aLoc[] = {{srpop.Left+15, srpop.Top+2},//历年贪腐案件统计

					{srpop.Left+3, srpop.Top+4},//主要罪名

					{srpop.Left+3, srpop.Top+6},
					{srpop.Left+3, srpop.Top+7},
					{srpop.Left+3, srpop.Top+8},
					{srpop.Left+3, srpop.Top+9},
					{srpop.Left+3, srpop.Top+10},
					{srpop.Left+3, srpop.Top+11},

					{srpop.Left+21, srpop.Top+13}};//确认
	labels.pLoc = aLoc;

	areas.num = 1;
	SMALL_RECT aArea[] = {{srpop.Left + 21, srpop.Top + 13,
						   srpop.Left + 24, srpop.Top + 13}};/*按钮的热区位置*/
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
 * 函数名称: StatusLevelPunish
 * 函数功能: 以行政级别为条件分别统计某年度内各种行政级别贪腐案件的发生数，
 *           其中刑事处罚为死刑、无期徒刑、有期徒刑1-5年、
 *           有期徒刑6-10及有期徒刑11年以上的案件分别是多少起。
 * 输入参数: head 主链头指针
 * 输出参数: 无
 * 返 回 值: void
 * 调用说明:
 */
BOOL StatusLevelPunish(void)
{
    PROVINCE_NODE *hd;
    CASE_NODE *casenode;
    /*初始化结构变量*/
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

	char *s1 = "2016年各行政级别贪腐案件及其刑事处罚统计";
	char *s2 = "行政级别       1-5年   6-10年   11年以上   无期徒刑   死刑";

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
				sprintf(p[i],"副科级             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;

			case 'B':
				sprintf(p[i],"正科级             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;

			case 'C':
				sprintf(p[i],"副初级             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;

			case 'D':
				sprintf(p[i],"正处级             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;
			case 'E':
				sprintf(p[i],"副厅级             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;

			case 'F':
				sprintf(p[i],"正厅级             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;

			case 'G':
				sprintf(p[i],"副部级             %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;
			case 'H':
				sprintf(p[i],"正部级及以上       %d        %d          %d          %d      %d",
                            levelcountpunishment[i].year[0],levelcountpunishment[i].year[1],
							levelcountpunishment[i].year[2],levelcountpunishment[i].year[3],levelcountpunishment[i].year[4]);
				break;
            default:
                sprintf(p[i],"----------------------------------------------------------");
                break;
		}
	}

	char *pCh[] = {s1,s2,p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],"确认"};
	int iHot = 1;

	pos.X = strlen(pCh[0]) + 24;
	pos.Y = 17;
	srpop.Left = (SCR_COL - pos.X) / 2;
	srpop.Right = srpop.Left + pos.X - 1;
	srpop.Top = (SCR_ROW - pos.Y) / 2;
	srpop.Bottom = srpop.Top + pos.Y - 1;

	att = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;  /*白底黑字*/
	labels.num = 11;
	labels.ppLabel = pCh;
	COORD aLoc[] = {{srpop.Left+12, srpop.Top+2},//历年贪腐案件统计

					{srpop.Left+3, srpop.Top+4},

					{srpop.Left+3, srpop.Top+6},
					{srpop.Left+3, srpop.Top+7},
					{srpop.Left+3, srpop.Top+8},
					{srpop.Left+3, srpop.Top+9},
					{srpop.Left+3, srpop.Top+10},
					{srpop.Left+3, srpop.Top+11},
					{srpop.Left+3, srpop.Top+12},
					{srpop.Left+3, srpop.Top+13},

					{srpop.Left+30, srpop.Top+15}};//确认
	labels.pLoc = aLoc;

	areas.num = 1;
	SMALL_RECT aArea[] = {{srpop.Left + 30, srpop.Top + 15,
						   srpop.Left + 33, srpop.Top + 15}};/*按钮的热区位置*/
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
 * 函数名称: StatusProUI
 * 函数功能: 显示查询日期的界面
 *
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: 总为TRUE
 * 调用说明:
 */
BOOL StatusProUI(void)
{
    LABEL_BUNDLE labels;
    HOT_AREA areas;
    SMALL_RECT srpop;
    COORD pos;
    WORD att;

    char *pCh[] = {"请输入起始日期",
                   "          ",
                   "请输入截止日期",
                   "          ",
                   "确定","取消"};
    int iHot = 1;

    pos.X = strlen(pCh[2]) + 6;
    pos.Y = 12;
    srpop.Left = (SCR_COL - pos.X) / 2;
    srpop.Right = srpop.Left + pos.X - 1;
    srpop.Top = (SCR_ROW - pos.Y) / 2;
    srpop.Bottom = srpop.Top + pos.Y - 1;

    att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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
                           srpop.Left + 15, srpop.Top + 10}};/*四个热区的位置*/
    char aSort[] = {1,1,0,0};
    char aTag[] = {1,2,3,4};
    areas.pArea = aArea;
    areas.pSort = aSort;
    areas.pTag = aTag;
    PopUp(&srpop, att, &labels, &areas);/*弹出录入省份信息的对话框*/
    /**< 画'-'处理界面 */
    pos.X=srpop.Left+1;
    pos.Y=srpop.Top+9;
    FillConsoleOutputCharacter(std_out,'-',srpop.Right-srpop.Left-1,pos,&ul);

    /**<以下处理对话框的输入 */
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
    char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/
    GetConsoleCursorInfo(std_out,&lpCur);
	lpCur.bVisible=TRUE;
	SetConsoleCursorInfo(std_out,&lpCur);
	SetConsoleCursorPosition(std_out, pos); // 设置光标位置
	WORD att2=BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY;  //信息输入区域背景色
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[1],&res);
	FillConsoleOutputAttribute(std_out,att2,10,aLoc[3],&res);
	SetHotPoint(&areas,iHot);
	while(1)
	{
		ReadConsoleInput(std_in, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
		{

            /* 按ESC键退出循环*/
			if (keyRec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyRec.Event.KeyEvent.bKeyDown)
            {
                GetConsoleCursorInfo(std_out,&lpCur);
                lpCur.bVisible=FALSE;
                SetConsoleCursorInfo(std_out,&lpCur);
                PopOff();
                return TRUE;
            }

			/*backspace键的处理*/
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

            /*enter 键的处理*/
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
                        char *plabel_name[] = {"日期不能为空，录入失败！","确认"};
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
                if(asc == 0)/*处理上下键*/
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
                    GetConsoleScreenBufferInfo(std_out,&bInfo); // 获取光标位置
                    pos=bInfo.dwCursorPosition;
                    WriteConsoleOutputCharacter(std_out, &ch, 1, pos, &ul);  //显示输入的字符
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
 * 函数名称: StatusProvince
 * 函数功能: 以案件发生地所属省份为条件分别统计某个时间段内各省份贪腐案件的发生数，
 *            涉案金额总数，按案件发生数降序排序后输出统计结果。
 * 输入参数: 开始日期与截止日期
 * 输出参数: 无
 * 返 回 值: void
 * 调用说明:
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
    sprintf(p[0],"%s--%s各省份贪腐案件统计",startdate,enddate);
    p[1] = "省份          案件数      涉案总金额";
    p[labelnum-1] = "确认";
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

	att = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;  //白底黑字
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
						    srpop.Left + 21, srpop.Top+7+cnt}};//按钮的热区位置
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
 * 函数名称: StatusReportTime
 * 函数功能: 统计媒体报道数最多的10个贪腐案件，按报道数降序排序后输出案件编号、
 *           被告行政级别、主要罪名、涉案金额、刑事处罚和媒体报道次数。
 * 输入参数: head 主链头指针
 * 输出参数: 无
 * 返 回 值: void
 * 调用说明:
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
            sum++;                                             /*得到总案件数*/
        }
    }
    struct MEDIAMOST mediamost[10];
    struct MEDIAMOST realcase[sum];
    /*初始化统计节点*/
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
            mediamost[i].classlevel1 = 'P';/*赋一个'A'~'H'无关的字母*/
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
						sprintf(p[i],"%s       副科级      受贿     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       副科级      贪污     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       副科级  挪用公款     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       副科级      渎职     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       副科级  滥用职权     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       副科级      其他     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'B':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       正科级      受贿     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       正科级      贪污     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       正科级  挪用公款     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       正科级      渎职     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       正科级  滥用职权     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       正科级      其他     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'C':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       副处级      受贿     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       副处级      贪污     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       副处级  挪用公款     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       副处级      渎职     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       副处级    滥用职权     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       副处级      其他     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'D':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       正处级      受贿     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       正处级      贪污     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       正处级  挪用公款     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       正处级      渎职     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       正处级  滥用职权     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       正处级      其他     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'E':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       副厅级      受贿     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       副厅级      贪污     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       副厅级  挪用公款     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       副厅级      渎职     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       副厅级  滥用职权     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       副厅级      其他     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);

						break;
				}
				break;
			case 'F':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       正厅级      受贿     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       正厅级      贪污     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       正厅级  挪用公款     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       正厅级      渎职     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       正厅级  滥用职权     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       正厅级      其他     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'G':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       副部级      受贿     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       副部级      贪污     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       副部级  挪用公款     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       副部级      渎职     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       副部级  滥用职权     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       副部级      其他     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
			case 'H':
				switch(mediamost[i].accuname1)
				{
					case '1':
						sprintf(p[i],"%s       正部级      受贿     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '2':
						sprintf(p[i],"%s       正部级      贪污     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '3':
						sprintf(p[i],"%s       正部级  挪用公款     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '4':
						sprintf(p[i],"%s       正部级      渎职     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '5':
						sprintf(p[i],"%s       正部级  滥用职权     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
					case '6':
						sprintf(p[i],"%s       正部级      其他     %-14.3f%-11d%d",
							mediamost[i].casenum1,mediamost[i].money1,mediamost[i].punish,mediamost[i].reportcount);
						break;
				}
				break;
            default :
                sprintf(p[i],"%s",line);
                break;
        }
	}


	char *pCh[] = {"媒体报道次数前十的贪腐案件详细信息"," 案件编号       行政级别   主要罪名  涉案金额   刑事处罚   报道次数",
                    p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],"确认"};
	int iHot = 1;

	pos.X = strlen(pCh[0]) + 40;
	pos.Y = 19;
	srpop.Left = (SCR_COL - pos.X) / 2;
	srpop.Right = srpop.Left + pos.X - 1;
	srpop.Top = (SCR_ROW - pos.Y) / 2;
	srpop.Bottom = srpop.Top + pos.Y - 1;

	att = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;  //白底黑字
	labels.num = 13;
	labels.ppLabel = pCh;
	COORD aLoc[] = {{srpop.Left+20, srpop.Top+2},//历年贪腐案件统计

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

					{srpop.Left+34, srpop.Top+17}};//确认
	labels.pLoc = aLoc;

	areas.num = 1;
	SMALL_RECT aArea[] = {{srpop.Left + 34, srpop.Top + 17,
						    srpop.Left + 37, srpop.Top + 17}};//按钮的热区位置
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
 * 函数名称: StatusReportType
 * 函数功能: 弹出功能函数对应的界面
 * 输入参数:
 * 输出参数: 无
 * 返 回 值: BOOL类型, 总为TRUE
 *
 * 调用说明:
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

	char *s1 = "贪腐案件报道统计";
	char *s2 = "媒体类别       总报道次数";

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
                sprintf(p[i],"报纸              %d",statusmediatype[i].cnt);
                break;
            case '2':
                sprintf(p[i],"广播              %d",statusmediatype[i].cnt);
                break;
            case '3':
                sprintf(p[i],"电视              %d",statusmediatype[i].cnt);
                break;
            case '4':
                sprintf(p[i],"互联网            %d",statusmediatype[i].cnt);
                break;
            default:
                sprintf(p[i],"--------------------");
        }
    }

	char *pCh[] = {s1,s2,p[0],p[1],p[2],p[3],"确认"};
	int iHot = 1;

	pos.X = strlen(pCh[0]) + 14;
	pos.Y = 13;
	srpop.Left = (SCR_COL - pos.X) / 2;
	srpop.Right = srpop.Left + pos.X - 1;
	srpop.Top = (SCR_ROW - pos.Y) / 2;
	srpop.Bottom = srpop.Top + pos.Y - 1;

	att = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;  /*白底黑字*/
	labels.num = 7;
	labels.ppLabel = pCh;
	COORD aLoc[] = {{srpop.Left+7, srpop.Top+2},//历年贪腐案件统计

					{srpop.Left+3, srpop.Top+4},

					{srpop.Left+5, srpop.Top+6},
					{srpop.Left+5, srpop.Top+7},
					{srpop.Left+5, srpop.Top+8},
					{srpop.Left+5, srpop.Top+9},

					{srpop.Left+13, srpop.Top+11}};//确认
	labels.pLoc = aLoc;

	areas.num = 1;
	SMALL_RECT aArea[] = {{srpop.Left + 13, srpop.Top + 11,
						   srpop.Left + 16, srpop.Top + 11}};/*按钮的热区位置*/
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
 * 函数名称: ExeFunction
 * 函数功能: 执行由主菜单号和子菜单号确定的功能函数.
 * 输入参数: m 主菜单项号
 *           s 子菜单项号
 * 输出参数: 无
 * 返 回 值: BOOL类型, TRUE 或 FALSE
 *
 * 调用说明: 仅在执行函数ExitSys时, 才可能返回FALSE, 其他情况下总是返回TRUE
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

    for (i=1,loc=0; i<m; i++)  /*根据主菜单号和子菜单号计算对应下标*/
    {
        loc += sub_menu_num[i-1];
    }
    loc += s - 1;

    if (pfun[loc] != NULL)
    {
        bo = (*pfun[loc])();  /*用函数指针调用所指向的功能函数*/
    }
    return bo;
}
