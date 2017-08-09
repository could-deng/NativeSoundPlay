#include <jni.h>
#include<android/log.h>
// LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "OSLESMediaPlayer", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , "OSLESMediaPlayer", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , "OSLESMediaPlayer", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , "OSLESMediaPlayer", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , "OSLESMediaPlayer", __VA_ARGS__)
// for native audio
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <assert.h>
#include <sys/types.h>

// for native asset manager
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <string.h>



// engine interfaces
static SLObjectItf engineObject = NULL;
static SLEngineItf engineEngine;
// URI player interfaces
static SLObjectItf uriPlayerObject = NULL;
static SLPlayItf uriPlayerPlay;
static SLSeekItf uriPlayerSeek;
static SLPlaybackRateItf uriPlaybackRate;
// output mix interfaces
static SLObjectItf outputMixObject = NULL;
// playback rate (default 1x:1000)
static SLpermille playbackMinRate = 500;
static SLpermille playbackMaxRate = 2000;
static SLpermille playbackRateStepSize;
//Pitch
static SLPitchItf uriPlaybackPitch;
static SLpermille playbackMinPitch = 500;
static SLpermille playbackMaxPitch = 2000;



// buffer queue player interfaces
static SLObjectItf bqPlayerObject = NULL;
static SLPlayItf bqPlayerPlay;
static SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
static SLEffectSendItf bqPlayerEffectSend;
static SLMuteSoloItf bqPlayerMuteSolo;
static SLVolumeItf bqPlayerVolume;


//add (ratePlayback buffer queue player interfaces)
static SLPlaybackRateItf bqPlaybackRate;


// file descriptor player interfaces
static SLObjectItf fdPlayerObject = NULL;
static SLPlayItf fdPlayerPlay;
static SLSeekItf fdPlayerSeek;
static SLMuteSoloItf fdPlayerMuteSolo;
static SLVolumeItf fdPlayerVolume;


// pointer and size of the next player buffer to enqueue, and number of remaining buffers
static short *nextBuffer;
static unsigned nextSize;
static int nextCount;




// this callback handler is called every time a buffer finishes playing
void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
    assert(bq == bqPlayerBufferQueue);
    assert(NULL == context);
    // for streaming playback, replace this test by logic to find and fill the next buffer
    if (--nextCount > 0 && NULL != nextBuffer && 0 != nextSize) {
        SLresult result;
        // enqueue another buffer
        result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, nextBuffer, nextSize);
        // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
        // which for this code example would indicate a programming error
        assert(SL_RESULT_SUCCESS == result);
        (void)result;
    }
}




// create the engine and output mix objects
JNIEXPORT void Java_com_fitmix_sdk_common_OSLESMediaPlayer_createEngine( //´´½¨²¥·ÅÒýÇæ
		JNIEnv* env, jclass clazz) {
	SLresult result;
	// create engine
	LOGD("create engine");
	result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
	assert(SL_RESULT_SUCCESS == result);
	// realize the engine
	LOGD("realize the engine");
	result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
	assert(SL_RESULT_SUCCESS == result);
	// get the engine interface, which is needed in order to create other objects
	LOGD("get the engine interface");
	result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE,
			&engineEngine);
	assert(SL_RESULT_SUCCESS == result);
	// create output mix, with environmental reverb specified as a non-required interface
	LOGD("create output mix");
	const SLInterfaceID ids[1] = { SL_IID_PLAYBACKRATE };
	const SLboolean req[1] = { SL_BOOLEAN_FALSE };
	result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1,
			ids, req);
	assert(SL_RESULT_SUCCESS == result);
	// realize the output mix
	LOGD("realize the output mix");
	result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
	assert(SL_RESULT_SUCCESS == result);
}
JNIEXPORT void Java_com_fitmix_sdk_common_OSLESMediaPlayer_releaseEngine( //ÊÍ·ÅÒýÇæ
		JNIEnv* env, jclass clazz) {
	// destroy URI audio player object, and invalidate all associated interfaces
	if (uriPlayerObject != NULL) {
		(*uriPlayerObject)->Destroy(uriPlayerObject);
		uriPlayerObject = NULL;
		uriPlayerPlay = NULL;
		uriPlayerSeek = NULL;
	}

	// destroy buffer queue audio player object, and invalidate all associated interfaces
	if (bqPlayerObject != NULL) {
		(*bqPlayerObject)->Destroy(bqPlayerObject);
		bqPlayerObject = NULL;
		bqPlayerPlay = NULL;
		bqPlayerBufferQueue = NULL;
		bqPlayerEffectSend = NULL;
		bqPlayerMuteSolo = NULL;
		bqPlayerVolume = NULL;
		//
		bqPlaybackRate = NULL;
	}

	// destroy file descriptor audio player object, and invalidate all associated interfaces
	if (fdPlayerObject != NULL) {
		(*fdPlayerObject)->Destroy(fdPlayerObject);
		fdPlayerObject = NULL;
		fdPlayerPlay = NULL;
		fdPlayerSeek = NULL;
		fdPlayerMuteSolo = NULL;
		fdPlayerVolume = NULL;
		//

	}

	// destroy output mix object, and invalidate all associated interfaces
	if (outputMixObject != NULL) {
		(*outputMixObject)->Destroy(outputMixObject);
		outputMixObject = NULL;
	}
	// destroy engine object, and invalidate all associated interfaces
	if (engineObject != NULL) {
		(*engineObject)->Destroy(engineObject);
		engineObject = NULL;
		engineEngine = NULL;
	}
}
/*
 void OnCompletion(JNIEnv* env, jclass clazz)
 {
 jclass cls = env->GetObjectClass(thiz);
 if (cls != NULL)
 {
 jmethodID mid = env->GetMethodID(cls, "OnCompletion", "()V");
 if (mid != NULL)
 {
 env->CallVoidMethod(thiz, mid, 1234);
 }
 }
 }*/
void playStatusCallback(SLPlayItf play, void* context, SLuint32 event) {
	//LOGD("playStatusCallback");
}
// create URI audio player
JNIEXPORT jboolean Java_com_fitmix_sdk_common_OSLESMediaPlayer_createAudioPlayer(
		JNIEnv* env, jclass clazz, jstring uri) {
	SLresult result;
	// convert Java string to UTF-8
	const jbyte *utf8 = (*env)->GetStringUTFChars(env, uri, NULL);
	assert(NULL != utf8);
	// configure audio source
	// (requires the INTERNET permission depending on the uri parameter)
	SLDataLocator_URI loc_uri = { SL_DATALOCATOR_URI, (SLchar *) utf8 };
	SLDataFormat_MIME format_mime = { SL_DATAFORMAT_MIME, NULL,
			SL_CONTAINERTYPE_UNSPECIFIED };
	SLDataSource audioSrc = { &loc_uri, &format_mime };
	// configure audio sink
	SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX,
			outputMixObject };
	SLDataSink audioSnk = { &loc_outmix, NULL };
	// create audio player
	const SLInterfaceID ids[2] = { SL_IID_SEEK, SL_IID_PLAYBACKRATE };
	const SLboolean req[2] = { SL_BOOLEAN_FALSE, SL_BOOLEAN_TRUE };
	result = (*engineEngine)->CreateAudioPlayer(engineEngine, &uriPlayerObject,
			&audioSrc, &audioSnk, 2, ids, req);
	// note that an invalid URI is not detected here, but during prepare/prefetch on Android,
	// or possibly during Realize on other platforms
	assert(SL_RESULT_SUCCESS == result);
	// release the Java string and UTF-8
	(*env)->ReleaseStringUTFChars(env, uri, utf8);
	// realize the player
	result = (*uriPlayerObject)->Realize(uriPlayerObject, SL_BOOLEAN_FALSE);
	// this will always succeed on Android, but we check result for portability to other platforms
	if (SL_RESULT_SUCCESS != result) {
		(*uriPlayerObject)->Destroy(uriPlayerObject);
		uriPlayerObject = NULL;
		return JNI_FALSE;
	}
	// get the play interface
	result = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_PLAY,
			&uriPlayerPlay);
	assert(SL_RESULT_SUCCESS == result);
	// get the seek interface
	result = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_SEEK,
			&uriPlayerSeek);
	assert(SL_RESULT_SUCCESS == result);
	// get playback rate interface
	result = (*uriPlayerObject)->GetInterface(uriPlayerObject,
			SL_IID_PLAYBACKRATE, &uriPlaybackRate);
	assert(SL_RESULT_SUCCESS == result);
	/* // get playback pitch interface
	 result = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_PITCH, &uriPlaybackPitch);
	 assert(SL_RESULT_SUCCESS == result);*/
	// register callback function
	result = (*uriPlayerPlay)->RegisterCallback(uriPlayerPlay,
			playStatusCallback, 0);
	assert(SL_RESULT_SUCCESS == result);
	result = (*uriPlayerPlay)->SetCallbackEventsMask(uriPlayerPlay,
			SL_PLAYEVENT_HEADATEND); // head at end
	assert(SL_RESULT_SUCCESS == result);
	SLmillisecond msec;
	result = (*uriPlayerPlay)->GetDuration(uriPlayerPlay, &msec);
	assert(SL_RESULT_SUCCESS == result);
	// no loop
	result = (*uriPlayerSeek)->SetLoop(uriPlayerSeek, SL_BOOLEAN_TRUE, 0, msec);
	assert(SL_RESULT_SUCCESS == result);

	SLuint32 capa;
	result = (*uriPlaybackRate)->GetRateRange(uriPlaybackRate, 0,
			&playbackMinRate, &playbackMaxRate, &playbackRateStepSize, &capa);
	assert(SL_RESULT_SUCCESS == result);
	result = (*uriPlaybackRate)->SetPropertyConstraints(uriPlaybackRate,
			SL_RATEPROP_PITCHCORAUDIO);
	if (SL_RESULT_PARAMETER_INVALID == result) {
		LOGD("Parameter Invalid");
	}
	if (SL_RESULT_FEATURE_UNSUPPORTED == result) {
		LOGD("Feature Unsupported");
	}
	if (SL_RESULT_SUCCESS == result) {
		assert(SL_RESULT_SUCCESS == result);
		LOGD("Success");
	}
	/*
	 result = (*uriPlaybackPitch)->GetPitchCapabilities(uriPlaybackPitch, &playbackMinPitch, &playbackMaxPitch);
	 assert(SL_RESULT_SUCCESS == result);*/
	/*
	 SLpermille minRate, maxRate, stepSize, rate = 1000;
	 SLuint32 capa;
	 (*uriPlaybackRate)->GetRateRange(uriPlaybackRate, 0, &minRate, &maxRate, &stepSize, &capa);
	 (*uriPlaybackRate)->SetRate(uriPlaybackRate, minRate);
	 */
	return JNI_TRUE;
}
JNIEXPORT void Java_com_fitmix_sdk_common_OSLESMediaPlayer_releaseAudioPlayer(
		JNIEnv* env, jclass clazz) {
	// destroy URI audio player object, and invalidate all associated interfaces
	if (uriPlayerObject != NULL) {
		(*uriPlayerObject)->Destroy(uriPlayerObject);
		uriPlayerObject = NULL;
		uriPlayerPlay = NULL;
		uriPlayerSeek = NULL;
		uriPlaybackRate = NULL;
	}
	 // destroy buffer queue audio player object, and invalidate all associated interfaces
	    if (bqPlayerObject != NULL) {
	        (*bqPlayerObject)->Destroy(bqPlayerObject);
	        bqPlayerObject = NULL;
	        bqPlayerPlay = NULL;
	        bqPlayerBufferQueue = NULL;
	        bqPlayerEffectSend = NULL;
	        bqPlayerMuteSolo = NULL;
	        bqPlayerVolume = NULL;
	        //
	        bqPlaybackRate = NULL;
	    }

	    // destroy file descriptor audio player object, and invalidate all associated interfaces
	    if (fdPlayerObject != NULL) {
	        (*fdPlayerObject)->Destroy(fdPlayerObject);
	        fdPlayerObject = NULL;
	        fdPlayerPlay = NULL;
	        fdPlayerSeek = NULL;
	        fdPlayerMuteSolo = NULL;
	        fdPlayerVolume = NULL;
	        //

	    }

}






//add
// create buffer queue audio player
void Java_com_fitmix_sdk_common_OSLESMediaPlayer_createBufferQueueAudioPlayer(JNIEnv* env,
        jclass clazz)
{
	SLresult result;
	// configure audio source
	SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {
			SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2 };
	SLDataFormat_PCM format_pcm = { SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_8,
			SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
			SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN };
	SLDataSource audioSrc = { &loc_bufq, &format_pcm };

	// configure audio sink
	SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX,
			outputMixObject };
	SLDataSink audioSnk = { &loc_outmix, NULL };

	// create audio player
	const SLInterfaceID ids[3] = { SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND,
	/*SL_IID_MUTESOLO,*/SL_IID_VOLUME };
	const SLboolean req[3] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE,
	/*SL_BOOLEAN_TRUE,*/SL_BOOLEAN_TRUE };
	result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject,
			&audioSrc, &audioSnk, 3, ids, req);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// realize the player
	result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// get the play interface
	result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY,
			&bqPlayerPlay);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// get the buffer queue interface
	result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE,
			&bqPlayerBufferQueue);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// register callback on the buffer queue
	result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue,
			bqPlayerCallback, NULL);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// get the effect send interface
	result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_EFFECTSEND,
			&bqPlayerEffectSend);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

#if 0   // mute/solo is not supported for sources that are known to be mono, as this is
	// get the mute/solo interface
	result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_MUTESOLO, &bqPlayerMuteSolo);
	assert(SL_RESULT_SUCCESS == result);
	(void)result;
#endif

	// get the volume interface
	result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME,
			&bqPlayerVolume);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// set the player's state to playing
	result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

 /*
    // get playback rate interface
        	result = (*bqPlayerObject)->GetInterface(bqPlayerObject,
        			SL_IID_PLAYBACKRATE, &bqPlaybackRate);
        	assert(SL_RESULT_SUCCESS == result);
     	 // get playback pitch interface
        	result = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_PITCH, &uriPlaybackPitch);
        	 assert(SL_RESULT_SUCCESS == result);
        	// register callback function
        	result = (*bqPlayerPlay)->RegisterCallback(bqPlayerPlay,
        			playStatusCallback, 0);
        	assert(SL_RESULT_SUCCESS == result);
        	result = (*bqPlayerPlay)->SetCallbackEventsMask(bqPlayerPlay,
        			SL_PLAYEVENT_HEADATEND); // head at end
        	assert(SL_RESULT_SUCCESS == result);
        	SLmillisecond msec;
        	result = (*bqPlayerPlay)->GetDuration(bqPlayerPlay, &msec);
        	assert(SL_RESULT_SUCCESS == result);
        	// no loop
//        	result = (*uriPlayerSeek)->SetLoop(uriPlayerSeek, SL_BOOLEAN_TRUE, 0, msec);
//        	assert(SL_RESULT_SUCCESS == result);

         	SLuint32 capa;
        	result = (*bqPlaybackRate)->GetRateRange(bqPlaybackRate, 0,
        			&playbackMinRate, &playbackMaxRate, &playbackRateStepSize, &capa);
        	assert(SL_RESULT_SUCCESS == result);
        	result = (*bqPlaybackRate)->SetPropertyConstraints(bqPlaybackRate,
        			SL_RATEPROP_PITCHCORAUDIO);
        	if (SL_RESULT_PARAMETER_INVALID == result) {
        		LOGD("Parameter Invalid");
        	}
        	if (SL_RESULT_FEATURE_UNSUPPORTED == result) {
        		LOGD("Feature Unsupported");
        	}
        	if (SL_RESULT_SUCCESS == result) {
        		assert(SL_RESULT_SUCCESS == result);
        		LOGD("Success");
        	}

*/

}




// create asset audio player
jboolean Java_com_fitmix_sdk_common_OSLESMediaPlayer_createAssetAudioPlayer(JNIEnv* env, jclass clazz,
        jobject assetManager, jstring filename)
{
	LOGD("createAssetAudioPlayer");
	 SLresult result;

	// convert Java string to UTF-8
	const char *utf8 = (*env)->GetStringUTFChars(env, filename, NULL);
	assert(NULL != utf8);

	// use asset manager to open asset by filename
	AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
	assert(NULL != mgr);
	AAsset* asset = AAssetManager_open(mgr, utf8, AASSET_MODE_UNKNOWN);

	// release the Java string and UTF-8
	(*env)->ReleaseStringUTFChars(env, filename, utf8);

	// the asset might not be found
	if (NULL == asset) {
		return JNI_FALSE;
	}

	// open asset as file descriptor
	off_t start, length;
	int fd = AAsset_openFileDescriptor(asset, &start, &length);
	assert(0 <= fd);
	AAsset_close(asset);

	// configure audio source
	SLDataLocator_AndroidFD loc_fd = { SL_DATALOCATOR_ANDROIDFD, fd, start,
			length };
	SLDataFormat_MIME format_mime = { SL_DATAFORMAT_MIME, NULL,
			SL_CONTAINERTYPE_UNSPECIFIED };
	SLDataSource audioSrc = { &loc_fd, &format_mime };

	// configure audio sink
	SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX,
			outputMixObject };
	SLDataSink audioSnk = { &loc_outmix, NULL };

	// create audio player
	const SLInterfaceID ids[3] = { SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME };
	const SLboolean req[3] =
			{ SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
	result = (*engineEngine)->CreateAudioPlayer(engineEngine, &fdPlayerObject,
			&audioSrc, &audioSnk, 3, ids, req);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// realize the player
	result = (*fdPlayerObject)->Realize(fdPlayerObject, SL_BOOLEAN_FALSE);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// get the play interface
	result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_PLAY,
			&fdPlayerPlay);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// get the seek interface
	result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_SEEK,
			&fdPlayerSeek);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// get the mute/solo interface
	result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_MUTESOLO,
			&fdPlayerMuteSolo);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// get the volume interface
	result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_VOLUME,
			&fdPlayerVolume);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

	// enable whole file looping
	result = (*fdPlayerSeek)->SetLoop(fdPlayerSeek, SL_BOOLEAN_TRUE, 0,
			SL_TIME_UNKNOWN);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;


	// get playback rate interface
	result = (*fdPlayerObject)->GetInterface(fdPlayerObject,
			SL_IID_PLAYBACKRATE, &bqPlaybackRate);
	assert(SL_RESULT_SUCCESS == result);
	(void) result;

//
//	SLuint32 capa;
//	result = (*bqPlaybackRate)->GetRateRange(bqPlaybackRate, 0,
//			&playbackMinRate, &playbackMaxRate, &playbackRateStepSize, &capa);
//	assert(SL_RESULT_SUCCESS == result);
//	result = (*bqPlaybackRate)->SetPropertyConstraints(bqPlaybackRate,
//			SL_RATEPROP_PITCHCORAUDIO);
//	if (SL_RESULT_PARAMETER_INVALID == result) {
//		LOGD("Parameter Invalid");
//	}
//	if (SL_RESULT_FEATURE_UNSUPPORTED == result) {
//		LOGD("Feature Unsupported");
//	}
//	if (SL_RESULT_SUCCESS == result) {
//		assert(SL_RESULT_SUCCESS == result);
//		LOGD("Success");
//	}

	return JNI_TRUE;
}
























void setPlayState(SLuint32 state) {
	SLresult result;
	// make sure the URI audio player was created
	if (NULL != uriPlayerPlay) {
		// set the player's state
		result = (*uriPlayerPlay)->SetPlayState(uriPlayerPlay, state);
		assert(SL_RESULT_SUCCESS == result);
	}
	if (NULL != fdPlayerPlay) {
			// set the player's state
			result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, state);
			assert(SL_RESULT_SUCCESS == result);
		}
}
SLuint32 getPlayState() {
	SLresult result;
	// make sure the URI audio player was created
	if (NULL != uriPlayerPlay) {
		SLuint32 state;
		result = (*uriPlayerPlay)->GetPlayState(uriPlayerPlay, &state);
		assert(SL_RESULT_SUCCESS == result);
		return state;
	}
	if (NULL != fdPlayerPlay) {
			SLuint32 state;
			result = (*fdPlayerPlay)->GetPlayState(fdPlayerPlay, &state);
			assert(SL_RESULT_SUCCESS == result);
			return state;
		}
	return 0;
}
// play
JNIEXPORT void Java_com_fitmix_sdk_common_OSLESMediaPlayer_play(JNIEnv* env,
		jclass clazz) {
	setPlayState(SL_PLAYSTATE_PLAYING);
}
// stop
JNIEXPORT void Java_com_fitmix_sdk_common_OSLESMediaPlayer_stop(JNIEnv* env,
		jclass clazz) {
	setPlayState(SL_PLAYSTATE_STOPPED);
}
// pause
JNIEXPORT void Java_com_fitmix_sdk_common_OSLESMediaPlayer_pause(JNIEnv* env,
		jclass clazz) {
	setPlayState(SL_PLAYSTATE_PAUSED);
}
// pause
JNIEXPORT jboolean Java_com_fitmix_sdk_common_OSLESMediaPlayer_isPlaying(
		JNIEnv* env, jclass clazz) {
	return (getPlayState() == SL_PLAYSTATE_PLAYING);
}
// set position
JNIEXPORT void Java_com_fitmix_sdk_common_OSLESMediaPlayer_seekTo(
		JNIEnv* env, jclass clazz, jint position) {
	if (NULL != uriPlayerPlay) {
		//SLuint32 state = getPlayState();
		//setPlayState(SL_PLAYSTATE_PAUSED);
		SLresult result;
		result = (*uriPlayerSeek)->SetPosition(uriPlayerSeek, position,
				SL_SEEKMODE_FAST);
		assert(SL_RESULT_SUCCESS == result);
		//setPlayState(state);
	}
}
// get duration
JNIEXPORT jint Java_com_fitmix_sdk_common_OSLESMediaPlayer_getDuration(
		JNIEnv* env, jclass clazz) {
	if (NULL != uriPlayerPlay) {
		SLresult result;
		SLmillisecond msec;
		result = (*uriPlayerPlay)->GetDuration(uriPlayerPlay, &msec);
		assert(SL_RESULT_SUCCESS == result);
		return msec;
	}
	return 0.0f;
}
// get current position
JNIEXPORT jint Java_com_fitmix_sdk_common_OSLESMediaPlayer_getPosition(
		JNIEnv* env, jclass clazz) {
	if (NULL != uriPlayerPlay) {
		SLresult result;
		SLmillisecond msec;
		result = (*uriPlayerPlay)->GetPosition(uriPlayerPlay, &msec);
		assert(SL_RESULT_SUCCESS == result);
		return msec;
	}
	return 0.0f;
}
//llllllllllllllllllll
JNIEXPORT void Java_com_fitmix_sdk_common_OSLESMediaPlayer_setPitch(
		JNIEnv* env, jclass clazz, jint rate) {
	if (NULL != uriPlaybackPitch) {
		SLresult result;
		result = (*uriPlaybackPitch)->SetPitch(uriPlaybackPitch, rate);
		assert(SL_RESULT_SUCCESS == result);
	}
}

JNIEXPORT void Java_com_fitmix_sdk_common_OSLESMediaPlayer_setRate(
		JNIEnv* env, jclass clazz, jint rate) {
	if (NULL != uriPlaybackRate) {
		SLresult result;
		result = (*uriPlaybackRate)->SetRate(uriPlaybackRate, rate);
		assert(SL_RESULT_SUCCESS == result);
	}
	if (NULL != bqPlaybackRate) {
			SLresult result;
			result = (*bqPlaybackRate)->SetRate(bqPlaybackRate, rate);
			assert(SL_RESULT_SUCCESS == result);

	}
}

JNIEXPORT jint Java_com_fitmix_sdk_common_OSLESMediaPlayer_getRate(
		JNIEnv* env, jclass clazz) {
	if (NULL != uriPlaybackRate) {
		LOGD("getRate_____________NULL != uriPlaybackRate");
		SLresult result;
		SLpermille rate;
		result = (*uriPlaybackRate)->GetRate(uriPlaybackRate, &rate);
		assert(SL_RESULT_SUCCESS == result);
		return rate;
	}
	if (NULL != bqPlaybackRate) {
		LOGD("getRate_____________NULL != bqPlaybackRate");
		SLresult result;
		SLpermille rate;
		result = (*bqPlaybackRate)->GetRate(bqPlaybackRate, &rate);
		assert(SL_RESULT_SUCCESS == result);
		return rate;
	}
	return 0;
}
// create URI audio player
JNIEXPORT jboolean Java_com_fitmix_sdk_common_OSLESMediaPlayer_setLoop(
		JNIEnv* env, jclass clazz, jint startPos, jint endPos) {
	SLresult result;
	result = (*uriPlayerSeek)->SetLoop(uriPlayerSeek, SL_BOOLEAN_TRUE, startPos,
			endPos);
	assert(SL_RESULT_SUCCESS == result);
	return JNI_TRUE;
}
// create URI audio player
JNIEXPORT jboolean Java_com_fitmix_sdk_common_OSLESMediaPlayer_setNoLoop(
		JNIEnv* env, jclass clazz) {
	SLresult result;
	if (NULL != uriPlayerSeek) {
		// enable whole file looping
		result = (*uriPlayerSeek)->SetLoop(uriPlayerSeek, SL_BOOLEAN_TRUE, 0,
				SL_TIME_UNKNOWN);
		assert(SL_RESULT_SUCCESS == result);
	}
	return JNI_TRUE;
}
