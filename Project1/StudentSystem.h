#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dirent.h>
#include <windows.h>
#include <stdbool.h>
typedef struct Account {
	char idnumber[20];
	char password[20];
} Acc;


typedef struct Student {
	char name[20];
	Acc acc;
	char className[20];
	int grade[3];
	int rank[3];
	int totalRank;
	struct Student* next;
} Stu;// 定义学生结构体


typedef struct Teacher {
	char name[20];
	Acc acc;
	char className[20];
	char email[100];
} Tea;// 定义教师结构体
typedef struct Admin {
	Acc acc;
	char name[20];
} Admin;

typedef struct Class {
	char className[20];
	Stu* students; // 班级的学生链表
	Tea teacher;
	struct Class* next;
} Classes;// 定义班级结构体

void MenuPrint();//打印登录主页面
void safeInput(char* buffer, int maxLength);//安全输入函数
void safeStrCopy(char* dest, const char* src, int maxLength);//安全拷贝函数
void ClearBuffer();//清除缓冲区
void Login();//登录程序
void StudentMenu(Stu* student);//打印学生页面
void TeacherMenu(Tea* teacher);//打印教师界面
void AdminMenu(Admin* admin);//打印管理员界面
void* AuthenticateUserWithInput(const char *filename, const char *username, const char *password, int userType);//身份匹配
Classes* CreateClass(Classes* headList, char* className);//创建班级链表
void AddStudentToClass(Classes* classPtr,const Stu* Student);//添加学生
void DeleteStudentFromClassByID(Classes* classPtr);//移除学生
Classes* FindClassByName(Classes* classList, const char* className);//根据班级名字查找班级
Stu* FindStudentInClass(Classes* classPtr, const char* key);//查询学生成绩
void DisplayClassInformation(const Classes* classPtr);//打印班级信息
Classes* BatchEntry(Classes* headList, const char* filename);//将文件信息批量录入
void SaveStudentInfoToFile(const Classes* classList, const char* folderPath);//将各个班级信息进行保存
void ChangeAdminIntoTeacher(Admin* admin);//用管理员打开教师端
void AddSingleStudentIntoClass(Classes* headList, const char* className);//加入单独的学生
void LoadClasslistToFile();//从文件夹中加载信息
void DeleteStudentImformation(Classes* list);
Classes* ImportClassInfoFromFolder(const char* folderPath, Classes* headList) ;//导入原先的内容
void DeleteStudentImformationMenu(Admin* admin);
void AddTeacherInfoToFile();//保存教师信息
void AddUserAccountMenu(Admin* admin);
void AddAdminAccount();
void AddTeacherAccount();
void ChangePassword();
void DisplayStudentPersonalGrade(Stu* student);//展示个人成绩
void CalculateClassAverages(const Classes* classPtr, float* subjectAverages, float* totalAverage);//计算平均数
void AnalyzeStudentGrades(const Stu* student, const Classes* classPtr);//分析学生成绩函数
int CalculateTotalGrade(const Stu* student);
void CalculateRanks(Classes* classPtr);//计算全班排名
void UpdateStudentInfoInFile(const char* filename, Stu* student, int isClassFile); //将修改后的密码更新至文件中
void ChangeStudentPassword(Stu* student, const char* studentFolder, const char* classFolder);//修改文件
void CheckStudentInformation(Classes* classPtr);//查看学生信息
void ModifyStudentInfoInClass(Classes* classPtr, const char* classFolder,const char* studentFolder);//修改学生信息
Stu* SortStudentsByTotalGrade(Stu* head);//实现学生链表的冒泡排序
void DisplayStudentImformationMenu(Tea* teacher);//展示学生信息菜单
void AnalyzeClass(Classes* classPtr);
Classes* DeepCopyClass(Classes* original);//复制班级链表信息
void AskSaveToFileAboutDelete();//确认是否删除
void DeleteStudentInfoAndUpdateFile(const char* filename, Stu* student,int isClassFile);//将删除信息保存至文件中
void ClearClassInfo(Classes* classPtr);//清空旧班级链表
void ExportClassToFile(Classes* classPtr, const char* folderPath);//导出单个班级文件
void AppendStudentsToFile(const Classes* classPtr, const char* filePath);//追加到学生文件中
void UpdateTeacherClass(const char* filename, const char* teacherID, const char* className);//更新教师文件
void ExportAllStudentsToFile(Classes* headList, const char* filename);//更新student.txt的内容
void InitializeTodoList(Stu *student);//申诉成绩
void PasswordRecovery();//找回密码
Stu* FindStudentInfo(const char* name, const char* id);
void SubmitToAdmin(Stu *student, const char *todoItem);//将申诉信息写到管理员端
void ViewTodoList();//查看代办
void RejectAndRecordAppeal(Stu* student);//驳回申诉
bool CheckStudentExistence(Stu* student);
bool CheckRejectedAppeals(const char *studentName);//查看驳回申诉的记录
void DeleteTodoItem(const char *filename, const char *lineToDelete);//处理后对待办事项进行删除
void ModifyStudentInfo(Stu* student, const char* classFolder, const char* studentFolder);//直接更改学生信息
void ViewTodoListAdmin();//管理员端的查看代办
