#ifndef __ARRANGE_H__
#define __ARRANGE_H__

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
using namespace std;

//todo：主程序入口记得加srand(time(NULL));

//教室类型：、普通教室、多媒体教室、机房
enum SiteType { NORMAL = 0, MTMD, CPTRM };

//教室容量：小课室(1个班)、中课室(3个班)、大课室(5个班)
enum SiteCpct { SMALL = 0, MEDIUM, LARGE };

//教师位置：正常、前/后一节在N楼有课、前/后一节在研究院有课
//enum TchPosi { NORMAL = 0, NORTH, SOUTH };

class Appointment {
public:
	int room;
	int course; //不是课程时为-1 
	int week, day, period;

	Appointment();
	Appointment(int r, int c, int w, int d, int p);
};

typedef Appointment* TSchedule[20][5][4];

class Teacher {
public:
	string tchId;
	
	TSchedule schedule;
	
	Teacher();
	Teacher(string id);
	void clearSchedule();
};

class Site {
public:
	string siteId;
	int capacity;
	int type;
	
	TSchedule schedule;
	
	Site();
	Site(string id, int c, int t);
	void clearSchedule();
};

class StuClass {
public:
	string classId;			//班号
	
	vector<int> attCrs;		//计划修的课程
	TSchedule schedule;		//课程表
	
	StuClass();
	StuClass(string id);
	bool insertCourse(int crs);
	bool insertAble(int crs, int day, int prd);
	bool operator < (const StuClass& scl) const;
	bool operator > (const StuClass& scl) const;
	void clearSchedule();
};

class Course {
private:
	
public:
	string crsId;
	string crsName;
	bool tchWeek[20];	//上课周次
	int weekcnt;		//每周课时数
	int env;			//上课环境（设备需求）
	bool inserted;
	
	vector<int> attCls;	//上课班级
	int tutor;			//上课教师
	
	Course();
	Course(string id, string name, int w, int e, int t);
};

extern vector <Teacher>   	teachers;  	//教师表
extern vector <Site>      	sites;	 	//教室表
extern vector <Course>    	courses;	//课程计划表
extern vector <StuClass> 	classes;   	//班级表
extern vector <Appointment>	appms;		//最终排课表

bool arranging();

#endif

