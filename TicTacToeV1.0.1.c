/*		20250319		
	I'm back!!!
*/

#include <stdio.h>
#include <stdlib.h>

const int side_len = 3;
//以下函数按照出现顺序排列
void intro( void );//用于介绍该款井字棋游戏
void Continue( void );//用于继续输出文本
void reset( int chess[][ side_len ] );//用于初始化/重置井字棋棋盘
void presentation( int chess[][ side_len ] );//用于展示每走一步棋后的棋盘
int check_win( int chess[][ side_len ] ) ;//用于检查每走一步棋是否产生胜、负、和

int main( int argc, const char *argv[] )
{
	int chess[ side_len ][ side_len ];//用一个3行3列的二维数组装棋盘
	//缺点:局限在3 x 3
	
	intro();//调用介绍函数
	Continue();//给玩家阅读时间	
	reset( chess );//调用重置函数
	Continue();//其实可以system( "pause" );printf( "\n" );
		  	            
	int row, column;//行,列,以及输入的数字(棋子)
	int cnt = 2;//除以2可以得到回合数(丰富内容,同时判断该哪方下棋)
	
	while ( 1 ){
		int is_o_turn = cnt % 2;//0表示轮到x,1表示轮到o
		if ( !is_o_turn ) printf( "\t第%d个回合!\n", cnt / 2);//没轮到o => 轮到x => 新一轮回合
		
		if ( is_o_turn ) printf( "该O下棋,请输入坐标:" );
		else printf( "该X下棋,请输入坐标:" );
		
		scanf( "%d %d", &row, &column );
		cnt++;//走一步棋
		
		if ( is_o_turn ) chess[ row - 1 ][ column - 1 ] = 0;//将对应坐标覆盖
		else chess[ row - 1 ][ column - 1 ] = 1;//is_o_turn => 默认为0, !is_o_turn => 1
		presentation( chess );//展示棋盘
		
		int checkwin = check_win( chess );//得到检测函数的返回值
		if ( checkwin == 0 ) continue;//没有输赢和棋产生,继续下棋
	//缺陷是:有些时候已经和棋,不需要继续下棋,但是程序设定要在初始棋盘覆盖完以后才能判断为和棋
		if ( checkwin == 'X' ) printf( "\t~~~X赢了~~~\n\n" );//利用制表符美化输出形式
		else if ( checkwin == 'O' )
			printf( "\t~~~O赢了~~~\n\n" );
		else//checkwin = 1
			printf( "\t~~~平局~~~\n\n" );
		
		int if_continue;
		printf( "是否继续游戏?继续请按1,结束请按0:\t" );
		scanf( "%d", &if_continue );
		
		if ( if_continue ){
			reset(chess);
			cnt = 2;
			getchar();//过滤Continue函数前的换行符
			Continue();
			continue;	
		}
		else{
			printf( "欢迎下次再来!(只需要按F11)\n" );
			break;
		}
	}
	
	return 0;
}

void intro( void )
{
	printf( "\t井字棋游戏!\n\n");//制表符美化输出形式,两个换行符拉开上下文间距
	printf( "规则:\n ");
	printf( "对战双方:X VS O, X先手\n" );
	printf( "输入格式:轮到某一方时,输入2个数字(以空格分开),分别表示行,列\n" );
	printf( "例如:轮到X时,输入3 1,表示在第3行第1列,即坐标(3,1)处放置X\n" );
	printf( "棋盘展示:每次输入都会展示棋盘情况,X用1表示,O用0表示\n" );
	printf( "注意:输入后记得按enter键!\n" );
	
	printf( "\n" );
}

void Continue( void )
{
	char ch;
	printf( "按enter键继续:\t" );
	scanf( "%c", &ch );
	//热知识:可以往函数头输入(scanf)东西,调用一次就要输入一次
	printf( "\n" );
}

void reset( int chess[][ side_len ] )
{
	printf( "棋盘初始化如下:\n" );
	
	printf( "行\\列  " );//两个\是一个\(逃逸字符的使用)
	for ( int j = 0; j < side_len; j++ ) printf( "%d列 ", j + 1 );
	printf( "\n" );
	
	for ( int i = 0; i < side_len; i++ ){
		printf( "第%d行  ", i + 1 );
		for ( int j = 0; j < side_len; j++ ){
			chess[i][j] = -1; // 将数组元素设为无效值
      // 不需要打印数组元素 => 每个格子就是空气
		}
		printf( "\n" );
	}
	
	printf( "\n" );
}

void presentation( int chess[][ side_len ] )
{
	printf( "棋盘展示如下:\n" );
	
	printf( "行\\列 " );
	for ( int j = 0; j < side_len; j++ ) printf( " %d列", j + 1 );
	printf( "\n" );
		
	for ( int i = 0; i < side_len; i++ ){
		printf( "第%d行  ", i + 1 );
		for ( int j = 0; j < side_len; j++ ){
			if ( chess[i][j] == -1 ) //printf("%d  ", chess[i][j]);
				printf( "   " );
			else printf( "  %d", chess[i][j] );
		}
		printf( "\n" );
	}
	
	printf( "\n" );
}

int check_win( int chess[][ side_len ] )
{
	int cnt1, cnt0;
	//检查每行有没有三个同样的棋子
	for ( int i = 0; i < side_len; i++ ){//控制行
		cnt1 = cnt0 = 0;
		for ( int j = 0; j < side_len; j++ ){//控制列
			if( chess[i][j] == 1 ) cnt1++;
			if( chess[i][j] == 0 ) cnt0++;
		}
		if ( cnt0 == side_len ) return 'O';//很巧妙,返回的实际是字母的ASCII码值
		if ( cnt1 == side_len ) return 'X';
	}
	//检查每列有没有三个同样的棋子
	for ( int j = 0; j < side_len; j++ ){//控制列
		cnt1 = cnt0 = 0;
		for ( int i = 0; i < side_len; i++ ){//控制行
			if( chess[i][j] == 1 ) cnt1++;
			if( chess[i][j] == 0 ) cnt0++;
		}
		if ( cnt0 == side_len ) return 'O';
		if ( cnt1 == side_len ) return 'X';
	}
	//检查每斜排(斜向右上)有没有三个同样的棋子
	//检查顺序:矩阵左上端 -> 右下端
	//缺点:此方法只能从矩形的一个角检查到矩阵对角线,另一半检查不到
	//优点:此方法适用性比只看chess[0][0],[1][1],[2][2]广
	//另外,这种检查方式建议草稿本画矩阵理解
	for ( int m = 0; m < side_len; m++ ){//控制斜排
		cnt1 = cnt0 = 0;
		for ( int i = 0; i <= m; i++ ){//控制行
			int j = m - i;//控制列
			if( chess[i][j] == 1 ) cnt1++;
			if( chess[i][j] == 0 ) cnt0++;
		}
		if ( cnt0 == side_len ) return 'O';
		if ( cnt1 == side_len ) return 'X';
	}
	//检查对角线有没有三个同样的棋子
	cnt1 = cnt0 = 0;
	for ( int i = 0, j = 0; i < side_len; j = ++i ){
		if( chess[i][j] == 1 ) cnt1++;
		if( chess[i][j] == 0 ) cnt0++;
	}
	if ( cnt0 == side_len ) return 'O';
	if ( cnt1 == side_len ) return 'X';
	
	cnt1 = cnt0 = 0;
	for ( int i = 0, j = 2; i < side_len; ++i, --j ){
		if( chess[i][j] == 1 ) cnt1++;
		if( chess[i][j] == 0 ) cnt0++;
	}
	if ( cnt0 == side_len ) return 'O';
	if ( cnt1 == side_len ) return 'X';
	//检查是不是和棋(不是和棋的话已经在上面就结束了)
	int if_tie = 1;
	for ( int i = 0; i < side_len; i++ ){//控制行
		for ( int j = 0; j < side_len; j++ ){//控制列
			if ( chess[i][j] == -1 ){
				if_tie = 0;
				break;
			}
		}
		if ( !if_tie ) break;
	}
	//缺点是必须要棋盘被棋子占满才可以
	return if_tie;
}
