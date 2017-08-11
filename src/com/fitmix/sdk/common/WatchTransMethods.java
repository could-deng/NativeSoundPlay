package com.fitmix.sdk.common;

public class WatchTransMethods {
	
	public static native int square(int num);
	
	/*
	功能说明:将IronCloud的GPS数据和Distance数据融合成jason文件
	参数说明:pGPSFile		GPS数据文件路径(in)
			 pDistanceFile	Distance数据文件路径(in)
			 pJasonFile		Json数据文件路径(out)
	返 回 值:		0		成功
					1		参数错误
					2		不能创建jason文件
					3		内存分配失败
					4		数据文件解析错误

	 */
    public static native int converToGPSJason(String pGPSFile,String pDistanceFile,String pJasonFile);
    /*
	功能说明:将IronCloud的Step数据和Distance数据,HR数据融合成jason文件
	参数说明:pStepFile		Step数据文件路径(in)
			 pDistanceFile	Distance数据文件路径(in)
			 pHRFile		HR数据文件路径(in)
			 pJasonFile		Json数据文件路径(out)
	返 回 值:		0		成功
					1		参数错误
					2		不能创建jason文件
					3		内存分配失败
					4		数据文件解析错误

    */
    public static native int converToStepJason(String pStepFile, String pDistanceFile, String pHRFile, String pJasonFile);

    /*
	功能说明:将Log数据融合成jason文件
	参数说明:pLogFile		Log数据文件路径(in)
			 pJasonFile		Json数据文件路径(out)
	返 回 值:		0		成功
					1		参数错误
					2		不能创建jason文件
					3		内存分配失败
					4		数据文件解析错误

    */
    public static native int converToLogJason(String pLogFile,String pJasonFile);
    
}
