package com.eegeo;

import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.app.Activity;
import android.content.res.AssetManager;

public class MainActivity extends Activity implements SurfaceHolder.Callback
{
    private EegeoSurfaceView m_surfaceView;
    private SurfaceHolder m_surfaceHolder;
    private long m_nativeAppWindowPtr;
	private View m_view;
	
    public static native long startNativeCode(MainActivity activity, AssetManager assetManager, float dpi);
    public static native void stopNativeCode();
    public static native void pauseNativeCode();
    public static native void resumeNativeCode();
    public static native void setNativeSurface(Surface surface);
    public static native void startGame(int gameId);
    
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        System.loadLibrary("native-activity");
        
        setContentView(R.layout.activity_main);  

        m_surfaceView = (EegeoSurfaceView)findViewById(R.id.surface);
        m_surfaceView.getHolder().addCallback(this);
        
        createUI();
    }
    
    private void createUI()
    {
        try
        {
        	final RelativeLayout uiRoot = (RelativeLayout) findViewById(R.id.ui_container);
        	m_view = getLayoutInflater().inflate(R.layout.change_game_ui, uiRoot, false);
        	
        	final Button scavengerHuntButton = (Button)m_view.findViewById(R.id.scavenger_hunt_button);
        	final Button helicopterButton = (Button)m_view.findViewById(R.id.helicopter_button);
        	
        	scavengerHuntButton.setOnClickListener
        	(
    			new OnClickListener() 
	        	{
	                @Override
	                public void onClick(View v) 
	                {
	                	startGame(1);
	                }
	            }
        	);
        	
        	helicopterButton.setOnClickListener
        	(
        		new OnClickListener() 
        		{
	                @Override
	                public void onClick(View v) 
	                {
	                	startGame(2);
	                }
        		}
        	);
        	
        	uiRoot.addView(m_view);
        }
        catch (Exception e)
        {
            Log.v("ChangeGameUI", e.getMessage() == null ? "Error, but no message?!" : e.getMessage());
        }                            
    }
        
    @Override
    protected void onStart() 
    {
        super.onStart();
        
        DisplayMetrics dm = getResources().getDisplayMetrics();
		float dpi = dm.ydpi;
        
        m_nativeAppWindowPtr = startNativeCode(this, getAssets(), dpi);
        
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
        
        if(m_surfaceHolder != null)
        {
        	setNativeSurface(m_surfaceHolder.getSurface());
        }
    }
    
    @Override
    protected void onPause() 
    {
        pauseNativeCode();
        super.onPause();
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
		m_surfaceHolder = null;
		setNativeSurface(null);
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) 
	{
		m_surfaceHolder = holder;
		setNativeSurface(m_surfaceHolder.getSurface());
	}
}