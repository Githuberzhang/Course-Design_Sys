
Created by ZhangMing

All Rights Reserved

耗时两个月的管理系统

全部用C语言实现

没有使用任何图形库

控制台程序，调用若干Console API函数接口

pro.txt     case.txt      media.txt为测试数据

main.c  为主程序，tes.h为头文件




贪腐案件信息管理系统

1 需要处理的基础数据

对全国各省（直辖市、自治区）发生的贪腐案件信息进行管理，主要包括所属省份监管信息，贪腐案件基本信息，及媒体报道基本信息。

所属省份监管信息参考：

中文字段名	类型及长度	举例

所属省份	char[8]	“shanxi”    山西

纪检负责人	char[15]	“zhangsan”  张三

联系电话	char[15]	“0351*******”


贪腐案件基本信息参考：

中文字段名	类型及长度	               举例

案件编号	char[12]	“20090222001”  年+月+日+顺序编号

立案日期	char[10]	“yyyymmdd”

所属省份	char[8]	         “heilongjiang”   案件所属省份

被告人姓名	char[15]	“wangwu”      王五

行政级别	char	          ‘A’~’H’：分别代表副科级、正科级、副处级、正处级、副厅级、正厅级、副部级和正部级及以上

主要罪名	char	      ‘1’~‘6’：分别代表受贿、贪污、挪用公款、渎职、滥用职权和其他

涉案金额	float	              2000.0      单位：万元人民币

刑事处罚	int	          1~99：有期徒刑  大于等于100：无期徒刑  小于0：死刑



媒体报道基本信息参考：

中文字段名	类型及长度	举例

所属省份	char[8]	“shanxi”    山西

案件编号	char[12]	“20090222001”  年+月+日+顺序编号

报道日期	char[10]	“20090222”     年(4)+月(2)+日(2)

媒体类别	char		‘1’、‘2’、‘3’、‘4’   分别代表四大媒体：报纸、广播、电视和互联网

媒体名称	char[20]	“souhuxinwenwang”    搜狐新闻网

内容索引	char[50]	“http://news.sohu.com/20090222/ n262392247. shtml”



2 系统基本功能

本系统需要实现数据维护，数据查询和数据统计三个主要功能模块，另外根据情况添加辅助功能模块。下面给出了三个主要模块的功能需求，辅助功能模块根据各人的理解和分析自己设计。

(1) 数据维护

本模块实现对案件发生地所属省份的监管信息，贪腐案件的基本信息，及媒体报道基本信息等三方面基本信息的数据维护功能，又分为三个子模块。

1）所属省份监管信息维护

包括对所属省份监管信息的录入、修改和删除等功能。

2）贪腐案件基本信息维护

包括对贪腐案件基本信息的录入、修改和删除等功能。

3）媒体报道基本信息维护

包括对媒体报道基本信息的录入、修改和删除等功能。


(2) 数据查询

本模块实现对案件发生地所属省份的监管信息，贪腐案件的基本信息，及媒体报道基本信息等三方面基本信息的数据查询功能，又分为三个子模块。

1）所属省份监管信息查询

实现两个功能。

①以省份名称为条件来查找并显示满足条件的所属省份监管信息。例如，查找并显示所属省份为“shanxi”的监管信息。

②以纪检负责人姓名为条件来查找并显示满足条件的所属省份监管信息。例如，查找并显示纪检负责人姓名为“zhangsan”的所属省份监管信息。


2）贪腐案件的基本信息查询

实现两个功能。

①以案件编号为条件查找并显示满足条件的贪腐案件的基本信息。例如，查找并显示案件编号为“20080327005”的贪腐案件的基本信息。

②以被告姓名和行政级别为条件查找并显示满足条件的贪腐案件的基本信息。例如，查找并显示被告姓名为“wangwu”（王五）且行政级别为“D”（正处级）的所有贪腐案件的基本信息。

3）媒体报道基本信息查询

实现两个功能。

①以案件编号和媒体名称为条件查找并显示满足条件的媒体报道基本信息。例如，查找并显示事故编号为“20061102015”且媒体名称为“souhuxinwenwang”的媒体报 道基本信息。

②以报道日期和媒体名称的一部分为条件查找并显示满足条件的媒体报道基本信息。例如，查找并显示报道日期为“20090503”且媒体名称中包括字符子串“wang”的所有媒体报道基本信息。

(3) 数据统计

本模块实现五个方面的数据统计功能，前四个功能需求已给出，第五个自行设计。

①以主要罪名为条件分别统计历年来各种罪名贪腐案件的发生数、涉案金额，按案件发生数降序排序后，输出统计结果。

②以行政级别为条件分别统计某年度内各种行政级别贪腐案件的发生数，其中刑事处罚为死刑、无期徒刑、有期徒刑1-5年、有期徒刑6-10及有期徒刑11年以上的案件分别是多少起。

③以案件发生地所属省份为条件分别统计某个时间段内各省份贪腐案件的发生数，涉案金额总数，按案件发生数降序排序后输出统计结果。

④统计媒体报道数最多的10个贪腐案件，按报道数降序排序后输出案件编号、被告行政级别、主要罪名、涉案金额、刑事处罚和媒体报道次数。

⑤有关安全事故其他方面的数据统计。
