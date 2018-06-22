#ifndef __ARRANGE_H__
#define __ARRANGE_H__

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
using namespace std;

//todo����������ڼǵü�srand(time(NULL));

//�������ͣ�����ͨ���ҡ���ý����ҡ�����
enum SiteType { NORMAL = 0, MTMD, CPTRM };

//����������С����(1����)���п���(3����)�������(5����)
enum SiteCpct { SMALL = 0, MEDIUM, LARGE };

//��ʦλ�ã�������ǰ/��һ����N¥�пΡ�ǰ/��һ�����о�Ժ�п�
//enum TchPosi { NORMAL = 0, NORTH, SOUTH };

class Appointment {
public:
	int room;
	int course; //���ǿγ�ʱΪ-1 
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
	string classId;			//���
	
	vector<int> attCrs;		//�ƻ��޵Ŀγ�
	TSchedule schedule;		//�γ̱�
	
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
	bool tchWeek[20];	//�Ͽ��ܴ�
	int weekcnt;		//ÿ�ܿ�ʱ��
	int env;			//�Ͽλ������豸����
	bool inserted;
	
	vector<int> attCls;	//�Ͽΰ༶
	int tutor;			//�Ͽν�ʦ
	
	Course();
	Course(string id, string name, int w, int e, int t);
};

extern vector <Teacher>   	teachers;  	//��ʦ��
extern vector <Site>      	sites;	 	//���ұ�
extern vector <Course>    	courses;	//�γ̼ƻ���
extern vector <StuClass> 	classes;   	//�༶��
extern vector <Appointment>	appms;		//�����ſα�

bool arranging();

#endif

