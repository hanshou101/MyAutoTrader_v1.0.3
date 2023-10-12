#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <time.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
using namespace std;
#include<iostream>  
#include<io.h> 
#include <afxwin.h> 

void main()
{
 CString filenames[1024];
 int count = 0;
 char path[MAX_PATH];
 while (cin >> path) { // . for current directory, empty for root directory
  FindAllFile(path, filenames, count);
  for (int i = 0; i < count; i++)
   cout << (LPCSTR)(filenames[i].GetBuffer(filenames[i].GetLength())) << endl;
 }
 return 0;

}



void test1()
{
	ofstream o_file("ag1406_20140512.txt",ios::trunc);
	//o_file << "20140428" << " " <<Q_BarTime_2<< " " << BSVolume <<" "<<"1409" <<" "<< Mn_open << " " << Mn_high << " " << Mn_low << " " << Mn_close << endl; //将内容写入到文本文件中
	o_file.close();						//关闭文件

}


void test2()
{

char line[1024]={0};
vector < double > data(10);
std::ifstream fin("ag1406_20140512.txt", std::ios::in);

while(fin.getline(line, sizeof(line)))
{
	std::stringstream word(line);
	//cout << line << endl;	//打印整行数据
	
		for (int i = 0; i < 10; i++)
		{
			word >> data[i];
			cout << "Configuration:" << data[i] << endl;	//打印某个数据
	
		}
		
}
fin.clear();
fin.close();

}

double sciToDouble(const string& str)
{
    stringstream ss(str);
    double d = 0;
    ss >> d;

    if (ss.fail())
    {
            string s = "无法把 [" + str + "] 转换为数值！";
            throw (s);
    }
    return d;
}

void FindAllFile(CString path, CString* filenames, int& count)
{
 CFileFind finder;
 BOOL working = finder.FindFile(path + "\\*.*");
 while (working)
 {
  working = finder.FindNextFile();
  if (finder.IsDots())
   continue;
  if (finder.IsDirectory())
  {
   //FindAllFile(finder.GetFilePath(), filenames, count);
  } 
  else 
  {
   CString filename = finder.GetFileName();
   filenames[count++] = filename;
  }
 }
}