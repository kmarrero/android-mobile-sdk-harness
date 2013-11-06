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
    
    //application
    public static native void visitLocation(long nativeAppWindowPtr, String location);
    
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        System.loadLibrary("native-activity");
        
        setContentView(R.layout.activity_main);  

        m_surfaceView = (EegeoSurfaceView)findViewById(R.id.surface);
        m_surfaceView.getHolder().addCallback(this);
    }
    
    public void showVisitMenu()
    {
    	final MainActivity self = this;
    	
    	runOnUiThread(new Runnable()
	    {
	        public void run()
	        {
	            try
	            {
	            	final RelativeLayout uiRoot = (RelativeLayout)findViewById(R.id.ui_container);
	            	final View view = getLayoutInflater().inflate(R.layout.ui_menu, uiRoot, false);
	            	
	            	final Spinner spinner = (Spinner)view.findViewById(R.id.places);
	            
	            	String items[] = new String[5];
	            	items[0] = "NYC";
	            	items[1] = "London";
	            	items[2] = "SF";
	            	items[3] = "Kyoto";
	            	items[4] = "Melbourne";
	                ArrayAdapter<String> adapter = new ArrayAdapter<String>(self, android.R.layout.simple_spinner_item, items);
	            	spinner.setAdapter(adapter);
	            	
	            	final Button close = (Button)view.findViewById(R.id.close);
	            	final Button visit = (Button)view.findViewById(R.id.visit);
	            	
	            	close.setOnClickListener(new OnClickListener() {
	                    @Override
	                    public void onClick(View v) {
	                    	close.setOnClickListener(null);
	                    	visit.setOnClickListener(null);
	                    	uiRoot.removeView(view);
	                    }
	                });
	            	
	            	visit.setOnClickListener(new OnClickListener() {
	                    @Override
	                    public void onClick(View v) {
	                    	String selection = (String)spinner.getSelectedItem();
	                    	visitLocation(m_nativeAppWindowPtr, selection);
	                    }
	                });
	            	
	            	uiRoot.addView(view);
	            }
	            catch (Exception e)
	            {
	                //Log.v("InputBox", e.getMessage() == null ? "Error, but no message?!" : e.getMessage());
	            }                            
	        }
	    });
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