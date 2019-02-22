#include "StdAfx.h"
#include "GameControl.h"
#include "KeyMouMng.h"
#include "WaitForEvent.h"
#include "Config.h" 
#include "tlhelp32.h"
#include "VerificationCode.h"
#include "VPNControler.h"

char g_ExePath[MAX_PATH] = {0};

CGameControl::CGameControl(HWND hShow):
m_Index(0), m_hShow(hShow),m_Stop(false),m_RoleIndex(0)
{
	GetPath(g_ExePath);
#ifndef _DEBUG
	KillProcess("Client.exe");
	KillProcess("DNF.exe");
#endif
}
bool CGameControl::FindCurrentAccountIndex()
{
	if(m_Stop){
		return false;
	}
	//�ҵ���һ��û�д�����ɫ���˺�
	const auto& accounts = config_instance.accounts;
	m_Index++;
	if(m_Index == accounts.size())
	{
		return false;
	}
	return true;
}



void CGameControl::Stop()
{
	m_Stop = true;
}

bool CGameControl::GameProcess()
{
	config_instance.SaveData();
	Sleep(1000);
	bool bSuccess = false;
	int tryTimes = 0;
	while(tryTimes++ <= config_instance.ip_try_times){
		if (!StartGame())
		{
			break;
		}
		if(!InputCodes()){
			break;
		}
		auto now = GetTickCount();
		while(true){
			Sleep(500);
			if(GetGameWnd()){
				bSuccess = true;
				break;
			}else if(GetTickCount()-now > 2*60*1000 ){
				break;
			}
		}
		if(bSuccess){
			break;
		}
		KillProcess("Client.exe");
		KillProcess("DNF.exe");
	}
	if(!bSuccess){
		KillProcess("Client.exe");
		KillProcess("DNF.exe");
		return FindCurrentAccountIndex();
	}
	if(!CreateRole()){
		return FindCurrentAccountIndex();;
	}
	EndGame();
	return FindCurrentAccountIndex();
}

CGameControl::~CGameControl(void)
{
}

void CGameControl::SelectAreaProcess()
{
	while(!FindImageInLoginWnd("SelectArea.png")){
		Sleep(500);
	}
	while(FindImageInLoginWnd("SelectArea.png")){
		CKeyMouMng::Ptr()->MouseMoveAndClick(414,549);  //���ѡ�������
		Sleep(500);
	}
	LOG_DEBUG<<"��ʼѡ��";
	SelectArea();
	LOG_DEBUG<<"ѡ�����";
	while(!FindImageInLoginWnd("QQ.png")){
		CKeyMouMng::Ptr()->MouseMoveAndClick(1041,554);  //�����¼��Ϸ
		int Times(0);
		bool delayShow = false;
		while( Times++ <= 5){
			if(FindImageInLoginWnd("Delay.png")){
				delayShow = true;
				break;
			}
			Sleep(100);
		}
		if(delayShow){
			CKeyMouMng::Ptr()->MouseMoveAndClick(619+rand()%81, 426+rand()%24);  //�������
			Sleep(500);
		}
		Sleep(500);
	}
	
}

bool CGameControl::StartGame()
{
#ifndef DEBUG
	if(!SwitchVPN()){
		PostMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_IP_FAILED, NULL);
		return false;
	}
#endif // DEBUG
	STARTUPINFOA StartupInfo;
	PROCESS_INFORMATION ProcessInformation;
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));
	//StartupInfo.dwFlags = STARTF_USESHOWWINDOW;//ָ��wShowWindow��Ա��Ч
	StartupInfo.wShowWindow = TRUE;//�˳�Ա��ΪTRUE�Ļ�����ʾ�½����̵�������
	CreateProcessA((config_instance.game_path+"\\Client.exe").c_str(),
		NULL,
		NULL, NULL,
		0,
		NULL,
		NULL,
		config_instance.game_path.c_str(),
		&StartupInfo,
		&ProcessInformation);
	if(ProcessInformation.hProcess==NULL)
	{
		AfxMessageBox(_T("������Ϸʧ��"), MB_OK);
		return false;
	}
	PostMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_START, NULL);
	return true;
}

bool CGameControl::InputCodes()
{
	SelectAreaProcess();
	PostMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_LOGIN, NULL);
	while (!FindImageInLoginWnd("LoginByCode.png"))
	{
		Sleep(500);
	}
	ClickAgreement();
	Sleep(500);
	InputAccount();	
	//��¼��ɣ�������֤�����
	bool bVerficationCode = false;
	int Times = 0;
	while(Times++<=6){
		Sleep(500);
		if(FindImageInLoginWnd("VerificationCode.png")||FindImageInLoginWnd("PassWordWrong.png")){
			bVerficationCode = true;
			break;
		}
	}
	if(bVerficationCode){
		LOG_DEBUG<<"��Ҫ������֤��";
		int iTryTimes = 0;
		while(iTryTimes++<=config_instance.loginFailTimes){
			int verficationCodeTimes = 0;//��֤������4��
			while(FindImageInLoginWnd("VerificationCode.png")&&verficationCodeTimes++<=4){
				if(SaveVerificationCodeImage())
				{
					CString strRe = CVerificationCode::Ptr()->pRecYZM_A((LPSTR)(LPCSTR)common::stringToCString(string(g_ExePath)+"VerificationCode\\tmp.png"),
						common::stringToCString(config_instance.verification_account_code),common::stringToCString(config_instance.verification_password),(LPSTR)(LPCSTR)_T("65395"));
					int pos = strRe.Find("|!|");
					if (pos>-1)
					{
						CString result = strRe.Left(pos);
						CKeyMouMng::Ptr()->MouseMoveAndClick(479,365);  //�����֤��༭��
						CKeyMouMng::Ptr()->InputString(common::CStringTostring(result));
						CKeyMouMng::Ptr()->KeyboardButtonEx(VK_RETURN);
						Sleep(500);
						CKeyMouMng::Ptr()->MouseMoveAndClick(550,457);  //�����֤��ȷ��
						Sleep(500);
					}
				}
			}
			if(FindImageInLoginWnd("PassWordWrong.png")){
				CKeyMouMng::Ptr()->MouseMoveAndClick(600,434);  //���ȷ��
				Sleep(500);
				inputPasswordAndLogin();
			}
		}
		if(iTryTimes==config_instance.loginFailTimes){
			return false;
		}
	}
	return true;
}

bool CGameControl::CreateRole()
{
	LOG_DEBUG<<"��ʼ������ɫ";
	m_RoleIndex = 0;
	//����Ѿ���ë���ŵ�ֱ�ӽ��봴����ɫ���棬Ҳ������
	while (!FindImageInGameWnd("GameStart.png")&&!FindImageInGameWnd("Role.png")&&!FindImageInGameWnd("RiskGroup.png"))
	{
		Sleep(500);
	}
	LOG_DEBUG<<"�Ƿ���Ҫ����ð����";
	while(FindImageInGameWnd("RiskGroup.png"))
	{
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(405,290);  //���ð���ű༭��
		for (auto i = 0; i<20; i++)
		{	
			Sleep(100);
			CKeyMouMng::Ptr()->DirKeyDown(VK_BACK);
			Sleep(100);
			CKeyMouMng::Ptr()->DirKeyUp(VK_BACK);
		}
		const auto groupName = CreateName(16);
		LOG_DEBUG<<"��ð�������֡�"<<groupName.c_str();
		CKeyMouMng::Ptr()->InputString(groupName);
		CKeyMouMng::Ptr()->KeyboardButtonEx(VK_RETURN);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(347,428);  //�������
		Sleep(2000);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(387,363);  //���ȷ��
		Sleep(2000);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(403,323);  //����Ѿ�����ð��������
	}
	if (FindImageInGameWnd("Blocked.png"))
	{
		KillProcess("DNF.exe");
		Sleep(1000);
		return false;
	}
	PostMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_CREATE_ROLE, NULL);
	CreateOneRole();
	if(config_instance.secondRole.compare("��������ɫ")!=0){
		CreateOneRole();
	}
	PostMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_CREATE_ROLE_DONE, NULL);
	return true;
}

void CGameControl::EndGame()
{
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(550,548);  //���������Ϸ
	Sleep(1000);
	KillProcess("DNF.exe");
	Sleep(2000);
}

void CGameControl::SetAccountIndex(const int& index)
{
	this->m_Index = index;
}


void CGameControl::ClickAgreement()
{
	if(FindImageInGameWnd("Agreement.png")){
		CKeyMouMng::Ptr()->MouseMoveAndClick(948,413); //���ͬ��Э��
	}
	Sleep(100);
}


void CGameControl::InputAccount()
{
	CKeyMouMng::Ptr()->MouseMoveAndClick(1092,328);  //����༭��
	Sleep(200);
	for (auto i = 0; i<20; i++)
	{	
		WAIT_STOP_RETURN(10);
		CKeyMouMng::Ptr()->DirKeyDown(VK_BACK);
		Sleep(100);
		CKeyMouMng::Ptr()->DirKeyUp(VK_BACK);
	}
	const auto& account = config_instance.accounts.at(this->m_Index);
	//�������û���
	CKeyMouMng::Ptr()->InputString(account.qq);
	CKeyMouMng::Ptr()->KeyboardButtonEx(VK_TAB);
	LOG_DEBUG<<"�˺� "<<account.qq.c_str()<<" ���� "<<account.password.c_str();
	inputPasswordAndLogin();
}

void CGameControl::inputPasswordAndLogin()
{
	Sleep(1000);
	const auto& account = config_instance.accounts.at(this->m_Index);
	CKeyMouMng::Ptr()->InputString(account.password);
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClick(1044,482);  //�����¼
}

bool CGameControl::CreateOneRole()
{
	LOG_DEBUG<<"���Դ�����ɫ��";
	if(!FindImageInGameWnd("Role.png")){
		Sleep(1000);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(178,548);  //���������ɫ
		Sleep(1000);
	}
	//���ѡ��ְҵ
	SelectProfession();
	m_RoleIndex++;
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(691, 432);  //������ɫ�ڶ���
	Sleep(1000);
	while(!FindImageInGameWnd("NameCheckPass.png", 0.99, false))
	{
		for (auto i = 0; i<12; i++)
		{	
			Sleep(100);
			CKeyMouMng::Ptr()->DirKeyDown(VK_BACK);
			Sleep(100);
			CKeyMouMng::Ptr()->DirKeyUp(VK_BACK);
		}
		const auto name = CreateName(12);
		auto& account = config_instance.accounts.at(this->m_Index);
		account.role_name = name;
		account.status = STATUS_SUCCESS;
		config_instance.SaveData();
		LOG_DEBUG<<"����ɫ���֡�"<<name.c_str();
		CKeyMouMng::Ptr()->InputString(name);
		CKeyMouMng::Ptr()->KeyboardButtonEx(VK_RETURN);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(479,289);  //�������ظ�
		LOG_DEBUG<<"�������ظ� "<<name.c_str();
		Sleep(1000);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(400,321);//��������
		Sleep(1000);
	}
	while(FindImageInGameWnd("Role.png"))
	{
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(370,354); //���ȷ��
		Sleep(500);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(373,320); //���ȷ��
		Sleep(500);
	}
	LOG_DEBUG<<"���ȷ��";
	Sleep(1000);

	int nXtmp = 0,nYtmp = 0;
	HWND hGameWnd = GetGameWnd();
	if (hGameWnd == NULL) 
		return false;
	TCHAR ImagePath[MAX_PATH] = {0};
	wsprintf(ImagePath, _T("%sMatchImage\\Game\\Success.png"), g_ExePath);
	auto bFind = ImageMatchFromHwnd(hGameWnd,ImagePath,0.5,nXtmp,nYtmp,false);
	if (bFind)
	{
		LOG_DEBUG<<"�ɹ� ������ "<<nXtmp<<" y "<< nYtmp;
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(nXtmp+95, nYtmp+76);  //����༭��
	}
	Sleep(500);
	LOG_DEBUG<<"���������ɫ�ɹ�";
	return true;
}

BOOL CGameControl::SaveVerificationCodeImage()
{
	auto hWnd = GetLoginWnd();
	BOOL bresMatched = FALSE;
	LPBYTE   lpBits;
	IplImage    *Img = NULL;
	//���������
	try
	{	
		HWND hDesktop = ::GetDesktopWindow();  
		//������Ϊ�����ȡȫ������
		if(NULL == hWnd)  
		{  
			hWnd = hDesktop;
		}  

		RECT rect;  
		::GetWindowRect(hWnd, &rect);  

		if (rect.left < 0 || rect.top < 0)
		{
			LOG_DEBUG<<"���屻��С����";
			SetWindowPos(hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
			::GetWindowRect(hWnd, &rect); 
		}else
		{
			SetWindowPos(hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_DRAWFRAME | SWP_NOSIZE);
		}

		int nWidth = rect.right - rect.left;  
		int nHeight = rect.bottom - rect.top;  

		int size = sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD);
		BITMAPINFO* bmpinfo = (BITMAPINFO*)(new BYTE[size]);// ��ΪBITMAPINFO�ṹ���Ѿ�������һ��RGBQUAD��Ա�����Է���ʱ����256ɫֻ���ٸ�255��RGBQUAD�Ŀռ�
		ZeroMemory(bmpinfo, size);
		bmpinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpinfo->bmiHeader.biWidth = nWidth;
		bmpinfo->bmiHeader.biHeight = nHeight;
		bmpinfo->bmiHeader.biPlanes = 1;
		bmpinfo->bmiHeader.biBitCount = 32;
		bmpinfo->bmiHeader.biCompression = BI_RGB;
		//bmpinfo->bmiHeader.biClrUsed = 0; // ǰ�涼�Ѿ���ʼ��Ϊ0�������ظ�
		//bmpinfo->bmiHeader.biClrImportant = 0;
		bmpinfo->bmiHeader.biSizeImage = nWidth*nHeight*4;// ����biCompression��ΪBI_RGB�Ĵ˲�����Ϊ0

		BITMAPINFO* tmpBmp = new BITMAPINFO(*bmpinfo);
		tmpBmp->bmiHeader.biWidth = 118;
		tmpBmp->bmiHeader.biHeight = 39;
		HDC hSrcDC = ::GetWindowDC(hWnd);  
		HDC hMemDC = ::CreateCompatibleDC(hSrcDC); 
		HBITMAP hBitmap = CreateDIBSection(hSrcDC, tmpBmp, DIB_RGB_COLORS,(void **)&lpBits, NULL, 0);
		//HBITMAP hBitmap = ::CreateCompatibleBitmap(hSrcDC, nWidht, nHeight);  
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);
		BOOL bRet = ::BitBlt(hMemDC, 0, 0, 118, 39, hSrcDC, 532, 344, SRCCOPY|CAPTUREBLT); 
		//_DbgPrint("bRet = %d, �� = %d, �� = %d, ����X���� = %d, ����Y����= %d",
		//		   bRet ,nWidht,nHeight,rect.left,rect.top);

		Img = HBitmapToLpl(hBitmap);

		if (Img == NULL)
		{
			cvReleaseImage(&Img);
			goto _Error;
		}
		cvSaveImage((string(g_ExePath)+"VerificationCode\\tmp.png").c_str(),Img);
		//-----------------------------
_Error:
		if(Img){
			cvReleaseImage(&Img);
		}	

		::SelectObject(hMemDC, hOldBitmap);  
		::DeleteObject(hBitmap);  
		::DeleteDC(hMemDC);  
		::ReleaseDC(hWnd, hSrcDC);  
		delete [] bmpinfo;
		bmpinfo = NULL;
		delete [] tmpBmp;
		tmpBmp = NULL;
		SetWindowPos(hWnd,HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_DRAWFRAME | SWP_NOSIZE);
	}catch(...)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL
			);

		LOG_DEBUG<<(char*)lpMsgBuf;

		LocalFree(lpMsgBuf);
		return FALSE;
	}
	return TRUE;
}

BOOL CGameControl::ImageMatchFromHwnd(HWND hWnd,const TCHAR* ImagePath,float fSame, OUT int& nX,OUT int& nY,bool bSave, bool bGray)
{
	BOOL bresMatched = FALSE;
	LPBYTE   lpBits;
	IplImage    *Img = NULL;
	IplImage    *ImgGray = NULL;
	IplImage    *Tpl = NULL;
	IplImage    *TplGray = NULL;
	IplImage    *matRes = NULL;
	IplImage    *matchArea = NULL;
	IplImage    *ImgCompare = NULL;
	IplImage    *TplCompare = NULL;
	double min_val;
	double max_val;
	CvPoint min_loc;
	CvPoint max_loc;
	//���������
	try
	{	
		HWND hDesktop = ::GetDesktopWindow();  
		//������Ϊ�����ȡȫ������
		if(NULL == hWnd)  
		{  
			hWnd = hDesktop;
		}  

		RECT rect;  
		::GetWindowRect(hWnd, &rect);  

		if (rect.left < 0 || rect.top < 0)
		{
			LOG_DEBUG<<"���屻��С����";
			SetWindowPos(hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
			::GetWindowRect(hWnd, &rect); 
		}else
		{
			SetWindowPos(hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_DRAWFRAME | SWP_NOSIZE);
		}

		int nWidth = rect.right - rect.left;  
		int nHeight = rect.bottom - rect.top;  

		int size = sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD);
		BITMAPINFO* bmpinfo = (BITMAPINFO*)(new BYTE[size]);// ��ΪBITMAPINFO�ṹ���Ѿ�������һ��RGBQUAD��Ա�����Է���ʱ����256ɫֻ���ٸ�255��RGBQUAD�Ŀռ�
		ZeroMemory(bmpinfo, size);
		bmpinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpinfo->bmiHeader.biWidth = nWidth;
		bmpinfo->bmiHeader.biHeight = nHeight;
		bmpinfo->bmiHeader.biPlanes = 1;
		bmpinfo->bmiHeader.biBitCount = 32;
		bmpinfo->bmiHeader.biCompression = BI_RGB;
		//bmpinfo->bmiHeader.biClrUsed = 0; // ǰ�涼�Ѿ���ʼ��Ϊ0�������ظ�
		//bmpinfo->bmiHeader.biClrImportant = 0;
		bmpinfo->bmiHeader.biSizeImage = nWidth*nHeight*4;// ����biCompression��ΪBI_RGB�Ĵ˲�����Ϊ0

		HDC hSrcDC = ::GetWindowDC(hWnd);  
		HDC hMemDC = ::CreateCompatibleDC(hSrcDC); 
		HBITMAP hBitmap = CreateDIBSection(hSrcDC, bmpinfo, DIB_RGB_COLORS,(void **)&lpBits, NULL, 0);
		//HBITMAP hBitmap = ::CreateCompatibleBitmap(hSrcDC, nWidht, nHeight);  
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);  
		BOOL bRet = ::BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, 0, 0, SRCCOPY|CAPTUREBLT); 
		//_DbgPrint("bRet = %d, �� = %d, �� = %d, ����X���� = %d, ����Y����= %d",
		//		   bRet ,nWidht,nHeight,rect.left,rect.top);

		Img = HBitmapToLpl(hBitmap);

		if (Img == NULL)
		{
			cvReleaseImage(&Img);
			goto _Error;
		}

		ImgGray = cvCreateImage(cvGetSize(Img),8,1);
		cvCvtColor(Img,ImgGray,CV_BGR2GRAY);

		if (bSave)
		{
			cvSaveImage("E:\\tmpfile\\Img.png",Img);
			cvSaveImage("E:\\tmpfile\\ImgGray.png",ImgGray);
		}

		Tpl = cvLoadImage( ImagePath, CV_LOAD_IMAGE_COLOR );

		if( Tpl == NULL ) 
		{ 
			LOG_DEBUG<<"��ԭͼ��ʧ��:"<<ImagePath;
			goto _Error;
		} 

		TplGray = cvCreateImage(cvGetSize(Tpl),8,1);
		cvCvtColor(Tpl,TplGray,CV_BGR2GRAY);

		if (bSave)
		{
			cvSaveImage("E:\\tmpfile\\Tpl.png",Tpl);
			cvSaveImage("E:\\tmpfile\\TplGray.png",TplGray);
		}
		//---------------------��ʼ�ȶ�
		int iwidth = Img->width - Tpl->width + 1;
		int iheight = Img->height - Tpl->height + 1;
		//����ƥ����ͼ
		matRes=cvCreateImage(cvSize(iwidth, iheight),IPL_DEPTH_32F, 1);  
		LOG_DEBUG<<"matRes:"<<matRes;
		//�ҵ�ƥ��λ��
		if(bGray){
			ImgCompare = ImgGray;
			TplCompare = TplGray;
		}else{
			ImgCompare = Img;
			TplCompare = Tpl;
		}
		cvMatchTemplate(ImgCompare,TplCompare,matRes,CV_TM_CCOEFF_NORMED);
		//�ҵ����ƥ��λ�� 
		cvMinMaxLoc(matRes,&min_val,&max_val,&min_loc,&max_loc,NULL);
		//_DbgPrint("ƥ��λ��X = %d,Y = %d",max_loc.x,max_loc.y);

		/*cvSetImageROI(ImgCompare,cvRect(max_loc.x,max_loc.y,TplCompare->width,TplCompare->height));
		matchArea = cvCreateImage(cvGetSize(TplCompare),8,1);
		cvCopy(ImgCompare,matchArea,0);
		cvResetImageROI(ImgCompare);

		if (bSave)
		{
		cvSaveImage("E:\\tmpfile\\matchedArea.png",matchArea);
		}*/

		bresMatched =(fSame <= max_val)?TRUE:FALSE;

		if (bresMatched)
		{
			//ƥ������
			//_DbgPrint("ƥ��ɹ�");
			nX = max_loc.x;
			nY = max_loc.y;
		}
		else
		{
			//_DbgPrint("ƥ�䲻�ɹ�");
		}

		//-----------------------------
_Error:
		if (matRes)
		{
			cvReleaseImage(&matRes);
		}
		if(matchArea){
			cvReleaseImage(&matchArea);
		}
		if(Img){
			cvReleaseImage(&Img);
		}
		if(Tpl){
			cvReleaseImage(&Tpl);
		}
		if(ImgGray){
			cvReleaseImage(&ImgGray);
		}
		if(TplGray){
			cvReleaseImage(&TplGray);
		}

		::SelectObject(hMemDC, hOldBitmap);  
		::DeleteObject(hBitmap);  
		::DeleteDC(hMemDC);  
		::ReleaseDC(hWnd, hSrcDC);  
		delete [] bmpinfo;
		bmpinfo = NULL;
		SetWindowPos(hWnd,HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_DRAWFRAME | SWP_NOSIZE);
	}catch(...)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL
			);

		LOG_DEBUG<<(char*)lpMsgBuf;

		LocalFree(lpMsgBuf);
		return NULL;
	}


	return bresMatched;
}

IplImage* CGameControl::HBitmapToLpl(HBITMAP hBmp)
{   
	int image_width=0;
	int image_height=0;
	int image_depth=0;
	int image_nchannels=0;
	try
	{	
		BITMAP bmp;
		GetObject(hBmp,sizeof(BITMAP),&bmp);   			 	

		image_nchannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8 ;
		image_depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;
		image_width=bmp.bmWidth;
		image_height=bmp.bmHeight;
		////_DbgPrint("image_width=%d,image_height=%d,image_depth=%d,image_nchannels=%d",image_width,image_height,image_depth,image_nchannels);
		IplImage* img = cvCreateImage(cvSize(image_width,image_height),image_depth,image_nchannels); 
		BYTE *pBuffer = new BYTE[image_height*image_width*image_nchannels];    	  
		GetBitmapBits(hBmp,image_height*image_width*image_nchannels,pBuffer);    
		memcpy(img->imageData,pBuffer,image_height*image_width*image_nchannels);   
		delete[] pBuffer;    
		IplImage *dst = cvCreateImage(cvGetSize(img),img->depth,3);
		cvCvtColor(img,dst,CV_BGRA2BGR); 
		cvReleaseImage(&img);   
		return dst;   
	}
	catch(...)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL
			);

		LOG_DEBUG<<(char*)lpMsgBuf;

		LocalFree(lpMsgBuf);
		return NULL;
	}
}

BOOL CGameControl::FindImageInGameWnd(const string& image, float fSame, bool bGray)
{
	int nXtmp = 0,nYtmp = 0;
	HWND hGameWnd = GetGameWnd();
	if (hGameWnd == NULL) 
		return FALSE;
	TCHAR ImagePath[MAX_PATH] = {0};
	wsprintf(ImagePath, _T("%sMatchImage\\Game\\%s"), g_ExePath, image.c_str());
	auto bFind = ImageMatchFromHwnd(hGameWnd,ImagePath,fSame,nXtmp,nYtmp,false, bGray);
	LOG_DEBUG<<"����ͼƬ"<<image.c_str()<< " ��� "<<bFind<<" X "<< nXtmp <<" y "<< nYtmp;
	return bFind;
}

BOOL CGameControl::FindImageInLoginWnd(const string& image)
{
	int nXtmp = 0,nYtmp = 0;
	HWND hLogin = GetLoginWnd();
	if (hLogin == NULL) 
		return FALSE;
	TCHAR ImagePath[MAX_PATH] = {0};
	wsprintf(ImagePath, _T("%sMatchImage\\Game\\%s"), g_ExePath, image.c_str());
	auto bFind = ImageMatchFromHwnd(hLogin,ImagePath,0.5,nXtmp,nYtmp,false);
	LOG_DEBUG<<"����ͼƬ"<<image.c_str()<< " ��� "<<bFind<<" X "<< nXtmp <<" y "<< nYtmp;
	return bFind;
}

string CGameControl::CreateName(const unsigned int & count)
{
	string Name;
	srand((int)time(0));
	for(unsigned int i(0); i < count; i++){
		char ch = 'a';
		if(rand()%2==0){
			ch = rand()%26+'a';
		}else{
			ch = rand()%10+'0';
		}
		Name.push_back(ch);
	}
	return Name;
}

void CGameControl::SelectProfession()
{
	srand((int)time(0));
	auto positionX = 0;
	auto positionY = 0;
	auto roleName = config_instance.firstRole;
	auto professionName = config_instance.firstRoleProfession;
	if (m_RoleIndex==1)
	{
		roleName = config_instance.secondRole;
		professionName = config_instance.secondRoleProfession;
	}
	for (auto i(0); i < config_instance.professionPositions.size(); i++)
	{
		if(config_instance.professionPositions.at(i).name == roleName)
		{
			positionX = config_instance.professionPositions.at(i).positionX+10+rand()%53;
			positionY = config_instance.professionPositions.at(i).positionY+10+rand()%43;
			break;
		}
	}
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(positionX, positionY);  //�����һְҵ
	Sleep(500+rand()%500);
	auto professionIndex(0);//��ɫ�ڶ�ְҵ�ĸ��������ڶ�λ
	for (auto i(0); i < config_instance.professions.size(); i++)
	{
		if(config_instance.professions.at(i).name == roleName)
		{
			for (auto j(0); j < config_instance.professions.at(i).profession.size(); j++)
			{
				if(config_instance.professions.at(i).profession.at(j) == professionName)
				{
					professionIndex = j;
					break;
				}
			}
			break;
		}
	}
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(177+rand()%53+10+professionIndex*77,408+rand()%27+10);  //����ڶ�ְҵ
	Sleep(500+rand()%500);
}

void CGameControl::SelectArea()
{
	srand((int)time(0));
	auto nm = config_instance.servername;
	for(auto i(0); i < config_instance.game_area.size(); i++)
	{
		const auto& areaInfo = config_instance.game_area.at(i);
		if(areaInfo.name == config_instance.areaname)
		{
			if(areaInfo.group == "Telecom"){
				CKeyMouMng::Ptr()->MouseMoveAndClick(222+rand()%25, 119+rand()%60);  //�������
			}else if(areaInfo.group == "Unicom"){
				CKeyMouMng::Ptr()->MouseMoveAndClick(222+rand()%25, 207+rand()%60);  //�����ͨ
			}
			Sleep(800);
			//����index�����
			CKeyMouMng::Ptr()->MouseMoveAndClick(280+rand()%90+(areaInfo.index%5)*123, 148+rand()%25+(areaInfo.index/5)*52);  //�������������
			Sleep(800);
			for(auto j(0); j < areaInfo.server.size(); j++)
			{
				if(areaInfo.server.at(j) == config_instance.servername)
				{
					CKeyMouMng::Ptr()->MouseMoveAndClick(280+rand()%90+(j%5)*123, 384+rand()%25+(j/5)*52);  //�������
					Sleep(800);
					break;
				}
			}
			break;
		}
	}	
}

void CGameControl::ResetIndex()
{
	m_Index = 0;
}

void CGameControl::GameLoop()
{
	while(true){
		GameProcess();
	}
	ResetIndex();
}

BOOL CGameControl::KillProcess(const string& processName)
{
	CString strProcessName = common::stringToCString(processName);

	strProcessName.MakeLower();

	//�������̿���(TH32CS_SNAPPROCESS��ʾ�������н��̵Ŀ���)

	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//PROCESSENTRY32���̿��յĽṹ��
	PROCESSENTRY32 pe;
	//ʵ������ʹ��Process32First��ȡ��һ�����յĽ���ǰ�����ĳ�ʼ������
	pe.dwSize = sizeof(PROCESSENTRY32);
	//�����IFЧ��ͬ:

	//if(hProcessSnap == INVALID_HANDLE_VALUE)   ��Ч�ľ��
	if (!Process32First(hSnapShot, &pe))
	{
		return FALSE;
	}

	//��������Ч  ��һֱ��ȡ��һ�����ѭ����ȥ
	while (Process32Next(hSnapShot, &pe))
	{
		//pe.szExeFile��ȡ��ǰ���̵Ŀ�ִ���ļ�����
		CString scTmp = pe.szExeFile;
		//�Ƚϵ�ǰ���̵Ŀ�ִ���ļ����ƺʹ��ݽ������ļ������Ƿ���ͬ
		//��ͬ�Ļ�Compare����0
		if (scTmp.MakeLower().Find(strProcessName) != -1)
		{
			//�ӿ��ս����л�ȡ�ý��̵�PID(������������е�PID)
			DWORD dwProcessID = pe.th32ProcessID;
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
			::TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
			return TRUE;
		}
	}
	CloseHandle(hSnapShot);
	return FALSE;
}

bool CGameControl::SwitchVPN()
{
	CVPNControler controler;
	controler.clickOnSwitchButton();
	auto Times(0);
	while(Times++<=config_instance.ip_try_times){
		Sleep(1000);
		CString currentIP = CVPNControler::GetSystemIp();
		LOG_DEBUG<<"ip "<<common::CStringTostring(currentIP).c_str();
		if(currentIP.Compare(common::stringToCString(config_instance.ip_address))!=0&&m_LastIP.Compare(currentIP)!=0)
		{
			m_LastIP = currentIP;
			PostMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_IP, (LPARAM)&m_LastIP);
			return true;
		}
	}
	return false;
}
