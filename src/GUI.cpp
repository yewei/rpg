#include "../include/GUI.h"

/******************************画点，画线，画矩形*******************************/
void GUI::DrawPixel(SDL_Surface *screen,int x,int y,Uint8 R,Uint8 G,Uint8 B){
	if(SDL_MUSTLOCK(screen))SDL_LockSurface(screen);
	Uint32 color = SDL_MapRGB(screen->format,R,G,B);
	switch(screen->format->BytesPerPixel)
    {
       case 1:    // Assuming 8-bpp
       {
              Uint8    *bufp;
              bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
              *bufp = color;
       }
       break;
       case 2:    // Probable 15-bpp or 16-bpp
       {
              Uint16    *bufp;
              bufp = (Uint16 *)screen->pixels + y*screen->pitch + x*3;
              if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
              {
                     bufp[0] = color;
                     bufp[1] = color>>8;
                     bufp[2] = color>>16;
              }else{
                     bufp[2] = color;
                     bufp[1] = color>>8;
                     bufp[0] = color>>16;
              }
       }
       break;
       case 4:    // Probable 32-bpp
       {
              Uint32    *bufp;
              bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
              *bufp = color;
       }
       break;
    }
	if(SDL_MUSTLOCK(screen))SDL_UnlockSurface(screen);

}

void GUI::DrawLine(SDL_Surface* screen , int x1 , int y1 , int x2 , int y2 , Uint8 R, Uint8 G, Uint8 B) {
    int delta_x , delta_y , half_x , half_y , dotn , x , y , dir_x , dir_y , b;
    dir_x = 1; dir_y = 1;
    if ( (delta_x = x2-x1) < 0 ) { delta_x = -delta_x; dir_x = -1; }
    if ( (delta_y = y2-y1) < 0 ) { delta_y = -delta_y; dir_y = -1; }
    x = x1;
    y = y1;
    b = 0;
    if ( delta_x < delta_y ) {
        half_y = delta_y >> 1;
        dotn = delta_y;
        do {
            DrawPixel(screen , x , y , R, G, B);
            y += dir_y;
            b += delta_x;
            if ( b > half_y ) {
                b -= delta_y;
                x += dir_x;
            }
        } while ( dotn -- );
    }
    else {
        half_x = delta_x >> 1;
        dotn = delta_x;
        do {
            DrawPixel (screen , x , y , R, G, B);
            x += dir_x;
            b += delta_y;
            if ( b > half_x ) {
                b -= delta_x;
                y += dir_y;
            }
        } while ( dotn -- );
    }
    return ;
}

void GUI::DrawRect(SDL_Surface* screen,int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B,int thick){
	for(int i=0;i<thick;i++){
		DrawLine(screen, x, y+i, x+w, y+i, R, G, B);
		DrawLine(screen, x+w-i, y, x+w-i, y+h, R, G, B);
		DrawLine(screen, x+w, y+h+i, x, y+h+i, R, G, B);
		DrawLine(screen, x+i, y+h, x+i, y, R, G, B);
	}
}

/******************************Button*******************************/
GUI::GUIButton::GUIButton(string upImage, string downImage, int x, int y, GUI::GUIObject* pParent, int id){
	m_upImage = new RPG_Image(upImage);
	m_downImage = new RPG_Image(downImage);
	m_fClickEvent = NULL;
	m_fOverEvent = NULL;
	m_fOutEvent = NULL;
	m_fClickEventEx = NULL;
	m_fOverEventEx = NULL;
	m_fOutEventEx = NULL;
	m_x = x;
	m_y = y;
	m_w = m_upImage->getWidth();
	m_h = m_upImage->getHeight();
	m_upImage->setColorKey(255, 0, 255);
	m_downImage->setColorKey(255, 0, 255);
	m_switch = 0; //默认显示m_upImage
	m_isInButton = false; //默认鼠标不在Button上
	m_pParent = pParent;
	m_id = id;
	//添加事件
	RPG_EventManager::addListen(this, RPG_EventManager::EVENTMANAGER_ONLBUTTONDOWN);
	RPG_EventManager::addListen(this, RPG_EventManager::EVENTMANAGER_ONLBUTTONUP);
	RPG_EventManager::addListen(this, RPG_EventManager::EVENTMANAGER_ONMOUSEMOVE);
	//添加渲染
	GUIRenderManager::addRender(this);
}

GUI::GUIButton::GUIButton(string upImage, string downImage, int x, int y, GUI::GUIObject* pParent){
	new(this)GUIButton(upImage, downImage, x, y, pParent, -1);
}

GUI::GUIButton::GUIButton(string upImage, string downImage, GUI::GUIObject* pParent){
	new(this)GUIButton(upImage, downImage, 0, 0, pParent);
}

GUI::GUIButton::~GUIButton(){
	delete m_upImage;
	delete m_downImage;
	//删除事件
	RPG_EventManager::delListen(this, RPG_EventManager::EVENTMANAGER_ONLBUTTONDOWN);
	RPG_EventManager::delListen(this, RPG_EventManager::EVENTMANAGER_ONLBUTTONUP);
	RPG_EventManager::delListen(this, RPG_EventManager::EVENTMANAGER_ONMOUSEMOVE);
	//删除渲染
	GUIRenderManager::delRender(this);
}

void GUI::GUIButton::onClickEvent(){
	if(m_fClickEvent!=NULL){
		m_fClickEvent(m_pParent);
		return;
	}
	if(m_fClickEventEx!=NULL){
		m_fClickEventEx(m_pParent, m_id);
		return;
	}
}

void GUI::GUIButton::onOverEvent(){
	if(m_fOverEvent!=NULL){
		m_fOverEvent(m_pParent);
		return;
	}
	if(m_fOverEventEx!=NULL){
		m_fOverEventEx(m_pParent, m_id);
		return;
	}
}

void GUI::GUIButton::onOutEvent(){
	if(m_fOutEvent!=NULL){
		m_fOutEvent(m_pParent);
		return;
	}
	if(m_fOutEventEx!=NULL){
		m_fOutEventEx(m_pParent, m_id);
		return;
	}
}

void GUI::GUIButton::onLButtonDown(int mx, int my){
	if(inside(mx, my)){
		//鼠标点击时在按钮内
		m_switch = 1; //当鼠标按下时显示downImage
		//onClickEvent();
	}
}

void GUI::GUIButton::onLButtonUp(int mx, int my){
	m_switch = 0; //当鼠标松开时显示upImage
	if(inside(mx, my))onClickEvent(); //触发按钮事件
}

void GUI::GUIButton::onMouseMove(int mx, int my){
	if( inside(mx, my) && !m_isInButton){
		//如果原先在按钮外边,现在到达按钮里面
		onOverEvent();
		m_isInButton = true;
	}
	else if( !inside(mx, my) && m_isInButton){
		//如果原先在按钮里面,现在到达按钮外面
		onOutEvent();
		m_isInButton = false;
	}
}

void GUI::GUIButton::draw(SDL_Surface* destSurf){
	if(m_switch==0)
		m_upImage->drawImage(destSurf, m_x, m_y);
	else
		m_downImage->drawImage(destSurf, m_x, m_y);
}

/******************************Label*******************************/
int GUI::GUILabel::LabelCount = 0;

GUI::GUILabel::GUILabel(const std::string& font, int size, const std::string& message, int r, int g, int b, int x, int y){
	SDL_Surface* t;
	if(LabelCount==0){ //第一个实例初始化TTF库
		if(TTF_Init()<0)throw "不能初始化TTF库";
	}
	LabelCount++; //实例数加１
	m_x = x;
	m_y = y;
	SDL_Color textColor;
	textColor.r = r;
	textColor.g = g;
	textColor.b = b;
	m_pFont = TTF_OpenFont(font.c_str(), size);
	if(m_pFont==0)throw "不能打开字符文件"+font;
	vector<string> msgs = splitString(message, "\n"); //分行
	for(int i=0; i<msgs.size(); i++){
		t = myTTF_RenderString_Blended(m_pFont, msgs[i], textColor);
		m_textSurface.push_back(t);
	}
	if(msgs.size()>0)m_height = m_textSurface[0]->h; //文字高度 
	else m_height = 0;
	//添加渲染
	GUIRenderManager::addRender(this);
}

GUI::GUILabel::GUILabel(const std::string& font, int size, const std::string& message, int r, int g, int b){
	new(this)GUILabel(font, size, message, r, g, b, 0, 0);
}

GUI::GUILabel::~GUILabel(){
	TTF_CloseFont(m_pFont);
	for(int i=0; i<m_textSurface.size(); i++){
		SDL_FreeSurface(m_textSurface[i]); //释放每个表面
	}
	LabelCount--;
	if(LabelCount==0){ //析构最后一个Label时推出TTF库
		TTF_Quit();
	}
	//删除渲染
	GUIRenderManager::delRender(this);
}

vector<string> GUI::GUILabel::splitString(std::string strSrc, std::string strSep){
	string::size_type pos = 0; 
    vector<string> AfterSeparatorString; 
    while( true ) 
    {
		string::size_type n = strSrc.find_first_of( strSep , pos ); 
        if( n != -1 ) 
        { 
            string sTemp; 
            sTemp.assign( strSrc,0, n ); 
            strSrc.erase( pos,n + 1 ); 
			AfterSeparatorString.push_back( sTemp ); 
        } 
        else 
        { 
			if("" != strSrc) 
			{ 
				AfterSeparatorString.push_back( strSrc ); 
			} 
            break; 
		} 
    } 
	return AfterSeparatorString;
}

void GUI::GUILabel::draw(SDL_Surface* destSurf){
	SDL_Rect t;
	t.x = m_x;
	for(int i=0; i<m_textSurface.size(); i++){
		//显示每一行
		t.y = m_y + i*m_height;
		SDL_BlitSurface(m_textSurface[i], NULL, destSurf, &t);
	}
}

/*****************************MessageBox*******************************/
GUI::GUIMessageBox::GUIMessageBox(string caption, string message, string dlg, string yes1, 
							string yes2, string no1, string no2, int x, int y, string font){
	m_dlg = new RPG_Image(dlg);
	m_dlg->setColorKey(255, 0, 255);
	int width = m_dlg->getWidth();
	int height = m_dlg->getHeight();
	m_caption = new GUILabel(font, 15, caption, 0, 0, 255, x+30, y+25);
	GUIRenderManager::delRender(m_caption); //m_caption的显示交给this的draw函数
	m_message = new GUILabel(font, 17, message, 0, 0, 0, x+40, y+5+0.2*height);
	GUIRenderManager::delRender(m_message);
	m_yes = new GUIButton(yes1, yes2, x+0.2*width-20, y+0.6*height+35, this);
	GUIRenderManager::delRender(m_yes);
	m_no = new GUIButton(no1, no2, x+0.6*width-20, y+0.6*height+35, this);
	GUIRenderManager::delRender(m_no);
	m_x = x;
	m_y = y;
	m_return = MB_NULL;
	m_yes->setfClickEvent(onYesClickHelper);
	m_no->setfClickEvent(onNoClickHelper);
	//添加渲染
	GUIRenderManager::addRender(this);
}

GUI::GUIMessageBox::~GUIMessageBox(){
	freeControls();
	//删除渲染
	GUIRenderManager::delRender(this);
}

void GUI::GUIMessageBox::freeControls(){
	if(m_dlg){delete m_dlg;m_dlg = NULL;}
	if(m_caption){delete m_caption;m_caption = NULL;}
	if(m_message){delete m_message;m_message = NULL;}
	if(m_yes){delete m_yes;m_yes = NULL;}
	if(m_no){delete m_no;m_no = NULL;}
}

void GUI::GUIMessageBox::draw(SDL_Surface* destSurf){
	if(m_return==MB_NULL){ //只有没有做出选择前才显示
		m_dlg->drawImage(destSurf, m_x, m_y);
		m_caption->draw(destSurf);
		m_message->draw(destSurf);
		m_yes->draw(destSurf);
		m_no->draw(destSurf);
	}
}

void GUI::GUIMessageBox::onYesClick(void) {
	m_return = MB_YES;
	freeControls();
}

void GUI::GUIMessageBox::onNoClick(void) {
	m_return = MB_NO;
	freeControls();
}

/*****************************BoxList*******************************/
GUI::GUIListBox::GUIListBox(int x, int y, int w, int num){
	m_x = x;
	m_y = y;
	m_h = num*16+2; //一个字体16像素高
	m_w = w;
	m_num = num;
	m_topNum = 0;
	m_selectItem = -1;
	m_fontH = m_h;
	m_toUp = new GUIButton("image\\gui\\toUp1.bmp", "image\\gui\\toUp2.bmp", m_x+m_w+5, m_y, this);
	GUIRenderManager::delRender(m_toUp);
	m_toDown = new GUIButton("image\\gui\\toDown1.bmp", "image\\gui\\toDown2.bmp", m_x+m_w+5, m_y+m_h-13, this);
	GUIRenderManager::delRender(m_toDown);
	m_toUp->setfClickEvent(onToUpClickHelper);
	m_toDown->setfClickEvent(onToDownClickHelper);
	RPG_EventManager::addListen(this, RPG_EventManager::EVENTMANAGER_ONLBUTTONDOWN);
	RPG_EventManager::addListen(this, RPG_EventManager::EVENTMANAGER_ONKEYDOWN);
	GUIRenderManager::addRender(this);
}

GUI::GUIListBox::~GUIListBox(){
	delete m_toUp;
	delete m_toDown;
	for(int i=0; i<m_itemList.size(); i++)
		delete m_itemList[i];
	RPG_EventManager::delListen(this, RPG_EventManager::EVENTMANAGER_ONLBUTTONDOWN);
	RPG_EventManager::delListen(this, RPG_EventManager::EVENTMANAGER_ONKEYDOWN);
	GUIRenderManager::delRender(this);
}

void GUI::GUIListBox::onToUpClick(){
	if(m_topNum>0){
		m_topNum--;
	}
}

void GUI::GUIListBox::onToDownClick(){
	if(m_topNum<m_itemList.size()-1){
		m_topNum++;
	}
}

void GUI::GUIListBox::onLButtonDown(int mx, int my){
	if(inside(mx, my)){
		int index = (my-m_y)/m_fontH + m_topNum; //计算选中了项目的编号
		if(index>=0 && index<m_itemList.size())
			m_selectItem = index;
	}
}

void GUI::GUIListBox::onKeyDown(SDLKey sym){
	if(sym==SDLK_UP)onToUpClick();
	else if(sym==SDLK_DOWN)onToDownClick();
}

void GUI::GUIListBox::addItem(string value){
	GUILabel* label = new GUILabel("font\\simkai.ttf", 15, value, 0, 0, 0);
	GUIRenderManager::delRender(label);
	m_fontH = label->getHeight(); //文字高度
	m_itemList.push_back(label);
	m_itemValue.push_back(value);
}

void GUI::GUIListBox::draw(SDL_Surface* destSurf){
	SDL_Rect t;
	t.x = m_x;
	t.y = m_y;
	t.h = m_h;
	t.w = m_w;
	//SDL_FillRect(destSurf, &t, SDL_MapRGB(destSurf->format, 190,190,190));
	if(m_selectItem>=0){
		//选中项高亮显示
		int index = m_selectItem-m_topNum;
		if(index>=0 && index<m_num){
			t.y = index*m_fontH+m_y;
			t.h = m_fontH;
			SDL_FillRect(destSurf, &t, SDL_MapRGB(destSurf->format, 205,183,163));
		}
	}
	m_toUp->draw(destSurf);
	m_toDown->draw(destSurf);
	for(int i=m_topNum; i<m_topNum+m_num&&i<m_itemList.size(); i++){
		m_itemList[i]->setPos(m_x, m_y+(i-m_topNum)*m_fontH);
		m_itemList[i]->draw(destSurf);
	}
	DrawRect(destSurf, m_x, m_y, m_w, m_h, 220, 230, 150, 1);
}

/*****************************ProgressBar*******************************/
GUI::GUIProgressBar::GUIProgressBar(int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B){
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
	m_R = R;
	m_G = G;
	m_B = B;
	m_radio = 0.0;
	GUIRenderManager::addRender(this);
}

GUI::GUIProgressBar::~GUIProgressBar(){
	GUIRenderManager::delRender(this);
}

void GUI::GUIProgressBar::draw(SDL_Surface *destSurf){
	SDL_Rect t;
	Uint32 color= SDL_MapRGB(destSurf->format, m_R, m_G, m_B);
	t.x = m_x;
	t.y = m_y;
	t.w = m_w * m_radio;
	t.h = m_h;
	SDL_FillRect(destSurf, &t, color);
	DrawRect(destSurf, m_x-2, m_y-2, m_w+3, m_h+3, m_R, m_G, m_B, 1);
}

/*****************************RenderManager*******************************/
vector<GUI::GUIObject*> GUI::GUIRenderManager::m_renderList;

void GUI::GUIRenderManager::addRender(GUI::GUIObject *object){
	m_renderList.push_back(object);
}

void GUI::GUIRenderManager::delRender(GUI::GUIObject *object){
	vector<GUIObject*>::iterator iter = m_renderList.begin();
	for(; iter!=m_renderList.end(); ){
		if((*iter)==object){
			iter = m_renderList.erase(iter);
		}
		else{
			iter++;
		}
	}
}

void GUI::GUIRenderManager::guiRender(SDL_Surface* destSurf){
	for(int i=0; i<m_renderList.size(); i++){
		m_renderList[i]->draw(destSurf);
	}
}

