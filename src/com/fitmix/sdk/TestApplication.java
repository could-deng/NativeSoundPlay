package com.fitmix.sdk;

import android.app.Application;
import android.os.Environment;

import java.io.File;

/**
 * Created by dengyuanqiang on 2017/8/5.
 */

public class TestApplication extends Application {
    String appPath = Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator
            +"TransferWord"+ File.separator;
    @Override
    public void onCreate() {
        super.onCreate();
        File file = new File(appPath);
        file.mkdirs();
    }
}
