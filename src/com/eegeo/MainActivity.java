package com.eegeo;

import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.Spinner;
import android.app.Activity;
import android.content.res.AssetManager;

public class MainActivity extends Activity implements SurfaceHolder.Callback
{
    private EegeoSurfaceView m_surfaceView;
    private long m_nativeAppWindowPtr;
    
    //lifecycle
    public static native long startNativeCode(MainActivity activity, AssetManager assetManager);
    public static native void stopNativeCode();
    public static native void pauseNativeCode();
    public static native void resumeNativeCode();
    public static native void setNativeSurface(Surface surface);
    
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        System.loadLibrary("native-activity");
        
        setContentView(R.layout.activity_main);  

        m_surfaceView = (EegeoSurfaceView)findViewById(R.id.surface);
        m_surfaceView.getHolder().addCallback(this);
    }
    
    @Override
    protected void onStart() 
    {
        super.onStart();
        m_nativeAppWindowPtr = startNativeCode(this, getAssets());
        
        if(m_nativeAppWindowPtr == 0)
        {
        	throw new RuntimeException("Failed to start native code.");
        }
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
        m_nativeAppWindowPtr = 0;
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