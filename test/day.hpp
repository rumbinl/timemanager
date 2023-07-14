#include <iostream>
#include <string>
#include <skia/include/core/SkCanvas.h>
#include <skia/include/core/SkFont.h>
#include <skia/include/core/SkFontMetrics.h>

using namespace std;

static string month_name[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

class Day
{
	public:
		Day(int d, int m, int y)
		{
			day=d;
			month=m; 
			year=y;
			taskCount=0;
		}

		void setXYWH(float x, float y, float w, float h)
		{
			xpos=x;	
			ypos=y;
			width=w;
			height=h;
		}

		int getYear() { return year; }

		int getMonth() { return month; }

		int getDay() { return day; }

		void setDMY(int d, int m, int y)
		{
			day=d;
			month=m;
			year=y;
		}

		void setTaskCount(int newTaskCount)
		{
			taskCount = newTaskCount;
		}

		float getX() { return xpos; }
		float getY() { return ypos; }
		float getW() { return width; }
		float getH() { return height; }

		int getTaskCount() {
			return taskCount;
		}
	private:
		int year,month,day,taskCount;
		bool isSelected;
		float xpos,ypos,width,height;
};

class Month 
{
	public:
		Month(int numDays, int m, int y)
		{
			offY=80;
			offX=80;
			this->numDays=numDays;
			dayList = vector<Day>(numDays,Day(0,0,0));
			minDayList = vector<int>(numDays,0);
			for(int i=0;i<numDays;i++)
			year=y;
			month=m;
			int i=0;
			int w=100,h=100;	
			selected = numDays;
			for(int y=0;y<((numDays+6)/7);y++)
			{
				for(int x=0;x<7 && x+y*7<numDays;x++)
				{
					dayList[i].setDMY(i+1,m,y);
					dayList[i].setXYWH(x*w,y*h,w,h);
					i++;
				}
			}

		}

		void pollDayPress(float mouseX, float mouseY, bool* render)
		{
			for(Day& day : dayList)
			{
				float dayX=day.getX()+offX, dayY=day.getY()+offY, dayW=day.getW(), dayH=day.getH();
				if(mouseX>dayX&&mouseX<dayX+dayW&&mouseY>dayY&&mouseY<dayY+dayH)
				{
					selected = day.getDay();
					*render=true;
					break;
				}
			}
		}

void scheduleTask()
{

	//dayList[minDayList[deadlineDay]].increaseTaskCount(1);


	for(int i=0;i<numDays;i++)

		minDayList[i]=dayList[minDayList[i-1]].getTaskCount()<=dayList[i].getTaskCount()?minDayList[i-1]:i;

}

		void renderMonth(SkCanvas* canvas, SkPaint* p, SkFont* f)
		{
			SkFontMetrics fMet;
			f->getMetrics(&fMet);
			canvas->drawString((month_name[month-1]+" "+std::to_string(year)).c_str(),80,60,*f,*p);
			
			int padding=5,i=0;
			float circles=10;
			for(Day d : dayList)
			{
				float x=d.getX()+offX, y=d.getY()+offY,w=d.getW(),h=d.getH();
				if(d.getDay() == selected)
				{
					p->setColor(SK_ColorRED);
					canvas->drawRect(SkRect::MakeXYWH(x,y,w,h),*p);
				}
				p->setColor(SK_ColorBLACK);
				canvas->drawString(std::to_string(i+1).c_str(), x+padding, y+padding+fMet.fCapHeight,*f,*p);
				canvas->drawLine(x,y,x+w,y,*p);
				canvas->drawLine(x,y+h,x+w,y+h,*p);
				canvas->drawLine(x,y,x,y+h,*p);
				canvas->drawLine(x+w,y,x+w,y+h,*p);
				for(int j=0;j<dayList[i].getTaskCount();j++)
					canvas->drawCircle(w/circles+x+j*2*w/circles,fMet.fCapHeight+y+padding+w/circles,w/circles,*p);
				i++;
			}
		}
		
		void addTask(int d)
		{
			dayList[d-1].setTaskCount(dayList[d-1].getTaskCount()+1);
		}

		~Month()
		{
			dayList.clear();
		}
	private:
		int month, year, numDays;
		float offX,offY;
		int selected;
		vector<Day> dayList;
		vector<int> minDayList;
};
