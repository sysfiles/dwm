#pragma once
#include <windows.h>
#include <mutex>
#include <string>
using namespace std;
#define  血量 0x94
#define  阵型偏移   0x9c
#define 坐标X偏移   0x260
#define  更新时机  0x17e
//#define  护甲 0x286
//#define  是否下蹲  0xb38

#define  白色 (D3DCOLOR_ARGB(255, 255, 255, 255))
#define  绿色 { 0, 255, 0, 255 }
#define  红色 -65536
typedef struct _D3DPOS_
{
	float x;
	float y;
	float z;
}ImVec3;
struct player_list
{
	bool effective;//是否有效
	bool self;//是自己
	short 更新时机EX;
	int aimbot_len;//自瞄长度
	int camp;//阵营
	int blood;//血量
	ImVec3 d3d_pos;//三维坐标
	ImVec3 head_bone;//自瞄位置
};
const int g_players_count = 64;





struct Matrix4x4 {
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;

		};
		struct {
			float _11, _21, _31, _41;
			float _12, _22, _32, _42;
			float _13, _23, _33, _43;
			float _14, _24, _34, _44;

		};
		float m[4][4];
	};
};


class Lock 
{
private:
	static inline std::mutex mutex;
	std::scoped_lock<std::mutex> lock{ mutex };
};

struct ImVec2
{
	float  x, y;
};





void initialize_address();
void init_play_list(player_list * players);
DWORD read_memory(HANDLE process, int address, void* recv, int size);
void add_play_list(player_list * pTagrgetPlayers, player_list * pTemp);
void set_self_camp(player_list* players);
void get_self_location(ImVec3* location);
void render_player_box(player_list* players);
void render();
bool worldToScreen(IN Matrix4x4 * matrix, IN ImVec3 * world_pos, int window_width, int window_heigt, OUT ImVec2* ScreenPos);
void clear_play_list();
/*
判断 点是否在窗口范围内
参数一:窗口左顶点
参数二：窗口宽
参数三:窗口高
参数四: 需要判断的点
*/
BOOL IsPointInWindowsRect(POINT wnd_left_pos, ULONG 窗口宽, ULONG 窗口高, POINT target_pos);

void get_window_size(HWND target, LONG& x, LONG& y, LONG& width, LONG& height);
//获取自己的阵营
int get_self_camp(player_list* players);
//转化为矩阵信息
bool to_rect_info(Matrix4x4* matrix, ImVec3* d3d_pos, int window_width, int window_heigt, int& x, int& y, int& w, int& h);
//计算自瞄距离
int get_aimbot_len(int window_w, int window_h, int x, int y);

BOOL CNot(BOOL a);
//获取鼠标左键按下状态
bool get_mouse_left_down();

void aimbot_players(player_list* player, float max_fov = 30);
int get_recent_head_location(player_list* players, ImVec3* self_location);
void get_current_angle(float* angle);
//获取自瞄角度
void get_aimbot_angle(ImVec3* self_location, ImVec3* player_location, float* aim_angle, bool squat, float recoil);
void set_current_angle(float* angle);
ULONG write_memory(HANDLE process, int address, void* data, int size);
bool key_status(ULONG key);
void monitor_key();
void dram_menum();
string get_distance_len(ImVec3* pos);