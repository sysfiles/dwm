#include "initData.h"
#include "Dwm_Dram.h"
extern Dwm_Dram* pDwm;
int nCnt; //�Ż����� ��¼����
namespace G
{
	HWND g_hwnd;
	HANDLE g_hanle;
	ULONG_PTR g_matrix_address;
	ULONG_PTR g_angle_address;
	ULONG_PTR g_self_address;
	ULONG_PTR g_players_address;
	ULONG_PTR g_players_bar;
	player_list g_players[g_players_count];
	Matrix4x4 g_matrix;
	RECT rect;
}

namespace Menum 
{
	BOOL show;
	BOOL rect;
	BOOL ambot;
	BOOL point;
}

void initialize_address()
{

	ULONG_PTR engine_module_address = (ULONG_PTR)GetModuleHandleW(L"engine.dll");
	ULONG_PTR client_module_address = (ULONG_PTR)GetModuleHandleW(L"client.dll");
	//ULONG_PTR server_module_address = (ULONG_PTR)GetModuleHandleW(L"server.dll");


	int matrix_address = engine_module_address + 0x596EF0;
	int angle_address = engine_module_address + 0x4622D0 - 4;
	int self_address = client_module_address + 0x04D3904;
	int players_address = client_module_address + 0x04D3904;
	ULONG_PTR players_bar = client_module_address + 0x04D3904;


	G::g_matrix_address = matrix_address;
	G::g_angle_address = angle_address;  //��� xy
	G::g_self_address = self_address;
	G::g_players_address = players_address;
	G::g_players_bar = players_bar;
	G::g_hanle = GetCurrentProcess();
	G::g_hwnd = FindWindowW(L"Valve001", L"Counter-Strike Source");
}

DWORD read_memory(HANDLE process, int address, void* recv, int size)
{
	DWORD read_size;
	//ReadProcessMemory(process, (LPCVOID)address, recv, size, &read_size);
	__try
	{
		RtlCopyMemory(recv, (LPCVOID)address, size);
		read_size = size;
	}
	__except (1)
	{
		RtlZeroMemory(recv, size);
		read_size = 0;
	}
	return read_size;
}


void add_play_list(player_list * pTagrgetPlayers, player_list * pTemp)
{
	Lock lock;

	RtlCopyMemory(pTagrgetPlayers, pTemp, sizeof(player_list));
}

void clear_play_list()
{
	Lock lock;

	nCnt = 0;
	RtlZeroMemory(G::g_players, sizeof(G::g_players));
}


void init_play_list(player_list * players)
{
	int player_base_address = 0;
	int bone_base_address = 0;
	ULONG offset1 = 0;
	ULONG offset2 = 0;
	ULONG offset3 = 0;
	ULONG offset4 = 0;
	player_list pTemp = { 0 };

	clear_play_list();


	for (int i = 0; i < g_players_count; i++)
	{
		player_base_address = 0;
		bone_base_address = 0;
		offset1 = 0;
		offset2 = 0;
		offset3 = 0;
		offset4 = 0;
		RtlZeroMemory(&pTemp, sizeof(player_list));


		read_memory(G::g_hanle, (G::g_players_address + i * 0x10), &player_base_address, sizeof(int));
		if (player_base_address == 0) continue;

		read_memory(G::g_hanle, player_base_address + Ѫ��, &pTemp.blood, sizeof(int));
		if (pTemp.blood <= 0) continue;

		read_memory(G::g_hanle, player_base_address + ����ʱ��, &pTemp.����ʱ��EX, sizeof(short));
		if (pTemp.����ʱ��EX != 0) {
			continue;
		}

		pTemp.effective = true;
		pTemp.aimbot_len = 9999;


		if (read_memory(G::g_hanle, (G::g_players_bar + i * 0x10), &offset1, sizeof(int)))
		{

			read_memory(G::g_hanle, (int)(offset1 + 0x14), &offset2, sizeof(float));
			read_memory(G::g_hanle, (int)(offset2 + 0x44), &offset3, sizeof(float));
			read_memory(G::g_hanle, (int)(offset3 + 0xc0), &offset4, sizeof(float));
			bone_base_address = offset4 + 0xc;

			//Ϊʲô +32 �� ��Ϊ �۲�������ݿ�֪  ��һ�� x���� �� �ڶ��� x���� ��� 32
			bone_base_address = bone_base_address + 21 * 32;//21��ͷ

			read_memory(G::g_hanle, (bone_base_address), &pTemp.head_bone.x, sizeof(float));
			read_memory(G::g_hanle, (bone_base_address + 16), &pTemp.head_bone.y, sizeof(float));
			read_memory(G::g_hanle, (bone_base_address + 32), &pTemp.head_bone.z, sizeof(float));

		}

		read_memory(G::g_hanle, player_base_address + ����Xƫ��, &pTemp.d3d_pos, sizeof(pTemp.d3d_pos));
		read_memory(G::g_hanle, player_base_address + ����ƫ��, &pTemp.camp, sizeof(int));


		add_play_list(&players[nCnt], &pTemp);
		nCnt++;

	}
	
}

void set_self_camp(player_list* players)
{
	ImVec3 self_pos = { 0 };
	get_self_location(&self_pos);

	for (int i = 0; i < g_players_count; i++)
	{
		if (players[i].effective)
		{

			int temp_x = abs((int)self_pos.x - (int)players[i].d3d_pos.x);
			int temp_y = abs((int)self_pos.y - (int)players[i].d3d_pos.y);
			if (temp_x < 10.0f && temp_y < 10.f)
			{
				//�ж� ����
				// ��������x,y��� ��ֻҪ����10.f ������Ϊ ���Լ�

				players[i].self = true;
				//InterlockedExchange((long*)&players[i].self, true);
				break;
			}
		}
	}
}

//��ȡ�Լ���λ��
void get_self_location(ImVec3* location)
{

	int location_base_address = 0;
	read_memory(G::g_hanle, G::g_self_address, &location_base_address, sizeof(int));
	if (location_base_address)
		read_memory(G::g_hanle, location_base_address + ����Xƫ��, location, sizeof(ImVec3));
}




//������ҷ���
void render_player_box(player_list* players)
{
	ImVec2 pos = { 0,0 };
	
	LONG window_x = 0; //�����󶥵�x
	LONG window_y = 0;//�����󶥵�y
	LONG window_w = 0;//���ڵĿ�
	LONG window_h = 0;//���ڵĸ�

	get_window_size(G::g_hwnd, window_x, window_y, window_w, window_h);
	window_w /= 2;
	window_h /= 2;

	RtlZeroMemory(&G::g_matrix, sizeof(Matrix4x4));
	read_memory(G::g_hanle, G::g_matrix_address, &G::g_matrix, sizeof(Matrix4x4));


	int self_camp = get_self_camp(players);

	for (int i = 0; i < g_players_count; i++)
	{
		if (players[i].effective  && players[i].self == false && players[i].blood > 1 && (players[i].����ʱ��EX == 0) && self_camp != players[i].camp)
		{
			int x=0, y=0, w=0, h=0;
			if (to_rect_info(&G::g_matrix, &players[i].d3d_pos, window_w, window_h, x, y, w, h))
			{
				if (IsPointInWindowsRect({ window_x ,window_y }, window_w * 2, window_h * 2, { x + window_x ,y + window_y }))
				{
					players[i].aimbot_len = get_aimbot_len(window_w, window_h, x + (w / 2), y + (h / 2));


					pDwm->Dwm_StartDraw();

					if (Menum::rect) 
					{
						x += window_x;
						y += window_y;

						pDwm->draw_rect(x, y, w, h, ��ɫ, 2);
					}
					if (Menum::point)
					{
						pos = { 0,0 };
						worldToScreen(&G::g_matrix, &players[i].head_bone, window_w, window_h, &pos);

						if (IsPointInWindowsRect({ window_x ,window_y }, window_w * 2, window_h * 2, { (LONG)pos.x + window_x ,(LONG)pos.y + window_y }))
						{
							pos.x += window_x;
							pos.y += window_y;

							pDwm->draw_text(pos.x , pos.y , ��ɫ, "*");
							//pDwm->DrawCircleFilled(pos.x + window_x, pos.y + window_y, 5.0f,��ɫ, 10);
						}
						
						
					}

					pDwm->Dwm_EndDraw();
					Sleep(1);

				}
			}
		}
	}
}


bool worldToScreen(IN Matrix4x4 * matrix, IN ImVec3 * world_pos, int window_width, int window_heigt, OUT ImVec2* ScreenPos)
{
	//���Z

	ImVec2 pos = { 0,0 };

	float to_target = matrix->m[2][0] * world_pos->x
		+ matrix->m[2][1] * world_pos->y
		+ matrix->m[2][2] * world_pos->z
		+ matrix->m[2][3];
	if (to_target < 0.01f)
	{
		ScreenPos->x = 0;
		ScreenPos->y = 0;
		return false;
	}

	to_target = 1.0f / to_target;

	//���x
	ScreenPos->x = window_width + (matrix->m[0][0] * world_pos->x
		+ matrix->m[0][1] * world_pos->y
		+ matrix->m[0][2] * world_pos->z
		+ matrix->m[0][3]) * to_target * window_width;


	ScreenPos->y = window_heigt - (matrix->m[1][0] * world_pos->x
		+ matrix->m[1][1] * world_pos->y
		+ matrix->m[1][2] * world_pos->z
		+ matrix->m[1][3]) * to_target * window_heigt;



	return true;
}
/*
�ж� ���Ƿ��ڴ��ڷ�Χ��
����һ:�����󶥵�
�����������ڿ�
������:���ڸ�
������: ��Ҫ�жϵĵ�
*/
BOOL IsPointInWindowsRect(POINT wnd_left_pos, ULONG ���ڿ�, ULONG ���ڸ�, POINT target_pos)
{


	if ((target_pos.x > wnd_left_pos.x) && (target_pos.y > wnd_left_pos.y) && (target_pos.x < (wnd_left_pos.x + ���ڿ�)) &&
		(target_pos.y < (wnd_left_pos.y + ���ڸ�)))
	{
		return TRUE;
	}

	return FALSE;
}


//��ȡ����λ�ô�С
void get_window_size(HWND target, LONG& x/*�󶥵�x*/, LONG& y/*�󶥵�y*/, LONG& width/*���ڿ�*/, LONG& height/*���ڸ�*/)
{
	//���ش����󶥵� x,y
	x = G::rect.left;
	y = G::rect.top;

	//���ش��ڵĴ�С
	width = G::rect.right - G::rect.left;
	height = G::rect.bottom - G::rect.top;

	if (GetWindowLongW(target, GWL_STYLE) & WS_CAPTION)
	{
		x += 8;
		width -= 8;
		y += 30;
		height -= 30;
	}
}

//��ȡ�Լ�����Ӫ
int get_self_camp(player_list* players)
{
	for (int i = 0; i < g_players_count; i++)
		if (players[i].effective && players[i].self) return players[i].camp;
	return 2;
}

//ת��Ϊ������Ϣ
bool to_rect_info(Matrix4x4* matrix, ImVec3* d3d_pos, int window_width, int window_heigt, int& x, int& y, int& w, int& h)
{
	//���Z
	float to_target = matrix->m[2][0] * d3d_pos->x
		+ matrix->m[2][1] * d3d_pos->y
		+ matrix->m[2][2] * d3d_pos->z
		+ matrix->m[2][3];
	if (to_target < 0.01f)
	{
		x = y = w = h = 0;
		return false;
	}

	to_target = 1.0f / to_target;

	//���x
	float to_width = window_width + (matrix->m[0][0] * d3d_pos->x
		+ matrix->m[0][1] * d3d_pos->y
		+ matrix->m[0][2] * d3d_pos->z
		+ matrix->m[0][3]) * to_target * window_width;

	//���y
	float to_height_h = window_heigt - (matrix->m[1][0] * d3d_pos->x
		+ matrix->m[1][1] * d3d_pos->y
		+ matrix->m[1][2] * (d3d_pos->z + 75.0f)/*���+75.f ��Ҫ���� ��*/
		+ matrix->m[1][3]) * to_target * window_heigt;

	float to_height_w = window_heigt - (matrix->m[1][0] * d3d_pos->x
		+ matrix->m[1][1] * d3d_pos->y
		+ matrix->m[1][2] * (d3d_pos->z - 5.0f)
		+ matrix->m[1][3]) * to_target * window_heigt;

	x = (int)(to_width - (to_height_w - to_height_h) / 4.0f);
	y = (int)(to_height_h);
	w = (int)((to_height_w - to_height_h) / 2.0f);
	h = (int)(to_height_w - to_height_h);
	return true;
}

//�����������
int get_aimbot_len(int window_w, int window_h, int x, int y)
{
	int temp_x = abs(window_w - x);
	int temp_y = abs(window_h - y);
	return (int)sqrt((temp_x * temp_x) + (temp_y * temp_y));
}

BOOL CNot(BOOL a)
{
	if (a)
	{
		return FALSE;
	}

	return TRUE;
}

//��ȡ����������״̬
bool get_mouse_left_down()
{
	return GetAsyncKeyState(VK_LBUTTON) & 0x8000;
}

//��ȡ������������λ��
int get_recent_head_location(player_list* players, ImVec3* self_location)
{
	int index = -1;
	int camp = get_self_camp(players);
	for (int i = 0; i < g_players_count; i++)
	{
		if (players[i].effective && camp != players[i].camp)
		{
			if (index == -1) index = i;
			else if (players[index].aimbot_len > players[i].aimbot_len) index = i;
		}
	}
	return index;
}

ULONG write_memory(HANDLE process, int address, void* data, int size)
{
	ULONG write_size = 0;
//	WriteProcessMemory(process, (LPVOID)address, data, size, &write_size);
	__try {
		RtlCopyMemory((LPVOID)address, data, size);
		write_size = size;
	}
	__except (1)
	{
		write_size = 0;
	}


	return write_size;
}

//��ȡ����Ƕ�
void get_aimbot_angle(ImVec3* self_location, ImVec3* player_location, float* aim_angle, bool squat, float recoil)
{
	float x = self_location->x - player_location->x;
	float y = self_location->y - player_location->y;
	float z = self_location->z - player_location->z + 65.0f + 15.0f;
	if (squat) z -= 15.0f;
	z += recoil;

	const float pi = 3.1415f;
	aim_angle[0] = (float)atan(z / sqrt(x * x + y * y)) / pi * 180.f;
	aim_angle[1] = (float)atan(y / x);

	if (x >= 0.0f && y >= 0.0f) aim_angle[1] = aim_angle[1] / pi * 180.0f - 180.0f;
	else if (x < 0.0f && y >= 0.0f) aim_angle[1] = aim_angle[1] / pi * 180.0f;
	else if (x < 0.0f && y < 0.0f) aim_angle[1] = aim_angle[1] / pi * 180.0f;
	else if (x >= 0.0f && y < 0.0f) aim_angle[1] = aim_angle[1] / pi * 180.f + 180.0f;
}
//���õ�ǰ�Ƕ�
void set_current_angle(float* angle)
{
	write_memory(G::g_hanle, G::g_angle_address, angle, sizeof(float) * 2);
}
//���鿪��
void aimbot_players(player_list* player, float max_fov)
{
	
	ImVec3 self_location = { 0 };
	get_self_location(&self_location);

	int aim_index = get_recent_head_location(player, &self_location);
	if (aim_index == -1) return;

	float current_angle[2] = { 0 };
	get_current_angle(current_angle);



	float aim_angle[2] = { 0 };
	get_aimbot_angle(&self_location, &player[aim_index].head_bone, aim_angle, FALSE, -12);

	if (abs((int)aim_angle[0] - (int)current_angle[0]) > max_fov
		|| abs((int)aim_angle[1] - (int)current_angle[1]) > max_fov)
		return;

	set_current_angle(aim_angle);
}
/*
��ȡ ���x
��ȡ ���y
*/
void get_current_angle(float* angle)
{

	read_memory(G::g_hanle, G::g_angle_address, angle, sizeof(float) * 2);
}
bool key_status(ULONG key)
{
	if (GetAsyncKeyState(key) & 1)
	{
		return true;
	}
	return false;
}


void monitor_key()
{

	if (key_status(VK_F1))
	{
		Menum::rect = CNot(Menum::rect);
	}

	if (key_status(VK_F2))
	{
		Menum::point = CNot(Menum::point);
	}
	if (key_status(VK_F3))
	{
		Menum::ambot = CNot(Menum::ambot);
	}
	if (key_status(VK_INSERT))
	{
		Menum::show = CNot(Menum::show);
	}

}

void dram_menum()
{
	if (!Menum::show)
	{
		pDwm->Dwm_StartDraw();
		pDwm->draw_text(50+ G::rect.left, 200 - 30+ G::rect.top, ��ɫ,"Ins / End ");
		pDwm->draw_text(50 + G::rect.left, 200 + G::rect.top, ��ɫ,(Menum::rect ? "F1 Rect On" : "F1 Rect Off") );
		pDwm->draw_text(50 + G::rect.left, 200 + 30 + G::rect.top, ��ɫ,(Menum::point ? "F2 point On" : "F2 point Off"));
		pDwm->draw_text(50 + G::rect.left, 200 + 60 + G::rect.top, ��ɫ,(Menum::ambot ? "F3 ambot On" : "F3 ambot Off"));
		
		pDwm->Dwm_EndDraw();
		Sleep(1);
	}
}






void render()
{

	static BOOL bOnce = FALSE;
	if (!bOnce)
	{
		initialize_address();
		bOnce = TRUE;
	}

	GetWindowRect(G::g_hwnd, &G::rect);

	dram_menum();
	monitor_key();

	init_play_list(G::g_players);

	set_self_camp(G::g_players);

	render_player_box(G::g_players);

	if (Menum::ambot) {
		if (get_mouse_left_down())
		{
			aimbot_players(G::g_players);
		}
	}
}

string get_distance_len(ImVec3* pos)
{
	string szTemp;
	ImVec3 self_pos = { 0 };
	get_self_location(&self_pos);

	ULONG temp_x = abs(pos->x - self_pos.x);
	ULONG temp_y = abs(pos->y - self_pos.y);
	ULONG temp_z = abs(pos->z - self_pos.z);


	return std::to_string((ULONG)sqrt((temp_x * temp_x) + (temp_y * temp_y) + (temp_z * temp_z)) - 32);

}
