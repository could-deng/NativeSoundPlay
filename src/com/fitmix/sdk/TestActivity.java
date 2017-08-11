package com.fitmix.sdk;

import java.io.File;
import java.io.IOException;

import com.example.nativesoundplay.R;
import com.fitmix.sdk.common.WatchTransMethods;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class TestActivity extends Activity {
	/** Load jni .so on initialization */
    static {
         System.loadLibrary("watch-data-transfer");
    }
    String appPath = Environment.getExternalStorageDirectory().getAbsolutePath()+ File.separator
            +"TransferWord"+ File.separator;
    String compassPath = appPath+"Sensor"+File.separator+"SENSOR_COMPASS.txt";
    String distancePath = appPath+"Sensor"+File.separator+"SENSOR_DISTANCE.txt";
    String GPSPath = appPath+"Sensor"+File.separator+"SENSOR_GPS.txt";//File.separator;
    String gsensorPath = appPath+"Sensor"+File.separator+"SENSOR_GSENSOR.txt";
    String heartratePath = appPath+"Sensor"+File.separator+"SENSOR_HEARTRATE.txt";
    String sportLogPath = appPath +"Sensor"+ File.separator + "SPORT_LOG_FILE.txt";
    
    
    
    TextView tt;
    String gpsJson = appPath + "Sensor" +File.separator + "gpsSensor.txt";// +File.separator;
    String stepJson = appPath + "Sensor" +File.separator + "stepSensor.txt";// +File.separator;
    String logJson = appPath + "Sensor" + File.separator + "sportLog.txt";
   
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activitiy_test);
        tt = (TextView) findViewById(R.id.tt_test);
        tt.setText(WatchTransMethods.square(2) + "");
//        tt.setText(WatchTransMethods.converToGPSJason("","","")+"");
    }
    public void onclick(View view){
        switch (view.getId()){
            case R.id.bt_1:
                try {
//                    File gpsJsonFile = new File(gpsJson);
//                    if(!gpsJsonFile.exists()) {
//                            gpsJsonFile.createNewFile();
//                    }
//                    Log.e("ttttt",gpsJson);
//                    int result = WatchTransMethods.converToGPSJason(gpsJson,"0",gpsJsonFile.getAbsolutePath());
                    int result = WatchTransMethods.converToGPSJason(GPSPath,distancePath,gpsJson);
                    Log.e("TT", "result1 == "+result);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                break;
            case R.id.bt_2:
            	try {
//            		File stepJsonFile = new File(stepJson);
//                    if(!stepJsonFile.exists()) {
//                    	stepJsonFile.createNewFile();
//                    }
                    int result = WatchTransMethods.converToStepJason(gsensorPath,distancePath,heartratePath,stepJson);
                    Log.e("TT", "result2 == "+result);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            	
                break;
            case R.id.bt_3:
            	try {
//            		File logJsonFile = new File(logJson);
//                    if(!logJsonFile.exists()) {
//                    	logJsonFile.createNewFile();
//                    }
                    int result = WatchTransMethods.converToLogJason(sportLogPath,logJson);
                    Log.e("TT", "result3 == "+result);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            	
            	break;
        }
    }

}
