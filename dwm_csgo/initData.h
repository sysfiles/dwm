#pragma once
#include <windows.h>
#include <mutex>
#include <string>
using namespace std;
#define  Ѫ�� 0x94
#define  ����ƫ��   0x9c
#define ����Xƫ��   0x260
#define  ����ʱ��  0x17e
//#define  ���� 0x286
//#define  �Ƿ��¶�  0xb38

#define  ��ɫ (D3DCOLOR_ARGB(255, 255, 255, 255))
#define  ��ɫ { 0, 255, 0, 255 }
#define  ��ɫ -65536
typedef struct _D3DPOS_
{
	float x;
	float y;
	float z;
}ImVec3;
struct player_list
{
	bool effective;//�Ƿ���Ч
	bool self;//���Լ�
	short ����ʱ��EX;
	int aimbot_len;//���鳤��
	int camp;//��Ӫ
	int blood;//Ѫ��
	ImVec3 d3d_pos;//��ά����
	ImVec3 head_bone;//����λ��
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
�ж� ���Ƿ��ڴ��ڷ�Χ��
����һ:�����󶥵�
�����������ڿ�
������:���ڸ�
������: ��Ҫ�жϵĵ�
*/
BOOL IsPointInWindowsRect(POINT wnd_left_pos, ULONG ���ڿ�, ULONG ���ڸ�, POINT target_pos);

void get_window_size(HWND target, LONG& x, LONG& y, LONG& width, LONG& height);
//��ȡ�Լ�����Ӫ
int get_self_camp(player_list* players);
//ת��Ϊ������Ϣ
bool to_rect_info(Matrix4x4* matrix, ImVec3* d3d_pos, int window_width, int window_heigt, int& x, int& y, int& w, int& h);
//�����������
int get_aimbot_len(int window_w, int window_h, int x, int y);

BOOL CNot(BOOL a);
//��ȡ����������״̬
bool get_mouse_left_down();

void aimbot_players(player_list* player, float max_fov = 30);
int get_recent_head_location(player_list* players, ImVec3* self_location);
void get_current_angle(float* angle);
//��ȡ����Ƕ�
void get_aimbot_angle(ImVec3* self_location, ImVec3* player_location, float* aim_angle, bool squat, float recoil);
void set_current_angle(float* angle);
ULONG write_memory(HANDLE process, int address, void* data, int size);
bool key_status(ULONG key);
void monitor_key();
void dram_menum();
string get_distance_len(ImVec3* pos);