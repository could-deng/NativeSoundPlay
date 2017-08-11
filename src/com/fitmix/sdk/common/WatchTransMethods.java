package com.fitmix.sdk.common;

public class WatchTransMethods {
	
	public static native int square(int num);
	
	/*
	����˵��:��IronCloud��GPS���ݺ�Distance�����ںϳ�jason�ļ�
	����˵��:pGPSFile		GPS�����ļ�·��(in)
			 pDistanceFile	Distance�����ļ�·��(in)
			 pJasonFile		Json�����ļ�·��(out)
	�� �� ֵ:		0		�ɹ�
					1		��������
					2		���ܴ���jason�ļ�
					3		�ڴ����ʧ��
					4		�����ļ���������

	 */
    public static native int converToGPSJason(String pGPSFile,String pDistanceFile,String pJasonFile);
    /*
	����˵��:��IronCloud��Step���ݺ�Distance����,HR�����ںϳ�jason�ļ�
	����˵��:pStepFile		Step�����ļ�·��(in)
			 pDistanceFile	Distance�����ļ�·��(in)
			 pHRFile		HR�����ļ�·��(in)
			 pJasonFile		Json�����ļ�·��(out)
	�� �� ֵ:		0		�ɹ�
					1		��������
					2		���ܴ���jason�ļ�
					3		�ڴ����ʧ��
					4		�����ļ���������

    */
    public static native int converToStepJason(String pStepFile, String pDistanceFile, String pHRFile, String pJasonFile);

    /*
	����˵��:��Log�����ںϳ�jason�ļ�
	����˵��:pLogFile		Log�����ļ�·��(in)
			 pJasonFile		Json�����ļ�·��(out)
	�� �� ֵ:		0		�ɹ�
					1		��������
					2		���ܴ���jason�ļ�
					3		�ڴ����ʧ��
					4		�����ļ���������

    */
    public static native int converToLogJason(String pLogFile,String pJasonFile);
    
}
