*****************************************************************************************
* 
* kami2d library
*
*****************************************************************************************
*
* homepage : http://sepwind.blogspot.kr/
* author : hong chan, choi 
* email : sepwind@gmail.com
*
*****************************************************************************************

compile

1. dll 빌드 및 lib 파일 생성 (현재 프로젝트는 c++ builder xe2 로 생성되었으면, 필요할 경우 visual studio 생성가능)
2. include 폴더 경로 추가

usage

1. view2d 를 상속받아 onRender 부분에서 화면에 그리는 기능을 구현
2. 사용자의 뷰에서( MFC의 경우 CDialog/CView 등, 빌더의 경우 TForm 등) 핸들(HWND)을 지정하여 라이브러리 초기화 실시
  initialize(HWND)
3. MFC 기반에서 멀티터치의 경우 MSDN 참고 