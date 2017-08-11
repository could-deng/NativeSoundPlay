package com.fitmix.sdk;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;
import android.widget.Toast;

import com.example.nativesoundplay.R;
import com.fitmix.sdk.common.OSLESMediaPlayer;
import com.fitmix.sdk.common.WatchTransMethods;

public class MainActivity extends Activity {

	static AssetManager assetManager;
	boolean isplay=false;//URI player
	boolean isplay_asset=false; //AssetAudio Player;
	SeekBar seekBarRate;
//	String path = Environment.getExternalStorageDirectory()+File.separator+"fitmix"+File.separator+"voice"+File.separator+"150bmp.mp3";
	String path = Environment.getExternalStorageDirectory()+ File.separator+"fitmix"+File.separator+"Music"+File.separator+"185.mp3.down";

	
	/** Load jni .so on initialization */
    static {
         System.loadLibrary("watch-data-transfer");
    }
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        
        TextView tt_test = (TextView)(findViewById(R.id.tt_test));
        tt_test.setText(WatchTransMethods.square(2)+"");
        		
        
        
        
        
        
        
        
//        assetManager = getAssets();
//        try {
//			copyBigDataToSD(path);
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//		}
        
        OSLESMediaPlayer.createEngine();
        OSLESMediaPlayer.createBufferQueueAudioPlayer();
        
        // initialize native audio system
        Button start  =(Button)(findViewById(R.id.startplay));
        start.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
//		        isplay = OSLESMediaPlayer.createAudioPlayer(path);
//				OSLESMediaPlayer.play();
				
//				OSLESMediaPlayer.pause();
//				OSLESMediaPlayer.releaseEngine();
//		    	OSLESMediaPlayer.releaseAudioPlayer();
				File file = new File(Environment.getExternalStorageDirectory()+ File.separator+"fitmix"+File.separator+"Music"+File.separator+"150bmp.mp3");
				if(!file.exists()){
					Toast.makeText(MainActivity.this, "!file.exist()", Toast.LENGTH_SHORT).show();
					return;
				}
		    	OSLESMediaPlayer.createAudioPlayer(Environment.getExternalStorageDirectory()+ File.separator+"fitmix"+File.separator+"Music"+File.separator+"150bmp.mp3");
		    	OSLESMediaPlayer.play();
			}
		});
        
       Button startplay_resources = (Button)findViewById(R.id.startplay_resources);
        startplay_resources.setOnClickListener(new OnClickListener() {
        	boolean created = false;
			@Override
			public void onClick(View arg0) {
//				if (!created) {
//              	created = OSLESMediaPlayer.createAssetAudioPlayer(assetManager, "150bpm.mp3");
//              	isplay_asset = created;
//              	Toast.makeText(getApplicationContext(), created?"1":"0", Toast.LENGTH_SHORT).show();
//              }
//              if (created) {
//            	  OSLESMediaPlayer.play();
//              }
				
//				isplay = OSLESMediaPlayer.createAudioPlayer(Environment.getExternalStorageDirectory()+File.separator+"fitmix"+File.separator+"voice"+File.separator+"150bmp.mp3");
				File file = new File(Environment.getExternalStorageDirectory()+ File.separator+"150bmp.mp3");
				if(!file.exists()){
					Toast.makeText(MainActivity.this, "!file.exist(),"+file.getAbsolutePath(), Toast.LENGTH_SHORT).show();
					Log.i("TT", file.getAbsolutePath()+","+file.getPath());
					return;
				}
				isplay = OSLESMediaPlayer.createAudioPlayer(Environment.getExternalStorageDirectory()+File.separator+"150bmp.mp3");
				OSLESMediaPlayer.play();
			}
		});
        
        
        
        Button getRate  =(Button)(findViewById(R.id.getRate));
        getRate.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				Toast.makeText(getApplicationContext(), OSLESMediaPlayer.getRate()+"", Toast.LENGTH_SHORT).show();
			}
		});
        Button pause  = (Button)(findViewById(R.id.pause));
        pause.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				OSLESMediaPlayer.pause();
			}
		});
        
        Button setBPM240  = (Button)(findViewById(R.id.setBPM240));
        setBPM240.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				OSLESMediaPlayer.setRate(240*1000/150);
			}
		});
        
        seekBarRate = (SeekBar)findViewById(R.id.seekBarRate);
//        seekBarRate.setMax(1500);
        seekBarRate.setMax(240*1000/150);
        seekBarRate.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar arg0) {
				Toast.makeText(getApplicationContext(), 80*1000/150+arg0.getProgress()+"", Toast.LENGTH_SHORT).show();
//				OSLESMediaPlayer.setRate(80*1000/150+arg0.getProgress());
				OSLESMediaPlayer.setRate(80*1000/150+arg0.getProgress());
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar arg0) {
							
			}
			
			@Override
			public void onProgressChanged(SeekBar arg0, int arg1, boolean arg2) {
				
			}
		});
        ListView bpm_lv = (ListView)findViewById(R.id.bpm_lv);
        final String[] strs = new String[] {
        	    "80", "90", "100",
        	    "110", "120", "130",
        	    "140", "150", "160",
        	    "170", "180", "190",
        	    "200", "210", "220",
        	    "230", "240"
        	    };
        bpm_lv.setAdapter(new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1, strs));
        bpm_lv.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
					long arg3) {
				OSLESMediaPlayer.setRate((80+arg2*10)*1000/150);
				Toast.makeText(getApplicationContext(), (80+arg2*10)*1000/150+"", Toast.LENGTH_SHORT).show();
			}
		});
//        OSLESMediaPlayer.createEngine();
    }
    

    @Override
    protected void onPause() {
    	// TODO Auto-generated method stub
    	super.onPause();
    	OSLESMediaPlayer.pause();
    }
    @Override
    protected void onDestroy() {
    	// TODO Auto-generated method stub
    	super.onDestroy();
    	OSLESMediaPlayer.releaseEngine();
    	OSLESMediaPlayer.releaseAudioPlayer();
    }
    private void copyBigDataToSD(String strOutFileName) throws IOException 
    {  
    	File file = new File(strOutFileName);
    	if(!file.exists()){
	        InputStream myInput;  
	        OutputStream myOutput = new FileOutputStream(file);  
	        myInput = this.getAssets().open("150bpm.mp3");  
	        byte[] buffer = new byte[1024];  
	        int length = myInput.read(buffer);
	        while(length > 0)
	        {
	            myOutput.write(buffer, 0, length); 
	            length = myInput.read(buffer);
	        }
	        
	        myOutput.flush();  
	        myInput.close();  
	        myOutput.close();
    	}
    }
    
}
