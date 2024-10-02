/*
 * coolang_define.h
 *
 *  Created on: 2022��8��16��
 *      Author: dell
 */

#ifndef COOLANG_DEFINE_H_
#define COOLANG_DEFINE_H_

#define debug false
#define Intg int
#define Intl int64_t
#define Numb double

#define safe_delete(p)  { if(p) { delete (p);     (p)=NULL; } }
#define FILEKEYEND "_end_"
#define FILEKEYBUILTIN "built-in"
#define COOLANG_PROJECT_PATH "COOLANG_PROJECT_PATH"
#define COOLANG_HOME "COOLANG_HOME"
#define COOLANG_SOURCE_CODE_SUFFIX ".cos"
#define COOLANG_CHARACTER_CODE_SUFFIX ".coc"
#define COOLANG_BINARY_CODE_SUFFIX ".cob"

#define T_  true
#define F_ false
#define A_ 3

#define N 0 //ʲôҲ����
#define QS 1
#define QE 2
#define C 3
#define M 4
#define ME 5
#define F 6
#define R 7  //����
#define B 8  //��֧
#define L 9  //ѭ��
#define TD 10 //�Ƶ�
#define S 11 //ϵͳ
#define Y 12 //����
#define LST 13 //�б�
#define FIL 14 //�ļ� ԭ���ϣ�FIL��LIN���ܴ�ϱ��ʽ������������������Ƕ����ʽ��FIL��LIN�ᱻֱ�Ӻ���
#define LIN 15 //����
#define LOAD 16 

#define Dz 0
#define Sz 1
#define Bs 2
#define Zf 3

#define S_Dz 0  //����arg��data���ɣ�����������Ϊ�������ĵ�ַ��������<dataTable>����ֵ
#define Sz 1    //����arg����data��������<dataTable>ֵ
#define S_Bs 2  //����arg��data���ɣ�����������Ϊ�������ı�ʶ��������<dataTable>����ֵ
#define Zf 3    //����arg��data���ɣ�����������Ϊ�ַ���������<dataTable>ֵ
#define Z_Dz 4  //����arg������������Ϊ�������ĵ�ַ����<dataTable>�б��滻ΪS_AR
#define M_Bs 5  //����arg����������һ��ģ���õ��ķ���+-*/��
#define M_Dz 6  //����arg����������һ��ģ���õ��ķ����ڷ��ű��еĵ�ַ
#define X_Bs 7  //����arg����������һ�������ڱ��������ԣ�������û��������ϵͳ���е���
#define X_Dz 8  //����arg����������һ�������ڱ��������ԣ�������û��������ϵͳ���е���
#define S_AR 9  //ͨ������data����������һ�����ʵ����������<dataTable>��ֵ
#define Y_Dz 10 //����arg����������һ�����ã����������ñ�������ΪCode����Ԫʽ��־λ��������<dataTable>����ע�⣬����Y_Dz��arg��rootAR����Ϊ��
#define S_LST LST//#define LST 13 //����arg������Data��������Data�д�����һ���б�
#define S_PTR 14//������Data������˵���Դ�data���и�ֵ������(��ѯ���ȴ�С��)������ʹ��ptr��ָdata����ɾ����dataʱ��ɾ��ptr��ָdata��

#define FN 1   //��������������
#define FB 2   //��ͨ������
#define RFB 3  //�����壬����return
#define BRB 4  //��֧����
#define LOB 5  //ѭ������
#define SYSB 6 //ϵͳ����

#define Du 2
#define R_ R //һ��������ִ��





#endif /* COOLANG_DEFINE_H_ */
