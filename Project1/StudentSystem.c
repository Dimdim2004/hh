#include "StudentSystem.h"
#define MAX_LENGTH 20
#define STUDENT_TYPE 1
#define TEACHER_TYPE 2
#define ADMIN_TYPE 3
Classes* headList = NULL;

void MenuPrint() {
	system("cls");
	
	headList = ImportClassInfoFromFolder("C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\学生信息", headList);
	ExportAllStudentsToFile( headList, "C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\students.txt");
	system("cls");
	
	// 打印菜单
	printf("*****************************************************\n");
	printf("************\t欢迎使用学生管理系统\t*************\n");
	printf("*****************************************************\n");
	printf("*****************************************************\n");
	printf("*****************    1、账号登录    *****************\n");
	printf("*****************    2、退出系统    *****************\n");
	printf("*****************************************************\n");
	
	char choice[3]; // 为换行符和字符串结束符留出额外空间
	printf("请输入你想要的模式：");
	do {
		fgets(choice, sizeof(choice), stdin);
		
		// 去除换行符
		choice[strcspn(choice, "\n")] = '\0';
		
		// 检查输入是否为空，如果是，提示用户重新输入
		if (strlen(choice) == 0) {
			printf("输入为空，请重新输入：\n");
			continue;
		}
		
		// 检查输入是否是 '1' 或 '2'
		if (strlen(choice) == 1 && (choice[0] == '1' || choice[0] == '2')) {
			if (choice[0] == '1') {
				Login(); // 调用登录函数
				break;
			} else if (choice[0] == '2') {
				printf("选择了退出系统。\n");
				exit(0);
			}
		}
		ClearBuffer();
		printf("输入有误，请重新输入：\n");
	} while (1);
}



void ClearBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void safeInput(char* buffer, int maxLength) {
	fgets(buffer, maxLength, stdin); // 使用fgets接收用户输入
	buffer[strcspn(buffer, "\n")] = '\0'; // 去除换行符
}

void safeStrCopy(char* dest, const char* src, int maxLength) {
	strncpy(dest, src, maxLength - 1); // 使用strncpy进行安全拷贝
	dest[maxLength - 1] = '\0'; // 确保目标字符串以null结尾
}

int safeInputInt() {
	char buffer[MAX_LENGTH];
	int num;
	while (1) {
		safeInput(buffer, sizeof(buffer));
		if (sscanf(buffer, "%d", &num) == 1) {
			return num;
		} else {
			printf("请输入一个有效的数字：");
		}
	}
}

void Login() {
	char username[MAX_LENGTH];
	char password[MAX_LENGTH];
	int count = 3;
	system("cls");
	do {
		printf("请输入你的用户账号: ");
		safeInput(username, sizeof(username));
		printf("请输入你的用户密码: ");
		char key;
		int num = 0;
		while ((key = getch()) != '\r') {
			if (key == '\b') {
				if (num > 0) {
					num--;
					printf("\b \b");
				}
			} else {
				if (num < MAX_LENGTH - 1) {
					password[num++] = key;
					printf("*");
				}
			}
		}
		password[num] = '\0';
		printf("\n");
		Stu* studentResult = AuthenticateUserWithInput("C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\students.txt", username, password, STUDENT_TYPE);
		if (studentResult) {
			StudentMenu(studentResult);
			free(studentResult);
			return;
		}

		Tea* teacherResult = AuthenticateUserWithInput("C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\teachers.txt", username, password, TEACHER_TYPE);
		if (teacherResult) {
			TeacherMenu(teacherResult);
			free(teacherResult);
			return;
		}

		Admin* adminResult = AuthenticateUserWithInput("C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\admins.txt", username, password, ADMIN_TYPE);
		if (adminResult) {
			AdminMenu(adminResult);
			free(adminResult);
			return;
		}

		printf("账号密码不匹配，请重新输入\n");
		printf("您还有%d次机会\n", --count);
	} while (count > 0);

	if (count == 0) {
		printf("您的次数已用完！\n");
		char choice;
		do {
			printf("您是否要进行密码找回？(Y/N): ");
			scanf(" %c", &choice);
			ClearBuffer(); // 清空输入缓冲区
			if (choice == 'Y' || choice == 'y') {
				// 调用密码找回函数
				PasswordRecovery("C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目","C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\学生信息");
				break;
			} else if (choice == 'N' || choice == 'n') {
				printf("按任意键退出程序！\n");
				system("pause");
				exit(0);
				break;
			} else {
				printf("输入无效，请重新输入 (Y/N).\n");
			}
		} while (1);
	}
}
void* AuthenticateUserWithInput(const char *filename, const char *username, const char *password, int userType) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		perror("文件打开失败：");
		exit(EXIT_FAILURE);
	}
	char buffer[200];
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		if (userType == STUDENT_TYPE) {
			Stu student;
			if (sscanf(buffer, "%s %s %s %s %d %d %d", student.acc.idnumber, student.acc.password,student.name, student.className, &student.grade[0], &student.grade[1], &student.grade[2]) == 7) {
				if (strcmp(student.acc.idnumber, username) == 0 && strcmp(student.acc.password, password) == 0) {
					fclose(file);
					Stu* result = malloc(sizeof(Stu));
					*result = student;
					return result;
				}
			}
		} else if (userType == TEACHER_TYPE) {
			Tea teacher;
			if (sscanf(buffer, "%s %s %s %s", teacher.acc.idnumber, teacher.acc.password, teacher.name, teacher.className) == 4) {
				if (strcmp(teacher.acc.idnumber, username) == 0 && strcmp(teacher.acc.password, password) == 0) {
					fclose(file);
					Tea* result = malloc(sizeof(Tea));
					*result = teacher;
					return result;
				}
			}
		} else if (userType == ADMIN_TYPE) {
			Admin admin;
			if (sscanf(buffer, "%s %s %s", admin.acc.idnumber, admin.acc.password, admin.name) == 3) {
				if (strcmp(admin.acc.idnumber, username) == 0 && strcmp(admin.acc.password, password) == 0) {
					fclose(file);
					Admin* result = malloc(sizeof(Admin));
					*result = admin;
					return result;
				}
			}
		}
	}

	fclose(file);
	return NULL;
}


// 菜单函数
void StudentMenu(Stu* student) {
	Classes* current = FindClassByName(headList, student->className);
	if (current == NULL) {
		printf("未找到班级 %s\n", student->className);
		return;
	}
	CalculateRanks(current);
	

	char choice[3];
	do {
		system("cls");
		printf("欢迎您！%s 同学\n", student->name);
		printf("*****************************************************\n");
		printf("*****************  1、个人成绩查询  *****************\n");
		printf("*****************  2、查询本班信息  *****************\n");
		printf("*****************  3、个人成绩分析  *****************\n");
		printf("*****************  4、个人成绩申诉  *****************\n");
		printf("*****************  5、修改密码      *****************\n");
		printf("*****************  6、返回上一层    *****************\n");
		printf("*****************  0、退出该系统    *****************\n");
		printf("*****************************************************\n");
		printf("请输入你的选择：");
		fgets(choice, sizeof(choice), stdin);
		if (strlen(choice) == 2 && choice[1] == '\n') {
			switch (choice[0]) {
			case '1':
				DisplayStudentPersonalGrade(student);
				break;
			case '2':
				DisplayClassInformation(current);
				break;
			case '3':
				AnalyzeStudentGrades(student, current);
				break;
			case '4':
				InitializeTodoList(student);
				break;
			case '5':
				ChangeStudentPassword(student,"C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目","C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\学生信息");
				break;
			case '6':
				MenuPrint();
				break;
			case '0':
				exit(0);
				break;
			default:
				printf("输入有误，请重新输入：\n");
				break;
			}
			printf("\n按下任意键继续操作...\n");
			getchar(); // 等待用户按下任意键
		} else {
			ClearBuffer();
			printf("输入有误，请按回车键重新返回选择!\n");
			ClearBuffer();
		} 

	} while (1);
}


void InitializeTodoList(const Stu *student) {
	FILE *file = fopen("代办事项.txt", "a");
	if (file == NULL) {
		printf("无法打开文件。\n");
		return;
	}
	
	// 将所有信息放在一行中
	fprintf(file, "申请者：%s ID：%s 申请原因：学生 %s 的成绩需更新。\n",
		student->name, student->acc.idnumber,student->name);
	printf("已经将情况反馈！请耐心等候！");
	fclose(file);
}

void PasswordRecovery(const char* studentFolder, const char* classFolder){
	char studentID[MAX_LENGTH];
	char studentName[MAX_LENGTH];
	
	// 获取用户输入的学号
	printf("请输入你的学号: ");
	safeInput(studentID, sizeof(studentID));
	
	// 获取用户输入的姓名
	printf("请输入你的姓名: ");
	safeInput(studentName, sizeof(studentName));
	
	Stu* student = FindStudentInfo(studentName, studentID);
	
	char studentFilePath[256];
	snprintf(studentFilePath, sizeof(studentFilePath), "%s\\students.txt", studentFolder);
	UpdateStudentInfoInFile(studentFilePath, student, 0);
	
	// 更新班级文件中的学生密码
	char classFilePath[256];
	snprintf(classFilePath, sizeof(classFilePath), "%s\\%s.txt", classFolder, student->className);
	UpdateStudentInfoInFile(classFilePath, student, 1);
	
	printf("你的账号已被成功重置！");
	printf("请按回车键返回登陆界面...");
	ClearBuffer();
	MenuPrint();
}

Stu* FindStudentInfo(const char* name, const char* id) {
	FILE* fp = fopen("C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\students.txt", "r");
	if (fp == NULL) {
		printf("无法打开文件 \n");
		return NULL;
	}
	
	Stu* student = NULL;
	char buffer[MAX_LENGTH];
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		char currentIdnumber[MAX_LENGTH], currentPassword[MAX_LENGTH];
		char currentName[MAX_LENGTH], currentClassName[MAX_LENGTH];
		int grade[3];
		
		if (sscanf(buffer, "%s %s %s %s %d %d %d", currentIdnumber, currentPassword, currentName, currentClassName, &grade[0], &grade[1], &grade[2]) == 7) {
			if (strcmp(id, currentIdnumber) == 0 && strcmp(name, currentName) == 0) {
				student = (Stu*)malloc(sizeof(Stu));
				if (student == NULL) {
					printf("内存分配失败\n");
					fclose(fp);
					return NULL;
				}
				strcpy(student->name, currentName);
				strcpy(student->acc.idnumber,currentIdnumber);
				strcpy(student->acc.password, "123456");
				strcpy(student->className, currentClassName);
				for (int i = 0; i < 3; ++i) {
					student->grade[i] = grade[i];
				}
				break;
			}
		}
	}
	
	fclose(fp);
	if (student == NULL) {
		printf("学生信息不匹配\n");
		printf("按任意键退出程序！\n");
		system("pause");
		exit(0);
	}
	return student;
}

void DisplayStudentPersonalGrade(Stu* student) {
	system("cls");	
	printf("您好！%s同学\n", student->name);
	printf("你本次考试的成绩如下：\n");
	printf("语文: %d\n", student->grade[0]);
	printf("数学: %d\n", student->grade[1]);
	printf("英语: %d\n", student->grade[2]);
	printf("总分: %d\n", student->grade[2]+student->grade[1]+student->grade[0]);	
}

void AnalyzeStudentGrades(const Stu* student, const Classes* classPtr) {
	system("cls");
	printf("学生姓名：%s\n", student->name);
	printf("学生学号：%s\n", student->acc.idnumber);
	printf("所属班级：%s\n", student->className);
	printf("语文成绩：%d\n", student->grade[0]);
	printf("数学成绩：%d\n", student->grade[1]);
	printf("英语成绩：%d\n", student->grade[2]);
	
	// 计算班级平均分和总分平均分
	float subjectAverages[3];
	float totalAverage;
	CalculateClassAverages(classPtr, subjectAverages, &totalAverage);
	
	// 计算学生各科与班级平均分的偏差百分比
	float chinesePercentage = ((float)(student->grade[0] - subjectAverages[0]) / subjectAverages[0]) * 100;
	float mathPercentage = ((float)(student->grade[1] - subjectAverages[1]) / subjectAverages[1]) * 100;
	float englishPercentage = ((float)(student->grade[2] - subjectAverages[2]) / subjectAverages[2]) * 100;
	
	// 显示评估结果
	printf("语文成绩偏差百分比：%.2f%%\n", chinesePercentage);
	printf("数学成绩偏差百分比：%.2f%%\n", mathPercentage);
	printf("英语成绩偏差百分比：%.2f%%\n", englishPercentage);
	
	// 对偏差百分比进行评估
	printf("语文成绩评估：%s\n", chinesePercentage > 0 ? "高于班级平均分" : (chinesePercentage < 0 ? "低于班级平均分" : "与班级平均分相近"));
	printf("数学成绩评估：%s\n", mathPercentage > 0 ? "高于班级平均分" : (mathPercentage < 0 ? "低于班级平均分" : "与班级平均分相近"));
	printf("英语成绩评估：%s\n", englishPercentage > 0 ? "高于班级平均分" : (englishPercentage < 0 ? "低于班级平均分" : "与班级平均分相近"));
}

int CalculateTotalGrade(const Stu* student) {
	int total = 0;
	for (int i = 0; i < 3; ++i) {
		total += student->grade[i];
	}
	return total;
}

void CalculateRanks(Classes* classPtr) {
	// 计算每个学生的各科成绩排名
	for (Stu* current = classPtr->students; current != NULL; current = current->next) {
		for (int i = 0; i < 3; ++i) {
			int rank = 1;
			for (Stu* other = classPtr->students; other != NULL; other = other->next) {
				if (other != current && other->grade[i] > current->grade[i]) {
					++rank;
				}
			}
			current->rank[i] = rank;
		}
	}
	
	// 计算每个学生的总分排名
	for (Stu* current = classPtr->students; current != NULL; current = current->next) {
		int totalRank = 1;
		for (Stu* other = classPtr->students; other != NULL; other = other->next) {
			if (other != current && CalculateTotalGrade(other) > CalculateTotalGrade(current)) {
				++totalRank;
			}
		}
		current->totalRank = totalRank;
	}
}

void CalculateClassAverages(const Classes* classPtr, float* subjectAverages, float* totalAverage) {
	// 初始化各项成绩总和和学生数量
	int totalGrade[3] = {0};
	int studentCount = 0;
	
	// 遍历学生链表，累加各项成绩总和和学生数量
	Stu* current = classPtr->students;
	while (current != NULL) {
		for (int i = 0; i < 3; ++i) {
			totalGrade[i] += current->grade[i];
		}
		++studentCount;
		current = current->next;
	}
	
	// 计算各项成绩平均分
	for (int i = 0; i < 3; ++i) {
		subjectAverages[i] = (float)totalGrade[i] / studentCount;
	}
	
	// 计算总分平均分
	*totalAverage = ((float)totalGrade[0] + totalGrade[1] + totalGrade[2]) / (studentCount);
}

void UpdateStudentInfoInFile(const char* filename, Stu* student, int isClassFile) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		perror("文件打开失败：");
		exit(EXIT_FAILURE);
	}
	
	// 临时数组用于存储读取的每一行内容
	char buffer[256];
	// 临时文件用于暂存更新后的内容
	char tempFilename[256];

	snprintf(tempFilename, sizeof(tempFilename), "%s.temp", filename);
	FILE* tempFile = fopen(tempFilename, "w");
	if (tempFile == NULL) {
		perror("临时文件创建失败：");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	
	// 如果是班级文件，跳过第一行，因为它存储的是老师信息
	if (isClassFile ) {
		fgets(buffer, sizeof(buffer), file);
		fputs(buffer, tempFile);
	}
	
	int found = 0;//查看是否找到目标学生

	// 逐行读取文件内容并更新密码
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		// 提取每行数据的学号和密码
		char currentIdnumber[MAX_LENGTH], currentPassword[MAX_LENGTH], name[MAX_LENGTH], className[MAX_LENGTH];
		int grade[3];
		if (sscanf(buffer, "%s %s %s %s %d %d %d", currentIdnumber, currentPassword, name, className, &grade[0], &grade[1], &grade[2]) == 7) {
			// 如果当前行的学号与需要更新密码的学号匹配，则更新密码
			if (strcmp(name, student->name) == 0||strcmp(currentIdnumber, student->acc.idnumber) == 0) {
				fprintf(tempFile, "%s %s %s %s %d %d %d\n", student->acc.idnumber ,student->acc.password, student->name, student->className, student->grade[0], student->grade[1], student->grade[2]);
				found = 1; 
			} else {
				// 否则将当前行内容原样写入临时文件
				fputs(buffer, tempFile);
			}
		}
	}
	if (!found) {
		fputs("\n", tempFile);
		fprintf(tempFile, "%s %s %s %s %d %d %d\n", student->acc.idnumber ,student->acc.password, student->name, student->className, student->grade[0], student->grade[1], student->grade[2]);
	}
	fclose(file);
	fclose(tempFile);
	
	// 删除原始文件，并将临时文件重命名为原始文件名
	if (remove(filename) != 0) {
		perror("原始文件删除失败：");
		exit(EXIT_FAILURE);
	}
	if (rename(tempFilename, filename) != 0) {
		perror("临时文件重命名失败：");
		exit(EXIT_FAILURE);
	}
}

void ChangeStudentPassword(Stu* student, const char* studentFolder, const char* classFolder) {
	char oldPassword[20];
	char newPassword[20];
	char confirmPassword[20];
	
	do {
		printf("请输入原密码：");
		fgets(oldPassword, sizeof(oldPassword), stdin);
		oldPassword[strcspn(oldPassword, "\n")] = '\0'; // 移除末尾的换行符
		
		if (strcmp(oldPassword, student->acc.password) != 0) {
			printf("原密码输入错误，请重新输入！\n");
			continue;
		}
		
		do {
			printf("请输入新密码：");
			fgets(newPassword, sizeof(newPassword), stdin);
			newPassword[strcspn(newPassword, "\n")] = '\0'; // 移除末尾的换行符
			
			printf("请再次输入新密码：");
			fgets(confirmPassword, sizeof(confirmPassword), stdin);
			confirmPassword[strcspn(confirmPassword, "\n")] = '\0'; // 移除末尾的换行符
			
			if (strcmp(newPassword, confirmPassword) != 0) {
				printf("两次输入的密码不一致，请重新输入！\n");
			}
		} while (strcmp(newPassword, confirmPassword) != 0);
		
		// 更新学生结构体中的密码
		strcpy(student->acc.password, newPassword);
		
		char studentFilePath[256];
		snprintf(studentFilePath, sizeof(studentFilePath), "%s\\students.txt", studentFolder);
		UpdateStudentInfoInFile(studentFilePath, student, 0);
		
		// 更新班级文件中的学生密码
		char classFilePath[256];
		snprintf(classFilePath, sizeof(classFilePath), "%s\\%s.txt", classFolder, student->className);
		UpdateStudentInfoInFile(classFilePath, student, 1);
		
		printf("密码修改成功！\n");
		return;
	} while (1);
}

void TeacherMenu(Tea* teacher) {
	Classes* current = FindClassByName(headList, teacher->className);
	if (current == NULL) {
		printf("未找到班级 %s\n", teacher->className);
		return;
	}
	
	char choice[3];
	do {
		float subjectAverages[3];
		float totalAverage;
		CalculateClassAverages(current, subjectAverages, &totalAverage);
		CalculateRanks(current);
		system("cls");
		printf("欢迎您!%s老师\n", teacher->name);
		printf("*****************************************************\n");
		printf("*****************  1、增加学生信息  *****************\n");
		printf("*****************  2、删除学生信息  *****************\n");
		printf("*****************  3、查看学生信息  *****************\n");
		printf("*****************  4、修改学生信息  *****************\n");
		printf("*****************  5、展示本班成绩  *****************\n");
		printf("*****************  6、导出本班成绩  *****************\n");
		printf("*****************  7、本班成绩分析  *****************\n");
		printf("*****************  8、提醒成绩更新  *****************\n");
		printf("*****************  9、返回上一层    *****************\n");
		printf("*****************  0、退出该系统    *****************\n");
		printf("*****************************************************\n");
		printf("请输入你的选择：");
		fgets(choice, sizeof(choice), stdin);
		if (strlen(choice) == 2 && choice[1] == '\n') {
			switch (choice[0]) {
			case '1':
				AddSingleStudentIntoClass(headList, teacher->className); 
				// 增加学生信息函数
				break;
			case '2':
				DeleteStudentFromClassByID(current);
				// 删除学生信息函数
				break;
			case '3':
				CheckStudentInformation(current);
				// 查看学生信息函数
				break;
			case '4':
				ModifyStudentInfoInClass(current, "C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\学生信息", "C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目"); // 修改学生信息函数
				break;
			case '5':
				DisplayStudentImformationMenu(teacher); 
				// 展示本班成绩函数
				break;
			case '6':
				{
					char folderPath[MAX_LENGTH];
					printf("请输入想要导出的文件路径：");
					fgets(folderPath, sizeof(folderPath), stdin);
					folderPath[strcspn(folderPath, "\n")] = '\0';
					SaveStudentInfoToFile(current, folderPath); 
					// 导出本班成绩函数
					break;
				}
			case '7':
				AnalyzeClass(current); // 本班成绩分析函数
				break;
			case '8':
				// 提醒成绩更新函数
				break;
			case '9':
				MenuPrint(); // 返回上一层
				break;
			case '0':
				exit(0); // 退出该系统
				break;
			default:
				printf("输入有误，请重新输入！\n");
			}
			ClearBuffer();
			printf("\n按下任意键继续操作...\n");
			getchar();
		} else {
			ClearBuffer();
			printf("输入有误，请按回车键重新返回选择!\n");
			ClearBuffer();
		}
	} while (1);
}

void ViewTodoList() {
	FILE *file = fopen("代办事项.txt", "r");
	if (file == NULL) {
		printf("无法打开文件。\n");
		return;
	}
	
	char line[MAX_LENGTH];
	while (fgets(line, sizeof(line), file) != NULL) {
		printf("%s",line);
		char name[MAX_LENGTH], idnumber[MAX_LENGTH];
		if (sscanf(line, "申请者：%s ID：%s", name, idnumber) == 2) {
			printf("姓名：%s\n", name);
			printf("学号：%s\n", idnumber);
			Stu *student = FindStudentInfo(name, idnumber);
			if (student != NULL) {
				printf("班级：%s\n", student->className);
				printf("语文成绩：%d 数学成绩：%d 英语成绩：%d\n\n", student->grade[0],student->grade[1],student->grade[2]);
				
				// 打印菜单供教师选择
				int choice;
				do {
					PrintMenu();
					scanf("%d", &choice);
					getchar(); // 消耗换行符
					
					switch (choice) {
					case 1:
						SubmitToAdmin(student);
						break;
					case 2:
						RejectAppeal();
						break;
					default:
						printf("无效选项，请重新选择。\n");
						break;
					}
				} while (choice != 1 && choice != 2);
				free(student);
			}
		}
	}
	fclose(file);
}

void CheckStudentInformation(Classes* classPtr){
	char key[MAX_LENGTH];
	printf("请输入需要查询学生的名字或者ID:");
	fgets(key, sizeof(key), stdin);
	key[strcspn(key, "\n")] = '\0';
	
	Stu* student = FindStudentInClass(classPtr,key);
	if(student != NULL){	
		system("cls");	
		printf("学生姓名：%s\n", student->name);
		printf("学生学号：%s\n", student->acc.idnumber);
		printf("所属班级：%s\n", student->className);
		printf("语文成绩：%d\n", student->grade[0]);
		printf("数学成绩：%d\n", student->grade[1]);
		printf("英语成绩：%d\n", student->grade[2]);
		printf("总分: %d\n", student->grade[2]+student->grade[1]+student->grade[0]);
	}else{
		printf("查无此人！\n");
	}
}

void ModifyStudentInfoInClass(Classes* classPtr, const char* classFolder, const char* studentFolder) {
	// 显示班级学生信息供教师选择
	DisplayClassInformation(classPtr);
	
	// 提示教师输入要修改的学生信息
	char key[20];
	printf("请输入要修改信息的学生学号或姓名：");
	safeInput(key, sizeof(key));
	
	// 查找要修改信息的学生
	Stu* student = FindStudentInClass(classPtr, key);
	system("cls");
	if (student == NULL) {
		printf("未找到该名学生\n");
		return;
	}
	
	// 显示学生当前信息
	printf("学生信息如下：\n");
	printf("姓名：%s\n", student->name);
	printf("学号：%s\n", student->acc.idnumber);
	printf("密码：%s\n", student->acc.password);
	printf("班级：%s\n", student->className);
	printf("语文成绩：%d\n", student->grade[0]);
	printf("数学成绩：%d\n", student->grade[1]);
	printf("英语成绩：%d\n", student->grade[2]);
	printf("\n");
	
	int newGrades[3];
	
	if (strcmp(student->acc.idnumber, key) == 0) {
		// 通过学号查找到学生，要求输入姓名、密码和成绩
		printf("请输入修改后的学生姓名：");
		fgets(student->name, sizeof(student->name), stdin);
		student->name[strcspn(student->name, "\n")] = '\0';
		
		do {
			printf("请输入修改后的语文成绩：");
			newGrades[0] = safeInputInt();
			if (newGrades[0] > 100) {
				printf("错误：成绩不能超过100分，请重新输入。\n");
			}
		} while (newGrades[0] > 100);
		
		do {
			printf("请输入修改后的数学成绩：");
			newGrades[1] = safeInputInt();
			if (newGrades[1] > 100) {
				printf("错误：成绩不能超过100分，请重新输入。\n");
			}
		} while (newGrades[1] > 100);
		
		do {
			printf("请输入修改后的英语成绩：");
			newGrades[2] = safeInputInt();
			if (newGrades[2] > 100) {
				printf("错误：成绩不能超过100分，请重新输入。\n");
			}
		} while (newGrades[2] > 100);
	} else {
		// 通过姓名查找到学生，要求输入学号成绩
		printf("请输入修改后的学生学号：");
		fgets(student->acc.idnumber, sizeof(student->acc.idnumber), stdin);
		student->acc.idnumber[strcspn(student->acc.idnumber, "\n")] = '\0';
		
		do {
			printf("请输入修改后的语文成绩：");
			newGrades[0] = safeInputInt();
			if (newGrades[0] > 100) {
				printf("错误：成绩不能超过100分，请重新输入。\n");
			}
		} while (newGrades[0] > 100);
		
		do {
			printf("请输入修改后的数学成绩：");
			newGrades[1] = safeInputInt();
			if (newGrades[1] > 100) {
				printf("错误：成绩不能超过100分，请重新输入。\n");
			}
		} while (newGrades[1] > 100);
		
		do {
			printf("请输入修改后的英语成绩：");
			newGrades[2] = safeInputInt();
			if (newGrades[2] > 100) {
				printf("错误：成绩不能超过100分，请重新输入。\n");
			}
		} while (newGrades[2] > 100);
		
	}
	
	// 更新学生成绩
	for (int i = 0; i < 3; ++i) {
		student->grade[i] = newGrades[i];
	}
	
	// 更新班级文件中的学生信息和密码
	char studentFilePath[256];
	snprintf(studentFilePath, sizeof(studentFilePath), "%s\\students.txt", studentFolder);
	UpdateStudentInfoInFile(studentFilePath, student, 0);
	
	
	// 更新班级文件中的学生密码
	char classFilePath[256];
	snprintf(classFilePath, sizeof(classFilePath), "%s\\%s.txt", classFolder, student->className);
	UpdateStudentInfoInFile(classFilePath, student, 1);
	
	printf("学生信息已成功修改！\n");
	Sleep(2500); 
}


void DisplayStudentImformationMenu(Tea* teacher) {
	Classes* current = FindClassByName(headList, teacher->className);
	Classes* backupClassPtr = DeepCopyClass(current);
	int choice;
	do {
		system("cls");
		printf("请选择展示的方式\n");
		printf("*****************************************************\n");
		printf("*****************  1、按学号展示    *****************\n");
		printf("*****************  2、按总分展示    *****************\n");
		printf("*****************  3、返回上一层    *****************\n");
		printf("*****************  0、退出该程序    *****************\n");
		printf("*****************************************************\n");
		printf("请输入你的选择：");
		scanf("%d", &choice);
		ClearBuffer();
		switch (choice) {
		case 1:
			DisplayClassInformation(current);
			// 通过学号顺序展示
			break;
		case 2:
			Stu* sortedStudents = SortStudentsByTotalGrade(backupClassPtr->students);
			backupClassPtr->students = sortedStudents;
			DisplayClassInformation(backupClassPtr);	
			// 通过总分排名展示
			break;
		case 3:
			
			TeacherMenu(teacher);
			//返回教师菜单
			break;
		case 0:
			
			exit(0);
			// 增添账号密码函数
			break;
		default:
			printf("输入有误，请重新输入:\n");
		}
		// 提示用户按下任意键继续操作
		ClearBuffer();
		printf("\n按下任意键继续操作...\n");
		getchar(); // 等待用户按下任意键
	} while (1);
}

Classes* DeepCopyClass(Classes* original) {
	// 创建一个新的班级结构体副本
	Classes* copy = (Classes*)malloc(sizeof(Classes));
	if (copy == NULL) {
		perror("内存分配失败");
		exit(EXIT_FAILURE);
	}
	
	// 复制班级名称
	strcpy(copy->className, original->className);
	
	// 复制教师信息
	strcpy(copy->teacher.name, original->teacher.name);
	strcpy(copy->teacher.acc.idnumber, original->teacher.acc.idnumber);
	strcpy(copy->teacher.acc.password, original->teacher.acc.password);
	strcpy(copy->teacher.className, original->teacher.className);
	
	
	// 复制学生链表
	copy->students = NULL; // 初始化新链表头节点为NULL
	Stu* originalStudent = original->students;
	Stu* previousCopyStudent = NULL; // 用于跟踪新链表的上一个节点
	while (originalStudent != NULL) {
		// 复制学生信息
		Stu* copyStudent = (Stu*)malloc(sizeof(Stu));
		if (copyStudent == NULL) {
			perror("内存分配失败");
			exit(EXIT_FAILURE);
		}
		strcpy(copyStudent->name, originalStudent->name);
		strcpy(copyStudent->acc.idnumber, originalStudent->acc.idnumber);
		strcpy(copyStudent->acc.password, originalStudent->acc.password);
		strcpy(copyStudent->className, originalStudent->className);
		for (int i = 0; i < 3; i++) {
			copyStudent->grade[i] = originalStudent->grade[i];
			copyStudent->rank[i] = originalStudent->rank[i];
		}
		copyStudent->totalRank = originalStudent->totalRank;
		copyStudent->next = NULL; // 新节点的下一个节点初始化为NULL
		
		// 将新节点添加到新链表中
		if (copy->students == NULL) {
			copy->students = copyStudent; // 如果新链表为空，将新节点作为头节点
		} else {
			previousCopyStudent->next = copyStudent; // 否则将新节点连接到上一个节点的后面
		}
		
		// 更新上一个节点为当前节点
		previousCopyStudent = copyStudent;
		
		// 移动到下一个原始链表节点
		originalStudent = originalStudent->next;
	}
	
	return copy;
}

Stu* SortStudentsByTotalGrade(Stu* head) {
	int swapped;
	Stu *ptr1;
	Stu *lptr = NULL;
	Stu *newHead = head; // 新链表的头指针，默认为原始链表的头指针
	
	// 检查链表是否为空
	if (head == NULL)
		return NULL;
	
	do {
		swapped = 0;
		ptr1 = newHead;
		
		while (ptr1->next != lptr) {
			if (CalculateTotalGrade(ptr1) < CalculateTotalGrade(ptr1->next)) {
				// 交换节点的指针而不是节点本身
				Stu* temp = ptr1->next;
				ptr1->next = ptr1->next->next;
				temp->next = ptr1;
				if (ptr1 == newHead) {
					newHead = temp; // 更新新链表的头指针
				} else {
					Stu* prev = newHead;
					while (prev->next != ptr1) {
						prev = prev->next;
					}
					prev->next = temp;
				}
				swapped = 1;
			}
			else {
				ptr1 = ptr1->next;
			}
		}
		lptr = ptr1;
	} while (swapped);
	
	return newHead; // 返回新链表的头指针
}

void AnalyzeClass(Classes* classPtr) {
	float subjectAverages[3];
	float totalAverage;
	CalculateClassAverages(classPtr, subjectAverages, &totalAverage);
	CalculateRanks(classPtr);
	
	system("cls");
	printf("班级：%s\n", classPtr->className);
	printf("----------------------------------------------------------------------------------------------------\n");
	printf("|                        |  %-20s  |  %-20s  |  %-20s  |\n", "语文" , "数学" , "英语");
	printf("----------------------------------------------------------------------------------------------------\n");
	printf("|         平均分         |  %-20.2f  |  %-20.2f  |  %-20.2f  |\n",
		subjectAverages[0], subjectAverages[1], subjectAverages[2]);
	printf("----------------------------------------------------------------------------------------------------\n\n");
	int subjectDistribution[3][6] = {0};
	Stu* current = classPtr->students;
	while (current != NULL) {
		for (int i = 0; i < 3; i++) {
			
			if (current->grade[i] < 60) {
				subjectDistribution[i][0]++;
			} else if (current->grade[i] < 70) {
				subjectDistribution[i][1]++;
			} else if (current->grade[i] < 80) {
				subjectDistribution[i][2]++;
			} else if (current->grade[i] < 90) {
				subjectDistribution[i][3]++;
			} else if (current->grade[i] < 100) {
				subjectDistribution[i][4]++;
			} else {
				subjectDistribution[i][5]++;
			}
		}
		current = current->next;
	}
	printf("各科成绩分布情况：\n");
	printf("----------------------------------------------------------------------------------------------------\n");
	printf("|         分数段         |  %-20s  |  %-20s  |  %-20s  |\n", "语文" , "数学" , "英语");
	printf("----------------------------------------------------------------------------------------------------\n");
	printf("|          0-59          |  %-20d  |  %-20d  |  %-20d  |\n",
		subjectDistribution[0][0], subjectDistribution[1][0], subjectDistribution[2][0]);
	printf("|         60-69          |  %-20d  |  %-20d  |  %-20d  |\n",
		subjectDistribution[0][1], subjectDistribution[1][1], subjectDistribution[2][1]);
	printf("|         70-79          |  %-20d  |  %-20d  |  %-20d  |\n",
		subjectDistribution[0][2], subjectDistribution[1][2], subjectDistribution[2][2]);
	printf("|         80-89          |  %-20d  |  %-20d  |  %-20d  |\n",
		subjectDistribution[0][3], subjectDistribution[1][3], subjectDistribution[2][3]);
	printf("|         90-99          |  %-20d  |  %-20d  |  %-20d  |\n",
		subjectDistribution[0][4], subjectDistribution[1][4], subjectDistribution[2][4]);
	printf("|          100+          |  %-20d  |  %-20d  |  %-20d  |\n",
		subjectDistribution[0][5], subjectDistribution[1][5], subjectDistribution[2][5]);
	printf("----------------------------------------------------------------------------------------------------\n\n");
	
	int scoreDistribution[5]={0};
	current = classPtr->students;
	while (current != NULL) {
		
		int totalScore = current->grade[0] + current->grade[1] + current->grade[2];

		
		if (totalScore < 180) {
			scoreDistribution[0]++;
		} else if (totalScore < 210) {
			scoreDistribution[1]++;
		} else if (totalScore < 240) {
			scoreDistribution[2]++;
		} else if (totalScore < 270) {
			scoreDistribution[3]++;
		} else if (totalScore < 300) {
			scoreDistribution[4]++;
		} else {
			scoreDistribution[5]++;
		}
		
		current = current->next;
	}
	
	printf("----------------------------------------------------------------------------------------------------\n");
	printf("|         总分段        |           人数         |     %-40s     |\n"," ");
	printf("----------------------------------------------------------------------------------------------------\n");
	printf("|         0-179         |  %-20d  |     %-40s     |\n", scoreDistribution[0]," ");
	printf("|         180-209       |  %-20d  |     %-40s     |\n", scoreDistribution[1]," ");
	printf("|         210-239       |  %-20d  |     %-40s     |\n", scoreDistribution[2]," ");
	printf("|         240-269       |  %-20d  |     %-40s     |\n", scoreDistribution[3]," ");
	printf("|         270-299       |  %-20d  |     %-40s     |\n", scoreDistribution[4]," ");
	printf("|         300           |  %-20d  |     %-40s     |\n", scoreDistribution[4]," ");
	printf("----------------------------------------------------------------------------------------------------\n");
}

void AdminMenu(Admin* admin) {

	int choice;
	do {
		system("cls");
		printf("欢迎您!%s管理员\n", admin->name);
		printf("*****************************************************\n");
		printf("*****************  1、查看代办事项  *****************\n");
		printf("*****************  2、进入教师端    *****************\n");
		printf("*****************  3、增添账号密码  *****************\n");
		printf("*****************  4、删除账号密码  *****************\n");
		printf("*****************  5、修改账号密码  *****************\n");
		printf("*****************  6、批量录入账号  *****************\n");
		printf("*****************  7、批量导出账号  *****************\n");
		printf("*****************  8、返回上一层    *****************\n");
		printf("*****************  0、退出该程序    *****************\n");
		printf("*****************************************************\n");
		printf("请输入你的选择：");
		scanf("%d", &choice);
		ClearBuffer();
		switch (choice) {
			case 1:
				
				// 查看代办事项函数
				break;
			case 2:
				ChangeAdminIntoTeacher(admin);
				// 进入教师端函数
				break;
			case 3:
				AddUserAccountMenu(admin);
				// 增添账号密码函数
				break;
			case 4:
				DeleteStudentImformationMenu(admin);
				//删除账号密码
				break;
			case 5:
			
				
				// 修改账号密码函数
				break;
			case 6:
				char filename[256];
				printf("请输入存储学生信息文件的路径：");
				safeInput(filename, sizeof(filename));
				BatchEntry(headList, filename);
				break;
			case 7:
				// 批量导出账号函数
				SaveStudentInfoToFile(headList, "C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\学生信息");
				break;
			case 8:
				// 返回上一层
				MenuPrint();
				break;
			case 0:
				exit(0);
				break;
			default:
				printf("输入有误，请重新输入:\n");
		}
		
		ClearBuffer();
		printf("\n按下任意键继续操作...\n");
		getchar(); // 等待用户按下任意键
	} while (1);
}


void DeleteStudentImformationMenu(Admin* admin) {
	
	int choice;
	do {
		system("cls");
		printf("请选择删除的方式\n");
		printf("*****************************************************\n");
		printf("*****************  1、删除学生账号  *****************\n");
		printf("*****************  2、删除教师账号  *****************\n");
		printf("*****************  3、返回上一层    *****************\n");
		printf("*****************  0、退出该程序    *****************\n");
		printf("*****************************************************\n");
		printf("请输入你的选择：");
		scanf("%d", &choice);
		ClearBuffer();
		switch (choice) {
			case 1:
			
				// 通过名字删除
				break;
			case 2:
				
				// 通过学号删除
				break;
			case 3:

				//返回上一页
			case 0:
				exit(0);
				// 退出该程序 
				break;
			default:
				printf("输入有误，请重新输入:\n");
		}
		
		// 提示用户按下任意键继续操作
		ClearBuffer();
		printf("\n按下任意键继续操作...\n");
		getchar(); // 等待用户按下任意键
	} while (1);
}
void ChangeAdminIntoTeacher(Admin* admin) {
	printf("请输入你要查看的班级名字：");
	char className[MAX_LENGTH];
	scanf("%s", className);
	ClearBuffer();
	Tea teacher;
	strcpy(teacher.name, admin->name);
	strcpy(teacher.className, className);
	strcpy(teacher.acc.idnumber, admin->acc.idnumber);
	strcpy(teacher.acc.password, admin->acc.password);
	TeacherMenu(&teacher);
}

void AskSaveToFileAboutDelete(const char* classFolder, const char* studentFolder, Stu* student) {
	char choice;
	int isValidInput = 0;
	
	do {
		printf("是否将操作保存至文件？(Y/N): ");
		scanf(" %c", &choice);
		ClearBuffer(); // 清空缓冲区
		
		char classFilePath[256];
		snprintf(classFilePath, sizeof(classFilePath), "%s\\%s.txt", classFolder, student->className);
		
		if (choice == 'Y' || choice == 'y') {
			DeleteStudentInfoAndUpdateFile(classFilePath, student, 1);
			DeleteStudentInfoAndUpdateFile(studentFolder, student, 0);
			printf("已保存至文件。\n");
			isValidInput = 1; // 标记用户输入有效
		} else if (choice == 'N' || choice == 'n') {
			printf("未保存至文件。\n");
			isValidInput = 1; // 标记用户输入有效
		} else {
			printf("输入无效，请重新输入。\n");
		}
	} while (!isValidInput); // 当用户输入无效时继续循环询问
}


void DeleteStudentInfoAndUpdateFile(const char* filename, Stu* student,int isClassFile) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		perror("文件打开失败：");
		exit(EXIT_FAILURE);
	}
	
	// 临时数组用于存储读取的每一行内容
	char buffer[256];
	// 临时文件用于暂存更新后的内容
	char tempFilename[256];
	snprintf(tempFilename, sizeof(tempFilename), "%s.temp", filename);
	FILE* tempFile = fopen(tempFilename, "w");
	if (tempFile == NULL) {
		perror("临时文件创建失败：");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	
	// 如果是班级文件，跳过第一行，因为它存储的是老师信息
	if (isClassFile ) {
		fgets(buffer, sizeof(buffer), file);
		fputs(buffer, tempFile);
	}
	
	// 逐行读取文件内容并删除指定学生信息
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		// 提取每行数据的学号和班级名称
		char currentIdnumber[MAX_LENGTH], name[MAX_LENGTH], currentClassName[MAX_LENGTH],password[MAX_LENGTH];
		int grade[3];
		if (sscanf(buffer, "%s %s %s %s %d %d %d", currentIdnumber,password, name, currentClassName, &grade[0], &grade[1], &grade[2]) == 7) {
			// 如果当前行的班级名称与指定的班级名称相同，且学号不同，则保留该行
			if (strcmp(name, student->name) != 0) {
				fputs(buffer, tempFile);
			}
			
		}
	}
	
	fclose(file);
	fclose(tempFile);
	
	// 删除原始文件，并将临时文件重命名为原始文件名
	if (remove(filename) != 0) {
		perror("原始文件删除失败：");
		exit(EXIT_FAILURE);
	}
	if (rename(tempFilename, filename) != 0) {
		perror("临时文件重命名失败：");
		exit(EXIT_FAILURE);
	}
}


void SaveStudentInfoToFile(const Classes* classList, const char* folderPath) {
	const Classes* currentClass = classList;
	while (currentClass != NULL) {
		char filename[256];
		snprintf(filename, sizeof(filename), "%s\\%s.txt", folderPath, currentClass->className); // 使用班级名字作为文件名，并指定文件夹路径

		FILE* fp = fopen(filename, "w");
		if (fp == NULL) {
			perror("文件打开失败：");
			exit(EXIT_FAILURE);
		}
		fprintf(fp, "%s %s %s",currentClass->teacher.name, currentClass->teacher.acc.idnumber, currentClass->teacher.acc.password);
		const Stu* currentStudent = currentClass->students;
		while (currentStudent != NULL) {
			fprintf(fp, "%s %s %s %s %d %d %d\n",
			        currentStudent->acc.idnumber, currentStudent->acc.password,
			        currentStudent->name, currentStudent->className, currentStudent->grade[0], currentStudent->grade[1], currentStudent->grade[2]);
			currentStudent = currentStudent->next;
		}

		fclose(fp);
		currentClass = currentClass->next;
	}
}

void AddStudentToClass(Classes* classPtr, const Stu* student) {
	Stu* newStudent = (Stu*)malloc(sizeof(Stu));
	if (newStudent == NULL) {
		perror("内存分配失败：");
		exit(EXIT_FAILURE);
	}
	strcpy(newStudent->name, student->name);
	strcpy(newStudent->acc.idnumber, student->acc.idnumber);
	strcpy(newStudent->acc.password, student->acc.password);
	strcpy(newStudent->className, student->className);
	for (int i = 0; i < 3; i++) {
		newStudent->grade[i] = student->grade[i];
	}
	newStudent->next = NULL;

	// 添加学生节点到班级的学生链表
	if (classPtr->students == NULL) {
		classPtr->students = newStudent;
	} else {
		Stu* current = classPtr->students;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newStudent;
	}
}

void DeleteStudentFromClassByID(Classes* classPtr) {
	// 查找要删除的学生
	
	char key[MAX_LENGTH];
	printf("请输入查找的学生ID：");
	fgets(key, sizeof(key), stdin);
	key[strcspn(key, "\n")] = '\0';
	
	Stu* currentStudent = classPtr->students;
	Stu* prevStudent = NULL;
	while (currentStudent != NULL) {
		// 检查学号和姓名是否匹配
		if (strcmp(currentStudent->acc.idnumber, key) == 0 || strcmp(currentStudent->name, key) == 0) {
			// 删除当前学生节点
			if (prevStudent == NULL) {
				// 要删除的学生是链表的第一个节点
				classPtr->students = currentStudent->next;
			} else {
				// 要删除的学生不是链表的第一个节点
				prevStudent->next = currentStudent->next;
			}
			// 释放内存
			printf("成功在链表中删除学生 %s\n", key);
			AskSaveToFileAboutDelete("C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\学生信息", "C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目",currentStudent);
			free(currentStudent);

			return;
		}
		// 更新指针
		prevStudent = currentStudent;
		currentStudent = currentStudent->next;
	}
	// 没有找到要删除的学生
	printf("没有找到要删除的学生\n");
}

Stu* FindStudentInClass(Classes* classPtr, const char* key) {
	Stu* currentStudent = classPtr->students;
	while (currentStudent != NULL) {
		
		// 检查学号是否匹配
		if (strcmp(currentStudent->acc.idnumber, key) == 0) {
			return currentStudent;
		}
		// 检查姓名是否匹配
		if (strcmp(currentStudent->name, key) == 0) {
			return currentStudent;
		}

		currentStudent = currentStudent->next;
	}
	// 没有找到匹配的学生，返回NULL
	printf("没有找到对应的学生\n");
	return NULL;
}


void DisplayClassInformation(const Classes* classPtr) {
	system("cls");
	printf("-----------------------------------------------------------------------------\n");
	printf("| %-39s                                   |\n", classPtr->className);
	printf("| %-60s              |\n", " ");
	printf("| %-20s    | %-20s    |                       |\n", "班级教师姓名", "班级教师账号");
	printf("| %-20s    | %-20s    |                       |\n", classPtr->teacher.name, classPtr->teacher.acc.idnumber);
	printf("| %-60s              |\n", " ");
	printf("| %-5s | %-15s | %-15s | %-5s | %-5s | %-5s | %-5s |\n", "排名", "姓名", "学号", "语文", "数学", "英语", "总分");
	
	// 遍历学生链表并显示信息
	Stu* current = classPtr->students;
	while (current != NULL) {
		printf("| %-5d | %-15s | %-15s | %-5d | %-5d | %-5d | %-5d |\n", current->totalRank, current->name, current->acc.idnumber, current->grade[0], current->grade[1], current->grade[2], current->grade[0]+current->grade[1]+current->grade[2]);
		current = current->next;
	}
	
	// 显示班级的平均分和总分平均分
	float subjectAverages[3];
	float totalAverage;
	CalculateClassAverages(classPtr, subjectAverages, &totalAverage);
	printf("| %-60s              |\n", " ");
	printf("| %-20s    | %-20s   | %-20s   |\n", "语文平均分", "数学平均分","英语平均分");
	printf("| %-20.2f    | %-20.2f   | %-20.2f   |\n", subjectAverages[0], subjectAverages[1], subjectAverages[2]);
	printf("| %-60s              |\n", "总分平均分");
	printf("| %-60.2f              |\n", totalAverage);
	
	printf("-----------------------------------------------------------------------------\n");
}


Classes* CreateClass(Classes* headList, char* className) {
	char teacherID[MAX_LENGTH], teacherPassword[MAX_LENGTH];
	int MAX_ATTEMPTS = 3;
	for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt) {
		printf("请输入教师账号：");
		safeInput(teacherID, sizeof(teacherID));
		printf("请输入教师密码：");
		safeInput(teacherPassword, sizeof(teacherPassword));

		// 尝试验证用户账号密码
		Tea* authenticatedTeacher = AuthenticateUserWithInput("C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\teachers.txt", teacherID, teacherPassword, TEACHER_TYPE);
		// 如果验证成功，检查老师是否已有班级
		if (authenticatedTeacher != NULL) {
			// 如果老师已经有班级，不能创建新班级
		
				// 创建新班级
				Classes* newClass = (Classes*)malloc(sizeof(Classes));
				if (newClass == NULL) {
					perror("内存分配失败：");
					exit(EXIT_FAILURE);
				}
				
				strcpy(newClass->className, className);
				newClass->next = NULL;
				
				// 复制验证过的教师信息到新班级
				strcpy(newClass->teacher.name, authenticatedTeacher->name);
				strcpy(newClass->teacher.acc.idnumber, authenticatedTeacher->acc.idnumber);
				strcpy(newClass->teacher.acc.password, authenticatedTeacher->acc.password);
				
				// 更新教师的班级信息到文件
				//UpdateTeacherClass("C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\teachers.txt", teacherID, className);
				
				free(authenticatedTeacher);  // 释放验证过的教师信息
				return newClass;  // 返回新创建的班级
			
		} else {
			printf("教师账号或密码错误，剩余尝试次数：%d\n", MAX_ATTEMPTS - attempt);
		}
	}
	printf("已达到最大尝试次数，无法创建班级\n");
	return NULL;
}

void UpdateTeacherClass(const char* filename, const char* teacherID, const char* className) {
	FILE* file = fopen(filename, "r+");
	if (file == NULL) {
		perror("文件打开失败：");
		exit(EXIT_FAILURE);
	}
	
	// 临时数组用于存储读取的每一行内容
	char buffer[256];
	// 临时文件用于暂存更新后的内容
	char tempFilename[256];
	
	snprintf(tempFilename, sizeof(tempFilename), "%s.temp", filename);
	FILE* tempFile = fopen(tempFilename, "w");
	if (tempFile == NULL) {
		perror("临时文件创建失败：");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	
	// 逐行读取文件内容并更新教师班级信息
	int found = 0;
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		char currentID[MAX_LENGTH], currentPassword[MAX_LENGTH], currentName[MAX_LENGTH], currentClassName[MAX_LENGTH];
		if (sscanf(buffer, "%s %s %s %s", currentID, currentPassword, currentName, currentClassName) == 4) {
			if (strcmp(currentID, teacherID) == 0) {
				fprintf(tempFile, "%s %s %s %s\n", currentID, currentPassword, currentName, className);
				found = 1;
			} else {
				fputs(buffer, tempFile);
			}
		}
	}
	
	if (!found) {
		fprintf(tempFile, "%s %s %s %s\n", teacherID, "teacherPassword", "teacherName", className);
	}
	
	fclose(file);
	fclose(tempFile);
	
	// 删除原始文件，并将临时文件重命名为原始文件名
	if (remove(filename) != 0) {
		perror("原始文件删除失败：");
		exit(EXIT_FAILURE);
	}
	if (rename(tempFilename, filename) != 0) {
		perror("临时文件重命名失败：");
		exit(EXIT_FAILURE);
	}
}

Classes* FindClassByName(Classes* classList, const char* className) {
	
	Classes* current = classList;

	while (current != NULL) {
		if (strcmp(current->className, className) == 0) {
			return current;  // 找到了相应的班级
		}
		current = current->next;
	}

	return NULL;  // 未找到相应的班级
}

Classes* ImportClassInfoFromFolder(const char* folderPath, Classes* headList) {
	DIR *dir;
	struct dirent *ent;
	
	// 打开指定路径的文件夹
	if ((dir = opendir(folderPath)) != NULL) {
		// 遍历文件夹中的文件
		while ((ent = readdir(dir)) != NULL) {
			// 确保文件名以 ".txt" 结尾
			if (strstr(ent->d_name, ".txt") != NULL) {
				char filePath[256];
				snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, ent->d_name);
				
				// 从文件导入班级学生信息
				FILE* fp = fopen(filePath, "r");
				if (fp == NULL) {
					perror("文件打开失败：");
					continue;  // 继续处理下一个文件
				}
				
				// 创建班级节点
				Classes* currentClass = (Classes*)malloc(sizeof(Classes));
				if (currentClass == NULL) {
					perror("内存分配失败：");
					exit(EXIT_FAILURE);
				}
				
				// 从文件名获取班级名
				char className[MAX_LENGTH];
				char* fileName = strtok(ent->d_name, "."); // 去除文件后缀
				strcpy(className, fileName);
				strcpy(currentClass->className, className);
				
				// 读取老师信息并添加到当前班级
				char buffer[300];
				if (fgets(buffer, sizeof(buffer), fp) == NULL) {
					perror("读取老师信息失败：");
					continue; // 继续处理下一个文件
				}
				
				// 解析并存储教师信息
				char teacherName[MAX_LENGTH], teacherID[MAX_LENGTH], teacherPassword[MAX_LENGTH];
				sscanf(buffer, "%s %s %s", teacherName, teacherID, teacherPassword);
				strcpy(currentClass->teacher.name, teacherName);
				strcpy(currentClass->teacher.acc.idnumber, teacherID);
				strcpy(currentClass->teacher.acc.password, teacherPassword);
				
				currentClass->students = NULL;
				currentClass->next = NULL;
				
				// 将班级添加到班级列表
				if (headList == NULL) {
					headList = currentClass;
				} else {
					Classes* temp = headList;
					while (temp->next != NULL) {
						temp = temp->next;
					}
					temp->next = currentClass;
				}
				
				// 读取学生信息并添加到当前班级
				while (fgets(buffer, sizeof(buffer), fp) != NULL) {
					Stu* newStudent = (Stu*)malloc(sizeof(Stu));
					if (newStudent == NULL) {
						perror("内存分配失败：");
						exit(EXIT_FAILURE);
					}
					sscanf(buffer, "%s %s %s %s %d %d %d", 
						newStudent->acc.idnumber, newStudent->acc.password, newStudent->name, 
						newStudent->className, &newStudent->grade[0], &newStudent->grade[1], &newStudent->grade[2]);
					newStudent->next = NULL;
					
					// 将学生添加到当前班级
					AddStudentToClass(currentClass, newStudent);
				}
				fclose(fp);
			}
		}
		closedir(dir); // 关闭文件夹
	} else {
		perror("无法打开文件夹");
		exit(EXIT_FAILURE);
	}
	printf("文件内容导入成功！\n");
	Sleep(1500); 
	system("cls"); 
	return headList; 
}



void AddUserAccountMenu(Admin* admin) {
	
	int choice;
	do {
		system ("cls");
		printf("*****************************************************\n");
		printf("*****************  1、添加学生账号  *****************\n");
		printf("*****************  2、添加教师账号  *****************\n");
		printf("*****************  3、添加管理员账号 ****************\n");
		printf("*****************  4、返回上一页    *****************\n");
		printf("*****************  0、退出系统      *****************\n");
		printf("*****************************************************\n");
		printf("请输入你想要的模式：");
		scanf("%d", &choice);
		ClearBuffer();
		switch (choice) {
			case 1:
				char className[MAX_LENGTH];
				printf("请输入学生所属班级名称：");
				scanf("%s", className);
				AddSingleStudentIntoClass(headList,className);
				break;
			case 2:
				AddTeacherAccount();
				break;
			case 3:
				AddAdminAccount();
				break;
			case 4:
				AdminMenu(admin);
				break;
			case 0:
				exit(0);
			default:
				printf("输入有误，请重新输入:\n");
		}
		// 提示用户按下任意键继续操作
		ClearBuffer();
		printf("\n按下任意键继续操作...\n");
		ClearBuffer(); // 等待用户按下任意键
	} while (1);
}
void AddTeacherAccount() {
	Tea new_teacher;
	
	// 输入教师信息，包括姓名、工号、密码等
	printf("请输入教师姓名：");
	scanf("%s", new_teacher.name);
	printf("请输入教师工号：");
	scanf("%s", new_teacher.acc.idnumber);
	printf("请输入教师密码：");
	scanf("%s", new_teacher.acc.password);
	
	// 保存教师信息到文件中
	FILE* file = fopen("C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\teachers.txt", "a");
	if (file == NULL) {
		perror("文件打开失败：");
		exit(EXIT_FAILURE);
	}
	fprintf(file, "\n");
	fprintf(file, "%s %s %s\n", new_teacher.acc.idnumber, new_teacher.acc.password, new_teacher.name);
	fclose(file);
	
	printf("教师账号添加成功！\n");
}

void AddAdminAccount() {
	Admin new_admin;
	
	// 输入管理员信息，包括姓名、工号、密码等
	printf("请输入管理员工号：");
	safeInput(new_admin.acc.idnumber,sizeof(new_admin.acc.idnumber));
	printf("请输入管理员密码：");
	safeInput(new_admin.acc.password,sizeof(new_admin.acc.password));
	printf("请输入管理员姓名：");
	safeInput(new_admin.name,sizeof(new_admin.name));

	// 保存管理员信息到文件中
	FILE* file = fopen("C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\admin.txt", "a");
	if (file == NULL) {
		perror("文件打开失败：");
		exit(EXIT_FAILURE);
	}
	fprintf(file, "\n");
	fprintf(file, "%s %s %s\n", new_admin.acc.idnumber, new_admin.acc.password, new_admin.name);
	fclose(file);
	
	printf("管理员账号添加成功！\n");
}
void AddTeacherInfoToFile() {
	char teacherID[MAX_LENGTH];
	char teacherPassword[MAX_LENGTH];
	char teacherName[MAX_LENGTH];

	// 获取管理员输入的老师信息
	printf("请输入要添加的老师账号：");
	safeInput(teacherID,sizeof(teacherID));
	printf("请输入要添加的老师密码：");
	safeInput(teacherPassword,sizeof(teacherPassword));
	printf("请输入要添加的老师姓名：");
	safeInput(teacherName,sizeof(teacherName));

	// 打开teacher.txt文件，将老师信息追加写入文件
	FILE* teacherFile = fopen("teachers.txt", "a");
	if (teacherFile == NULL) {
		perror("文件打开失败：");
		exit(EXIT_FAILURE);
	}

	// 写入老师信息到文件
	fprintf(teacherFile, "%s %s %s\n", teacherID, teacherPassword, teacherName);
	fclose(teacherFile);

	printf("老师信息添加成功并保存到文件！\n");
}
void AddSingleStudentIntoClass(Classes* headList, const char* className) {
	Stu* newStudent = (Stu*)malloc(sizeof(Stu));
	if (newStudent == NULL) {
		perror("内存分配失败：");
		exit(EXIT_FAILURE);
	}
	
	char newID[MAX_LENGTH],  newPassword[MAX_LENGTH];
	int newGrades[3];
	printf("请输入学生姓名：");
	fgets(newID, sizeof(newID), stdin);
	newID[strcspn(newID, "\n")] = '\0';
	printf("请输的学生密码：");
	fgets(newPassword, sizeof(newPassword), stdin);      
	newPassword[strcspn(newPassword, "\n")] = '\0';
	
	do {
		printf("请输入语文成绩：");
		newGrades[0] = safeInputInt();
		if (newGrades[0] > 100) {
			printf("错误：成绩不能超过100分，请重新输入。\n");
		}
	} while (newGrades[0] > 100);
	
	do {
		printf("请输入数学成绩：");
		newGrades[1] = safeInputInt();
		if (newGrades[1] > 100) {
			printf("错误：成绩不能超过100分，请重新输入。\n");
		}
	} while (newGrades[1] > 100);
	
	do {
		printf("请输入英语成绩：");
		newGrades[2] = safeInputInt();
		if (newGrades[2] > 100) {
			printf("错误：成绩不能超过100分，请重新输入。\n");
		}
	} while (newGrades[2] > 100);
	
	Classes* currentClass = FindClassByName(headList, className);
	if (currentClass == NULL) {
		fprintf(stderr, "错误：未找到班级 %s\n", className);
		free(newStudent);
		return;
	}
	
	AddStudentToClass(currentClass, newStudent);
}

void ExportAllStudentsToFile(Classes* headList, const char* filename) {
	FILE* fp = fopen(filename, "w");
	if (fp == NULL) {
		perror("文件打开失败：");
		return;
	}
	
	Classes* currentClass = headList;
	while (currentClass != NULL) {
		Stu* currentStudent = currentClass->students;
		while (currentStudent != NULL) {
			fprintf(fp, "%s %s %s %s %d %d %d\n",
				currentStudent->acc.idnumber, currentStudent->acc.password,
				currentStudent->name, currentStudent->className,
				currentStudent->grade[0], currentStudent->grade[1], currentStudent->grade[2]);
			currentStudent = currentStudent->next;
		}
		currentClass = currentClass->next;
	}
	
	fclose(fp);
}

//C:\Users\bb\Desktop\2023届学生管理系统\数据\class1的副本.txt

// 导出班级信息到文件
void ExportClassToFile(Classes* classPtr, const char* folderPath) {
	char filename[256];
	snprintf(filename, sizeof(filename), "%s\\%s.txt", folderPath, classPtr->className);

	FILE* fp = fopen(filename, "w+");
	if (fp == NULL) {
		perror("文件打开失败：");
		return;
	}
	// 写入老师信息
	fprintf(fp, "%s %s %s\n", classPtr->teacher.name, classPtr->teacher.acc.idnumber, classPtr->teacher.acc.password);
	
	// 写入学生信息
	const Stu* currentStudent = classPtr->students;
	while (currentStudent != NULL) {
		fprintf(fp, "%s %s %s %s %d %d %d\n",
			currentStudent->acc.idnumber, currentStudent->acc.password,
			currentStudent->name, currentStudent->className,
			currentStudent->grade[0], currentStudent->grade[1], currentStudent->grade[2]);
		currentStudent = currentStudent->next;
	}
	
	fclose(fp);
}

void AppendStudentsToFile(const Classes* classPtr, const char* filePath) {

	FILE* fp = fopen(filePath, "a"); // 打开文件以追加方式写入
	if (fp == NULL) {
		perror("文件打开失败：");
		return;
	}
	
	// 将学生信息追加到文件中
	const Stu* currentStudent = classPtr->students;
	while (currentStudent != NULL) {
		fprintf(fp, "%s %s %s %s %d %d %d\n",
			currentStudent->acc.idnumber, currentStudent->acc.password,
			currentStudent->name, currentStudent->className,
			currentStudent->grade[0], currentStudent->grade[1], currentStudent->grade[2]);
		currentStudent = currentStudent->next;
	}
	
	fclose(fp);
}

// 批量导入班级信息
Classes* BatchEntry(Classes* headList, const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		perror("文件打开失败：");
		return headList;
	}
	
	Classes* currentClass = NULL;
	
	int grade[3];
	char name[MAX_LENGTH], idNumber[MAX_LENGTH], className[MAX_LENGTH];
	int isFirstIteration = 1; // 标志变量，记录是否是第一次循环
	while (fscanf(fp, "%s %s %s %d %d %d", name, idNumber, className, &grade[0], &grade[1], &grade[2]) != EOF) {
		// 检查当前班级是否已经存在于链表中
		currentClass = FindClassByName(headList, className);
		
		if (currentClass != NULL) {
			// 如果存在同名班级，且是第一次循环，则询问是否覆盖内容
			if (isFirstIteration) {
				char choice;
				do {
					printf("班级 %s 已存在，是否覆盖其内容？(Y/N): ", className);
					scanf(" %c", &choice);
					ClearBuffer(); // 清空缓冲区
					if (choice == 'Y' || choice == 'y') {
						// 覆盖当前班级节点的内容
						ClearClassInfo(currentClass); 
						break;
					} else if (choice == 'N' || choice == 'n') {
						break; // 用户选择不覆盖，则跳过导入该班级
					} else {
						printf("输入无效，请重新输入 (Y/N).\n");
					}
				} while (1);
				isFirstIteration = 0; // 将标志变量设为 false，表示已经不是第一次循环了
			} 
		} else {
			// 如果当前班级不在班级列表中，创建一个新的班级
			currentClass = CreateClass(headList, className);
			if (headList == NULL) {
				headList = currentClass;
			} else {
				Classes* temp = headList;
				while (temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = currentClass;
			}
		}
		
		// 创建学生
		Stu student;
		strcpy(student.name, name);
		strcpy(student.acc.idnumber, idNumber);
		strcpy(student.acc.password, "123456");
		strcpy(student.className, className);
		for (int i = 0; i < 3; i++) {
			student.grade[i] = grade[i];
		}
		
		// 将学生添加到当前班级
		AddStudentToClass(currentClass, &student);
	}
	fclose(fp);
	
	// 导入成功后询问用户是否导出至文件
	char choice;
	printf("是否将导入的班级信息导出至文件？(Y/N): ");
	do {
		scanf(" %c", &choice);
		ClearBuffer(); // 清空缓冲区
		if (choice == 'Y' || choice == 'y') {
			// 导出班级信息到文件
			ExportClassToFile(currentClass, "C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\学生信息");
			// 追加学生信息到 student.txt
			AppendStudentsToFile(currentClass,"C:\\Users\\bb\\Desktop\\2023届学生管理系统\\系统项目\\students.txt");
			printf("导入成功！");
			break;
		} else if (choice == 'N' || choice == 'n') {
			break;
		} else {
			printf("输入无效，请重新输入 (Y/N).\n");
		}
	} while (1);
	
	return headList;
}


void ClearClassInfo(Classes* classPtr) {
	// 清空老师信息
	strcpy(classPtr->teacher.name, "");
	strcpy(classPtr->teacher.acc.idnumber, "");
	strcpy(classPtr->teacher.acc.password, "");
	
	// 释放学生链表内存
	Stu* currentStudent = classPtr->students;
	while (currentStudent != NULL) {
		Stu* temp = currentStudent;
		currentStudent = currentStudent->next;
		free(temp);
	}
	classPtr->students = NULL; // 将班级链表的学生指针设为空
	
	
	printf("班级 %s 内容已成功清空！\n", classPtr->className);
}



int main() {
	MenuPrint();
	return 0;
}
