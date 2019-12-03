
// ParkDetectDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ParkDetect.h"
#include "ParkDetectDlg.h"
#include "afxdialogex.h"
#include "recognizing.h"

//--추가--

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>


using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CParkDetectDlg 대화 상자


CParkDetectDlg::CParkDetectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARKDETECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CParkDetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_Possible, m_possible_list);
	DDX_Control(pDX, IDC_LIST_Impossible, m_impossible_list);
}

BEGIN_MESSAGE_MAP(CParkDetectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CParkDetectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_Start, &CParkDetectDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDCANCEL, &CParkDetectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_EVENT, &CParkDetectDlg::OnBnClickedBtnEvent)
END_MESSAGE_MAP()


// CParkDetectDlg 메시지 처리기

BOOL CParkDetectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	//주차 가능 리스트 초기화
	CString str;
	for (int i = 1; i <= 10; i++) {
		str.Format(L"%d", i);
		m_possible_list.InsertString(i - 1, str);
	}

	change = 1;
	//주차 가능 이미지로 초기화
	for (int i = 0; i < 10; i++) {
		GetDlgItem(IDC_PL_1+i)->GetWindowRect(m_image_rect[i]);
		ScreenToClient(m_image_rect[i]);
		//-----추가-----
		m_image[i].Load(L"C:\\WorkSpace\\ImageProcessing\\ParkDetect\\ParkDetect\\parkinglot_image\\NoCar_black_bmp.bmp");
	}
	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CParkDetectDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		dc.SetStretchBltMode(COLORONCOLOR);
		for (int i = 0; i < 10; i++) {
			m_image[i].Draw(dc, m_image_rect[i]);
		}
		//CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CParkDetectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CParkDetectDlg::OnBnClickedBtnStart()
{	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	VideoCapture cap1(0);
	if (!cap1.isOpened())
		cout << "첫번째 카메라를 열 수 없습니다." << endl;

	Mat frame1;
	namedWindow("Parking Lot", WINDOW_AUTOSIZE);
	//////


	while (1) {
		//웹캡으로부터 한 프레임을 읽어옴
		cap1 >> frame1;

		resize(frame1, frame1, Size(300, 900), 0, 0, INTER_CUBIC);
		imshow("Parking Lot", frame1);
		
		int* lotptr = recognizing(frame1);

		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		CPaintDC dc(this);
		CString str;
		int index;

		for (int i = 0; i < 10; i++) {
			change = lotptr[i];
			str.Format(_T("%d"), i + 1);
			m_image[i].Destroy();

			//
			if (change == 1) {//주차 가능(차 없음)->주차 불가(차 있음)
				//추가
				/*
				index = m_possible_list.FindString(-1, str);
				m_possible_list.DeleteString(index);
				m_impossible_list.InsertString(-1, str);
				m_image[i].Load(L"C:\\WorkSpace\\ImageProcessing\\ParkDetect\\ParkDetect\\parkinglot_image\\Car_black_bmp.bmp");
				*/
				index = m_impossible_list.FindString(-1, str);
				m_impossible_list.DeleteString(index);
				m_possible_list.InsertString(-1, str);
				m_image[i].Load(L"C:\\WorkSpace\\ImageProcessing\\ParkDetect\\ParkDetect\\parkinglot_image\\NoCar_black_bmp.bmp");
			}
			else {//주차 불가(차 있음)->주차 가능(차 없음)
				/*
				index = m_impossible_list.FindString(-1, str);
				m_impossible_list.DeleteString(index);
				m_possible_list.InsertString(-1, str);
				m_image[i].Load(L"C:\\WorkSpace\\ImageProcessing\\ParkDetect\\ParkDetect\\parkinglot_image\\NoCar_black_bmp.bmp");
				*/

				index = m_possible_list.FindString(-1, str);
				m_possible_list.DeleteString(index);
				m_impossible_list.InsertString(-1, str);
				m_image[i].Load(L"C:\\WorkSpace\\ImageProcessing\\ParkDetect\\ParkDetect\\parkinglot_image\\Car_black_bmp.bmp");
			}
			InvalidateRect(m_image_rect[i], FALSE);
			m_image[i].Draw(dc, m_image_rect[i]);
			change *= -1;
			//
		}
		// ESC키를 누르면 웹캠 종료
		if (waitKey(10) == 27) {
			cv::destroyWindow("Parking Lot");
			break;
		}
	}

	

}





void CParkDetectDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CParkDetectDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
	
}


void CParkDetectDlg::OnBnClickedBtnEvent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CPaintDC dc(this);
	int i = 8;//ex)주차칸 7번(6+1)에 이벤트 발생했다고 가정
	CString str;
	str.Format(_T("%d"), i+1);
	int index;
	m_image[i].Destroy();


	if (change == 1) {//주차 가능(차 없음)->주차 불가(차 있음)
		//추가
		
		index = m_possible_list.FindString(-1, str);
		m_possible_list.DeleteString(index);
		m_impossible_list.InsertString(-1, str);		
		m_image[i].Load(L"C:\\WorkSpace\\ImageProcessing\\ParkDetect\\ParkDetect\\parkinglot_image\\Car_black_bmp.bmp");
	}
	else {//주차 불가(차 있음)->주차 가능(차 없음)
		index = m_impossible_list.FindString(-1, str);
		m_impossible_list.DeleteString(index);
		m_possible_list.InsertString(-1, str);
		m_image[i].Load(L"C:\\WorkSpace\\ImageProcessing\\ParkDetect\\ParkDetect\\parkinglot_image\\NoCar_black_bmp.bmp");
	}
	InvalidateRect(m_image_rect[i], FALSE);
	m_image[i].Draw(dc, m_image_rect[i]);
	change *= -1;
}
