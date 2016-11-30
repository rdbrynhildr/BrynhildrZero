
#include					<winsock.h>
#include					<olectl.h>
#include					<audioclient.h>
#include					"dsound.h"

#pragma						comment(lib,"wsock32")
#pragma						comment(lib,"dsound")
#pragma						comment(lib,"imm32")
#pragma						comment(lib,"winmm.lib")

char						*g_connect_ver;						//�ʐM�o�[�W����
char						*g_ip;								//IP
unsigned short				g_port;								//Port
char						*g_encryption_key;					//�p�X���[�h�i�ő�16byte�j
SOCKET						g_sock1;							//�\�P�b�g�i����n�j
SOCKET						g_sock2;							//�\�P�b�g�i�摜�n�j
SOCKET						g_sock3;							//�\�P�b�g�i�����n�j
long						g_sock1_ct;							//����n�ڑ��J�E���g
long						g_sock2_ct;							//�摜�n�ڑ��J�E���g
struct sockaddr_in			g_addr;

long						g_end;								//�I���t���O
HWND						g_hwnd;								//�E�C���h�E�n���h��
long						g_window_cx;						//�E�C���h�E��
long						g_window_cy;						//�E�C���h�E����
long						g_capture_cx;						//�L���v�`���[��
long						g_capture_cy;						//�L���v�`���[����
long						g_mouse_x;							//�}�E�X���W�w��
long						g_mouse_y;							//�}�E�X���W�x��
char						g_mouse_left[256];					//�}�E�X�{�^���i���j
char						g_mouse_right[256];					//�}�E�X�{�^���i�E�j
char						g_mouse_middle[256];				//�}�E�X�{�^���i���j
char						g_mouse_wheel[256];					//�}�E�X�z�C�[��
char						g_mouse_x1[256];					//�}�E�X�{�^���i�߂�j
char						g_mouse_x2[256];					//�}�E�X�{�^���i�i�ށj
char						g_keyboard1[256];					//�L�[�{�[�h
char						g_keyboard2[256];					//�L�[�{�[�h
long						g_shift_down;						//SHIFT�L�[
long						g_alt_down;							//ALT�L�[
long						g_ctrl_down;						//CTRL�L�[

LPDIRECTSOUND				g_ds;
LPDIRECTSOUNDBUFFER			g_dsb;
DSBUFFERDESC				g_ds_desc;
WAVEFORMATEXTENSIBLE		*g_ca_format;
char						*g_ds_sound_buf;
long						g_ds_sound_pt;

struct COM_DATA													//�w�b�_�[�i256byte�j
{
	char					data_type;							//1:�ʏ�f�[�^
	char					thread;								//1:����n,2:�摜�n,3:�����n

	char					___filler_1[1];

	char					sound_type;							//0:�����Ȃ�,1:��������
	char					encryption;							//0:��Í����ʐM

	char					___filler_2[3];

	long					data_size;							//�f�[�^�T�C�Y

	char					___filler_3[4];

	char					check_digit_enc[16];				//�`�F�b�N�f�B�W�b�g�i�n�b�V�����j

//32

	long					check_digit;						//�`�F�b�N�f�B�W�b�g
	char					ver[4];								//�ʐM�o�[�W����
	long					samplerate;							//�T���v�����[�g
	long					image_cx;							//�摜��
	long					image_cy;							//�摜����
	long					server_cx;							//�T�[�o�[����ʕ�
	long					server_cy;							//�T�[�o�[����ʍ���
	long					control;							//1:����

//64

	char					mouse_move;							//0:�}�E�X�Î~,1:�}�E�X����

	char					___filler_4[1];

	char					mouse_x1;							//�}�E�X�߂�{�^���i1:�_�E��,2:�A�b�v,3:�_�u���N���b�N�j
	char					mouse_x2;							//�}�E�X�i�ރ{�^���i1:�_�E��,2:�A�b�v,3:�_�u���N���b�N�j
	long					mouse_x;							//�}�E�X���W�w��
	long					mouse_y;							//�}�E�X���W�x��
	char					mouse_left;							//�}�E�X���{�^���i1:�_�E��,2:�A�b�v,3:�_�u���N���b�N�j
	char					mouse_right;						//�}�E�X�E�{�^���i1:�_�E��,2:�A�b�v,3:�_�u���N���b�N�j
	char					mouse_middle;						//�}�E�X���{�^���i1:�_�E��,2:�A�b�v,3:�_�u���N���b�N�j
	char					mouse_wheel;						//�}�E�X�z�C�[���ړ���
	char					keycode;							//�L�[�R�[�h
	char					keycode_flg;						//�L�[�t���O�i0x00:KeyUp,0x80:KeyDown�j

	char					___filler_6[2];

	char					monitor_no;							//���j�^�[�ԍ�
	char					monitor_count;						//���j�^�[��

	char					___filler_7[6];

	long					sound_capture;						//0:DirectSound,1:CoreAudio

//96

	char					___filler_8[40];

	long					keydown;							//�L�[�����i1:��������,0:�����Ȃ��j
	long					video_quality;						//�掿�i1:�Œ�掿,3:��掿,5:�W���掿,7:���掿,9:�ō��掿�j

//144

	long					mouse_cursor;						//�}�E�X�J�[�\���\���i0:����,1:�\���Œ�,2:��\���Œ�j

	char					___filler_9[20];

	long					gamepad1;							//�Q�[���p�b�h�iXpos�j
	long					gamepad2;							//�Q�[���p�b�h�iYpos�j
	long					gamepad3;							//�Q�[���p�b�h�iZpos�j
	long					gamepad4;							//�Q�[���p�b�h�iRpos�j

	long					client_scroll_x;					//�X�N���[���ʒu�w��
	long					client_scroll_y;					//�X�N���[���ʒu�x��

//192

	char					___filler_10[24];

	double					zoom;								//�g�嗦�i1.0:���{�j

//224

	char					___filler_11[4];

	long					mode;								//5:�p�u���b�N���[�h
	long					sound_quality;						//�����i1:�Œቹ��,2:�ቹ��,3:�W������,4:������,5:�ō������j

	char					___filler_12[4];

	long					gamepad5;							//�Q�[���p�b�h�iPov�j
	long					gamepad6;							//�Q�[���p�b�h�iButtons�j
	long					gamepad7;							//�Q�[���p�b�h�iUpos�j
	long					gamepad8;							//�Q�[���p�b�h�iVpos�j

//256

};

//��������������
bool ds_init(long p_samplerate)
{
	DirectSoundCreate(0,&g_ds,0);

	if (g_ds == 0)
	{
		return false;
	}

	g_ds->SetCooperativeLevel(GetDesktopWindow(),DSSCL_NORMAL);

	g_ca_format = (WAVEFORMATEXTENSIBLE*)malloc(sizeof(WAVEFORMATEXTENSIBLE));

	memset(g_ca_format,0,sizeof(WAVEFORMATEXTENSIBLE));

	g_ca_format->Format.wFormatTag = WAVE_FORMAT_PCM;
	g_ca_format->Format.wBitsPerSample = 16;
	g_ca_format->Format.nSamplesPerSec = p_samplerate;
	g_ca_format->Format.nChannels = 2;
	g_ca_format->Format.nBlockAlign = g_ca_format->Format.nChannels * (g_ca_format->Format.wBitsPerSample / 8);
	g_ca_format->Format.nAvgBytesPerSec = g_ca_format->Format.nBlockAlign * g_ca_format->Format.nSamplesPerSec;

	memset(&g_ds_desc,0,sizeof(DSBUFFERDESC));

	g_ds_desc.dwSize = sizeof(DSBUFFERDESC);
	g_ds_desc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLFREQUENCY;
	g_ds_desc.dwBufferBytes = g_ca_format->Format.nAvgBytesPerSec * 3;
	g_ds_desc.lpwfxFormat = &g_ca_format->Format;

	g_ds->CreateSoundBuffer(&g_ds_desc,&g_dsb,0);

    return true;
}

//�����������
void ds_exit()
{
	if (g_dsb != 0)
	{
		g_dsb->Stop();
		g_dsb->Release();
		g_dsb = 0;
	}

	if (g_ds != 0)
	{
		g_ds->Release();
		g_ds = 0;
	}

	if (g_ca_format != 0)
	{
		free(g_ca_format);
		g_ca_format = 0;
	}
}

//���M����
long send_data(SOCKET *p_sock,char *p_buf,long p_size)
{
	char key[16 + 1];
	memset(&key,'@',sizeof(key));
	key[16] = 0x00;

	for (long sx1 = 0;sx1 < 16;sx1 ++)
	{
		if (g_encryption_key[sx1] == 0x00)
		{
			break;
		}

		key[sx1] = g_encryption_key[sx1];
	}

	COM_DATA *com_data = (COM_DATA*)p_buf;
	long data_long = com_data->data_type + (com_data->data_size & 0x0000ffff);

	com_data->encryption = 0;
	com_data->check_digit = ~data_long;

	for (long sx1 = 0;sx1 < 16;sx1 ++)
	{
		char key_char = ~key[sx1];
		key_char += (char)(sx1 * ~com_data->check_digit);
		com_data->check_digit_enc[sx1] = key_char;
	}

	return send(*p_sock,p_buf,p_size,0);
}

//��M����
long recv_data(SOCKET *p_sock,char *p_buf,long p_size)
{
	long size = 0;

	for (;size < p_size;)
	{
		long ret = recv(*p_sock,p_buf + size,p_size - size,0);

		if (ret > 0)
		{
			size += ret;
		}
		else
		{
			size = -1;

			break;
		}
	}

	return size;
}

//�}�E�X����
void mouse_send(char p_type,char p_value)
{
	for (long sx1 = 0;sx1 < 256;sx1 ++)
	{
		if (p_type == 0)
		{
			if (g_mouse_left[sx1] == 0)
			{
				g_mouse_left[sx1] = p_value;

				break;
			}
		}

		if (p_type == 1)
		{
			if (g_mouse_right[sx1] == 0)
			{
				g_mouse_right[sx1] = p_value;

				break;
			}
		}

		if (p_type == 2)
		{
			if (g_mouse_middle[sx1] == 0)
			{
				g_mouse_middle[sx1] = p_value;

				break;
			}
		}

		if (p_type == 3)
		{
			if (g_mouse_wheel[sx1] == 0)
			{
				g_mouse_wheel[sx1] = p_value;

				break;
			}
		}

		if (p_type == 4)
		{
			if (g_mouse_x1[sx1] == 0)
			{
				g_mouse_x1[sx1] = p_value;

				break;
			}
		}

		if (p_type == 5)
		{
			if (g_mouse_x2[sx1] == 0)
			{
				g_mouse_x2[sx1] = p_value;

				break;
			}
		}
	}
}

//�L�[�{�[�h����
void keyboard_send(char p_type,char p_value)
{
	if (p_value == VK_OEM_AUTO ||
		p_value == VK_OEM_ENLW)
	{
		p_value = VK_HANJA;
	}

	for (long sx1 = 0;sx1 < 256;sx1 ++)
	{
		if (g_keyboard1[sx1] == 0 &&
			g_keyboard2[sx1] == 0)
		{
			g_keyboard1[sx1] = p_value;
			g_keyboard2[sx1] = p_type;

			break;
		}
	}
}

//����n�X���b�h
DWORD WINAPI thread1(LPVOID p_arg)
{
	COM_DATA com_data;

	long mouse_x_old = -1;
	long mouse_y_old = -1;

	for (;g_end == 0;)
	{
		Sleep(10);

		//����n

		if (g_sock1 == -1)
		{
			g_sock1 = socket(AF_INET,SOCK_STREAM,0);

			g_sock1_ct = 0;

			if (connect(g_sock1,(struct sockaddr *)&g_addr,sizeof(sockaddr_in)) == SOCKET_ERROR)
			{
				//�ڑ��G���[

				closesocket(g_sock1);
				g_sock1 = -1;

				closesocket(g_sock2);
				g_sock2 = -1;

				closesocket(g_sock3);
				g_sock3 = -1;

				Sleep(1000);

				continue;
			}
		}

		memset(&com_data,0,sizeof(COM_DATA));

		memcpy(&com_data.ver,g_connect_ver,4);

		com_data.data_type = 1;
		com_data.thread = 1;
		com_data.mode = 5;
		com_data.monitor_no = 1;

		//����n

		com_data.control = 1;
		com_data.mouse_cursor = 0;

		if (mouse_x_old == -1 && 
			mouse_y_old == -1)
		{
			mouse_x_old = g_mouse_x;
			mouse_y_old = g_mouse_y;
		}

		if (mouse_x_old != g_mouse_x || 
			mouse_y_old != g_mouse_y)
		{
			mouse_x_old = g_mouse_x;
			mouse_y_old = g_mouse_y;

			com_data.mouse_move = 1;

			long x = (long)((double)g_mouse_x * ((double)g_capture_cx / (double)g_window_cx));
			long y = (long)((double)g_mouse_y * ((double)g_capture_cy / (double)g_window_cy));

			com_data.mouse_x = x;
			com_data.mouse_y = y;
		}

		if (g_mouse_left[0] != 0)
		{
			com_data.mouse_left = g_mouse_left[0];
		}

		if (com_data.mouse_left != 0)
		{
			for (long sx1 = 0;sx1 < 255;sx1 ++)
			{
				g_mouse_left[sx1] = g_mouse_left[sx1 + 1];
			}

			g_mouse_left[255] = 0;
		}

		if (g_mouse_right[0] != 0)
		{
			com_data.mouse_right = g_mouse_right[0];
		}

		if (com_data.mouse_right != 0)
		{
			for (long sx1 = 0;sx1 < 255;sx1 ++)
			{
				g_mouse_right[sx1] = g_mouse_right[sx1 + 1];
			}

			g_mouse_right[255] = 0;
		}

		if (g_mouse_middle[0] != 0)
		{
			com_data.mouse_middle = g_mouse_middle[0];
		}

		if (com_data.mouse_middle != 0)
		{
			for (long sx1 = 0;sx1 < 255;sx1 ++)
			{
				g_mouse_middle[sx1] = g_mouse_middle[sx1 + 1];
			}

			g_mouse_middle[255] = 0;
		}

		if (g_mouse_wheel[0] != 0)
		{
			com_data.mouse_wheel = g_mouse_wheel[0];
		}

		if (com_data.mouse_wheel != 0)
		{
			for (long sx1 = 0;sx1 < 255;sx1 ++)
			{
				g_mouse_wheel[sx1] = g_mouse_wheel[sx1 + 1];
			}

			g_mouse_wheel[255] = 0;
		}

		if (g_mouse_x1[0] != 0)
		{
			com_data.mouse_x1 = g_mouse_x1[0];
		}

		if (com_data.mouse_x1 != 0)
		{
			for (long sx1 = 0;sx1 < 255;sx1 ++)
			{
				g_mouse_x1[sx1] = g_mouse_x1[sx1 + 1];
			}

			g_mouse_x1[255] = 0;
		}

		if (g_mouse_x2[0] != 0)
		{
			com_data.mouse_x2 = g_mouse_x2[0];
		}

		if (com_data.mouse_x2 != 0)
		{
			for (long sx1 = 0;sx1 < 255;sx1 ++)
			{
				g_mouse_x2[sx1] = g_mouse_x2[sx1 + 1];
			}

			g_mouse_x2[255] = 0;
		}

		if ((GetKeyState(VK_SHIFT) & 0x8000) != 0)
		{
			if (g_shift_down != 1)
			{
				keyboard_send((char)0x80,VK_SHIFT);
			}

			g_shift_down = 1;
		}
		else
		{
			if (g_shift_down != 0)
			{
				keyboard_send((char)0x00,VK_SHIFT);
			}

			g_shift_down = 0;
		}

		if ((GetKeyState(VK_MENU) & 0x8000) != 0)
		{
			if (g_alt_down != 1)
			{
				keyboard_send((char)0x80,VK_MENU);
			}

			g_alt_down = 1;
		}
		else
		{
			if (g_alt_down != 0)
			{
				keyboard_send((char)0x00,VK_MENU);
			}

			g_alt_down = 0;
		}

		if ((GetKeyState(VK_CONTROL) & 0x8000) != 0)
		{
			if (g_ctrl_down != 1)
			{
				keyboard_send((char)0x80,VK_CONTROL);
			}

			g_ctrl_down = 1;
		}
		else
		{
			if (g_ctrl_down != 0)
			{
				keyboard_send((char)0x00,VK_CONTROL);
			}

			g_ctrl_down = 0;
		}

		if (g_shift_down != 0 ||
			g_alt_down != 0 ||
			g_ctrl_down != 0)
		{
			com_data.keydown = 1;
		}

		com_data.keycode = g_keyboard1[0];
		com_data.keycode_flg = g_keyboard2[0];

		if (com_data.keycode != 0 || 
			com_data.keycode_flg != 0)
		{
			com_data.keydown = 1;

			for (long sx1 = 0;sx1 < 255;sx1 ++)
			{
				g_keyboard1[sx1] = g_keyboard1[sx1 + 1];
				g_keyboard2[sx1] = g_keyboard2[sx1 + 1];
			}

			g_keyboard1[255] = 0;
			g_keyboard2[255] = 0;
		}

		JOYINFOEX gamepad_btn;

		gamepad_btn.dwFlags = JOY_RETURNALL;
		gamepad_btn.dwSize = sizeof(JOYINFOEX);

		if (::joyGetPosEx(JOYSTICKID1,&gamepad_btn) == JOYERR_NOERROR)
		{
			com_data.gamepad1 = gamepad_btn.dwXpos;
			com_data.gamepad2 = gamepad_btn.dwYpos;
			com_data.gamepad3 = gamepad_btn.dwZpos;
			com_data.gamepad4 = gamepad_btn.dwRpos;
			com_data.gamepad5 = gamepad_btn.dwPOV;
			com_data.gamepad6 = gamepad_btn.dwButtons;
			com_data.gamepad7 = gamepad_btn.dwUpos;
			com_data.gamepad8 = gamepad_btn.dwVpos;
		}
		else
		{
			com_data.gamepad1 = 0x7fff;
			com_data.gamepad2 = 0x7fff;
			com_data.gamepad3 = 0x7fff;
			com_data.gamepad4 = 0x7fff;
			com_data.gamepad5 = 0xffff;
			com_data.gamepad6 = 0x0000;
			com_data.gamepad7 = 0x0000;
			com_data.gamepad8 = 0x0000;
		}

		//�摜�n

		com_data.zoom = 1.0;
		com_data.image_cx = g_capture_cx;
		com_data.image_cy = g_capture_cy;
		com_data.client_scroll_x = 0;
		com_data.client_scroll_y = 0;
		com_data.video_quality = 3;

		//�����n

		com_data.sound_type = 1;
		com_data.sound_capture = 1;
		com_data.sound_quality = 3;

		//�w�b�_�[���M
		if (send_data(&g_sock1,(char*)&com_data,sizeof(COM_DATA)) < 0)
		{
			closesocket(g_sock1);
			g_sock1 = -1;

			closesocket(g_sock2);
			g_sock2 = -1;

			closesocket(g_sock3);
			g_sock3 = -1;

			continue;
		}

		//�w�b�_�[��M
		if (recv_data(&g_sock1,(char*)&com_data,sizeof(COM_DATA)) < 0)
		{
			closesocket(g_sock1);
			g_sock1 = -1;

			closesocket(g_sock2);
			g_sock2 = -1;

			closesocket(g_sock3);
			g_sock3 = -1;

			continue;
		}

		if (com_data.mode == 0)
		{
			//�p�X���[�h�G���[

			continue;
		}

		if (g_sock1_ct < 999)
		{
			g_sock1_ct ++;
		}
	}

	ExitThread(TRUE);

	return 0;
}

//�摜�n�X���b�h
DWORD WINAPI thread2(LPVOID p_arg)
{
	COM_DATA com_data;

	for (;g_end == 0;)
	{
		Sleep(10);

		//�摜�n

		if (g_sock1 == -1)
		{
			continue;
		}

		if (g_sock1_ct < 5)
		{
			continue;
		}

		if (g_sock2 == -1)
		{
			g_sock2 = socket(AF_INET,SOCK_STREAM,0);

			g_sock2_ct = 0;

			if (connect(g_sock2,(struct sockaddr *)&g_addr,sizeof(sockaddr_in)) == SOCKET_ERROR)
			{
				//�ڑ��G���[

				closesocket(g_sock2);
				g_sock2 = -1;

				continue;
			}
		}

		//�w�b�_�[��M
		if (recv_data(&g_sock2,(char *)&com_data,sizeof(COM_DATA)) < 0)
		{
			closesocket(g_sock1);
			g_sock1 = -1;

			closesocket(g_sock2);
			g_sock2 = -1;

			closesocket(g_sock3);
			g_sock3 = -1;

			continue;
		}

		long image_size = com_data.data_size;

		char *image_buf = (char *)malloc(image_size);

		//�{�̎�M
		if (recv_data(&g_sock2,(char *)image_buf,image_size) < 0)
		{
			free(image_buf);
			
			closesocket(g_sock1);
			g_sock1 = -1;

			closesocket(g_sock2);
			g_sock2 = -1;

			closesocket(g_sock3);
			g_sock3 = -1;

			continue;
		}

		//�`��

		IPicture *pic;
		IStream *str;
		long cx;
		long cy;

		HDC hdc = GetDC(g_hwnd);

		HGLOBAL hgbl = GlobalAlloc(GPTR,image_size);

		memcpy(hgbl,image_buf,image_size);

		CreateStreamOnHGlobal(hgbl,TRUE,&str);

		OleLoadPicture(str,image_size,TRUE,IID_IPicture,(LPVOID*)&pic);

		pic->get_Width(&cx);
		pic->get_Height(&cy);

		pic->Render(hdc,0,0,g_window_cx,g_window_cy,0,cy,cx,-cy,0);

		pic->Release();
		str->Release();

		GlobalUnlock(hgbl);

		ReleaseDC(g_hwnd,hdc);

		free(image_buf);

		if (g_sock2_ct < 999)
		{
			g_sock2_ct ++;
		}
	}

	ExitThread(TRUE);

	return 0;
}

//�����n�X���b�h
DWORD WINAPI thread3(LPVOID p_arg)
{
	COM_DATA com_data;

	for (;g_end == 0;)
	{
		Sleep(10);

		//�����n

		if (g_sock2 == -1)
		{
			continue;
		}

		if (g_sock2_ct < 5)
		{
			continue;
		}

		if (g_sock3 == -1)
		{
			g_sock3 = socket(AF_INET,SOCK_STREAM,0);

			if (connect(g_sock3,(struct sockaddr *)&g_addr,sizeof(sockaddr_in)) == SOCKET_ERROR)
			{
				//�ڑ��G���[
 
				closesocket(g_sock3);
				g_sock3 = -1;

				continue;
			}

			g_ds = 0;
			g_dsb = 0;
			g_ca_format = 0;
		}

		//�w�b�_�[��M
		if (recv_data(&g_sock3,(char *)&com_data,sizeof(COM_DATA)) < 0)
		{
			closesocket(g_sock3);
			g_sock3 = -1;

			ds_exit();

			continue;
		}

		long samplerate = com_data.samplerate;

		long sound_size = com_data.data_size;
		char *sound_buf = (char *)malloc(sound_size);

		//�{�̎�M
		if (recv_data(&g_sock3,(char *)sound_buf,sound_size) < 0)
		{
			closesocket(g_sock3);
			g_sock3 = -1;

			ds_exit();

			continue;
		}

		if (g_ds == 0)
		{
			ds_init(com_data.samplerate);
		}

		//�Đ�
		if (g_dsb != 0)
		{
			LPVOID	lock_data;
			DWORD	lock_size;

			g_dsb->Lock(0,g_ca_format->Format.nAvgBytesPerSec * 3,&lock_data,&lock_size,0,0,0);

			if (g_ds_sound_pt + com_data.data_size <= long(g_ca_format->Format.nAvgBytesPerSec) * 3)
			{
				memcpy((char *)lock_data + g_ds_sound_pt,sound_buf,long(com_data.data_size));

				g_ds_sound_pt += long(com_data.data_size);
			}
			else
			{
				long pt = g_ca_format->Format.nAvgBytesPerSec * 3 - g_ds_sound_pt;

				memcpy((char *)lock_data + g_ds_sound_pt,sound_buf,pt);
				memcpy((char *)lock_data,sound_buf + pt,long(com_data.data_size) - pt);

				g_ds_sound_pt = long(com_data.data_size) - pt;
			}

			g_dsb->Unlock(lock_data,lock_size,0,0);

			DWORD status;
			g_dsb->GetStatus(&status);

			if (g_ds_sound_pt >= long(double(g_ca_format->Format.nAvgBytesPerSec) * 0.1))
			{
				if ((status & DSBSTATUS_PLAYING) == 0)
				{
					g_dsb->Play(0,0,DSBPLAY_LOOPING);
				}
			}
		}

		free(sound_buf);
	}

	ds_exit();

	ExitThread(TRUE);

	return 0;
}

//�E�C���h�E�v���V�[�W��
LRESULT CALLBACK proc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	if (message == WM_SYSCOMMAND)
	{
		if ((wParam & 0xfff0) == SC_KEYMENU)
		{
			//MENU�L�[����

			return false;
		}
	}

	if (message == WM_CLOSE)
	{
		//�E�C���h�E�N���[�Y

		g_end = 1;
	}

	if (g_end == -1)
	{
		if (message == WM_COMMAND)
		{
			if (LOWORD(wParam) == 65200)
			{
				//Ok�{�^��

				g_end = 0;
			}
		}
	}

	if (g_end == 0)
	{
		//�}�E�X�n

		if (message == WM_MOUSEMOVE)
		{
			g_mouse_x = LOWORD(lParam);
			g_mouse_y = HIWORD(lParam);
		}

		if (message == WM_LBUTTONDOWN)
		{
			mouse_send(0,1);
		}

		if (message == WM_LBUTTONUP)
		{
			mouse_send(0,2);
		}

		if (message == WM_LBUTTONDBLCLK)
		{
			mouse_send(0,3);
		}

		if (message == WM_RBUTTONDOWN)
		{
			mouse_send(1,1);
		}

		if (message == WM_RBUTTONUP)
		{
			mouse_send(1,2);
		}

		if (message == WM_RBUTTONDBLCLK)
		{
			mouse_send(1,3);
		}

		if (message == WM_MBUTTONDOWN)
		{
			mouse_send(2,1);
		}

		if (message == WM_MBUTTONUP)
		{
			mouse_send(2,2);
		}

		if (message == WM_MBUTTONDBLCLK)
		{
			mouse_send(2,3);
		}

		if (message == WM_MOUSEWHEEL)
		{
			short delta = HIWORD(wParam);

			if (delta > 0)
			{
				if (delta / 120 > 9)
				{
					delta = 9 * 120;
				}

				mouse_send(3,delta / 10);
			}

			if (delta < 0)
			{
				if (delta / 120 < -9)
				{
					delta = -9 * 120;
				}

				mouse_send(3,delta / 10);
			}
		}

		if (message == WM_XBUTTONDOWN)
		{
			if ((GET_XBUTTON_WPARAM(wParam) & XBUTTON1) != 0)
			{
				mouse_send(4,1);
			}
			else
			{
				mouse_send(5,1);
			}
		}

		if (message == WM_XBUTTONUP)
		{
			if ((GET_XBUTTON_WPARAM(wParam) & XBUTTON1) != 0)
			{
				mouse_send(4,2);
			}
			else
			{
				mouse_send(5,2);
			}
		}

		if (message == WM_XBUTTONDBLCLK)
		{
			if ((GET_XBUTTON_WPARAM(wParam) & XBUTTON1) != 0)
			{
				mouse_send(4,3);
			}
			else
			{
				mouse_send(5,3);
			}
		}

		//�L�[�{�[�h�n

		if (message == WM_KEYDOWN ||
			message == WM_SYSKEYDOWN)
		{
			if (GetFocus() != 0)
			{
				if (wParam != VK_SHIFT &&
					wParam != VK_MENU &&
					wParam != VK_CONTROL)
				{
					keyboard_send((char)0x80,wParam);
				}
			}
		}

		if (message == WM_KEYUP ||
			message == WM_SYSKEYUP)
		{
			if (GetFocus() != 0)
			{
				if (wParam != VK_SHIFT &&
					wParam != VK_MENU &&
					wParam != VK_CONTROL)
				{
					keyboard_send(0,wParam);
				}
			}
		}
	}

	return DefWindowProc(hWnd,message,wParam,lParam);
}

//�h��ׂ�
void fill_window(COLORREF p_rgb)
{
	RECT rect;
	HBRUSH brs;
	HDC hdc;

	GetClientRect(g_hwnd,&rect);
	brs = CreateSolidBrush(p_rgb);
	hdc = GetDC(g_hwnd);
	FillRect(hdc,&rect,brs);

	ReleaseDC(g_hwnd,hdc);
	DeleteObject(brs);
}

//���C�����[�`��
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	g_end = -1;

	g_ip = (char *)malloc(256);
	g_encryption_key = (char *)malloc(17);
	g_connect_ver = (char *)malloc(5);

	g_window_cx = 960;										//�E�C���h�E��
	g_window_cy = 540;										//�E�C���h�E����

	g_capture_cx = 1920;									//�L���v�`���[��
	g_capture_cy = 1080;									//�L���v�`���[����

	strcpy(g_connect_ver,"0000");							//�ʐM�o�[�W����

	g_sock1 = -1;
	g_sock2 = -1;
	g_sock3 = -1;

	g_sock1_ct = 0;
	g_sock2_ct = 0;

	WNDCLASSEX wc;
	memset(&wc,0,sizeof(WNDCLASSEX));
	wc.cbSize			= sizeof(wc);
	wc.lpszClassName	= L"ZeroShikiKaiSan";
	wc.lpfnWndProc		= proc;
	wc.style			= CS_VREDRAW | CS_HREDRAW;
	wc.hCursor			= LoadCursor(0,IDC_ARROW);
	wc.hInstance		= hInst;
	RegisterClassEx(&wc);

	g_hwnd = CreateWindowEx(WS_EX_DLGMODALFRAME,wc.lpszClassName,wc.lpszClassName,WS_DLGFRAME | WS_SYSMENU,16,16,g_window_cx,g_window_cy + 28,0,0,hInst,0);

	HFONT font1 = CreateFont(13,0,0,0,FW_NORMAL,0,0,0,SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,L"Tahoma");

	CreateWindowEx(0,L"Static",L"ip",WS_VISIBLE | WS_CHILD | SS_CENTER,310,252,133,15,g_hwnd,(HMENU)65000,0,0);
	CreateWindowEx(0,L"Static",L"port",WS_VISIBLE | WS_CHILD | SS_CENTER,444,252,66,15,g_hwnd,(HMENU)65001,0,0);
	CreateWindowEx(0,L"Static",L"password",WS_VISIBLE | WS_CHILD | SS_CENTER,511,252,133,15,g_hwnd,(HMENU)65002,0,0);

	CreateWindowEx(WS_EX_CLIENTEDGE,L"Edit",L"192.168.0.1",WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_CENTER | ES_AUTOHSCROLL,310,268,134,21,g_hwnd,(HMENU)65100,0,0);
	CreateWindowEx(WS_EX_CLIENTEDGE,L"Edit",L"55500",WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_CENTER | ES_AUTOHSCROLL,444,268,67,21,g_hwnd,(HMENU)65101,0,0);
	CreateWindowEx(WS_EX_CLIENTEDGE,L"Edit",L"",WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_CENTER | ES_AUTOHSCROLL | ES_PASSWORD,511,268,134,21,g_hwnd,(HMENU)65102,0,0);

	CreateWindowEx(0,L"Button",L"Ok",WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,645,268,31,20,g_hwnd,(HMENU)65200,0,0);

	SendMessage(GetDlgItem(g_hwnd,65000),WM_SETFONT,(WPARAM)font1,0);
	SendMessage(GetDlgItem(g_hwnd,65001),WM_SETFONT,(WPARAM)font1,0);
	SendMessage(GetDlgItem(g_hwnd,65002),WM_SETFONT,(WPARAM)font1,0);
	SendMessage(GetDlgItem(g_hwnd,65100),WM_SETFONT,(WPARAM)font1,0);
	SendMessage(GetDlgItem(g_hwnd,65101),WM_SETFONT,(WPARAM)font1,0);
	SendMessage(GetDlgItem(g_hwnd,65102),WM_SETFONT,(WPARAM)font1,0);
	SendMessage(GetDlgItem(g_hwnd,65200),WM_SETFONT,(WPARAM)font1,0);

	ImmDisableIME(0);

	ShowWindow(g_hwnd,SW_SHOW);

	MSG msg;

	for (;;)
	{
		if (PeekMessage(&msg,0,0,0,PM_NOREMOVE))
		{
			if (GetMessage(&msg,0,0,0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		Sleep(10);

		if (g_end != -1)
		{
			break;
		}
	}

	wchar_t data[256];

	memset(&data,0,sizeof(data));
	GetWindowText(GetDlgItem(g_hwnd,65100),data,256);
	wcstombs(g_ip,data,256);

	memset(&data,0,sizeof(data));
	GetWindowText(GetDlgItem(g_hwnd,65101),data,6);
	g_port = (unsigned short)_wtol(data);

	memset(&data,0,sizeof(data));
	GetWindowText(GetDlgItem(g_hwnd,65102),data,17);
	wcstombs(g_encryption_key,data,17);

	memset(&g_addr,0,sizeof(sockaddr_in));
	g_addr.sin_family = AF_INET;
	g_addr.sin_addr.s_addr = inet_addr(g_ip);
	g_addr.sin_port = htons(g_port);

	WSADATA wsadata;
	WSAStartup(MAKEWORD(1,1),&wsadata);

	ShowWindow(GetDlgItem(g_hwnd,65000),SW_HIDE);
	ShowWindow(GetDlgItem(g_hwnd,65001),SW_HIDE);
	ShowWindow(GetDlgItem(g_hwnd,65002),SW_HIDE);
	ShowWindow(GetDlgItem(g_hwnd,65100),SW_HIDE);
	ShowWindow(GetDlgItem(g_hwnd,65101),SW_HIDE);
	ShowWindow(GetDlgItem(g_hwnd,65102),SW_HIDE);
	ShowWindow(GetDlgItem(g_hwnd,65200),SW_HIDE);

	fill_window(RGB(0,0,0));

	HANDLE hthd1 = 0;
	HANDLE hthd2 = 0;
	HANDLE hthd3 = 0;

	if (g_end == 0)
	{
		hthd1 = CreateThread(0,0,thread1,0,0,0);
		hthd2 = CreateThread(0,0,thread2,0,0,0);
		hthd3 = CreateThread(0,0,thread3,0,0,0);
	}

	for (;;)
	{
		if (PeekMessage(&msg,0,0,0,PM_NOREMOVE))
		{
			if (GetMessage(&msg,0,0,0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		Sleep(10);

		if (g_end != 0)
		{
			if (g_sock1 != -1)
			{
				closesocket(g_sock1);
				g_sock1 = -1;
			}

			if (g_sock2 != -1)
			{
				closesocket(g_sock2);
				g_sock2 = -1;
			}

			if (g_sock3 != -1)
			{
				closesocket(g_sock3);
				g_sock3 = -1;
			}

			if (hthd1 != 0)
			{
				WaitForSingleObject(hthd1,3000);
				CloseHandle(hthd1);
				hthd1 = 0;
			}

			if (hthd2 != 0)
			{
				WaitForSingleObject(hthd2,3000);
				CloseHandle(hthd2);
				hthd2 = 0;
			}

			if (hthd3 != 0)
			{
				WaitForSingleObject(hthd3,3000);
				CloseHandle(hthd3);
				hthd3 = 0;
			}

			if (hthd1 == 0 &&
				hthd2 == 0 &&
				hthd3 == 0)
			{
				break;
			}
		}
	}

	free(g_ip);
	free(g_encryption_key);
	free(g_connect_ver);

	DeleteObject(font1);

	return 0;
}
