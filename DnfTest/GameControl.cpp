#include "StdAfx.h"
#include "GameControl.h"
#include "KeyMouMng.h"
#include "WaitForEvent.h"
#include "Config.h"
char g_ExePath[MAX_PATH] = {0};


CGameControl::CGameControl(void)
{
	GetPath(g_ExePath);
}


CGameControl::~CGameControl(void)
{
}

void CGameControl::ClickLoginInArea()
{
	CKeyMouMng::Ptr()->MouseMoveAndClick(828,496);  //点击编辑框
}

void CGameControl::InputCodes()
{
	while (!IsCanLogin())
	{
		Sleep(500);
	}
	ClickLoginInArea();
	Sleep(1000);
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
	CKeyMouMng::Ptr()->InputPassword(account.password.c_str());
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClick(1044,482);  //点击登录
}

void CGameControl::CreateRole()
{
	while (!IsCanCreateRoles())
	{
		Sleep(500);
	}
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(468,823);  //点击创建角色
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(1416,645);  //创建角色第二步
	Sleep(1000);
	const auto& account = config_instance.accounts.at(this->m_Index);
	CKeyMouMng::Ptr()->InputCharByKeyBoard(account.role_name.c_str());
	_DbgPrint("【角色名字】 CreateRole name %s\n", account.role_name.c_str());
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(910,428);  //点击检测重复
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(802,483); 
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(755,529); //点击确定
	Sleep(1000);
	CKeyMouMng::Ptr()->MouseMoveAndClickGameWnd(872,535); //点击创建角色成功
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

bool CGameControl::IsCanCreateRoles()
{
	BOOL bFind = FALSE;
	int nXtmp = 0,nYtmp = 0;
	HWND hGameWnd = GetGameWnd();
	if (hGameWnd == NULL) 
		return FALSE;

	TCHAR ImagePath[MAX_PATH] = {0};
	wsprintf(ImagePath, _T("%sMatchImage\\Game\\CreateRole.png"), g_ExePath);

	bFind = ImageMatchFromHwnd(hGameWnd,ImagePath,0.5,nXtmp,nYtmp,false);
	if (bFind)
	{
		_DbgPrint("【可以进入游戏了】 CreateRole 图片 X:%d Y:%d\n",nXtmp,nYtmp);
		bFind = TRUE;
	}else
	{
		//_DbgPrint("没找到CanEnterGame图片\n");
		bFind = FALSE;
	}	

	return bFind;
}

bool CGameControl::IsCanLogin()
{
	BOOL bFind = FALSE;
	int nXtmp = 0,nYtmp = 0;
	HWND hLoginWnd = GetLoginWnd();
	if (hLoginWnd == NULL) 
		return FALSE;

	TCHAR ImagePath[MAX_PATH] = {0};
	wsprintf(ImagePath, _T("%sMatchImage\\Game\\Login.png"), g_ExePath);

	bFind = ImageMatchFromHwnd(hLoginWnd,ImagePath,0.5,nXtmp,nYtmp,false);
	if (bFind)
	{
		_DbgPrint("【可以进入登录了】 Login 图片 X:%d Y:%d\n",nXtmp,nYtmp);
		bFind = TRUE;
	}else
	{
		//_DbgPrint("没找到CanEnterGame图片\n");
		bFind = FALSE;
	}

	return bFind;
}

BOOL CGameControl::ImageMatchFromHwnd(HWND hWnd,const TCHAR* ImagePath,float fSame, OUT int& nX,OUT int& nY,bool bSave)
{
	BOOL bresMatched = FALSE;
	LPBYTE   lpBits;
	IplImage    *Img;
	IplImage    *ImgGray;
	IplImage    *Tpl;
	IplImage    *TplGray;
	IplImage    *matRes;
	IplImage    *matchArea;
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
			_DbgPrint("窗体被最小化了");
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
			_DbgPrint("打开原图像失败:%s",ImagePath);
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
		//找到匹配位置
		cvMatchTemplate(ImgGray,TplGray,matRes,CV_TM_CCOEFF_NORMED);
		//找到最佳匹配位置 
		cvMinMaxLoc(matRes,&min_val,&max_val,&min_loc,&max_loc,NULL); 
		_DbgPrint("MaxVal:%f",max_val);
		//_DbgPrint("匹配位置X = %d,Y = %d",max_loc.x,max_loc.y);

		cvSetImageROI(ImgGray,cvRect(max_loc.x,max_loc.y,TplGray->width,TplGray->height));
		matchArea = cvCreateImage(cvGetSize(TplGray),8,1);
		cvCopy(ImgGray,matchArea,0);
		cvResetImageROI(ImgGray);

		if (bSave)
		{
			cvSaveImage("E:\\tmpfile\\matchedArea.png",matchArea);
		}

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
		cvReleaseImage(&matRes);
		cvReleaseImage(&matchArea);
		cvReleaseImage(&Img);
		cvReleaseImage(&Tpl);
		cvReleaseImage(&ImgGray);
		cvReleaseImage(&TplGray);

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

		_DbgPrint((char*)lpMsgBuf);

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

		_DbgPrint((char*)lpMsgBuf);

		LocalFree(lpMsgBuf);
		return NULL;
	}
}
