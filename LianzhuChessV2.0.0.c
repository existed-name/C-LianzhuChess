/*			20250319
	众所周知,我对编程是热爱的(手动狗头)
	但是最近几天的刷题、数据结构学习比较坐牢,
再加上各种负面情绪堆积有点多,于是今天下午刷/
学不动了,不想刷题了,心累...
	想起昨天刷知乎看到的"胖猫事件"(胖猫热爱电
竞,工作是代打,每天工作13小时以上),有个知友在
分析时提到了一句,大意是:每天繁重的工作,再强烈
的热爱也会被消磨"
	再回看我刚接触C语言的时候,那个是真的热爱
--虽然刚开始还很无知...然后接触OJ刷题,刷久了
(虽然也只有少少几个星期),感觉变味了,没刚开始
学C的激情快乐了...
	直到后来我开始做小项目的时候,我才重新找回
以前的快乐、痴迷
	那句话确实很有道理
	不过终究是跑不脱刷题的,只能说多调整自己,
同时保证必要的做项目--刷题为了巩固数据结构/算法
能力,最终也是为了我更好做项目的
*/

/*		
			项目开发
(一)功能实现:
1.玩法介绍
 (1)可以用2个字符串数组分别装游戏模式名称及介绍,
    每个模式用1个数字表示
 (2)设置函数用于介绍游戏模式
 (3)可以给出注意事项
2.模式选择
 (1)设置函数作为各个模式的中转站,通过在该函数中做
    出选择跳转到相应模式
 (2)返回值为选择模式对应的
3.模式游玩
 (1)每个模式用1个函数封装
 (2)选择该模式后可以给出一些提示
 (3)创建(并初始化)棋盘,每一步棋展示棋盘,最后重置
    棋盘,可以分别用函数实现
 (4)通过用户输入点坐标在二维数组相应位置放置棋子,
    棋子的种类由轮到哪一方决定
 (5)走子后胜负和的判断,也是封装到函数中,返回值为
    int => 可以return 'X', 'O'表示X/O获胜,以及返
	回是否为和棋
4.续玩选项
 (1)封装到1个函数中,返回的特定值代表继续该模式/切
    换模式/退出游戏
	
(二)细节优化:
	1.防止用户输入已存在棋子的坐标进而覆盖该棋子
	2.提供更多棋子选择 => O, X, 0, 1, √...甚至可
	  以自定义
	3.由用户决定哪个棋子先手、后手
	4.黑窗口展示内容的整齐度、美观度
*/

#include <stdio.h>
#include <stdlib.h>

/*				*/
typedef char data_type;
#define SEP_LINE "--------------------------------"
#define RIGHT_ANGLE_BRACKET ">>>>" // 放需要用户输入的语句前
#define LEFT_ANGLE_BRACKET "<<<<" 
#define BACK_SLASH4 "\\\\\\\\"
#define SHORT_DASH4 "----" // 类似二级标题
#define WAVY_LINE4 "~~~~"
#define POUND_KEY4 "####" // 警示作用
#define SPACE4 "    " // 首行缩进
#define SPACE3 "   "
#define SPACE2 "  " // 隔开冒号和输入数据
#define SPACE1 " "
#define SPACE ' ' // 棋盘空位
#define STAR4 "****" // 类似一级标题
#define PUT_1_LINE_FEED printf( "\n" )
#define PUT_2_LINE_FEED printf( "\n\n" )
// 1个\t => 8个字符宽度
enum MODE 
{ 
	tic_tac_toe_ = 1, gobang_, custom_, 
	barrier_, remove_, vanish_, bomb_,
};// 本来之前是从0开始,但是发现太多地方是从1开始, 于是也改为从1开始
const int mode_num = 7;

enum END_OPTION
{
	end_game_, continue_mode_, switch_mode_,
};

/*				*/
typedef struct _point_node
{
	int row;
	int col;
	struct _point_node *next;
} PtNode;

typedef struct _point_list
{
	PtNode *head;
	PtNode *tail;
} PtList;

/*				*/
int int_input( void );
void game_intro( void );
const char *mode_name_get( const int mode_option );
void mode_brief_intro( void );
void pause( void );
int mode_select( void );
int mode_jump_to( const int mode_option );
int get_userinput_idx( const int user_input );
/*				*/
void tic_tac_toe_mode( void );
void gobang_mode( void );
void custom_mode( void );
/*				*/
void mode_detail_intro( const int mode_option );
void game_prepare_stage( const int mode_option, int *side_len, int *chess_in_a_line,
						 data_type *player1, data_type *player2 );
void game_play_core( const int side_len, const int chess_in_a_line, 
					 const data_type player1, const data_type player2 );
data_type **board_create( const int side_len );
void *safe_malloc( const size_t size );
void board_free( data_type **board, const int side_len );
void board_init( data_type **board, const int side_len );
void board_reset( data_type **board, PtList *list );
void chess_put( data_type **board, const int side_len, const data_type player, PtList *list );
void board_show( data_type **board, const int side_len );
int check_win( data_type **board, const int side_len, const data_type player1, 
			   const data_type player2, const int chess_in_a_line );
int continue_option( void );
/*				*/
PtList *list_create( void );
PtNode *node_create( const int row, const int col );
void list_push_back( PtList *list, const int row, const int col );
int list_empty( PtList *list );
PtNode *list_pop_front( PtList *list );
void list_clear( PtList *list );

int main( int argc, const char *argv[] )
{
	game_intro();
	int mode_option = -1, end_option = -1;
	for ( int i = 0; ; ++i ){
		if ( end_option == continue_mode_ ) mode_option = mode_jump_to( mode_option );
		else mode_option = mode_select();
		end_option = continue_option();
		if ( end_option == end_game_ ) break;
	}
	
	return 0;
}

int int_input( void )
{
	int integer;
	scanf( "%d", &integer );
	return integer;
}

void game_intro( void )
{
	printf( "\t\t欢迎来到连珠棋游戏!\n" );
	pause();
	
	const int sentence = 7;
	char *game_intro_arr[] = {
		"游戏规则",
		"游戏玩法:双人游戏",
		"输入格式:轮到某一方时,输入2个数字(用空格分开),分别表示行、列,即可在对应位置放置棋子",
		"例如:轮到X时,输入3 1,表示在第3行第1列,即坐标(3,1)处放置X",
		"游戏效果:每次输入都会展示棋盘情况",
		"游戏结果:达到特定条件即可获胜",
		"PS:每次输入后按下enter键才有效!",
	};
	
	for ( int i = 0; i < sentence; ++i ){
		if ( i == 0 ){
			printf( SPACE4 BACK_SLASH4 "%s" BACK_SLASH4, game_intro_arr[i] );
		} else {
			PUT_2_LINE_FEED;
			printf( SPACE4 "%s", game_intro_arr[i] );
			if ( i == sentence - 1 ) PUT_1_LINE_FEED;
		}
	}
	
	pause();
	mode_brief_intro();
	pause();
}

const char *mode_name_get( int mode_option )
{
	const char *mode_name[] = {
		"井字棋模式",
		"五子棋模式",
		"自定义模式",
		"障碍模式",
		"消除模式",// 原"覆盖模式",cover_
		"消失模式",
		"扫雷模式",
	};
	int mode_idx = get_userinput_idx( mode_option );
	return mode_name[ mode_idx ];
}

void mode_brief_intro( void )
{	
	char *mode_brief_intro_[] = {
		"经典3X3棋盘,每一行/列/斜线上满3个相同棋子即可获胜",
		"9X9棋盘,玩法就不介绍了",
		"自定义棋盘边长(正方形)、形成连线需要的棋子个数(连珠数)、使用的棋子种类",
		"自定义+每隔一段时间棋盘上会刷新障碍,障碍处不能放置棋子",
		"自定义+可将棋盘上任意点清除",
		"自定义+每隔一段时间会消失一些棋子",
		"自定义+棋子放在有地雷的地方会爆炸",
	};
	
	printf( SPACE4 BACK_SLASH4 "游戏模式介绍" BACK_SLASH4 "\n" );
	for ( int i = 0; i < mode_num; ++i ){
		printf( "\n" SPACE4 "模式%d: %s--%s\n", i + 1, mode_name_get( i + 1 ), mode_brief_intro_[i] );
	}
}

void pause( void )
{
	PUT_2_LINE_FEED;
	printf( WAVY_LINE4 );
	system( "pause" );
	PUT_2_LINE_FEED;
}

int mode_select( void )
{
	int option;
	printf( RIGHT_ANGLE_BRACKET "请输入你选择的模式序号(记得按enter键):" SPACE2 );
	do {
		option = int_input();
		PUT_1_LINE_FEED;
		mode_jump_to( option );
		// 由于option还要用作循环条件判断,这里不可以直接jump_to_mode( int_input() );
	} while ( !( tic_tac_toe_ <= option && option <= custom_ ) );
	
	return option;
}

int mode_jump_to( int mode_option )
{
/*	本来mode_select( void )函数中是一段switch-case,后头
我想单独写一个mode_continue( int mode_option )函数,专门
用于继续当前模式,但是发现写出来跟mode_select( void )里头
的switch-case基本上一样,于是为了避免重复,就把这段switch-
case提取出来单独成了jump_to_mode( int mode_option )函数
*/	
	switch ( mode_option ){
		case tic_tac_toe_ : tic_tac_toe_mode(); break;
		case gobang_ : gobang_mode(); break;
		case custom_ : custom_mode(); break;
		case barrier_ : 
		case remove_ : 
		case vanish_ : 
		case bomb_ : printf( POUND_KEY4 "模式开发中,请重新选择:" SPACE2 ); break;
		default : printf( POUND_KEY4 "你输入的序号有误,请重新输入:" SPACE2 ); break;
	}
	return mode_option;
}

int get_userinput_idx( int user_input )
{
/* 	对用户输入的序号进行偏移调整,使其适应从0开始的数组索引
	通过这个函数,可以确保用户输入与程序内部数据结构的兼容性,
简化了在代码其他部分处理用户输入时的逻辑--by AI
*/  int internal_idx = user_input - 1;
    return internal_idx;
}

void tic_tac_toe_mode( void )
{
	mode_detail_intro( tic_tac_toe_ );
/*
	const int side_len, chess_in_a_line;
	const data_type player1, player2;
	game_prepare_stage( tic_tac_toe_, &side_len, &chess_in_a_line, &player1, &player2 );
	编译器警告[Warning] passing argument 3 of 'game_prepare_stage' discards 'const' qualifier 
from pointer target type [-Wdiscarded-qualifiers]
	我的本意是在tic_tac_toe_mode( void )函数内设定const变量防止被修改,而传入const变量的指针
是为了初始化const变量( 局部const变量可以通过指针间接修改 )
	知乎直答跟我讲"尽管技术上可以通过指针绕过const限制,但这是不良的编程实践",那我也只能暂时
放弃了
*/	
	int side_len, chess_in_a_line;
	data_type player1, player2;
	game_prepare_stage( tic_tac_toe_, &side_len, &chess_in_a_line, &player1, &player2 );
	game_play_core( side_len, chess_in_a_line, player1, player2 );
}

void gobang_mode( void )
{
	mode_detail_intro( gobang_ );
	int side_len, chess_in_a_line;
	data_type player1, player2;
	game_prepare_stage( gobang_, &side_len, &chess_in_a_line, &player1, &player2 );
	game_play_core( side_len, chess_in_a_line, player1, player2 );
}

void custom_mode( void )
{
	mode_detail_intro( custom_ );
	int side_len, chess_in_a_line;
	data_type player1, player2;
	game_prepare_stage( custom_, &side_len, &chess_in_a_line, &player1, &player2 );
	game_play_core( side_len, chess_in_a_line, player1, player2 );	
}

void mode_detail_intro( const int mode_option )
{
	PUT_1_LINE_FEED;
	printf( SPACE4 STAR4 STAR4 "%s" STAR4 STAR4 "\n", mode_name_get( mode_option ) );
	const char* mode_detail_intro_[] = {
		"",
		"",
		"",
		"障碍物由#表示,每次刷新的数量、位置随机~~~",
		"可消除对手、己方的棋子,空格被消除后会变成虚空@",
		"棋子随机消失,也有可能什么都没发生...",
		"地雷随机刷新,踩到后以自身为中心3x3范围内爆炸",
	};
	int mode_idx = get_userinput_idx( mode_option );
	printf( "%s\n", mode_detail_intro_[ mode_idx ] );
}

void game_prepare_stage( const int mode_option, int *side_len, int *chess_in_a_line,
						 data_type *player1, data_type *player2 )
{	
	switch ( mode_option ){
		case tic_tac_toe_ : 
			*side_len = 3; *chess_in_a_line = 3; 
			*player1 = 'X'; *player2 = 'O'; 
			break;
		case gobang_ : 
			*side_len = 9; *chess_in_a_line = 5; 
			*player1 = 'X'; *player2 = 'O'; 
			break;
		case custom_ : 
		case barrier_ : 
		case remove_ : 
		case vanish_ : 
		case bomb_ : 
			printf( RIGHT_ANGLE_BRACKET "请设置棋盘边长、连珠数,中间用空格隔开:" SPACE2 );
			scanf( "%d %d", side_len, chess_in_a_line );
			printf( RIGHT_ANGLE_BRACKET "请输入先手、后手使用的棋子,中间用空格隔开(eg:1 a表示先手为'1',后手为'a'):" SPACE2 );
			scanf( "\n%c %c", player1, player2 );//输入1个字符后可以输入多个字符或者换行再输入1个字符
			break;
		default : printf( POUND_KEY4 "你输入的序号有误,请重新输入:" SPACE2 ); break;
	}
	
	printf( SPACE4 "对战双方:%c VS %c, %c先手\n", *player1, *player2, *player1 );
	printf( SPACE4 "准备好了吗?祝你们好运!\n" );
	pause();
}

void game_play_core( const int side_len, const int chess_in_a_line, 
					 const data_type player1, const data_type player2 )
{
	data_type **board = board_create( side_len );
	board_init( board, side_len );
	board_show( board, side_len );
	PtList *list = list_create();
	
	for ( int step_cnt = 0; ; ++step_cnt ){
		if ( step_cnt % 2 == 0 ){
			printf( SPACE4 SHORT_DASH4 "第%d回合" SHORT_DASH4, step_cnt / 2 + 1 );
			PUT_2_LINE_FEED;
			printf( SPACE4 "轮到%c下棋:" SPACE2, player1 );
			chess_put( board, side_len, player1, list );
		} else {
			printf( SPACE4 "轮到%c下棋:" SPACE2, player2 );
			chess_put( board, side_len, player2, list );
		}
		PUT_1_LINE_FEED;
		board_show( board, side_len );
		
		int game_result = check_win( board, side_len, player1, player2, chess_in_a_line );
		int if_break = 1;
		switch ( game_result ) {
			case 0 :
				if_break = 0;
				continue;// break;
			case 1 :
				printf( SPACE4 STAR4 STAR4 "平局" STAR4 STAR4 );	
				break;
			default :
				// player1 OR 2
				printf( SPACE4 STAR4 STAR4 "%c获胜"  STAR4 STAR4, game_result );
				break;
		}
		if ( if_break ) break;
	}
	PUT_2_LINE_FEED;
	list_clear( list );// 虽然reset函数中已经清了,但是保险起见再清理一遍
}

data_type **board_create( const int side_len )
{
	data_type **board = ( data_type **) safe_malloc( sizeof( data_type * ) * side_len );
	if ( board == NULL ){
		printf( POUND_KEY4 POUND_KEY4 "行指针内存分配失败!请重启游戏!\n" POUND_KEY4 POUND_KEY4 );
		exit( EXIT_FAILURE );
	}
	
	for ( int i = 0; i < side_len; ++i ){
		board[i] = NULL;// 行指针初始化
	}
	
	for ( int i = 0; i < side_len; ++i ){
		board[i] = ( data_type *) safe_malloc( sizeof( data_type ) * side_len );
		if ( board[i] == NULL ){
			printf( POUND_KEY4 POUND_KEY4 "行内存分配失败!请重启游戏!\n" POUND_KEY4 POUND_KEY4 );
			board_free( board, side_len );
			exit( EXIT_FAILURE );
		}
	}
	
	return board;
}

void *safe_malloc( const size_t size )
{
	void *ptr = NULL;
	ptr = malloc( size );
	if ( ptr == NULL ){
		fprintf( stderr, POUND_KEY4 POUND_KEY4 "%s函数未能成功分配%u字节内存!\n" POUND_KEY4 POUND_KEY4, __func__, size );
//		fprintf( stderr, "\t%s函数未能成功分配%u字节内存!请重新加载游戏!\n", __func__, size );
//		exit( EXIT_FAILURE );
//		直接eixt会导致不知道哪个地方的函数调用者发生错误,所以在函数调用者处进行exit,便于故障定位
	}
	return ptr;
}

void board_free( data_type **board, const int side_len )
{
	for ( int row = 0; row < side_len; ++row ){
		if ( board[ row ] == NULL ) break;
		free( board[ row ] );
		board[ row ] = NULL;
	}
	free( board );
	board = NULL;
}

void board_init( data_type **board, const int side_len )
{
	for ( int row = 0; row < side_len; ++row ){
		for ( int col = 0; col < side_len; ++ col ){
			board[ row ][ col ] = SPACE;
		}
	}
/*	当时问知乎直答:char *arr = ''; printf( "%s", arr[0] );
的结果,虽然说应该改成printf( "%s", arr );但是它讲到" '\0'不
产生可见字符"时突然给了我灵感 => 可以尝试'\0'而不是空格
	之后再问,空格打印出来是占了1个位置的,但是'\0'不占位置 =>
'\0'要多打印1个空格来保持输出的棋盘整齐
*/	
}

void board_reset( data_type **board, PtList *list )
{
/*
	for ( int row = 0; row < side_len; ++row ){
		for ( int col = 0; col < side_len; ++ col ){
			board[ row ][ col ] = ' ';
		}
	}
	算了,n^2的复杂度,干脆重新建一个
	
	board_free( board, side_len );
	board = board_create( side_len );
	但是每个点必须有个东西,放置棋子的时候才好检查可不可以放,
展示棋盘的时候才好打印
	
	最后还是考虑单独建一个初始化函数,reset函数选择将特定位置
重置为空格,就可以避免嵌套for访问二维数组,进而降低时间复杂度
*/	
	while ( !list_empty( list ) ){
		PtNode *node = list_pop_front( list );
		board[ node -> row - 1 ][ node -> col - 1 ] = SPACE;
	}
}

void chess_put( data_type **board, const int side_len, const data_type player, PtList *list )
{
	int row = int_input(), col = int_input();
	int is_row_ok = ( 1 <= row && row <= side_len ), is_col_ok = ( 1 <= col && col <= side_len );
	
	while ( !( is_row_ok && is_col_ok ) ){
		printf( POUND_KEY4 "你输入的坐标越界了,请重新输入:" SPACE2 );
		row = int_input(), col = int_input();
		is_row_ok = ( 1 <= row && row <= side_len ), is_col_ok = ( 1 <= col && col <= side_len );
	}
	
	if ( board[ row - 1 ][ col - 1 ] == SPACE ){
		is_row_ok = is_col_ok = 1;
	} else {
		is_row_ok = is_col_ok = 0;
	}
	
	while ( !( is_row_ok && is_col_ok ) ){
		printf( POUND_KEY4 "这个位置已经被占了,请重新输入:" SPACE2 );
		row = int_input(), col = int_input();
		if ( board[ row - 1 ][ col - 1 ] == SPACE ){
			is_row_ok = is_col_ok = 1;
		} else {
			is_row_ok = is_col_ok = 0;
		}
	}
	
	board[ row - 1 ][ col - 1 ] = player;
	list_push_back( list, row, col );
}

void board_show( data_type **board, const int side_len )
{
	printf( "棋盘展示如下" RIGHT_ANGLE_BRACKET "\n"
			"行\\列" );
			
	for ( int i = 0; i < side_len; ++i ){
		printf( SPACE2 "%d列", i + 1 );
	}
	PUT_1_LINE_FEED;
	
	for ( int row = 0; row < side_len; ++row ){
		printf( "第%d行", row + 1 );
		for ( int col = 0; col < side_len; ++col ){
			printf( SPACE3 "%c" SPACE1, board[ row ][ col ] );
		}
		PUT_2_LINE_FEED;
	}
/*	细细研究一下棋盘在终端的展示情况
	----棋盘展示如下----
	行\列  1列  2列  3列
	第1行    X    O    X
	
	第2行    X    X    O
	
	第3行    O    O    X
	问题主要在每列棋子与"i列"的对齐,之前试的输出2个空格,
再输出棋子,发现棋子位置逐渐左偏;而输出4个空格再输出棋子,
发现棋子位置逐渐右偏;3个空格的时候右总感觉差了一点
	汉字通常占2个字符宽度,那么每组"  i列"就要占5个字符宽
度 => 以5个字符宽度为完整1列的完全宽度。我们想要棋子(1个字符
宽度)放在"i列"的中间 => "i列"的第2个字符宽度处 => "  i列"
即完整1列的第4个字符宽度位置,最后1个位置(第5个字符宽度处)
用空格补上
	于是终于得到解决方案 => SPACE3 + 棋子 + SPACE
*/
}

int check_win( data_type **board, const int side_len, const data_type player1, 
			   const data_type player2, const int chess_in_a_line )
{
//	chess_in_a_line,表示连珠数 => 连成1条线(获胜)所需的棋子数
/*	我想设计1个通用的函数用来检查任意边长棋盘双方的获胜情况,
常规思路是对棋盘的行、列、斜线分三部分检查,但是由于扫描了很多
没有棋子的格子,会导致时间复杂度很高,那么我在想可不可以把记录
每步棋坐标的链表分成2部分 => 得到对战双方的棋子分布情况,根据
分布情况判断是否产生"连珠"
*/	

//	常规思路:行、列、斜线三部分检查棋盘
	for ( int row = 0; row < side_len; ++row ){
		int cnt_player1 = 0, cnt_player2 = 0;
		for ( int col = 0; col < side_len; ++col ){
			if ( board[ row ][ col ] == player1 ) ++cnt_player1;
			if ( board[ row ][ col ] == player2 ) ++cnt_player2;
		}
		if ( cnt_player1 >= chess_in_a_line ) return player1;
		if ( cnt_player2 >= chess_in_a_line ) return player2;
	}
	
	for ( int col = 0; col < side_len; ++col ){
		int cnt_player1 = 0, cnt_player2 = 0;
		for ( int row = 0; row < side_len; ++row ){
			if ( board[ row ][ col ] == player1 ) ++cnt_player1;
			if ( board[ row ][ col ] == player2 ) ++cnt_player2;
		}
		if ( cnt_player1 >= chess_in_a_line ) return player1;
		if ( cnt_player2 >= chess_in_a_line ) return player2;
	}
	
/*	斜线采用"向量法"检查,确定斜线向量在二维数组中的起点、终点,
一直从起点遍历到终点即可,之后再平移斜线向量 => 移动起点、
终点,继续遍历斜线
*/	

/*	
	int start_row = 0, start_col = 0, end_row = side_len - 1, end_col = side_len - 1;
	// 主对角线及其以上的斜线
	for ( ; start_col < side_len && end_row > -1; ++start_col, --end_row ){
		// 每条斜线的遍历
		int cnt_player1 = 0, cnt_player2 = 0;
		for ( int row = start_row, col = start_col; row <= end_row && col <= end_col; ++row, ++col ){
			if ( board[ row ][ col ] == player1 ) ++cnt_player1;
			if ( board[ row ][ col ] == player2 ) ++cnt_player2;
		}
		if ( cnt_player1 >= chess_in_a_line ) return player1;
		if ( cnt_player2 >= chess_in_a_line ) return player2;
	}
	// 主对角线以下的斜线
	start_row = 1, start_col = 0, end_row = side_len - 1, end_col = side_len - 2;
		// 由于主对角线已经遍历过,这里把起点终点下移
	for ( ; start_row < side_len && end_col > -1; ++start_row, --end_col ){
		// 每条斜线的遍历
		int cnt_player1 = 0, cnt_player2 = 0;
		for ( int row = start_row, col = start_col; row <= end_row && col <= end_col; ++row, ++col ){
			if ( board[ row ][ col ] == player1 ) ++cnt_player1;
			if ( board[ row ][ col ] == player2 ) ++cnt_player2;
		}
		if ( cnt_player1 >= chess_in_a_line ) return player1;
		if ( cnt_player2 >= chess_in_a_line ) return player2;
	}
	// ↑↑↑没写好,其实可以合并,斜线向量从一个顶点往对顶点平移
*/	
//	斜线:主对角线方向 || 向量方向:左上 →右下 || 向量平移方向:右上 →左下(可以由起点决定,也可以决定起点、循环结束条件)
	int start_row = 0, start_col = side_len - 1, end_row = 0, end_col = side_len - 1;
	for ( ; start_row < side_len && end_col > -1; ){
		int cnt_player1 = 0, cnt_player2 = 0;
		for ( int row = start_row, col = start_col; row <= end_row && col <= end_col; ++row, ++col ){
			if ( board[ row ][ col ] == player1 ) ++cnt_player1;
			if ( board[ row ][ col ] == player2 ) ++cnt_player2;
		}
		if ( cnt_player1 >= chess_in_a_line ) return player1;
		if ( cnt_player2 >= chess_in_a_line ) return player2;
		
		if ( start_col > 0 && end_row < side_len - 1 ){
			// 对角线上半部分
			--start_col;
			++end_row;
			// 起点从( 0, side_len - 1 )左移到( 0,0 ),终点从( 0, side_len - 1 )下移到( side_len - 1, side_len - 1 )
		} else {
			// 对角线下半部分
			++start_row;
			--end_col;
			// 起点从( 0,0 )下移到( side_len - 1, 0 ),终点从( side_len - 1, side_len - 1 )左移到( side_len - 1, 0 )
		}
	}

//	斜线:次对角线方向 || 向量方向:左下 →右上 || 向量平移方向:左上 →右下
	start_row = 0, start_col = 0 ,end_row = 0, end_col = 0;
	for ( ; start_col < side_len && end_row < side_len; ){
		int cnt_player1 = 0, cnt_player2 = 0;
		for ( int row = start_row, col = start_col; row >= end_row && col <= end_col; --row, ++col ){
			if ( board[ row ][ col ] == player1 ) ++cnt_player1;
			if ( board[ row ][ col ] == player2 ) ++cnt_player2;
		}
		if ( cnt_player1 >= chess_in_a_line ) return player1;
		if ( cnt_player2 >= chess_in_a_line ) return player2;
		
		if ( start_row < side_len - 1 && end_col < side_len - 1 ){
			// 起点从( 0,0 )下移到( side_len - 1, 0 ),终点从( 0,0 )右移到( 0, side_len - 1 )
			++start_row;
			++end_col;
		} else {
			// 起点从( side_len - 1, 0 )右移到( side_len - 1, side_len - 1 ),终点从( 0, side_len - 1 )下移到( side_len - 1, side_len - 1 )
			++start_col;
			++end_row;
			// 注意:应该由 "最后检查的半个棋盘的" "向量平移时起点、终点发生改变的" 横/纵坐标(通常是起点终点各1各)作循环结束条件
		}
	}
	
//	剩下的情况 => 和棋OR对局未结束
/*	如何判断和棋?
	最简单方法:上面没测出胜负,那么这里只要棋子没摆满棋盘 => 对局未结束,否则和棋。
但是这种方法可能只适用于井字棋、5子棋、自定义模式,另外时间复杂度也高
	也想过求出链表长度 => 双方下棋步数 => 棋盘上棋子数, 跟棋盘格子数比较得结果,
但是这种方法也只适用于井字棋、5子棋、自定义模式
*/
	int if_tie = 1;
	for ( int row = 0; row < side_len; ++row ){
		for ( int col = 0; col < side_len; ++col ){
			if ( board[ row ][ col ] == SPACE ){
				if_tie = 0;
				break;
			}
		}
		if ( !if_tie ) break;
	}
	return if_tie;
}

int continue_option( void )
{
	printf( "\n" SPACE4 "退出游戏请按0,继续该模式请按1,切换模式请按2:" SPACE2 );
	int option = int_input();
	PUT_1_LINE_FEED;
	while( !( 0 <= option && option <= 2 ) ){
		printf( SPACE4 "你输入的选项有误,请重新输入:" SPACE2 );
		option = int_input();
	}
	if ( !option ) printf( "\t\t欢迎下次再来!\n" );
	return option;
}

PtList *list_create( void )
{
	PtList *list = ( PtList *) safe_malloc( sizeof( PtList ) );
	if ( list == NULL ){
		printf( "\t链表创建失败,请重启游戏!\n" );
		exit( EXIT_FAILURE );
	}
	list -> head = list -> tail = NULL;
	return list;
}

PtNode *node_create( const int row, const int col )
{
	PtNode *node = ( PtNode *) safe_malloc( sizeof( PtNode ) );
	if ( node == NULL ){
		printf( "\t代表坐标( %d,%d )的结点创建失败,请重启游戏!\n", row, col );
		exit( EXIT_FAILURE );
	}
	node -> row = row;
	node -> col = col;
	node -> next = NULL;
	return node;
}

void list_push_back( PtList *list, const int row, const int col )
{
	PtNode *node = node_create( row, col );
	if ( list_empty( list ) ){
		list -> head = list -> tail = node;
	} else {
		list -> tail = list -> tail -> next = node;
	}
}

int list_empty( PtList *list )
{
	return ( list -> head == NULL );
}

PtNode *list_pop_front( PtList *list )
{
	PtNode *old_head = list -> head;
	if ( old_head ){
		PtNode *new_head = old_head -> next;
		free( list -> head );
		list -> head = new_head;
		if ( new_head == NULL ){
			//说明链表只有1个结点,原来表头 = 表尾
			list -> tail = NULL;
		}
	}
	return old_head;
}

void list_clear( PtList *list )
{
/*	for ( PtNode *curr = list -> head; curr; ){
		PtNode *next = curr -> next;
		free( curr );
		curr = next;
	}
	
	while ( list -> head ){
		Node *temp = list -> head;
		list -> head = temp -> next;
		free( temp )
	}
	其实主要是写来玩的,毕竟目前链表就记录每个
棋子的位置,还没其他用处,而且在board_reset()函数
中,一直pop,最后是不需要clear的
*/	
	while ( !list_empty( list ) ){
		list_pop_front( list );
	}
/*	甚至不用写list -> head = list -> tail = NULL;
因为list_pop_front( list );在最后已经执行了
*/
}
