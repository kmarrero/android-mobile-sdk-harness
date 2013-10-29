package com.eegeo;

import android.os.Bundle;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.app.Activity;
import android.content.res.AssetManager;

public class MainActivity extends Activity implements SurfaceHolder.Callback
{
    private EegeoSurfaceView m_surfaceView;
    private AssetManager m_assetManager;
    private MainActivity m_self;
    
    //lifecycle
    public static native void startNativeCode(MainActivity activity, AssetManager assetManager);
    public static native void stopNativeCode();
    public static native void pauseNativeCode();
    public static native void resumeNativeCode();
    public static native void setNativeSurface(Surface surface);

    //touch input
    public static native void processNativePointerDown(int primaryActionIndex, int primaryActionIdentifier, int pointerCount, float[] x, float y[], int[] pointerIdentity, int[] pointerIndex);
    public static native void processNativePointerUp(int primaryActionIndex, int primaryActionIdentifier, int pointerCount, float[] x, float y[], int[] pointerIdentity, int[] pointerIndex);
    public static native void processNativePointerMove(int primaryActionIndex, int primaryActionIdentifier, int pointerCount, float[] x, float y[], int[] pointerIdentity, int[] pointerIndex);

    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        System.loadLibrary("native-activity");
        
        setContentView(R.layout.activity_main);  

        m_surfaceView = (EegeoSurfaceView)findViewById(R.id.surface);
        m_surfaceView.setMainActivity(this);
        m_surfaceView.getHolder().addCallback(this);

        //store an object reference to keep lifetime bound to activity so reference passed via jni is not collected
        m_assetManager = this.getAssets();
        m_self = this;
    }
    
    void processTouchEvent(MotionEvent e)
    {
        //we need to convert multi-touch event handling into struct of arrays for many pointers to send over JNI
    	
    	//C++ event representation is like;
    	/*
			float x, y;
			int pointerIdentity;
			int pointerIndex;
    	 */
    	   
    	int pointerCount = e.getPointerCount();
    	int primaryActionIndex = e.getActionIndex();
    	int primaryActionIdentifier = e.getPointerId(primaryActionIndex);
    	
    	float[] xArray = new float[pointerCount];
    	float[] yArray = new float[pointerCount];
    	int[] pointerIdentityArray = new int[pointerCount];
    	int[] pointerIndexArray = new int[pointerCount];
    	
    	for(int pointerIndex = 0; pointerIndex < pointerCount; ++pointerIndex)
    	{
            xArray[pointerIndex] = e.getX(pointerIndex);
            yArray[pointerIndex] = e.getY(pointerIndex);
            pointerIdentityArray[pointerIndex] = e.getPointerId(pointerIndex);
            pointerIndexArray[pointerIndex] = pointerIndex;
        }
    	
	    switch (e.getActionMasked()) 
	    {
	        case MotionEvent.ACTION_DOWN:
	        case MotionEvent.ACTION_POINTER_DOWN:
	        {	
	        	processNativePointerDown(primaryActionIndex, primaryActionIdentifier, pointerCount, xArray, yArray, pointerIdentityArray, pointerIndexArray);
	        }break;

	        case MotionEvent.ACTION_POINTER_UP:
	        case MotionEvent.ACTION_UP:
	        {      
	        	processNativePointerUp(primaryActionIndex, primaryActionIdentifier, pointerCount, xArray, yArray, pointerIdentityArray, pointerIndexArray);
	        }  break;
	        
	        case MotionEvent.ACTION_MOVE:
	        {	 
	        	processNativePointerMove(primaryActionIndex, primaryActionIdentifier, pointerCount, xArray, yArray, pointerIdentityArray, pointerIndexArray);
	        }break;
	    }
    }
    
    @Override
    protected void onStart() 
    {
        super.onStart();
    	startNativeCode(m_self, m_assetManager);
    }

    @Override
    protected void onResume() 
    {
        super.onResume();
        resumeNativeCode();
    }
    
    @Override
    protected void onPause() 
    {
        super.onPause();
        pauseNativeCode();
    }

    @Override
    protected void onStop() 
    {
        super.onStop();
        stopNativeCode();
    }

	@Override
	public void surfaceCreated(SurfaceHolder holder) 
	{
		//nothing to do
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) 
	{
		setNativeSurface(null);
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) 
	{
		setNativeSurface(holder.getSurface());
	}
}