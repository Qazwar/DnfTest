#include "StdAfx.h"
#include "GameControl.h"
#include "KeyMouMng.h"
#include "WaitForEvent.h"
#include "Config.h"
char g_ExePath[MAX_PATH] = {0};


CGameControl::CGameControl(HWND hShow):
m_Index(0), m_hShow(hShow)
{
	GetPath(g_ExePath);
}
bool CGameControl::FindCurrentAccountIndex()
{
	//找到第一个没有创建角色的账号
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



CGameControl::~CGameControl(void)
{
}

void CGameControl::ClickLoginInArea()
{
	CKeyMouMng::Ptr()->MouseMoveAndClick(828,496);  //点击编辑框
	while(true){
		if(FindImageInLoginWnd("QQ.png")){
			break;
		}
		Sleep(500);
		CKeyMouMng::Ptr()->MouseMoveAndClick(828,496);  //点击编辑框
	}
}

void CGameControl::StartGame()
{
	STARTUPINFOA StartupInfo;
	PROCESS_INFORMATION ProcessInformation;
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));
	//StartupInfo.dwFlags = STARTF_USESHOWWINDOW;//指定wShowWindow成员有效
	StartupInfo.wShowWindow = TRUE;//此成员设为TRUE的话则显示新建进程的主窗口
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
		AfxMessageBox(_T("启动游戏失败"), MB_OK);
		return;
	}
	SendMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_START, NULL);
}

void CGameControl::InputCodes()
{
	while (!FindImageInLoginWnd("Login.png"))
	{
		Sleep(500);
	}
	Sleep(000);
	ClickLoginInArea();
	SendMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_LOGIN, NULL);
	while (!FindImageInLoginWnd("LoginByCode.png"))
	{
		Sleep(500);
	}
	ClickAgreement();
	Sleep(500);
	CKeyMouMng::Ptr()->MouseMoveAndClick(1092,328);  //点击编辑框
	Sleep(200);
	for (auto i = 0; i<20; i++)
	{	
		WAIT_STOP_RETURN(10);
		CKeyMouMng::Ptr()->DirKeyDown(VK_BACK);
		Sleep(100);
		CKeyMouMng::Ptr()->DirKeyUp(VK_BACK);
	}
	const auto& account = config_instance.accounts.at(this->m_Index);
	//先输入用户名
	CKeyMouMng::Ptr()->InputCharByKeyBoard(account.qq.c_str());
	CKeyMouMng::Ptr()->KeyboardButtonEx(VK_TAB);
	Sleep(1000);
	//CKeyMouMng::Ptr()->MouseMoveAndClick(1092,373);  //点击编辑框
	CKeyMouMng::Ptr()->InputPassword( const_cast<char*>(account.password.c_str()));
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClick(1044,482);  //点击登录
	LOG_DEBUG<<"点击登录按钮";
}

void CGameControl::CreateRole()
{
	LOG_DEBUG<<"开始创建角色";
	while (!IsCanCreateRoles())
	{
		Sleep(500);
	}
	SendMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_CREATE_ROLE, NULL);
	LOG_DEBUG<<"可以创建角色了";
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(468,823);  //点击创建角色
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(1416,645);  //创建角色第二步
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
		const auto name = CreateName();
		auto& account = config_instance.accounts.at(this->m_Index);
		account.role_name = name;
		account.status = STATUS_SUCCESS;
		config_instance.SaveData();
		LOG_DEBUG<<"【角色名字】"<<name.c_str();
		CKeyMouMng::Ptr()->InputCharByKeyBoard(name.c_str());
		CKeyMouMng::Ptr()->KeyboardButtonEx(VK_RETURN);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(910,428);  //点击检测重复
		LOG_DEBUG<<"点击检测重复 "<<name.c_str();
		Sleep(1000);
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(802,483);//点击检测结果
		Sleep(1000);
	}
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(755,529); //点击确定
	LOG_DEBUG<<"点击确定";
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
		LOG_DEBUG<<"成功 的坐标 "<<nXtmp<<" y "<< nYtmp;
		CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(nXtmp+140, nYtmp+115);  //点击编辑框
	}
	LOG_DEBUG<<"点击创建角色成功";
	SendMessage(m_hShow, WM_UPDATE_GAME_STATUS, (WPARAM)GAME_CREATE_ROLE_DONE, NULL);
}

void CGameControl::EndGame()
{
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(1022,819);  //点击结束游戏
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
		CKeyMouMng::Ptr()->MouseMoveAndClick(948,413); //点击创建角色成功
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
	//获得桌面句柄
	try
	{	
		HWND hDesktop = ::GetDesktopWindow();  
		//如果句柄为空则接取全屏来找
		if(NULL == hWnd)  
		{  
			hWnd = hDesktop;
		}  

		RECT rect;  
		::GetWindowRect(hWnd, &rect);  

		if (rect.left < 0 || rect.top < 0)
		{
			LOG_DEBUG<<"窗体被最小化了";
			SetWindowPos(hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
			::GetWindowRect(hWnd, &rect); 
		}else
		{
			SetWindowPos(hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_DRAWFRAME | SWP_NOSIZE);
		}

		int nWidth = rect.right - rect.left;  
		int nHeight = rect.bottom - rect.top;  

		int size = sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD);
		BITMAPINFO* bmpinfo = (BITMAPINFO*)(new BYTE[size]);// 因为BITMAPINFO结构中已经包含了一个RGBQUAD成员，所以分配时对于256色只需再跟255个RGBQUAD的空间
		ZeroMemory(bmpinfo, size);
		bmpinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpinfo->bmiHeader.biWidth = nWidth;
		bmpinfo->bmiHeader.biHeight = nHeight;
		bmpinfo->bmiHeader.biPlanes = 1;
		bmpinfo->bmiHeader.biBitCount = 32;
		bmpinfo->bmiHeader.biCompression = BI_RGB;
		//bmpinfo->bmiHeader.biClrUsed = 0; // 前面都已经初始化为0，不用重复
		//bmpinfo->bmiHeader.biClrImportant = 0;
		bmpinfo->bmiHeader.biSizeImage = nWidth*nHeight*4;// 对于biCompression设为BI_RGB的此参数可为0

		HDC hSrcDC = ::GetWindowDC(hWnd);  
		HDC hMemDC = ::CreateCompatibleDC(hSrcDC); 
		HBITMAP hBitmap = CreateDIBSection(hSrcDC, bmpinfo, DIB_RGB_COLORS,(void **)&lpBits, NULL, 0);
		//HBITMAP hBitmap = ::CreateCompatibleBitmap(hSrcDC, nWidht, nHeight);  
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);  
		BOOL bRet = ::BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, 0, 0, SRCCOPY|CAPTUREBLT); 
		//_DbgPrint("bRet = %d, 宽 = %d, 高 = %d, 窗体X坐标 = %d, 窗体Y坐标= %d",
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
			LOG_DEBUG<<"打开原图像失败:"<<ImagePath;
			goto _Error;
		} 

		TplGray = cvCreateImage(cvGetSize(Tpl),8,1);
		cvCvtColor(Tpl,TplGray,CV_BGR2GRAY);

		if (bSave)
		{
			cvSaveImage("E:\\tmpfile\\Tpl.png",Tpl);
			cvSaveImage("E:\\tmpfile\\TplGray.png",TplGray);
		}
		//---------------------开始比对
		int iwidth = Img->width - Tpl->width + 1;
		int iheight = Img->height - Tpl->height + 1;
		//创建匹配结果图
		matRes=cvCreateImage(cvSize(iwidth, iheight),IPL_DEPTH_32F, 1);  
		LOG_DEBUG<<"matRes:"<<matRes;
		//找到匹配位置
		if(bGray){
			ImgCompare = ImgGray;
			TplCompare = TplGray;
		}else{
			ImgCompare = Img;
			TplCompare = Tpl;
		}
		cvMatchTemplate(ImgCompare,TplCompare,matRes,CV_TM_CCOEFF_NORMED);
		//找到最佳匹配位置 
		cvMinMaxLoc(matRes,&min_val,&max_val,&min_loc,&max_loc,NULL);
		//_DbgPrint("匹配位置X = %d,Y = %d",max_loc.x,max_loc.y);

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
			//匹配上了
			//_DbgPrint("匹配成功");
			nX = max_loc.x;
			nY = max_loc.y;
		}
		else
		{
			//_DbgPrint("匹配不成功");
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
	LOG_DEBUG<<"查找图片"<<image.c_str()<< " 结果 "<<bFind<<" X "<< nXtmp <<" y "<< nYtmp;
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
	LOG_DEBUG<<"查找图片"<<image.c_str()<< " 结果 "<<bFind<<" X "<< nXtmp <<" y "<< nYtmp;
	return bFind;
}

string CGameControl::CreateName()
{
	//随机选取12个英文字母
	string Name;
	srand((int)time(0));
	for(int i(0); i < 12; i++){
		char ch = rand()%26+'a';
		Name.push_back(ch);
	}
	return Name;
}
