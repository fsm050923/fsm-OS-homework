#include <iostream>
#include <iomanip>
#include <malloc.h>
class DiskArm {
public:
 DiskArm();
 ~DiskArm();
 void InitSpace(char* MethodName); //��ʼ��Ѱ����¼
 void Report(); //�����㷨ִ?���
 void FCFS(); 
 void SSTF();
 void SCAN();
 void CSCAN();
 void LOOK();
private:
 int *Request; //�����������
 int *Cylinder; //?����?����
 int RequestNumber; //����������
 int CurrentCylinder; //��ǰ����
 int SeekDirection; //��ͷ?��
 int SeekNumber; //�Ʊ�����
 int SeekChang; //��ͷ��ͷ��
};
