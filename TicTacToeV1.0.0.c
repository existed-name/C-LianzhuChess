/*
					引言
    2024年11月21日星期4至11月24日星期7,用时不低于8小时,
这款井字棋小游戏终于初步完成,版本号为V1.0.0.20241124_Alpaha.
    游戏创作灵感其实来源于B站翁恺老师在"8.2.3二维数组"
(第79集),这本来只是一项以课堂活动形式呈现的任务,但是却
让我"饥渴难耐".
	我认为,编程不应该只是沉迷于算法题(当然,这是每个coder
都要经历的),还应该重视项目实现,能将一个程序跑起来,实现
跳出算法题的功能,满足日常使用需求(用户需求)(包括多方面,
网购、支付、游戏、浏览等等),将自己的代码"变现",是coder
的最大乐趣之一
	相对于几乎任何游戏,这款游戏可以说是so easy,so weak,
然而,我却从中得到了很多成就感,很多快乐--凌驾于AC之上的
快乐,如同当初打印出hello world一般的快乐,这可能是计算机
专业最吸引人的一个地方了.
	当然,限于知识水平以及经验,本款游戏不足之处尚多,我需
要更新的地方也很多,不过我已经做得很好了.
	我纪念这一天,2024年11月24号星期天,以及这个时刻13:40
(左右),即使这最初的版本号会随着时间的流逝而变化,最终离开
我的脑海.
*/


#include <stdio.h>

//以下函数按照出现顺序排列
void intro(void);//用于介绍该款井字棋游戏
void Continue(void);//用于继续输出文本
void reset(int chess[][3]);//用于初始化/重置井字棋棋盘
void presentation(int chess[][3]);//用于展示每走一步棋后的棋盘
int check_win(int chess[][3]);//用于检查每走一步棋是否产生胜、负、和
//建议阅读主函数中,遇到上述函数时前往上述函数块阅读该函数,便于看懂代码

int main(void)
{
	int chess[3][3];//用一个3行3列的二维数组装棋盘
	//缺点:局限在3 x 3
	
	intro();//调用介绍函数
	Continue();//给玩家阅读时间	
	reset(chess);//调用重置函数
	Continue();
		  	            
	int row,column,num;//行,列,以及输入的数字(棋子)
	int cnt = 2;//除以2可以得到回合数(丰富内容,同时判断该哪方下棋)
	
	while (1){
		if ( cnt % 2 == 0 ) printf("\t第%d个回合!\n", cnt / 2);//也可以写if ( !( cnt % 2 ) )
		
		if ( cnt % 2 == 0 ) printf("该X下棋,输入数据:");
		else printf("该O下棋,输入数据:");
		
		scanf("%d %d %d", &row, &column, &num);
		cnt++;//走一步棋
		
		chess[row - 1][column - 1] = num;//将对应坐标覆盖
		presentation(chess);//展示棋盘
		
		int checkwin = check_win(chess);//得到检测函数的返回值
		if ( checkwin == 0 ) continue;//没有输赢和棋产生,继续下棋
	//缺陷是:有些时候已经和棋,不需要继续下棋,但是程序设定要在初始棋盘覆盖完以后才能判断为和棋
		if ( checkwin == 'X' ) printf("\t~~~X赢了~~~\n\n");//利用制表符美化输出形式
		else if ( checkwin == 'O' )
			printf("\t~~~O赢了~~~\n\n");
		else//checkwin = 1
			printf("\t~~~平局~~~\n\n");
		
		int if_continue;
		printf("是否继续游戏?继续请按1,结束请按0:\t");
		scanf("%d", &if_continue);
		
		if ( if_continue ){
			reset(chess);
			cnt = 2;
			getchar();//过滤Continue函数前的换行符
			Continue();
			continue;	
		}
		else{
			printf("\t欢迎下次再来!(只需要按F11)\n\n");
			break;
		}
	}
	
	return 0;
}

void intro()
{
	printf("\t井字棋游戏!\n\n");//制表符美化输出形式,两个换行符拉开上下文间距
	printf("规则:\t");
	printf("X用1表示,O用0表示;X先手\n");
	printf("输入时,输入3个数字,分别表示行,列,以及棋子,再按enter键即可\n");
	printf("例如:3 1 1,表示在第3行第1列放置'1',即X");
	printf("\n\n");
}

void Continue(void)
{
	char ch;
	printf("按enter键继续:\t");
	scanf("%c", &ch); // 才知道函数里面也可以 scanf
	printf("\n");
}

void reset(int chess[][3])
{
	printf("棋盘初始化如下:\n");
	
	printf("行\\\\列  ");//两个\是一个\(逃逸字符的使用)
	for ( int j = 0; j < 3; j++ ) printf("%d列 ", j + 1);
	printf("\n");
	
	for ( int i = 0; i < 3; i++ ){
		printf("第%d行  ", i + 1);
		for ( int j = 0; j < 3; j++ ){
			chess[i][j] = -1;
			printf("%d  ", chess[i][j]);
		}
		printf("\n");
	}
	
	printf("\n\n");
}

void presentation(int chess[][3])
{
	printf("棋盘展示如下:\n");
	
	printf("行\\\\列  ");
	for ( int j = 0; j < 3; j++ ) printf("%d列 ", j + 1);
	printf("\n");
		
	for ( int i = 0; i < 3; i++ ){
		printf("第%d行  ", i + 1);
		for ( int j = 0; j < 3; j++ ){
			if ( chess[i][j] == -1 ) printf("%d  ", chess[i][j]);
			else printf(" %d  ", chess[i][j]);
		}
		printf("\n");
	}
	
	printf("\n");
}

int check_win(int chess[][3])
{
	//注意:不能用sum = 0,再对一排或者一列求和,sum = 0不能说明是三个0,另外sum = 3只在3子棋有效,4子棋部分有效
	int cnt1;
	int cnt0;
	//检查每行有没有三个同样的棋子
	for ( int i = 0; i < 3; i++ ){//控制行
		cnt1 = 0;
		cnt0 = 0;
		for ( int j = 0; j < 3; j++ ){//控制列
			if( chess[i][j] == 1 ) cnt1++;
			if( chess[i][j] == 0 ) cnt0++;
		}
		if ( cnt0 == 3 ) return 'O';
		if ( cnt1 == 3 ) return 'X';
	}
	//检查每列有没有三个同样的棋子
	for ( int j = 0; j < 3; j++ ){//控制列
		cnt1 = 0;
		cnt0 = 0;
		for ( int i = 0; i < 3; i++ ){//控制行
			if( chess[i][j] == 1 ) cnt1++;
			if( chess[i][j] == 0 ) cnt0++;
		}
		if ( cnt0 == 3 ) return 'O';
		if ( cnt1 == 3 ) return 'X';
	}
	//检查每斜排(斜向右上)有没有三个同样的棋子
	//检查顺序:矩阵左上端 -> 右下端
	//缺点:此方法只能从矩形的一个角检查到矩阵对角线,另一半检查不到
	//优点:此方法适用性比只看chess[0][0],[1][1],[2][2]广
	//另外,这种检查方式建议草稿本画矩阵理解
	for ( int m = 0; m < 3; m++ ){//控制斜排
		cnt1 = 0;
		cnt0 = 0;
		for ( int i = 0; i <= m; i++ ){//控制行
			int j = m - i;//控制列
			if( chess[i][j] == 1 ) cnt1++;
			if( chess[i][j] == 0 ) cnt0++;
		}
		if ( cnt0 == 3 ) return 'O';
		if ( cnt1 == 3 ) return 'X';
	}
	//检查每斜排(斜向右下)有没有三个同样的棋子
	//检查顺序:矩阵左下端 -> 右上端(注:此方法只能检查到矩阵对角线)
	for ( int m = 2; m >= 0; m-- ){//控制斜排
		cnt1 = 0;
		cnt0 = 0;
		for ( int i = m; i <= 2; i++ ){//控制行
			int j = i - m;//控制列
			if( chess[i][j] == 1 ) cnt1++;
			if( chess[i][j] == 0 ) cnt0++;
		}
		if ( cnt0 == 3 ) return 'O';
		if ( cnt1 == 3 ) return 'X';
	}
	//检查是不是和棋(不是和棋的话已经在上面就结束了)
	int if_tie = 0;
	for ( int i = 0; i < 3; i++ ){//控制行
		for ( int j = 0; j < 3; j++ ){//控制列
			if ( chess[i][j] == -1 ) if_tie = 0;
			else if_tie = 1;
		}
	}
	//if_tie = 0,说明不是和棋,继续下棋, = 1,则是和棋
	return if_tie;
}
