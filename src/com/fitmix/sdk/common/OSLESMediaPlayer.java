package com.fitmix.sdk.common;

import android.content.res.AssetManager;

public class OSLESMediaPlayer {
	public native static void createEngine();
	 public native static void releaseEngine();
	 public native static boolean createAudioPlayer(String uri);
	 public native static void releaseAudioPlayer();
	 public native static void play();
	 public native void stop();
	 public native static void pause();
	 public native boolean isPlaying();
	 public native static void seekTo(int position);
	 public native int getDuration();
	 public native int getPosition();
	 public native void setPitch(int rate);
	 public native static void setRate(int rate);
	 public native static int getRate();
	 public native void setLoop( int startPos, int endPos );
	 public native void setNoLoop();

	 //add play resource
	 public static native void createBufferQueueAudioPlayer();
	 public static native boolean createAssetAudioPlayer(AssetManager assetManager, String filename);
	 //
	 
	 public interface OnCompletionListener {
	  public void OnCompletion();
	 }
	 private OnCompletionListener mCompletionListener;
	 public void SetOnCompletionListener( OnCompletionListener listener )
	 {
	  mCompletionListener = listener;
	 }

	 private void OnCompletion()
	 {
	  mCompletionListener.OnCompletion();
	  int position = getPosition();
	  int duration = getDuration();
	  if( position != duration )
	  {
	   int a = 0;
	  }
	  else
	  {
	   int c = 0;
	  }
	 }
	}
