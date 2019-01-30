#include "StdAfx.h"
#include "GameControl.h"
#include "KeyMouMng.h"
#include "WaitForEvent.h"
#include "Config.h" 
#include "tlhelp32.h"

char g_ExePath[MAX_PATH] = {0};
CDD*::CGameControl::dd = new CDD;

CGameControl::CGameControl(HWND hShow):
m_Index(0), m_hShow(hShow),m_Stop(false)
{
	GetPath(g_ExePath);
	KillProcess("Client.exe");
	KillProcess("DNF.exe");
}
bool CGameControl::FindCurrentAccountIndex()
{
	if(m_Stop){
		return false;
	}
	//�ҵ���һ��û�д�����ɫ���˺�
	const auto& accounts = config_instance.accounts;
	for(auto i(m_Index); i <= accounts.size(); i++)
	{
		if(i==accounts.size()||accounts.at(i).status == STATUS_INIT){
			m_Index = i;
			break;
		}
	}
	if(m_Index == accounts.size()){
		return false;
	}
	return true;
}



void CGameControl::Stop()
{
	m_Stop = true;
}

void CGameControl::GameProcess()
{
	Sleep(1000);
	bool bSuccess = false;
	while(true){
		StartGame();
		InputCodes();
		auto now = GetTickCount();
		auto hwnd = GetGameWnd();
		while(true){
			if(hwnd){
				bSuccess;
			}else if(GetTickCount()-now > 3*60*1000 ){
				break;
			}
		}
		if(bSuccess){
			break;
		}
	}
	CreateRole();
	EndGame();
}

CGameControl::~CGameControl(void)
{
}

void CGameControl::ClickLoginInArea()
{
	while(true){
		if(FindImageInLoginWnd("QQ.png")){
			break;
		}
		Sleep(500);
		CKeyMouMng::Ptr()->MouseMoveAndClick(828,496);  //����༭��
	}
}

void CGameControl::StartGame()
{
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
		return;
	}
	PostMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_START, NULL);
}

void CGameControl::InputCodes()
{
	while (!FindImageInLoginWnd("Login.png"))
	{
		Sleep(500);
	}
	Sleep(1000);
	ClickLoginInArea();
	PostMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_LOGIN, NULL);
	while (!FindImageInLoginWnd("LoginByCode.png"))
	{
		Sleep(500);
	}
	ClickAgreement();
	Sleep(500);
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
	CKeyMouMng::Ptr()->InputCharByKeyBoard(account.qq.c_str());
	CKeyMouMng::Ptr()->KeyboardButtonEx(VK_TAB);
	Sleep(1000);
	LOG_DEBUG<<"�˺� "<<account.qq.c_str()<<" ���� "<<account.password.c_str();
	CKeyMouMng::Ptr()->InputPassword( const_cast<char*>(account.password.c_str()));
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClick(1044,482);  //�����¼
	LOG_DEBUG<<"�����¼��ť";
}

void CGameControl::CreateRole()
{
	LOG_DEBUG<<"��ʼ������ɫ";
	while (!IsCanCreateRoles())
	{
		Sleep(500);
	}
	LOG_DEBUG<<"�Ƿ���Ҫ����ð����";
	if (FindImageInGameWnd("RiskGroup.png"))
	{
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(627,432);  //���ð���ű༭��
		const auto groupName = CreateName(16);
		LOG_DEBUG<<"��ð�������֡�"<<groupName.c_str();
		CKeyMouMng::Ptr()->InputCharByKeyBoard(groupName.c_str());
		CKeyMouMng::Ptr()->KeyboardButtonEx(VK_RETURN);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(519,642);  //�������
		Sleep(500);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(762,544);  //���ȷ��
		Sleep(100);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(801,412);  //����Ѿ�����ð��������
	}
	PostMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_CREATE_ROLE, NULL);
	LOG_DEBUG<<"���Դ�����ɫ��";
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(468,823);  //���������ɫ
	Sleep(1000);
	//���ѡ��ְҵ
	SelectProfession();
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(1416,645);  //������ɫ�ڶ���
	Sleep(1000);
	while(!FindImageInGameWnd("NameCheckPass.png", 0.99, false))
	{
		for (auto i = 0; i<12; i++)
		{	
			WAIT_STOP_RETURN(10);
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
		CKeyMouMng::Ptr()->InputCharByKeyBoard(name.c_str());
		CKeyMouMng::Ptr()->KeyboardButtonEx(VK_RETURN);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(910,428);  //�������ظ�
		LOG_DEBUG<<"�������ظ� "<<name.c_str();
		Sleep(1000);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(802,483);//��������
		Sleep(1000);
	}
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(755,529); //���ȷ��
	LOG_DEBUG<<"���ȷ��";
	Sleep(1000);

	int nXtmp = 0,nYtmp = 0;
	HWND hGameWnd = GetGameWnd();
	if (hGameWnd == NULL) 
		return;	
	TCHAR ImagePath[MAX_PATH] = {0};
	wsprintf(ImagePath, _T("%sMatchImage\\Game\\Success.png"), g_ExePath);
	auto bFind = ImageMatchFromHwnd(hGameWnd,ImagePath,0.5,nXtmp,nYtmp,false);
	if (bFind)
	{
		LOG_DEBUG<<"�ɹ� ������ "<<nXtmp<<" y "<< nYtmp;
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(nXtmp+140, nYtmp+115);  //����༭��
	}
	LOG_DEBUG<<"���������ɫ�ɹ�";
	PostMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_CREATE_ROLE_DONE, NULL);
}

void CGameControl::EndGame()
{
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(1022,819);  //���������Ϸ
	Sleep(2000);
}

void CGameControl::SetAccountIndex(const int& index)
{
	this->m_Index = index;
}

BOOL CGameControl::IsCanCreateRoles()
{
	return FindImageInGameWnd("GameStart.png");
}

void CGameControl::ClickAgreement()
{
	if(FindImageInGameWnd("Agreement.png")){
		CKeyMouMng::Ptr()->MouseMoveAndClick(948,413); //���ͬ��Э��
	}
	Sleep(100);
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
	//���ѡȡ12��Ӣ����ĸ
	string Name;
	srand((int)time(0));
	for(int i(0); i < count; i++){
		char ch = rand()%26+'a';
		Name.push_back(ch);
	}
	return Name;
}

void CGameControl::SelectProfession()
{
	srand((int)time(0));
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(354+rand()%910,698+rand()%190);  //���ְҵ
	Sleep(rand()%1000);
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(460+rand()%448,606+rand()%64);  //���ְҵ
	Sleep(rand()%1000);
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
