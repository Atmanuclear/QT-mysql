#include "Arrange.h"

vector <Teacher>   	 teachers;  	//教师表
vector <Site>      	 sites;	 		//教室表
vector <Course>    	 courses;		//课程计划表
vector <StuClass>  	 classes;   	//班级表
vector <Appointment> appms;			//最终排课表

int random(int min, int max) //[min, max)
{
	double r1 = rand()/32768.0;
	int r2 = (int)(r1 * (max - min));
	return r2 + min;
}

/*
int trans(int week, int day, int period)
{
	return (day*4+period)*20+week;
}
 
bool ClassIn(vector<int> c1, int cls)
{
	for(int i=0; i<c1->size(); i++)
		if(c1[i] == cls) return true;
	return false;
}
*/

int allocRoomSame(int rmtype, int rmcpct, int crs, int day, int prd)
{
	bool flag;
	//分配合适的时间全空的教室 
	for(int i=0; i<sites.size(); i++){
		if(sites[i].type == rmtype && sites[i].capacity == rmcpct){
			flag = true;
			for(int j=0; j<20; j++){
				if(sites[i].schedule[j][day][prd] != NULL && courses[crs].tchWeek[j]){
					flag = false;
					break;
				}
			}
			if(flag) return i;
		}
	}
	return -1; 
	
}

int allocRoomDiff(int rmtype, int rmcpct, int week, int day, int prd)
{
	//分配零散课室
	bool flag;
	for(int i=0; i<sites.size(); i++){
		if(sites[i].type == rmtype && sites[i].capacity == rmcpct){
			if(sites[i].schedule[week][day][prd] == NULL)
				return i;
		}
	}
	//没有合适的就找能容纳下的 
	for(int i=0; i<sites.size(); i++){
		if(sites[i].type == rmtype && sites[i].capacity >= rmcpct){
			if(sites[i].schedule[week][day][prd] == NULL)
				return i;
		}
	}
	return -1; 
}

void getClsAttCrs()
{
	int t;
	for(int i=0; i<courses.size(); i++){
		for(int j=0; j<courses[i].attCls.size(); j++){
			t = courses[i].attCls[j];
			classes[t].attCrs.push_back(i);
		}
	}
}

void initSchedule()
{
	int r, c, w, d, p;
	for(int i=0; i<appms.size(); i++){
		r = appms[i].room;
		c = appms[i].course;
		w = appms[i].week;
		d = appms[i].day;
		p = appms[i].period;
		
		if(c >= 0){
			int t = courses[c].tutor;
			teachers[t].schedule[w][d][p] = &appms[i];
			for(int j=0; j<courses[c].attCls.size(); j++){
				int cl = courses[c].attCls[j];
				classes[cl].schedule[w][d][p] = &appms[i];
			}
		}
		if(r >= 0) sites[r].schedule[w][d][p] = &appms[i];
	}
}

void clearAppm(TSchedule* sch)
{
	for(int i=0; i<20; i++)
		for(int j=0; j<5; j++)
			for(int k=0; k<4; k++)
				(*sch)[i][j][k] = NULL;
}

void clearAll()
{
	for(int i=0; i<teachers.size(); i++) teachers[i].clearSchedule();
	for(int i=0; i<classes.size(); i++) classes[i].clearSchedule();
	for(int i=0; i<sites.size(); i++) sites[i].clearSchedule();
	appms.clear();
}

bool arranging()
{
	getClsAttCrs();
	initSchedule();
	
	int cnt = 0;
	bool wflag;
	vector<Appointment> backup = appms;
	do{
		wflag = true;
		clearAll();
		appms = backup;
		for(int i=0; i<classes.size(); i++){
			for(int j=0; j<classes[i].attCrs.size(); j++){
				int crs = classes[i].attCrs[j];
				if(!classes[i].insertCourse(crs)){
					wflag = false;
					break;
				}
			}
			if(!wflag) break;
		}
		++cnt;
	}while(!wflag && cnt < 20);
	if(cnt >= 20) return false;
	return true;
}






Appointment::Appointment()
{
	room = -1;
	course = -1;
	week = -1;
	day = -1;
	period = -1;
}

Appointment::Appointment(int r, int c, int w, int d, int p)
{
	room = r;
	course = c;
	week = w;
	day = d;
	period = p;
}

Teacher::Teacher()
{
	clearSchedule();
}

Teacher::Teacher(string id)
{
	tchId = id;
	clearSchedule();
}

void Teacher::clearSchedule()
{
	clearAppm(&schedule);
}

Site::Site()
{
	capacity = SMALL;
	clearSchedule();
}

Site::Site(string id, int c, int t)
{
	siteId = id;
	capacity = c;
	type = t;
	clearSchedule();
}

void Site::clearSchedule()
{
	clearAppm(&schedule);
}

StuClass::StuClass()
{
	clearSchedule();
}

StuClass::StuClass(string id)
{
	classId = id;
	clearSchedule();
}

bool StuClass::operator < (const StuClass& scl) const
{
	return attCrs.size() < scl.attCrs.size();
}

bool StuClass::operator > (const StuClass& scl) const
{
	return attCrs.size() > scl.attCrs.size();
}

bool StuClass::insertCourse(int crs)
{
	if(courses[crs].inserted) return true;
	
	vector<pair<int, int> > avlTime;
	bool avlb[5][4];
	
	for(int i=0; i<5; i++)
		for(int j=0; j<4; j++){
			avlb[i][j] = insertAble(crs, i, j);
			if(avlb[i][j]) avlTime.push_back(pair<int, int>(i, j));
		}
	
	int wkcnt = courses[crs].weekcnt;
	if(avlTime.size() < wkcnt) return false;
	int cpct = courses[crs].attCls.size() / 2;
	int cenv = courses[crs].env;
	int t = courses[crs].tutor;
	int day, prd;
//	int whcnt = 0;
	
	for(int w=0; w<wkcnt; w++){
		int rd = random(0, avlTime.size());
		day = avlTime[rd].first;
		prd = avlTime[rd].second;
		avlTime.erase(avlTime.begin() + rd);
		
		int rm = allocRoomSame(cenv, cpct, crs, day, prd);
		for(int i=0; i<20; i++){
			if(courses[crs].tchWeek[i]){
				if(rm == -1){
					rm = allocRoomDiff(cenv, cpct, i, day, prd);
					if(rm == -1) return false;
				}
				appms.push_back(Appointment(rm, crs, i, day, prd));
				Appointment* app = &appms[appms.size()-1];
				teachers[t].schedule[i][day][prd] = app;
				sites[rm].schedule[i][day][prd] = app;
				schedule[i][day][prd] = app;
				for(int j=0; j<courses[crs].attCls.size(); j++){
					int k = courses[crs].attCls[j];
					classes[k].schedule[i][day][prd] = app;
				}
				courses[crs].inserted = true;
			}
		}
	}
	return true;
}

bool StuClass::insertAble(int crs, int day, int prd)
{
	for(int i=0; i<20; i++){
		//检测周冲突
		if(schedule[i][day][prd] != NULL && courses[crs].tchWeek[i])
			return false;

		//检测老师是否得空
		int t = courses[crs].tutor;
		if(teachers[t].schedule[i][day][prd] != NULL)
			return false;
	}
	return true;
}

void StuClass::clearSchedule()
{
	clearAppm(&schedule);
}

Course::Course()
{
	weekcnt = 0;
	env = SMALL;
	tutor = -1;
	inserted = false;
    for(int i=0; i<20; i++) tchWeek[i] = false;
}

Course::Course(string id, string name, int w, int e, int t)
{
	crsId = id;
	crsName = name;
	weekcnt = w;
	env = e;
	tutor = t;
	inserted = false;
    for(int i=0; i<20; i++) tchWeek[i] = false;
}

